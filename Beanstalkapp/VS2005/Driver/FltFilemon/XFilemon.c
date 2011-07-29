#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>
#include <wchar.h>
#include "XFilemon.h"
#include "XFilemonCtrl.h"


#pragma prefast(disable:__WARNING_ENCODE_MEMBER_FUNCTION_POINTER, "Not valid for kernel mode drivers")


SCANNER_DATA ScannerData;

const UNICODE_STRING ScannerExtensionsToScan[] =
    { RTL_CONSTANT_STRING( L"doc"),
      RTL_CONSTANT_STRING( L"txt"),
      RTL_CONSTANT_STRING( L"bat"),
      RTL_CONSTANT_STRING( L"cmd"),
      RTL_CONSTANT_STRING( L"inf"),
      /*RTL_CONSTANT_STRING( L"ini"),   Removed, to much usage*/
      {0, 0, NULL}
    };


//
//  Function prototypes
//

NTSTATUS
ScannerPortConnect (
    __in PFLT_PORT ClientPort,
    __in_opt PVOID ServerPortCookie,
    __in_bcount_opt(SizeOfContext) PVOID ConnectionContext,
    __in ULONG SizeOfContext,
    __deref_out_opt PVOID *ConnectionCookie
    );

VOID
ScannerPortDisconnect (
    __in_opt PVOID ConnectionCookie
    );

NTSTATUS
ScannerpScanFileInUserMode (
    __in PFLT_INSTANCE Instance,
    __in PFILE_OBJECT FileObject,
    __out PBOOLEAN SafeToOpen
    );

BOOLEAN
ScannerpCheckExtension (
    __in PUNICODE_STRING Extension
    );

//
//  Assign text sections for each routine.
//

#ifdef ALLOC_PRAGMA
    #pragma alloc_text(INIT, DriverEntry)
    #pragma alloc_text(PAGE, ScannerInstanceSetup)
    #pragma alloc_text(PAGE, ScannerPreCreate)
    #pragma alloc_text(PAGE, ScannerPortConnect)
    #pragma alloc_text(PAGE, ScannerPortDisconnect)
#endif


//
//  Constant FLT_REGISTRATION structure for our filter.  This
//  initializes the callback routines our filter wants to register
//  for.  This is only used to register with the filter manager
//

const FLT_OPERATION_REGISTRATION Callbacks[] = {

    { IRP_MJ_CREATE,
      0,
      ScannerPreCreate,
      ScannerPostCreate},

/*
    { IRP_MJ_CLEANUP,
      0,
      ScannerPreCleanup,
      NULL},*/



	{ IRP_MJ_DIRECTORY_CONTROL,
	  0,
	  ScannerPreCreate,
	  ScannerPostCreate} ,


/*
    { IRP_MJ_WRITE,
      0,
      ScannerPreWrite,
      NULL},*/


    { IRP_MJ_OPERATION_END}
};


const FLT_CONTEXT_REGISTRATION ContextRegistration[] = {

    { FLT_STREAMHANDLE_CONTEXT,
      0,
      NULL,
      sizeof(SCANNER_STREAM_HANDLE_CONTEXT),
      'chBS' },

    { FLT_CONTEXT_END }
};

const FLT_REGISTRATION FilterRegistration = {

    sizeof( FLT_REGISTRATION ),         //  Size
    FLT_REGISTRATION_VERSION,           //  Version
    0,                                  //  Flags
    ContextRegistration,                //  Context Registration.
    Callbacks,                          //  Operation callbacks
    ScannerUnload,                      //  FilterUnload
    ScannerInstanceSetup,               //  InstanceSetup
    ScannerQueryTeardown,               //  InstanceQueryTeardown
    NULL,                               //  InstanceTeardownStart
    NULL,                               //  InstanceTeardownComplete
    NULL,                               //  GenerateFileName
    NULL,                               //  GenerateDestinationFileName
    NULL                                //  NormalizeNameComponent
};

////////////////////////////////////////////////////////////////////////////
//
//    Filter initialization and unload routines.
//
////////////////////////////////////////////////////////////////////////////

NTSTATUS
DriverEntry (
    __in PDRIVER_OBJECT DriverObject,
    __in PUNICODE_STRING RegistryPath
    )
{
    OBJECT_ATTRIBUTES oa;
    UNICODE_STRING uniString;
    PSECURITY_DESCRIPTOR sd;
    NTSTATUS status;
	UNICODE_STRING			usImagePath		= RTL_CONSTANT_STRING( L"ImagePath" );
	OBJECT_ATTRIBUTES		objFile;
	IO_STATUS_BLOCK			iostatus;

    UNREFERENCED_PARAMETER( RegistryPath );

    // 获取注册表
	RtlInitUnicodeString(&ScannerData.usModulePath, L"");
	status = MyGetKeyValue(RegistryPath, &usImagePath, &ScannerData.usModulePath);
	DbgPrint("[DriverEntry] (%X)%S\n", status, ScannerData.usModulePath.Buffer);
	// 打开文件
	ScannerData.hSysFile = NULL;
	status = MyGetVolumnPath(&ScannerData.usModulePath, &ScannerData.usVolumePath);
	DbgPrint("[DriverEntry] (%X)%S\n", status, ScannerData.usVolumePath.Buffer);

    status = FltRegisterFilter( DriverObject,
                                &FilterRegistration,
                                &ScannerData.Filter );

    if (!NT_SUCCESS( status )) {

        return status;
    }
    //
    //  Create a communication port.
    //

    RtlInitUnicodeString( &uniString, ScannerPortName );

    //
    //  We secure the port so only ADMINs & SYSTEM can acecss it.
    //

    status = FltBuildDefaultSecurityDescriptor( &sd, FLT_PORT_ALL_ACCESS );

    if (NT_SUCCESS( status )) {

        InitializeObjectAttributes( &oa,
                                    &uniString,
                                    OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                                    NULL,
                                    sd );

        status = FltCreateCommunicationPort( ScannerData.Filter,
                                             &ScannerData.ServerPort,
                                             &oa,
                                             NULL,
                                             ScannerPortConnect,
                                             ScannerPortDisconnect,
                                             NULL,
                                             1 );
        //
        //  Free the security descriptor in all cases. It is not needed once
        //  the call to FltCreateCommunicationPort() is made.
        //

        FltFreeSecurityDescriptor( sd );

        if (NT_SUCCESS( status )) {

            //
            //  Start filtering I/O.
            //

            status = FltStartFiltering( ScannerData.Filter );

            if (NT_SUCCESS( status )) {

                return STATUS_SUCCESS;
            }

            FltCloseCommunicationPort( ScannerData.ServerPort );
        }
    }

    FltUnregisterFilter( ScannerData.Filter );

    return status;
}


NTSTATUS
ScannerPortConnect (
    __in PFLT_PORT ClientPort,
    __in_opt PVOID ServerPortCookie,
    __in_bcount_opt(SizeOfContext) PVOID ConnectionContext,
    __in ULONG SizeOfContext,
    __deref_out_opt PVOID *ConnectionCookie
    )
{
    PAGED_CODE();

    UNREFERENCED_PARAMETER( ServerPortCookie );
    UNREFERENCED_PARAMETER( ConnectionContext );
    UNREFERENCED_PARAMETER( SizeOfContext);
    UNREFERENCED_PARAMETER( ConnectionCookie );

    ASSERT( ScannerData.ClientPort == NULL );
    ASSERT( ScannerData.UserProcess == NULL );

    //
    //  Set the user process and port.
    //

    ScannerData.UserProcess = PsGetCurrentProcess();
    ScannerData.ClientPort = ClientPort;

    DbgPrint( "!!! scanner.sys --- connected, port=0x%p\n", ClientPort );

    return STATUS_SUCCESS;
}


VOID
ScannerPortDisconnect(
     __in_opt PVOID ConnectionCookie
     )
/*++

Routine Description

    This is called when the connection is torn-down. We use it to close our
    handle to the connection

Arguments

    ConnectionCookie - Context from the port connect routine

Return value

    None

--*/
{
    UNREFERENCED_PARAMETER( ConnectionCookie );

    PAGED_CODE();

    DbgPrint( "!!! scanner.sys --- disconnected, port=0x%p\n", ScannerData.ClientPort );

    //
    //  Close our handle to the connection: note, since we limited max connections to 1,
    //  another connect will not be allowed until we return from the disconnect routine.
    //

    FltCloseClientPort( ScannerData.Filter, &ScannerData.ClientPort );

    //
    //  Reset the user-process field.
    //

    ScannerData.UserProcess = NULL;
}


NTSTATUS
ScannerUnload (
    __in FLT_FILTER_UNLOAD_FLAGS Flags
    )
/*++

Routine Description:

    This is the unload routine for the Filter driver.  This unregisters the
    Filter with the filter manager and frees any allocated global data
    structures.

Arguments:

    None.

Return Value:

    Returns the final status of the deallocation routines.

--*/
{
    UNREFERENCED_PARAMETER( Flags );
	DbgPrint("[ScannerUnload] Enter.\n");
    //
    //  Close the server port.
    //

    FltCloseCommunicationPort( ScannerData.ServerPort );

	if(NULL != ScannerData.hSysFile)
		ZwClose(ScannerData.hSysFile);
    //
    //  Unregister the filter
    //

    FltUnregisterFilter( ScannerData.Filter );

    return STATUS_SUCCESS;
}

NTSTATUS
ScannerInstanceSetup (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_SETUP_FLAGS Flags,
    __in DEVICE_TYPE VolumeDeviceType,
    __in FLT_FILESYSTEM_TYPE VolumeFilesystemType
    )
{
/*
   	WCHAR					szVolumn[32]	= {0};
   	WCHAR*					pPoint			= NULL;
   	UNICODE_STRING			usVolumn;
   	NTSTATUS				status;
   
   	ScannerData.fltVolume = NULL;
   	//wcscpy(szVolumn, L"D:");
   	// 	pPoint = wcsstr(szVolumn, L':');
   	// 	if(NULL != pPoint)
   	// 	{
   	// 		pPoint++;
   	// 		*pPoint = 0;
   	// 	}
   	usVolumn.MaximumLength = sizeof(szVolumn) / sizeof(szVolumn[0]);
   	usVolumn.Length = wcslen(szVolumn);
   	usVolumn.Buffer = szVolumn;
   	// ExAllocatePool(NonPagedPool, usVolumn.MaximumLength);
   	// 	RtlZeroMemory(usVolumn.Buffer, usVolumn.MaximumLength);
   	// 	RtlCopyMemory(usVolumn.Buffer, szVolumn, usVolumn.MaximumLength);
   	//status = FltGetVolumeFromName(ScannerData.Filter, &usVolumn, &ScannerData.fltVolume);
   
   //	DbgPrint("[DriverEntry] FltGetVolumeFromName %S, (0x%X) %d", usVolumn.Buffer, status, ScannerData.fltVolume);
   	// 	ExFreePool(usVolumn.Buffer);
      */
   
	
	UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );
    UNREFERENCED_PARAMETER( VolumeFilesystemType );

    PAGED_CODE();

    ASSERT( FltObjects->Filter == ScannerData.Filter );

	DbgPrint("[ScannerInstanceSetup] Enter.\n");
    //
    //  Don't attach to network volumes.
    //
    if (VolumeDeviceType == FILE_DEVICE_NETWORK_FILE_SYSTEM) {

       return STATUS_FLT_DO_NOT_ATTACH;
    }

	//FltGetVolumeName(FltObjects->Volume, &usVolumn, NULL); 
	DbgPrint("[ScannerInstanceSetup] Attach volumn[0x%x].\n", FltObjects->Volume);
    return STATUS_SUCCESS;
}

NTSTATUS
ScannerQueryTeardown (
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
    )
{
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );

	DbgPrint("[ScannerQueryTeardown] Enter.\n");
    return STATUS_SUCCESS;
}


FLT_PREOP_CALLBACK_STATUS
ScannerPreCreate (
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
    )
{
	PFLT_FILE_NAME_INFORMATION nameInfo;
	NTSTATUS status;


    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( CompletionContext );

    PAGED_CODE();

    if (IoThreadToProcess( Data->Thread ) == ScannerData.UserProcess) {

        DbgPrint( "!!! scanner.sys -- allowing create for trusted process \n" );

        return FLT_PREOP_SUCCESS_NO_CALLBACK;
    }


	//
	//  If this create was failing anyway, don't bother scanning now.
	//

	if (!NT_SUCCESS( Data->IoStatus.Status ) ||
		(STATUS_REPARSE == Data->IoStatus.Status)) 
	{
		//DbgPrint("[ScannerPreCreate] Data->IoStatus.Status:%d.\n", Data->IoStatus.Status);
		return FLT_PREOP_SUCCESS_NO_CALLBACK;
	}


	status = FltGetFileNameInformation( Data,
		FLT_FILE_NAME_NORMALIZED |
		FLT_FILE_NAME_QUERY_DEFAULT,
		&nameInfo );

	if (!NT_SUCCESS( status )) {

		return FLT_POSTOP_FINISHED_PROCESSING;
	}

	FltParseFileNameInformation( nameInfo );
	if(0 != PathIsWorkPath(nameInfo->Name.Buffer))
	{
// 		FltCancelFileOpen( FltObjects->Instance, FltObjects->FileObject );
// 
// 		Data->IoStatus.Status = STATUS_ACCESS_DENIED;
// 		Data->IoStatus.Information = 0;
		DbgPrint("[ScannerPreCreate] monitoer:%wZ \n", &nameInfo->Name);
		if(FlagOn( (FILE_CREATE<<24), Data->Iopb->Parameters.Create.Options ))
		{
			Data->IoStatus.Information = 0;
			Data->IoStatus.Status = STATUS_ACCESS_DENIED;
			return FLT_PREOP_COMPLETE;
		}
		FltReleaseFileNameInformation( nameInfo );
		return FLT_PREOP_SUCCESS_WITH_CALLBACK;
	}
	//DbgPrint("[ScannerPreCreate] not monitoer:%wZ \n", &nameInfo->Name);
	FltReleaseFileNameInformation( nameInfo );
	return FLT_PREOP_SUCCESS_NO_CALLBACK;
   // return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}

BOOLEAN
ScannerpCheckExtension (
    __in PUNICODE_STRING Extension
    )
{
    const UNICODE_STRING *ext;

    if (Extension->Length == 0) {

        return FALSE;
    }

    //
    //  Check if it matches any one of our static extension list
    //

    ext = ScannerExtensionsToScan;

    while (ext->Buffer != NULL) {

        if (RtlCompareUnicodeString( Extension, ext, TRUE ) == 0) {

            //
            //  A match. We are interested in this file
            //

            return TRUE;
        }
        ext++;
    }

    return FALSE;
}


FLT_POSTOP_CALLBACK_STATUS
ScannerPostCreate (
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __in_opt PVOID CompletionContext,
    __in FLT_POST_OPERATION_FLAGS Flags
    )
{
    PSCANNER_STREAM_HANDLE_CONTEXT scannerContext;
    FLT_POSTOP_CALLBACK_STATUS returnStatus = FLT_POSTOP_FINISHED_PROCESSING;
    PFLT_FILE_NAME_INFORMATION nameInfo;
    NTSTATUS status;
    BOOLEAN safeToOpen, scanFile;
	UNICODE_STRING			ustrLog		= RTL_CONSTANT_STRING( L"log" );

    UNREFERENCED_PARAMETER( CompletionContext );
    UNREFERENCED_PARAMETER( Flags );

    //
    //  If this create was failing anyway, don't bother scanning now.
    //
	// 文件未找到
// 	if(STATUS_OBJECT_NAME_NOT_FOUND == Data->IoStatus.Status)
// 	{
// 		DbgPrint("[ScannerPosCreate] create file:(0x%X)%wZ \n", FltObjects->Instance, &FltObjects->FileObject->FileName);
// 		// IoCancelFileOpen(FltObjects->FileObject->DeviceObject, FltObjects->FileObject);	
// 
// 		Data->IoStatus.Status = STATUS_ACCESS_DENIED;
// 		Data->IoStatus.Information = 0;
// 		//IoCompleteRequest()
// 		return FLT_POSTOP_FINISHED_PROCESSING;
// 	}

    if (!NT_SUCCESS( Data->IoStatus.Status ) ||
        (STATUS_REPARSE == Data->IoStatus.Status)) 
	{
		//DbgPrint("[ScannerPreCreate] Data->IoStatus.Status:%d.\n", Data->IoStatus.Status);
        return FLT_POSTOP_FINISHED_PROCESSING;
    }

	if(FltObjects->FileObject->WriteAccess)
	{
		FltCancelFileOpen( FltObjects->Instance, FltObjects->FileObject );
		DbgPrint("[ScannerPreCreate] CancelFile(0x%X):%wZ.\n", FltObjects->Instance, &FltObjects->FileObject->FileName);

		Data->IoStatus.Status = STATUS_ACCESS_DENIED;
		Data->IoStatus.Information = 0;

		return FLT_POSTOP_FINISHED_PROCESSING;
	}
	if(FltObjects->FileObject->DeleteAccess)
	{
		FltCancelFileOpen( FltObjects->Instance, FltObjects->FileObject );
		Data->IoStatus.Status = STATUS_ACCESS_DENIED;
		Data->IoStatus.Information = 0;

		return FLT_POSTOP_FINISHED_PROCESSING;
	}

	return FLT_POSTOP_FINISHED_PROCESSING;
    //
    //  Check if we are interested in this file.
    //

    status = FltGetFileNameInformation( Data,
                                        FLT_FILE_NAME_NORMALIZED |
                                            FLT_FILE_NAME_QUERY_DEFAULT,
                                        &nameInfo );

    if (!NT_SUCCESS( status )) {

        return FLT_POSTOP_FINISHED_PROCESSING;
    }

    FltParseFileNameInformation( nameInfo );
// 	DbgPrint("[ScannerPreCreate] Open File \"%S\".\n"
// 		, nameInfo->Name.Buffer);
	if(FltObjects->FileObject->WriteAccess && 0 != PathIsWorkPath(nameInfo->Name.Buffer))
	{
		FltCancelFileOpen( FltObjects->Instance, FltObjects->FileObject );

		Data->IoStatus.Status = STATUS_ACCESS_DENIED;
		Data->IoStatus.Information = 0;

		return FLT_POSTOP_FINISHED_PROCESSING;
	}
	FltReleaseFileNameInformation( nameInfo );
	return FLT_POSTOP_FINISHED_PROCESSING;
    //
    //  Check if the extension matches the list of extensions we are interested in
    //
	if(FltObjects->FileObject->WriteAccess)
	{
		DbgPrint("[ScannerPreCreate] Read file predir:\"%S\" name:\"%S\" volumn:\"%S\" final:\"%S\" stream:\"%S\".\n"
			, nameInfo->ParentDir.Buffer
			, nameInfo->Name.Buffer
			, nameInfo->Volume.Buffer
			, nameInfo->FinalComponent.Buffer
			, nameInfo->Stream.Buffer);
		if(0 == RtlCompareUnicodeString(&nameInfo->Extension, &ustrLog, TRUE))
		{
			FltCancelFileOpen( FltObjects->Instance, FltObjects->FileObject );

			Data->IoStatus.Status = STATUS_ACCESS_DENIED;
			Data->IoStatus.Information = 0;

			return FLT_POSTOP_FINISHED_PROCESSING;
		}
	}
    scanFile = ScannerpCheckExtension( &nameInfo->Extension );

    //
    //  Release file name info, we're done with it
    //

    FltReleaseFileNameInformation( nameInfo );

    if (!scanFile) {

        //
        //  Not an extension we are interested in
        //

        return FLT_POSTOP_FINISHED_PROCESSING;
    }

    (VOID) ScannerpScanFileInUserMode( FltObjects->Instance,
                                       FltObjects->FileObject,
                                       &safeToOpen );

    if (!safeToOpen) {

        //
        //  Ask the filter manager to undo the create.
        //

        DbgPrint( "!!! scanner.sys -- foul language detected in postcreate !!!\n" );

        DbgPrint( "!!! scanner.sys -- undoing create \n" );

        FltCancelFileOpen( FltObjects->Instance, FltObjects->FileObject );

        Data->IoStatus.Status = STATUS_ACCESS_DENIED;
        Data->IoStatus.Information = 0;

        returnStatus = FLT_POSTOP_FINISHED_PROCESSING;

    } else if (FltObjects->FileObject->WriteAccess) {

        //
        //
        //  The create has requested write access, mark to rescan the file.
        //  Allocate the context.
        //

        status = FltAllocateContext( ScannerData.Filter,
                                     FLT_STREAMHANDLE_CONTEXT,
                                     sizeof(SCANNER_STREAM_HANDLE_CONTEXT),
                                     PagedPool,
                                     &scannerContext );

        if (NT_SUCCESS(status)) {

            //
            //  Set the handle context.
            //

            scannerContext->RescanRequired = TRUE;

            (VOID) FltSetStreamHandleContext( FltObjects->Instance,
                                              FltObjects->FileObject,
                                              FLT_SET_CONTEXT_REPLACE_IF_EXISTS,
                                              scannerContext,
                                              NULL );

            //
            //  Normally we would check the results of FltSetStreamHandleContext
            //  for a variety of error cases. However, The only error status 
            //  that could be returned, in this case, would tell us that
            //  contexts are not supported.  Even if we got this error,
            //  we just want to release the context now and that will free
            //  this memory if it was not successfully set.
            //

            //
            //  Release our reference on the context (the set adds a reference)
            //

            FltReleaseContext( scannerContext );
        }
    }

    return returnStatus;
}


FLT_PREOP_CALLBACK_STATUS
ScannerPreCleanup (
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
    )
/*++

Routine Description:

    Pre cleanup callback.  If this file was opened for write access, we want
    to rescan it now.

Arguments:

    Data - The structure which describes the operation parameters.

    FltObject - The structure which describes the objects affected by this
        operation.

    CompletionContext - Output parameter which can be used to pass a context
        from this pre-cleanup callback to the post-cleanup callback.

Return Value:

    Always FLT_PREOP_SUCCESS_NO_CALLBACK.

--*/
{
    NTSTATUS status;
    PSCANNER_STREAM_HANDLE_CONTEXT context;
    BOOLEAN safe;

    UNREFERENCED_PARAMETER( Data );
    UNREFERENCED_PARAMETER( CompletionContext );

    status = FltGetStreamHandleContext( FltObjects->Instance,
                                        FltObjects->FileObject,
                                        &context );

    if (NT_SUCCESS( status )) {

        if (context->RescanRequired) {

            (VOID) ScannerpScanFileInUserMode( FltObjects->Instance,
                                               FltObjects->FileObject,
                                               &safe );

            if (!safe) {

                DbgPrint( "!!! scanner.sys -- foul language detected in precleanup !!!\n" );
            }
        }

        FltReleaseContext( context );
    }


    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_PREOP_CALLBACK_STATUS
ScannerPreWrite (
    __inout PFLT_CALLBACK_DATA Data,
    __in PCFLT_RELATED_OBJECTS FltObjects,
    __deref_out_opt PVOID *CompletionContext
    )
/*++

Routine Description:

    Pre write callback.  We want to scan what's being written now.

Arguments:

    Data - The structure which describes the operation parameters.

    FltObject - The structure which describes the objects affected by this
        operation.

    CompletionContext - Output parameter which can be used to pass a context
        from this pre-write callback to the post-write callback.

Return Value:

    Always FLT_PREOP_SUCCESS_NO_CALLBACK.

--*/
{
    FLT_PREOP_CALLBACK_STATUS returnStatus = FLT_PREOP_SUCCESS_NO_CALLBACK;
    NTSTATUS status;
    PSCANNER_NOTIFICATION notification = NULL;
    PSCANNER_STREAM_HANDLE_CONTEXT context = NULL;
    ULONG replyLength;
    BOOLEAN safe = TRUE;
    PUCHAR buffer;

    UNREFERENCED_PARAMETER( CompletionContext );

    //
    //  If not client port just ignore this write.
    //

    if (ScannerData.ClientPort == NULL) {

        return FLT_PREOP_SUCCESS_NO_CALLBACK;
    }

    status = FltGetStreamHandleContext( FltObjects->Instance,
                                        FltObjects->FileObject,
                                        &context );

    if (!NT_SUCCESS( status )) {

        //
        //  We are not interested in this file
        //

        return FLT_PREOP_SUCCESS_NO_CALLBACK;

    }

    //
    //  Use try-finally to cleanup
    //

    try {

        //
        //  Pass the contents of the buffer to user mode.
        //

        if (Data->Iopb->Parameters.Write.Length != 0) {

            //
            //  Get the users buffer address.  If there is a MDL defined, use
            //  it.  If not use the given buffer address.
            //

            if (Data->Iopb->Parameters.Write.MdlAddress != NULL) {

                buffer = MmGetSystemAddressForMdlSafe( Data->Iopb->Parameters.Write.MdlAddress,
                                                       NormalPagePriority );

                //
                //  If we have a MDL but could not get and address, we ran out
                //  of memory, report the correct error
                //

                if (buffer == NULL) {

                    Data->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
                    Data->IoStatus.Information = 0;
                    returnStatus = FLT_PREOP_COMPLETE;
                    leave;
                }

            } else {

                //
                //  Use the users buffer
                //

                buffer  = Data->Iopb->Parameters.Write.WriteBuffer;
            }

            //
            //  In a production-level filter, we would actually let user mode scan the file directly.
            //  Allocating & freeing huge amounts of non-paged pool like this is not very good for system perf.
            //  This is just a sample!
            //

            notification = ExAllocatePoolWithTag( NonPagedPool,
                                                  sizeof( SCANNER_NOTIFICATION ),
                                                  'nacS' );
            if (notification == NULL) {

                Data->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
                Data->IoStatus.Information = 0;
                returnStatus = FLT_PREOP_COMPLETE;
                leave;
            }

            notification->BytesToScan = min( Data->Iopb->Parameters.Write.Length, SCANNER_READ_BUFFER_SIZE );

            //
            //  The buffer can be a raw user buffer. Protect access to it
            //

            try  {

                RtlCopyMemory( &notification->Contents,
                               buffer,
                               notification->BytesToScan );

            } except( EXCEPTION_EXECUTE_HANDLER ) {

                //
                //  Error accessing buffer. Complete i/o with failure
                //

                Data->IoStatus.Status = GetExceptionCode() ;
                Data->IoStatus.Information = 0;
                returnStatus = FLT_PREOP_COMPLETE;
                leave;
            }

            //
            //  Send message to user mode to indicate it should scan the buffer.
            //  We don't have to synchronize between the send and close of the handle
            //  as FltSendMessage takes care of that.
            //

            replyLength = sizeof( SCANNER_REPLY );

            status = FltSendMessage( ScannerData.Filter,
                                     &ScannerData.ClientPort,
                                     notification,
                                     sizeof( SCANNER_NOTIFICATION ),
                                     notification,
                                     &replyLength,
                                     NULL );

            if (STATUS_SUCCESS == status) {

               safe = ((PSCANNER_REPLY) notification)->SafeToOpen;

           } else {

               //
               //  Couldn't send message. This sample will let the i/o through.
               //

               DbgPrint( "!!! scanner.sys --- couldn't send message to user-mode to scan file, status 0x%X\n", status );
           }
        }

        if (!safe) {

            //
            //  Block this write if not paging i/o (as a result of course, this scanner will not prevent memory mapped writes of contaminated
            //  strings to the file, but only regular writes). The effect of getting ERROR_ACCESS_DENIED for many apps to delete the file they
            //  are trying to write usually.
            //  To handle memory mapped writes - we should be scanning at close time (which is when we can really establish that the file object
            //  is not going to be used for any more writes)
            //

            DbgPrint( "!!! scanner.sys -- foul language detected in write !!!\n" );

            if (!FlagOn( Data->Iopb->IrpFlags, IRP_PAGING_IO )) {

                DbgPrint( "!!! scanner.sys -- blocking the write !!!\n" );

                Data->IoStatus.Status = STATUS_ACCESS_DENIED;
                Data->IoStatus.Information = 0;
                returnStatus = FLT_PREOP_COMPLETE;
            }
        }

    } finally {

        if (notification != NULL) {

            ExFreePoolWithTag( notification, 'nacS' );
        }

        if (context) {

            FltReleaseContext( context );
        }
    }

    return returnStatus;
}

//////////////////////////////////////////////////////////////////////////
//  Local support routines.
//
/////////////////////////////////////////////////////////////////////////

NTSTATUS
ScannerpScanFileInUserMode (
    __in PFLT_INSTANCE Instance,
    __in PFILE_OBJECT FileObject,
    __out PBOOLEAN SafeToOpen
    )
{
    NTSTATUS status = STATUS_SUCCESS;
    PVOID buffer = NULL;
    ULONG bytesRead;
    PSCANNER_NOTIFICATION notification = NULL;
    FLT_VOLUME_PROPERTIES volumeProps;
    LARGE_INTEGER offset;
    ULONG replyLength, length;
    PFLT_VOLUME volume = NULL;

    *SafeToOpen = TRUE;

    //
    //  If not client port just return.
    //

    if (ScannerData.ClientPort == NULL) {

        return STATUS_SUCCESS;
    }

    try {

        //
        //  Obtain the volume object .
        //

        status = FltGetVolumeFromInstance( Instance, &volume );

        if (!NT_SUCCESS( status )) {

            leave;
        }

        //
        //  Determine sector size. Noncached I/O can only be done at sector size offsets, and in lengths which are
        //  multiples of sector size. A more efficient way is to make this call once and remember the sector size in the
        //  instance setup routine and setup an instance context where we can cache it.
        //

        status = FltGetVolumeProperties( volume,
                                         &volumeProps,
                                         sizeof( volumeProps ),
                                         &length );
        //
        //  STATUS_BUFFER_OVERFLOW can be returned - however we only need the properties, not the names
        //  hence we only check for error status.
        //

        if (NT_ERROR( status )) {

            leave;
        }

        length = max( SCANNER_READ_BUFFER_SIZE, volumeProps.SectorSize );

        //
        //  Use non-buffered i/o, so allocate aligned pool
        //

        buffer = FltAllocatePoolAlignedWithTag( Instance,
                                                NonPagedPool,
                                                length,
                                                'nacS' );

        if (NULL == buffer) {

            status = STATUS_INSUFFICIENT_RESOURCES;
            leave;
        }

        notification = ExAllocatePoolWithTag( NonPagedPool,
                                              sizeof( SCANNER_NOTIFICATION ),
                                              'nacS' );

        if(NULL == notification) {

            status = STATUS_INSUFFICIENT_RESOURCES;
            leave;
        }

        //
        //  Read the beginning of the file and pass the contents to user mode.
        //

        offset.QuadPart = bytesRead = 0;
        status = FltReadFile( Instance,
                              FileObject,
                              &offset,
                              length,
                              buffer,
                              FLTFL_IO_OPERATION_NON_CACHED |
                               FLTFL_IO_OPERATION_DO_NOT_UPDATE_BYTE_OFFSET,
                              &bytesRead,
                              NULL,
                              NULL );

        if (NT_SUCCESS( status ) && (0 != bytesRead)) {

            notification->BytesToScan = (ULONG) bytesRead;

            //
            //  Copy only as much as the buffer can hold
            //

            RtlCopyMemory( &notification->Contents,
                           buffer,
                           min( notification->BytesToScan, SCANNER_READ_BUFFER_SIZE ) );

            replyLength = sizeof( SCANNER_REPLY );

            status = FltSendMessage( ScannerData.Filter,
                                     &ScannerData.ClientPort,
                                     notification,
                                     sizeof(SCANNER_NOTIFICATION),
                                     notification,
                                     &replyLength,
                                     NULL );

            if (STATUS_SUCCESS == status) {

                *SafeToOpen = ((PSCANNER_REPLY) notification)->SafeToOpen;

            } else {

                //
                //  Couldn't send message
                //

                DbgPrint( "!!! scanner.sys --- couldn't send message to user-mode to scan file, status 0x%X\n", status );
            }
        }

    } finally {

        if (NULL != buffer) {

            FltFreePoolAlignedWithTag( Instance, buffer, 'nacS' );
        }

        if (NULL != notification) {

            ExFreePoolWithTag( notification, 'nacS' );
        }

        if (NULL != volume) {

            FltObjectDereference( volume );
        }
    }

    return status;
}

// 读取注册表
NTSTATUS MyGetKeyValue(PUNICODE_STRING pKeyPath, PUNICODE_STRING pValueName, PUNICODE_STRING pValue)
{
	NTSTATUS						status						= STATUS_SUCCESS;
	HANDLE							hKey						= NULL;
	OBJECT_ATTRIBUTES				obj;
	PKEY_VALUE_FULL_INFORMATION		pKeyInfo					= NULL;
	ULONG							ulKeyInfoSize				= 0;
	ULONG							ulKeyInfoSizeNeeded			= 0;

	PAGED_CODE();

	InitializeObjectAttributes(&obj, pKeyPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	status = ZwOpenKey(&hKey, KEY_READ, &obj);
	if(!NT_SUCCESS(status))
	{
		DbgPrint("[MyGetKeyValue] ZwOpenKey: 0x%X\n", status);
		return status;
	}
	status = ZwQueryValueKey( hKey,
		pValueName,
		KeyValueFullInformation,
		pKeyInfo,
		ulKeyInfoSize,
		&ulKeyInfoSizeNeeded );
	if( (status != STATUS_BUFFER_TOO_SMALL) && (status != STATUS_BUFFER_OVERFLOW) )
	{
		DbgPrint("[MyGetKeyValue] ZwQueryValueKey: 0x%X\n", status);
		goto end;
	}
	// 读取KEY
	ulKeyInfoSize = ulKeyInfoSizeNeeded;
	pKeyInfo = (PKEY_VALUE_FULL_INFORMATION)ExAllocatePool(NonPagedPool, ulKeyInfoSizeNeeded);
	if(NULL == pKeyInfo)
	{
		goto end;
	}
	status = ZwQueryValueKey( hKey,
		pValueName,
		KeyValueFullInformation,
		pKeyInfo,
		ulKeyInfoSize,
		&ulKeyInfoSizeNeeded );
	if( (status != STATUS_SUCCESS) || (ulKeyInfoSizeNeeded != ulKeyInfoSize) )
	{
		DbgPrint("[MyGetKeyValue] ZwQueryValueKey: 0x%X\n", status);
		goto end;
	}
	pValue->MaximumLength = pKeyInfo->DataLength + 2;
	pValue->Buffer = (PWSTR)ExAllocatePool(NonPagedPool, pValue->MaximumLength);
	pValue->Length = pKeyInfo->DataLength;
	RtlZeroMemory(pValue->Buffer, pValue->MaximumLength);
	RtlCopyMemory(pValue->Buffer, ((char *)pKeyInfo + pKeyInfo->DataOffset), pValue->Length);
	DbgPrint("[MyGetKeyValue] (%d)%S", pKeyInfo->DataLength, (WCHAR *)((char *)pKeyInfo+pKeyInfo->DataOffset));
	// 退出清理工作
end:
	if(NULL != hKey)
		ZwClose(hKey);
	if(NULL != pKeyInfo)
		ExFreePool(pKeyInfo);

	return status;
}
// 得到
NTSTATUS MyGetVolumnPath(PUNICODE_STRING pPath, PUNICODE_STRING pVolumnPath)
{
	NTSTATUS			status			= STATUS_PATH_NOT_COVERED;
	WCHAR				szPath[32]		= {0};
	PWCHAR				pPot			= NULL;
	UNICODE_STRING		usLink;
	OBJECT_ATTRIBUTES	objattr;
	HANDLE				hSymbLink		= NULL;
	
	pPot = wcschr(pPath->Buffer, L':');
	if(NULL == pPot)
	{
		return status;
	}
	pPot--;
	wcscpy(szPath, L"\\??\\X:");
	szPath[4] = *pPot;
	usLink.Buffer = szPath;
	usLink.MaximumLength = sizeof(szPath);
	usLink.Length = wcslen(szPath) * sizeof(WCHAR);
	pPot += 2;
	InitializeObjectAttributes(&objattr
		, &usLink
		, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE
		, NULL, NULL);
	status = ZwOpenSymbolicLinkObject(&hSymbLink, FILE_ALL_ACCESS, &objattr);
	if(STATUS_SUCCESS != status)
	{
		DbgPrint("[MyGetVolumnPath] (%X)%S", status, usLink.Buffer);
		return status;
	}
	pVolumnPath->MaximumLength = 1024;
	pVolumnPath->Length = 1024;
	pVolumnPath->Buffer = ExAllocatePool(NonPagedPool, pVolumnPath->MaximumLength);
	status = ZwQuerySymbolicLinkObject(hSymbLink, pVolumnPath, &pVolumnPath->Length);
	if(STATUS_SUCCESS == status)
	{
		wcscat(pVolumnPath->Buffer, pPot);
		pPot = wcsrchr(pVolumnPath->Buffer, L'\\');
		if(NULL == pPot)
			pPot = wcsrchr(pVolumnPath->Buffer, L'/');
		if(NULL != pPot)
			*(pPot+1) = 0;
		pVolumnPath->Length = sizeof(WCHAR)*wcslen(pVolumnPath->Buffer);
	}
	ZwClose(hSymbLink);
	return status;
}
// 路径是不是工作目录
LONG	PathIsWorkPath(PWSTR pPath)
{
	static	WCHAR*		pWorkDir		= NULL;
	static	LONG		nWorkDir		= 0;
	WCHAR*				pFile			= NULL;
	WCHAR				wcTmp			= 0;
	int					i				= 0;

	PAGED_CODE();
	if(NULL == pWorkDir)
	{
		pWorkDir = ScannerData.usVolumePath.Buffer;
		nWorkDir = wcslen(pWorkDir);
	}
	if(NULL == pWorkDir || NULL == pPath)
		return 0;
	
	for(i = 0; pPath[i] && i < nWorkDir; i++)
	{
		if(pPath[i] == pWorkDir[i])
			continue;
		wcTmp = pWorkDir[i];
		if(wcTmp >= 'A' && wcTmp <= 'Z')
			wcTmp = wcTmp - 'A' + 'a';
		else if(wcTmp >= 'a' && wcTmp <= 'z')
			wcTmp = wcTmp - 'a' + 'A';
		else
			return 0;
		if(pPath[i] != wcTmp)
			return 0;
	}
	return i == nWorkDir;
}

// 得到工作目录信息
NTSTATUS GetWorkDirInfo(PUNICODE_STRING pUstr)
{
/*
	OBJECT_ATTRIBUTES		objectattribute;
	HANDLE					hSymbolc;
	NTSTATUS				ntStatus;
	ULONG					uLen;
	UNICODE_STRING			ucDevice;
	WCHAR					szBuff[512]				= {0};
	PFILE_OBJECT			fileobject				= NULL;
	PDEVICE_OBJECT			deviceobject			= NULL;

	PAGED_CODE();

	DbgPrint("[GetWorkDirInfo] work path: %S\n", pUstr->Buffer);
	wcsncpy(szBuff, pUstr->Buffer, 512);
	InitializeObjectAttributes(&objectattribute
		, &uclink
		, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE
		, NULL, NULL);
	ntStatus = ZwOpenSymbolicLinkObject(&hSymbolc, FILE_ALL_ACCESS, &objectattribute);
	if(!NT_SUCCESS(ntStatus))
		return ntStatus;

	DbgPrint("Get 'c:' device.\n");
	ucDevice.Buffer = szBuff;
	ucDevice.Length = arrayof(szBuff);
	ntStatus = ZwQuerySymbolicLinkObject(hSymbolc, &ucDevice, &uLen);
	DbgPrint("Get 'c:' device name: %S\n", szBuff);
	if(!NT_SUCCESS(ntStatus))
		return ntStatus;

	DbgPrint("Open 'c:' device\n");
	ntStatus = IoGetDeviceObjectPointer(&ucDevice, SYNCHRONIZE|FILE_ANY_ACCESS, &fileobject, &deviceobject);
	if(!NT_SUCCESS(ntStatus))
		return ntStatus;

	DbgPrint("attach device to device stack.\n");
	pex->AttachedToDeviceObject = IoAttachDeviceToDeviceStack(pex->pDevice, deviceobject);
	ObDereferenceObject(fileobject);
	if(pex->AttachedToDeviceObject)
	{
		DbgPrint("attach 'c:' success.\n");
		pex->AttachedToDeviceObject = deviceobject;
	}
	else
	{
		DbgPrint("attach 'c:' faild.\n");
		return -1;
	}

	DbgPrint("Enter SfAttatchDevice\n");*/

	return STATUS_SUCCESS;
}