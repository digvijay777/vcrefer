/*
 *	Fast io function
 */
#include "XFilemon.h"
#include "FastIoFunction.h"


BOOLEAN SfFastIoCheckIfPossible(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN ULONG Length
								, IN BOOLEAN Wait, IN ULONG LockKey, IN BOOLEAN CheckForReadOperation
								, OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;
	
	PAGED_CODE();
	DbgPrint("Enter SfFastIoCheckIfPossible.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;
	//if(VALID_FAST_IO_DISPATCH_HANDLER(fastIoDispatch, FastIoCheckIfPossible))
	if(NULL == fastIoDispatch || NULL == fastIoDispatch->FastIoCheckIfPossible)
		return FALSE;
	
	return (fastIoDispatch->FastIoCheckIfPossible)(FileObject, FileOffset, Length
		, Wait, LockKey, CheckForReadOperation, IoStatus, DeviceObject);
}

BOOLEAN SfFastIoRead(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN ULONG Length
					 , IN BOOLEAN Wait, IN ULONG LockKey, OUT PVOID Buffer, OUT PIO_STATUS_BLOCK IoStatus
					 , IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfFastIoRead.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;
	if(NULL == fastIoDispatch || NULL == fastIoDispatch->FastIoRead)
		return FALSE;

	return (fastIoDispatch->FastIoRead)(FileObject, FileOffset, Length, Wait, LockKey
		, Buffer, IoStatus, DeviceObject);
}

BOOLEAN SfFastIoWrite(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN ULONG Length
					  , IN BOOLEAN Wait, IN ULONG LockKey, IN PVOID Buffer, OUT PIO_STATUS_BLOCK IoStatus
					  , IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfFastIoWrite.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;
	if(NULL == fastIoDispatch || NULL == fastIoDispatch->FastIoWrite)
		return FALSE;

	return (fastIoDispatch->FastIoWrite)(FileObject, FileOffset, Length, Wait, LockKey, Buffer
		, IoStatus, DeviceObject);
}

BOOLEAN SfFastIoQueryBasicInfo(IN struct _FILE_OBJECT *FileObject, IN BOOLEAN Wait, OUT PFILE_BASIC_INFORMATION Buffer
							   , OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfFastIoQueryBasicInfo.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->FastIoQueryBasicInfo)
		return FALSE;

	return (fastIoDispatch->FastIoQueryBasicInfo)(FileObject, Wait, Buffer, IoStatus, DeviceObject);
}

BOOLEAN SfFastIoQueryStandardInfo(IN struct _FILE_OBJECT *FileObject, IN BOOLEAN Wait, OUT PFILE_STANDARD_INFORMATION Buffer
								  , OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfFastIoQueryStandardInfo.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->FastIoQueryStandardInfo)
		return FALSE;

	return (fastIoDispatch->FastIoQueryStandardInfo)(FileObject, Wait, Buffer, IoStatus, DeviceObject);
}

BOOLEAN SfFastIoLock(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN PLARGE_INTEGER Length
					 , PEPROCESS ProcessId, ULONG Key, BOOLEAN FailImmediately, BOOLEAN ExclusiveLock
					 , OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfFastIoLock.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->FastIoLock)
		return FALSE;

	return (fastIoDispatch->FastIoLock)(FileObject, FileOffset, Length, ProcessId, Key, FailImmediately
		, ExclusiveLock, IoStatus, DeviceObject);
}

BOOLEAN SfFastIoUnlockSingle(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN PLARGE_INTEGER Length
							 , PEPROCESS ProcessId, ULONG Key, OUT PIO_STATUS_BLOCK IoStatus
							 , IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfFastIoUnlockSingle.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->FastIoUnlockSingle)
		return FALSE;

	return (fastIoDispatch->FastIoUnlockSingle)(FileObject, FileOffset, Length
		, ProcessId, Key, IoStatus, DeviceObject);
}

BOOLEAN SfFastIoUnlockAll(IN struct _FILE_OBJECT *FileObject, PEPROCESS ProcessId, OUT PIO_STATUS_BLOCK IoStatus
						  , IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfFastIoUnlockAll.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->FastIoUnlockAll)
		return FALSE;

	return (fastIoDispatch->FastIoUnlockAll)(FileObject, ProcessId, IoStatus, DeviceObject);
}

BOOLEAN SfFastIoUnlockAllByKey(IN struct _FILE_OBJECT *FileObject, PVOID ProcessId, ULONG Key
							   , OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfFastIoUnlockAllByKey.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->FastIoUnlockAllByKey)
		return FALSE;

	return (fastIoDispatch->FastIoUnlockAllByKey)(FileObject, ProcessId, Key, IoStatus, DeviceObject);
}


BOOLEAN SfFastIoDeviceControl(IN struct _FILE_OBJECT *FileObject, IN BOOLEAN Wait, IN PVOID InputBuffer OPTIONAL
							  , IN ULONG InputBufferLength, OUT PVOID OutputBuffer OPTIONAL, IN ULONG OutputBufferLength
							  , IN ULONG IoControlCode, OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfFastIoDeviceControl.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->FastIoDeviceControl)
		return FALSE;

	return (fastIoDispatch->FastIoDeviceControl)(FileObject, Wait, InputBuffer, InputBufferLength
		, OutputBuffer, OutputBufferLength, IoControlCode, IoStatus, DeviceObject);
}

VOID SfAcquireFileForNtCreateSection(IN struct _FILE_OBJECT *FileObject)
{
	DbgPrint("Enter SfAcquireFileForNtCreateSection.\n");
}

VOID SfReleaseFileForNtCreateSection(IN struct _FILE_OBJECT *FileObject)
{
	DbgPrint("Enter SfReleaseFileForNtCreateSection.\n");
}


VOID SfFastIoDetachDevice(IN struct _DEVICE_OBJECT *SourceDevice, IN struct _DEVICE_OBJECT *TargetDevice)
{
	DbgPrint("Enter SfFastIoDetachDevice.\n");
}

BOOLEAN SfFastIoQueryNetworkOpenInfo(IN struct _FILE_OBJECT *FileObject, IN BOOLEAN Wait , OUT struct _FILE_NETWORK_OPEN_INFORMATION *Buffer
									 , OUT struct _IO_STATUS_BLOCK *IoStatus, IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfFastIoQueryNetworkOpenInfo.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->FastIoQueryNetworkOpenInfo)
		return FALSE;

	return (fastIoDispatch->FastIoQueryNetworkOpenInfo)(FileObject, Wait, Buffer
		, IoStatus, DeviceObject);
}

BOOLEAN SfMdlRead(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN ULONG Length, IN ULONG LockKey
				  , OUT PMDL *MdlChain, OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfMdlRead.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->MdlRead)
		return FALSE;

	return (fastIoDispatch->MdlRead)(FileObject, FileOffset, Length, LockKey
		, MdlChain, IoStatus, DeviceObject);
}

BOOLEAN SfMdlReadComplete(IN struct _FILE_OBJECT *FileObject, IN PMDL MdlChain, IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfMdlReadComplete.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->MdlReadComplete)
		return FALSE;

	return (fastIoDispatch->MdlReadComplete)(FileObject, MdlChain, DeviceObject);
}

BOOLEAN SfPrepareMdlWrite(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN ULONG Length, IN ULONG LockKey
						  , OUT PMDL *MdlChain, OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfPrepareMdlWrite.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->PrepareMdlWrite)
		return FALSE;

	return (fastIoDispatch->PrepareMdlWrite)(FileObject, FileOffset, Length, LockKey
		, MdlChain, IoStatus, DeviceObject);
}

BOOLEAN SfMdlWriteComplete(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN PMDL MdlChain
						   , IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfMdlWriteComplete.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->MdlWriteComplete)
		return FALSE;

	return (fastIoDispatch->MdlWriteComplete)(FileObject, FileOffset, MdlChain, DeviceObject);
}

//
//  If this routine is present, it will be called by FsRtl
//  to acquire the file for the mapped page writer.
//

NTSTATUS SfReleaseForModWrite(IN struct _FILE_OBJECT *FileObject, IN struct _ERESOURCE *ResourceToRelease
							  , IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfReleaseForModWrite.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return STATUS_SUCCESS;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return STATUS_SUCCESS;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->ReleaseForModWrite)
		return STATUS_SUCCESS;

	return (fastIoDispatch->ReleaseForModWrite)(FileObject, ResourceToRelease, DeviceObject);
}

//
//  If this routine is present, it will be called by FsRtl
//  to acquire the file for the mapped page writer.
//

NTSTATUS SfAcquireForCcFlush(IN struct _FILE_OBJECT *FileObject, IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfAcquireForCcFlush.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return STATUS_SUCCESS;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return STATUS_SUCCESS;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->AcquireForCcFlush)
		return STATUS_SUCCESS;

	return (fastIoDispatch->AcquireForCcFlush)(FileObject, DeviceObject);
}

NTSTATUS SfReleaseForCcFlush(IN struct _FILE_OBJECT *FileObject,IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfReleaseForCcFlush.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return STATUS_SUCCESS;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return STATUS_SUCCESS;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->ReleaseForCcFlush)
		return STATUS_SUCCESS;

	return (fastIoDispatch->ReleaseForCcFlush)(FileObject, DeviceObject);
}

BOOLEAN SfFastIoReadCompressed(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN ULONG Length
							   , IN ULONG LockKey, OUT PVOID Buffer, OUT PMDL *MdlChain, OUT PIO_STATUS_BLOCK IoStatus
							   , OUT struct _COMPRESSED_DATA_INFO *CompressedDataInfo,IN ULONG CompressedDataInfoLength
							   , IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfFastIoReadCompressed.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->FastIoReadCompressed)
		return FALSE;

	return (fastIoDispatch->FastIoReadCompressed)(FileObject, FileOffset, Length
		, LockKey, Buffer, MdlChain, IoStatus, CompressedDataInfo, CompressedDataInfoLength, DeviceObject);
}

BOOLEAN SfFastIoWriteCompressed(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN ULONG Length
								, IN ULONG LockKey, IN PVOID Buffer, OUT PMDL *MdlChain, OUT PIO_STATUS_BLOCK IoStatus
								, IN struct _COMPRESSED_DATA_INFO *CompressedDataInfo, IN ULONG CompressedDataInfoLength
								, IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfFastIoWriteCompressed.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->FastIoWriteCompressed)
		return FALSE;

	return (fastIoDispatch->FastIoWriteCompressed)(FileObject, FileOffset, Length
		, LockKey, Buffer, MdlChain, IoStatus, CompressedDataInfo, CompressedDataInfoLength, DeviceObject);
}

BOOLEAN SfMdlReadCompleteCompressed(IN struct _FILE_OBJECT *FileObject, IN PMDL MdlChain, IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfMdlReadCompleteCompressed.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->MdlReadCompleteCompressed)
		return FALSE;

	return (fastIoDispatch->MdlReadCompleteCompressed)(FileObject, MdlChain, DeviceObject);
}

BOOLEAN SfMdlWriteCompleteCompressed(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN PMDL MdlChain
									 , IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfMdlWriteCompleteCompressed.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->MdlWriteCompleteCompressed)
		return FALSE;

	return (fastIoDispatch->MdlWriteCompleteCompressed)(FileObject, FileOffset, MdlChain, DeviceObject);
}

BOOLEAN SfFastIoQueryOpen(IN struct _IRP *Irp, OUT PFILE_NETWORK_OPEN_INFORMATION NetworkInformation
						  , IN struct _DEVICE_OBJECT *DeviceObject)
{
	PDEVICE_OBJECT			nextDeviceObject;
	PFAST_IO_DISPATCH		fastIoDispatch;

	PAGED_CODE();
	DbgPrint("Enter SfFastIoQueryOpen.\n");
	if(NULL == DeviceObject->DeviceExtension)
		return FALSE;
	// IS_MY_DEVICE_OBJECT();
	nextDeviceObject = ((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->AttachedToDeviceObject;
	if(NULL == nextDeviceObject)
		return FALSE;
	// 得到设备的fastio分发函数接口
	fastIoDispatch = nextDeviceObject->DriverObject->FastIoDispatch;

	if(NULL == fastIoDispatch || NULL == fastIoDispatch->FastIoQueryOpen)
		return FALSE;

	return (fastIoDispatch->FastIoQueryOpen)(Irp, NetworkInformation, DeviceObject);
}
