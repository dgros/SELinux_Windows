#include "main.h"

Politique * pol = NULL;

int RemplirPolitique()
{
    char buf[2010];

    FILE * file;
    char action[20];
    char sujet[50];
    char objet[50];
    char tclass[50];
    char droit[100];
    int i= 0,j=0;
    char buffer[2048];

    pol = (Politique *) malloc(sizeof(struct Politique));
    if(pol == NULL)
        return -1;

	file = fopen("\\SvcIps\\explorer.te", "rb");
	if(file == NULL)
		return 2;

    ZeroMemory(buffer,2048);
	fread(buffer, 2048, 1, file);
	fclose(file);

    while(i < strlen(buffer))
    {
        ZeroMemory(action, 20);
        ZeroMemory(sujet, 50);
        ZeroMemory(objet, 50);
        ZeroMemory(tclass, 50);
        ZeroMemory(droit, 100);
        j=0;
        //get action
        while(buffer[i] != ' ')
        {
            action[j] = buffer[i];
            j++;
            i++;
        }
        j=0;
        while(buffer[i] == ' ' ) i++;
        //get sujet
        while(buffer[i] != ' ')
        {
            sujet[j] = buffer[i];
            j++;
            i++;
        }
        j=0;
        while(buffer[i] == ' ' ) i++;
        //get objet
        while(buffer[i] != ':')
        {
            objet[j] = buffer[i];
            j++;
            i++;
        }
        j=0;
        i=i+1;
        //get class
        while(buffer[i] != ' ')
        {
            tclass[j] = buffer[i];
            j++;
            i++;
        }
        while(buffer[i] == ' ' || buffer[i] == '{') i++;
        j=0;
        while(buffer[i] != '}')
        {
            droit[j] = buffer[i];
            j++;
            i++;
        }
        droit[j-1] = ' ';
        droit[j] = '\0';
        i = i+2;
        pol = AddToListPol( pol, action, sujet, objet, tclass, droit);
    }

    ZeroMemory(action, 20);
    ZeroMemory(sujet, 50);
    ZeroMemory(objet, 50);
    ZeroMemory(tclass, 50);
    ZeroMemory(droit, 100);
    pol = AddToListPol( pol, action, sujet, objet, tclass, droit);

    return 1;
}

int AutorisationPol(char * stype, char * otype, char * classo, char * droitd)
{
	Politique *list_temp = NULL;
	list_temp = (Politique*)malloc(sizeof(struct Politique));
	list_temp = pol;

	while(list_temp->suivant != NULL)
	{
		if(strcmp(list_temp->sujet, stype)==0)
		{
            if(strcmp(list_temp->objet, otype)==0)
            {
                if(strcmp(list_temp->tclass, classo )==0)
                {
                    if(strcmp(list_temp->droit, droitd )==0)
                    {
                        if(strcmp(list_temp->action, "allow" )==0)
                            return 1;
                    }
                }
            }
		}

        list_temp = list_temp->suivant;
	}
	 list_temp = NULL;
	 return 0;
}


Politique* AddToListPol(Politique* liste, char * action, char * sujet, char * objet, char * tclass, char * droit)
{
	Politique* newlist = NULL;
	Politique* temp = NULL;
	newlist = (Politique *)malloc(sizeof(struct Politique));

	ZeroMemory(newlist->action, 20);
    ZeroMemory(newlist->sujet, 50);
    ZeroMemory(newlist->objet, 50);
    ZeroMemory(newlist->tclass, 50);
    ZeroMemory(newlist->droit, 100);

	strncpy(newlist->action, action, strlen(action));
    strncpy(newlist->sujet, sujet, strlen(sujet));
    strncpy(newlist->objet, objet, strlen(objet));
    strncpy(newlist->tclass, tclass, strlen(tclass));
    strncpy(newlist->droit, droit, strlen(droit));

	newlist->suivant = NULL;

	if(liste == NULL)
	{
		return newlist;
	}
	else
	{
		temp = (Politique *)malloc(sizeof(struct Politique));
		temp = liste;
		while( temp->suivant != NULL )
		{
			temp = temp->suivant;
		}
		temp->suivant = newlist;
		return liste;
	}
}
