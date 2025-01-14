   "), (UINT32)aFrequencies[u]);
    }
    SLOG(eInfo1, szHeader);
    SLOG(eInfo1, "");

    //
    // Print table
    for (u=0; u < u32MatrixDimension; u++)
    {
        TCHAR  szRow[256]={0};
        TCHAR* pszRow = szRow;
        pszRow +=_stprintf(pszRow, TEXT("%6ld          "), (UINT32)aTestMatrix[u][0].finputSR);
        for (v=0; v < u32MatrixDimension; v++)
        {
            pszRow +=_stprintf(pszRow, TEXT("%5.1lf          "), aTestMatrix[u][v].dSNR);
        }
        SLOG(eInfo1, szRow);
        SLOG(eInfo1, "");
    }
    SLOG(eInfo1, "----------------------------------------------------------------------------------------");

    return FNS_PASS;
}

DWORD Test_FixedFormatConvertor_SNR16(SRC_ALGORITHM_TYPE SrcType)
{
    SLOG(eInfo1, "Create a graph Mem-->32-->Fmt-->16-->SRC-->Fmt-->16-->Mem");
    SLOG(eInfo1, "The test will determine the SNR of the converter for %s quality (16 bit)", (SrcType==SRC_QUALITY_LINEAR) ? "LINEAR" : "OTHER");
	UINT nChannels=1;

	//
    // Input/output formats
    AUDIO_FRAME_FORMAT InputFormat, OutputFormat, InputFormat1, OutputFormat1;

	//
    // Structure used for test pairs
    typedef struct _SRPAIR
    {
        float  finputSR, foutputSR, fFrequency;
        double dSNR;
    } SRPAIR;

    UINT32 u,v;

    //
    // Initialize test matrix
    float aFrequencies[] = { 8000, 15999, 16000, 11025, 22050, 31999, 32000, 44100, 48000 }; 
    const UINT32 u32MatrixDimension = sizeof(aFrequencies)/sizeof(aFrequencies[0]);
    SRPAIR  aTestMatrix[u32MatrixDimension][u32MatrixDimension];
    for (u=0; u < u32MatrixDimension; u++)
        for (v=0; v < u32MatrixDimension; v++)
        {
            aTestMatrix[u][v].finputSR  = aFrequencies[u];
            aTestMatrix[u][v].foutputSR = aFrequencies[v];
            aTestMatrix[u][v].fFrequency= 997.00;
            aTestMatrix[u][v].dSNR= 0;
        }
    


    for (u=0; u < u32MatrixDimension; u++)
        for (v=0; v < u32MatrixDimension; v++)
        {
            // create input out/formats
            BUILD_FORMAT(&InputFormat,
                        KSDATAFORMAT_SUBTYPE_PCM, 
                        1,
                        2,
                        16,
                        aTestMatrix[u][v].finputSR);

            BUILD_FORMAT(&InputFormat1,
                        KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, 
                        1,
                        4,
                        32,
                        aTestMatrix[u][v].finputSR);

            BUILD_FORMAT(&OutputFormat1,
                        KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, 
                        1,
                        4,
                        32,
                        aTestMatrix[u][v].foutputSR);

            BUILD_FORMAT(&OutputFormat,
                        KSDATAFORMAT_SUBTYPE_PCM, 
                        1,
                        2,
                        16,
                        aTestMatrix[u][v].foutputSR);

            // create graph descriptors
            GRAPH_APO_DESCRIPTOR aApoDescriptors[] = { { APO_FMT_i16f32, NULL } , { APO_SRC , NULL }, { APO_FMT_f32i16, NULL } };
            GRAPH_BINDING_DESCRIPTOR aBindingDescriptors[] = { 
                { 0, 0, true  }, 
                { 0, 1, false },
                { 1, 1, true  },
                { 1, 2, false },
                { 2, 2, true  },
                { 2, 3, false } };
            GRAPH_CONNECTION_DESCRIPTOR aConnectionDescriptors[4]; 
            
			ZeroMemory(aConnectionDescriptors, sizeof(aConnectionDescriptors));
            CopyMemory(&aConnectionDescriptors[0].FrameFormat, &InputFormat, sizeof(InputFormat)); 
            CopyMemory(&aConnectionDescriptors[1].FrameFormat, &InputFormat1, sizeof(InputFormat)); 
            CopyMemory(&aConnectionDescriptors[2].FrameFormat, &OutputFormat1, sizeof(OutputFormat)); 
            CopyMemory(&aConnectionDescriptors[3].FrameFormat, &OutputFormat, sizeof(OutputFormat)); 

			//
			// Always pushmode
			UINT32 u32Ms10 = (UINT32)(InputFormat.f32FramesPerSecond/100.00 + 0.5);
	        aConnectionDescriptors[0].ConnectionDescriptor.u32MaxFrameCount =  u32Ms10; 
			aConnectionDescriptors[1].ConnectionDescriptor.u32MaxFrameCount =  u32Ms10; 
			aConnectionDescriptors[2].ConnectionDescriptor.u32MaxFrameCount =  u32Ms10*24 + 4;

            aConnectionDescriptors[0].ConnectionDescriptor.u32MaxFrameCount =  u32Ms10;
            aConnectionDescriptors[1].ConnectionDescriptor.u32MaxFrameCount =  u32Ms10; 
            aConnectionDescriptors[2].ConnectionDescriptor.u32MaxFrameCount =  u32Ms10*24 + 4; 
            aConnectionDescriptors[3].ConnectionDescriptor.u32MaxFrameCount =  u32Ms10*24 + 4; 
            
			//aConnectionDescriptors[1].ConnectionDescriptor.u32ExtraFrameCount =  4; 

            SLOG(eInfo1, "");
            SLOG(eInfo1, "");
            SLOG(eInfo1, "*********************************************************************************************");
            SLOG(eInfo1, "Testing conversion %.5lf --> %.5lf", aTestMatrix[u][v].finputSR, aTestMatrix[u][v].foutputSR);
            SLOG(eInfo1, "*********************************************************************************************");
            SLOG(eInfo1, "");
            SLOG(eInfo1, "");

            HRESULT hr;


            // In this case we run always in the push model which means that
            // the output buffer size is constant and the input frames will vary
            CGraphRepresentation graph(aConnectionDescriptors,
                                    sizeof(aConnectionDescriptors)/sizeof(aConnectionDescriptors[0]), 
                                    aBindingDescriptors,
                                    sizeof(aBindingDescriptors)/sizeof(aBindingDescriptors[0]), 
                                    aApoDescriptors,
                                    sizeof(aApoDescriptors)/sizeof(aApoDescriptors[0]),                                     
                                    (UINT32)InputFormat.f32FramesPerSecond*3,  // process 2 seconds
                                    (UINT32)OutputFormat.f32FramesPerSecond*3, // process 3 seconds
                                    NULL,                                      // use output memory endpoint
                                    aTestMatrix[u][v].fFrequency
                                    );

            // Change SRC type
            graph.SetSRCAlgorithm(SrcType);
            graph.SetSRCFlow(AUDIO_FLOW_PUSH);

            if (FAILED(hr = graph.BuildGraph()))
            {
                SLOG(eError, "ERROR: Can't build graph");
                return FNS_FAIL;
            }

            MFTIME mfTime = 0;
            MFTIME mfPeriod = (MFTIMEFREQUENCY/1000)*10; // 10 ms per clock

            // start the endpoints
            graph.StartEndpoints();
            graph.StartEndpoints(NULL, true);

            CComPtr<IAudioMemoryEndpointEx> pMemoryInputEndpoint;
            CComPtr<IAudioMemoryEndpointEx> pMemoryOutputEndpoint;

            // this will always work
            graph.GetEndpoint(true)->QueryInterface(__uuidof(IAudioMemoryEndpointEx), (LPVOID*)&pMemoryInputEndpoint);  
            graph.GetEndpoint(false)->QueryInterface(__uuidof(IAudioMemoryEndpointEx), (LPVOID*)&pMemoryOutputEndpoint);  

			// set the input endpoint in push mode because the processor only supports
			// pull mode
//			SLOG(eInfo1, "PUSH'ing %d frames trough the graph each time", u32Ms10/10);
//			pMemoryInputEndpoint->SetPushMode(u32Ms10/10);

			UINT32 u32InputPosition=0;
			UINT32 u32OutputPosition=0;

	    // we need this to get rid of as assert in the processor 
	    // we get this assert because the processor will still treat the
	    // the graph as a pull graph and we don't have enough space in our connections
	    // for both pull and push
	    mfPeriod/=10;

            // run until no more data
            while (1)  
            {
				//
				// check that we have enough input space
				if (u32InputPosition + (UINT32)2*u32Ms10*nChannels*InputFormat.f32FramesPerSecond/1000 >=
					nChannels*InputFormat.f32FramesPerSecond*3*2)
				{
					break;
				}
			
				u32InputPosition  = pMemoryInputEndpoint->GetPosition();
				u32OutputPosition = pMemoryOutputEndpoint->GetPosition();
//				SLOG(eInfo1, "Pos input = %ld, Pos output = %ld", u32InputPosition, u32OutputPosition);
                graph.Step(&mfTime, &mfPeriod); 
            }

			u32InputPosition  = pMemoryInputEndpoint->GetPosition();
            u32OutputPosition = pMemoryOutputEndpoint->GetPosition();

            WAVEFORMATEX wfx;
            FORMAT2WAVEXIEEE(&OutputFormat, &wfx);

			//
            // validate samples in the output buffer
            UINT32 nAvailableFrames = u32OutputPosition/wfx.nBlockAlign;
            UINT32 nOffset = (UINT32)OutputFormat.f32FramesPerSecond/2;
            UINT32 nFrames = (UINT32)OutputFormat.f32FramesPerSecond;
            if (nAvailableFrames < 3*OutputFormat.f32FramesPerSecond/2)
            {
                SLOG(eError, "ERROR: Not enough output frames - expected ~2s");
                return FNS_FAIL;
            }

            // design filter
            float fNormalFrequency = aTestMatrix[u][v].fFrequency*2/OutputFormat.f32FramesPerSecond; 
			// this means that the old frequency should be completely attenuated!
			// in oher words we should have only noise in the signal meaning that
			// we can pick any frequency and get ~0dB Snr
			// in the case were the frequency is aliased back we should get a negative Snr
			if (fNormalFrequency >= 1)
			{
				float f = fNormalFrequency;
				while (f>=1) f-=1;
				fNormalFrequency= f;
			}
            double dSNR=0;
            double* pf64Filter=NULL;
            KAISER_INFO kinfo;
            if (!KaiserNotchDesign(fNormalFrequency, wfx.wBitsPerSample, &pf64Filter, kinfo))
            {
                SLOG(eError, "INTERNAL ERROR: Out of memory of filter design bug");
                return FNS_PENDING;
            }

#ifdef _DEBUG
            KaiserDumpText(L"C:\\MATLABR11\\WORK\\InputBuffer.m", 
                           (short*)graph.GetInputData(), 
                           (UINT32)1024);
            KaiserDumpText(L"C:\\MATLABR11\\WORK\\OutputBuffer.m", 
                           (short*)(((LPBYTE)graph.GetOutputData()) + nOffset*wfx.nBlockAlign), 
                           (UINT32)2048);
            KaiserDumpText(L"C:\\MATLABR11\\WORK\\KaiserDesign.m", 
                           (double*)pf64Filter, 
                           kinfo.M + 1);
#endif

            // calculate the SNR
            if (fir_snr(((LPBYTE)graph.GetOutputData()) + nOffset*wfx.nBlockAlign, // data to analyze
                        nFrames, // number of frames to analyze
                        &wfx,    // data type
                        pf64Filter,  // filter to use
                        kinfo.M + 1, // number of taps
                        &dSNR))
            {
                SLOG(eInfo1, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
                SLOG(eInfo1, "");
                SLOG(eInfo1, "SNR calculated in time domain: ^^ %4.2lf ^^", dSNR);
                SLOG(eInfo1, "");
                SLOG(eInfo1, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");

                aTestMatrix[u][v].dSNR = dSNR;
            }
        }

    //
    // Print headers 
    TCHAR  szHeader[256];
    memset(szHeader, TEXT(' '), sizeof(TCHAR)*256);
    TCHAR* pszHeader = szHeader + 16;
    
    SLOG(eInfo1, "----------------------------------------------------------------------------------------");
    for (u=0; u < u32MatrixDimension; u++)
    {
        pszHeader +=_stprintf(pszHeader, TEXT("%6ld          "), (UINT32)aFrequencies[u]);
    }
    SLOG(eInfo1, szHeader);
    SLOG(eInfo1, "");

    //
    // Print table
    for (u=0; u < u32MatrixDimension; u++)
    {
        TCHAR  szRow[256]={0};
        TCHAR* pszRow = szRow;
        pszRow +=_stprintf(pszRow, TEXT("%6ld          "), (UINT32)aTestMatrix[u][0].finputSR);
        for (v=0; v < u32MatrixDimension; v++)
        {
            pszRow +=_stprintf(pszRow, TEXT("%5.1lf          "), aTestMatrix[u][v].dSNR);
        }
        SLOG(eInfo1, szRow);
        SLOG(eInfo1, "");
    }
    SLOG(eInfo1, "----------------------------------------------------------------------------------------");

    return FNS_PASS;
} // Test_FixedFormatConvertor_SNR16


DWORD Test_FixedFormatConvertor_SNR32_KMIXER(BOOL bUseVariableFrequency=false)
{
    SLOG(eInfo1, "Create a graph Mem-->32-->Mixer-->KSRC-->-->32-->Mem");
 
    //
    // Input/output formats
    AUDIO_FRAME_FORMAT InputFormat, OutputFormat;

    //
    // Structure used for test pairs
    typedef struct _SRPAIR
    {
        float  finputSR, foutputSR, fFrequency;
        double dSNR;
    } SRPAIR;

    UINT32 u,v;

    //
    // Initialize test matrix
    float aFrequencies[] = { 8000, 16000, 11025, 22050, 32000, 44100, 48000, 96000 }; 
    UINT32 u32MatrixDimension = sizeof(aFrequencies)/sizeof(aFrequencies[0]);
    SRPAIR  aTestMatrix[MAX_RATES][MAX_RATES];
    for (u=0; u < u32MatrixDimension; u++)
        for (v=0; v < u32MatrixDimension; v++)
        {
            aTestMatrix[u][v].finputSR  = aFrequencies[u];
            aTestMatrix[u][v].foutputSR = aFrequencies[v];
			if (bUseVariableFrequency)
			{
				// always use a frequency near input signal Nyquist
				// note that if we downsample we expect the SNR to be zero
				// otherwise as good as possible
	            aTestMatrix[u][v].fFrequency= min(20000, min(aTestMatrix[u][v].finputSR/2,aTestMatrix[u][v].foutputSR/2))  - 1000;
			}
			else
	            aTestMatrix[u][v].fFrequency= 997.00;
            aTestMatrix[u][v].dSNR= 0;
        }
    


    //
    // For each pair
    for (u=0; u < u32MatrixDimension; u++)
        for (v=0; v < u32MatrixDimension; v++)
        {
            // create input out/formats
            BUILD_FORMAT(&InputFormat,
                        KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, 
                        1,
                        4,
                        32,
                        aTestMatrix[u][v].finputSR);

            BUILD_FORMAT(&OutputFormat,
                        KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, 
                        1,
                        4,
                        32,
                        aTestMatrix[u][v].foutputSR);

            // create graph descriptors
            GRAPH_APO_DESCRIPTOR aApoDescriptors[] = { { APO_MIXER, NULL } , { APO_KMIXER , NULL } };
            GRAPH_BINDING_DESCRIPTOR aBindingDescriptors[] = { 
                { 0, 0, true  }, 
                { 0, 1, false },
                { 1, 1, true  },
                { 1, 2, false } };
            GRAPH_CONNECTION_DESCRIPTOR aConnectionDescriptors[3]; 
            
			ZeroMemory(aConnectionDescriptors, sizeof(aConnectionDescriptors));
            CopyMemory(&aConnectionDescriptors[0].FrameFormat, &InputFormat, sizeof(InputFormat)); 
            CopyMemory(&aConnectionDescriptors[1].FrameFormat, &InputFormat, sizeof(InputFormat)); 
            CopyMemory(&aConnectionDescriptors[2].FrameFormat, &OutputFormat, sizeof(OutputFormat)); 
            // all the connection buffers will have 20 ms of data 
            aConnectionDescriptors[0].ConnectionDescriptor.u32MaxFrameCount =  2*(UINT32)InputFormat.f32FramesPerSecond/100; 
            aConnectionDescriptors[1].ConnectionDescriptor.u32MaxFrameCount =  2*(UINT32)InputFormat.f32FramesPerSecond/100; 
            aConnectionDescriptors[2].ConnectionDescriptor.u32MaxFrameCount =  2*(UINT32)OutputFormat.f32FramesPerSecond/100; 
            //aConnectionDescriptors[1].ConnectionDescriptor.u32ExtraFrameCount =  4; 
            //aConnectionDescriptors[2].ConnectionDescriptor.u32ExtraFrameCount =  0; 

            SLOG(eInfo1, "");
            SLOG(eInfo1, "");
            SLOG(eInfo1, "*********************************************************************************************");
            SLOG(eInfo1, "Testing conversion %.5lf --> %.5lf", aTestMatrix[u][v].finputSR, aTestMatrix[u][v].foutputSR);
            SLOG(eInfo1, "*********************************************************************************************");
            SLOG(eInfo1, "");
            SLOG(eInfo1, "");

            HRESULT hr;


            // In this case we run always in the push model which means that
            // the output buffer size is constant and the input frames will vary
            CGraphRepresentation graph(aConnectionDescriptors,
                                    sizeof(aConnectionDescriptors)/sizeof(aConnectionDescriptors[0]), 
                                    aBindingDescriptors,
                                    sizeof(aBindingDescriptors)/sizeof(aBindingDescriptors[0]), 
                                    aApoDescriptors,
                                    sizeof(aApoDescriptors)/sizeof(aApoDescriptors[0]),                                     
                                    (UINT32)InputFormat.f32FramesPerSecond*2,  // process 2 seconds
                                    (UINT32)OutputFormat.f32FramesPerSecond*2, // process 3 seconds
                                    NULL,                                      // use output memory endpoint
                                    aTestMatrix[u][v].fFrequency
                                    );


            if (FAILED(hr = graph.BuildGraph()))
            {
                SLOG(eError, "ERROR: Can't build graph");
                return FNS_FAIL;
            }

			//
            // Start the endpoints
            graph.StartEndpoints();
            graph.StartEndpoints(NULL, true);

            MFTIME mfTime = 0;
            MFTIME mfPeriod = (MFTIMEFREQUENCY/1000)*10; // 10 ms per clock

            // run until no more data
            while (graph.IsDataAvailable(true) && graph.IsDataAvailable(false))  
            {
                graph.Step(&mfTime, &mfPeriod); 
            }
            CComPtr<IAudioMemoryEndpointEx> pMemoryInputEndpoint;
            CComPtr<IAudioMemoryEndpointEx> pMemoryOutputEndpoint;

            // this will always work
            graph.GetEndpoint(true)->QueryInterface(__uuidof(IAudioMemoryEndpointEx), (LPVOID*)&pMemoryInputEndpoint);  
            graph.GetEndpoint(false)->QueryInterface(__uuidof(IAudioMemoryEndpointEx), (LPVOID*)&pMemoryOutputEndpoint);  

            UINT32 u32InputPosition  = pMemoryInputEndpoint->GetPosition();
            UINT32 u32OutputPosition = pMemoryOutputEndpoint->GetPosition();

            WAVEFORMATEX wfx;
            FORMAT2WAVEXIEEE(&OutputFormat, &wfx);

            // samples in the output buffer
            UINT32 nAvailableFrames = u32OutputPosition/wfx.nBlockAlign;
            UINT32 nOffset = (UINT32)OutputFormat.f32FramesPerSecond/2;
            UINT32 nFrames = (UINT32)OutputFormat.f32FramesPerSecond;
            if (nAvailableFrames < 3*OutputFormat.f32FramesPerSecond/2)
            {
                SLOG(eError, "ERROR: Not enough output frames - expected ~2s");
                return FNS_FAIL;
            }

            // design filter
            float fNormalFrequency = aTestMatrix[u][v].fFrequency*2/OutputFormat.f32FramesPerSecond; 
			// this means that the old frequency should be completely attenuated!
			// in oher words we should have only noise in the signal meaning that
			// we can pick any frequency and get ~0dB Snr
			// in the case were the frequency is aliased back we should get a negative Snr
			if (fNormalFrequency >= 1)
			{
				float f = fNormalFrequency;
				while (f>=1) f-=1;
				fNormalFrequency= f;
			}
            double dSNR=0;
            double* pf64Filter=NULL;
            KAISER_INFO kinfo;
            if (!KaiserNotchDesign(fNormalFrequency, wfx.wBitsPerSample, &pf64Filter, kinfo))
            {
                SLOG(eError, "INTERNAL ERROR: Out of memory or filter design bug");
                return FNS_PENDING;
            }

#ifdef _DEBUG
            KaiserDumpText(L"C:\\Temp\\InputBuffer.m", 
                        (float*)graph.GetInputData(), 
                        (UINT32)1024);
            KaiserDumpText(L"C:\\Temp\\OutputBuffer.m", 
                        (float*)(((LPBYTE)graph.GetOutputData()) + nOffset*wfx.nBlockAlign), 
                        (UINT32)2048);
            KaiserDumpText(L"C:\\Temp\\KaiserDesign.m", 
                        (double*)pf64Filter, 
                        kinfo.M + 1);
#endif

            // calculate the SNR
            if (fir_snr(((LPBYTE)graph.GetOutputData()) + nOffset*wfx.nBlockAlign, // data to analyze
                        nFrames, // number of frames to analyze
                        &wfx,    // data type
                        pf64Filter,  // filter to use
                        kinfo.M + 1, // number of taps
                        &dSNR))
            {
                SLOG(eInfo1, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
                SLOG(eInfo1, "");
                SLOG(eInfo1, "SNR calculated in time domain: ^^ %4.2lf ^^", dSNR);
                SLOG(eInfo1, "");
                SLOG(eInfo1, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");

                aTestMatrix[u][v].dSNR = dSNR;
            }
            else
            {
                SLOG(eError, "INTERNAL ERROR: Out of memory");
                return FNS_PENDING;
            }
        }

    //
    // Print headers 
    TCHAR  szHeader[256];
    memset(szHeader, TEXT(' '), sizeof(TCHAR)*256);
    TCHAR* pszHeader = szHeader + 16;
    
    SLOG(eInfo1, "----------------------------------------------------------------------------------------");
    for (u=0; u < u32MatrixDimension; u++)
    {
        pszHeader +=_stprintf(pszHeader, TEXT("%6ld          "), (UINT32)aFrequencies[u]);
    }
    SLOG(eInfo1, szHeader);
    SLOG(eInfo1, "");

    //
    // Print table
    for (u=0; u < u32MatrixDimension; u++)
    {
        TCHAR  szRow[256]={0};
        TCHAR* pszRow = szRow;
        pszRow +=_stprintf(pszRow, TEXT("%6ld          "), (UINT32)aTestMatrix[u][0].finputSR);
        for (v=0; v < u32MatrixDimension; v++)
        {
            pszRow +=_stprintf(pszRow, TEXT("%5.1lf          "), aTestMatrix[u][v].dSNR);
        }
        SLOG(eInfo1, szRow);
        SLOG(eInfo1, "");
    }
    SLOG(eInfo1, "----------------------------------------------------------------------------------------");

    return FNS_PASS;
} // Test_FixedFormatConvertor_SNR32_KMIXER




//********************************************************************************************
// TEST CASES START HERE
//********************************************************************************************

//
// Mono test Linear push
DWORD Test_FixedFormatConvertor_SNR_Linear32()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_LINEAR);
}

//
// Multichannel test Linear  push
DWORD Test_FixedFormatConvertor_SNR_Linear32_Multi()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_LINEAR, 4, AUDIO_FLOW_PUSH);
}

//
// Mono test Linear pull
DWORD Test_FixedFormatConvertor_SNR_Linear32_Pull()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_LINEAR, 1, AUDIO_FLOW_PULL);
}

//
// Multichannel test Linear pull
DWORD Test_FixedFormatConvertor_SNR_Linear32_Multi_Pull()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_LINEAR, 4, AUDIO_FLOW_PULL);
}

//
// Nyquist test linear mono, push
DWORD Test_FixedFormatConvertor_SNR_Linear32_Nyq()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_LINEAR, 1, AUDIO_FLOW_PUSH, true);
}

//
// 16 bit linear mono, push
DWORD Test_FixedFormatConvertor_SNR_Linear16()
{
    return Test_FixedFormatConvertor_SNR16(SRC_QUALITY_LINEAR);
}

//
// Kmixer mono tests 32 bit only
DWORD Test_FixedFormatConvertor_SNR32_KMIXER_1()
{
	return Test_FixedFormatConvertor_SNR32_KMIXER();
}

DWORD Test_FixedFormatConvertor_SNR32_KMIXER_2()
{
	return Test_FixedFormatConvertor_SNR32_KMIXER(true);
}

//
// Mono test Sinc push
DWORD Test_FixedFormatConvertor_SNR_Sinc1_32()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC1);
}
DWORD Test_FixedFormatConvertor_SNR_Sinc2_32()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC2);
}
DWORD Test_FixedFormatConvertor_SNR_Sinc3_32()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC3);
}

//
// 16 bit sinc mono, push
DWORD Test_FixedFormatConvertor_SNR_Sinc1_16()
{
    return Test_FixedFormatConvertor_SNR16(SRC_QUALITY_SINC1);
}
DWORD Test_FixedFormatConvertor_SNR_Sinc2_16()
{
    return Test_FixedFormatConvertor_SNR16(SRC_QUALITY_SINC2);
}
DWORD Test_FixedFormatConvertor_SNR_Sinc3_16()
{
    return Test_FixedFormatConvertor_SNR16(SRC_QUALITY_SINC3);
}

//
// Nyquist test sinc mono, push
DWORD Test_FixedFormatConvertor_SNR_Sinc1_32_Nyq()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC1, 1, AUDIO_FLOW_PUSH, true);
}
DWORD Test_FixedFormatConvertor_SNR_Sinc2_32_Nyq()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC2, 1, AUDIO_FLOW_PUSH, true);
}
DWORD Test_FixedFormatConvertor_SNR_Sinc3_32_Nyq()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC3, 1, AUDIO_FLOW_PUSH, true);
}


//
// Multichannel test sinc  push
DWORD Test_FixedFormatConvertor_SNR_Sinc1_32_Multi()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC1, 4, AUDIO_FLOW_PUSH);
}
DWORD Test_FixedFormatConvertor_SNR_Sinc2_32_Multi()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC2, 4, AUDIO_FLOW_PUSH);
}
DWORD Test_FixedFormatConvertor_SNR_Sinc3_32_Multi()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC3, 4, AUDIO_FLOW_PUSH);
}

//
// Mono test sinc pull
DWORD Test_FixedFormatConvertor_SNR_Sinc1_32_Pull()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC1, 1, AUDIO_FLOW_PULL);
}
DWORD Test_FixedFormatConvertor_SNR_Sinc2_32_Pull()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC2, 1, AUDIO_FLOW_PULL);
}
DWORD Test_FixedFormatConvertor_SNR_Sinc3_32_Pull()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC3, 1, AUDIO_FLOW_PULL);
}

//
// Multichannel test sinc pull
DWORD Test_FixedFormatConvertor_SNR_Sinc1_32_Multi_Pull()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC1, 4, AUDIO_FLOW_PULL);
}
DWORD Test_FixedFormatConvertor_SNR_Sinc2_32_Multi_Pull()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC2, 4, AUDIO_FLOW_PULL);
}
DWORD Test_FixedFormatConvertor_SNR_Sinc3_32_Multi_Pull()
{
    return Test_FixedFormatConvertor_SNR32(SRC_QUALITY_SINC3, 4, AUDIO_FLOW_PULL);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
//  SRC DMO quality testing
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
class CDMOResamplerAttribs
{
	WAVEFORMATEXTENSIBLE	m_wfxInput;
	WAVEFORMATEXTENSIBLE	m_wfxOutput;
	CComPtr<IMediaObject>   m_pDMO;
public:
	CDMOResamplerAttribs( WAVEFORMATEX* pwfxInput, WAVEFORMATEX* pwfxOutput);
	
	//@@ Creates the CMPT SRC DMO and sets it up for processing at the 
	//@@ rates specified in the input and output formats
	HRESULT Initialize();
};

CDMOResamplerAttribs::CDMOResamplerAttribs( WAVEFORMATEX* pwfxInput, WAVEFORMATEX* pwfxOutput ) {
	// copy the input parameters
	_ASSERT( pwfxInput );
	_ASSERT( pwfxOutput );
	_ASSERT( pwfxInput->cbSize + sizeof(WAVEFORMATEX)  <= sizeof(m_wfxInput) );
	CopyMemory( &m_wfxInput, pwfxInput, pwfxInput->cbSize + sizeof(WAVEFORMATEX) );
	_ASSERT( pwfxOutput->cbSize + sizeof(WAVEFORMATEX)  <= sizeof(m_wfxOutput) );
	CopyMemory( &m_wfxOutput, pwfxOutput, pwfxOutput->cbSize + sizeof(WAVEFORMATEX) );
}

HRESULT CDMOResamplerAttribs::Initialize() {
	HRESULT hr = S_OK;
	CComPtr<IWMResamplerProps> pResamplerConfig;
	AM_MEDIA_TYPE              inputMediaType;
	AM_MEDIA_TYPE			   outputMediaType;
	DWORD					   dwInputBufferSize;
	DWORD					   dwInputBufferAlignment;
	DWORD					   dwInputBufferLookahead;
	DWORD					   dwOutputBufferSize;
	DWORD					   dwOutputBufferAlignment;

	// instantiate DMO
	hr = m_pDMO.CoCreateInstance( CLSID_CResamplerMediaObject );

	if ( FAILED(hr) ) {
		SLOG(eError,"ERROR: Can't instantiate the resampler DMO 0x%X", hr);
		goto DONE;
	}

	// configure DMO
	hr = CreateAudioMediaType( &m_wfxInput,  &inputMediaType, FALSE);
	if ( FAILED(hr) ) {
		SLOG(eError,"ERROR: Can't fill media type 0x%X", hr);
		goto DONE;
	}
	hr = CreateAudioMediaType( &m_wfxOutput, &outputMediaType, FALSE);
	if ( FAILED(hr) ) {
		SLOG(eError,"ERROR: Can't fill media type 0x%X", hr);
		goto DONE;
	}

	hr = m_pDMO->SetInputType( 0, &inputMediaType, 0 );
	if ( FAILED(hr) ) {
		SLOG(eError,"ERROR: Failed setting DMO input type 0x%X", hr);
		goto DONE;
	}

	hr = m_pDMO->SetOutputType( 0, &outputMediaType, 0 );
	if ( FAILED(hr) ) {
		SLOG(eError,"ERROR: Failed setting DMO output type 0x%X", hr);
		goto DONE;
	}

	// optimize DMO processing pass
	hr = m_pDMO->AllocateStreamingResources();
	if ( FAILED(hr) ) {
		SLOG(eError,"ERROR: AllocateStreamingResources failed 0x%X", hr);
		goto DONE;
	}

	// get information about DMO's buffer alignment, size and lookahead
	hr = m_pDMO->GetInputSizeInfo( 0, &dwInputBufferSize, &dwInputBufferAlignment, &dwInputBufferLookahead );
	if ( FAILED(hr) ) {
		SLOG(eError,"ERROR: GetInputSizeInfo failed 0x%X", hr);
		goto DONE;
	}

	hr = m_pDMO->GetOutputSizeInfo( 0, &dwOutputBufferSize, &dwInputBufferAlignment );
	if ( FAILED(hr) ) {
		SLOG(eError,"ERROR: GetOutputSizeInfo failed 0x%X", hr);
		goto DONE;
	}

DONE:
	return hr;
}
*/
!include  $(TSTPROJECT_ROOT)\common\etwConsumers\etwConsumers.mk

TARGETNAME         = HybridPerf
TARGETTYPE         = DYNLINK
TARGET_DESTINATION = Graphics\D3D\Conf
DLLDEF             =

USE_ATL     = 1

USE_MSVCRT  = 1
NT_SIGNCODE = 1

C_DEFINES = $(C_DEFINES) -DINLINE_TEST_METHOD_MARKUP /DUNICODE  /D_UNICODE
# -DUNICODE -D_UNICODE

INCLUDES = $(INCLUDES); \
    $(SDKTOOLS_INC_PATH)\WexTest\Cue; \
    $(SDKTOOLS_INC_PATH)\WexTest\Tools; \
    $(WINDOWS_TST_ROOT)\common\etwPostProcessing\XperfHelper\inc; \
    $(WINDOWS_TST_ROOT)\common\etwPostProcessing\DxEtwHelper\inc; \
    $(BASE_VID_TST_ROOT)\Tests\SchTaef\RenderLib\inc; \
    $(WINDOWS_TST_ROOT)\common\etw\inc; \
    $(WINDOWS_TST_ROOT)\common\etwConsumers\inc; \
    $(WINDOWS_TST_ROOT)\common\etwConsumers\etwlib\inc; \
    $(WINDOWS_TST_OBJ_ROOT)\Common\etw\manifest\$O; \


#PRECOMPILED_INCLUDE = stdafx.h
#PRECOMPILED_CXX = 1

SOURCES = \
    Main.cpp \
    kitContent.kml \
    DefaultResource.rc # Autogenerated file name + version for Device Guard whitelisting effort

TARGETLIBS = $(TARGETLIBS) \
    $(SDK_LIB_PATH)\kernel32.lib \
    $(SDK_LIB_PATH)\shlwapi.lib \
    $(SDK_LIB_PATH)\advapi32.lib \
    $(SDKTOOLS_LIB_PATH)\WexTest\Cue\Wex.Common.lib \
    $(SDKTOOLS_LIB_PATH)\WexTest\Cue\Wex.Logger.lib \
    $(SDK_LIB_PATH)\Ntdll.lib \
    $(SDKTOOLS_LIB_PATH)\WexTest\Cue\Te.Common.lib \
    $(WINDOWS_LIB_PATH)\d3dcompiler_test.lib \
    $(SDK_LIB_PATH)\d3d11.lib \
    $(SDK_LIB_PATH)\dxgi.lib \
    $(SDK_LIB_PATH)\d2d1.lib \
    $(WINDOWS_LIB_PATH)\d3dx9.lib \
    $(WINDOWS_TST_COMMON)\etwPostProcessing\XperfHelper\lib\$O\XperfHelper.lib \
    $(WINDOWS_TST_COMMON)\etwPostProcessing\DxEtwHelper\lib\$O\DxEtwHelper.lib \
    $(BASE_VID_TST_OBJ_ROOT)\Tests\SchTaef\RenderLib\lib\$O\workload.lib \
    $(WINDOWS_TST_COMMON)\etwConsumers\etwLib\lib\unicode\$O\etwLibConsumer.lib \
    $(PUBLIC_ROOT)\internal\windows\lib\*\unicode\etw.lib \



# Autogenerated. Sets file name for Device Guard whitelisting effort, used in RC.exe.
C_DEFINES=$(C_DEFINES) -D___TARGETNAME="""$(TARGETNAME).$(TARGETTYPE)"""
MUI_VERIFY_NO_LOC_RESOURCE=1
!include ..\conf.mk

TARGETNAME=PShader

SOURCES=PShader.cpp \
        PShader.rc \
        opmod.cpp \
        address.cpp \
        register.cpp \
        srcmod.cpp \
        dstmod.cpp \
        ReadShader.cpp \
        VertShader.cpp \
        limits.cpp \
	legacy.cpp


SOURCES=$(SOURCES) kitContent.kml

/*++
Copyright (c) Microsoft Corporation

Module Name:

    controller.c

Abstract:

    USB Controller Extension Driver

--*/

#include "pch.h"

#include "controller.tmh"

//
// Global triage data for dbgeng and 0x9F support
//


UCX_TRIAGE_INFO g_UcxTriageInfo = {

    UCX_TRIAGE_INFO_VERSION_1,                                        // Version;
    NULL,                                                                // UcxDriverContext 
    
    // _UCXCONTROLLER_PRIVCONTEXT
    sizeof(UCXCONTROLLER_PRIVCONTEXT),                                // UcxControllerPrivContextSize
    FIELD_OFFSET(UCXCONTROLLER_PRIVCONTEXT, Sig),                     // UcxControllerPrivContext_SigOffset
    FIELD_OFFSET(UCXCONTROLLER_PRIVCONTEXT, UcxControllerListEntry),  // UcxControllerPrivContext_UcxControllerListEntryOffset
    FIELD_OFFSET(UCXCONTROLLER_PRIVCONTEXT, UcxController),           // UcxControllerOffset
    FIELD_OFFSET(UCXCONTROLLER_PRIVCONTEXT, QueueAddress0Ownership),  // UcxControllerPrivContext_QueueAddress0OwnershipOffset
    FIELD_OFFSET(UCXCONTROLLER_PRIVCONTEXT, QueueUsbDeviceMgmt),      // UcxControllerPrivContext_QueueUsbDeviceMgmtOffset
    FIELD_OFFSET(UCXCONTROLLER_PRIVCONTEXT, QueuePendUsbDeviceMgmtDuringControllerReset),
                                                                      // UcxControllerPrivContext_QueuePendUsbDeviceMgmtDuringControllerResetOffset
    FIELD_OFFSET(UCXCONTROLLER_PRIVCONTEXT, QueueTreePurge),          // UcxControllerPrivContext_QueueTreePurgeOffset
    FIELD_OFFSET(UCXCONTROLLER_PRIVCONTEXT, QueueUcxDefault),         // UcxControllerPrivContext_QueueUcxDefaultOffset
    FIELD_OFFSET(UCXCONTROLLER_PRIVCONTEXT, UcxRootHubContext),       // UcxControllerPrivContext_UcxRootHubContextOffset
    FIELD_OFFSET(UCXCONTROLLER_PRIVCONTEXT, SystemThreadQueue),  // UcxControllerPrivContext_SystemThreadQueueOffset,

    // _WDFDRIVER_UCX_CONTEXT
    sizeof(WDFDRIVER_UCX_CONTEXT),                                    // WdfDriverUcxContextSize
    FIELD_OFFSET(WDFDRIVER_UCX_CONTEXT, Sig),                         // WdfDriverUcxContext_SigOffset
    FIELD_OFFSET(WDFDRIVER_UCX_CONTEXT, UcxWdfDevice),                // WdfDriverUcxContext_UcxWdfDeviceOffset
    FIELD_OFFSET(WDFDRIVER_UCX_CONTEXT, UcxControllerListHead),       // WdfDriverUcxContext_ControllerListHeadOffset
    FIELD_OFFSET(WDFDRIVER_UCX_CONTEXT, UcxControllerListCount),      // WdfDriverUcxContext_ControllerListCountOffset

    // UCXROOTHUB_PRIVCONTEXT
    
    sizeof(UCXROOTHUB_PRIVCONTEXT),                                        // UcxRootHubPrivContextSize
    FIELD_OFFSET(UCXROOTHUB_PRIVCONTEXT, Sig),                             // UcxRootHubPrivContext_SigOffset
    FIELD_OFFSET(UCXROOTHUB_PRIVCONTEXT, UcxUsbDeviceContextOfUcxRootHub), // UcxRootHubPrivContext_UcxUsbDeviceContextOfUcxRootHubOffset
    FIELD_OFFSET(UCXROOTHUB_PRIVCONTEXT, UcxInterruptEndpoint),            // UcxRootHubPrivContext_UcxInterruptEndpointOffset
    FIELD_OFFSET(UCXROOTHUB_PRIVCONTEXT, RootHubInterruptQueueContext),    // UcxRootHubPrivContext_RootHubInterruptQueueContextOffset
    FIELD_OFFSET(UCXROOTHUB_PRIVCONTEXT, DefaultQueue),                    // UcxRootHubPrivContext_DefaultQueueOffset
    FIELD_OFFSET(UCXROOTHUB_PRIVCONTEXT, AsyncResetIrpFromHub),            // UcxRootHubPrivContext_AsyncResetIrpFromHubOffset

    // UCXUSBDEVICE_PRIVCONTEXT
    
    sizeof(UCXUSBDEVICE_PRIVCONTEXT),                                             // UcxUsbDevicePrivContextSize
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, Sig),                                  // UcxUsbDevicePrivContext_SigOffset
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, ChildListHead),                        // UcxUsbDevicePrivContext_ChildListHeadOffset
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, ChildListEntry),                       // UcxUsbDevicePrivContext_ChildListEntryOffset
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, EndpointListHead),                     // UcxUsbDevicePrivContext_EndpointListHeadOffset
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, UcxFunctionInfo),                      // UcxUsbDevicePrivContext_UcxFunctionInfoOffset
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, UcxFunctionInfoWdfMemory),             // UcxUsbDevicePrivContext_UcxFunctionInfoWdfMemoryOffset
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, PendingUsbDeviceEnableIrp),            // UcxUsbDevicePrivContext_PendingUsbDeviceEnableIrp
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, PendingUsbDeviceResetIrp),             // UcxUsbDevicePrivContext_PendingUsbDeviceResetIrp
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, PendingUsbDeviceStartIrp),             // UcxUsbDevicePrivContext_PendingUsbDeviceStartIrp
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, PendingUsbDevicePurgeIrp),             // UcxUsbDevicePrivContext_PendingUsbDevicePurgeIrp
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, PendingEndpointsConfigureIrp),         // UcxUsbDevicePrivContext_PendingEndpointsConfigureIrp
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, PendingEndpointsConfigureRequest),     // UcxUsbDevicePrivContext_PendingEndpointsConfigureRequest
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, PendingUsbDeviceResetRequest),         // UcxUsbDevicePrivContext_PendingUsbDeviceResetRequest
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, PendingUsbDeviceDisableRequest),       // UcxUsbDevicePrivContext_PendingUsbDeviceDisableRequest
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, PendingUsbDeviceStartRequest),         // UcxUsbDevicePrivContext_PendingUsbDeviceStartRequest
    FIELD_OFFSET(UCXUSBDEVICE_PRIVCONTEXT, PendingUsbDevicePurgeRequest),         // UcxUsbDevicePrivContext_PendingUsbDevicePurgeRequest	  
    
    // UCX_FORWARD_PROGRESS_WORKITEM
    
    sizeof(UCX_FORWARD_PROGRESS_WORKITEM),                     // UcxForwardProgressWorkitemSize
    FIELD_OFFSET(UCX_FORWARD_PROGRESS_WORKITEM, Signature),    // UcxForwardProgressWorkitem_SignatureOffset
    FIELD_OFFSET(UCX_FORWARD_PROGRESS_WORKITEM, ListEntry),    // UcxForwardProgressWorkitem_ListEntryOffset
    FIELD_OFFSET(UCX_FORWARD_PROGRESS_WORKITEM, WdmWorkItem),  // UcxForwardProgressWorkitem_WdmWorkItemOffset

    // UCXENDPOINT_PRIVCONTEXT
    
    sizeof(UCXENDPOINT_PRIVCONTEXT),                           // UcxEndpointPrivContextSize
    FIELD_OFFSET(UCXENDPOINT_PRIVCONTEXT, Sig),                // UcxEndpointPrivContext_SigOffset
    FIELD_OFFSET(UCXENDPOINT_PRIVCONTEXT, EndpointListEntry),  // UcxEndpointPrivContext_EndpointListEntryOffset
    FIELD_OFFSET(UCXENDPOINT_PRIVCONTEXT, UcxSStreamsContext), // UcxEndpointPrivContext_UcxSStreamsContextOffset

    // UCXSSTREAMS_PRIVCONTEXT
    
    sizeof(UCXSSTREAMS_PRIVCONTEXT),                       // UcxSStreamsPrivContextSize
    FIELD_OFFSET(UCXSSTREAMS_PRIVCONTEXT, Sig),            // UcxSStreamsPrivContext_SigOffset
    FIELD_OFFSET(UCXSSTREAMS_PRIVCONTEXT, UcxSStreams),    // UcxSStreamsPrivContext_UcxSStreamsOffset
    FIELD_OFFSET(UCXSSTREAMS_PRIVCONTEXT, StreamContexts), // UcxSStreamsPrivContext_StreamContextsOffset
    NULL,                                                  // WdfTriageInfoPtr
    &g_WdfDriverUcxContext                                 // GlobalWdfDriverUcxContext
};

VOID
Controller_CsqInsertAbortPipeIrp(
    PIO_CSQ   Csq,
    PIRP      Irp
    )
/*++

Routine Description:

    Inserts an AbortPipe Irp into a cancel safe queue.
    
    We really do not put the Irp into any queue. The purpose of this Csq is just to
    allow Csq to handle cancellation synchronization. We will never scan the queue for 
    a particular Irp.

Arguments:

    UNUSED

--*/
{

    UNREFERENCED_PARAMETER(Csq);
    UNREFERENCED_PARAMETER(Irp);
    return;
}

VOID
Controller_CsqRemoveAbortPipeIrp(
    PIO_CSQ Csq,
    PIRP    Irp
    )
/*++

Routine Description:

    This Routine basically does nothing. Please see description in:
    Controller_CsqInsertAbortPipeIrp
    
Arguments:


--*/
{
    UNREFERENCED_PARAMETER(Csq);
    UNREFERENCED_PARAMETER(Irp);
}

PIRP
Controller_CsqPeekNextAbortPipeIrp(
    PIO_CSQ    Csq,
    PIRP       Irp,
    PVOID      PeekContext
    )
/*++

Routine Description:

    This Routine should never be called. 
    We have this function implemented to call IoCsqInitialize 
    Please see description in:
        Controller_CsqInsertAbortPipeIrp
    
Arguments:


--*/
{
    UNREFERENCED_PARAMETER(Csq);
    UNREFERENCED_PARAMETER(Irp);
    UNREFERENCED_PARAMETER(PeekContext);

    NT_ASSERTMSG("This function should never be called", FALSE);

    return NULL;
}

VOID
Controller_CsqAcquireAbortPipeIrpLock(
     PIO_CSQ Csq,
     PKIRQL  Irql
     )
/*++

Routine Description:

    This Routine acquires the lock for the AbortPipe Csq. 
    
Arguments:
    
    Csq - Pointer to the IO_CSQ for the AbortPipeCsq
    
    Irql - irql value before the lock was acquired.

--*/
#pragma warning (suppress:28167) //  changes the IRQL and does not restore the IRQL before it exits.
{
    PUCXCONTROLLER_PRIVCONTEXT        ucxControllerContext;

    TRY {

        ucxControllerContext = (PUCXCONTROLLER_PRIVCONTEXT)CONTAINING_RECORD(
            Csq,
            UCXCONTROLLER_PRIVCONTEXT,
            AbortPipeIrpCsq);

        KeAcquireSpinLock(&ucxControllerContext->AbortPipeIrpCsqLock, Irql);

    } FINALLY {

    }
   
}

VOID
Controller_CsqReleaseAbortPipeIrpLock(
     PIO_CSQ Csq,
     KIRQL   Irql
     )
/*++

Routine Description:

    This Routine Releases the lock for the AbortPipe Csq. 
    
Arguments:
    
    Csq - Pointer to the IO_CSQ for the AbortPipeCsq
    
    Irql - irql value to be restored after releasing the lock.

--*/
#pragma warning (suppress:28167) //  changes the IRQL and does not restore the IRQL before it exits.
{
    PUCXCONTROLLER_PRIVCONTEXT        ucxControllerContext;

    TRY {

        ucxControllerContext = (PUCXCONTROLLER_PRIVCONTEXT)CONTAINING_RECORD(
            Csq,
            UCXCONTROLLER_PRIVCONTEXT,
            AbortPipeIrpCsq);

        KeReleaseSpinLock(&ucxControllerContext->AbortPipeIrpCsqLock, Irql);

    } FINALLY {

    }
   
}

VOID
Controller_CsqCompleteCancelledAbortPipeIrp(
    PIO_CSQ Csq,
    PIRP    Irp
    )
/*++

Routine Description:

    This Routine completes a cancelled AbortPipe Irp 
    
Arguments:
    
    Csq - Pointer to the IO_CSQ for the AbortPipeCsq
    
    Irp - Cancelled Irp

--*/
{
    PIO_STACK_LOCATION irpStack;
    PURB               urb;

    UNREFERENCED_PARAMETER(Csq);
    
    TRY {
        
        irpStack = IoGetCurrentIrpStackLocation(Irp);
        urb = (PURB)irpStack->Parameters.Others.Argument1;

        //
        // COMPAT:  Using same value as returned by USBPORT_Core_CompleteAbortRequest
        // 
        // Also, cancelled Abort Urb is completed by the 2.0 stack at
        // DISPATCH_LEVEL explicitly
        //

        Xrb_IoCompleteRequest_OnDispatchIfNonXrb(Irp, urb, STATUS_CANCELLED, USBD_STATUS_DEVICE_GONE);

    } FINALLY {

    }
}

NTSTATUS 
UcxControllerGetDeviceCapabilities(
    __in 
        WDFDEVICE               Device,
    __out 
        PDEVICE_CAPABILITIES    Capabilities
    )
/*++

Routine Description:

    Function retrieves the DEVICE_CAPABILITIES descriptor from the device

Arguments:

    Device - handle to device 

    Capabilities - device capabilities

Return Value:

    NT status code

--*/
{
    WDFREQUEST                  request;
    WDFIOTARGET                 target;
    WDF_REQUEST_SEND_OPTIONS    options;
    WDF_REQUEST_REUSE_PARAMS    reuse;
    IO_STACK_LOCATION           stack;        
    NTSTATUS                    status;    

    TRY {

        request = NULL;
        target = WdfDeviceGetIoTarget(Device);

        status = WdfRequestCreate(WDF_NO_OBJECT_ATTRIBUTES, target, &request);

        if (!NT_SUCCESS(status)) {
            LEAVE;
        }

        //
        // All pnp IRPs must be initialized with STATUS_NOT_SUPPORTED
        //
        WDF_REQUEST_REUSE_PARAMS_INIT(&reuse, WDF_REQUEST_REUSE_NO_FLAGS, STATUS_NOT_SUPPORTED);
        WdfRequestReuse(request, &reuse);

        //        
        // Initialize device capabilities according to the DDK docs
        //
        RtlZeroMemory(Capabilities, sizeof(DEVICE_CAPABILITIES));
        Capabilities->Size = sizeof(DEVICE_CAPABILITIES);
        Capabilities->Version  =  1;
        Capabilities->Address  = (ULONG) -1;
        Capabilities->UINumber = (ULONG) -1;

        RtlZeroMemory(&stack, sizeof(stack));
        stack.MajorFunction = IRP_MJ_PNP;
        stack.MinorFunction = IRP_MN_QUERY_CAPABILITIES;
        stack.Parameters.DeviceCapabilities.Capabilities = Capabilities;

        WdfRequestWdmFormatUsingStackLocation(request, &stack);

        WDF_REQUEST_SEND_OPTIONS_INIT(&options, WDF_REQUEST_SEND_OPTION_SYNCHRONOUS);

        if (FALSE == WdfRequestSend(request, target, &options)){
            //
            // No Error handling is needed here. The only reason the 
            // if statuement exists in this case is that WdfRequestSend has an 
            // incorrect _Must_inspect_result_ annotataion. If WDF_REQUEST_SEND_OPTION_SYNCHRONOUS
            // is used, there is no need to check the return value of WdfRequestSend.
            //
            NT_ASSERTMSG("WdfRequestSend failed", FALSE);
        }

        status = WdfRequestGetStatus(request);

        if (!NT_SUCCESS(status)) {
            LEAVE;
        }
        
    } FINALLY {

        if (request != NULL) {
            WdfObjectDelete(request);
        }

    }

    return status;

}

_Must_inspect_result_
__drv_requiresIRQL(PASSIVE_LEVEL)
NTSTATUS
EXPORT(UcxControllerCreate)(
    __in_opt 
        PUCX_GLOBALS                  Globals,
    __in 
        WDFDEVICE                     Device,
    __in 
        PUCX_CONTROLLER_CONFIG        Config,
    __in_opt 
        PWDF_OBJECT_ATTRIBUTES        Attributes,
    __out 
        UCXCONTROLLER                *Controller
    )
/*++

Routine Description:

    This export function creates the controller  object 
    
Arguments:

    Globals - Driver globals of the controller driver. This information is not
        being used. 
    
    Device - The handle to the WDFDEVICE
    
    Config - Pointer to a config structure describing various configuration
        information for the controller creation.
    
    Attributes - Pointer to the object attributes structure 
    
    Controller - Output - Pointer to a locatin that will recieve a handle to the 
        controller object

Return Value:

    NT_SUCCESS status if a controller object is  successfully created
    
--*/
{
    NTSTATUS                                    status;
    UCXCONTROLLER                               ucxController = NULL;
    PUCXCONTROLLER_PRIVCONTEXT                  ucxControllerContext = NULL;
    
    WDF_OBJECT_ATTRIBUTES                       objectAttributes;
    PWDF_OBJECT_ATTRIBUTES                      objectAttributes1;
    PWDF_OBJECT_ATTRIBUTES                      objectAttributes2;

    WDF_IO_QUEUE_CONFIG                         queueConfig;
    WDFQUEUE                                    queueAddress0Ownership;
    WDFQUEUE                                    queueUsbDeviceMgmt;
    WDFQUEUE                                    queueTreePurge;
    WDFQUEUE                                    queuePendUsbDeviceMgmtDuringControllerReset;
    WDFQUEUE                                    queueUcxDefault;
    WDF_IO_QUEUE_FORWARD_PROGRESS_POLICY        queueForwardProgressPolicy;
    PADDRESS0_OWNERSHIP_QUEUE_CONTEXT           address0OwnershipQueueContext;
    PWDFDEVICE_UCX_CONTEXT                      wdfDeviceUcxContext;
    WDFSTRING                                   deviceInterfaceString;
    KIRQL                                       irql;
    PDEVICE_OBJECT                              deviceObject;

    TRY {
        //
        // Parameter Validation
        //
        
        if (NULL == Controller) {
            status = STATUS_INVALID_PARAMETER;
            LogError(FlagController, "Controller pointer cannot be NULL");
            LEAVE;
        }

        if (NULL == Config) {
            status = STATUS_INVALID_PARAMETER;
            LogError(FlagController, "Config pointer cannot be NULL");
            LEAVE;
        }
        
        if (Config->Size != sizeof(UCX_CONTROLLER_CONFIG)) {
            status = STATUS_INVALID_PARAMETER;
            LogError(FlagController, "Config structure size is %d, expected Max %d", 
                     Config->Size, sizeof(UCX_CONTROLLER_CONFIG) );
            LEAVE;
        }

        if (((NULL != Config->EvtControllerGetDumpData) && (NULL == Config->EvtControllerFreeDumpData)) ||
            ((NULL == Config->EvtControllerGetDumpData) && (NULL != Config->EvtControllerFreeDumpData))) { 
            status = STATUS_INVALID_PARAMETER;
            LogError(FlagController, "Config %p: Both or none of GetDumpData/FreeDumpData should be NULL", 
                     Config);
            LEAVE;
        }

        if (Attributes != WDF_NO_OBJECT_ATTRIBUTES) {

            //
            // As per the interface definition, the UCXCONTROLLER has to be
            // parented to the WDFDEVICE, and the controller driver must not 
            // set the ParentObject value.
            //
            if (Attributes->ParentObject != NULL) {
                status = STATUS_INVALID_PARAMETER;
                LogError(FlagController, "Attributes structures has ParentObject set"); 
                LEAVE;
            }
        }

        //
        // Add a UCX Specific Context for the WDF Device, so that we can 
        // track the UCXCONTROLLER and UCXROOTHUB associated with the WDFDEVICE
        //

        WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&objectAttributes, WDFDEVICE_UCX_CONTEXT);

        status = WdfObjectAllocateContext(Device, &objectAttributes, &wdfDeviceUcxContext);
        if (!NT_SUCCESS(status)) {
            LogError(FlagController, "Could not add WDFDEVICE_UCX_CONTEXT to WDFDEVICE, %!STATUS!", 
                     status);
            LEAVE;
        }

        wdfDeviceUcxContext->Sig = SigWdfDeviceUcxContext;

        //
        // Create ucxcontroller object, using the Attributes given to us by 
        // the controller driver to us. If the controller driver didnt give us
        // the attributes, use your own attributes.
        //
        
        *Controller = NULL;

        WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&objectAttributes,
                                                UCXCONTROLLER_PRIVCONTEXT);
        objectAttributes.EvtDestroyCallback = Controller_EvtDestroyCallback;

        if (Attributes != WDF_NO_OBJECT_ATTRIBUTES) {
            
      
            objectAttributes1 = Attributes;
            objectAttributes2 = &objectAttributes;
        
        } else {

            objectAttributes1 = &objectAttributes;
            objectAttributes2 = WDF_NO_OBJECT_ATTRIBUTES;

        }

        //
        // UCXCONTROLLER is to be parented to the WDFDEVICE
        //
        objectAttributes1->ParentObject = Device;

        //
        // We need our Destroy callback on Passive. 
        //
        objectAttributes1->ExecutionLevel = WdfExecutionLevelPassive;
        
        status = Ucx_WdfObjectCreateWith2Attributes(objectAttributes1, 
                                                    objectAttributes2,
                                                    &ucxController);
        
        if (!NT_SUCCESS(status)) { 
            LogError(FlagController, "Ucx_WdfObjectCreateWith2Attributes Failed %!STATUS!", status);
            LEAVE;
        }

        ucxControllerContext = UcxControllerGetContext(ucxController);

        //
        // Initialize and populate UcxControllerContext 
        //
        ucxControllerContext->Sig = SigUcxControllerContext;
        
        ucxControllerContext->ParentWdfDevice = Device;

        ucxControllerContext->UcxTriageInfo = &g_UcxTriageInfo;

        if (g_UcxTriageInfo.UcxDriverContext == NULL) {
            g_UcxTriageInfo.UcxDriverContext = &g_WdfDriverUcxContext;
        }

        //
        // Update the global WDF Triage info structure
        //
        g_UcxTriageInfo.WdfTriageInfoPtr = (PWDF_TRIAGE_INFO) WdfGetTriageInfo();

        InitializeListEntry(&ucxControllerContext->UcxControllerListEntry);

        ucxControllerContext->UcxController = ucxController;

        RtlCopyMemory(&ucxControllerContext->Config, 
                      Config,
                      Config->Size);

        wdfDeviceUcxContext->UcxControllerContext = ucxControllerContext;

        LogInfo(FlagController, "UcxController 0x%p, UcxControllerContext 0x%p", 
                ucxController, ucxControllerContext);

        KeInitializeSpinLock(&ucxControllerContext->TopologyLock);
        ucxControllerContext->ChildListCount = 0;
        ucxControllerContext->ChildEndpointListCount = 0;

        status = StateMachineEngine_Init(ucxControllerContext,
                                         &ucxControllerContext->SmEngineContext,
                                         ucxController,
                                         ucxControllerContext,
                                         ControllerResetStateRhPdoInDxIndex,
                                         ControllerResetStateTable,
                                         Etw_UcxControllerStateMachineEvent,
                                         Etw_UcxControllerStateMachineTransition);
        
        if (!NT_SUCCESS(status)) {
            LogError(FlagController, "StateMachineEngine_Init Failed, ucxControllerContext 0x%p, %!STATUS!", 
                     ucxControllerContext, status);
            LEAVE;
        }

        ucxControllerContext->SmEngineContextInitalized = TRUE;

        ucxControllerContext->SmEngineContext.SpecialHandlingForControllerResetEventsFromHCD = 1;

        KeInitializeEvent(&ucxControllerContext->RHPdoMayExitD0,
                          SynchronizationEvent,
                          FALSE);

        KeInitializeEvent(&ucxControllerContext->ControllerResetCompleteProcessed,
                          SynchronizationEvent,
                          FALSE);
        
        KeInitializeSpinLock(&ucxControllerContext->ResetLock);

        ucxControllerContext->ControllerFailed = 0;
        
        //
        // Initialize the reference count to 1. This is needed since when the ResetReferenceCount
        // goes to 0, we invoke controller reset.
        //
        ucxControllerContext->ResetReferenceCount = 1;

        ucxControllerContext->PendingTreePurgeRequest = NULL;
        ucxControllerContext->PendingTreePurgeEndpointCount = 0;

        //
        // Check if verifier is enabled.
        //
        deviceObject = WdfDeviceWdmGetDeviceObject(Device);
        
        if (MmIsDriverVerifying(deviceObject->DriverObject) != 0) {
            ucxControllerContext->DriverVerifierEnabled = TRUE;
        }
        
        //
        // Check if Wdf Verifier is Enabled. 
        //
        if (Globals->WdfDriverGlobals->DriverFlags & WdfVerifyOn) {
            ucxControllerContext->DriverVerifierEnabled = TRUE;
        }


        //
        // Create a WDFQUEUE to handle IOCTL_INTERNAL_USB_ADDRESS0_OWNERSHIP_ACQUIRE
        // requests
        //

        WDF_IO_QUEUE_CONFIG_INIT(&queueConfig,
                                 WdfIoQueueDispatchSequential);

        queueConfig.PowerManaged = WdfFalse;

        queueConfig.EvtIoInternalDeviceControl = 
            Controller_EvtAddress0OwnershipIoInternalDeviceControl;
        
        WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&objectAttributes, 
                                                ADDRESS0_OWNERSHIP_QUEUE_CONTEXT);

        objectAttributes.ParentObject = ucxController;

        status = WdfIoQueueCreate(Device, 
                                  &queueConfig,
                                  &objectAttributes,
                                  &queueAddress0Ownership);
        
        if (!NT_SUCCESS(status)) {
            LogError(FlagController, 
                     "WdfIoQueueCreate for queueAddress0Ownership Failed %!STATUS!", status);
            LEAVE;
        }                

        WDF_IO_QUEUE_FORWARD_PROGRESS_POLICY_DEFAULT_INIT(
            &queueForwardProgressPolicy, 
            ADDRESS0_OWNERSHIP_TOTAL_FORWARD_PROGRESS_REQUESTS);

        status = WdfIoQueueAssignForwardProgressPolicy(queueAddress0Ownership,
                                                       &queueForwardProgressPolicy);

        if (!NT_SUCCESS(status)) {
            LogError(FlagController,
                     "WdfIoQueueAssignForwardProgressPolicy Failed %!STATUS!", status);
            LEAVE;
        }

        address0OwnershipQueueContext = Address0OwnershipQueueGetContext(queueAddress0Ownership);
        address0OwnershipQueueContext->Sig = SigAddress0OwnerShipQueueContext;
        address0OwnershipQueueContext->UcxController = ucxController;
        
        ucxControllerContext->QueueAddress0Ownership = queueAddress0Ownership;
        queueAddress0Ownership = NULL;
        
        //
        // Create a WDFQUEUE to manage USB Devices: 
        //     IOCTL_INTERNAL_USB_USBDEVICE_ENABLE
        //     IOCTL_INTERNAL_USB_USBDEVICE_RESET
        //     IOCTL_INTERNAL_USB_USBDEVICE_ADDRESS
        //     IOCTL_INTERNAL_USB_USBDEVICE_UPDATE
        //     IOCTL_INTERNAL_USB_USBDEVICE_HUB_INFO
        //     IOCTL_INTERNAL_USB_USBDEVICE_DISABLE
        //     IOCTL_INTERNAL_ENDPOINTS_CONFIGURE
        //     IOCTL_INTERNAL_DEFAULT_ENDPOINT_UPDATE
        //     IOCTL_INTERNAL_USB_ENDPOINT_RESET
        //     IOCTL_INTERNAL_USB_USBDEVICE_PURGEIO
        //     IOCTL_INTERNAL_USB_USBDEVICE_STARTIO
        //     IOCTL_INTERNAL_USB_SUBMIT_URB: 
        //         URB_FUNCTION_OPEN_STATIC_STREAMS & URB_FUNCTION_CLOSE_STATIC_STREAMS
        //

        WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchParallel);        
        
        queueConfig.PowerManaged = WdfFalse;
        
        queueConfig.Settings.Parallel.NumberOfPresentedRequests = 
            Config->NumberOfPresentedDeviceMgmtEvtCallbacks;

        queueConfig.EvtIoInternalDeviceControl = UsbDevice_EvtMgmtIoInternalDeviceControl;

        WDF_OBJECT_ATTRIBUTES_INIT(&objectAttributes);
        objectAttributes.ParentObject = ucxController;

        status = WdfIoQueueCreate(Device, 
                                  &queueConfig,
                                  &objectAttributes,
                                  &queueUsbDeviceMgmt);
        
        if (!NT_SUCCESS(status)) {
            LogError(FlagController, 
                     "WdfIoQueueCreate for queueUsbDeviceMgmt Failed %!STATUS!", status);
            LEAVE;
        }

        //
        // Enable forward progress on this queue
        //
        WDF_IO_QUEUE_FORWARD_PROGRESS_POLICY_DEFAULT_INIT(
            &queueForwardProgressPolicy,
            USBDEVICEMGMT_TOTAL_FORWARD_PROGRESS_REQUESTS);
        
        status = WdfIoQueueAssignForwardProgressPolicy(queueUsbDeviceMgmt,
                                                       &queueForwardProgressPolicy);

        if (!NT_SUCCESS(status)) {
            LogError(FlagController, 
                     "WdfIoQueueAssignForwardProgressPolicy Failed %!STATUS!", status);
            LEAVE;
        }

        ucxControllerContext->QueueUsbDeviceMgmt = queueUsbDeviceMgmt;
        queueUsbDeviceMgmt = NULL;

        //
        // Create a WDFQUEUE to serialize TreePurge
        //     IOCTL_INTERNAL_USB_USBDEVICE_TREE_PURGEIO
        //

        WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchSequential);

        queueConfig.PowerManaged = WdfFalse;

        queueConfig.EvtIoInternalDeviceControl = UsbDevice_EvtTreePurgeIoInternalDeviceControl;

        WDF_OBJECT_ATTRIBUTES_INIT(&objectAttributes);
        objectAttributes.ParentObject = ucxController;

        status = WdfIoQueueCreate(Device, 
                                  &queueConfig,
                                  &objectAttributes,
                                  &queueTreePurge);
        
        if (!NT_SUCCESS(status)) {
            LogError(FlagController, 
                     "WdfIoQueueCreate for queueTreePurge Failed %!STATUS!", status);
            LEAVE;
        }

        //
        // Enable forward progress on this queue
        //

        WDF_IO_QUEUE_FORWARD_PROGRESS_POLICY_DEFAULT_INIT(
            &queueForwardProgressPolicy,
            TREE_PURGE_TOTAL_FORWARD_PROGRESS_REQUESTS);
        
        status = WdfIoQueueAssignForwardProgressPolicy(queueTreePurge,
                                                       &queueForwardProgressPolicy);

        if (!NT_SUCCESS(status)) {
            LogError(FlagController, 
                     "WdfIoQueueAssignForwardProgressPolicy Failed %!STATUS!", status);
            LEAVE;
        }

        ucxControllerContext->QueueTreePurge = queueTreePurge;
        queueTreePurge = NULL;

        //
        // Create a WDFQUEUE to pend following ioctls while controller reset is going on: 
        //     IOCTL_INTERNAL_USB_USBDEVICE_RESET
        //     IOCTL_INTERNAL_USB_USBDEVICE_DISABLE
        //     IOCTL_INTERNAL_ENDPOINTS_CONFIGURE
        //
        WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchParallel);        
        
        queueConfig.PowerManaged = WdfFalse;
        
        queueConfig.EvtIoInternalDeviceControl = UsbDevice_EvtFailMgmtIoInternalDeviceControl;

        WDF_OBJECT_ATTRIBUTES_INIT(&objectAttributes);
        objectAttributes.ParentObject = ucxController;

        status = WdfIoQueueCreate(Device, 
                                  &queueConfig,
                                  &objectAttributes,
                                  &queuePendUsbDeviceMgmtDuringControllerReset);
        
        if (!NT_SUCCESS(status)) {
            LogError(FlagController, 
                     "WdfIoQueueCreate for queuePendUsbDeviceMgmtDuringControllerReset Failed %!STATUS!", status);
            LEAVE;
        }

        ucxControllerContext->QueuePendUsbDeviceMgmtDuringControllerReset = 
            queuePendUsbDeviceMgmtDuringControllerReset;
        queuePendUsbDeviceMgmtDuringControllerReset = NULL;

        //
        // Create a WDFQUEUE to manage all IOCTLS that come down to the
        // enumerated roothub PDO that are not handled by any
        // other queue, these Include:  
        //     IOCTL_INTERNAL_USB_ROOTHUB_GET_INFO
        //     IOCTL_INTERNAL_USB_ROOTHUB_GET_20PORT_INFO
        //     IOCTL_INTERNAL_USB_ROOTHUB_GET_30PORT_INFO
        //     IOCTL_INTERNAL_USB_REGISETER_COMPOSITE_DEVICE
        //     IOCTL_INTERNAL_USB_UNREGISTER_COMPOSITE_DEVICE
        //     IOCTL_INTERNAL_USB_SET_FUNCTION_HANDLE_DATA
        //     IOCTL_INTERNAL_USB_NEGOTIATE_INTERFACE_VERSION
        //     IOCTL_INTERNAL_USB_QUERY_USB_CAPABILITY



        WDF_IO_QUEUE_CONFIG_INIT(&queueConfig,
                                 WdfIoQueueDispatchParallel);
        
        queueConfig.PowerManaged = WdfFalse;

        queueConfig.EvtIoInternalDeviceControl = Controller_EvtDefaultIoInternalDeviceControl;

        WDF_OBJECT_ATTRIBUTES_INIT(&objectAttributes);
        objectAttributes.ParentObject = ucxController;

        status = WdfIoQueueCreate(Device, 
                                  &queueConfig,
                                  &objectAttributes,
                                  &queueUcxDefault);
        
        if (!NT_SUCCESS(status)) {
            LogError(FlagController, 
                     "WdfIoQueueCreate for queueUcxDefault Failed %!STATUS!", status);
            LEAVE;
        }

        //
        // Enable forward progress on this queue
        //

        WDF_IO_QUEUE_FORWARD_PROGRESS_POLICY_EXAMINE_INIT(
            &queueForwardProgressPolicy,
            UCXDEFAULT_TOTAL_FORWARD_PROGRESS_REQUESTS,
            Controller_EvtDefaultIoWdfIrpForForwardProgress);
        
        //
        // Currently Wdf supports setting forward progress policy only on 
        // specific queues. 
        //
        status = WdfIoQueueAssignForwardProgressPolicy(queueUcxDefault,
                                                       &queueForwardProgressPolicy);

        if (!NT_SUCCESS(status)) {
            LogError(FlagController, 
                     "WdfIoQueueAssignForwardProgressPolicy Failed %!STATUS!", status);
            LEAVE;
        }
        
        ucxControllerContext->QueueUcxDefault = queueUcxDefault;
        queueUcxDefault = NULL;
        
        //
        // Create a CSQ to park all ABORT_PIPE urbs from the client
        // drivers.
        // The only purpose of this
        // queue is to park the Abort Pipe Urbs, so that in case the abort pipe
        // urb is cancelled by the client driver, we can deal with the
        // cancellation
        // The handling of the Abort Pipe Urb starts in the Wdm Irp Preprocess
        // routine for the Roothub Pdo. This unusual design has been chosen to 
        // maintain compatibility with the exisiting 2.0 stack, which either
        // starts handling the Abort Pipe urb in the context of the callers 
        // thread, or fails it. 
        //

        KeInitializeSpinLock(&ucxControllerContext->AbortPipeIrpCsqLock);
        IoCsqInitialize(&ucxControllerContext->AbortPipeIrpCsq,
                        Controller_CsqInsertAbortPipeIrp,
                        Controller_CsqRemoveAbortPipeIrp,
                        Controller_CsqPeekNextAbortPipeIrp,
                        Controller_CsqAcquireAbortPipeIrpLock,
                        Controller_CsqReleaseAbortPipeIrpLock,
                        Controller_CsqCompleteCancelledAbortPipeIrp);

        //
        // Register a device interface for compatibility 
        //
        status = WdfDeviceCreateDeviceInterface(Device,
                                                &GUID_CLASS_USB_HOST_CONTROLLER,
                                                NULL);
        if (!NT_SUCCESS(status)) {
            LogError(FlagController, 
                     "WdfDeviceCreateDeviceInterface for GUID_CLASS_USB_HOST_CONTROLLER Failed %!STATUS!", 
                     status);
            LEAVE;
        }

        WDF_OBJECT_ATTRIBUTES_INIT(&objectAttributes);
        objectAttributes.ParentObject = ucxController;
        
        status = WdfStringCreate(NULL, &objectAttributes, &deviceInterfaceString);

        if (!NT_SUCCESS(status)) {
            LogError(FlagController, "WdfStringCreate Failed for Controller 0x%p, %!STATUS!", 
                     (PVOID) Device, status);
            LEAVE;
        }

        status = WdfDeviceRetrieveDeviceInterfaceString(Device,
                                                        &GUID_CLASS_USB_HOST_CONTROLLER,
                                                        NULL,
                                                        deviceInterfaceString);
        
        if (!NT_SUCCESS(status)) {
            LogError(FlagController, 
                     "WdfDeviceRetrieveDeviceInterfaceString for GUID_CLASS_USB_HOST_CONTROLLER Failed. Fdo 0x%p, %!STATUS!", 
                     Device, status);
            LEAVE;
        }

        WdfObjectReferenceWithTag(deviceInterfaceString, TAG(EXPORT(UcxControllerCreate)));
        ucxControllerContext->SymbolicNameForHostControllerInterface = deviceInterfaceString;

        //
        // Lets get the capabilities of the FDO
        // 
        status = UcxControllerGetDeviceCapabilities(Device, &ucxControllerContext->HcCaps);
        if (!NT_SUCCESS(status)) {
            LogError(FlagController, "UcxRootHubGetDeviceCapabilities Failed %!STATUS!", status);
            LEAVE;
        }
        
        ucxControllerContext->ControllerFlavor = USB_HcGeneric;

        status = Wmi_Create(Device);

        if (!NT_SUCCESS(status)) {

            LogError(FlagController, "Wmi_Create Failed %!STATUS!", status);
            
            LEAVE;
        }

        KeInitializeSpinLock(&ucxControllerContext->SystemThreadQueueLock);
        
        InitializeListHead(&ucxControllerContext->SystemThreadQueue);

        KeInitializeEvent(&ucxControllerContext->SystemThreadWorkPending,
                          NotificationEvent,
                          FALSE);

        ucxControllerContext->SystemThreadPaused = TRUE;

        ucxControllerContext->WaitOnControllerResetComplete = TRUE;

        ucxControllerContext->ForwardProgressEnabledInHcd = FALSE;

        KeInitializeMutex(&ucxControllerContext->EnableForwardProgressMutex, 0);

        //
        // Insert the UcxControllerContext into the global controller
        // list and increment the controller count.
        //
        KeAcquireSpinLock(&g_WdfDriverUcxContext->UcxControllerListLock, &irql);

        InsertTailList(&g_WdfDriverUcxContext->UcxControllerListHead,
                       &ucxControllerContext->UcxControllerListEntry);

        g_WdfDriverUcxContext->UcxControllerListCount++;

        KeReleaseSpinLock(&g_WdfDriverUcxContext->UcxControllerListLock, irql);

    } FINALLY {

        if (!NT_SUCCESS(status)) {
            
            if (ucxController != NULL) {
                WdfObjectDelete(ucxController);
            }
        } else {
            *Controller = ucxController;

            Etw_UcxControllerCreate(NULL, ucxControllerContext);
        }
        
        LogVerbose(FlagController,"UcxControllerCreate 0x%x", status);

    }

    return status;

}

__drv_maxIRQL(DISPATCH_LEVEL)
VOID
EXPORT(UcxControllerNeedsReset)(
    __in_opt 
        PUCX_GLOBALS                  Globals,
    __in 
        UCXCONTROLLER                 Controller
    )
/*++

Routine Description:

    This export function is called by the HCD at the time it needs to be reset.  
    
    This function initiates a Non-Pnp Controller Reset operation if it can by queuing an event 
        into the Controller reset state machine.

Arguments:

    Globals - Driver globals of the controller driver. This information is not
        being used. 
    
    Controller - The handle to the UCXCONTROLLER object

Return Value:

    BOOLEAN - True if the NeedsReset request was accepted by UCX, False otherwise.
        
--*/
{
    PUCXCONTROLLER_PRIVCONTEXT  ucxControllerContext;

    UNREFERENCED_PARAMETER(Globals);
    
    TRY {
        
        ucxControllerContext = UcxControllerGetContext(Controller);
        Controller_EventAdd(ucxControllerContext, ControllerResetEventHCDNeedsReset);
    
    } FINALLY {

    }

    return;

}

__drv_maxIRQL(DISPATCH_LEVEL)
VOID
EXPORT(UcxControllerResetComplete)(
    __in_opt 
        PUCX_GLOBALS                        Globals,
    __in 
        UCXCONTROLLER                       Controller,
    __in
        PUCX_CONTROLLER_RESET_COMPLETE_INFO UcxControllerResetCompleteInfo
    )
/*++

Routine Description:

    This export function is called by the HCD to inform UCX that the reset operation has compelted. 
    
Arguments:

    Globals - Driver globals of the controller driver. This information is not
        being used. 
    
    Controller - The handle to the UCXCONTROLLER object

Return Value:

    VOID
        
--*/
{
    PUCXCONTROLLER_PRIVCONTEXT  ucxControllerContext;

    UNREFERENCED_PARAMETER(Globals);

    TRY {
    
        ucxControllerContext = UcxControllerGetContext(Controller);

        if (UcxControllerResetCompleteInfo->UcxControllerState == UcxControllerStateLost) {
            ucxControllerContext->WasDeviceProgrammingLost = 1;
        }
        
        ucxControllerContext->WasRootHubReset = 1;

        Controller_EventAdd(ucxControllerContext, ControllerResetEventResetCompleteFromHCD);
    
        if (ucxControllerContext->WaitOnControllerResetComplete) {
            
            NT_ASSERT(ucxControllerContext->RHPdoInD0 == FALSE);

            //
            // We didnt get a chance to prepare the endpoints and devices for controller reset 
            // earlier. So the Controller Reset state machine would 
            //
            KeWaitForSingleObject(&ucxControllerContext->ControllerResetCompleteProcessed,
                                  Executive, 
                                  KernelMode,
                                  FALSE,
                                  NULL);

        }

    } FINALLY {

    }

    return;

}

__drv_maxIRQL(DISPATCH_LEVEL)
VOID
EXPORT(UcxControllerSetFailed)(
    __in_opt 
        PUCX_GLOBALS                        Globals,
    __in 
        UCXCONTROLLER                       Controller
    )
/*++

Routine Description:

    This export function is called by the HCD to inform UCX that the controller is failed (e.g.
    missing) and never to come back.

    HCD must call this function in the following conditions:
    - HCD fails D0Entry
    - HCD stops processing endpoint I/O
    
Arguments:

    Globals - Driver globals of the controller driver. This information is not
        being used. 
    
    Controller - The handle to the UCXCONTROLLER object

Return Value:

    VOID
        
--*/
{
    PUCXCONTROLLER_PRIVCONTEXT  ucxControllerContext;
    PUCXROOTHUB_PRIVCONTEXT     ucxRootHubContext;

    UNREFERENCED_PARAMETER(Globals);

    TRY {

        ucxControllerContext = UcxControllerGetContext(Controller);

        //
        // First set ControllerFailed so other functions know not to undo our actions
        //
        if (0 != InterlockedCompareExchange(&ucxControllerContext->ControllerFailed,
                                            1 /*Exchange*/,
                                            0 /*Comparand*/)) {
            LogInfo(FlagController, "HCD already reported failed UcxController 0x%p", Controller);
            LEAVE;
        }

        LogInfo(FlagController, "HCD reported failed UcxController 0x%p", Controller);

        //
        // Disconnect all the devices so that all endpoints are purged (once it reaches the
        // disconnected state). This is needed because if the stack is in D0 and the controller is
        // removed, the HCD may stop processing endpoint I/O, in which case clients' synchronous
        // D0Exit I/O will hang until this purge cancels it.
        //        
        // Note: Do this before Adding the ControllerResetEventControllerFailed event in the 
        // controller state machine. This is to prevent a kernel stack overflow that happens due to
        // the following sequence that happens on the same thread
        //     * Fail RootHubIO
        //     * RootHub notifies child DSM (for a child hub level1) and DSM sends a purge down
        //     * Ucx Calls XHCI to purge
        //     * The request for child hub at level_1 is purged
        //     * Driver for level1 hub notifies child DSM (for a child hub level 2) and DSM sends a purge down
        //     * Ucx Calls XHCI to purge 
        //     * The request for child hub at level_2 is purged
        //     * Driver for level 2 hub notfies .......
        //
        ucxRootHubContext = ucxControllerContext->UcxRootHubContext;
        UsbDevice_Disconnect(UCXROOTHUB_TO_UCXUSBDEVICE(ucxRootHubContext->UcxRootHub), 0);

        Controller_EventAdd(ucxControllerContext, ControllerResetEventControllerFailed);

    } FINALLY {

    }

    return;
}
    
VOID
Controller_EvtDestroyCallback(
    WDFOBJECT               UcxController
    )
/*++

Routine Description:

    This event callback is called when the UcxController object memory is being freed by WDF.
        
Arguments:

    UcxController - The handle for the UCXCONTROLLER object

Return Value:

    VOID

--*/
{
    PUCXCONTROLLER_PRIVCONTEXT        ucxControllerContext; 
    WDFSTRING                         deviceInterfaceString;
    KIRQL                             irql;

    TRY {

        ucxControllerContext = UcxControllerGetContext(UcxController);

        //
        // Terminate the system thread
        //
        if (ucxControllerContext->SystemThread != NULL) {
            
            KeAcquireSpinLock(&ucxControllerContext->SystemThreadQueueLock, &irql);

            ucxControllerContext->SystemThreadTerminate = TRUE;

            KeSetEvent(&ucxControllerContext->SystemThreadWorkPending, 0, FALSE);                      

            KeReleaseSpinLock(&ucxControllerContext->SystemThreadQueueLock, irql);

            KeWaitForSingleObject(ucxControllerContext->SystemThread,
                                  Executive,
                                  KernelMode,
                                  FALSE,
                                  NULL);
            
        }

        if (ucxControllerContext->SmEngineContextInitalized) {
            StateMachineEngine_ReleaseResources(&ucxControllerContext->SmEngineContext);
            ucxControllerContext->SmEngineContextInitalized = FALSE;
        }

        NT_ASSERTMSG("Some Client Did not Unregister, Memory Leak will occur", 
                     ucxControllerContext->USBDInterfaceCount == 0);
        
        //
        // Remove the UcxControllerContext from the global controller
        // list and decrement the controller count.
        //
        KeAcquireSpinLock(&g_WdfDriverUcxContext->UcxControllerListLock, &irql);

        if (IsListEmpty(&ucxControllerContext->UcxControllerListEntry) == FALSE) {
            RemoveEntryList(&ucxControllerContext->UcxControllerListEntry);
            g_WdfDriverUcxContext->UcxControllerListCount--;
        }
        
        KeReleaseSpinLock(&g_WdfDriverUcxContext->UcxControllerListLock, irql);

        InitializeListEntry(&ucxControllerContext->UcxControllerListEntry);

        Etw_UcxControllerDelete(NULL, ucxControllerContext);

        if (NULL != ucxControllerContext->SymbolicNameForHostControllerInterface) {
            deviceInterfaceString =  ucxControllerContext->SymbolicNameForHostControllerInterface;
            ucxControllerContext->SymbolicNameForHostControllerInterface = NULL;
            WdfObjectDereferenceWithTag(deviceInterfaceString, TAG(EXPORT(UcxControllerCreate)));
        }

    } FINALLY {

    }
}

VOID
Controller_EvtDefaultIoInternalDeviceControl(
    WDFQUEUE                WdfQueue,
    WDFREQUEST              Request,
    size_t                  OutputBufferLength,
    size_t                  InputBufferLength,
    ULONG                   IoControlCode
    )
/*++

Routine Description:

    This event callback function handles the Ioctls that come to the enumerated
    roothub Pdo, and were not forwarded to any other queue.
    The RootHub Pdo's irp preprocess routine forwards these Ioctls to this Queue
    
    These include: 
        IOCTL_INTERNAL_USB_ROOTHUB_GET_INFO
        IOCTL_INTERNAL_USB_ROOTHUB_GET_20PORT_INFO
        IOCTL_INTERNAL_USB_ROOTHUB_GET_30PORT_INFO
        IOCTL_INTERNAL_USB_REGISETER_COMPOSITE_DEVICE
        IOCTL_INTERNAL_USB_UNREGISTER_COMPOSITE_DEVICE
        IOCTL_INTERNAL_USB_SET_FUNCTION_HANDLE_DATA
        IOCTL_INTERNAL_USB_NEGOTIATE_INTERFACE_VERSION
        IOCTL_INTERNAL_USB_QUERY_USB_CAPABILITY
        IOCTL_INTERNAL_USB_REGISTER_COMPOSITE_DEVICE
        IOCTL_INTERNAL_USB_UNREGISTER_COMPOSITE_DEVICE
        IOCTL_INTERNAL_USB_SET_FUNCTION_HANDLE_DATA
        IOCTL_INTERNAL_USB_REQUEST_REMOTE_WAKE_NOTIFICATION
        IOCTL_INTERNAL_USB_GET_DUMP_DATA
        IOCTL_INTERNAL_USB_FREE_DUMP_DATA
        IOCTL_INTERNAL_USB_NOTIFY_FORWARD_PROGRESS          
    
Arguments:

    WdfQueue - A handle to the queue object that handles the above mentioned ioctls

    WdfRequest - A handle to the wdfrequest on this queue.

    OutputBufferLength - Not used
    
    InputBufferLength - Not used
    
    IoControlCode - Describles the Ioctl code.

Return Value:

    VOID

--*/
{

    WDFDEVICE                                   wdfDevice;
    PWDFDEVICE_UCX_CONTEXT                      wdfDeviceUcxContext;
    PUCXCONTROLLER_PRIVCONTEXT                  ucxControllerContext;
    PUCXROOTHUB_PRIVCONTEXT                     ucxRootHubContext;
    UCXUSBDEVICE                                ucxUsbDevice;
    PUCXUSBDEVICE_PRIVCONTEXT                   ucxUsbDeviceContext;
    PUCXENDPOINT_PRIVCONTEXT                    ucxEndpointContext;
    PIRP                                        irp;
    UCXROOTHUB                                  ucxRootHub;
    PHUBUCX_ROOTHUB_INFO                        hubUcxRootHubInfo;
    PREGISTER_COMPOSITE_DEVICE                  registerCompositeDevice;
    PUSBD_FUNCTION_HANDLE                       usbdFunctionHandles;
    PUSB_FUNCTION_DATA                          usbFunctionData;
    PREQUEST_REMOTE_WAKE_NOTIFICATION           remoteWakeNotification;
    PQUERY_USB_CAPABILITY                       capability;
    PUCHAR                                      capabilityOutputBuffer;
    PUSB_DUMP_DEVICE_INFO                       dumpDeviceInfo;
    PUSB_DUMP_CONTROLLER_INFO                   dumpControllerInfo;
    BOOLEAN                                     resetReferenceAcquired;
    PUSB_FORWARD_PROGRESS_INFO                  forwardProgressInfo;
    NTSTATUS                                    status; 

    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);
    
    TRY {
        NT_ASSERT (
            IoControlCode == IOCTL_INTERNAL_USB_ROOTHUB_GET_INFO ||
            IoControlCode == IOCTL_INTERNAL_USB_ROOTHUB_GET_20PORT_INFO ||
            IoControlCode == IOCTL_INTERNAL_USB_ROOTHUB_GET_30PORT_INFO ||
            IoControlCode == IOCTL_INTERNAL_USB_REGISTER_COMPOSITE_DEVICE ||
            IoControlCode == IOCTL_INTERNAL_USB_UNREGISTER_COMPOSITE_DEVICE ||
            IoControlCode == IOCTL_INTERNAL_USB_SET_FUNCTION_HANDLE_DATA ||
            IoControlCode == IOCTL_INTERNAL_USB_REQUEST_REMOTE_WAKE_NOTIFICATION ||
            IoControlCode == IOCTL_INTERNAL_USB_QUERY_USB_CAPABILITY ||
            IoControlCode == IOCTL_INTERNAL_USB_GET_DUMP_DATA ||
            IoControlCode == IOCTL_INTERNAL_USB_FREE_DUMP_DATA ||
            IoControlCode == IOCTL_INTERNAL_USB_NOTIFY_FORWARD_PROGRESS
            );

        wdfDevice = WdfIoQueueGetDevice(WdfQueue);
        wdfDeviceUcxContext = WdfDeviceGetUcxContext(wdfDevice);
        ucxControllerContext = wdfDeviceUcxContext->UcxControllerContext;
        ucxRootHubContext = ucxControllerContext->UcxRootHubContext;
        
        ucxRootHub = WdfObjectContextGetObject(ucxRootHubContext);

        LogVerbose(FlagController, "%!UCX_IOCTL! WdfRequest 0x%p, ucxControllerContext 0x%p, UcxRootHubContext 0x%p", 
                   IoControlCode,  Request, ucxControllerContext, ucxRootHubContext);

        resetReferenceAcquired = FALSE;

        switch (IoControlCode) {
        case IOCTL_INTERNAL_USB_ROOTHUB_GET_INFO:
        case IOCTL_INTERNAL_USB_ROOTHUB_GET_20PORT_INFO:
        case IOCTL_INTERNAL_USB_ROOTHUB_GET_30PORT_INFO:
            resetReferenceAcquired = Controller_ResetReferenceAcquire(ucxControllerContext);
            
            if (FALSE == resetReferenceAcquired) {
                LogInfo(FlagController, "Controller_ResetReferenceAcquire failed");
                WdfRequestComplete(Request, STATUS_NO_SUCH_DEVICE);
                LEAVE;
            }

            break;
        }
        
        switch (IoControlCode) {
        case IOCTL_INTERNAL_USB_ROOTHUB_GET_INFO: 

            hubUcxRootHubInfo = (PHUBUCX_ROOTHUB_INFO) Ucx_WdfRequestGetArg1(Request);
            ucxEndpointContext = UcxEndpointGetContext(ucxRootHubContext->UcxInterruptEndpoint);

            //
            // Only ROOTHUB_INFO part of HUBUCX_ROOTHUB_INFO is exposed to HCD. Set the
            // Size of this structure to sizeof(ROOTHUB_INFO), to be fixed up in the
            // completion routine.
            //
            NT_ASSERT(hubUcxRootHubInfo->Size == sizeof(HUBUCX_ROOTHUB_INFO));
            hubUcxRootHubInfo->Size = sizeof(ROOTHUB_INFO);
            hubUcxRootHubInfo->UsbdInterruptPipeHandle = &ucxEndpointContext->UcxPipeInfo;

            ucxRootHubContext->Config.EvtRootHubGetInfo(ucxRootHub, Request);
            break;
        
        case IOCTL_INTERNAL_USB_ROOTHUB_GET_20PORT_INFO: 
            ucxRootHubContext->Config.EvtRootHubGet20PortInfo(ucxRootHub, Request);
            break;
        
        case IOCTL_INTERNAL_USB_ROOTHUB_GET_30PORT_INFO: 
            ucxRootHubContext->Config.EvtRootHubGet30PortInfo(ucxRootHub, Request);
            break;
                                                         
        case IOCTL_INTERNAL_USB_REGISTER_COMPOSITE_DEVICE: 
            
            Ucx_WdfRequestGetArgs(Request, &registerCompositeDevice, &ucxUsbDevice, NULL);
            ucxUsbDeviceContext = UcxUsbDeviceGetContext(ucxUsbDevice);
            irp = WdfRequestWdmGetIrp(Request);
            usbdFunctionHandles = irp->AssociatedIrp.SystemBuffer;
            UsbDevice_RegisterCompositeDevice(ucxUsbDeviceContext,
                                              registerCompositeDevice,
                                              usbdFunctionHandles,
                                              Request);
            
            break;
        
        case IOCTL_INTERNAL_USB_UNREGISTER_COMPOSITE_DEVICE: 
            
            Ucx_WdfRequestGetArgs(Request, NULL, &ucxUsbDevice, NULL);
            ucxUsbDeviceContext = UcxUsbDeviceGetContext(ucxUsbDevice);
            UsbDevice_UnregisterCompositeDevice(ucxUsbDeviceContext, Request);
            break;
        
        case IOCTL_INTERNAL_USB_SET_FUNCTION_HANDLE_DATA: 
            
            Ucx_WdfRequestGetArgs(Request, NULL, &ucxUsbDevice, NULL);
            ucxUsbDeviceContext = UcxUsbDeviceGetContext(ucxUsbDevice);
            irp = WdfRequestWdmGetIrp(Request);
            usbFunctionData = (PUSB_FUNCTION_DATA) irp->AssociatedIrp.SystemBuffer;
            UsbDevice_SetFunctionData(ucxUsbDeviceContext, usbFunctionData, Request);
            break;
        
        case IOCTL_INTERNAL_USB_REQUEST_REMOTE_WAKE_NOTIFICATION: 
            
            Ucx_WdfRequestGetArgs(Request, &remoteWakeNotification, &ucxUsbDevice, NULL);
            ucxUsbDeviceContext = UcxUsbDeviceGetContext(ucxUsbDevice);
            UsbDevice_RemoteWakeNotificationRequest(ucxUsbDeviceContext, 
                                                    remoteWakeNotification, 
                                                    Request);
            break;
        
        case IOCTL_INTERNAL_USB_QUERY_USB_CAPABILITY: 

            capability = (PQUERY_USB_CAPABILITY)Ucx_WdfRequestGetArg1(Request);
            irp = WdfRequestWdmGetIrp(Request);
            capabilityOutputBuffer = (PUCHAR) irp->AssociatedIrp.SystemBuffer;

            Controller_QueryUsbCapability(ucxControllerContext,
                                          capability,
                                          capabilityOutputBuffer,
                                          Request);
            break;

        case IOCTL_INTERNAL_USB_GET_DUMP_DATA:

            irp = WdfRequestWdmGetIrp(Request);
            dumpControllerInfo = irp->UserBuffer;
            dumpDeviceInfo = irp->AssociatedIrp.SystemBuffer;
            ucxUsbDevice = dumpDeviceInfo->DeviceHandle;

            Controller_GetDumpData(ucxControllerContext,
                                   ucxUsbDevice,
                                   dumpDeviceInfo,
                                   dumpControllerInfo,
                                   Request);
            break;

        case IOCTL_INTERNAL_USB_FREE_DUMP_DATA:

            irp = WdfRequestWdmGetIrp(Request);
            dumpControllerInfo = irp->AssociatedIrp.SystemBuffer;

            Controller_FreeDumpData(ucxControllerContext,
                                    dumpControllerInfo,
                                    Request);
            break;

        case IOCTL_INTERNAL_USB_NOTIFY_FORWARD_PROGRESS:

            forwardProgressInfo = (PUSB_FORWARD_PROGRESS_INFO)Ucx_WdfRequestGetArg1(Request);
            
            status = Controller_EnableForwardProgress(ucxControllerContext,
                                                      forwardProgressInfo);

            WdfRequestComplete(Request, status);
            break;

        default: 
            LogWarning(FlagController, "A IOCTL %d was unexpected in this function", IoControlCode);
            WdfRequestComplete(Request, STATUS_INVALID_DEVICE_REQUEST);
            break;
        }
    
    } FINALLY {

        if (resetReferenceAcquired) {
            Controller_ResetReferenceRelease(ucxControllerContext);
        }

    }

    return;
}

VOID
Controller_EvtAddress0OwnershipIoInternalDeviceControl(
    WDFQUEUE                WdfQueue,
    WDFREQUEST              Request,
    size_t                  OutputBufferLength,
    size_t                  InputBufferLength,
    ULONG                   IoControlCode
    )
/*++

Routine Description:

    This event callback function handles the 
    IOCTL_INTERNAL_USB_ADDRESS0_OWNERSHIP_ACQUIRE ioctl. 
    
Arguments:

    WdfQueue - A handle to the queue object that handles the above mentioned ioctl

    WdfRequest - A handle to the wdfrequest on this queue.

    OutputBufferLength - Not used
    
    InputBufferLength - Not used
    
    IoControlCode - Describles the Ioctl code.

Return Value:

    VOID

Comment:
    
    Completing a request on this give the hub driver ownership to interract on 
    Address 0.
    
    The Queue is stopped in this callback and is started again when the hub 
    driver releases the ownership by calling a release address 0 function.

--*/
{
    PADDRESS0_OWNERSHIP_QUEUE_CONTEXT           address0OwnershipQueueContext;
    PADDRESS0_OWNERSHIP_ACQUIRE                 address0OwnershipAcquire;
    
    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);

    TRY {
        switch (IoControlCode) {
        
        case IOCTL_INTERNAL_USB_ADDRESS0_OWNERSHIP_ACQUIRE: 
        
            //
            // Stopping the queue since we dont want to handle another 
            // request till this one is released
            //
            
            WdfIoQueueStop(WdfQueue, NULL, NULL);

            //
            // Get (from the Request Parameters) and store (In Queue Context) 
            // the info of the requesting Hub & UsbDevice
            //
            address0OwnershipAcquire = (PADDRESS0_OWNERSHIP_ACQUIRE) Ucx_WdfRequestGetArg1(Request);
            
            address0OwnershipQueueContext = Address0OwnershipQueueGetContext(WdfQueue);
                        
            NT_ASSERT(NULL == address0OwnershipQueueContext->CurrentOwnerHub);
            NT_ASSERT(NULL == address0OwnershipQueueContext->CurrentOwnerUsbDevice);
            
            NT_ASSERT(address0OwnershipAcquire->Hub);

            LogVerbose(FlagController, "Granting Addr0 Ownership Hub 0x%p, Device 0x%p",
                       address0OwnershipAcquire->Hub, address0OwnershipAcquire->UsbDevice);

            address0OwnershipQueueContext->CurrentOwnerHub = address0OwnershipAcquire->Hub;
            address0OwnershipQueueContext->CurrentOwnerUsbDevice = address0OwnershipAcquire->UsbDevice;

            WdfRequestComplete(Request, STATUS_SUCCESS);

            break;
        
        default:
            LogError(FlagController, "Unknown Ioctl Code %d", IoControlCode);
            WdfRequestComplete(Request, STATUS_INVALID_DEVICE_REQUEST);
        }
    } FINALLY {

    }

    return;
}

VOID
Controller_Address0OwnershipRelease(
    __in 
        UCXUSBDEVICE       Hub,
    __in 
        UCXUSBDEVICE       UsbDevice
    )
/*++

Routine Description:

    This query interface function is called by the hub driver when it wants 
    to release the address 0 ownership 
    
Arguments:

    Hub - The handle to the UCXUSBDEVICE representing the hub that owns the 
        Address 0 ownership
        
    UsbDevice - The handle handle to the UCXUSBDEVICE representing the usbdevice
       for which the ownership was acquired.

Return Value:

    VOID

--*/
{
    PUCXUSBDEVICE_PRIVCONTEXT         ucxUsbDeviceContext;
    PUCXCONTROLLER_PRIVCONTEXT        ucxControllerContext;
    WDFQUEUE                          address0OwnershipQueue;
    PADDRESS0_OWNERSHIP_QUEUE_CONTEXT address0OwnershipQueueContext;

    UNREFERENCED_PARAMETER(UsbDevice);

    TRY {

        LogVerbose(FlagController, "Releasing Addr0 Ownership Hub 0x%p, Device 0x%p",
                   Hub, UsbDevice);
        //
        // Find the address 0 ownership queue and clear the owner information
        // from there. 
        //
        ucxUsbDeviceContext = UcxUsbDeviceGetContext(Hub);

        ucxControllerContext = ucxUsbDeviceContext->UcxControllerContext;
        
        address0OwnershipQueue = ucxControllerContext->QueueAddress0Ownership; 

        address0OwnershipQueueContext = Address0OwnershipQueueGetContext(address0OwnershipQueue);

        NT_ASSERT(address0OwnershipQueueContext->CurrentOwnerHub == Hub);

        address0OwnershipQueueContext->CurrentOwnerHub = NULL;
        address0OwnershipQueueContext->CurrentOwnerUsbDevice = NULL;

        //
        // Unblock the queue
        //

        WdfIoQueueStart(address0OwnershipQueue);

    } FINALLY {


    }
    return;
}

BOOLEAN
Controller_AreSuperspeedStreamsSupported(
    __in
        UCXUSBDEVICE        Hub
    )
/*++

Routine Description:

    This query interface function is called by the hub driver when it wants 
    to determine if the host controller supports superspeed stream endpoints in
    order to determine whether it needs to enumerate a storage device as BOT 
    instead of UASP.
    
Arguments:

    Hub - The handle to the UCXUSBDEVICE representing the hub

Return Value:

    BOOLEAN

--*/
{
    PUCXUSBDEVICE_PRIVCONTEXT         ucxUsbDeviceContext;
    PUCXCONTROLLER_PRIVCONTEXT        ucxControllerContext;
    BOOLEAN                           result;
    USHORT                            maxStreams;
    ULONG                             resultLength;
    NTSTATUS                          status;

    TRY {

        LogVerbose(FlagController, "Getting superspeed stream endpoint capability for Hub 0x%p",
                   Hub);

        result = FALSE;
        maxStreams = 0;

        ucxUsbDeviceContext = UcxUsbDeviceGetContext(Hub);

        ucxControllerContext = ucxUsbDeviceContext->UcxControllerContext;
        
        if (NULL == ucxControllerContext->Config.EvtControllerQueryUsbCapability) {
            LogInfo(FlagController, "HCD does not provide EvtControllerQueryUsbCapability");
            LEAVE;
        } 


        status = ucxControllerContext->Config.EvtControllerQueryUsbCapability(
            ucxControllerContext->UcxController,
            (GUID*)&GUID_USB_CAPABILITY_STATIC_STREAMS,
            sizeof(USHORT),
            &maxStreams,
            &resultLength);

        if (!NT_SUCCESS(status)) {
            LogError(FlagController, 
                     "EvtControllerQueryUsbCapability failed for GUID_USB_CAPABILITY_STATIC_STREAMS, controller context(0x%p), status %!STATUS!",
                     ucxControllerContext, status);

            LEAVE;
        }

        if (maxStreams > 0) {
            result = TRUE;
        }

    } FINALLY {


    }
    return result;
}


BOOLEAN
Controller_QueryClearTtBufferOnAsyncTransferCancel(
    __in
        PUCXCONTROLLER_PRIVCONTEXT  UcxControllerContext
    )
/*++

Routine Description:

    This routine queries the controller for the
    DEVICE_CLEAR_TT_BUFFER_ON_ASYNC_TRANSFER_CANCEL capability to
    determine whether or not a Clear TT Buffer request should be issued
    to a TT hub if a transfer is canceled on a control or bulk endpoint
    on a low speed or full speed device downstream of the TT hub.

Arguments:

    UcxControllerContext - Ucx context for the UCXCONTOLLER object

Return Value:

    BOOLEAN

--*/
{
    BOOLEAN     result;
    ULONG       resultLength;
    NTSTATUS    status;

    TRY {

        LogVerbose(FlagController, "Querying ClearTtBufferOnAsyncTransferCancel capability, controller context 0x%p",
                   UcxControllerContext);

        if (NULL == UcxControllerContext->Config.EvtControllerQueryUsbCapability) {
            LogInfo(FlagController, "HCD does not provide EvtControllerQueryUsbCapability");
            result = FALSE;
            LEAVE;
        } 

        status = UcxControllerContext->Config.EvtControllerQueryUsbCapability(
            UcxControllerContext->UcxController,
            (GUID*)&GUID_USB_CAPABILITY_CLEAR_TT_BUFFER_ON_ASYNC_TRANSFER_CANCEL,
            0,
            NULL,
            &resultLength);

        if (NT_SUCCESS(status)) {
            LogInfo(FlagController, "Controller supports ClearTtBufferOnAsyncTransferCancel capability, controller context 0x%p",
                    UcxControllerContext);
            result = TRUE;
        } else {
            LogInfo(FlagController, "Controller does not support ClearTtBufferOnAsyncTransferCancel capability, controller context 0x%p",
                    UcxControllerContext);
            result = FALSE;
        }

    } FINALLY {

    }

    return result;
}


WDF_IO_FORWARD_PROGRESS_ACTION
Controller_EvtDefaultIoWdfIrpForForwardProgress(
    WDFQUEUE                Queue,
    PIRP                    Irp
    )
/*++

Routine Description:

    This event callback function that determines if a request should be used 
    for forward progress or not. 
    
Arguments:

    Queue - A handle to the queue object that handles the above mentioned ioctl

    Irp - A pointer to the irp

Return Value:

    Returns WdfIoForwardProgressActionUseReservedRequest if the reserved request
    should be used. 
    
    Returns WdfIoForwardProgressActionFailRequest otherwise.
    
Comment:
    
    The reserved request is used for only the following ioctls
--*/
{
    WDF_IO_FORWARD_PROGRESS_ACTION    action;
    PIO_STACK_LOCATION                ioStackLocation;

    UNREFERENCED_PARAMETER(Queue);

    TRY {
        
        ioStackLocation = IoGetCurrentIrpStackLocation(Irp);

        switch ((ULONG_PTR) ioStackLocation->Parameters.Others.Argument3) {
        
        case IOCTL_INTERNAL_USB_REQUEST_REMOTE_WAKE_NOTIFICATION:
            //
            // This request will remain pended with Ucx for an indefinate amount of time.
            // Thus cannot use a reserved request for this.
            //
            action = WdfIoForwardProgressActionFailRequest;
            break;

        default:
            action = WdfIoForwardProgressActionUseReservedRequest;
            break;
        }
    } FINALLY {

    }
    return action;
}

VOID
Controller_QueryUsbCapability(
    __in 
        PUCXCONTROLLER_PRIVCONTEXT                UcxControllerContext,
    __in 
        PQUERY_USB_CAPABILITY                     UsbCapability,
    __out_bcount(UsbCapability->OutputBufferLength)
        PUCHAR                                    OutputBuffer,
    __in
        WDFREQUEST                                Request
    )
/*++

Routine Description:

    Handles a get capability request from the client driver.

Arguments:

    UcxControllerContext - Ucx context for the UCXCONTOLLER object
    
    UsbCapability - The structure specifying the Usb Capability being querried. 
    
    OutputBuffer - The output buffer
         
    Request - WDFREQUEST representing this request.         
    
--*/
{
    NTSTATUS               status;
    PUSBDI_HANDLE_DATA     usbdiHandleData;

    TRY {
        
        if (UsbCapability->Size != sizeof(*UsbCapability)) {
            status = STATUS_INVALID_PARAMETER;
            LogError(FlagController, "Incorrect Version %d in GetCapability (0x%p)",  
                     UsbCapability->Version, UsbCapability);
            LEAVE;
        }

        if (UsbCapability->Version != QUERY_USB_CAPABILITY_LATEST) {
            status = STATUS_INVALID_PARAMETER;
            LogError(FlagController, "Incorrect Version %d in GetCapability (0x%p)",  
                     UsbCapability->Version, UsbCapability);
            LEAVE;
        }

        if (UsbCapability->USBDIHandle == NULL) {
            status = STATUS_INVALID_PARAMETER;
            LogError(FlagController, "Null USBDHandle in GetCapability (0x%p)",  UsbCapability);
            LEAVE;
        }
        
        usbdiHandleData = (PUSBDI_HANDLE_DATA) UsbCapability->USBDIHandle;

        UsbCapability->ResultLength = 0;

        if (UsbCapability->OutputBufferLength != 0 && OutputBuffer == NULL) {
            status = STATUS_INVALID_PARAMETER;
            LogError(FlagController, "Output buffer should not be NULL. GetCapability (0x%p)",  
                     UsbCapability);
            LEAVE;
        }

        if (UsbCapability->OutputBufferLength == 0 && OutputBuffer != NULL) {
            status = STATUS_INVALID_PARAMETER;
            LogError(FlagController, "Output buffer should be zero. GetCapability (0x%p)", 
                     UsbCapability);
            LEAVE;
        }

        do {

            if (RtlCompareMemory(&UsbCapability->CapabilityType,
                                 &GUID_USB_CAPABILITY_CHAINED_MDLS,
                                 sizeof(GUID)) == sizeof(GUID)) {
                if (OutputBuffer != NULL) {
                    status = STATUS_INVALID_PARAMETER;
                    LogError(FlagController, "Output buffer shoudl be null for GUID_USB_CAPABILITY_CHAINED_MDLS (0x%p)",
                             UsbCapability);
                    LEAVE;
                }

                if (FALSE == usbdiHandleData->ChainedMdlSupportGranted) {
                    if (Ucx_FailDueToVerifierSetting(usbdiHandleData->UsbVerifierFailChainedMdlSupport)) {
                        status = Ucx_GetRandomErrorNTSTATUS();
                        LogInfo(FlagController, "UsbVerifier failing Chained Mdl Support, Status %!STATUS!", status);
                        LEAVE;
                    }
                }
                
                break;
            }

            if (RtlCompareMemory(&UsbCapability->CapabilityType,
                                 &GUID_USB_CAPABILITY_SELECTIVE_SUSPEND,
                                 sizeof(GUID)) == sizeof(GUID)) {
                if (OutputBuffer != NULL) {
                    status = STATUS_INVALID_PARAMETER;
                    LogError(FlagController, "Output buffer should be null for GUID_USB_CAPABILITY_SELECTIVE_SUSPEND (0x%p)",
                             UsbCapability);
                    LEAVE;
                }
                
                break;
            }

            if (RtlCompareMemory(&UsbCapability->CapabilityType,
                                 &GUID_USB_CAPABILITY_HIGH_BANDWIDTH_ISOCH,
                                 sizeof(GUID)) == sizeof(GUID)) {
                if (UsbCapability->OutputBufferLength != sizeof(ULONG)) {
                    status = STATUS_INVALID_PARAMETER;
                    LogError(FlagController, "Output Buffer Size should be sizeof(ULONG) for GUID_USB_CAPABILITY_HIGH_BANDWIDTH_ISOCH (0x%p)",
                             UsbCapability);
                    LEAVE;
                }

                break;
            }

            if (RtlCompareMemory(&UsbCapability->CapabilityType,
                                 &GUID_USB_CAPABILITY_STATIC_STREAMS,
                                 sizeof(GUID)) == sizeof(GUID)) {
                if (UsbCapability->OutputBufferLength != sizeof(USHORT)) {
                    status = STATUS_INVALID_PARAMETER;
                    LogError(FlagController, "Output Buffer Size should be sizeof(USHORT) for GUID_USB_CAPABILITY_STATIC_STREAMS (0x%p)",
                             UsbCapability);
                    LEAVE;
                }
                
                if (Ucx_FailDueToVerifierSetting(usbdiHandleData->UsbVerifierFailStaticStreamSupport)) {
                    status = Ucx_GetRandomErrorNTSTATUS();
                    LogInfo(FlagUsbdi, "UsbVerifier failing Static Stream Support, Status %!STATUS!", status);
                    LEAVE;
                }
                
                break;
            }

            // Unsupported Capability
            status = STATUS_INVALID_PARAMETER;
            LogError(FlagController, "Unsupported Capability (0x%p), {%08x-%04x-%04x-%02x%02x-%02x-%02x-%02x-%02x-%02x-%02x}",
                     UsbCapability,
                     UsbCapability->CapabilityType.Data1,
                     UsbCapability->CapabilityType.Data2,
                     UsbCapability->CapabilityType.Data3,
                     UsbCapability->CapabilityType.Data4[0],
                     UsbCapability->CapabilityType.Data4[1],
                     UsbCapability->CapabilityType.Data4[2],
                     UsbCapability->CapabilityType.Data4[3],
                     UsbCapability->CapabilityType.Data4[4],
                     UsbCapability->CapabilityType.Data4[5],
                     UsbCapability->CapabilityType.Data4[6],
                     UsbCapability->CapabilityType.Data4[7]);
        
        } WHILE(0);

        if (NULL == UcxControllerContext->Config.EvtControllerQueryUsbCapability) {
            status = STATUS_NOT_SUPPORTED;
            LogInfo(FlagController, "HCD does not provide EvtControllerQueryUsbCapability");
            LEAVE;
        } 


        status = UcxControllerContext->Config.EvtControllerQueryUsbCapability(
            UcxControllerContext->UcxController,
            &UsbCapability->CapabilityType,
            UsbCapability->OutputBufferLength,
            OutputBuffer,
            &UsbCapability->ResultLength);

        if (!NT_SUCCESS(status)) {
            LogError(FlagController, 
                     "EvtControllerQueryUsbCapability failed for Capability (0x%p), controller context(0x%p), status %!STATUS!",
                     UsbCapability, UcxControllerContext, status);

            LEAVE;
        }

        if (RtlCompareMemory(&UsbCapability->CapabilityType,
                             &GUID_USB_CAPABILITY_STATIC_STREAMS,
                             sizeof(GUID)) == sizeof(GUID)) {
            
            USHORT numStreams = * (PUSHORT)OutputBuffer;
            //
            // This has already been verified before
            //
            __assume(UsbCapability->OutputBufferLength == sizeof(USHORT));
            
            if (usbdiHandleData->UsbVerifierStaticStreamCountOverride != 0) {
                if (usbdiHandleData->StreamCountGranted != 0) {
                    //
                    // Report the same value as reported last time.
                    //
                    NT_ASSERT(usbdiHandleData->StreamCountGranted <= numStreams);
                    numStreams = (USHORT) min(usbdiHandleData->StreamCountGranted, numStreams);
                
                } else {

                    if (usbdiHandleData->UsbVerifierStaticStreamCountOverride == 0xFFFFFFFF) {
                        numStreams = (USHORT)Ucx_NonZeroRandomWithMax(numStreams);
                    } else if (usbdiHandleData->UsbVerifierStaticStreamCountOverride < numStreams) {
                        numStreams = (USHORT)usbdiHandleData->UsbVerifierStaticStreamCountOverride;
                    }
                }
                *(PUSHORT)OutputBuffer = numStreams;
            }

            usbdiHandleData->StreamSupportGranted = TRUE;
            usbdiHandleData->StreamCountGranted = numStreams;
            
            LogInfo(FlagController, "Stream Support Granted (%d), Capabiltiy 0x%p", numStreams, UsbCapability);
        
        } else if (RtlCompareMemory(&UsbCapability->CapabilityType,
                                    &GUID_USB_CAPABILITY_CHAINED_MDLS,
                                    sizeof(GUID)) == sizeof(GUID)) {
            usbdiHandleData->ChainedMdlSupportGranted = TRUE;        
        }

    } FINALLY {
        
        WdfRequestComplete(Request, status);
    }
}

VOID
Controller_GetDumpData(
    __in 
        PUCXCONTROLLER_PRIVCONTEXT                UcxControllerContext,
    __in
        UCXUSBDEVICE                              UcxUsbDevice,
    __in
        PUSB_DUMP_DEVICE_INFO                     DumpDeviceInfo,
    __inout
        PUSB_DUMP_CONTROLLER_INFO                 DumpControllerInfo,
    __inout
        WDFREQUEST                                Request
    )
/*++

Routine Description:

    Handles a Crashdump initialization request from the client driver.

Arguments:

    UcxControllerContext - Ucx context for the UCXCONTOLLER object.
    
    UcxUsbDevice - The handle to the UCXUSBDEVICE representing the Crashdump
        target disk.

    DumpDeviceInfo - Storage specific device information supplied by client
        driver.

    DumpControllerInfo - Crashdump related controller information to be returned
        to the client driver.
         
    Request - WDFREQUEST representing this request.         
    
--*/
{
    NTSTATUS    status;

    TRY {

        if (NULL == UcxControllerContext->Config.EvtControllerGetDumpData) {
            status = STATUS_NOT_SUPPORTED;
            LogInfo(FlagController, "HCD did not provide EvtControllerGetDumpData");
            LEAVE;
        }

        status = UcxControllerContext->Config.EvtControllerGetDumpData(
            UcxControllerContext->UcxController,
            UcxUsbDevice,
            DumpDeviceInfo,
            DumpControllerInfo);

        if (!NT_SUCCESS(status)) {
            LogError(FlagController,
                     "EvtControllerGetDumpData failed for controller context(0x%p), status %!STATUS!",
                     UcxControllerContext, status);
            LEAVE;
        }

    } FINALLY {
        
        WdfRequestComplete(Request, status);
    }
}

VOID
Controller_FreeDumpData(
    __in 
        PUCXCONTROLLER_PRIVCONTEXT                UcxControllerContext,
    __inout
        PUSB_DUMP_CONTROLLER_INFO                 DumpControllerInfo,
    __inout
        WDFREQUEST                                Request
    )
/*++

Routine Description:

    Handles a Crashdump cleanup request from the client driver.

Arguments:

    UcxControllerContext - Ucx context for the UCXCONTOLLER object.

    DumpControllerInfo - Crashdump related controller information.
         
    Request - WDFREQUEST representing this request.         
    
--*/
{
    NTSTATUS    status;

    TRY {

        if (NULL == UcxControllerContext->Config.EvtControllerFre