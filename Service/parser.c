/**
 * fichier parsant le file_context avec les expressions et les stockant sous la forme chaine <==> regexp
 *
 **/

#include "main.h"


List * malist = NULL;

int RemplirListe()
{
	FILE *file;
	char regexp[200];
	char regexp_s[200];
	char context[50];
	char buffer[2048];
	char * temp;
	int i =0, j=0, count=0;

	malist = (List *)malloc(sizeof(struct List));
	if(malist == NULL)
		return 3;
	file = fopen("\\SvcIps\\file_context", "rb");
	if(file == NULL)
		return 2;
    ZeroMemory(buffer,2048);
	fread(buffer, 2048, 1, file);
	fclose(file);
	temp = strstr(buffer,"%");
	while(temp != NULL)
	{
        ZeroMemory(regexp,200);
        ZeroMemory(regexp_s,200);
        ZeroMemory(context,50);
        i=0;
		while(temp[i] != ' ' && i < 200)
		{
			regexp[i]= temp[i];
			i++;
		}
		regexp[i+1] = '\0';

		if(count ==0)
		{
            i = 0;
            for(j=0; j< strlen(regexp);j++)
            {
                regexp_s[j] =  regexp[i];
                i++;
            }
            count=1;
		}
        else
        {
            i=1;
            for(j=0; j< strlen(regexp);j++)
            {
                regexp_s[j] =  regexp[i];
                i++;
            }
            i=i-1;
        }

		temp = temp +2;
		j=0;
		while(temp[i] != '\n' && j < 50)
		{
			context[j]= temp[i];
			i++;
			j++;
		}
        context[j] = '\0';
		malist = AddToList(malist, regexp_s, context);
		temp = strstr(temp+2,"\n");

	}


	return 1;
}

void rechercher(char * string, char * context)
{
	List *list_temp = NULL;
	int retour = 0;
	list_temp = (List*)malloc(sizeof(struct List));
	list_temp = malist;

	while(list_temp->suivant != NULL && retour == 0)
	{
		retour = MatchExp(string,list_temp->regexp);
		if( retour == 1)
		{
            strncpy(context, list_temp->context, strlen(list_temp->context));
            break;
		}
		list_temp = list_temp->suivant;

	}
	if(retour ==0)
        strncpy(context,"unknown_u:object_r:unknown_t", strlen("unknown_u:object_r:unknown_t"));
    list_temp = NULL;
}

List* AddToList(List* liste, char *premier, char *deuxieme)
{
	List* newlist = NULL;
	List* temp = NULL;
	newlist = (List *)malloc(sizeof(struct List));
	ZeroMemory(newlist->regexp,200);
	ZeroMemory(newlist->context,50);
	strncpy(newlist->regexp, premier, strlen(premier));
	strncpy(newlist->context, deuxieme, strlen(deuxieme));
	newlist->suivant = NULL;

	if(liste == NULL)
	{
		return newlist;
	}
	else
	{
		temp = (List *)malloc(sizeof(struct List));
		temp = liste;
		while( temp->suivant != NULL )
		{
			temp = temp->suivant;
		}
		temp->suivant = newlist;
		return liste;
	}
}
