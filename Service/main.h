#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include "regex.h"

//#define  __MSVCRT_VERSION__ 0x0601

typedef struct List{
	char regexp[200];
	char context[50];
	struct List * suivant;
}List ;


typedef struct Trans{
	char trans1[50];
	char trans2[50];
	struct Trans * suivant;
}Trans ;

typedef struct Politique{
    char action[20];
    char sujet[50];
    char objet[50];
    char tclass[50];
    char droit[100];
    struct Politique * suivant;
}Politique;

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;
HANDLE device;
char buffdriver[1024];
SC_HANDLE manager;

void  ServiceMain(int argc, char** argv);
void  ControlHandler(DWORD request);
int InitService();
void ListenToDriver();
char path[512];
int RemplirListe();
int RemplirListeProcess();
int remplirstruct();
int rechercherTrans(char * type, char * type1);
int rechercherProcessType(char * string);
void rechercherProcess(char * string, char * context);
List* AddToListProcess(List* liste, char *premier, char *deuxieme);
void rechercher(char * string, char * context);
List * AddToList(List *liste, char *premier, char *deuxieme);
Trans* AddToListTrans(Trans* liste, char *premier, char *deuxieme);
int MatchExp (char *str_request ,char *str_regex );
int WriteToLog(char* str);

Politique* AddToListPol(Politique* liste, char * action, char * sujet, char * objet, char * tclass, char * droit);
int RemplirPolitique();
int AutorisationPol(char * stype, char * otype, char * classo, char * droitd);


char Names[10];// C:
char SystemDrive[3]; // %SystemDrive% <==> C:
char SystemRoot[12]; // %SystemRoot% <==> C:\Windows
char ProgramFiles[16]; // %ProgramFiles% <==> C:\Program Files
char DeviceName[512]; // \Device\HarddiskVolume2

//Déclaration des fonctions
int BufferTreatment(char * string, char * context, int bol);
int GetEnvironnement();
void  TransformToVariable(char * thread, char * retour);
void GetSecurityContext(char * string, char * context, int bol);

// Fonctions de Kernel32
HANDLE (WINAPI * FindFirstVolume) (LPTSTR, DWORD);
BOOL (WINAPI * FindNextVolume) (HANDLE,LPTSTR, DWORD);
BOOL (WINAPI * FindVolumeClose) (HANDLE);
BOOL ( WINAPI * GetVolumePathNamesForVolumeNameA) (LPCSTR,LPCSTR,DWORD,PDWORD);
