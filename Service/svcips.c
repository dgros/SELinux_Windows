#include "main.h"


#define SLEEP_TIME 5000
#define LOGFILE "\\SvcIps\\ips.log"
#define DriverName "ips"

int WriteToLog(char* str)
{
	FILE* log;
	log = fopen(LOGFILE, "a+");
	if (log == NULL)
		return -1;
	fprintf(log, "%s\n", str);
	fclose(log);
	return 0;
}

int main()
{
    SERVICE_TABLE_ENTRY ServiceTable[2];
    ServiceTable[0].lpServiceName = "svcips";
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;

    StartServiceCtrlDispatcher(ServiceTable);
    return 1;
}


void ServiceMain(int argc, char** argv)
{
    int error;
    int result;

    ServiceStatus.dwServiceType        = SERVICE_WIN32;
    ServiceStatus.dwCurrentState       = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode      = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint         = 0;
    ServiceStatus.dwWaitHint           = 0;

    hStatus = RegisterServiceCtrlHandler("svcips", (LPHANDLER_FUNCTION)ControlHandler);
    if (hStatus == (SERVICE_STATUS_HANDLE)0)
    {
        return;
    }

    error = InitService();
    if (error)
    {
	    ServiceStatus.dwCurrentState       = SERVICE_STOPPED;
        ServiceStatus.dwWin32ExitCode      = -2;
        SetServiceStatus(hStatus, &ServiceStatus);
        return;
    }

    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus (hStatus, &ServiceStatus);

result=0;

    while (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
	{

		if (result)
		{
			ServiceStatus.dwCurrentState       = SERVICE_STOPPED;
			ServiceStatus.dwWin32ExitCode      = -3;
			SetServiceStatus(hStatus, &ServiceStatus);
			return;
		}

		Sleep(SLEEP_TIME);
	}
    return;
}


int InitService()
{
    int result;

    SC_HANDLE service;
    SC_HANDLE  schService;
    DWORD      err;
    DWORD      dw;
    SERVICE_STATUS  serviceStatus;
    BOOL ret;
    DWORD buff[64];

    result = 0;
    manager = OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);

	service = CreateService(manager,
                            DriverName,
                            DriverName,
                            SERVICE_ALL_ACCESS,
                            SERVICE_KERNEL_DRIVER,
                            SERVICE_DEMAND_START,
                            SERVICE_ERROR_NORMAL,
                            "\\SvcIps\\ips.sys",
                            0,0,0,0,0);

    if (service == NULL)
    {
        err = GetLastError();

        if (err == ERROR_SERVICE_EXISTS)
        {
            schService = OpenService (manager, DriverName, SERVICE_ALL_ACCESS);

            ControlService(schService, SERVICE_CONTROL_STOP, &serviceStatus);
            ret = DeleteService (schService);
            service = CreateService(manager,
                                    DriverName,
                                    DriverName,
                                    SERVICE_ALL_ACCESS,
                                    SERVICE_KERNEL_DRIVER,
                                    SERVICE_DEMAND_START,
                                    SERVICE_ERROR_NORMAL,
                                    "\\SvcIps\\ips.sys",0,0,0,0,0);
        }

    }

    schService = OpenService (manager,
                              DriverName,
                              SERVICE_ALL_ACCESS
                              );

    if (schService == NULL)
    {
        return -1;
    }

    ret = StartService (schService,    // service identifier
                        0,             // number of arguments
                        NULL           // pointer to arguments
                        );

	StartService(service,0,NULL);
	CloseServiceHandle(service);
	CloseServiceHandle(schService);
	// Chargement des context

    RemplirListe();
    RemplirListeProcess();
    WriteToLog("Politique chargée...");
   remplirstruct();
   RemplirPolitique();


    // Chargement des politiques
    CreateThread(0,0,(LPTHREAD_START_ROUTINE)ListenToDriver,0,0,&dw);

    device=CreateFile("\\\\.\\ips",GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_SYSTEM,0);
    ZeroMemory(buffdriver,1042);
    buff[0]=(DWORD)&buffdriver[0];
    DeviceIoControl(device,6666,buff,100,buff,100,&dw,0);

    return(result);
}


void ControlHandler(DWORD request)
{
    SC_HANDLE       schService;
//    SC_HANDLE       SchSCManager;
    BOOL            ret;
    SERVICE_STATUS  serviceStatus;
    DWORD buff[64];
    DWORD dw;

    switch(request)
    {
        case SERVICE_CONTROL_STOP:
            DeviceIoControl(device,7777,buff,100,buff,100,&dw,0);
                CloseHandle(device);
                ZeroMemory(buffdriver, sizeof(buffdriver));
                schService = OpenService (manager,
                                          DriverName,
                                          SERVICE_ALL_ACCESS
                                          );

                if (schService == NULL)
                {
                  return;
                }

                ret = ControlService (schService,
                                      SERVICE_CONTROL_STOP,
                                      &serviceStatus
                                      );

               CloseServiceHandle (schService);
                schService = OpenService (manager,
                                          DriverName,
                                          SERVICE_ALL_ACCESS
                                          );
                ret = DeleteService (schService);
                ret = DeleteService (schService);
                ret = DeleteService (schService);

                CloseServiceHandle (schService);


            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED;
            SetServiceStatus (hStatus, &ServiceStatus);
            return;

        case SERVICE_CONTROL_SHUTDOWN:
            DeviceIoControl(device,7777,buff,100,buff,100,&dw,0);
                CloseHandle(device);
              //SchSCManager = OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);
                schService = OpenService (manager,
                                          DriverName,
                                          SERVICE_ALL_ACCESS
                                          );

                if (schService == NULL)
                {
                     return;
                }

                ret = ControlService (schService,
                                      SERVICE_CONTROL_STOP,
                                      &serviceStatus
                                      );
                ret = DeleteService (schService);


                CloseServiceHandle (schService);


            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState  = SERVICE_STOPPED;
            SetServiceStatus (hStatus, &ServiceStatus);
            return;

        default:
            break;
    }
    SetServiceStatus (hStatus,  &ServiceStatus);

    return;
}

void ListenToDriver()
{
    DWORD resp;
    char buffer[1024];
    char * name;
    char * tok;
    char * tok1;
    char * tok2;
    char * tok3;
    char * tok4;
    char * tok5;
    char * type1;
    char * types;
    char type2[50];
    int pid=0;
    int ppid=0;
    int trace = 0;
    int ret = 10;
    char temporaire[50];
    char temporaire1[50];
    char scontext[50];
    char tcontext[50];
    char context[50];
    char tclass[15];
    char action[100];
    char temp_path[512];
    char temp_path_object[512];
    char temp[512];
    char comm[520];
    time_t rawtime;
    struct tm * timeinfo;
    char * month[12]={"Jan","Fev","Mar","Apr","May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};


    GetEnvironnement();

	while(1)
	{

		memmove(&resp,&buffdriver[0],4);

		if(!resp)
		{
            Sleep(10);
            continue;
        }

        pid=0;
        ret= 0;
        ZeroMemory( temporaire, 50);
        ZeroMemory( temporaire1, 50);
        ZeroMemory( scontext, 50);
        ZeroMemory( tcontext, 50);
        ZeroMemory( type2, 50);
        ZeroMemory( context, 50);
        ZeroMemory( comm, 520);
        ZeroMemory( tclass, 15);
        ZeroMemory( action, 100);
        ZeroMemory( path, 512);
        ZeroMemory( temp, 512);
        ZeroMemory( temp_path, 512);
        ZeroMemory( temp_path_object, 512);

        name =  (char *) &buffdriver[8];

        sprintf(buffer,"%s",name);
        tok = strtok(name,"/");
		tok1 = strtok(NULL,"/");
		tok2 = strtok(NULL,"/");
		tok3 = strtok(NULL,"/");
		tok4 = strtok(NULL,"/");
		tok5 = strtok(NULL,"/");

        // Get Pid
		if(tok != NULL)
            pid = atoi(tok);
        // Get PPid
		if(tok4 != NULL)
            ppid = atoi(tok4);
        if(ppid <0 || ppid >65536)
            ppid = 1;


         // Get tclass
        if(tok3 != NULL)
		{
            strncpy(tclass, tok3, strlen(tok3));
		}


            // Get subject

           if(pid == 4)
            {
                strncpy(scontext, "system_u:object_r:system_t", strlen("system_u:object_r:system_t"));
                strncpy(temp_path, "System", strlen("System"));
                strncpy(comm, "System", strlen("System"));
            }
            else
            {
                if(tok1 != NULL)
                {
                    strncpy(temp, tok1, strlen(tok1));
                    BufferTreatment(temp,scontext, 1);
                    strncpy(comm, path, strlen(path));
                }

            }

            ZeroMemory( temp, 512);
            if(strcmp(tclass,"file") == 0 || strcmp(tclass,"dir") == 0 || strcmp(tclass,"driver") == 0 || strcmp(tclass,"process") == 0)
            {

                // Get object
                if(tok2 != NULL)
                {
                    strncpy(temp, tok2, strlen(tok2));
                    BufferTreatment(temp,tcontext, 2);
                    strncpy(temp_path_object, path, strlen(path));
                }
            }//end
            else
            {
                if(strcmp(tclass,"device") == 0)
                {
                    if(tok2 != NULL)
                    {
                        strncpy(tcontext, "system_u:object_r:device_t", strlen("system_u:object_r:device_t"));
                        strncpy(temp_path_object, tok2, strlen(tok2));
                    }
                }
                else
                {
                    if(strcmp(tclass,"registry") == 0)
                    {
                        if(tok2 != NULL)
                        {
                            strncpy(tcontext, "system_u:object_r:registry_t", strlen("system_u:object_r:registry_t"));
                            strncpy(temp_path_object, tok2, strlen(tok2));
                        }
                    }
                    else
                    {
                        if(tok2 != NULL)
                        {
                            strncpy(tcontext, "unknown_u:object_r:unknown_t", strlen("unknown_u:object_r:unknown_t"));
                            strncpy(temp_path_object, tok2, strlen(tok2));
                        }
                        else
                        {
                            strncpy(tcontext, "unknown_u:object_r:unknown_t", strlen("unknown_u:object_r:unknown_t"));
                            strncpy(temp_path_object, "NULL", strlen("NULL"));
                        }
                    }
                }
            }

            // Get Action
            if(tok5 != NULL)
                strncpy(action, tok5, strlen(tok5));

             if( (strcmp(temp_path_object, "%SystemDrive%\\") != 0 )  && ( strcmp(temp_path_object, "%SystemDrive%\\SvcIps") != 0 ))
             {
                    if(strcmp(tclass,"process")==0)
                    {
                        strncpy(temporaire,tcontext, strlen(tcontext));
                        type1 = strtok(temporaire, ":"); // Identite
                        type1 = strtok(NULL, ":"); // role
                        type1 = strtok(NULL, ":"); // type

                        ret =  rechercherTrans(type1, type2);
                        if(ret != 0)
                        {
                            ret = rechercherProcessType(type2);

                            if(ret ==1)
                            {
                                strncpy(temporaire1,scontext, strlen(scontext));
                                types = strtok(temporaire1, ":"); // Identite
                                types = strtok(NULL, ":"); // role
                                types = strtok(NULL, ":"); // type
                                ret = AutorisationPol(types, type1, tclass, action);
                            }
                        }

                    }
                    else
                    {
                        strncpy(temporaire,tcontext, strlen(tcontext));
                        type1 = strtok(temporaire, ":"); // Identite
                        type1 = strtok(NULL, ":"); // role
                        type1 = strtok(NULL, ":"); // type

                        strncpy(temporaire1,scontext, strlen(scontext));
                        types = strtok(temporaire1, ":"); // Identite
                        types = strtok(NULL, ":"); // role
                        types = strtok(NULL, ":"); // type

                         ret = AutorisationPol(types, type1, tclass, action);
                    }
             }

       if( (strcmp(temp_path_object, "%SystemDrive%\\") != 0 )  && ( strcmp(temp_path_object, "%SystemDrive%\\SvcIps") != 0 ))
       {

            time(&rawtime);
            timeinfo = localtime(&rawtime);
            trace = trace +1;

            switch(ret)
            {
                case 0:
                    sprintf(buffer,"%s %d %2d:%02d:%002d audit(%d:%i) avc:denied { %s} for pid=%i comm=\"%s\" ppid=%i path=\"%s\" scontext=%s tcontext=%s tclass=%s", month[timeinfo->tm_mon],timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min,timeinfo->tm_sec,rawtime, trace, action,pid, comm, ppid, temp_path_object, scontext, tcontext, tclass );
                    break;
                case 1:
                    sprintf(buffer,"%s %d %2d:%02d:%002d audit(%d:%i) avc:granted { %s} for pid=%i comm=\"%s\" ppid=%i path=\"%s\" scontext=%s tcontext=%s tclass=%s", month[timeinfo->tm_mon],timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min,timeinfo->tm_sec,rawtime, trace, action,pid, comm, ppid, temp_path_object, scontext, tcontext, tclass );
                    break;
                default:
                     sprintf(buffer,"%s %d %2d:%02d:%002d audit(%d:%i) avc:not implemented { %s} for pid=%i comm=\"%s\" ppid=%i path=\"%s\" scontext=%s tcontext=%s tclass=%s", month[timeinfo->tm_mon],timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min,timeinfo->tm_sec,rawtime, trace, action,pid, comm, ppid, temp_path_object, scontext, tcontext, tclass );

            }
            WriteToLog(buffer);
       }

//        if( (strcmp(temp_path_object, "%SystemDrive%\\") != 0 )  && ( strcmp(temp_path_object, "%SystemDrive%\\SvcIps") != 0 ))
//        {
//            if( tok1 != NULL && tok2 != NULL && tok3!=NULL && tok4 != NULL)
//            {
//                time(&rawtime);
//                timeinfo = localtime(&rawtime);
//                trace = trace +1;
//
//                sprintf(buffer,"%s %d %2d:%02d:%002d audit(%d:%i) avc:granted { %s} for pid=%i comm=\"%s\" ppid=%i path=\"%s\" scontext=%s tcontext=%s tclass=%s", month[timeinfo->tm_mon],timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min,timeinfo->tm_sec,rawtime, trace, action,pid, comm, ppid, temp_path_object, scontext, tcontext, tclass );
//                WriteToLog(buffer);
//            }
//        }

		resp = 1;
        memmove(&buffdriver[4],&resp,4);

        resp=0;
        memmove(&buffdriver[0],&resp,4);
	}
}

void SvcToPiga()
{
    //todo
}
