/**************************************
*  First project for IPK course       *
*  Client server app in C language    *
*  by Martin Kacmarcik - xkacma03     *
*  Faculty of Information Technology  *
*  University of Technology Brno      *
**************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/* function addToString add to msg the string given, based on topIndex */
void addToString (char *msg, char *string,int *topIndex)
{
    int lenghtOfString = strlen(string);
    //printf("lenght: %d top: %d\n", lenghtOfString, (*topIndex));
    (*topIndex)--;
    for(int i = 0; i < (lenghtOfString+1); i++)
    {
        if((*topIndex) != 127)
        {
            msg[(*topIndex)] = string[i];
            //printf("char %d: %c a top index: %d\n", i, string[i], (*topIndex));
            (*topIndex)++;
            msg[(*topIndex)] = '\0';
        }
        //printf("%d\n", i);
    }
}

//free alocated space
void garbage(char **array, int size,char *pvalue,char *hvalue){
    for(int i = 0; i < size; i++){
        free(array[i]);
    }
    if(pvalue != NULL)
        free(pvalue);
    if(hvalue != NULL)
        free(hvalue);
}

//return number of values for 'l' param
int getLenght(int argc, char**argv, int *skipl, int *skipu){
    int c;
    int arraySize = 0;
    int safeOptind = optind;
    opterr = 0;
    int togather = 0;
    //first run for array size
    while((c= getopt(argc, argv, "l:u:p:h:LUGNHS")) != -1){
        switch(c){
            case'l':
                optind--;
                togather++;
                if(togather == 2){
                    arraySize = 0;
                    (*skipu) = 1;
                }
                for( ; optind < argc && *argv[optind] != '-'; optind++){
                    arraySize++;
                }
                break;
            case'u':
                optind--;
                togather++;
                if(togather == 2){
                    arraySize = 0;
                    (*skipl) = 1;
                }
                for( ; optind < argc && *argv[optind] != '-'; optind++){
                    arraySize++;
                }
                break;
        }
    }
    optind = safeOptind;
    return arraySize;
}

//fill array with values of 'l' param
void fillArray(int argc,char **argv, char **array, int skipl, int skipu, int *argumentArray, char **pvalue, char **hvalue){
    int c;
    int poradi = 1;
    int arrayIndex = 0;
    int safeOptind = optind;
    char *newPointer = NULL;
    opterr = 0;
    while((c= getopt(argc, argv, "l:u:p:h:LUGNHS")) != -1){
        switch(c){
            case'l': // 0-l, 1-u, 2-L, 3-U, 4-G, 5-N, 6-H, 7-S, 8-H, 9-P
                argumentArray[0] = 1;
                if(skipl == 1)
                    break;
                optind--;
                for( ; optind < argc && *argv[optind] != '-'; optind++){
                    if((newPointer = malloc(strlen(argv[optind])+1)) == NULL)
                        printf("Problem s alokaci! Neco se deje se silou!\n");
                    strcpy(newPointer, argv[optind]);
                    array[arrayIndex] = newPointer;
                    arrayIndex++;
                }
                break;
            case'u':
                argumentArray[1] = 1;
                if(skipu == 1)
                    break;
                optind--;
                for( ; optind < argc && *argv[optind] != '-'; optind++){
                    if((newPointer = malloc(strlen(argv[optind])+1)) == NULL)
                        printf("Problem s alokaci! Neco se deje se silou!\n");
                    strcpy(newPointer, argv[optind]);
                    array[arrayIndex] = newPointer;
                    arrayIndex++;
                }
                break;
            case'p':
                argumentArray[9] = 1;
                char *newPointer1 = malloc(strlen(optarg)+1);
                strcpy(newPointer1, optarg);
                (*pvalue) = newPointer1;
                break;
            case'h':
                argumentArray[8] = 1;
                char *newPointer2 = malloc(strlen(optarg)+1);
                strcpy(newPointer2, optarg);
                (*hvalue) = newPointer2;
                break;
            case'L':
                argumentArray[2] = poradi;
                poradi++;
                break;
            case'U':
                argumentArray[3] = poradi;
                poradi++;
                break;
            case'G':
                argumentArray[4] = poradi;
                poradi++;
                break;
            case'N':
                argumentArray[5] = poradi;
                poradi++;
                break;
            case'H':
                argumentArray[6] = poradi;
                poradi++;
                break;
            case'S':
                argumentArray[7] = poradi;
                poradi++;
                break;
            case '?':
                if (optopt == 'l')
                  fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (optopt == 'u')
                  fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (optopt == 'h')
                  fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (optopt == 'p')
                  fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                  fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                  fprintf (stderr,
                           "Unknown option character `\\x%x'.\n",
                           optopt);
                exit(1);
        }
    }
    optind = safeOptind;
}

//check if the arguments given are valid (more -u ore non etc)
int argumentCheck(int *argumentArray){
    if(argumentArray[0] == 0 && argumentArray[1] == 0)
    {
        fprintf(stderr, "You need to give me atleast one of -u -l params! Try again.\n");
        return 1;
    }
    if(argumentArray[8] == 0 || argumentArray[9] == 0)
    {
        fprintf(stderr, "You need to give me -h AND -p param!!! Try again.\n");
        //return 1;
    }
    return 0;
}

void createGivenString(char *msg, char *value, int *argumentArray)
{
    int topIndex = 1;
    char buffer[2];
    memset(buffer, 0, 1);
    addToString(msg, value, &topIndex);
    addToString(msg, ":", &topIndex);
    for(int mak = 2; mak < 8; mak++)
    {
        sprintf(buffer, "%d", argumentArray[mak]);
        addToString(msg, buffer, &topIndex);
        addToString(msg, ":", &topIndex);
    }
}

int main (int argc, char **argv) {
    int argumentArray[] = {0,0,0,0,0,0,0,0,0,0}; // 0-l, 1-u, 2-L, 3-U, 4-G, 5-N, 6-H, 7-S, 8-H, 9-P
    int skipl = 0;
    int skipu = 0;
    char *pvalue = NULL;
    char *hvalue = NULL;
    int size = getLenght(argc, argv, &skipl, &skipu);
    char *valueArray[size];
    fillArray(argc, argv, valueArray, skipl, skipu, argumentArray, &pvalue, &hvalue);
    //argumentProcessing(argc, argv, argumentArray, &pvalue, &hvalue);
    if(argumentCheck(argumentArray) == 1){
        garbage(valueArray, size, pvalue, hvalue);
        return 1;
    }

    for(int mak = 0; mak < size; mak++)
    {
        //TESTING MSG START
        char msg[160];
        memset(msg, 0, 159);
        /*char recMsg[160];
        memset(recMsg, 0, 159);*/
        createGivenString(msg, valueArray[mak], argumentArray);

        int s, n;
        struct sockaddr_in sin; struct hostent *hptr;
        if ( (s = socket(PF_INET, SOCK_STREAM, 0 ) ) < 0) { /* create socket*/
            perror("error on socket");  /* socket error */
            return -1;
        }
        sin.sin_family = PF_INET;              /*set protocol family to Internet */
        sin.sin_port = htons(atoi(pvalue));  /* set port no. */
        if ( (hptr =  gethostbyname(hvalue)) == NULL){
            fprintf(stderr, "gethostname error: %s", argv[1]);
            return -1;
        }
        memcpy( &sin.sin_addr, hptr->h_addr, hptr->h_length);
        if (connect (s, (struct sockaddr *)&sin, sizeof(sin) ) < 0 ){
            perror("error on connect"); return -1;   /* connect error */
        }
        if ( write(s, msg, strlen(msg) +1) < 0 ) {  /* send message to server */
            perror("error on write");    return -1; /*  write error */
        }
        if ( ( n = read(s, msg, sizeof(msg) ) ) <0) {  /* read message from server */
            perror("error on read"); return -1; /*  read error */
        }
        printf ("%s\n", msg);  /* print message to screen */
        if (close(s) < 0) {
            perror("error on close");   /* close error */
        return -1;
        }
        //TESTING MSG END
    }

    garbage(valueArray, size, pvalue, hvalue);
    return 0;
}

    //just some random checks :))))
    /*for(int j = 0; j < 10; j++){
        if(argumentArray[j] == 1)
            printf("Na pozici %d je 1\n", j);
    }

    for(int i = 0; i < size; i++){
        printf("%s\n", valueArray[i]);
    }
    printf("sem tu a size je: %d\n", size);
    printf("%s %s P a H\n", pvalue, hvalue);*/
