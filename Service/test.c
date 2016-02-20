#include "main.h"


int BufferTreatment(char * string, char * context, int bol)
{
    ZeroMemory(path, 512);
    ZeroMemory(context, 50);
    TransformToVariable(string, path);
    GetSecurityContext(path, context, bol);

    return 1;
}

void TransformToVariable(char * thread, char * retour)
{
    char temp[512];
    int boolean =0;
    int i=0;
    if(strstr(thread,DeviceName) != NULL)
    {
        strncpy(retour,Names, strlen(Names)); // C:
        strcat(retour,strstr(thread,DeviceName)+strlen(DeviceName)+1 );
    }

    if(strstr(thread,"\\??\\") != NULL)
    {
        if(strstr(thread,Names) != NULL)
            strncpy(retour,strstr(thread,Names), strlen(strstr(thread, Names))); // C:
        else
            if(strstr(thread, SystemDrive))
                strncpy(retour,strstr(thread,SystemDrive), strlen(strstr(thread, SystemDrive))); // C:
            else
            {
                while(thread[i] != 58 && i< strlen(thread))
                {
                    if( (thread[i] >= 97 )&& ( thread[i] <= 122 ) )
                        thread[i] = thread[i] - 32;
                    i++;
                }
                if(strstr(thread,Names) != NULL)
                    strncpy(retour,strstr(thread,Names), strlen(strstr(thread, Names))); // C:
                else
                    if(strstr(thread, SystemDrive))
                        strncpy(retour,strstr(thread,SystemDrive), strlen(strstr(thread, SystemDrive)));
            }
    }

	if( strstr(thread, "SystemRoot") )
    {
        strcpy(temp, "%SystemRoot%");
        strcat(temp, strstr(thread, "\\SystemRoot")+strlen("\\SystemRoot"));
        ZeroMemory(retour, 512);
        strncpy(retour, temp, strlen(temp));
        boolean = 1;
        return;
    }

    if( strstr(retour, SystemRoot) )
    {
        strcpy(temp, "%SystemRoot%");
        strcat(temp, strstr(retour,SystemRoot)+strlen(SystemRoot));
        ZeroMemory(retour, 512);
        strncpy(retour, temp, strlen(temp));
        boolean = 1;
        return;
    }

    if( strstr(retour, "windows") )
    {
        strcpy(temp, "%SystemRoot%");
        strcat(temp, strstr(retour, "windows")+strlen("windows"));
        ZeroMemory(retour, 512);
        strncpy(retour, temp, strlen(temp));
        boolean = 1;
        return;
    }

    if( strstr(retour, ProgramFiles) )
    {
        strcpy(temp, "%ProgramFiles%");
        strcat(temp, strstr(retour,ProgramFiles)+strlen(ProgramFiles));
        ZeroMemory(retour, 512);
        strncpy(retour, temp, strlen(temp));
        boolean = 1;
        return;
    }

    if( strstr(retour, "program files") )
    {
        strcpy(temp, "%ProgramFiles%");
        strcat(temp, strstr(retour, "program files")+strlen("program files"));
        ZeroMemory(retour, 512);
        strncpy(retour, temp, strlen(temp));
        boolean = 1;
        return;
    }

    if( strstr(retour, SystemDrive) && boolean == 0)
    {
        strcpy(temp, "%SystemDrive%");
        strcat(temp, strstr(retour,SystemDrive)+strlen(SystemDrive));
        ZeroMemory(retour, 512);
        strncpy(retour, temp, strlen(temp));
        return;
    }

    strncpy(retour, thread, strlen(thread));
    return;
}


void GetSecurityContext(char * string, char * context, int bol)
{
    if(bol == 1)
        rechercherProcess(string, context);
    else
        rechercher(string, context);
    return;
}

int GetEnvironnement()
{
    char VolumeName[512];
    char System[20];
    char System_inst[4];
    HANDLE FindHandle;
    HMODULE hmod;
    DWORD  Error;
    int Index;
    DWORD  CharCount= 0;
    LPTSTR lpszVariable;
    LPTCH lpvEnv;
    char * tok;
    BOOL bol;

    ZeroMemory(SystemDrive, 3);
    ZeroMemory(SystemRoot, 12);
    ZeroMemory(ProgramFiles, 16);
    ZeroMemory(DeviceName, 512);
    ZeroMemory(Names, 10);

    GetSystemDirectory(System,sizeof(System) );
    strncpy(System_inst, System, 3);
    System_inst[3] = '\0';

    lpvEnv = GetEnvironmentStrings();

    if (lpvEnv == NULL)
    {
        return 0;
    }

    lpszVariable = (LPTSTR) lpvEnv;

    while (*lpszVariable)
    {
        if(strstr(lpszVariable,"SystemDrive"))
        {
            tok = strtok(lpszVariable,"=");
            tok = strtok(NULL,"=");
            strncpy(SystemDrive, tok, 2);
            SystemDrive[2]='\0';
            tok = NULL;
        }
        if(strstr(lpszVariable,"SystemRoot"))
        {
            tok = strtok(lpszVariable,"=");
            tok = strtok(NULL,"=");
            strncpy(SystemRoot, tok, 10);
            SystemRoot[10]='\0';
            tok = NULL;
        }
        if(strstr(lpszVariable,"ProgramFiles") && !strstr(lpszVariable, "Common") )
        {

            tok = strtok(lpszVariable,"=");
            tok = strtok(NULL,"=");
            strncpy(ProgramFiles, tok, 16);
            ProgramFiles[16]='\0';
            tok = NULL;
        }

        lpszVariable += lstrlen(lpszVariable) + 1;
    }
    FreeEnvironmentStrings(lpvEnv);


    hmod = LoadLibrary("Kernel32.dll");

    if(hmod)
    {
        FindFirstVolume = ( HANDLE (WINAPI *) (LPTSTR,DWORD) ) GetProcAddress(hmod, "FindFirstVolumeA");
        FindNextVolume = ( BOOL (WINAPI *) (HANDLE, LPTSTR,DWORD) ) GetProcAddress(hmod, "FindNextVolumeA");
        GetVolumePathNamesForVolumeNameA = ( BOOL ( WINAPI *) (LPCSTR,LPCSTR,DWORD,PDWORD) ) GetProcAddress(hmod,"GetVolumePathNamesForVolumeNameA") ;
        FindVolumeClose = (BOOL (WINAPI *) (HANDLE) ) GetProcAddress(hmod,"FindVolumeClose");
        FreeLibrary(hmod);
    }
    else
    {
        return 0;
    }

    FindHandle = FindFirstVolume(VolumeName,sizeof(VolumeName));

    if (FindHandle == INVALID_HANDLE_VALUE)
    {
        Error = GetLastError();
        return 0;
    }

    while(1)
    {
        Index = strlen(VolumeName)-1;
        if (VolumeName[0] != '\\' || VolumeName[1] != '\\' || VolumeName[2]  != '?' || VolumeName[3] != '\\' || VolumeName[Index] != '\\')
            {
                Error = ERROR_BAD_PATHNAME;
            }

        VolumeName[Index] = '\0';

        CharCount = QueryDosDeviceA(&VolumeName[4], DeviceName, sizeof(DeviceName));

        VolumeName[Index] = '\\';
        if(CharCount == 0)
            return 0;

       CharCount = 512;
       bol =  GetVolumePathNamesForVolumeNameA( &VolumeName[0], Names, CharCount, &CharCount );

       if(strcmp(Names, System_inst) == 0 )
        break;

       bol = FindNextVolume(FindHandle,VolumeName,sizeof(VolumeName));

       if(!bol)
        break;
    }

    FindVolumeClose(FindHandle);
    return 1;
}
