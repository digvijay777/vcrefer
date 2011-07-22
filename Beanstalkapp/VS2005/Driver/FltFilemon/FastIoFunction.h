#pragma once

//
// Define fast I/O procedure prototypes.
//
// Fast I/O read and write procedures.
//

BOOLEAN SfFastIoCheckIfPossible(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN ULONG Length
								, IN BOOLEAN Wait, IN ULONG LockKey, IN BOOLEAN CheckForReadOperation
								, OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject);

BOOLEAN SfFastIoRead(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN ULONG Length
					 , IN BOOLEAN Wait, IN ULONG LockKey, OUT PVOID Buffer, OUT PIO_STATUS_BLOCK IoStatus
					 , IN struct _DEVICE_OBJECT *DeviceObject);

BOOLEAN SfFastIoWrite(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN ULONG Length
					  , IN BOOLEAN Wait, IN ULONG LockKey, IN PVOID Buffer, OUT PIO_STATUS_BLOCK IoStatus
					  , IN struct _DEVICE_OBJECT *DeviceObject);
//
// Fast I/O query basic and standard information procedures.
//
BOOLEAN SfFastIoQueryBasicInfo(IN struct _FILE_OBJECT *FileObject, IN BOOLEAN Wait, OUT PFILE_BASIC_INFORMATION Buffer
							   , OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject);

BOOLEAN SfFastIoQueryStandardInfo(IN struct _FILE_OBJECT *FileObject, IN BOOLEAN Wait, OUT PFILE_STANDARD_INFORMATION Buffer
								  , OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject);
//
// Fast I/O lock and unlock procedures.
//
BOOLEAN SfFastIoLock(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN PLARGE_INTEGER Length
					 , PEPROCESS ProcessId, ULONG Key, BOOLEAN FailImmediately, BOOLEAN ExclusiveLock
					 , OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject);

BOOLEAN SfFastIoUnlockSingle(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN PLARGE_INTEGER Length
							 , PEPROCESS ProcessId, ULONG Key, OUT PIO_STATUS_BLOCK IoStatus
							 , IN struct _DEVICE_OBJECT *DeviceObject);

BOOLEAN SfFastIoUnlockAll(IN struct _FILE_OBJECT *FileObject, PEPROCESS ProcessId, OUT PIO_STATUS_BLOCK IoStatus
						  , IN struct _DEVICE_OBJECT *DeviceObject);

BOOLEAN SfFastIoUnlockAllByKey(IN struct _FILE_OBJECT *FileObject, PVOID ProcessId, ULONG Key
							   , OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject);

//
// Fast I/O device control procedure.
//
BOOLEAN SfFastIoDeviceControl(IN struct _FILE_OBJECT *FileObject, IN BOOLEAN Wait, IN PVOID InputBuffer OPTIONAL
							  , IN ULONG InputBufferLength, OUT PVOID OutputBuffer OPTIONAL, IN ULONG OutputBufferLength
							  , IN ULONG IoControlCode, OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject);
//
// Define callbacks for NtCreateSection to synchronize correctly with
// the file system.  It pre-acquires the resources that will be needed
// when calling to query and set file/allocation size in the file system.
//
VOID SfAcquireFileForNtCreateSection(IN struct _FILE_OBJECT *FileObject);

VOID SfReleaseFileForNtCreateSection(IN struct _FILE_OBJECT *FileObject);

//
// Define callback for drivers that have device objects attached to lower-
// level drivers' device objects.  This callback is made when the lower-level
// driver is deleting its device object.
//
VOID SfFastIoDetachDevice(IN struct _DEVICE_OBJECT *SourceDevice, IN struct _DEVICE_OBJECT *TargetDevice);
//
// This structure is used by the server to quickly get the information needed
// to service a server open call.  It is takes what would be two fast io calls
// one for basic information and the other for standard information and makes
// it into one call.
//
BOOLEAN SfFastIoQueryNetworkOpenInfo(IN struct _FILE_OBJECT *FileObject, IN BOOLEAN Wait , OUT struct _FILE_NETWORK_OPEN_INFORMATION *Buffer
									 , OUT struct _IO_STATUS_BLOCK *IoStatus, IN struct _DEVICE_OBJECT *DeviceObject);
//
//  Define Mdl-based routines for the server to call
//
BOOLEAN SfMdlRead(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN ULONG Length, IN ULONG LockKey
				  , OUT PMDL *MdlChain, OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject);

BOOLEAN SfMdlReadComplete(IN struct _FILE_OBJECT *FileObject, IN PMDL MdlChain, IN struct _DEVICE_OBJECT *DeviceObject);

BOOLEAN SfPrepareMdlWrite(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN ULONG Length, IN ULONG LockKey
						  , OUT PMDL *MdlChain, OUT PIO_STATUS_BLOCK IoStatus, IN struct _DEVICE_OBJECT *DeviceObject);

BOOLEAN SfMdlWriteComplete(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN PMDL MdlChain
						   , IN struct _DEVICE_OBJECT *DeviceObject);

//
//  If this routine is present, it will be called by FsRtl
//  to acquire the file for the mapped page writer.
//

NTSTATUS SfReleaseForModWrite(IN struct _FILE_OBJECT *FileObject, IN struct _ERESOURCE *ResourceToRelease
							  , IN struct _DEVICE_OBJECT *DeviceObject);

//
//  If this routine is present, it will be called by FsRtl
//  to acquire the file for the mapped page writer.
//

NTSTATUS SfAcquireForCcFlush(IN struct _FILE_OBJECT *FileObject, IN struct _DEVICE_OBJECT *DeviceObject);

NTSTATUS SfReleaseForCcFlush(IN struct _FILE_OBJECT *FileObject,IN struct _DEVICE_OBJECT *DeviceObject);

BOOLEAN SfFastIoReadCompressed(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN ULONG Length
							   , IN ULONG LockKey, OUT PVOID Buffer, OUT PMDL *MdlChain, OUT PIO_STATUS_BLOCK IoStatus
							   , OUT struct _COMPRESSED_DATA_INFO *CompressedDataInfo,IN ULONG CompressedDataInfoLength
							   , IN struct _DEVICE_OBJECT *DeviceObject);

BOOLEAN SfFastIoWriteCompressed(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN ULONG Length
								, IN ULONG LockKey, IN PVOID Buffer, OUT PMDL *MdlChain, OUT PIO_STATUS_BLOCK IoStatus
								, IN struct _COMPRESSED_DATA_INFO *CompressedDataInfo, IN ULONG CompressedDataInfoLength
								, IN struct _DEVICE_OBJECT *DeviceObject);

BOOLEAN SfMdlReadCompleteCompressed(IN struct _FILE_OBJECT *FileObject, IN PMDL MdlChain, IN struct _DEVICE_OBJECT *DeviceObject);

BOOLEAN SfMdlWriteCompleteCompressed(IN struct _FILE_OBJECT *FileObject, IN PLARGE_INTEGER FileOffset, IN PMDL MdlChain
									 , IN struct _DEVICE_OBJECT *DeviceObject);

BOOLEAN SfFastIoQueryOpen(IN struct _IRP *Irp, OUT PFILE_NETWORK_OPEN_INFORMATION NetworkInformation
						  , IN struct _DEVICE_OBJECT *DeviceObject);
