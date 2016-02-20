#include "driver.h"
#include "fonction.h"


#define ABSOLUTE(wait) (wait)

#define RELATIVE(wait) (-(wait))

#define NANOSECONDS(nanos) \
(((signed __int64)(nanos)) / 100L)

#define MICROSECONDS(micros) \
(((signed __int64)(micros)) * NANOSECONDS(1000L))

#define MILLISECONDS(milli) \
(((signed __int64)(milli)) * MICROSECONDS(1000L))

#define SECONDS(seconds) \
(((signed __int64)(seconds)) * MILLISECONDS(1000L))


ProcessInformation GetProcessInfo()
{
	PEPROCESS pep;
	ULONG ret;
	NTSTATUS rc,status;
	ProcessInformation  process;
	PFILE_OBJECT fileObject = NULL;
	ANSI_STRING temp;
	UNICODE_STRING temp_unicode;
	PVOID unicode;
	ULONG ppid = 0;
	PCHAR toto;
	
	// On recupere le PID
	process.pid = (LONG)PsGetCurrentProcessId();
	
	
	
	
	PsLookupProcessByProcessId (process.pid,&pep);
	toto = (PCHAR) pep;
	process.ppid = *((ULONG*)(toto+0x140));
	// On recupere les 16 premiers bits du nom du process
	process.name = PsGetProcessImageFileName(pep);

	// On recupere le path complet du process
	rc = ZwQueryInformationProcess(ZwCurrentProcess(),
								ProcessImageFileName,
								NULL,
								0,
								&ret);
								
								
	if(rc == STATUS_INFO_LENGTH_MISMATCH)
	{
		unicode = ExAllocatePool(PagedPool, ret);
		if(unicode != NULL)
		{
			rc = ZwQueryInformationProcess(ZwCurrentProcess(), ProcessImageFileName, unicode, ret, &ret);
			RtlInitUnicodeString(&temp_unicode, unicode );
			RtlUnicodeStringToAnsiString(&temp, &temp_unicode, TRUE);
			sprintf(process.pathname,"%s", &temp.Buffer[4]);
			ExFreePool(unicode);
			RtlFreeAnsiString(&temp);
		}
	}
	
	return process;
}

int GetObjectType(PUNICODE_STRING object)
{	
	
	ANSI_STRING toto; int i;
	char test[555];
	char ext[4];
	int nb_ext3 = 33;
	char * ext3[33]={".exe", \
	".jpg", \
	".dll", \
	".wer", \
	".ini", \
	".sys", \
	".HOM", \
	".log", \
	".edb", \
	".txt", \
	".mp3", \
	".wav", \
	".avi" , \
	".tmp" , \
	".bat" , \
	".dat", \
	".sdb" , \
	".Bin" , \
	".wmv" , \
	".etl" , \
	".xml" , \
	".sqm" , \
	".mui" , \
	".zip" , \
	".chk" ,\
	".DRV" ,\
	".cpl" ,\
	".url" ,\
	".msi" ,\
	".lnk", \
	".chm" ,\
	".cnt" ,\
	".hlp"
	};
	// char * ext4[]={};
	char * ext7[1]={".Config"};
	int type = 0, longueur,aze=0, temp=0;
	char directory1[]="\\";
	char directory2[]=":";
	char device1[]="}";
	
	RtlUnicodeStringToAnsiString(&toto, object, TRUE);
	sprintf(test,"%s",toto.Buffer);
	sprintf(ext,"%s",&test[strlen(test)-1 ]);
	
	if(!strcmp(ext,directory1) )
		return 3; //Directory
	if( !strcmp(ext, directory2) )
			return 3; //Directory
	//if( !strcmp(ext, device1) )
		//	return 4;
	RtlUnicodeStringToAnsiString(&toto, object, TRUE);
	sprintf(test,"%s",toto.Buffer);
	longueur = strlen(test);
	RtlFreeAnsiString(&toto);
	if( (test[1] == 'D'&& test[2]=='E' && test[3]=='V') || (test[1] == 'D'&& test[2]=='e' && test[3]=='v') || (test[4] == 'V'&& test[5]=='o' && test[6]=='l') || (test[4] == 'V'&& test[5]=='O' && test[6]=='L')) return 4; //Device
	if( (test[4] == 'P'&& test[5]=='I' && test[6]=='P' && test[7] =='E' )) return 4; // Device
	if( (test[4] == 'S'&& test[5]=='T' && test[6]=='O' && test[7] =='R' )) return 4; // Device
	if( (test[4] == 'F'&& test[5]=='D' && test[6]=='C' )) return 4; // Device
	if( (test[4] == 'I'&& test[5]=='D' && test[6]=='E') ) return 4; // Device
	if( (test[1] == 'M'&& test[2]=='o' && test[3]=='u' && test[4]=='n' && test[5]=='t' ) ) return 4; //Device
	
	while(longueur > 0)
	{
		if(test[longueur] == '.') break;
		aze++;
		longueur--;
	}
	if(longueur == 0)
		return 3;
	temp = aze;
	
	// type
	// 1 : file
	// 2 : driver
	// 3 : directory
	// 4 : device	
	switch (aze)
	{
		case 2:
			return 1;
			break;
		case 3:
			return 1;
			break;
		case 4:
			sprintf(ext,"%s",&test[strlen(test)-4]);
			for(i=0; i<nb_ext3;i++)
			{
				if(!_stricmp(ext, ext3[i]))
				{
					if(!_stricmp(ext,ext3[5]))
						type = 2;
					else
					{
						if(!_stricmp(ext,ext3[6]))
							type = 3;
						else
							type = 1;
					}
				break;
				}		
			}
			break;
		case 5:
			return 1;
			break;
		case 6:
			return 1;
			break;
		case 7:
			sprintf(ext,"%s",&test[strlen(test)-7]);
			for(i=0; i<1;i++)
			{
				if(!strcmp(ext, ext7[i]))
				{
					type = 1;
				break;
				}		
			}
			return 1;
			break;
		case 9:
			return 1;
			break;
		default:
			return 3;
	}
	return type;
}

int SendRequest(char * text)
{

	ULONG wait;
	int ret = 0;
	LARGE_INTEGER li;

	li.QuadPart=RELATIVE(MICROSECONDS(200));

	KeWaitForSingleObject(&event,Executive,KernelMode,0,0);
	
	DbgPrint("%s\n", text);
	strcpy(&output[8],text);
	wait=1;
	memmove(&output[0],&wait,4);
	
	while(1)
	{
		KeDelayExecutionThread(KernelMode,0,&li);
		memmove(&wait,&output[0],4);
		if(!wait)break;
	}
	memmove(&wait,&output[4],4);
	KeSetEvent(&event,0,0);
	ret = wait;
	// ret = 0;
	return ret;
}

NTSTATUS NewNtOpenKey(
	OUT PHANDLE KeyHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes)
{
	NTSTATUS rc, ntStatus;
	ProcessInformation process;
	char droit[100];
	char droit_auto[10];
	// int  retour_autorite;
	char buffer2[1024];
	// char buffer3[250];
	IO_STATUS_BLOCK iostatus;
	ANSI_STRING ansi_cible;
	
	RtlZeroMemory(&buffer2, 1024);
	// RtlZeroMemory(&buffer3, 250);
	RtlZeroMemory(&droit, 100);
	RtlZeroMemory(&droit_auto, 10);
	
	rc = ( (NTOPENKEY) (*OldNtOpenKey) ) (
			KeyHandle,
			DesiredAccess,
			ObjectAttributes);
	
	process = GetProcessInfo();
	DbgPrint("NtOpenKey : %i  %s\n", process.pid,process.pathname);
	if(NT_SUCCESS(rc))
	{
		process = GetProcessInfo();

		if( (DesiredAccess & FILE_READ_DATA) || (DesiredAccess &STANDARD_RIGHTS_READ) )
		{
			sprintf(droit,"read  ");
			// sprintf(droit_auto,"r");
		}	
		if( (DesiredAccess & FILE_EXECUTE) || (DesiredAccess & FILE_GENERIC_EXECUTE ) )
		{
			sprintf(droit,"%sexecute  ", droit);
			// sprintf(droit_auto,"%sx",droit_auto);
		}
		if( (DesiredAccess & FILE_WRITE_DATA) || (DesiredAccess & STANDARD_RIGHTS_WRITE) )
		{
			sprintf(droit,"%swrite  ", droit);
			// sprintf(droit_auto,"%sw",droit_auto);
		}
		if( (DesiredAccess & STANDARD_RIGHTS_WRITE ) ||  (DesiredAccess & FILE_WRITE_DATA) ||  (DesiredAccess & FILE_WRITE_ATTRIBUTES ) \
		||  (DesiredAccess & FILE_WRITE_EA ) ||  (DesiredAccess & FILE_APPEND_DATA ) )
		{
			sprintf(droit,"%screate  ", droit);
			// sprintf(droit_auto,"%sc",droit_auto);
		}
		if ( (DesiredAccess & DELETE) )
		{	
			sprintf(droit,"%sunlink  ", droit);
			// sprintf(droit_auto,"%su",droit_auto);
		}
		if( (DesiredAccess & FILE_READ_ATTRIBUTES) || (DesiredAccess & READ_CONTROL) )
		{
			sprintf(droit,"%sgetattr  ", droit);
			// sprintf(droit_auto,"%sg",droit_auto);
		}
		if( (DesiredAccess & FILE_WRITE_ATTRIBUTES) || (DesiredAccess & WRITE_DAC)  || (DesiredAccess & FILE_WRITE_EA) )
		{
			sprintf(droit,"%ssetattr  ", droit);
			// sprintf(droit_auto,"%ss",droit_auto);
		}
		if ( (DesiredAccess & FILE_APPEND_DATA) )
		{
			sprintf(droit,"%sappend  ", droit);
			// sprintf(droit_auto,"%sa",droit_auto);
		}	
		if ( (DesiredAccess & MAXIMUM_ALLOWED) )
		{
			sprintf(droit,"%smax  ", droit);	
			// sprintf(droit_auto,"%sm",droit_auto);
		}
		if ( (DesiredAccess & ACCESS_SYSTEM_SECURITY) )
		{
			sprintf(droit,"%ssec  ", droit);
			// sprintf(droit_auto,"%sy",droit_auto);
		}
		if( (DesiredAccess & STANDARD_RIGHTS_REQUIRED) || (DesiredAccess & STANDARD_RIGHTS_ALL) )
		{
			sprintf(droit,"%sdelete  ",droit);
			// sprintf(droit_auto,"%su",droit_auto);
		}
		
		RtlUnicodeStringToAnsiString(&ansi_cible,  ObjectAttributes->ObjectName, TRUE);
		sprintf(buffer2,"%i  %s  %s  Key  %s\n", process.pid, process.pathname, ansi_cible.Buffer, droit_auto);
		SendRequest(buffer2);
		RtlFreeAnsiString(&ansi_cible);
		RtlZeroMemory(&droit_auto, 10);
		RtlZeroMemory(&buffer2, 1024);
		
			
		// retour_autorite = SearchDatabase(process.pathname,droit_auto, ansi_cible.Buffer);
		
	
	} //Fin if NT_SUCCESS(rc)

	 
	return rc;
}


NTSTATUS  NewNtOpenFile(
  __out  PHANDLE FileHandle,
  __in   ACCESS_MASK DesiredAccess,
  __in   POBJECT_ATTRIBUTES ObjectAttributes,
  __out  PIO_STATUS_BLOCK IoStatusBlock,
  __in   ULONG ShareAccess,
  __in   ULONG OpenOptions
)
{
	NTSTATUS retour, status,ntStatus;
	ULONG ret, retLen;
	int i, rc,retour_autorite;
	int bool=0;
	char droit[100];
	int  type;
	char buffer[1024];
	PFILE_OBJECT fileObject = NULL;
	ProcessInformation process;
	ANSI_STRING temp_type;
	IO_STATUS_BLOCK iostatus;
	ANSI_STRING ansi_cible;
	HANDLE file;
	UNICODE_STRING        *path_str = NULL;
	
	
	retour = ( (NTOPENFILE)(*OldNtOpenFile) ) (
				FileHandle,
				DesiredAccess,
				ObjectAttributes,
				IoStatusBlock,
				ShareAccess,
				OpenOptions);
	
					
	if(NT_SUCCESS(retour) && ExGetPreviousMode() == UserMode)
	{
		RtlZeroMemory(&buffer, 1024);
		RtlZeroMemory(&droit, 100);
		process = GetProcessInfo();
		
			
		type =  GetObjectType(ObjectAttributes->ObjectName);

		
		if( (DesiredAccess & FILE_READ_DATA) || (DesiredAccess &STANDARD_RIGHTS_READ) )
		{
			sprintf(droit,"read ");
		}	
		if( (DesiredAccess & FILE_EXECUTE) || (DesiredAccess & FILE_GENERIC_EXECUTE ) )
		{
			sprintf(droit,"%sexecute ", droit);
		}
		if( (DesiredAccess & FILE_WRITE_DATA) || (DesiredAccess & STANDARD_RIGHTS_WRITE) )
		{
			sprintf(droit,"%swrite ", droit);
		}
		if( (DesiredAccess & STANDARD_RIGHTS_WRITE ) ||  (DesiredAccess & FILE_WRITE_DATA) ||  (DesiredAccess & FILE_WRITE_ATTRIBUTES ) \
		||  (DesiredAccess & FILE_WRITE_EA ) ||  (DesiredAccess & FILE_APPEND_DATA ) )
		{
			sprintf(droit,"%screate ", droit);
		}
		if ( (DesiredAccess & DELETE) )
		{	
			sprintf(droit,"%sunlink ", droit);
		}
		if( (DesiredAccess & FILE_READ_ATTRIBUTES) || (DesiredAccess & READ_CONTROL) )
		{
			sprintf(droit,"%sgetattr ", droit);
		}
		if( (DesiredAccess & FILE_WRITE_ATTRIBUTES) || (DesiredAccess & WRITE_DAC)  || (DesiredAccess & FILE_WRITE_EA) )
		{
			sprintf(droit,"%ssetattr ", droit);
		}
		if ( (DesiredAccess & FILE_APPEND_DATA) )
		{
			sprintf(droit,"%sappend ", droit);
		}
		if ( (DesiredAccess & MAXIMUM_ALLOWED) )
		{
			sprintf(droit,"%smax ", droit);	
		}
		if ( (DesiredAccess & ACCESS_SYSTEM_SECURITY) )
		{
			sprintf(droit,"%ssec ", droit);
		}
		
		if ( (DesiredAccess & GENERIC_READ) )
		{
			sprintf(droit,"%sgen_read ", droit);
		}
		
		if ( (DesiredAccess & GENERIC_WRITE) )
		{
			sprintf(droit,"%sgen_write ", droit);
		}
		
		if ( (DesiredAccess & GENERIC_EXECUTE) )
		{
			sprintf(droit,"%sgen_exec ", droit);
		}
		
		if ( (DesiredAccess & GENERIC_ALL) )
		{
			sprintf(droit,"%sgen_all ", droit);
		}
		
		
		RtlUnicodeStringToAnsiString(&ansi_cible,  ObjectAttributes->ObjectName, TRUE);
		if(strcmp( &ansi_cible.Buffer[0], "?") == 0 )
			return retour;
		
		if(strcmp(&ansi_cible.Buffer[strlen(ansi_cible.Buffer)-strlen("SvcIps\\ips.log")], "SvcIps\\ips.log") == 0  && strcmp(process.name,"explorer.exe") ==  0)
			return retour;
		
		
		if(strcmp(&ansi_cible.Buffer[strlen(ansi_cible.Buffer)-strlen("SvcIps\\ips.sys")], "SvcIps\\ips.sys") == 0  && strcmp(process.name,"explorer.exe") ==  0)
			return retour;
					
		if(strcmp(process.name,"Service.exe") ==  0)
			return retour;
			
		
		if(process.pid == 4)
		{
			switch(type)
			{
				case 1:
					sprintf(buffer,"%i/%s/System/file/%i/%s", process.pid,  ansi_cible.Buffer,process.ppid, droit);
					break;
				case 2:
					sprintf(buffer,"%i/%s/System/driver/%i/%s", process.pid, ansi_cible.Buffer,process.ppid, droit);
					break;
				case 3:
					sprintf(buffer,"%i/%s/System/dir/%i/%s", process.pid,  ansi_cible.Buffer, process.ppid,droit);
					break;
				case 4:
					sprintf(buffer,"%i/%s/System/device/%i/%s", process.pid, ansi_cible.Buffer,process.ppid, droit);
					break;
				default:
					sprintf(buffer,"%i/%s/System/null/%i/%s", process.pid,ansi_cible.Buffer, process.ppid,droit);
			}
		
		}
		else
		{
			switch(type)
			{
				case 1:
					sprintf(buffer,"%i/%s/%s/file/%i/%s", process.pid, process.pathname, ansi_cible.Buffer,process.ppid, droit);
					break;
				case 2:
					sprintf(buffer,"%i/%s/%s/driver/%i/%s", process.pid, process.pathname, ansi_cible.Buffer,process.ppid, droit);
					break;
				case 3:
					sprintf(buffer,"%i/%s/%s/dir/%i/%s", process.pid, process.pathname, ansi_cible.Buffer,process.ppid, droit);
					break;
				case 4:
					sprintf(buffer,"%i/%s/%s/device/%i/%s", process.pid, process.pathname, ansi_cible.Buffer, process.ppid,droit);
					break;
				default:
					sprintf(buffer,"%i/%s/%s/null/%i/%s", process.pid, process.pathname, ansi_cible.Buffer,process.ppid, droit);
			}
		}
		
		
		SendRequest(buffer);
		RtlFreeAnsiString(&ansi_cible);
		RtlZeroMemory(&buffer, 1024);
		
	} // Fin if (NT_SUCCESS(rc))
	return retour;
}




NTSTATUS NewNtCreateSection(
	OUT PHANDLE SectionHandle, 
	IN ULONG DesiredAccess, 
	IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL, 
	IN PLARGE_INTEGER MaximumSize OPTIONAL, 
	IN ULONG PageAttributess, 
	IN ULONG SectionAttributes, 
	IN HANDLE FileHandle OPTIONAL )
{
	ProcessInformation process;
    NTSTATUS rc = 0, ntStatus;
	PFILE_OBJECT fileObject = NULL;
	ANSI_STRING toto;
	ANSI_STRING ansi_cible;
	IO_STATUS_BLOCK iostatus;
	int bool=0;
	char buffer[1024];
	char pathname[300];
	char test[300];
	char ext[5];
	char exe[]=".exe";
	char sys[]=".sys";
	int retour_autorite;
	ULONG retLen, ret;
    UNICODE_STRING        *path_str = NULL;
	PPUBLIC_OBJECT_TYPE_INFORMATION publicobj;

	rc = ( (NTCREATESECTION)(*OldNtCreateSection) ) (
		SectionHandle, 
		DesiredAccess, 
		ObjectAttributes, 
		MaximumSize, 
		PageAttributess, 
		SectionAttributes, 
		FileHandle);
		
	process = GetProcessInfo();
		
	if(NT_SUCCESS(rc))
	{
		RtlZeroMemory(&buffer, 1024);
		process = GetProcessInfo();
		
		if((PageAttributess & PAGE_EXECUTE ))  
		{
			
			if(FileHandle != NULL)
			{
				ObReferenceObjectByHandle(FileHandle, 0, 0, KernelMode, &fileObject, NULL);
	
				if (fileObject)
				{
					ntStatus = ObQueryNameString(fileObject, (POBJECT_NAME_INFORMATION)path_str, 0, &retLen);
					path_str = (PUNICODE_STRING)ExAllocatePoolWithTag(NonPagedPool, retLen, 0);
					if (ntStatus == STATUS_INFO_LENGTH_MISMATCH)
					{
						if (path_str)
						{
							ntStatus = ObQueryNameString(fileObject, (POBJECT_NAME_INFORMATION)path_str, retLen, &retLen);
						}
					}
				
				RtlUnicodeStringToAnsiString(&ansi_cible,  path_str, TRUE);
				
				sprintf(ext,"%s",&ansi_cible.Buffer[strlen(ansi_cible.Buffer)-4 ]);
				if(strcmp(process.name,"sc.exe") ==  0)
					return rc;
					
				if( !strcmp(ext,exe) )
				{
					sprintf(buffer,"%i/%s/%s/process/%i/execute ", process.pid, process.pathname, ansi_cible.Buffer);
					SendRequest(buffer);
					RtlFreeAnsiString(&ansi_cible);
					RtlZeroMemory(&buffer, 1024);
				}
				
				if( _stricmp(ext,sys) == 0 )
				{
					if(process.pid==4)
						sprintf(buffer,"%i/System/%s/process/%i/execute ", process.pid, ansi_cible.Buffer ,process.ppid);
					else
						sprintf(buffer,"%i/%s/%s/process/%i/execute ", process.pid,process.pathname, ansi_cible.Buffer , process.ppid);
					SendRequest(buffer);
					RtlFreeAnsiString(&ansi_cible);
					RtlZeroMemory(&buffer, 1024);
				}
						
		
				ObDereferenceObject(fileObject);
				if (path_str)	
					ExFreePoolWithTag(path_str, 0);

				}
			}
		}
	
	} //if (NT_SUCCESS(rc))
	return rc;

}



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
					)
{
	NTSTATUS retour,status;
	ULONG retLen, ret;
    UNICODE_STRING        *path_str = NULL;
	ANSI_STRING ansi_cible;
	ProcessInformation process;
	int type, retour_autorite=0;
	char droit[300];
	char buffer[1024];
	IO_STATUS_BLOCK iostatus;
	
	retour = ((NTCREATEFILE)(OldNtCreateFile)) (
							FileHandle,
							DesiredAccess,
							ObjectAttributes,
							IoStatusBlock,
							AllocationSize,
							FileAttributes,
							ShareAccess,
							CreateDisposition,
							CreateOptions,
							EaBuffer,
							EaLength);
							
	process = GetProcessInfo();
	// DbgPrint("NtCreateFile : %i  %s\n", process.pid,process.pathname);
	
	
		if(NT_SUCCESS(retour) && ExGetPreviousMode() == UserMode)
	{
		RtlZeroMemory(&buffer, 1024);
		RtlZeroMemory(&droit, 100);
		process = GetProcessInfo();
		
			
		type =  GetObjectType(ObjectAttributes->ObjectName);

		
		if( (DesiredAccess & FILE_READ_DATA) || (DesiredAccess &STANDARD_RIGHTS_READ) )
		{
			sprintf(droit,"read ");
		}	
		if( (DesiredAccess & FILE_EXECUTE) || (DesiredAccess & FILE_GENERIC_EXECUTE ) )
		{
			sprintf(droit,"%sexecute ", droit);
		}
		if( (DesiredAccess & FILE_WRITE_DATA) || (DesiredAccess & STANDARD_RIGHTS_WRITE) )
		{
			sprintf(droit,"%swrite ", droit);
		}
		if( (DesiredAccess & STANDARD_RIGHTS_WRITE ) ||  (DesiredAccess & FILE_WRITE_DATA) ||  (DesiredAccess & FILE_WRITE_ATTRIBUTES ) \
		||  (DesiredAccess & FILE_WRITE_EA ) ||  (DesiredAccess & FILE_APPEND_DATA ) )
		{
			sprintf(droit,"%screate ", droit);
		}
		if ( (DesiredAccess & DELETE) )
		{	
			sprintf(droit,"%sunlink ", droit);
		}
		if( (DesiredAccess & FILE_READ_ATTRIBUTES) || (DesiredAccess & READ_CONTROL) )
		{
			sprintf(droit,"%sgetattr ", droit);
		}
		if( (DesiredAccess & FILE_WRITE_ATTRIBUTES) || (DesiredAccess & WRITE_DAC)  || (DesiredAccess & FILE_WRITE_EA) )
		{
			sprintf(droit,"%ssetattr ", droit);
		}
		if ( (DesiredAccess & FILE_APPEND_DATA) )
		{
			sprintf(droit,"%sappend ", droit);
		}
		if ( (DesiredAccess & MAXIMUM_ALLOWED) )
		{
			sprintf(droit,"%smax ", droit);	
		}
		if ( (DesiredAccess & ACCESS_SYSTEM_SECURITY) )
		{
			sprintf(droit,"%ssec ", droit);
		}
		
		if ( (DesiredAccess & GENERIC_READ) )
		{
			sprintf(droit,"%sgen_read ", droit);
		}
		
		if ( (DesiredAccess & GENERIC_WRITE) )
		{
			sprintf(droit,"%sgen_write ", droit);
		}
		
		if ( (DesiredAccess & GENERIC_EXECUTE) )
		{
			sprintf(droit,"%sgen_exec ", droit);
		}
		
		if ( (DesiredAccess & GENERIC_ALL) )
		{
			sprintf(droit,"%sgen_all ", droit);
		}
				
		RtlUnicodeStringToAnsiString(&ansi_cible,  ObjectAttributes->ObjectName, TRUE);
		if(strcmp( &ansi_cible.Buffer[0], "?") == 0 )
			return retour;
		
		if(strcmp(&ansi_cible.Buffer[strlen(ansi_cible.Buffer)-strlen("SvcIps\\ips.log")], "SvcIps\\ips.log") == 0  && strcmp(process.name,"explorer.exe") ==  0)
			return retour;
		
		if(strcmp(&ansi_cible.Buffer[strlen(ansi_cible.Buffer)-strlen("SvcIps\\ips.sys")], "SvcIps\\ips.sys") == 0  && strcmp(process.name,"explorer.exe") ==  0)
			return retour;
	
		if(strcmp(process.name,"Service.exe") ==  0)
			return retour;
			
		if(process.pid == 4)
		{
			switch(type)
			{
				case 1:
					sprintf(buffer,"%i/%s/System/file/%i/%s", process.pid,  ansi_cible.Buffer,process.ppid, droit);
					break;
				case 2:
					sprintf(buffer,"%i/%s/System/driver/%i/%s", process.pid, ansi_cible.Buffer,process.ppid, droit);
					break;
				case 3:
					sprintf(buffer,"%i/%s/System/dir/%i/%s", process.pid,  ansi_cible.Buffer,process.ppid, droit);
					break;
				case 4:
					sprintf(buffer,"%i/%s/System/device/%i/%s", process.pid, ansi_cible.Buffer,process.ppid, droit);
					break;
				default:
					sprintf(buffer,"%i/%s/System/null/%i/%s", process.pid,ansi_cible.Buffer,process.ppid, droit);
			}
		
		}
		else
		{
			switch(type)
			{
				case 1:
					sprintf(buffer,"%i/%s/%s/file/%i/%s", process.pid, process.pathname, ansi_cible.Buffer, process.ppid,droit);
					break;
				case 2:
					sprintf(buffer,"%i/%s/%s/driver/%i/%s", process.pid, process.pathname, ansi_cible.Buffer,process.ppid, droit);
					break;
				case 3:
					sprintf(buffer,"%i/%s/%s/dir/%i/%s", process.pid, process.pathname, ansi_cible.Buffer, process.ppid,droit);
					break;
				case 4:
					sprintf(buffer,"%i/%s/%s/device/%i/%s", process.pid, process.pathname, ansi_cible.Buffer, process.ppid,droit);
					break;
				default:
					sprintf(buffer,"%i/%s/%s/null/%i/%s", process.pid, process.pathname, ansi_cible.Buffer,process.ppid, droit);
			}
		}
		
		
		if(strcmp(&ansi_cible.Buffer[strlen(ansi_cible.Buffer)-strlen(".exe")], ".exe") == 0 || \
		   strcmp(&ansi_cible.Buffer[strlen(ansi_cible.Buffer)-strlen(".EXE")], ".EXE") == 0 || \
		   strcmp(&ansi_cible.Buffer[strlen(ansi_cible.Buffer)-strlen(".dll")], ".dll") == 0 || \
		   strcmp(&ansi_cible.Buffer[strlen(ansi_cible.Buffer)-strlen(".DLL")], ".DLL") == 0 || \
		   strcmp(&ansi_cible.Buffer[strlen(ansi_cible.Buffer)-strlen(".sys")], ".sys") == 0 || \
		   strcmp(&ansi_cible.Buffer[strlen(ansi_cible.Buffer)-strlen(".SYS")], ".SYS") == 0 || \
		   type == 4)
				SendRequest(buffer);
		RtlFreeAnsiString(&ansi_cible);
		RtlZeroMemory(&buffer, 1024);
		
	} // Fin if (NT_SUCCESS(rc))
		return retour;
}


