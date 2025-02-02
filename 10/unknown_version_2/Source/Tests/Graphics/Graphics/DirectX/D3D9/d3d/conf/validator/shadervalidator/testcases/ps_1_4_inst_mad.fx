try - Pointer to the device entry.  Will be filled with all 
                  information for the device.

Return Value:

    STATUS_SUCCESS when successful.
    STATUS_NO_SUCH_DEVICE when the requested device could not be found.
    STATUS_NO_MEMORY if allocation fails for an internal data structure.
    
--*/

{

    NTSTATUS Status;

    //
    // On EFI machines, it is easiest to process all block i/o devices at once.
    // They all share a common interface and can be obtained by querying for
    // devices that support the block i/o protocol.
    //

    Status = BlockIoFirmwareOpen(Device, DeviceEntry->DeviceData);
    return Status;
}

NTSTATUS
DiskFirmwareClose (
    __in PBLOCK_IO_DEVICE BlockIoDevice
    )

/*++

Routine Description:

    Closes an EFI disk device.  
     
    On EFI 1.10 systems this is to call CloseProtocol to notify the EFI handle
    database that there the handle is no longer being used.

Arguments:

    DeviceEntry - Block I/O device structure.

Return Value:

    STATUS_SUCCESS when successful.
    STATUS_INVALID_PARAMETER if the stored firmware handle is no longer valid.
    STATUS_NOT_FOUND if the firmware handle no longer supports the block i/o
        protocol.
        
    Both error cases result in some sort of internal error (Handle is 
    corrupted).
    
--*/

{

    PDISK_INTERNAL_DATA DiskData;

    //
    // Release partition table, if allocated.
    //

#ifdef ENABLE_PATCHED_PARTITIONS

    DiskData = &BlockIoDevice->InternalData.u.DiskData;
    if (DiskData->GptData.PartitionEntryArray != NULL) {
        BlMmFreeHeap(DiskData->GptData.PartitionEntryArray);
        DiskData->GptData.PartitionEntryArray = NULL;
    }

#endif

    return EfiCloseProtocol(BlockIoDevice->FirmwareData.Handle,
                            &EfiBlockIoProtocol,
                            BlockIoDevice->FirmwareData.BlockIoInterface);
}


/*++

Copyright (c) 2011  Microsoft Corporation

Module Name:

    usbbugcode.h

Abstract:

    Contains declarations of USB specific bug check codes

Environment:

    Kernel mode

Revision History:

    06-17-11 : moved bug codes to separate header file from dbg.h

--*/

#pragma once

/***********
USB BUGCODES

Parameter 1 to the USB bugcheck is always BUGCODE_USB_DRIVER
************/

//
// USBBUGCODE_INTERNAL_ERROR
// An internal error has occurred in the USB stack
// -- we will eventually never throw these instead
// -- we will find a more graceful way to handle them
//

#define USBBUGCODE_INTERNAL_ERROR               1

//
// USBBUGCODE_BAD_URB
// The USB client driver has submitted a URB that is already attached to another 
// irp pending in the bus driver.
//
// parameter 2 = address of other IRP 
// parameter 3 = address of IRP passed in
// parameter 4 = address URB that caused the error
//

#define USBBUGCODE_BAD_URB                      2

//
// USBBUGCODE_MINIPORT_ERROR
// The USB miniport driver has generated a bugcheck.
// This is usually in response to catastrophic hardware
// failure.
//
// parameter 2 = PCI Vendor,Product id for the controller
// parameter 3 = pointer to usbport.sys debug log
//

#define USBBUGCODE_MINIPORT_ERROR               3

//
// USBBUGCODE_DOUBLE_SUBMIT
// USBBUGCODE_IRP_DOUBLE_SUBMIT
// The USB client driver has submitted a IRP that is attached to a transfer and is 
// still pending in bus driver.  If the bus driver does not set a cancel routine 
// the the system would have buchecked when the irp completes.
//
// *function driver should still be on the stack. 
//
// parameter 2 = address of IRP
// parameter 3 = address URB that caused the error
// parameter 4 = transfer structure 
//

#define USBBUGCODE_DOUBLE_SUBMIT                USBBUGCODE_IRP_DOUBLE_SUBMIT
#define USBBUGCODE_IRP_DOUBLE_SUBMIT            4

// USBBUGCODE_INVALID_PHYSICAL_ADDRESS
//
//
// parameter 2 = device extension ptr for the host controller
// parameter 3 = PCI Vendor,Product id for the controller
// parameter 4 = ptr to Endpoint data structure
//

#define USBBUGCODE_INVALID_PHYSICAL_ADDRESS     5

// USBBUGCODE_BAD_SIGNATURE
//
// Internal data structure (object) is corrupted
//
// parameter 2 = object address
// parameter 3 = signature expected
// 
//

#define USBBUGCODE_BAD_SIGNATURE                6

// USBBUGCODE_MINIPORT_ASSERT_FAILURE
//
// Internal data structure (object) is corrupted
//
// parameter 2 = pointer to usbport.sys debug log
// parameter 3 = msg string
// parameter 4 = file name
// 
//

#define USBBUGCODE_MINIPORT_ASSERT_FAILURE      7

#define USBBUGCODE_RESERVED_USBHUB              8 

// USBBUGCODE_RESERVED_DUMP
//
// A critical error was hit in the dump miniport driver
//
// parameter 2 = dmpusbstor or uaspstor driver error code
// parameter 3 = failure status code
// parameter 4 = dmpusbstor or uaspstor extension
//

#define USBBUGCODE_RESERVED_DUMP                9

// USBBUGCODE_BOOT_DEVICE_RESET_FAILED
//
// A critical error was hit in the hub driver while resetting the device
//
// parameter 2 = driver error code
// parameter 3 = hub state context
// parameter 4 = hub extension
//
#define USBBUGCODE_BOOT_DEVICE_RESET_FAILED     10


typedef struct _XHCI_LIVEDUMP_CONTEXT {

    CHAR VendorId[5];
    CHAR DeviceId[5];
    CHAR RevisionId[5];
    CHAR FirmwareVersion[17];

    PVOID ControllerHandle;
    PVOID UsbDeviceHandle;
    PVOID EndpointHandle;
    PVOID TransferRingHandle;

} XHCI_LIVEDUMP_CONTEXT, *PXHCI_LIVEDUMP_CONTEXT;

typedef enum _FailedBootDeviceType {

    FailedBootDeviceType_UsbStorageDevice   = 1,
    FailedBootDeviceType_UsbHub             = 2,
    FailedBootDeviceType_UsbController      = 3

} FailedBootDeviceType;


typedef struct _USBHUB3_LIVEDUMP_CONTEXT {

    CHAR VendorId[5];
    CHAR ProductId[5];
    CHAR BcdDevice[5];

    PVOID HubFdoContext;
    PVOID DeviceContext;
    PVOID PortContext;

} USBHUB3_LIVEDUMP_CONTEXT, *PUSBHUB3_LIVEDUMP_CONTEXT;

/*++

USB3 BUGCODES

***************************************************************************************************
 NOTE: If you make any changes to the following, please update the following files:

     minkernel\ntos\nls\bugcodes.w

***************************************************************************************************

    Parameter 1: This is always the USB3 bugcheck code.
    The values of other parameters are dependent on Parameter 1.

--*/

typedef enum _USB3_BUGCHECK_REASON_CODE {

    USB3_BUGCODE_CLIENT_USED_AN_ACTIVE_URB = 1,
    /*++
    Parameter 1 : USB3_BUGCODE_CLIENT_USED_AN_ACTIVE_URB - A client driver used an URB that it had
        previously sent to the core stack.
    Parameter 2 : Irp (Optional), if a client driver resent an Urb to the core stack, this field
       represents the Irp used to resend the Urb to the core stack.
    Parameter 3 : Urb, The pointer to the Urb that has been re-used
    Parameter 4 : Client's Device Object
    --*/

    USB3_BUGCODE_BOOT_DEVICE_FAILED = 2,
    /*++
    Parameter 1 : USB3_BUGCODE_BOOT_DEVICE_FAILED - Boot Or Paging Device failed re-enumeration
    Parameter 2 : PDO for the failed device
    Parameter 3 : Type of failed device - storage device, hub, or controller.
    Parameter 4:  0
    --*/

    USB3_BUGCODE_CLIENT_CORRUPTED_XRB = 3,
    /*++
    Parameter 1 : USB3_BUGCODE_CLIENT_CORRUPTED_URB - A client driver has sent a corrupted URB to the
        core stack. This can happen because of the following reasons:
        - Client did not allocate the Urb using USBD_xxxUrbAllocate Routines
        - Client did a buffer under-run for the Urb
    Parameter 2 : Irp (Optional), if a client driver sent the corrupted Urb to the core stack,
       this field represents the Irp used to resend the Urb to the core stack.
    Parameter 3 : Urb, The pointer to the Urb that has been corrupted
    Parameter 4 : Client's Device Object
    --*/

    USB3_VERIFIER_BUGCODE_OPEN_STATIC_STREAMS_WITH_IRQL_TOO_HIGH = 0x800,
    /*++
    Parameter 1 : USB3_VERIFIER_BUGCODE_OPEN_STATIC_STREAMS_IRQL_TOO_HIGH - Open Streams request should only be
        called at PASSIVE LEVEL
    Parameter 2 : IRQL at which the open static streams request was sent
    Parameter 2 : Irp, Pointer to the Open Static Streams Irp
    Parameter 4:  Client's Device Object
    --*/

    USB3_VERIFIER_BUGCODE_OPEN_STATIC_STREAMS_WITHOUT_QUERYING_FOR_CAPABILITY = 0x801,
    /*++
    Parameter 1 : USB3_VERIFIER_BUGCODE_OPEN_STREAMS_WITHOUT_QUERYING_FOR_CAPABILITY - Before a client
        driver can send a USB3_VERIFIER_BUGCODE_OPEN_STREAMS_WITHOUT_QUERYING_FOR_CAPABILITY, it must
        query for streams capability. Only if the query is successful the client driver must attempt to
        open static streams.
    Parameter 2 : Irp, Pointer to the Open Static Streams Irp
    Parameter 3 : Urb, The pointer to the Open Static Streams Urb
    Parameter 4:  Client's Device Object
    --*/

    USB3_VERIFIER_BUGCODE_OPEN_STATIC_STREAMS_INVALID_STREAM_COUNT = 0x802,
    /*++
    Parameter 1 : USB3_VERIFIER_BUGCODE_OPEN_STATIC_STREAMS_INVALID_STREAM_COUNT - Client driver
        is trying to open an invalid number of static streams. The Number of Streams cannot be 0,
        and cannot be greater than the value returned to the client driver by Get Capability call
    Parameter 2 : Number of Static Streams client driver is trying to open
    Parameter 3 : Number of static streams that were granted to the client driver
    Parameter 4 : Urb, The pointer to the Open Static Streams Urb
    --*/

    USB3_VERIFIER_BUGCODE_STATIC_STREAMS_ALREADY_OPEN = 0x803,
    /*++
    Parameter 1 : USB3_VERIFIER_BUGCODE_STATIC_STREAMS_ALREADY_OPEN - A client driver cannot open
        static streams for an endpoint for which static streams have already been opened. The client
        driver must first close the previously opened static streams.
    Parameter 2 : Irp, Pointer to the Open Static Streams Irp
    Parameter 3 : Urb, The pointer to the Open Static Streams Urb
    Parameter 4:  Client's Device Object
    --*/

    USB3_VERIFIER_BUGCODE_CLIENT_LEAKED_USBD_HANDLE = 0x804,
    /*++
    Parameter 1 : USB3_VERIFIER_BUGCODE_CLIENT_LEAKED_USBD_HANDLE - A client driver forgot to close
        a handle it created earlier using USBD_CreateHandle or forgot to free an Urb it allocated
        using that USBD_HANDLE.
    Parameter 2 : The Leaked Handle Context. Please run !usbanalyze -v to get information about the leaked
        Handle and Urbs.
        Please turn on Driver Verifier on the client driver to ensure information about Leaked Urbs is
        presented.
    Parameter 3 : Device Object passed into the call USBD_CreateHandle
    Parameter 4 : 0
    --*/

    USB3_VERIFIER_BUGCODE_STATIC_STREAMS_CLOSE_IN_INVALID_DEVICE_STATE = 0x805,
    /*++
    Parameter 1 : USB3_VERIFIER_BUGCODE_STATIC_STREAMS_CLOSE_IN_INVALID_DEVICE_STATE - A client driver
        sent a close static streams urb in an invalid state like after processing D0 Exit.
    Parameter 2 : WDFREQUEST Handle for the close streams urb
    Parameter 3 : Urb, Pointer to the Closer streams Urb
    Parameter 4 : Client's Device Object
    --*/

    USB3_VERIFIER_BUGCODE_CHAINED_MDL_WITHOUT_QUERYING_FOR_CAPABILITY = 0x806,
    /*++
    Parameter 1 :  USB3_VERIFIER_BUGCODE_CHAINED_MDL_WITHOUT_QUERYING_FOR_CAPABILITY - Before a client
        driver can send a Chained Mdl, it must
        query for chained mdl capability. Only if the query is successful the client driver must
        attempt to use chained Mdls.
    Parameter 2 : Irp, Pointer to the Irp
    Parameter 3 : Urb, Pointer to the Urb
    Parameter 4 : Client's Device Object
    --*/

    USB3_VERIFIER_BUGCODE_CLIENT_USED_CHAINED_MDL = 0x807,
    /*++
    Parameter 1 : USB3_VERIFIER_BUGCODE_CLIENT_USED_CHAINED_MDL - A client driver has sent an URB to the
        core stack with TransferBufferLength longer than the MmGetMdlByteCount of the MDL passed in.
        The client driver must use USBD_QueryUsbCapability and
        use the URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER_USING_CHAINED_MDL or
        URB_FUNCTION_ISOCH_TRANSFER_USING_CHAINED_MDL to send a chaned MDL:
    Parameter 2 : Mdl, the chained MDL.
    Parameter 3 : Urb, The pointer to the Urb that used the changed MDL
    Parameter 4 : (optional) Client's Device Object if available
    --*/

    //
    // USBXHCI Bugcheck Codes
    //

    USB3_WER_BUGCODE_USBXHCI_HOST_SYSTEM_ERROR = 0x1001,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_HOST_SYSTEM_ERROR - The xHCI controller asserted the HSE
        bit, which indicates a host system error.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_HOST_CONTROLLER_ERROR = 0x1002,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_HOST_CONTROLLER_ERROR - The xHCI controller asserted the HCE
        bit, which indicates a host controller error.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_ENDPOINT_ERROR_AFTER_STOP = 0x1003,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_ENDPOINT_ERROR_AFTER_STOP - The xHCI stop endpoint command
        returned an unhandled completion code.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_STOP = 0x1004,
    /*++
    Parameter 1 : USB3_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_STOP - The xHCI endpoint state received
        a context state error after an xHCI endpoint stop command was issued.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_STALL_SET_DEQUEUE = 0x1005,
    /*++
    Parameter 1 : USB3_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_STALL_SET_DEQUEUE - Set dequeue pointer
        failed during an attempt to clear stall on control endpoint.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_STALL_RESET_EP = 0x1006,
    /*++
    Parameter 1 : USB3_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_STALL_SET_DEQUEUE - Reset EP failed
        during an attempt to clear stall on control endpoint.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_RESET_RECOVERY_RESET_FAILED = 0x1007,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_RESET_RECOVERY_RESET_FAILED - The reset of the
        xHCI controller failed during reset recovery.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_RESET_RECOVERY_RESTART_FAILED = 0x1008,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_RESET_RECOVERY_RESTART_FAILED - The restart of the
        xHCI controller failed during reset recovery.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_COMMAND_ABORT_FAILED = 0x1009,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_COMMAND_ABORT_FAILED - An xHCI controller command
        failed to complete after the command timeout abort.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_SET_DEQUEUE = 0x100A,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_SET_DEQUEUE - Set dequeue pointer
        failed during an attempt to set the dequeue pointer after endpoint stop completion.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_RESET_RECOVERY_STOP_FAILED = 0x100B,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_RESET_RECOVERY_STOP_FAILED - The stop of the
        xHCI controller failed during reset recovery.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_FIRMWARE_NOT_SUPPORTED = 0x100C,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_FIRMWARE_NOT_SUPPORTED - The firmware in the
        xHCI controller is not supported. The xHCI driver will not load on this controller
        unless the firmware is updated.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_CONTROLLER_GONE = 0x100D,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_CONTROLLER_GONE - The controller was detected
        to be physically removed.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_STREAM_ENDPOINT_ERROR = 0x100E,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_STREAM_ENDPOINT_ERROR - The driver detect an
        error on a stream enabled endpoint.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_FIRMWARE_OUTDATED = 0x100F,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_FIRMWARE_OUTDATED - The firmware in the
        xHCI controller is outdated. The xHCI driver will continue working with this
        controller but may run into some issues. A firmware update is recommended.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_UNHANDLED_TRANSFER_EVENT_TRB_COMPLETION_CODE = 0x1010,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_UNHANDLED_COMPLETION_CODE - A transfer event
        TRB completed with an unhandled completion code.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_EVENT_RING_FULL_ERROR = 0x1011,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_EVENT_RING_FULL_ERROR - The controller
        reported that the event ring became full. The controller is also known to
        drop events when this happens.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_OUT_OF_ORDER_COMMAND_COMPLETION = 0x1012,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_OUT_OF_ORDER_COMMAND_COMPLETION - The
        controller completed a command out of order.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_DEQUEUEPOINTER_MISMATCH_AFTER_COMMAND_ABORT = 0x1013,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_DEQUEUEPOINTER_MISMATCH_AFTER_COMMAND_ABORT
        After command abort completion, the command ring dequeue pointer reported
        by the controller is incorrect.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_BAD_SLOTID = 0x1014,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_BAD_SLOTID
        After enable slot completion, controller gave us a bad slot id
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_SET_ADDRESS_FAILED_BSR1 = 0x1015,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_SET_ADDRESS_FAILED_BSR1
        Controller failed a SetAddress command with BSR1. That is unexpected.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_ENABLE_SLOT_FAILED = 0x1016,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_ENABLE_SLOT_FAILED           
        Controller failed to enable a slot during a usbdevice reset. This is unexpected. 
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_ENDPOINTS_DECONFIGURE_FAILED = 0x1017,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_ENDPOINTS_DECONFIGURE_FAILED
        Controller failed an endpoints configure command where we were deconfiguring the endpoints.
        That is unexpected.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_DISABLE_SLOT_FAILED = 0x1018,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_DISABLE_SLOT_FAILED
        Controller failed a disable slot command. That is unexpected.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_USBDEVICE_RESET_FAILED = 0x1019,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_USBDEVICE_RESET_FAILED
        Controller failed a USB device reset command. That is unexpected.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_RESET_SET_DEQUEUE = 0x101a,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_RESET_SET_DEQUEUE
        After endpoint reset, Set Dequeue Pointer command failed.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_ENDPOINT_ERROR_AFTER_RESET = 0x101b,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_ENDPOINT_ERROR_AFTER_RESET
        The xHCI reset endpoint command returned an unhandled completion code.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_D0ENTRY_FAILED = 0x101c,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_D0ENTRY_FAILED
        The D0Entry for xHCI failed.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_ENDPOINTS_RECONFIGURE_FAILED_ON_CANCEL = 0x101d,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_ENDPOINTS_RECONFIGURE_FAILED_ON_CANCEL
        Temporarily dropping and adding a stream endpoint (as 2 commands) failed, when using the
        Configure Endpoint command instead of Set Dequeue Pointer during request cancellation.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_INVALID_TRANSFER_EVENT_PTR_ED_1 = 0x101e,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_INVALID_TRANSFER_EVENT_PTR_ED_1
        The controller indicated a transfer completion that was not pending on the controller.
        EventData == 1 (dereferencing the Transfer Event TRB's pointer would have caused a bugcheck)
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_INVALID_TRANSFER_EVENT_PTR_ED_0 = 0x101f,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_INVALID_TRANSFER_EVENT_PTR_ED_0
        The controller indicated a transfer completion that was not pending on the controller.
        EventData == 0 (logical address in transfer event TRB not matched)
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_INVALID_TRANSFER_EVENT_PTR_ED_0_DUPLICATE = 0x1020,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_INVALID_TRANSFER_EVENT_PTR_ED_0_DUPLICATE
        The controller indicated a transfer completion that was not pending on the controller.
        EventData == 0 (logical address in transfer event TRB not matched)
        The Transfer Event TRB may be redundant (points somewhere near a recently completed request).
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_ENDPOINTS_RECONFIGURE_FAILED_ON_RESET = 0x1021,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_ENDPOINTS_RECONFIGURE_FAILED_ON_RESET
        Temporarily dropping and adding a stream endpoint (as 2 commands) failed, when using the
        Configure Endpoint command as part of resetting an endpoint that was not Halted.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_ENDPOINTS_RECONFIGURE_FAILED = 0x1022,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_ENDPOINTS_RECONFIGURE_FAILED
       Dropping and adding the same endpoint (as 1 command) failed.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_ENDPOINT_NOT_STOPPED_AFTER_STOP_ENDPOINT_SUCCESS = 0x1023,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_ENDPOINT_NOT_STOPPED_AFTER_STOP_ENDPOINT_SUCCESS
       After successful completion of Stop Endpoint command, endpoint state is not stopped.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_ISOCH_ENDPOINT_HALTED = 0x1024,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_ISOCH_ENDPOINT_HALTED
        For an Isoch Endpoint, a Stop Endpoint Command failed with Context State Error and Endpoint
        state is Halted.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBXHCI_TEST_HOOK_IGNORE = 0x103F,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBXHCI_TEST_HOOK_IGNORE
       For testing purposes. Ignore.
    Parameter 2 : XHCI_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    //
    // USBHUB3 bugcheck codes - range from 0x3000 to 0x301F
    //

    USB3_WER_BUGCODE_USBHUB3_HUB_RESET_SUCCESSFUL = 0x3000,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBHUB3_HUB_RESET_SUCCESSFUL - A misbehaving hub was successfully reset by the hub driver.
    Parameter 2 : USBHUB3_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBHUB3_HUB_RESET_UNSUCCESSFUL = 0x3001,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBHUB3_HUB_RESET_SUCCESSFUL - A misbehaving hub failed to be reset successfully by the hub driver.
    Parameter 2 : USBHUB3_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBHUB3_SUPERSPEED_DISABLED = 0x3002,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBHUB3_SUPERSPEED_DISABLED - A non-function SuperSpeed hub was disabled by the hub driver.
    Parameter 2 : USBHUB3_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

    USB3_WER_BUGCODE_USBHUB3_DEVICE_ENUMERATION_FAILURE = 0x3003,
    /*++
    Parameter 1 : USB3_WER_BUGCODE_USBHUB3_DEVICE_ENUMERATION_FAILURE - A USB device failed enumeration.
    Parameter 2 : USBHUB3_LIVEDUMP_CONTEXT
    Parameter 3 : 0
    Parameter 4 : 0
    --*/

} USB3_BUGCHECK_REASON_CODE;

//
// Following defintions are added for !analyze to create bucket names
//

typedef struct _USB3_BUGCODE_DATA
{
    DWORD BugCode;
    PCSTR BugCodeDesc;
} USB3_BUGCODE_DATA, *PUSB3_BUGCODE_DATA;

static USB3_BUGCODE_DATA bcd[] =
{
    USB3_BUGCODE_CLIENT_USED_AN_ACTIVE_URB,                                     "CLIENT_USED_AN_ACTIVE_URB",
    USB3_BUGCODE_BOOT_DEVICE_FAILED,                                            "BOOT_DEVICE_FAILED",
    USB3_BUGCODE_CLIENT_CORRUPTED_XRB,                                          "CLIENT_CORRUPTED_XRB",
    USB3_VERIFIER_BUGCODE_OPEN_STATIC_STREAMS_WITH_IRQL_TOO_HIGH,               "OPEN_STATIC_STREAMS_WITH_IRQL_TOO_HIGH",
    USB3_VERIFIER_BUGCODE_OPEN_STATIC_STREAMS_WITHOUT_QUERYING_FOR_CAPABILITY,  "OPEN_STATIC_STREAMS_WITHOUT_QUERYING_FOR_CAPABILITY",
    USB3_VERIFIER_BUGCODE_OPEN_STATIC_STREAMS_INVALID_STREAM_COUNT,             "OPEN_STATIC_STREAMS_INVALID_STREAM_COUNT",
    USB3_VERIFIER_BUGCODE_STATIC_STREAMS_ALREADY_OPEN,                          "STATIC_STREAMS_ALREADY_OPEN",
    USB3_VERIFIER_BUGCODE_CLIENT_LEAKED_USBD_HANDLE,                            "CLIENT_LEAKED_USBD_HANDLE",
    USB3_VERIFIER_BUGCODE_STATIC_STREAMS_CLOSE_IN_INVALID_DEVICE_STATE,         "STATIC_STREAMS_CLOSE_IN_INVALID_DEVICE_STATE",
    USB3_VERIFIER_BUGCODE_CHAINED_MDL_WITHOUT_QUERYING_FOR_CAPABILITY,          "CHAINED_MDL_WITHOUT_QUERYING_FOR_CAPABILITY",
    USB3_VERIFIER_BUGCODE_CLIENT_USED_CHAINED_MDL,                              "CLIENT_USED_CHAINED_MDL",
    USB3_WER_BUGCODE_USBXHCI_HOST_SYSTEM_ERROR,                                 "HOST_SYSTEM_ERROR",
    USB3_WER_BUGCODE_USBXHCI_HOST_CONTROLLER_ERROR,                             "HOST_CONTROLLER_ERROR",
    USB3_WER_BUGCODE_USBXHCI_ENDPOINT_ERROR_AFTER_STOP,                         "ENDPOINT_ERROR_AFTER_STOP",
    USB3_WER_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_STOP,                          "CONTEXT_ERROR_AFTER_STOP",
    USB3_WER_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_STALL_SET_DEQUEUE,             "CONTEXT_ERROR_AFTER_STALL_SET_DEQUEUE",
    USB3_WER_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_STALL_RESET_EP,                "CONTEXT_ERROR_AFTER_STALL_RESET_EP",
    USB3_WER_BUGCODE_USBXHCI_RESET_RECOVERY_RESET_FAILED,                       "RESET_RECOVERY_RESET_FAILED",
    USB3_WER_BUGCODE_USBXHCI_RESET_RECOVERY_RESTART_FAILED,                     "RESET_RECOVERY_RESTART_FAILED",
    USB3_WER_BUGCODE_USBXHCI_COMMAND_ABORT_FAILED,                              "COMMAND_ABORT_FAILED",
    USB3_WER_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_SET_DEQUEUE,                   "CONTEXT_ERROR_AFTER_SET_DEQUEUE",
    USB3_WER_BUGCODE_USBXHCI_RESET_RECOVERY_STOP_FAILED,                        "RESET_RECOVERY_STOP_FAILED",
    USB3_WER_BUGCODE_USBXHCI_FIRMWARE_NOT_SUPPORTED,                            "FIRMWARE_NOT_SUPPORTED",
    USB3_WER_BUGCODE_USBXHCI_CONTROLLER_GONE,                                   "CONTROLLER_GONE",
    USB3_WER_BUGCODE_USBXHCI_STREAM_ENDPOINT_ERROR,                             "STREAM_ENDPOINT_ERROR",
    USB3_WER_BUGCODE_USBXHCI_FIRMWARE_OUTDATED,                                 "XHCI_FIRMWARE_OUTDATED",
    USB3_WER_BUGCODE_USBXHCI_UNHANDLED_TRANSFER_EVENT_TRB_COMPLETION_CODE,      "UNHANDLED_TRANSFER_EVENT_TRB_COMPLETION_CODE",
    USB3_WER_BUGCODE_USBXHCI_EVENT_RING_FULL_ERROR,                             "EVENT_RING_FULL_ERROR",
    USB3_WER_BUGCODE_USBXHCI_OUT_OF_ORDER_COMMAND_COMPLETION,                   "OUT_OF_ORDER_COMMAND_COMPLETION",
    USB3_WER_BUGCODE_USBXHCI_DEQUEUEPOINTER_MISMATCH_AFTER_COMMAND_ABORT,       "DEQUEUEPOINTER_MISMATCH_AFTER_COMMAND_ABORT",
    USB3_WER_BUGCODE_USBXHCI_BAD_SLOTID,                                        "USB3_WER_BUGCODE_USBXHCI_BAD_SLOTID",
    USB3_WER_BUGCODE_USBXHCI_SET_ADDRESS_FAILED_BSR1,                           "SET_ADDRESS_FAILED_BSR1",
    USB3_WER_BUGCODE_USBXHCI_ENABLE_SLOT_FAILED,                                "ENABLE_SLOT_FAILED",
    USB3_WER_BUGCODE_USBXHCI_ENDPOINTS_DECONFIGURE_FAILED,                      "ENDPOINTS_DECONFIGURE_FAILED",
    USB3_WER_BUGCODE_USBXHCI_DISABLE_SLOT_FAILED,                               "DISABLE_SLOT_FAILED",
    USB3_WER_BUGCODE_USBXHCI_USBDEVICE_RESET_FAILED,                            "USBDEVICE_RESET_FAILED",
    USB3_WER_BUGCODE_USBXHCI_CONTEXT_ERROR_AFTER_RESET_SET_DEQUEUE,             "CONTEXT_ERROR_AFTER_RESET_SET_DEQUEUE",
    USB3_WER_BUGCODE_USBXHCI_ENDPOINT_ERROR_AFTER_RESET,                        "ENDPOINT_ERROR_AFTER_RESET",
    USB3_WER_BUGCODE_USBXHCI_D0ENTRY_FAILED,                                    "D0ENTRY_FAILED",
    USB3_WER_BUGCODE_USBXHCI_ENDPOINTS_RECONFIGURE_FAILED_ON_CANCEL,            "ENDPOINTS_RECONFIGURE_FAILED_ON_CANCEL",
    USB3_WER_BUGCODE_USBXHCI_INVALID_TRANSFER_EVENT_PTR_ED_1,                   "INVALID_TRANSFER_EVENT_PTR_ED_1",
    USB3_WER_BUGCODE_USBXHCI_INVALID_TRANSFER_EVENT_PTR_ED_0,                   "INVALID_TRANSFER_EVENT_PTR_ED_0",
    USB3_WER_BUGCODE_USBXHCI_INVALID_TRANSFER_EVENT_PTR_ED_0_DUPLICATE,         "INVALID_TRANSFER_EVENT_PTR_ED_0_DUPLICATE",
    USB3_WER_BUGCODE_USBXHCI_ENDPOINTS_RECONFIGURE_FAILED_ON_RESET,             "ENDPOINTS_RECONFIGURE_FAILED_ON_RESET",
    USB3_WER_BUGCODE_USBXHCI_ENDPOINTS_RECONFIGURE_FAILED,                      "ENDPOINTS_RECONFIGURE_FAILED",
    USB3_WER_BUGCODE_USBHUB3_HUB_RESET_SUCCESSFUL,                              "HUB_RESET_SUCCESSFUL",
    USB3_WER_BUGCODE_USBHUB3_HUB_RESET_UNSUCCESSFUL,                            "HUB_RESET_UNSUCCESSFUL",
    USB3_WER_BUGCODE_USBHUB3_SUPERSPEED_DISABLED,                               "USBHUB3_SUPERSPEED_DISABLED",
    USB3_WER_BUGCODE_USBHUB3_DEVICE_ENUMERATION_FAILURE,                        "USBHUB3_DEVICE_ENUMERATION_FAILURE"
};
#include "Test_Include.fx"

int Test_Count = 199;

string TestInfo
<
	string TestType = "PS";
>
= "ps_1_4_inst_mad";

PS_CRITERIA PS_001_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_001_Desc = "ps_1_4 : mad source reg combination v0, v1, r0 is allowed";
string PS_001 = 
	"ps_1_4 "
	"mov r0, c0 "
	"mad r0, v0, v1, r0 ";

PS_CRITERIA PS_002_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_002_Desc = "ps_1_4 : mad source reg combination v0, v1, c0 is allowed";
string PS_002 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mad r0, v0, v1, c0 "
	"mov r0, c0 ";

PS_CRITERIA PS_003_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_003_Desc = "ps_1_4 : mad source reg combination v0, r0, v1 is allowed";
string PS_003 = 
	"ps_1_4 "
	"mov r0, c0 "
	"mad r0, v0, r0, v1 ";

PS_CRITERIA PS_004_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_004_Desc = "ps_1_4 : mad source reg combination v0, r0, r1 is allowed";
string PS_004 = 
	"ps_1_4 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, v0, r0, r1 ";

PS_CRITERIA PS_005_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_005_Desc = "ps_1_4 : mad source reg combination v0, r0, c0 is allowed";
string PS_005 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mad r0, v0, r0, c0 ";

PS_CRITERIA PS_006_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_006_Desc = "ps_1_4 : mad source reg combination v0, c0, v1 is allowed";
string PS_006 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mad r0, v0, c0, v1 "
	"mov r0, c0 ";

PS_CRITERIA PS_007_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_007_Desc = "ps_1_4 : mad source reg combination v0, c0, r0 is allowed";
string PS_007 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mad r0, v0, c0, r0 ";

PS_CRITERIA PS_008_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_008_Desc = "ps_1_4 : mad source reg combination v0, c0, c1 is allowed";
string PS_008 = 
	"ps_1_4 "
	"def c1, 1, 1, 1, 1 "
	"def c0, 1, 1, 1, 1 "
	"mad r0, v0, c0, c1 "
	"mov r0, c0 ";

PS_CRITERIA PS_009_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_009_Desc = "ps_1_4 : mad source reg combination r0, v0, v1 is allowed";
string PS_009 = 
	"ps_1_4 "
	"mov r0, c0 "
	"mad r0, r0, v0, v1 ";

PS_CRITERIA PS_010_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_010_Desc = "ps_1_4 : mad source reg combination r0, v0, r1 is allowed";
string PS_010 = 
	"ps_1_4 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0, v0, r1 ";

PS_CRITERIA PS_011_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_011_Desc = "ps_1_4 : mad source reg combination r0, v0, c0 is allowed";
string PS_011 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mad r0, r0, v0, c0 ";

PS_CRITERIA PS_012_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_012_Desc = "ps_1_4 : mad source reg combination r0, r1, v0 is allowed";
string PS_012 = 
	"ps_1_4 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0, r1, v0 ";

PS_CRITERIA PS_013_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_013_Desc = "ps_1_4 : mad source reg combination r0, r1, r2 is allowed";
string PS_013 = 
	"ps_1_4 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mov r2, c0 "
	"mad r0, r0, r1, r2 ";

PS_CRITERIA PS_014_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_014_Desc = "ps_1_4 : mad source reg combination r0, r1, c0 is allowed";
string PS_014 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0, r1, c0 ";

PS_CRITERIA PS_015_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_015_Desc = "ps_1_4 : mad source reg combination r0, c0, v0 is allowed";
string PS_015 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mad r0, r0, c0, v0 ";

PS_CRITERIA PS_016_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_016_Desc = "ps_1_4 : mad source reg combination r0, c0, r1 is allowed";
string PS_016 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0, c0, r1 ";

PS_CRITERIA PS_017_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_017_Desc = "ps_1_4 : mad source reg combination r0, c0, c1 is allowed";
string PS_017 = 
	"ps_1_4 "
	"def c1, 1, 1, 1, 1 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mad r0, r0, c0, c1 ";

PS_CRITERIA PS_018_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_018_Desc = "ps_1_4 : mad source reg combination c0, v0, v1 is allowed";
string PS_018 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mad r0, c0, v0, v1 "
	"mov r0, c0 ";

PS_CRITERIA PS_019_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_019_Desc = "ps_1_4 : mad source reg combination c0, v0, r0 is allowed";
string PS_019 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mad r0, c0, v0, r0 ";

PS_CRITERIA PS_020_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_020_Desc = "ps_1_4 : mad source reg combination c0, v0, c1 is allowed";
string PS_020 = 
	"ps_1_4 "
	"def c1, 1, 1, 1, 1 "
	"def c0, 1, 1, 1, 1 "
	"mad r0, c0, v0, c1 "
	"mov r0, c0 ";

PS_CRITERIA PS_021_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_021_Desc = "ps_1_4 : mad source reg combination c0, r0, v0 is allowed";
string PS_021 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mad r0, c0, r0, v0 ";

PS_CRITERIA PS_022_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_022_Desc = "ps_1_4 : mad source reg combination c0, r0, r1 is allowed";
string PS_022 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, c0, r0, r1 ";

PS_CRITERIA PS_023_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_023_Desc = "ps_1_4 : mad source reg combination c0, r0, c1 is allowed";
string PS_023 = 
	"ps_1_4 "
	"def c1, 1, 1, 1, 1 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mad r0, c0, r0, c1 ";

PS_CRITERIA PS_024_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_024_Desc = "ps_1_4 : mad source reg combination c0, c1, v0 is allowed";
string PS_024 = 
	"ps_1_4 "
	"def c1, 1, 1, 1, 1 "
	"def c0, 1, 1, 1, 1 "
	"mad r0, c0, c1, v0 "
	"mov r0, c0 ";

PS_CRITERIA PS_025_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_025_Desc = "ps_1_4 : mad source reg combination c0, c1, r0 is allowed";
string PS_025 = 
	"ps_1_4 "
	"def c1, 1, 1, 1, 1 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mad r0, c0, c1, r0 ";

PS_CRITERIA PS_026_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_026_Desc = "ps_1_4 : mad source reg combination r0_bias, c0, r1_bias is allowed";
string PS_026 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bias, c0, r1_bias ";

PS_CRITERIA PS_027_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_027_Desc = "ps_1_4 : mad source reg combination r0_bias, c0, 1 - r1 is allowed";
string PS_027 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bias, c0, 1 - r1 ";

PS_CRITERIA PS_028_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_028_Desc = "ps_1_4 : mad source reg combination r0_bias, c0, -r1 is allowed";
string PS_028 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bias, c0, -r1 ";

PS_CRITERIA PS_029_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_029_Desc = "ps_1_4 : mad source reg combination r0_bias, c0, r1_x2 is allowed";
string PS_029 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bias, c0, r1_x2 ";

PS_CRITERIA PS_030_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_030_Desc = "ps_1_4 : mad source reg combination r0_bias, c0, r1_bx2 is allowed";
string PS_030 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bias, c0, r1_bx2 ";

PS_CRITERIA PS_031_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_031_Desc = "ps_1_4 : mad source reg combination r0_bias, c0, -r1_x2 is allowed";
string PS_031 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bias, c0, -r1_x2 ";

PS_CRITERIA PS_032_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_032_Desc = "ps_1_4 : mad source reg combination r0_bias, c0, -r1_bx2 is allowed";
string PS_032 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bias, c0, -r1_bx2 ";

PS_CRITERIA PS_033_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_033_Desc = "ps_1_4 : mad source reg combination r0_bias, c0, r1 is allowed";
string PS_033 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bias, c0, r1 ";

PS_CRITERIA PS_034_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_034_Desc = "ps_1_4 : mad source reg combination 1 - r0, c0, r1_bias is allowed";
string PS_034 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, 1 - r0, c0, r1_bias ";

PS_CRITERIA PS_035_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_035_Desc = "ps_1_4 : mad source reg combination 1 - r0, c0, 1 - r1 is allowed";
string PS_035 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, 1 - r0, c0, 1 - r1 ";

PS_CRITERIA PS_036_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_036_Desc = "ps_1_4 : mad source reg combination 1 - r0, c0, -r1 is allowed";
string PS_036 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, 1 - r0, c0, -r1 ";

PS_CRITERIA PS_037_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_037_Desc = "ps_1_4 : mad source reg combination 1 - r0, c0, r1_x2 is allowed";
string PS_037 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, 1 - r0, c0, r1_x2 ";

PS_CRITERIA PS_038_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_038_Desc = "ps_1_4 : mad source reg combination 1 - r0, c0, r1_bx2 is allowed";
string PS_038 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, 1 - r0, c0, r1_bx2 ";

PS_CRITERIA PS_039_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_039_Desc = "ps_1_4 : mad source reg combination 1 - r0, c0, -r1_x2 is allowed";
string PS_039 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, 1 - r0, c0, -r1_x2 ";

PS_CRITERIA PS_040_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_040_Desc = "ps_1_4 : mad source reg combination 1 - r0, c0, -r1_bx2 is allowed";
string PS_040 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, 1 - r0, c0, -r1_bx2 ";

PS_CRITERIA PS_041_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_041_Desc = "ps_1_4 : mad source reg combination 1 - r0, c0, r1 is allowed";
string PS_041 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, 1 - r0, c0, r1 ";

PS_CRITERIA PS_042_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_042_Desc = "ps_1_4 : mad source reg combination -r0, c0, r1_bias is allowed";
string PS_042 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0, c0, r1_bias ";

PS_CRITERIA PS_043_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_043_Desc = "ps_1_4 : mad source reg combination -r0, c0, 1 - r1 is allowed";
string PS_043 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0, c0, 1 - r1 ";

PS_CRITERIA PS_044_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_044_Desc = "ps_1_4 : mad source reg combination -r0, c0, -r1 is allowed";
string PS_044 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0, c0, -r1 ";

PS_CRITERIA PS_045_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_045_Desc = "ps_1_4 : mad source reg combination -r0, c0, r1_x2 is allowed";
string PS_045 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0, c0, r1_x2 ";

PS_CRITERIA PS_046_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_046_Desc = "ps_1_4 : mad source reg combination -r0, c0, r1_bx2 is allowed";
string PS_046 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0, c0, r1_bx2 ";

PS_CRITERIA PS_047_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_047_Desc = "ps_1_4 : mad source reg combination -r0, c0, -r1_x2 is allowed";
string PS_047 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0, c0, -r1_x2 ";

PS_CRITERIA PS_048_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_048_Desc = "ps_1_4 : mad source reg combination -r0, c0, -r1_bx2 is allowed";
string PS_048 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0, c0, -r1_bx2 ";

PS_CRITERIA PS_049_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_049_Desc = "ps_1_4 : mad source reg combination -r0, c0, r1 is allowed";
string PS_049 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0, c0, r1 ";

PS_CRITERIA PS_050_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_050_Desc = "ps_1_4 : mad source reg combination r0_x2, c0, r1_bias is allowed";
string PS_050 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_x2, c0, r1_bias ";

PS_CRITERIA PS_051_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_051_Desc = "ps_1_4 : mad source reg combination r0_x2, c0, 1 - r1 is allowed";
string PS_051 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_x2, c0, 1 - r1 ";

PS_CRITERIA PS_052_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_052_Desc = "ps_1_4 : mad source reg combination r0_x2, c0, -r1 is allowed";
string PS_052 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_x2, c0, -r1 ";

PS_CRITERIA PS_053_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_053_Desc = "ps_1_4 : mad source reg combination r0_x2, c0, r1_x2 is allowed";
string PS_053 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_x2, c0, r1_x2 ";

PS_CRITERIA PS_054_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_054_Desc = "ps_1_4 : mad source reg combination r0_x2, c0, r1_bx2 is allowed";
string PS_054 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_x2, c0, r1_bx2 ";

PS_CRITERIA PS_055_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_055_Desc = "ps_1_4 : mad source reg combination r0_x2, c0, -r1_x2 is allowed";
string PS_055 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_x2, c0, -r1_x2 ";

PS_CRITERIA PS_056_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_056_Desc = "ps_1_4 : mad source reg combination r0_x2, c0, -r1_bx2 is allowed";
string PS_056 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_x2, c0, -r1_bx2 ";

PS_CRITERIA PS_057_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_057_Desc = "ps_1_4 : mad source reg combination r0_x2, c0, r1 is allowed";
string PS_057 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_x2, c0, r1 ";

PS_CRITERIA PS_058_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_058_Desc = "ps_1_4 : mad source reg combination r0_bx2, c0, r1_bias is allowed";
string PS_058 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bx2, c0, r1_bias ";

PS_CRITERIA PS_059_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_059_Desc = "ps_1_4 : mad source reg combination r0_bx2, c0, 1 - r1 is allowed";
string PS_059 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bx2, c0, 1 - r1 ";

PS_CRITERIA PS_060_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_060_Desc = "ps_1_4 : mad source reg combination r0_bx2, c0, -r1 is allowed";
string PS_060 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bx2, c0, -r1 ";

PS_CRITERIA PS_061_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_061_Desc = "ps_1_4 : mad source reg combination r0_bx2, c0, r1_x2 is allowed";
string PS_061 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bx2, c0, r1_x2 ";

PS_CRITERIA PS_062_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_062_Desc = "ps_1_4 : mad source reg combination r0_bx2, c0, r1_bx2 is allowed";
string PS_062 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bx2, c0, r1_bx2 ";

PS_CRITERIA PS_063_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_063_Desc = "ps_1_4 : mad source reg combination r0_bx2, c0, -r1_x2 is allowed";
string PS_063 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bx2, c0, -r1_x2 ";

PS_CRITERIA PS_064_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_064_Desc = "ps_1_4 : mad source reg combination r0_bx2, c0, -r1_bx2 is allowed";
string PS_064 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bx2, c0, -r1_bx2 ";

PS_CRITERIA PS_065_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_065_Desc = "ps_1_4 : mad source reg combination r0_bx2, c0, r1 is allowed";
string PS_065 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, r0_bx2, c0, r1 ";

PS_CRITERIA PS_066_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_066_Desc = "ps_1_4 : mad source reg combination -r0_x2, c0, r1_bias is allowed";
string PS_066 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0_x2, c0, r1_bias ";

PS_CRITERIA PS_067_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_067_Desc = "ps_1_4 : mad source reg combination -r0_x2, c0, 1 - r1 is allowed";
string PS_067 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0_x2, c0, 1 - r1 ";

PS_CRITERIA PS_068_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_068_Desc = "ps_1_4 : mad source reg combination -r0_x2, c0, -r1 is allowed";
string PS_068 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0_x2, c0, -r1 ";

PS_CRITERIA PS_069_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_069_Desc = "ps_1_4 : mad source reg combination -r0_x2, c0, r1_x2 is allowed";
string PS_069 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0_x2, c0, r1_x2 ";

PS_CRITERIA PS_070_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_070_Desc = "ps_1_4 : mad source reg combination -r0_x2, c0, r1_bx2 is allowed";
string PS_070 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0_x2, c0, r1_bx2 ";

PS_CRITERIA PS_071_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_071_Desc = "ps_1_4 : mad source reg combination -r0_x2, c0, -r1_x2 is allowed";
string PS_071 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0_x2, c0, -r1_x2 ";

PS_CRITERIA PS_072_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_072_Desc = "ps_1_4 : mad source reg combination -r0_x2, c0, -r1_bx2 is allowed";
string PS_072 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0_x2, c0, -r1_bx2 ";

PS_CRITERIA PS_073_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_073_Desc = "ps_1_4 : mad source reg combination -r0_x2, c0, r1 is allowed";
string PS_073 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0_x2, c0, r1 ";

PS_CRITERIA PS_074_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_074_Desc = "ps_1_4 : mad source reg combination -r0_bx2, c0, r1_bias is allowed";
string PS_074 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0_bx2, c0, r1_bias ";

PS_CRITERIA PS_075_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_075_Desc = "ps_1_4 : mad source reg combination -r0_bx2, c0, 1 - r1 is allowed";
string PS_075 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0_bx2, c0, 1 - r1 ";

PS_CRITERIA PS_076_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_076_Desc = "ps_1_4 : mad source reg combination -r0_bx2, c0, -r1 is allowed";
string PS_076 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0_bx2, c0, -r1 ";

PS_CRITERIA PS_077_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_077_Desc = "ps_1_4 : mad source reg combination -r0_bx2, c0, r1_x2 is allowed";
string PS_077 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0_bx2, c0, r1_x2 ";

PS_CRITERIA PS_078_Criteria = { true, false, 0x0104, -1, -1, -1, -1, 0, -1, 0.f };
string PS_078_Desc = "ps_1_4 : mad source reg combination -r0_bx2, c0, r1_bx2 is allowed";
string PS_078 = 
	"ps_1_4 "
	"def c0, 1, 1, 1, 1 "
	"mov r0, c0 "
	"mov r1, c0 "
	"mad r0, -r0_bx2, c0, r1_bx2 ";

PS_