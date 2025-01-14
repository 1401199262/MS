                                                         ZZZ                     ннн                                                                                                                                                                                                                                                                                                                                 ЃЃЃ                                 њњњ               ююю                                                                                                                                                                                                                                                                                                                                    ЃЃЃ                                    ╩╩╩         ddd                                                                                                                                                                                                                                                                                                                                       ЃЃЃ                                    ╩╩╩                                                                                                                                                                                                                                                                                                                                                   ЃЃЃ                                                                                                                                                                                                                                                                                                                                                                                          EEE   JJJ                                                   ,,,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          #include "precomp.hpp"
#include "SerializedJobsList.hpp"
#include "SerializedJobsList.tmh"

SerializedJobList::SerializedJobList() :
    m_ResetForPortPending(false),
    m_PortResetId(WDI_PORT_ID_ADAPTER),
    m_pResetForPortCompletionCallback(NULL),
    m_pEventQueue(NULL),
    m_pActiveJobsList(NULL),
    m_pActiveSerializedJob(NULL),
    m_AllowNewJobs(true)
{
    InitializeListHead(
        &m_JobList );
}


NDIS_STATUS SerializedJobList::Initialize(    
    _In_ EventQueue*     pEventQueue,
    _In_ ActiveJobsList* pActiveJobsList )
{
    NDIS_STATUS status = NDIS_STATUS_SUCCESS;
    TraceEntry();

    if ( pEventQueue == NULL ||
         pActiveJobsList == NULL )
    {
        status = NDIS_STATUS_INVALID_PARAMETER;
        goto exit;
    }

    m_pEventQueue = pEventQueue;
    m_pActiveJobsList = pActiveJobsList;

exit:
    TraceExit(status);
    return status;
}


NDIS_STATUS SerializedJobList::QueueSerializedJob(
    _In_ CJobBase*                     pSerializedJob,
    _In_ IOperationCompletionCallback* pCompletionCallback )
{
    NDIS_STATUS status = NDIS_STATUS_SUCCESS;
    Event* pQueueSerializedJobEvent = NULL;

    TraceEntry();

    if ( pSerializedJob == NULL )
    {
        status = NDIS_STATUS_INVALID_PARAMETER;
        goto exit;
    }

    if ( !m_AllowNewJobs )
    {
        WFCError("Cannot queue new serialized job, job queue is not accepting new jobs.");
        status = NDIS_STATUS_INVALID_STATE;
        goto exit;
    }

    status = pSerializedJob->set_SerializedJobCompletionCallback(
        pCompletionCallback );

    if ( status != NDIS_STATUS_SUCCESS )
    {
        goto exit;
    }

    status = pSerializedJob->get_FailsafeCompletionEvent(
        &pQueueSerializedJobEvent );

    if ( status != NDIS_STATUS_SUCCESS )
    {
        goto exit;
    }

    status = pQueueSerializedJobEvent->PopulateEvent(
        WiFiEventQueueSerializedJob,
        (IEventQueueCallback*)this,
        pSerializedJob );

    if ( status != NDIS_STATUS_SUCCESS )
    {
        goto exit;
    }

    status = m_pEventQueue->QueueEvent(
        pQueueSerializedJobEvent );

    if ( status != STATUS_SUCCESS )
    {
        goto exit;
    }

exit:

    TraceExit(status);
    return status;
}


void SerializedJobList::FlushQueuedJobs(
    _In_ NDIS_PORT_NUMBER NdisPortNumber
    )
{
    PLIST_ENTRY pCurrentEntry = m_JobList.Flink;
    PLIST_ENTRY pNextEntry = NULL; 
    TraceEntry();

    //
    // Flush must be called from event queue thread to avoid
    // synchronization race conditions.
    //
    NT_ASSERT(
        CDebugMetadata::get_CurrentThreadHandle() ==
        m_pEventQueue->get_ActiveThreadHandle() );

    if ( IsListEmpty(&m_JobList) )
    {
        goto exit;
    }

    while ( pCurrentEntry != &m_JobList )
    {
        WFC_JOB_TYPE jobType = WiFiJobTypeInvalid;
        CJobBase* pCurrentJob = CJobBase::get_JobFromListEntry(
            pCurrentEntry );
        bool bFlushJob = true;

        pNextEntry = pCurrentEntry->Flink;        

        // If a filter is provided, use it
        if (NdisPortNumber != INVALID_NDIS_PORT_NUMBER)
        {
            // Check if this job is for the port number we want
            // to flush jobs for. This is slightly problematic if some
            // Adapter job tried to use NDIS_DEFAULT_PORT_NUMBER. However
            // a flush on that port is only called on port delete, which is
            // when we are halting, so its OK if we end up flushing the 
            // non-running adapter jobs
            if (pCurrentJob->get_NdisPortNumber() != NdisPortNumber)
            {
                // Not the port we want to be flushing for
                WFCTrace(
                    "[A:%x] Not flushing job whose port number %d is not the one we are flushing %d.",
                    pCurrentJob->get_ActivityId(),
                    pCurrentJob->get_NdisPortNumber(),                    
                    NdisPortNumber );
                
                bFlushJob = false;
            }
        }

        if (bFlushJob)
        {
            // Will flush this job
            
            jobType = pCurrentJob->get_JobType();

            //
            // TODO: Add some callback into the job to signal "you never
            // ran, do any necessary clean-up".
            //
            // Without this, it is expected that the job will do any
            // necessary cleanup when it doesn't run in its destructor, or
            // in the parent completion callback. Improved contract would
            // have a call into the job (optionally implemented) to signal
            // this condition.
            //

            WFCInfo(
                "[A:%x] Flushing job (Type: %u) from queue.",
                pCurrentJob->get_ActivityId(),
                jobType );

            // Abort queued job
            this->CompleteQueuedJob(pCurrentJob,
                NDIS_STATUS_REQUEST_ABORTED
                );
        }

        pCurrentEntry = pNextEntry;
    }

exit:
    TraceExit(NDIS_STATUS_SUCCESS);
}

void SerializedJobList::OnEventQueueCallback(
    _In_ WFC_EVENT_TYPE        Type,
    _In_opt_ NDIS_STATUS        Status,
    _In_opt_ WFC_SENDER_OBJECT SenderObject,
    _In_opt_ WFC_EVENT_PAYLOAD Payload )
{
    UNREFERENCED_PARAMETER(Status);
    UNREFERENCED_PARAMETER(SenderObject);

    switch(Type)
    {
    case WiFiEventQueueSerializedJob:

        NT_ASSERT(Payload != NULL);

        if ( Payload != NULL )
        {
            AddJobToQueueHandler(
                (CJobBase*)Payload );
        }

        break;

    default:

        NT_ASSERT(FALSE);

    };
}

void SerializedJobList::CompleteQueuedJob(
    _In_ CJobBase* pAbortedJob,
    _In_ NDIS_STATUS CompletionStatus
    )
{
    NDIS_STATUS                   status = NDIS_STATUS_SUCCESS;
    Event*                        pSafeCompletionEvent;
    IOperationCompletionCallback* pSerializedJobCallback;

    TraceEntry();

    pAbortedJob->get_SerializedJobCompletionCallback(
        &pSerializedJobCallback );

    pAbortedJob->get_FailsafeCompletionEvent(
        &pSafeCompletionEvent );

    // Remove it from the list
    RemoveEntryList(
        pAbortedJob->get_SerializedJobListEntry() );
    InitializeListHead(
        pAbortedJob->get_SerializedJobListEntry() );

    // Notify the job that its aborted so it know that it didnt even run
    pAbortedJob->OnAborted();

    if ( pSerializedJobCallback != NULL )
    {
        // Signal the job that it has completed
        status = m_pEventQueue->DeferOperationCompletion(
            pSerializedJobCallback,
            CompletionStatus,
            pAbortedJob,
            pSafeCompletionEvent );
    }
    else
    {
        WFCTrace("Not completing serialized job callback because there isn't one.");
    }

    TraceExit(status);
}

void SerializedJobList::OnOperationCompleted(
    _In_ NDIS_STATUS        OperationStatus,
    _In_ WFC_SENDER_OBJECT SenderObject )
{
    NDIS_STATUS                   status = NDIS_STATUS_SUCCESS;
    CJobBase*                     pCompletedJob;
    Event*                        pSafeCompletionEvent;
    IOperationCompletionCallback* pSerializedJobCallback;

    NT_ASSERT(SenderObject != NULL);

    TraceEntry();

    if ( SenderObject == NULL )
    {
        NT_ASSERT(FALSE);
        status = NDIS_STATUS_INVALID_STATE;
        goto exit;
    }

    pCompletedJob = (CJobBase*)SenderObject;

    pCompletedJob->get_SerializedJobCompletionCallback(
        &pSerializedJobCallback );

    pCompletedJob->get_FailsafeCompletionEvent(
        &pSafeCompletionEvent );

    RemoveEntryList(
        pCompletedJob->get_SerializedJobListEntry() );
    InitializeListHead(
        pCompletedJob->get_SerializedJobListEntry() );
    m_pActiveSerializedJob = NULL;

    if ( pSerializedJobCallback != NULL )
    {
        status = m_pEventQueue->DeferOperationCompletion(
            pSerializedJobCallback,
            OperationStatus,
            pCompletedJob,
            pSafeCompletionEvent );
    }
    else
    {
        WFCTrace("Not completing serialized job callback because there isn't one.");
    }

exit:

    ActivateNextSerializedJob();

    TraceExit(status);
}


void SerializedJobList::AddJobToQueueHandler(
    _In_ CJobBase* pNewJob )
{
    TraceEntry();

    if ( pNewJob == NULL )
    {
        goto exit;
    }

    if ( !m_AllowNewJobs )
    {
        WFCError("Cannot queue new serialized job, job queue is not accepting new jobs.");

        OnOperationCompleted(
            NDIS_STATUS_INVALID_STATE,
            pNewJob );

        goto exit;
    }

    InsertTailList(
        &m_JobList,
        pNewJob->get_SerializedJobListEntry() );

    ActivateNextSerializedJob();

exit:
    TraceExit(NDIS_STATUS_SUCCESS);
}

void SerializedJobList::CancelJob(
    _In_ NDIS_PORT_NUMBER NdisPortNumber,
    _In_ WFC_JOB_TYPE     JobType,
    _In_opt_ CJobBase*    pJob
    )
{
    NDIS_STATUS ndisStatus = NDIS_STATUS_SUCCESS;
    CJobBase* currentJob;

    TraceEntry();

    currentJob = this->GetActiveSerializedJob();    
    if (currentJob && 
        (currentJob->get_NdisPortNumber() == NdisPortNumber) &&
        (currentJob->get_JobType() == JobType) &&
        ((pJob == NULL) || (pJob == currentJob)))
    {
        // Job is active in the queue. Cancel it
        WFCInfo("Cancelling active serialized job [O:%p][A:%x]", currentJob, currentJob->get_ActivityId());
        
        currentJob->CancelJob();
    }    
    else 
    {
        WFCInfo("Searching for job type %d, Port %x in serialized queue", JobType, NdisPortNumber);
        // Not active
        if (pJob == NULL)
        {        
            // Find matching job type & complete it
            currentJob = this->FindQueuedSerializedJob (
                NdisPortNumber,
                JobType);

            if (currentJob)
            {
                WFCInfo("Aborting job from serialized queue [O:%p][A:%x]", currentJob, currentJob->get_ActivityId());

                this->CompleteQueuedJob(currentJob,
                    NDIS_STATUS_REQUEST_ABORTED
                    );                
            }
        }
        else
        {
            WFCInfo("Aborting from serialized queue [O:%p][A:%x]", pJob, pJob->get_ActivityId());

            this->CompleteQueuedJob(pJob,
                NDIS_STATUS_REQUEST_ABORTED
                );
        }
    }
    TraceExit (ndisStatus);
}


void SerializedJobList::PreemptActiveJobIfNecessary(
    _In_ CJobBase* pBestCandidate )
{
    TraceEntry();

    if ( m_pActiveSerializedJob == NULL )
    {
        NT_ASSERT(FALSE);
        goto exit;
    }

    if ( !pBestCandidate->get_CanPreempt() ||
         !m_pActiveSerializedJob->get_IsPreemptable() )
    {
        goto exit;
    }

    if ( pBestCandidate->get_PriorityScope() == WiFiPriorityScopePort &&
         pBestCandidate->get_NdisPortNumber() != m_pActiveSerializedJob->get_NdisPortNumber() )
    {
        goto exit;
    }

    if ( pBestCandidate->get_JobPriority() >=
         m_pActiveSerializedJob->get_JobPriority() )
    {
        // Even if the priority of incoming job is lower, if current job supports deferral, 
        // request the job to defer if possible
        if (m_pActiveSerializedJob->get_IsDeferrable())
        {
            WFCInfo("Requesting active serialized job %d to defer itself early so that new job %d gets to run",
                m_pActiveSerializedJob->get_JobType(), pBestCandidate->get_JobType());
            m_pActiveSerializedJob->set_DeferHint(true);
        }
        goto exit;
    }

    WFCInfo("Cancelling existing task %d to schedule new task %d",
            m_pActiveSerializedJob->get_JobType(), pBestCandidate->get_JobType());
    m_pActiveSerializedJob->CancelJob();

exit:
    TraceExit(NDIS_STATUS_SUCCESS);
}


void SerializedJobList::ActivateNextSerializedJob()
{
    NDIS_STATUS status = NDIS_STATUS_SUCCESS;
    CJobBase* pNextBestJob;

    TraceEntry();

    do
    {
        pNextBestJob = ChooseBestCandidate();

        if ( pNextBestJob == NULL )
        {
            goto exit;
        }

        if ( m_pActiveSerializedJob != NULL )
        {
            PreemptActiveJobIfNecessary(
                pNextBestJob );

            goto exit;
        }
        else
        {
            RemoveEntryList(
                pNextBestJob->get_SerializedJobListEntry() );
            InitializeListHead(
                pNextBestJob->get_SerializedJobListEntry() );

            m_pActiveSerializedJob = pNextBestJob;

            status = m_pActiveJobsList->StartAddNewJob(
                pNextBestJob,
                (IOperationCompletionCallback*)this );

            if ( status == NDIS_STATUS_SUCCESS )
            {
                goto exit;
            }

            if ( status != NDIS_STATUS_SUCCESS )
            {
                OnOperationCompleted(
                    status,
                    pNextBestJob );
            }
        }
    } while ( pNextBestJob != NULL );

exit:
    TraceExit(status);
}


CJobBase* SerializedJobList::ChooseBestCandidate()
{
    CJobBase* pBestPortScopeCandidate = NULL;
    CJobBase* pBestCandidate = NULL;
    CJobBase* pBestDeferrableCandidate = NULL;

    PLIST_ENTRY pCurrentEntry = m_JobList.Flink;

    if ( IsListEmpty(&m_JobList) )
    {
        goto exit;
    }

    while ( pCurrentEntry != &m_JobList )
    {
        CJobBase* pCurrentCandidate = CJobBase::get_JobFromListEntry(
            pCurrentEntry );

        //
        // TODO: Confirm if this is right - Port Scope is only
        // relevant in relation to the currently active serialized job
        // (if there is one).
        //
        if ( m_pActiveSerializedJob != NULL && 
             pCurrentCandidate->get_NdisPortNumber() == m_pActiveSerializedJob->get_NdisPortNumber() &&
             pCurrentCandidate->get_PriorityScope() == WiFiPriorityScopePort &&
             pCurrentCandidate->get_JobPriority() <= m_pActiveSerializedJob->get_JobPriority() )
        {
            pBestPortScopeCandidate = pCurrentCandidate;
        }

        // If there are non-deferrable jobs waiting in the queue, allow those to run before any deferrable job
        // even if they are lower priority than the deferrable job. Assumption is that the non-deferrable jobs 
        // won't take long to complete and the deferrable job may have been requeued to allow these jobs to run.
        if ( pCurrentCandidate->get_IsDeferrable() )
        {
            if ( pBestDeferrableCandidate == NULL ||
                 pCurrentCandidate->get_JobPriority() < pBestDeferrableCandidate->get_JobPriority() )
            {
                pBestDeferrableCandidate = pCurrentCandidate;
            }
        }
        else
        {
            if ( pBestCandidate == NULL ||
                 pCurrentCandidate->get_JobPriority() < pBestCandidate->get_JobPriority() )
            {
                pBestCandidate = pCurrentCandidate;
            }
        }

        pCurrentEntry = pCurrentEntry->Flink;
    }

    if ( pBestCandidate == NULL )
    {
        pBestCandidate = pBestDeferrableCandidate;
    }

    if ( pBestPortScopeCandidate != NULL &&
         pBestCandidate->get_JobPriority() <= pBestPortScopeCandidate->get_JobPriority() )
    {
        pBestCandidate = pBestPortScopeCandidate;
    }

exit:
    return pBestCandidate;
}

CJobBase* SerializedJobList::FindQueuedSerializedJob(
    _In_ NDIS_PORT_NUMBER NdisPortNumber,
    _In_ WFC_JOB_TYPE     JobType,
    _In_ bool CheckPortNumber )
{
    CJobBase* pFindJob = NULL;

    PLIST_ENTRY pCurrentEntry = m_JobList.Flink;

    if ( IsListEmpty(&m_JobList) )
    {
        goto exit;
    }

    while ( pCurrentEntry != &m_JobList )
    {
        CJobBase* pCurrentCandidate = CJobBase::get_JobFromListEntry(
            pCurrentEntry );

        if ( pCurrentCandidate->get_JobType() == JobType &&
             ( !CheckPortNumber || pCurrentCandidate->get_NdisPortNumber() == NdisPortNumber ) )
        {
            pFindJob = pCurrentCandidate;
            goto exit;
        }

        pCurrentEntry = pCurrentEntry->Flink;
    }

exit:

    return pFindJob;
}
#pragma once

#include <nt.h>
#include <ntrtl.h>
#include <nturtl.h>
#include <windows.h>
#include <dxmini.h>
#include <d3dkmddi.h>
#include <d3dkmthk.h>
#include "AdapterUtil.h"
#include <initguid.h>
#include "D3D11Test.h"
#include "WGFTestCore.h"
#include "FormatHelper.h"
#include "d3d11internaltest.h"
				
#define	FORMAT_RT		DXGI_FORMAT_R32G32B32A32_FLOAT
#define PRIMITIVE_SIZE	32

typedef struct TEST_COLOR
{
	DOUBLE r;
	DOUBLE g;
	DOUBLE b;
	DOUBLE a;
} 	TEST_COLOR;

class CAsyncConfTest : public CD3D11Test
{
public:
	CAsyncConfTest()
		: m_pDebug(NULL)
		, m_nPrimitiveSize( PRIMITIVE_SIZE )
		, m_nIndexCount( 0 )
		, m_pVS( NULL )
		, m_pGS( NULL )
		, m_pPS( NULL )
		, m_FormatRT( FORMAT_RT )
		, m_pTextureRT( NULL )
		, m_pTextureRTCopy( NULL )
		, m_pTextureDepth( NULL )
		, m_pRTView( NULL )
		, m_pDSView( NULL )
		, m_pRSEnabled( NULL )
		, m_pDSSEnabled( NULL )
		, m_pVertexBuffer( NULL )
		, m_pIndexBuffer( NULL )
		, m_pConstBuffer( NULL )
		, m_pSOBuffer( NULL )
		, m_pLayout( NULL )
		, m_bGpuJobSetup( false )
	{
		m_bDeferredREF = GetFramework()->IsDeferredContextUsed() && GetFramework()->m_D3DOptions.DriverType == D3D_DRIVER_TYPE_REFERENCE;
	}
	virtual TEST_RESULT Setup();
	virtual void Cleanup( );

protected:
	ID3D11Debug			*m_pDebug;

	bool					m_bDeferredREF;
	int						m_nPrimitiveSize;
	int						m_nIndexCount;
	DXGI_FORMAT				m_FormatRT;

	ID3D11VertexShader			*m_pVS;
	ID3D11GeometryShader			*m_pGS;
	ID3D11PixelShader			*m_pPS;
	ID3D11Texture2D				*m_pTextureRT;
	ID3D11Texture2D				*m_pTextureRTCopy;
	ID3D11Texture2D				*m_pTextureDepth;
	ID3D11RenderTargetView		*m_pRTView;
	ID3D11DepthStencilView		*m_pDSView;
	ID3D11RasterizerState		*m_pRSEnabled;
	ID3D11DepthStencilState		*m_pDSSEnabled;
	ID3D11Buffer					*m_pVertexBuffer;
	ID3D11Buffer					*m_pIndexBuffer;
	ID3D11Buffer					*m_pConstBuffer;
	ID3D11Buffer					*m_pSOBuffer;
	ID3D11InputLayout			*m_pLayout;

	HRESULT			CpuBoundJob();
	HRESULT			GpuBoundJob(bool bExtraGPUWork);
	HRESULT			SetupGpuJob();
	HRESULT			ReadBackBufferColor(int x, int y, TEST_COLOR* pColor);
	bool			TestColorsSame(const TEST_COLOR& c1, const TEST_COLOR& c2, double delta);

protected:
	struct TVERTEX
	{
		float vPos[3];
		DWORD Color;
		
	};
	struct SOVERTEX
	{
		float vPos[4];
	};

private:
	TEST_RESULT SetupDirect3D( );
	TEST_RESULT SetupGeometry( bool bIsLevel9 );
	TEST_RESULT SetupRenderTarget( bool bIsLevel9 );
	TEST_RESULT SetupShaders( bool bIsLevel9 );
	void CleanupDirect3D( );

	TEST_RESULT SetupTestCase();

	bool m_bGpuJobSetup;
};
	
class CQueryConfTest : public CAsyncConfTest
{
public:
	virtual void InitTestParameters();
	virtual TEST_RESULT ExecuteTestCase();

protected:
	bool CallProcess(LPSTR pszwCommand);
	TEST_RESULT TimingTestCase();
	TEST_RESULT OcclusionTestCase();
	TEST_RESULT StatsTestCase();

		template<typename T>
		TEST_RESULT VerifyStats(const T& value1, const T& value2);

	bool				m_bCanBegin;
	bool				m_bNestQueries;
	bool				m_bOverlapQueries;
	int                 		m_nPrefixGpuIterations;
	int                 		m_nSuffixGpuIterations;
	bool				m_bDoGpuJob;
	D3D11_QUERY_DESC		m_QueryDesc;
};

class CPredicateConfTest : public CAsyncConfTest
{
public:
	virtual void InitTestParameters();
	virtual TEST_RESULT ExecuteTestCase();

protected:
	// reads the first pixel from a USAGE_DEFAULT texture 2D
	TEST_RESULT ReadTexture2DColor(ID3D11Texture2D* pTex, UINT uSubresource, void* Out);

	virtual TEST_RESULT InitializeForGpuJob() = 0;
	virtual void CleanupFromGpuJob() = 0;
	virtual void DoPredicatedGpuJob(UINT uIdx) = 0;
	virtual TEST_RESULT VerifyResults(UINT uIdx) = 0;

	bool				m_bHint;
	bool				m_bDoGpuJob;
	bool				m_bPredValue;
	D3D11_QUERY_DESC    m_PredDesc;
};

class CClearRTVPredicateConfTest : public CPredicateConfTest
{
public:
	CClearRTVPredicateConfTest();
	~CClearRTVPredicateConfTest();

protected:
	virtual TEST_RESULT InitializeForGpuJob();
	virtual void CleanupFromGpuJob();
	virtual void DoPredicatedGpuJob(UINT uIdx);
	virtual TEST_RESULT VerifyResults(UINT uIdx);

private:
	FLOAT_COLOR GetClearColor(UINT uIdx);
	ID3D11Texture2D*        m_pRT;
	ID3D11RenderTargetView* m_pRTV;
};

class CClearDSVPredicateConfTest : public CPredicateConfTest
{
public:
	CClearDSVPredicateConfTest();
	~CClearDSVPredicateConfTest();

protected:
	virtual TEST_RESULT InitializeForGpuJob();
	virtual void CleanupFromGpuJob();
	virtual void DoPredicatedGpuJob(UINT uIdx);
	virtual TEST_RESULT VerifyResults(UINT uIdx);

private:
	float GetClearDepth(UINT uIdx);
	ID3D11Texture2D*        m_pDepthBuffer;
	ID3D11DepthStencilView* m_pDSV;
};

class CCopyResourcePredicateConfTest : public CPredicateConfTest
{
public:
	CCopyResourcePredicateConfTest();
	~CCopyResourcePredicateConfTest();

protected:
	virtual TEST_RESULT InitializeForGpuJob();
	virtual void CleanupFromGpuJob();
	virtual void DoPredicatedGpuJob(UINT uIdx);
	virtual TEST_RESULT VerifyResults(UINT uIdx);

	DWORD GetColor(UINT uIdx);
	ID3D11Texture2D* m_pSrc0;
	ID3D11Texture2D* m_pSrc1;
	ID3D11Texture2D* m_pDst;
};

class CCopySubresourceRegionPredicateConfTest : public CCopyResourcePredicateConfTest
{
protected:
	virtual void DoPredicatedGpuJob(UINT uIdx);
};

class CUpdateSubresourcePredicateConfTest : public CPredicateConfTest
{
public:
	CUpdateSubresourcePredicateConfTest();
	~CUpdateSubresourcePredicateConfTest();

protected:
	virtual TEST_RESULT InitializeForGpuJob();
	virtual void CleanupFromGpuJob();
	virtual void DoPredicatedGpuJob(UINT uIdx);
	virtual TEST_RESULT VerifyResults(UINT uIdx);

	DWORD GetColor(UINT uIdx);
	ID3D11Texture2D* m_pDst;
};

class CResolveSubresourcePredicateConfTest : public CPredicateConfTest
{
public:
	CResolveSubresourcePredicateConfTest();
	~CResolveSubresourcePredicateConfTest();

protected:
	virtual TEST_RESULT InitializeForGpuJob();
	virtual void CleanupFromGpuJob();
	virtual void DoPredicatedGpuJob(UINT uIdx);
	virtual TEST_RESULT VerifyResults(UINT uIdx);

	DWORD GetColor(UINT uIdx);
	ID3D11Texture2D* m_pSrc0;
	ID3D11Texture2D* m_pSrc1;
	ID3D11Texture2D* m_pDst;
};

class CGenerateMipsPredicateConfTest : public CPredicateConfTest
{
public:
	CGenerateMipsPredicateConfTest();
	~CGenerateMipsPredicateConfTest();

protected:
	virtual TEST_RESULT InitializeForGpuJob();
	virtual void CleanupFromGpuJob();
	virtual void DoPredicatedGpuJob(UINT uIdx);
	virtual TEST_RESULT VerifyResults(UINT uIdx);

	DWORD GetColor(UINT uIdx);
	ID3D11Texture2D* m_pTex;
	ID3D11ShaderResourceView* m_pSRV;
};

class CDrawBasePredicateConfTest : public CPredicateConfTest
{
public:
	CDrawBasePredicateConfTest();
	~CDrawBasePredicateConfTest();

protected:
	virtual TEST_RESULT InitializeForGpuJob();
	virtual void CleanupFromGpuJob();
	virtual void DoPredicatedGpuJob(UINT uIdx);
	virtual TEST_RESULT VerifyResults(UINT uIdx);

	DWORD GetColor(UINT uIdx);
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	static const UINT VERTEX_COUNT = 8;
	static const UINT INDEX_COUNT  = 4;

	virtual void FillVertexBuffer(ID3D11Buffer* pBuffer, const TVERTEX* pVertices);
	virtual void Draw() = 0;
};

class CDrawPredicateConfTest : public CDrawBasePredicateConfTest
{
protected:
	virtual void Draw();
};

class CDrawInstancedPredicateConfTest : public CDrawBasePredicateConfTest
{
protected:
	virtual void Draw();
};

class CDrawIndexedPredicateConfTest : public CDrawBasePredicateConfTest
{
protected:
	virtual void Draw();
};

class CDrawIndexedInstancedPredicateConfTest : public CDrawBasePredicateConfTest
{
protected:
	virtual void Draw();
};

class CDrawAutoPredicateConfTest : public CDrawBasePredicateConfTest
{
protected:
	virtual void FillVertexBuffer(ID3D11Buffer* pBuffer, const TVERTEX* pVertices);
	virtual void Draw();
};

class CCounterConfTest : public CAsyncConfTest
{
public:
	virtual void InitTestParameters();
	virtual TEST_RESULT ExecuteTestCase();

	TEST_RESULT Setup();
	void Cleanup();

protected:
	bool			m_bNestQueries;
	bool			m_bOverlapQueries;
	int				m_nGpuJobs;
	int				m_nDeviceDependentNbr;
	D3D11_COUNTER		m_CounterName;
};

class COverAllocTest : public CAsyncConfTest
{
public:
	virtual void InitTestParameters();
	virtual TEST_RESULT ExecuteTestCase();

protected:
	bool				m_bOverAlloc;
	bool				m_bOverAllocIssue;
	D3D11_QUERY_DESC		m_QueryDesc;
};

class CAsyncConfApp : public CD3D11TestFramework
{
public:
	TESTTYPE GetTestType() const { return TESTTYPE_CONF; }
	void InitOptionVariables();
	bool InitTestGroups();

	UINT GetDefaultQueryTimeout() { return 30000; }
};
extern CAsyncConfApp  g_App;




// Given the results of query1 and query2 and the values of m_bOverlapQueries and m_bDoGpuJob
// check the statistics returned by the queries for zero.  Fail as approriate.
// This function is used for pipeline stats and so output stats
template<typename T>
TEST_RESULT CQueryConfTest::VerifyStats(const T& value1, const T& value2)
{
	TEST_RESULT tRes = RESULT_PASS;

	if (!m_bOverlapQueries)
	{
		if (value1 == value2)
		{
			WriteToLog( _T("Statistics values are identical.\n") );
			tRes = RESULT_FAIL;
			goto testDone;
		}

		if (m_bDoGpuJob)
		{
			if( value1.IsZero() )
			{
				WriteToLog( _T("Statistics value is zero when it should not be\n") );
				tRes = RESULT_FAIL;
				goto testDone;
			}

			if ( !value2.IsZero() )
			{
				WriteToLog( _T("Statistics value is not zero when it should be\n") );
				tRes = RESULT_FAIL;
				goto testDone;
			}
		}
		else
		{
			if( !value1.IsZero() )
			{
				WriteToLog( _T("Statistics value is not zero when it should be\n") );
				tRes = RESULT_FAIL;
				goto testDone;
			}

			if ( value2.IsZero() )
			{
				WriteToLog( _T("Statistics value is zero when it should not be\n") );
				tRes = RESULT_FAIL;
				goto testDone;
			}
		}
	}
	else
	{
		if ( !(value1 == value2) )
		{
			WriteToLog( _T("Statistics values are different.\n") );
			tRes = RESULT_FAIL;
			goto testDone;
		}

		if (m_bDoGpuJob)
		{
			if( value1.IsZero() )
			{
				WriteToLog( _T("Statistics value is zero when it should not be\n") );
				tRes = RESULT_FAIL;
				goto testDone;
			}
		}
		else
		{
			if( !value1.IsZero() )
			{
				WriteToLog( _T("Statistics value is not zero when it should be\n") );
				tRes = RESULT_FAIL;
				goto testDone;
			}
		}
	}

testDone:
	return tRes;
}
/*++

Copyright (C) Microsoft. All rights reserved.

Module Name:

    arbport.c

Abstract:

    This module contains functions for arbitrating I/O port resources











--*/

// -------------------------------------------------------------------- Includes

#include "pcip.h"

// --------------------------------------------------------------------- Defines

#define PORT_ARBITER_VERSION   0

//
// Flags for AllocationState WorkSpace
//
#define PORT_ARBITER_PREPROCESSED       0x00000001
#define PORT_ARBITER_IMPROVISED_DECODE  0x00000002
#define PORT_ARBITER_ISA_BIT_SET        0x00000004
#define PORT_ARBITER_BRIDGE_WINDOW      0x00000008

//
// Constants
//
#define PCI_BRIDGE_ISA_BIT_STRIDE       0x400
#define PCI_BRIDGE_ISA_BIT_WIDTH        0x100
#define PCI_BRIDGE_ISA_BIT_MAX          0xFFFF
#define MAX_10_BIT_DECODE               0x3FF
#define MAX_12_BIT_DECODE               0xFFF
#define MAX_16_BIT_DECODE               0xFFFF

// ----------------------------------------------------------------------- Types
// ------------------------------------------------------------------ Prototypes

NTSTATUS
PciIoArbiter_ReinitializeArbiter(
    _Inout_ PARBITER_INSTANCE Arbiter,
    _In_ ULONG ResourceCount,
    __in_ecount(ResourceCount) PCM_PARTIAL_RESOURCE_DESCRIPTOR Resources,
    _Inout_ PRTL_RANGE_LIST RangeList
    );

NTSTATUS
PciIoArbiter_QueryArbitrate(
    __in PARBITER_INSTANCE Arbiter,
    __in PARBITER_QUERY_ARBITRATE_PARAMETERS Parameters
    );

NTSTATUS
PciIoArbiter_PreprocessEntry(
    __in PARBITER_INSTANCE Arbiter,
    __inout PARBITER_ALLOCATION_STATE State
    );

BOOLEAN
PciIoArbiter_GetNextAllocationRange(
    __inout PARBITER_INSTANCE Arbiter,
    __inout PARBITER_ALLOCATION_STATE State
    );

BOOLEAN
PciIoArbiter_FindSuitableRange(
    __in PARBITER_INSTANCE Arbiter,
    __inout PARBITER_ALLOCATION_STATE State
    );

BOOLEAN
PciIoArbiter_OverrideConflict(
    __in PARBITER_INSTANCE Arbiter,
    __inout PARBITER_ALLOCATION_STATE State
    );

VOID
PciIoArbiter_AddAllocation(
    __in PARBITER_INSTANCE Arbiter,
    __in PARBITER_ALLOCATION_STATE State
    );

VOID
PciIoArbiter_BacktrackAllocation(
     __in PARBITER_INSTANCE Arbiter,
     __in PARBITER_ALLOCATION_STATE State
     );

VOID
PciAddOrBacktrackIoAllocation(
     __in PARBITER_INSTANCE Arbiter,
     __in PARBITER_ALLOCATION_STATE State,
     __in PARBITER_BACKTRACK_ALLOCATION Callback
    );

VOID
PciSetAllocationAttributes(
    __inout PARBITER_ALLOCATION_STATE State
    );

BOOLEAN
PciFindSuitableIsaBitRange(
    __in PARBITER_INSTANCE Arbiter,
    __inout PARBITER_ALLOCATION_STATE State
    );

BOOLEAN
PciAllAliasesAvailable(
    __in PARBITER_INSTANCE Arbiter,
    __in PARBITER_ALLOCATION_STATE State
    );

NTSTATUS
PciExcludeRangesFromWindow(
    __in ULONGLONG Start,
    __in ULONGLONG End,
    __inout PRTL_RANGE_LIST ArbiterRanges,
    __in PRTL_RANGE_LIST ExclusionRanges
    );

// --------------------------------------------------------------------- Pragmas

#pragma prefast(disable:__WARNING_UNUSED_SCALAR_ASSIGNMENT, "checked builds")

#ifdef ALLOC_PRAGMA
    #pragma alloc_text(PAGE, PciInitializeIoArbiter)
    #pragma alloc_text(PAGE, PciProgramIoArbiter)
    #pragma alloc_text(INIT, PciBuildDefaultIoExclusionLists)
    #pragma alloc_text(PAGE, PciIoArbiter_ReinitializeArbiter)
    #pragma alloc_text(PAGE, PciIoArbiter_QueryArbitrate)
    #pragma alloc_text(PAGE, PciIoArbiter_PreprocessEntry)
    #pragma alloc_text(PAGE, PciIoArbiter_GetNextAllocationRange)
    #pragma alloc_text(PAGE, PciIoArbiter_FindSuitableRange)
    #pragma alloc_text(PAGE, PciIoArbiter_OverrideConflict)
    #pragma alloc_text(PAGE, PciIoArbiter_AddAllocation)
    #pragma alloc_text(PAGE, PciIoArbiter_BacktrackAllocation)
    #pragma alloc_text(PAGE, PciAddOrBacktrackIoAllocation)
    #pragma alloc_text(PAGE, PciSetAllocationAttributes)
    #pragma alloc_text(PAGE, PciFindSuitableIsaBitRange)
    #pragma alloc_text(PAGE, PciAllAliasesAvailable)
    #pragma alloc_text(PAGE, PciExcludeRangesFromWindow)
#endif

// --------------------------------------------------------------------- Globals

//
// Range lists indicating the ranges excluded from decode when the ISA and/or
// VGA bits are set on a bridge.  Initialized by PciBuildDefaultIoExclusionLists
// from DriverEntry.
//
RTL_RANGE_LIST PciIsaBitExclusionList;
RTL_RANGE_LIST PciVgaAndIsaBitExclusionList;

ARBITER_ORDERING PciBridgeOrderings[] = {
    { 0x10000, MAXULONGLONG },
    { 0,       0xFFFF }
};

ARBITER_ORDERING_LIST PciBridgeOrderingList = {
    sizeof(PciBridgeOrderings) / sizeof (ARBITER_ORDERING),
    sizeof(PciBridgeOrderings) / sizeof (ARBITER_ORDERING),
    PciBridgeOrderings
};

// ------------------------------------------------------------------- Functions

NTSTATUS
PciInitializeIoArbiter(
    __in PPCI_BUS Bus,
    __out PARBITER_INSTANCE *IoArbiter
    )
/*++

Routine Description:

    This routine is called once per instantiation of an arbiter.
    It initializes the arbiter with the correct callbacks and
    with the provided resources.

Arguments:

    Bus - The bus this arbiter is for.

    IoArbiter - The arbiter to initialize.

    Resources - An array of resources to use to initialize the arbiter.

    ResourceCount - The number of elements in the Resources array.

Return Value:

    NT Status code.

--*/
{
    PARBITER_INSTANCE arbiter;
    PWSTR arbiterName;
    NTSTATUS status;

    PAGED_CODE();

    *IoArbiter = NULL;
    arbiterName = NULL;

    arbiter = PciAllocatePool(NonPagedPoolNx, sizeof(ARBITER_INSTANCE));
    if (arbiter == NULL) {
        status = STATUS_INSUFFICIENT_RESOURCES;
        goto cleanup;
    }
    RtlZeroMemory(arbiter, sizeof(ARBITER_INSTANCE));

    //
    // Set the Action Handler entry points.
    //
    arbiter->InitializeRangeList    = PciIoArbiter_ReinitializeArbiter;
    arbiter->PreprocessEntry        = PciIoArbiter_PreprocessEntry;
    arbiter->GetNextAllocationRange = PciIoArbiter_GetNextAllocationRange;
    arbiter->FindSuitableRange      = PciIoArbiter_FindSuitableRange;
    arbiter->AddAllocation          = PciIoArbiter_AddAllocation;
    arbiter->BacktrackAllocation    = PciIoArbiter_BacktrackAllocation;
    arbiter->OverrideConflict       = PciIoArbiter_OverrideConflict;
    arbiter->QueryArbitrate         = PciIoArbiter_QueryArbitrate;

    arbiter->UnpackRequirement = PciArbiter_UnpackRequirement;
    arbiter->PackResource      = PciArbiter_PackResource;
    arbiter->UnpackResource    = PciArbiter_UnpackResource;
    arbiter->ScoreRequirement  = PciArbiter_ScoreRequirement;

    //
    // Name the arbiter.
    //
    arbiterName = PciAllocatePool(NonPagedPoolNx, sizeof(L"PCI I/O Port (b=xxxxxxxx)"));
    if (arbiterName == NULL) {
        status = STATUS_INSUFFICIENT_RESOURCES;
        goto cleanup;
    }

    status = RtlStringCbPrintfW(arbiterName,
                                sizeof(L"PCI I/O Port (b=xxxxxxxx)"),
                                L"PCI I/O Port (b=%x)",
                                Bus->SecondaryBusNumber
                                );

    PCI_ASSERT(NT_SUCCESS(status));    
    
    //
    // Initialize the arbiter library's fields.
    //
    status =  ArbInitializeArbiterInstance(arbiter,
                                           Bus->DeviceObject,
                                           CmResourceTypePort,
                                           arbiterName,
                                           L"Pci",
                                           NULL
                                           );
    if (!NT_SUCCESS(status)) {
        goto cleanup;
    }

    *IoArbiter = arbiter;

    return STATUS_SUCCESS;

cleanup:

    if (arbiterName) {
        PciFreePool(arbiterName);
    }
    if (arbiter) {
        PciFreePool(arbiter);
    }

    return status;
}

NTSTATUS
PciProgramIoArbiter(
    __in PPCI_BUS Bus,
    __in PARBITER_INSTANCE Arbiter,
    __in_ecount(ResourceCount) PCM_PARTIAL_RESOURCE_DESCRIPTOR Resources,
    __in ULONG ResourceCount,
    __inout PRTL_RANGE_LIST RangeList
    )
/*++

Routine Description:

    This routine programs the provided resources into the arbiter.
    It first initializes the given range list with the resources
    and then does any arbiter setup required based on these resources.

Arguments:

    Bus - The bus this arbiter is for.

    Arbiter - The arbiter to program.

    Resources - An array of resources to use to program the arbiter.

    ResourceCount - The number of elements in the Resources array.

    RangeList - The range list to program.

Return Value:

    NT Status code.

--*/
{
    PCM_PARTIAL_RESOURCE_DESCRIPTOR descriptor;
    PRTL_RANGE_LIST exclusionList;
    PARBITER_INSTANCE rootArbiter;
    BOOLEAN foundResource;
    PPCI_BRIDGE bridge;
    PPCI_BUS rootBus;
    NTSTATUS status;
    ULONGLONG dummy;

    PAGED_CODE();

    //
    // First program the range list with the resources provided.
    //
    status = PciRangeListFromResourceList(Bus,
                                          Arbiter,
                                          Resources,
                                          ResourceCount,
                                          RangeList
                                          );
    if (!NT_SUCCESS(status)) {
        goto exit;
    }

    //
    // Now do some munging for PCI-PCI bridges.  If this isn't
    // a bridge or there are no resources here, early out.
    //
    if ((ResourceCount == 0) || !Bus->PciBridge) {
        status = STATUS_SUCCESS;
        goto exit;
    }

    bridge = Bus->PciBridge;

    //
    // Select the appropriate exclusion list.  Note that bridges with the
    // 16 bit VGA decoding bit don't get the VgaAndIsa exclusion list,
    // because the 16 bit VGA resources don't overlap with bridge windows.
    // This is true UNLESS the bridge window lives at 0-0xFFF, which can
    // never happen today.  If that ever can happen, a new exclusion
    // list is required and the following code is wrong.
    //
    exclusionList = NULL;
    if (bridge->Attributes.IsaBitSet) {

        if (bridge->Attributes.LegacyVgaBitSet) {
            exclusionList = &PciVgaAndIsaBitExclusionList;
        } else {
            exclusionList = &PciIsaBitExclusionList;
        }
    }

    //
    // Find the port window and if the ISA bit is set exclude
    // the ranges in the window that aren't available when
    // the ISA bit is set.
    //
    foundResource = FALSE;
    FOR_ALL_IN_ARRAY(Resources, ResourceCount, descriptor) {

        if ((descriptor->Type == CmResourceTypePort) &&
            (descriptor->Flags & CM_RESOURCE_PORT_WINDOW_DECODE)) {

            if (exclusionList) {
                status = PciExcludeRangesFromWindow(
                             descriptor->u.Port.Start.QuadPart,
                             descriptor->u.Port.Start.QuadPart
                                + descriptor->u.Port.Length - 1,
                             RangeList,
                             exclusionList
                             );
                if (!NT_SUCCESS(status)) {
                    goto exit;
                }
            }

            foundResource = TRUE;
            break;
        }
    }

    //
    // Now deal with sparse root busses.  This is when a root bus (typically
    // not the first root bus) is assigned only tiny slices of I/O space, not
    // enough for an entire bridge window.  Bridges can operate underneath
    // these, but no devices can be placed in the ranges that don't get down
    // the bus.  To handle this, make sure all ranges not allocated to the
    // root bus's arbiter are excluded from this arbiter.
    //
    if (foundResource == FALSE) {

        //
        // No I/O on the bus, so nothing to do for sparse roots.
        //
        status = STATUS_SUCCESS;
        goto exit;
    }

    rootBus = Bus->Root->Bus;
    rootArbiter = rootBus->IoArbiter;

    if (rootArbiter == NULL) {
        status = STATUS_INVALID_PARAMETER;
        goto exit;
    }

    ArbAcquireArbiterLock(rootArbiter);

    status = PciExcludeRangesFromWindow(
                descriptor->u.Port.Start.QuadPart,
                descriptor->u.Port.Start.QuadPart
                   + descriptor->u.Port.Length - 1,
                RangeList,
                rootArbiter->Allocation
                );

    ArbReleaseArbiterLock(rootArbiter);

    if (!NT_SUCCESS(status)) {
        goto exit;
    }

    //
    // Sanity check this to make sure that at least one port is available - if
    // not then fail start.  You could argue that we should really have this
    // marked as insufficient resources (code 12) as oppose to failed start
    // (code 10) but that is much harder and this has the desired effect.
    // We check by seeing if we can find a range for the minimal PCI requirements
    // of 4 ports alignment 4.
    //
    status = RtlFindRange(RangeList,
                          0,
                          MAXULONGLONG,
                          4,
                          4,
                          0,     // Flags
                          0,     // AttribureAvailableMask
                          NULL,  // Context
                          NULL,  // Callback
                          &dummy
                          );
    if (!NT_SUCCESS(status)) {
        status = STATUS_INSUFFICIENT_RESOURCES;
    }

exit:

    return status;
}

NTSTATUS
PciBuildDefaultIoExclusionLists(
    VOID
    )
{
    NTSTATUS status;
    ULONG windowBase;

    PAGED_CODE();

    PCI_ASSERT(PciIsaBitExclusionList.Count == 0);
    PCI_ASSERT(PciVgaAndIsaBitExclusionList.Count == 0);

    RtlInitializeRangeList(&PciIsaBitExclusionList);
    RtlInitializeRangeList(&PciVgaAndIsaBitExclusionList);

    for (windowBase = 0; windowBase <= 0xFFFF; windowBase += 0x400) {

        //
        // Add the x100-x3ff range to the ISA list
        //
        status = RtlAddRange(&PciIsaBitExclusionList,
                             windowBase + 0x100,
                             windowBase + 0x3FF,
                             0,
                             RTL_RANGE_LIST_ADD_IF_CONFLICT,
                             NULL,
                             NULL // this range is not on the bus
                             );
        if (!NT_SUCCESS(status)) {
            goto cleanup;
        }

        //
        // Add the x100-x3af, x3bc-x3bf and x3e0-x3ff ranges to the VGA/ISA list
        //
        status = RtlAddRange(&PciVgaAndIsaBitExclusionList,
                             windowBase + 0x100,
                             windowBase + 0x3AF,
                             0,
                             RTL_RANGE_LIST_ADD_IF_CONFLICT,
                             NULL,
                             NULL // this range is not on the bus
                             );
        if (!NT_SUCCESS(status)) {
            goto cleanup;
        }

        status = RtlAddRange(&PciVgaAndIsaBitExclusionList,
                             windowBase + 0x3BC,
                             windowBase + 0x3BF,
                             0,
                             RTL_RANGE_LIST_ADD_IF_CONFLICT,
                             NULL,
                             NULL // this range is not on the bus
                             );
        if (!NT_SUCCESS(status)) {
            goto cleanup;
        }

        status = RtlAddRange(&PciVgaAndIsaBitExclusionList,
                             windowBase + 0x3E0,
                             windowBase + 0x3FF,
                             0,
                             RTL_RANGE_LIST_ADD_IF_CONFLICT,
                             NULL,
                             NULL // this range is not on the bus
                             );
         if (!NT_SUCCESS(status)) {
            goto cleanup;
        }
    }

    return STATUS_SUCCESS;

cleanup:

    RtlFreeRangeList(&PciIsaBitExclusionList);
    RtlFreeRangeList(&PciVgaAndIsaBitExclusionList);

    return status;
}

// --------------------------------------------------------------- I/O Arbiter entrypoints

NTSTATUS
PciIoArbiter_ReinitializeArbiter(
    _Inout_ PARBITER_INSTANCE Arbiter,
    _In_ ULONG ResourceCount,
    __in_ecount(ResourceCount) PCM_PARTIAL_RESOURCE_DESCRIPTOR Resources,
    _Inout_ PRTL_RANGE_LIST RangeList
    )
/*++

Routine Description:

    This is the ReInitializeArbiter entrypoint for the PCI I/O arbiter.
    This takes a set of resources being speculatively assigned to the
    bridge this arbiter is for, and programs the arbiter with these
    resources.  This is used during rebalance.  After this call, a speculative
    arbitration using these resources will take place, and if that is
    successful, the bridge will be restarted with these resources.

Parameters:

    Arbiter - The arbiter to re-initialize.

    ResourceCount - The number of resources in the Resources array.

    Resources - The resources to use to re-initialize the arbiters.

    RangeList - The range list to program with the Resources.

Return Value:

    NT Status code.

--*/
{
    NTSTATUS status;
    PCI_BRIDGE_RESOURCES bridgeResources;
    PPCI_BUS bus;

    PAGED_CODE();

    bus = (PPCI_BUS)Arbiter->BusDeviceObject->DeviceExtension;

    //
    // Root buses can't be reconfigured.
    //

    if (!bus->PciBridge) {
        ERROR("PciIoArbiter_ReinitializeArbiter: PciBridge not initiazlied.\n");
        return STATUS_INVALID_PARAMETER;
    }

    TRACE(ARBITER_TRACE_LEVEL, "\nProgramming arbiter %p with new resources:\n", Arbiter);
    DebugDumpResourceArray(ARBITER_TRACE_LEVEL, Resources, ResourceCount);

    status = PciBridgeProcessStartResources(bus->PciBridge,
                                            Resources,
                                            ResourceCount,
                                            &bridgeResources
                                            );
    if (!NT_SUCCESS(status)) {

        TRACE(ARBITER_TRACE_LEVEL, "Could not process start resources - 0x%x.\n", status);
        return status;
    }

    //
    // Now free up the old range list and re-program it with the new resources.
    //
    RtlFreeRangeList(RangeList);

    status = PciProgramIoArbiter(bus,
                                 Arbiter,
                                 bridgeResources.ByIndex,
                                 PCI_BRIDGE_RESOURCE_COUNT,
                                 RangeList
                                 );

    Arbiter->Extension = (PVOID)PCI_ARBITER_STATE_REPROGRAMMED;

    return status;
}

NTSTATUS
PciIoArbiter_QueryArbitrate(
    __in PARBITER_INSTANCE Arbiter,
    __in PARBITER_QUERY_ARBITRATE_PARAMETERS Parameters
    )
/*++

Routine Description:

    This routine examines the arbiter state and determins whether the
    entry in the ArbitrationList should be arbitrated by this arbiter.

Arguments:

    Arbiter - The arbiter to examine

    Parameters - The parameters for the call.  This has one element,
        ArbitrationList, a list of ARBITER_LIST_ENTRY's, containing only one entry.
        This routine should determine if this list entry should be arbitrated
        by the arbiter.

Return Value:

    TRUE if this arbiter handles this list entry.
    FALSE otherwise.

--*/
{
    PARBITER_LIST_ENTRY arbiterEntry;

    PAGED_CODE();

    //
    // The parameters for the call are an ARBITER_LIST_ENTRY to
    // QueryArbitrate.  It should be the only entry in the list.
    //
    arbiterEntry = CONTAINING_RECORD(Parameters->ArbitrationList->Flink,
                                     ARBITER_LIST_ENTRY,
                                     ListEntry
                                     );

    PCI_ASSERT(arbiterEntry->ListEntry.Flink == Parameters->ArbitrationList);
    if (PciArbitrateRequirementInsideBridgeWindow(
            (PPCI_BUS)Arbiter->BusDeviceObject->DeviceExtension,
            arbiterEntry->PhysicalDeviceObject,
            arbiterEntry->Alternatives,
            arbiterEntry->AlternativeCount
            )) {

        return STATUS_SUCCESS;

    } else {

        TRACE(ARBITER_TRACE_LEVEL,
              "PciIoArbiter_QueryArbitrate: Not arbitrating request.\n"
              );
        return STATUS_UNSUCCESSFUL;
    }
}

NTSTATUS
PciIoArbiter_PreprocessEntry(
    __in PARBITER_INSTANCE Arbiter,
    __inout PARBITER_ALLOCATION_STATE State
    )
/*++

Routine Description:

    This is the PreprocessEntry entrypoint for the PCI I/O arbiter.
    The entry to preprocess corresponds to a requirement that needs
    to be arbitrated for.  This routine gets called before the arbiter
    actually tries to find a solution for this requirement, and is
    used to set some flags for use by RtlFindRange.

Parameters:

    Arbiter - The arbiter being used to arbitrate this requirement.

    State - The current allocation state of the requirement.

Return Value:

    NT Status code.

--*/
{
    BOOLEAN improviseDecode, windowDetected;
    PARBITER_ALTERNATIVE current;
    PDEVICE_OBJECT deviceObject;
    PPCI_DEVICE device;
    ULONGLONG greatestPort;
    ULONG defaultDecode;

    PAGED_CODE();

    UNREFERENCED_PARAMETER(Arbiter);

    TRACE(ARBITER_TRACE_LEVEL, "\nPciIoArbiter_PreprocessEntry for arbiter %p\n", Arbiter);

    deviceObject = State->Entry->PhysicalDeviceObject;
    device = (PPCI_DEVICE)deviceObject->DeviceExtension;

    //
    // If this is a PnP arbitration request for a PCI PDO, make sure the PDO
    // isn't already controlled by a legacy driver.
    //
    if (PCI_DEVICE_OBJECT(deviceObject) &&
        (State->Entry->RequestSource == ArbiterRequestPnpEnumerated)) {

        if (device->LegacyDriver) {

            return STATUS_DEVICE_BUSY;
        }
    }

    if (State->WorkSpace & PORT_ARBITER_PREPROCESSED) {

        //
        // We have already proprocessed this entry so don't repeat the work
        //
        return STATUS_SUCCESS;

    }

    State->WorkSpace = PORT_ARBITER_PREPROCESSED;

    //
    // Scan the alternatives and check if we have set any of the decode flags
    // are set or if we have to improvise.  Also check for any windows.
    //
    improviseDecode = FALSE;
    windowDetected = FALSE;
    greatestPort = 0;
    FOR_ALL_IN_ARRAY(State->Alternatives, State->AlternativeCount, current) {
        PCI_ASSERT(current->Descriptor->Type == CmResourceTypePort);
        PCI_ASSERT(current->Descriptor->Flags == State->Alternatives->Descriptor->Flags);

        //
        // Detect any I/O windows.  For root busses, all I/O resources are
        // windows.  For P-P bridges, only those marked with the window decode
        // flag are.
        //
        if (PCI_DEVICE_OBJECT(deviceObject) &&
            (current->Descriptor->Flags & CM_RESOURCE_PORT_WINDOW_DECODE)) {

            if (current != State->Alternatives) {
                PCI_ASSERT(windowDetected);
            }

            windowDetected = TRUE;
        }

        //
        // See if this requirement doesn't have any of the flags
        // indicating its decode type set.  Remember the maximum
        // value of all the alternatives to try to improvise the
        // decode type.
        //
        if (!(current->Descriptor->Flags &
              (CM_RESOURCE_PORT_10_BIT_DECODE
               | CM_RESOURCE_PORT_12_BIT_DECODE
               | CM_RESOURCE_PORT_16_BIT_DECODE
               | CM_RESOURCE_PORT_POSITIVE_DECODE))) {

            improviseDecode = TRUE;

            if (!LEGACY_REQUEST(State->Entry)) {

                ARB_PRINT(0,
                          ("Pnp device (%p) did not specify decodes for IO ports\n",
                           State->Entry->PhysicalDeviceObject
                          ));
            }
        }

        if (current->Maximum > greatestPort) {
            greatestPort = current->Maximum;
        }
    }

    if (improviseDecode) {

        State->WorkSpace |= PORT_ARBITER_IMPROVISED_DECODE;

        ARB_PRINT(1, ("Improvising decode "));

        switch (State->Entry->InterfaceType) {
        case PNPISABus:
        case Isa:

            //
            // If any of the ports is greater than can be decoded in 10 bits
            // assume a 16 bit decode
            //
            if (greatestPort > MAX_10_BIT_DECODE) {
                defaultDecode = CM_RESOURCE_PORT_16_BIT_DECODE;
                ARB_PRINT(1, ("of 16bit for Isa with ports > 0x3FF\n"));
            } else {
                defaultDecode = CM_RESOURCE_PORT_10_BIT_DECODE;
                ARB_PRINT(1, ("of 10bit for Isa\n"));
            }
            break;

        case Eisa:
        case MicroChannel:
        case PCMCIABus:
            ARB_PRINT(1, ("of 16bit for Eisa/MicroChannel/Pcmcia\n"));
            defaultDecode = CM_RESOURCE_PORT_16_BIT_DECODE;
            break;

        case PCIBus:
            ARB_PRINT(1, ("of positive for PCI\n"));
            defaultDecode = CM_RESOURCE_PORT_POSITIVE_DECODE;
            break;

        default:

            //
            // In NT < 5 we considered everything to be 16 bit decode so in the
            // absence of better information continue that tradition.
            //
            ARB_PRINT(1, ("of 16bit for unknown bus\n"));

            defaultDecode = CM_RESOURCE_PORT_16_BIT_DECODE;
            break;
        }

        FOR_ALL_IN_ARRAY(State->Alternatives, State->AlternativeCount, current) {

            current->Descriptor->Flags |= defaultDecode;
        }

    } else {

        //
        // Even if we are not improvising decodes make sure that they didn't
        // report 10 bit decode for a range over 0x3FF - if so we assume 16 bit
        // decode - think EISA net cards that say they're ISA
        //
        FOR_ALL_IN_ARRAY(State->Alternatives, State->AlternativeCount, current) {

            if ((current->Descriptor->Flags & CM_RESOURCE_PORT_10_BIT_DECODE) &&
                (greatestPort > MAX_10_BIT_DECODE )) {

                current->Descriptor->Flags &= ~CM_RESOURCE_PORT_10_BIT_DECODE;
                current->Descriptor->Flags |= CM_RESOURCE_PORT_16_BIT_DECODE;
            }
        }
    }

    //
    // If we detected a bridge window then check if the ISA bit is set on the bridge
    //
    if (windowDetected) {

        //
        // Make sure its a PCI bridge...
        //

        if ((device->HeaderType != PCI_BRIDGE_TYPE) &&
            (device->HeaderType != PCI_CARDBUS_BRIDGE_TYPE)) {
            
            ERROR("PciIoArbiter_PreprocessEntry: unexpected header type.\n");
            return STATUS_INVALID_PARAMETER;
        }

        if (device->SavedRegisters.bridge.BridgeContro