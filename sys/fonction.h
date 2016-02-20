#ifndef _FUNCTION_
#define _FUNCTION_

#include <wcstr.h>

#define SEC_FILE           0x800000     
#define SEC_IMAGE         0x1000000     
#define SEC_PROTECTED_IMAGE  0x2000000  
#define SEC_RESERVE       0x4000000     
#define SEC_COMMIT        0x8000000     
#define SEC_NOCACHE      0x10000000     
#define SEC_WRITECOMBINE 0x40000000     
#define SEC_LARGE_PAGES  0x80000000     
#define MEM_IMAGE         SEC_IMAGE
#define WRITE_WATCH_FLAG_RESET 0x01   


// SSDT 
char* liste[401] = {"NtAcceptConnectPort",\
"NtAccessCheck",\
"NtAccessCheckAndAuditAlarm",\
"NtAccessCheckByType",\
"NtAccessCheckByTypeAndAuditAlarm",\
"NtAccessCheckByTypeResultList",\
"NtAccessCheckByTypeResultListAndAuditAlarm",\
"NtAccessCheckByTypeResultListAndAuditAlarmByHandle",\
"NtAddAtom",\
"NtAddBootEntry",\
"NtAddDriverEntry",\
"NtAdjustGroupsToken",\
"NtAdjustPrivilegesToken",\
"NtAlertResumeThread",\
"NtAlertThread",\
"NtAllocateLocallyUniqueId",\
"NtAllocateReserveObject",\
"NtAllocateUserPhysicalPages",\
"NtAllocateUuids",\
"NtAllocateVirtualMemory",\
"NtAlpcAcceptConnectPort",\
"NtAlpcCancelMessage",\
"NtAlpcConnectPort",\
"NtAlpcCreatePort",\
"NtAlpcCreatePortSection",\
"NtAlpcCreateResourceReserve",\
"NtAlpcCreateSectionView",\
"NtAlpcCreateSecurityContext",\
"NtAlpcDeletePortSection",\
"NtAlpcDeleteResourceReserve",\
"NtAlpcDeleteSectionView",\
"NtAlpcDeleteSecurityContext",\
"NtAlpcDisconnectPort",\
"NtAlpcImpersonateClientOfPort",\
"NtAlpcOpenSenderProcess",\
"NtAlpcOpenSenderThread",\
"NtAlpcQueryInformation",\
"NtAlpcQueryInformationMessage",\
"NtAlpcRevokeSecurityContext",\
"NtAlpcSendWaitReceivePort",\
"NtAlpcSetInformation",\
"NtApphelpCacheControl",\
"NtAreMappedFilesTheSame",\
"NtAssignProcessToJobObject",\
"NtCallbackReturn",\
"NtCancelIoFile",\
"NtCancelIoFileEx",\
"NtCancelSynchronousIoFile",\
"NtCancelTimer",\
"NtClearEvent",\
"NtClose",\
"NtCloseObjectAuditAlarm",\
"NtCommitComplete",\
"NtCommitEnlistment",\
"NtCommitTransaction",\
"NtCompactKeys",\
"NtCompareTokens",\
"NtCompleteConnectPort",\
"NtCompressKey",\
"NtConnectPort",\
"NtContinue",\
"NtCreateDebugObject",\
"NtCreateDirectoryObject",\
"NtCreateEnlistment",\
"NtCreateEvent",\
"NtCreateEventPair",\
"NtCreateFile",\
"NtCreateIoCompletion",\
"NtCreateJobObject",\
"NtCreateJobSet",\
"NtCreateKey",\
"NtCreateKeyedEvent",\
"NtCreateKeyTransacted",\
"NtCreateMailslotFile",\
"NtCreateMutant",\
"NtCreateNamedPipeFile",\
"NtCreatePagingFile",\
"NtCreatePort",\
"NtCreatePrivateNamespace",\
"NtCreateProcess",\
"NtCreateProcessEx",\
"NtCreateProfile",\
"NtCreateProfileEx",\
"NtCreateResourceManager",\
"NtCreateSection",\
"NtCreateSemaphore",\
"NtCreateSymbolicLinkObject",\
"NtCreateThread",\
"NtCreateThreadEx",\
"NtCreateTimer",\
"NtCreateToken",\
"NtCreateTransaction",\
"NtCreateTransactionManager",\
"NtCreateUserProcess",\
"NtCreateWaitablePort",\
"NtCreateWorkerFactory",\
"NtDebugActiveProcess",\
"NtDebugContinue",\
"NtDelayExecution",\
"NtDeleteAtom",\
"NtDeleteBootEntry",\
"NtDeleteDriverEntry",\
"NtDeleteFile",\
"NtDeleteKey",\
"NtDeleteObjectAuditAlarm",\
"NtDeletePrivateNamespace",\
"NtDeleteValueKey",\
"NtDeviceIoControlFile",\
"NtDisableLastKnownGood",\
"NtDisplayString",\
"NtDrawText",\
"NtDuplicateObject",\
"NtDuplicateToken",\
"NtEnableLastKnownGood",\
"NtEnumerateBootEntries",\
"NtEnumerateDriverEntries",\
"NtEnumerateKey",\
"NtEnumerateSystemEnvironmentValuesEx",\
"NtEnumerateTransactionObject",\
"NtEnumerateValueKey",\
"NtExtendSection",\
"NtFilterToken",\
"NtFindAtom",\
"NtFlushBuffersFile",\
"NtFlushInstallUILanguage",\
"NtFlushInstructionCache",\
"NtFlushKey",\
"NtFlushProcessWriteBuffers",\
"NtFlushVirtualMemory",\
"NtFlushWriteBuffer",\
"NtFreeUserPhysicalPages",\
"NtFreeVirtualMemory",\
"NtFreezeRegistry",\
"NtFreezeTransactions",\
"NtFsControlFile",\
"NtGetContextThread",\
"NtGetCurrentProcessorNumber",\
"NtGetDevicePowerState",\
"NtGetMUIRegistryInfo",\
"NtGetNextProcess",\
"NtGetNextThread",\
"NtGetNlsSectionPtr",\
"NtGetNotificationResourceManager",\
"NtGetPlugPlayEvent",\
"NtGetWriteWatch",\
"NtImpersonateAnonymousToken",\
"NtImpersonateClientOfPort",\
"NtImpersonateThread",\
"NtInitializeNlsFiles",\
"NtInitializeRegistry",\
"NtInitiatePowerAction",\
"NtIsProcessInJob",\
"NtIsSystemResumeAutomatic",\
"NtIsUILanguageComitted",\
"NtListenPort",\
"NtLoadDriver",\
"NtLoadKey",\
"NtLoadKey2",\
"NtLoadKeyEx",\
"NtLockFile",\
"NtLockProductActivationKeys",\
"NtLockRegistryKey",\
"NtLockVirtualMemory",\
"NtMakePermanentObject",\
"NtMakeTemporaryObject",\
"NtMapCMFModule",\
"NtMapUserPhysicalPages",\
"NtMapUserPhysicalPagesScatter",\
"NtMapViewOfSection",\
"NtModifyBootEntry",\
"NtModifyDriverEntry",\
"NtNotifyChangeDirectoryFile",\
"NtNotifyChangeKey",\
"NtNotifyChangeMultipleKeys",\
"NtNotifyChangeSession",\
"NtOpenDirectoryObject",\
"NtOpenEnlistment",\
"NtOpenEvent",\
"NtOpenEventPair",\
"NtOpenFile",\
"NtOpenIoCompletion",\
"NtOpenJobObject",\
"NtOpenKey",\
"NtOpenKeyEx",\
"NtOpenKeyedEvent",\
"NtOpenKeyTransacted",\
"NtOpenKeyTransactedEx",\
"NtOpenMutant",\
"NtOpenObjectAuditAlarm",\
"NtOpenPrivateNamespace",\
"NtOpenProcess",\
"NtOpenProcessToken",\
"NtOpenProcessTokenEx",\
"NtOpenResourceManager",\
"NtOpenSection",\
"NtOpenSemaphore",\
"NtOpenSession",\
"NtOpenSymbolicLinkObject",\
"NtOpenThread",\
"NtOpenThreadToken",\
"NtOpenThreadTokenEx",\
"NtOpenTimer",\
"NtOpenTransaction",\
"NtOpenTransactionManager",\
"NtPlugPlayControl",\
"NtPowerInformation",\
"NtPrepareComplete",\
"NtPrepareEnlistment",\
"NtPrePrepareComplete",\
"NtPrePrepareEnlistment",\
"NtPrivilegeCheck",\
"NtPrivilegedServiceAuditAlarm",\
"NtPrivilegeObjectAuditAlarm",\
"NtPropagationComplete",\
"NtPropagationFailed",\
"NtProtectVirtualMemory",\
"NtPulseEvent",\
"NtQueryAttributesFile",\
"NtQueryBootEntryOrder",\
"NtQueryBootOptions",\
"NtQueryDebugFilterState",\
"NtQueryDefaultLocale",\
"NtQueryDefaultUILanguage",\
"NtQueryDirectoryFile",\
"NtQueryDirectoryObject",\
"NtQueryDriverEntryOrder",\
"NtQueryEaFile",\
"NtQueryEvent",\
"NtQueryFullAttributesFile",\
"NtQueryInformationAtom",\
"NtQueryInformationEnlistment",\
"NtQueryInformationFile",\
"NtQueryInformationJobObject",\
"NtQueryInformationPort",\
"NtQueryInformationProcess",\
"NtQueryInformationResourceManager",\
"NtQueryInformationThread",\
"NtQueryInformationToken",\
"NtQueryInformationTransaction",\
"NtQueryInformationTransactionManager",\
"NtQueryInformationWorkerFactory",\
"NtQueryInstallUILanguage",\
"NtQueryIntervalProfile",\
"NtQueryIoCompletion",\
"NtQueryKey",\
"NtQueryLicenseValue",\
"NtQueryMultipleValueKey",\
"NtQueryMutant",\
"NtQueryObject",\
"NtQueryOpenSubKeys",\
"NtQueryOpenSubKeysEx",\
"NtQueryPerformanceCounter",\
"NtQueryPortInformationProcess",\
"NtQueryQuotaInformationFile",\
"NtQuerySection",\
"NtQuerySecurityAttributesToken",\
"NtQuerySecurityObject",\
"NtQuerySemaphore",\
"NtQuerySymbolicLinkObject",\
"NtQuerySystemEnvironmentValue",\
"NtQuerySystemEnvironmentValueEx",\
"NtQuerySystemInformation",\
"NtQuerySystemInformationEx",\
"NtQuerySystemTime",\
"NtQueryTimer",\
"NtQueryTimerResolution",\
"NtQueryValueKey",\
"NtQueryVirtualMemory",\
"NtQueryVolumeInformationFile",\
"NtQueueApcThread",\
"NtQueueApcThreadEx",\
"NtRaiseException",\
"NtRaiseHardError",\
"NtReadFile",\
"NtReadFileScatter",\
"NtReadOnlyEnlistment",\
"NtReadRequestData",\
"NtReadVirtualMemory",\
"NtRecoverEnlistment",\
"NtRecoverResourceManager",\
"NtRecoverTransactionManager",\
"NtRegisterProtocolAddressInformation",\
"NtRegisterThreadTerminatePort",\
"NtReleaseKeyedEvent",\
"NtReleaseMutant",\
"NtReleaseSemaphore",\
"NtReleaseWorkerFactoryWorker",\
"NtRemoveIoCompletion",\
"NtRemoveIoCompletionEx",\
"NtRemoveProcessDebug",\
"NtRenameKey",\
"NtRenameTransactionManager",\
"NtReplaceKey",\
"NtReplacePartitionUnit",\
"NtReplyPort",\
"NtReplyWaitReceivePort",\
"NtReplyWaitReceivePortEx",\
"NtReplyWaitReplyPort",\
"NtRequestPort",\
"NtRequestWaitReplyPort",\
"NtResetEvent",\
"NtResetWriteWatch",\
"NtRestoreKey",\
"NtResumeProcess",\
"NtResumeThread",\
"NtRollbackComplete",\
"NtRollbackEnlistment",\
"NtRollbackTransaction",\
"NtRollforwardTransactionManager",\
"NtSaveKey",\
"NtSaveKeyEx",\
"NtSaveMergedKeys",\
"NtSecureConnectPort",\
"NtSerializeBoot",\
"NtSetBootEntryOrder",\
"NtSetBootOptions",\
"NtSetContextThread",\
"NtSetDebugFilterState",\
"NtSetDefaultHardErrorPort",\
"NtSetDefaultLocale",\
"NtSetDefaultUILanguage",\
"NtSetDriverEntryOrder",\
"NtSetEaFile",\
"NtSetEvent",\
"NtSetEventBoostPriority",\
"NtSetHighEventPair",\
"NtSetHighWaitLowEventPair",\
"NtSetInformationDebugObject",\
"NtSetInformationEnlistment",\
"NtSetInformationFile",\
"NtSetInformationJobObject",\
"NtSetInformationKey",\
"NtSetInformationObject",\
"NtSetInformationProcess",\
"NtSetInformationResourceManager",\
"NtSetInformationThread",\
"NtSetInformationToken",\
"NtSetInformationTransaction",\
"NtSetInformationTransactionManager",\
"NtSetInformationWorkerFactory",\
"NtSetIntervalProfile",\
"NtSetIoCompletion",\
"NtSetIoCompletionEx",\
"NtSetLdtEntries",\
"NtSetLowEventPair",\
"NtSetLowWaitHighEventPair",\
"NtSetQuotaInformationFile",\
"NtSetSecurityObject",\
"NtSetSystemEnvironmentValue",\
"NtSetSystemEnvironmentValueEx",\
"NtSetSystemInformation",\
"NtSetSystemPowerState",\
"NtSetSystemTime",\
"NtSetThreadExecutionState",\
"NtSetTimer",\
"NtSetTimerEx",\
"NtSetTimerResolution",\
"NtSetUuidSeed",\
"NtSetValueKey",\
"NtSetVolumeInformationFile",\
"NtShutdownSystem",\
"NtShutdownWorkerFactory",\
"NtSignalAndWaitForSingleObject",\
"NtSinglePhaseReject",\
"NtStartProfile",\
"NtStopProfile",\
"NtSuspendProcess",\
"NtSuspendThread",\
"NtSystemDebugControl",\
"NtTerminateJobObject",\
"NtTerminateProcess",\
"NtTerminateThread",\
"NtTestAlert",\
"NtThawRegistry",\
"NtThawTransactions",\
"NtTraceControl",\
"NtTraceEvent",\
"NtTranslateFilePath",\
"NtUmsThreadYield",\
"NtUnloadDriver",\
"NtUnloadKey",\
"NtUnloadKey2",\
"NtUnloadKeyEx",\
"NtUnlockFile",\
"NtUnlockVirtualMemory",\
"NtUnmapViewOfSection",\
"NtVdmControl",\
"NtWaitForDebugEvent",\
"NtWaitForKeyedEvent",\
"NtWaitForMultipleObjects",\
"NtWaitForMultipleObjects32",\
"NtWaitForSingleObject",\
"NtWaitForWorkViaWorkerFactory",\
"NtWaitHighEventPair",\
"NtWaitLowEventPair",\
"NtWorkerFactoryWorkerReady",\
"NtWriteFile",\
"NtWriteFileGather",\
"NtWriteRequestData",\
"NtWriteVirtualMemory",\
"NtYieldExecution"};

#endif