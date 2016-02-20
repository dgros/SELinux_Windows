#include "main.h"

Trans * chaine = NULL;

int remplirstruct()
{
    FILE *file;
	char chaine1[50];
	char chaine2[50];
	char buffer[2048];
	char buf[100];
	int cout = 0;
	int i=0, j=0, k=0
	;

    chaine = (Trans *) malloc(sizeof(struct Trans));
    if(chaine == NULL)
        return -1;

	file = fopen("\\SvcIps\\type_transition", "rb");
	if(file == NULL)
		return 2;

    ZeroMemory(buffer,2048);
    ZeroMemory(buf,100);
    ZeroMemory(chaine1,50);
    ZeroMemory(chaine2,50);
	fread(buffer, 2048, 1, file);
	fclose(file);


	while(i < strlen(buffer))
	{
        ZeroMemory(buf,100);
        ZeroMemory(chaine1,50);
        ZeroMemory(chaine2,50);
	    j=0;
	    k=0;

        while(buffer[i] != ';')
        {
            buf[j]=buffer[i];
            i++;
            j++;
        }

        buf[j+1] = ';';
        j=0;

        while(cout < 2)
        {
            if(buf[j] == ' ')
                cout = cout +1;
            j++;
        }

        while(buf[j] == ' ') j++;
        while(buf[j] != ' ')
        {
            chaine1[k] = buf[j];
            j++;
            k++;
        }
        chaine1[k] = '\0';
        while(buf[j] == ' ') j++;
        k=0;
        while(buf[j] != ';')
        {
            chaine2[k] = buf[j];
            j++;
            k++;
        }
        chaine2[k] = '\0';
        chaine = AddToListTrans(chaine, chaine1, chaine2);
        i= i+1 +1;
        cout = 0;
	}
	ZeroMemory(chaine1,50);
    ZeroMemory(chaine2,50);

    chaine = AddToListTrans(chaine, chaine1, chaine2);
    return 1;
}

int rechercherTrans(char * type, char * type1)
{
	Trans *list_temp = NULL;
	list_temp = (Trans*)malloc(sizeof(struct Trans));
	list_temp = chaine;

	while(list_temp->suivant != NULL)
	{
		if(strcmp(list_temp->trans1, type)==0)
		{
		    strncpy(type1, list_temp->trans2, strlen(list_temp->trans2));
		    return 1;
		}

        list_temp = list_temp->suivant;
	}
	 list_temp = NULL;
	 return 0;
}


Trans* AddToListTrans(Trans* liste, char *premier, char *deuxieme)
{
	Trans* newlist = NULL;
	Trans* temp = NULL;
	newlist = (Trans *)malloc(sizeof(struct Trans));
	ZeroMemory(newlist->trans1,50);
	ZeroMemory(newlist->trans2,50);
	strncpy(newlist->trans1, premier, strlen(premier));
	strncpy(newlist->trans2, deuxieme, strlen(deuxieme));
	newlist->suivant = NULL;

	if(liste == NULL)
	{
		return newlist;
	}
	else
	{
		temp = (Trans *)malloc(sizeof(struct Trans));
		temp = liste;
		while( temp->suivant != NULL )
		{
			temp = temp->suivant;
		}
		temp->suivant = newlist;
		return liste;
	}
}
