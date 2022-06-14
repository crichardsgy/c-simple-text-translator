#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define max 240

struct dictionary
{
 char english[max];
 char spanish[max];
};

void clearscreen();
int search(struct dictionary *dicttable, char word[],int size);

int compare (const void * a, const void *b)
{
    struct dictionary * ia=(struct dictionary*)a;
    struct dictionary * ib=(struct dictionary*)b;
    return (strcmp(ia->english,ib->english));
}

int main ()
{
    FILE *dictptr;
    char buffer[max],
        progstatus,
        englishinput[max],
        engsplitwords[max][max],
        translatedoutput[max];
    int i=0,
        j=0,
        dictrows=0,
        key=0,
        word=0;

    //opens dictionary file
    if ((dictptr = fopen("dictionary.txt", "r")) == NULL)
    {
        printf("Error Opening File");
        exit(1);
    }

    printf("Reading Data From File, Please Wait\n");
    
    while(fgets(buffer,max,dictptr))
    {
        dictrows++;
    }
    dictrows--;

    rewind(dictptr);

    //initializes structure array to the amount of rows in the dictionary file
    struct dictionary *dicttable = malloc(dictrows * sizeof(struct dictionary));

    //copies the data stored in the dictionary file to the structure
    for (i=0;i<=dictrows;i++)
    {
        fgets(buffer, max, dictptr);
        buffer[strcspn(buffer, "\n")] = 0; //removes the new lines that are automatically retrieved with fgets()
        char *english = strtok(buffer, ",");
        char *spanish = strtok(NULL, ",");

        //checks if the data is complete (i.e checks if there is corresponding translation for each word)
        if (english != NULL && spanish != NULL)
        {
            strcpy(dicttable[i].english, english);
            strcpy(dicttable[i].spanish, spanish);
        }
        else
        {
            strcpy(dicttable[i].english, "\0");
            strcpy(dicttable[i].spanish, "\0");
        }
    }

    fclose(dictptr);

    printf("Sorting %d Dictionary Entries In Alphabetical Order To Speed Up Retrieval (This Only Occurs Once At Program Launch)\n",dictrows);
    qsort(dicttable,dictrows, sizeof(struct dictionary), compare);
    clearscreen();

    while(1)
    {
        //accepts user input
        printf("Enter Text To Translate To Spanish (max. 240 characters)\n");
        fgets(buffer,max,stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        strncat(englishinput," ",max);
        strncat(englishinput,buffer,max);
    
        //splits string into individual array entries for each word
        for (i=0; i<=strlen(englishinput);i++)
        {
            if (englishinput[i] == ' ' || englishinput[i] == '\0')
            {
                engsplitwords[word][j]='\0';
                word++;
                j=0;
            }
            else
            {
                engsplitwords[word][j]=tolower(englishinput[i]); //converts each char to lowercase and adds it to the split words array
                j++;
            }
        }
        i = j = 0;

        //compares each word to the entire dictionary
        //loops through each word in the split user input array
        for (i=0;i<=word;i++)
        {
            if ((key = search(dicttable,engsplitwords[i],dictrows)) != -1)
            {
                strncat(translatedoutput," ",max); //adds a space between the words
                strncat(translatedoutput,dicttable[key].spanish,max);
            }
            else 
            {
                strncat(translatedoutput," ",max);
                strncat(translatedoutput,engsplitwords[i],max); //concatenates the inputted text if no match is found
            } 
        }
        
        while(1)
        {
            printf("\nSpanish:%s\n\n",translatedoutput);

            printf("Would You Like To Translate Another Sentence? (y/n)\n>>");
            scanf("%c",&progstatus);
            if(progstatus=='y')
            {
                clearscreen();
                word = 0;
                memset(buffer, 0, sizeof(buffer));
                memset(englishinput, 0, sizeof(englishinput));
                memset(engsplitwords, 0, sizeof(engsplitwords));
                memset(translatedoutput, 0, sizeof(translatedoutput));
                fflush(stdin);
                break;
            }
            else if (progstatus=='n')
            {
                free(dicttable);
                exit(1);
            }
            else
            {
                clearscreen();
                fflush(stdin);
                printf("Incorrect Input, Please Try Again\n");
            }
        }
    }
}

int search(struct dictionary *dicttable, char word[],int size)
{
    int low = 0;
    int high = size - 1;
    int middle=0;

    while( low <= high )
    {
        middle = (low + high) / 2;
        if (strcmp(dicttable[middle].english, word) == 0)
        {
            return middle;
        }
        else if (strcmp(dicttable[middle].english, word) < 0)
        {
            low=middle+1;
        }
        else
        {
            high=middle-1;
        }
    
    }
    
    return -1;
}

void clearscreen()
{
    #ifdef _WIN32 
        system("cls");
    #elif __unix__
        system("clear");
	#else
	    system("cls");
        system("clear");
	#endif
}
