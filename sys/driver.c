#include "driver.h"


/**
  * @description : Point d'entree du driver
  * @param : DriverObject
  * @param : RegistryPath == chemin dans le registre
  * @return : NTSTATUS
  **/
NTSTATUS DriverEntry(
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING RegistryPath
    )
{

	PDEVICE_OBJECT         deviceObject        = NULL; 
	NTSTATUS               ntStatus; 
	WCHAR                  deviceNameBuffer[]  = L"\\Device\\"DRIVER_NAME; 
	UNICODE_STRING         deviceNameUnicodeString; 
	WCHAR                  deviceLinkBuffer[]  = L"\\DosDevices\\"DRIVER_NAME; 
	UNICODE_STRING         deviceLinkUnicodeString; 
	WCHAR filename[] = L"\\??\\C:\\logips.txt";
	UNICODE_STRING file_path;
	OBJECT_ATTRIBUTES objatt;
	IO_STATUS_BLOCK iostatus;
	FILE_STANDARD_INFORMATION fileinfo;
	FILE_POSITION_INFORMATION filepos;
	int i;
	
	
	// On ecrit en UNICODE le nom du driver
	RtlInitUnicodeString (&deviceNameUnicodeString, deviceNameBuffer);
	
		// Creation du peripherique
	ntStatus = IoCreateDevice (DriverObject, 
								0, 
								&deviceNameUnicodeString, 
								0, 
								0, 
								TRUE, 
								&deviceObject 
								); 
	// Si la creation du peripherique a reussit, on peut faire le hook							
	if (NT_SUCCESS(ntStatus)) { 
		RtlInitUnicodeString (&deviceLinkUnicodeString, deviceLinkBuffer);
		ntStatus = IoCreateSymbolicLink (&deviceLinkUnicodeString, 
										 &deviceNameUnicodeString);
										 
		if (!NT_SUCCESS(ntStatus)) {
			IoDeleteDevice (deviceObject); 
			DbgPrint("DriverEntry: IoCreationSymbolicLink failed");
			return ntStatus; 
		} 
		
			DriverObject->MajorFunction[IRP_MJ_CREATE]         = DrvCreateClose;
		DriverObject->MajorFunction[IRP_MJ_CLOSE]          = DrvCreateClose;
		DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DriverDispatch;
		DriverObject->DriverUnload                         = DriverUnload; 
		
		KeInitializeEvent(&event,SynchronizationEvent,1);
		
		return STATUS_SUCCESS; 
	} 
	//La creation du peripherique a echoue
	else {
		DbgPrint("DriverEntry: IOCreateDevice failed");
		return ntStatus; 
	}
}


/**
 * @description : fonction permettant de decharger correctement le driver
 * @param : DriverObject
 * @return : void
 **/
VOID DriverUnload(
    IN PDRIVER_OBJECT DriverObject
    )
{
    WCHAR                  deviceLinkBuffer[]  = L"\\DosDevices\\"DRIVER_NAME;
    UNICODE_STRING         deviceLinkUnicodeString;

    
	RtlInitUnicodeString (&deviceLinkUnicodeString,
                          deviceLinkBuffer
                          );
	//Suppression des objets crees dans le DriverEntry
    IoDeleteSymbolicLink (&deviceLinkUnicodeString);
    IoDeleteDevice (DriverObject->DeviceObject);
}


/**
 * @description : Fonction qui va recuperer les IRP : DeviceIO
 * @param : DeviceObject
 * @param : IRP
 * @return : NTSTATUS
 **/
NTSTATUS DriverDispatch(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    )
{
	UCHAR * buff = 0; 
	ULONG address;
	PIO_STACK_LOCATION IrpSp;
	NTSTATUS               ntStatus; 
	IrpSp =  IoGetCurrentIrpStackLocation(Irp);
	

	if(IrpSp->Parameters.DeviceIoControl.IoControlCode==6666)
	{
		buff = (UCHAR*) Irp->AssociatedIrp.SystemBuffer;

		memmove(&address,&buff[0],4);
		output=(char*)MmMapIoSpace(MmGetPhysicalAddress((void*)address),1024,0);
		ntStatus=Hook_Function();
	}
	
	if(IrpSp->Parameters.DeviceIoControl.IoControlCode==7777)
	{
		//Fonction qui va enlever les hooks posees
		Unhook_fonction();
		MmUnmapIoSpace(output,1024);
		
		
	}
	

    Irp->IoStatus.Status      = STATUS_SUCCESS;
    IoCompleteRequest (Irp,
                       IO_NO_INCREMENT // choisir le mode que l'on souhaite
                       );
    return Irp->IoStatus.Status;
}

/**
 * @description : Fonction qui va recuperer les IRP CREATE ET CLOSE
 * @param : DeviceObject
 * @param : IRP
 * @return : NTSTATUS
 **/

NTSTATUS DrvCreateClose(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    )
{
	Irp->IoStatus.Status      = STATUS_SUCCESS;
    IoCompleteRequest (Irp,
                       IO_NO_INCREMENT // choisir le mode que l'on souhaite
                       );
    return Irp->IoStatus.Status;
}

/**
 * @description : Fonction plaçant les hooks sur la SSDT
 * @param : aucun
 * @return : NTSTATUS
 **/
 NTSTATUS Hook_Function()
 {
	g_pmdlSystemCall=IoAllocateMdl(KeServiceDescriptorTable.ServiceTableBase, KeServiceDescriptorTable.NumberOfServices*4, 0, 0, NULL);

   	if(!g_pmdlSystemCall)
      	return STATUS_UNSUCCESSFUL;

	//The MmBuildMdlForNonPagedPool routine receives an MDL that specifies a virtual memory buffer in nonpaged pool,
	//and updates it to describe the underlying physical pages. 
   	MmBuildMdlForNonPagedPool(g_pmdlSystemCall);
	
	//The MmMapLockedPages routine maps the physical pages that are described by a given MDL.
	
	MappedSystemCallTable=MmMapLockedPages(g_pmdlSystemCall, KernelMode);

	__try{
		OldNtCreateSection = (PVOID) InterlockedExchange( (PLONG) &MappedSystemCallTable[84], (LONG) NewNtCreateSection);
		// OldNtOpenKey =  (PVOID) InterlockedExchange( (PLONG) &MappedSystemCallTable[182], (LONG) NewNtOpenKey);
		OldNtOpenFile =  (PVOID) InterlockedExchange( (PLONG) &MappedSystemCallTable[179], (LONG) NewNtOpenFile);
		OldNtCreateFile = (PVOID) InterlockedExchange( (PLONG) &MappedSystemCallTable[66], (LONG) NewNtCreateFile);
		// OldNtAccessCheck = (PVOID) InterlockedExchange( (PLONG) &MappedSystemCallTable[1], (LONG) NewNtAccessCheck);
	}
	__except(1){
			DbgPrint("DriverEntry: Hook failed");

	}
	return STATUS_SUCCESS;
 }
 
 /**
  * @description : permet de de-hooker la SSDT
  * @param :  aucun
  * @return : void
  **/
 void Unhook_fonction()
{	
g_pmdlSystemCall=IoAllocateMdl(KeServiceDescriptorTable.ServiceTableBase, KeServiceDescriptorTable.NumberOfServices*4, 0, 0, NULL);

	__try{
	InterlockedExchange( (PLONG) &MappedSystemCallTable[66], (LONG) OldNtCreateFile);
	InterlockedExchange( (PLONG) &MappedSystemCallTable[84], (LONG) OldNtCreateSection);
	InterlockedExchange( (PLONG) &MappedSystemCallTable[179], (LONG) OldNtOpenFile);
	// InterlockedExchange( (PLONG) &MappedSystemCallTable[182], (LONG) OldNtOpenKey);
	// InterlockedExchange( (PLONG) &MappedSystemCallTable[1], (LONG) OldNtAccessCheck);
	
	}
	__except(1){
			DbgPrint("DriverEntry: Unhook failed");
	}
 
    // Unlock and Free MDL
	if(g_pmdlSystemCall)
	{
		MmUnmapLockedPages(MappedSystemCallTable, g_pmdlSystemCall);
		IoFreeMdl(g_pmdlSystemCall);
	}
}
