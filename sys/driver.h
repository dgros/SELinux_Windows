
#ifndef _DRIVER_
#define _DRIVER_

#include "ntifs.h"
#include <ntddk.h>
#include <stdio.h>
#include <sddl.h>
#include <string.h>
//#include <Wdm.h>




#define FILEIO_TYPE 40001

// Definition des defines et des variables
#define DRIVER_NAME L"ips"
UNICODE_STRING RegistryPath_Save;

HANDLE handlelog;
HANDLE pipehandle;
unsigned int save_add;
KEVENT event;
char *output; //Buffer d'ecriture pour la communication US/KM


//Structure contenant les informations sur le processus courant
typedef struct ProcessInformation{
	ULONG pid;
	ULONG ppid;
	UCHAR *name;
	char pathname[1024];
	// UNICODE_STRING pathname;
} ProcessInformation;
	
// Declaration des fonctions uitilisees 
VOID DriverUnload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS DriverDispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DrvCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS Hook_Function();
void Unhook_fonction();
NTSTATUS afficher_ssdt();
ProcessInformation GetProcessInfo();

int LoadPolicy();
void test();
int SearchDatabase(char * name, char * droit, char * cible);

#pragma pack(1)
typedef struct ServiceDescriptorEntry {
        unsigned int *ServiceTableBase;
        unsigned int *ServiceCounterTableBase; //Used only in checked build
        unsigned int NumberOfServices;
        unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry_t, *PServiceDescriptorTableEntry_t;
#pragma pack()

__declspec(dllimport)  ServiceDescriptorTableEntry_t KeServiceDescriptorTable;
#define SYSTEMSERVICE(_function)  KeServiceDescriptorTable.ServiceTableBase[ *(PULONG)((PUCHAR)_function+1)]

PMDL  g_pmdlSystemCall;
PVOID *MappedSystemCallTable;

// Redéfinition de PsLookupProcessByProcessId (modifier ntifs.h)   
NTSYSAPI
NTSTATUS
NTAPI
PsLookupProcessByProcessId (
            IN ULONG ProcessId,
            OUT PEPROCESS *Process);
UCHAR *PsGetProcessImageFileName( IN PEPROCESS Process );

//Fin de ntifs
NTSYSAPI
NTSTATUS
NTAPI ZwQueryInformationProcess(
  __in       HANDLE ProcessHandle,
  __in       PROCESSINFOCLASS ProcessInformationClass,
  __out      PVOID ProcessInformation,
  __in       ULONG ProcessInformationLength,
  __out_opt  PULONG ReturnLength
);

//Appels systeme utilises : En mode noyau, il faut utiliser les Zw*

NTSYSAPI 
NTSTATUS
NTAPI

ZwCreateNamedPipeFile(
OUT PHANDLE NamedPipeFileHandle, 
IN ACCESS_MASK DesiredAccess, 
IN POBJECT_ATTRIBUTES ObjectAttributes, 
OUT PIO_STATUS_BLOCK IoStatusBlock, 
IN ULONG ShareAccess, 
IN ULONG CreateDisposition,
 IN ULONG CreateOptions, 
 IN BOOLEAN WriteModeMessage, 
 IN BOOLEAN ReadModeMessage, 
 IN BOOLEAN NonBlocking, 
 IN ULONG MaxInstances, 
 IN ULONG InBufferSize, 
 IN ULONG OutBufferSize, 
 IN PLARGE_INTEGER DefaultTimeOut );


NTSYSAPI
NTSTATUS
NTAPI
ZwOpenFile(
	OUT PHANDLE phFile,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PIO_STATUS_BLOCK pIoStatusBlock,
	IN ULONG ShareMode,
	IN ULONG OpenMode
);

NTSYSAPI
NTSTATUS
NTAPI
  ZwReadFile(
    IN HANDLE  FileHandle,
    IN HANDLE  Event  OPTIONAL,
    IN PIO_APC_ROUTINE  ApcRoutine  OPTIONAL,
    IN PVOID  ApcContext  OPTIONAL,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    OUT PVOID  Buffer,
    IN ULONG  Length,
    IN PLARGE_INTEGER  ByteOffset  OPTIONAL,
    IN PULONG  Key  OPTIONAL
    );


NTSYSAPI
NTSTATUS
NTAPI
ZwCreateFile(
    OUT PHANDLE phFile,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize OPTIONAL,
    IN ULONG FileAttributes,
    IN ULONG ShareAccess,
    IN ULONG CreateDisposition,
    IN ULONG CreateOptions,
    IN PVOID EaBuffer OPTIONAL,
    IN ULONG EaLength
);


// Signature des fonctions à hooker


//NtOpenKey
NTSYSAPI
NTSTATUS
NTAPI
NtOpenKey(
	OUT PHANDLE KeyHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes);
	 
typedef NTSTATUS (*NTOPENKEY)(
	OUT PHANDLE KeyHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes);

NTOPENKEY OldNtOpenKey;

NTSTATUS NewNtOpenKey(
	OUT PHANDLE KeyHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes);

//Fin NtOpenKey

//Relatif aux fichiers
NTSYSAPI
NTSTATUS
NTAPI
 NtOpenFile(
  __out  PHANDLE FileHandle,
  __in   ACCESS_MASK DesiredAccess,
  __in   POBJECT_ATTRIBUTES ObjectAttributes,
  __out  PIO_STATUS_BLOCK IoStatusBlock,
  __in   ULONG ShareAccess,
  __in   ULONG OpenOptions
);

typedef NTSTATUS (*NTOPENFILE) (
  __out  PHANDLE FileHandle,
  __in   ACCESS_MASK DesiredAccess,
  __in   POBJECT_ATTRIBUTES ObjectAttributes,
  __out  PIO_STATUS_BLOCK IoStatusBlock,
  __in   ULONG ShareAccess,
  __in   ULONG OpenOptions
);

NTOPENFILE OldNtOpenFile;

NTSTATUS  NewNtOpenFile(
  __out  PHANDLE FileHandle,
  __in   ACCESS_MASK DesiredAccess,
  __in   POBJECT_ATTRIBUTES ObjectAttributes,
  __out  PIO_STATUS_BLOCK IoStatusBlock,
  __in   ULONG ShareAccess,
  __in   ULONG OpenOptions
);

// Relatif a NtCreateFile
NTSYSAPI
NTSTATUS
NTAPI
NtCreateFile(
    OUT PHANDLE phFile,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PLARGE_INTEGER AllocationSize OPTIONAL,
    IN ULONG FileAttributes,
    IN ULONG ShareAccess,
    IN ULONG CreateDisposition,
    IN ULONG CreateOptions,
    IN PVOID EaBuffer OPTIONAL,
    IN ULONG EaLength
);

typedef NTSTATUS (*NTCREATEFILE)(
            PHANDLE FileHandle,
            ACCESS_MASK DesiredAccess,
            POBJECT_ATTRIBUTES ObjectAttributes,
            PIO_STATUS_BLOCK IoStatusBlock,
            PLARGE_INTEGER AllocationSize OPTIONAL,
            ULONG FileAttributes,
            ULONG ShareAccess,
            ULONG CreateDisposition,
            ULONG CreateOptions,
            PVOID EaBuffer OPTIONAL,
            ULONG EaLength
);

NTCREATEFILE OldNtCreateFile;

NTSTATUS NewNtCreateFile(
                    PHANDLE FileHandle,
                    ACCESS_MASK DesiredAccess,
                    POBJECT_ATTRIBUTES ObjectAttributes,
                    PIO_STATUS_BLOCK IoStatusBlock,
                    PLARGE_INTEGER AllocationSize OPTIONAL,
                    ULONG FileAttributes,
                    ULONG ShareAccess,
                    ULONG CreateDisposition,
                    ULONG CreateOptions,
                    PVOID EaBuffer OPTIONAL,
                    ULONG EaLength
);
// fin de Relatif a NtCreateFile


//NtAccessCheckAndAuditAlarm

NTSYSAPI 
NTSTATUS
NTAPI
NtAccessCheckAndAuditAlarm(
    __in PUNICODE_STRING SubsystemName,
    __in_opt PVOID HandleId,
    __in PUNICODE_STRING ObjectTypeName,
    __in PUNICODE_STRING ObjectName,
    __in PSECURITY_DESCRIPTOR SecurityDescriptor,
    __in ACCESS_MASK DesiredAccess,
    __in PGENERIC_MAPPING GenericMapping,
    __in BOOLEAN ObjectCreation,
    __out PACCESS_MASK GrantedAccess,
    __out PNTSTATUS AccessStatus,
    __out PBOOLEAN GenerateOnClose
); 



typedef NTSTATUS (*NTACCESSCHECKANDAUDITALARM) (
    __in PUNICODE_STRING SubsystemName,
    __in_opt PVOID HandleId,
    __in PUNICODE_STRING ObjectTypeName,
    __in PUNICODE_STRING ObjectName,
    __in PSECURITY_DESCRIPTOR SecurityDescriptor,
    __in ACCESS_MASK DesiredAccess,
    __in PGENERIC_MAPPING GenericMapping,
    __in BOOLEAN ObjectCreation,
    __out PACCESS_MASK GrantedAccess,
    __out PNTSTATUS AccessStatus,
    __out PBOOLEAN GenerateOnClose
);

NTACCESSCHECKANDAUDITALARM OdlNtAccessCheckAndAuditAlarm;


NTSTATUS NewNtAccessCheckAndAuditAlarm(
    __in PUNICODE_STRING SubsystemName,
    __in_opt PVOID HandleId,
    __in PUNICODE_STRING ObjectTypeName,
    __in PUNICODE_STRING ObjectName,
    __in PSECURITY_DESCRIPTOR SecurityDescriptor,
    __in ACCESS_MASK DesiredAccess,
    __in PGENERIC_MAPPING GenericMapping,
    __in BOOLEAN ObjectCreation,
    __out PACCESS_MASK GrantedAccess,
    __out PNTSTATUS AccessStatus,
    __out PBOOLEAN GenerateOnClose 
); 
// NtAccessCheck
NTSYSAPI 
NTSTATUS
NTAPI
NtAccessCheck(
	IN PSECURITY_DESCRIPTOR SecurityDescriptor, 
	IN HANDLE ClientToken, 
	IN ACCESS_MASK DesiredAccess, 
	IN PGENERIC_MAPPING GenericMapping OPTIONAL, 
	OUT PPRIVILEGE_SET RequiredPrivilegesBuffer, 
	IN OUT PULONG BufferLength, 
	OUT PACCESS_MASK GrantedAccess, 
	OUT PNTSTATUS AccessStatus ); 
	
typedef NTSTATUS (*NTACCESSCHECK) (
	PSECURITY_DESCRIPTOR SecurityDescriptor, 
	HANDLE ClientToken, 
	ACCESS_MASK DesiredAccess, 
	PGENERIC_MAPPING GenericMapping, 
	PPRIVILEGE_SET RequiredPrivilegesBuffer, 
	PULONG BufferLength, 
	PACCESS_MASK GrantedAccess, 
	PNTSTATUS AccessStatus ); 
	
NTACCESSCHECK OldNtAccessCheck;

NTSTATUS NewNtAccessCheck(
	IN PSECURITY_DESCRIPTOR SecurityDescriptor, 
	IN HANDLE ClientToken, 
	IN ACCESS_MASK DesiredAccess, 
	IN PGENERIC_MAPPING GenericMapping OPTIONAL, 
	OUT PPRIVILEGE_SET RequiredPrivilegesBuffer, 
	IN OUT PULONG BufferLength, 
	OUT PACCESS_MASK GrantedAccess, 
	OUT PNTSTATUS AccessStatus ); 


//Fin NtAccessCheck

//NtCreateSection
NTSYSAPI 
NTSTATUS
NTAPI
NtCreateSection(
	OUT PHANDLE SectionHandle, 
	IN ULONG DesiredAccess, 
	IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL, 
	IN PLARGE_INTEGER MaximumSize OPTIONAL, 
	IN ULONG PageAttributess, 
	IN ULONG SectionAttributes, 
	IN HANDLE FileHandle OPTIONAL ); 
	
typedef NTSTATUS (*NTCREATESECTION)(
	HANDLE SectionHandle, 
	ULONG DesiredAccess, 
	POBJECT_ATTRIBUTES ObjectAttributes ,
	PLARGE_INTEGER MaximumSize,
	ULONG PageAttributess, 
	LONG SectionAttributes, 
	HANDLE FileHandle ); 
	
NTCREATESECTION OldNtCreateSection;

NTSTATUS NewNtCreateSection(
	OUT PHANDLE SectionHandle, 
	IN ULONG DesiredAccess, 
	IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL, 
	IN PLARGE_INTEGER MaximumSize OPTIONAL, 
	IN ULONG PageAttributess, 
	IN ULONG SectionAttributes, 
	IN HANDLE FileHandle OPTIONAL ); 

// Fin NtCreateSection


struct _SYSTEM_THREADS
{
        LARGE_INTEGER           KernelTime;
        LARGE_INTEGER           UserTime;
        LARGE_INTEGER           CreateTime;
        ULONG                   WaitTime;
        PVOID                   StartAddress;
        CLIENT_ID               ClientIs;
        KPRIORITY               Priority;
        KPRIORITY               BasePriority;
        ULONG                   ContextSwitchCount;
        ULONG                   ThreadState;
        KWAIT_REASON            WaitReason;
};

struct _SYSTEM_PROCESSES
{
        ULONG                  	NextEntryDelta;
        ULONG                  	ThreadCount;
        ULONG                   Reserved[6];
        LARGE_INTEGER           CreateTime;
        LARGE_INTEGER           UserTime;
        LARGE_INTEGER           KernelTime;
        UNICODE_STRING          ProcessName;
        KPRIORITY               BasePriority;
        ULONG                   ProcessId;
        ULONG                   InheritedFromProcessId;
        ULONG                   HandleCount;
        ULONG                   Reserved2[2];
        VM_COUNTERS            	VmCounters;
        IO_COUNTERS             IoCounters; //windows 2000 only
        struct _SYSTEM_THREADS  Threads[1];
};
			


#endif