/**************************************
*  First project for ISA course       *
*            SMTP server              *
*  by Martin Kacmarcik - xkacma03     *
*  Faculty of Information Technology  *
*  University of Technology Brno      *
**************************************/

#include "smtpklient.h"
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

//Global declaration of socket descriptor, params from main etc, needed for signals to close connection if needed
int s, n;
char **arrayOfEmailsGlobal;
int arrayOfEmailsPointerGlobal;
char *msgGlobal;
int argumentArray[4];
char *ivalueGlobal;
char *avalueGlobal;
int numberOfLinesGlobal;
char **charPointersGlobal;
FILE *inputFile;
int wasMemoryCleaned = 0;
char *sentMessage;
int inDataSection = 0;

/* signal handler - will catch and handle signals */
void sig_handler(int signo)
{
    //Handle SIGINIT signal
    if (signo == SIGINT)
    {
        fprintf(stderr, "SIGKILL RECIEVED, CLEANING NOW\n");
        signalCleaning();
        exit(99);
    }
    //Handle SIGTERM
    else if(signo == SIGTERM)
    {
        fprintf(stderr, "SIGTERM RECIEVED, CLEANING NOW\n");
        signalCleaning();
        exit(99);
    }
    //HANDLE SIGQUIT
    else if(signo == SIGQUIT)
    {
        fprintf(stderr, "SIGQUIT RECIEVED, CLEANING NOW\n");
        signalCleaning();
        exit(99);
    }
}

/* Function that will clean memory after signal handler calls it */
void signalCleaning()
{
    /* Need to do some declaration first ti mirror variables from other functions */
    //check if we ended in data section - need to quit data section first
    if(inDataSection == 1)
    {
        /* SEND THE ACTUAL MESSAGE */
        if ( write(s, msgGlobal, strlen(msgGlobal)) < 0)
        {
            perror("error on write");
        }
        /* end the transaction with dot character */
        free(sentMessage);
        sentMessage = NULL;
        sentMessage = malloc(sizeof("\r\n.\r\n"));
        strcpy(sentMessage, "\r\n.\r\n");
        if ( write(s, sentMessage, strlen(sentMessage)) < 0)
        {
            perror("error on write");
        }
        free(sentMessage);
        sentMessage = NULL;
    }
    char returnedMsg[256];
    char *quitMsg;
    quitMsg = malloc(sizeof("QUIT\r\n"));
    strcpy(quitMsg, "QUIT\r\n");
    //SEND QUIT to end connection
    if ( write(s, quitMsg, strlen(quitMsg)) < 0)
    {
        perror("error on write");
    }
    free(quitMsg);
    //Read response from server - this will ensure connection ends successfully
    memset(returnedMsg, 0, 255);
    if((n = read(s, returnedMsg, sizeof(returnedMsg))) < 0)
    {
        perror("error on read");
    }
    //Close socket
    close(s);
    /* cleaning memory from main*/
    if(argumentArray[0])
        free(avalueGlobal);
    if(argumentArray[2])
        free(ivalueGlobal);
    fclose(inputFile);
    for (int x = 0; x < numberOfLinesGlobal; x++)
    {
        free(charPointersGlobal[x]);
    }
    //Check if emails were cleared - if not do it
    if(wasMemoryCleaned == 0)
    {
        freeEmails(arrayOfEmailsGlobal, arrayOfEmailsPointerGlobal, msgGlobal);
    }
    //Check if sentMessage was cleared - if not do it
    if(sentMessage != NULL)
        free(sentMessage);

}

/* Print program help */
void printHelp()
{
    printf("Help for simple SMTP client by Martin Kacmarcik - xkacma03\nFor FIT VUTBR - ISA course 2015\nusage: ./smtpklient [-h] [-a IP] [-p port] -i file [-w seconds]\nWhere: H will print help\nA param is volatile address param, 127.0.0.1 is default\nP is port number, volatile, 25 is default\nI is mandratory and leads to config file with rcpt and message\nW is volatile param that will keep connection alive x seconds where x is subparam of W\n");
}

/* Process arguments and fill argument array and variables */
int argumentProcessing(int argc, char **argv, int *argumentArray, char **avalue, int *pvalue, char **ivalue, int *wvalue){
    //using getops to extract arguments, see getopt man page for mor info
    int c;
    int digitNumber;
    opterr = 0;
	while((c = getopt(argc, argv, "ha:p:i:w:")) != -1)
	{
		switch (c)
		{
			case 'a':
                argumentArray[0] = 1;
                (*avalue) = malloc(strlen(optarg)+1);
                strcpy(*avalue, optarg);
                break;
            case 'p':
                argumentArray[1] = 1;
                *pvalue = atoi(optarg);
                //check if port is a number, if not free values and return error
                digitNumber = floor (log10 (abs (*pvalue))) + 1;
                if((*pvalue == 0 && optarg[0] != '0') || digitNumber != strlen(optarg)){
                    fprintf(stderr, "Port is not a number!!!\n");
                    return 60;
                }

                break;
            case 'i':
                argumentArray[2] = 1;
                //musis alokovat pamet
                (*ivalue) = malloc(strlen(optarg)+1);
                strcpy(*ivalue, optarg);
                break;
            case 'w':
                argumentArray[3] = 1;
                *wvalue = atoi(optarg);
                //check if waiting seconds are a number, if not free values and return error
                digitNumber = floor (log10 (abs (*wvalue))) + 1;
                if((*wvalue == 0 && optarg[3] != '0') || digitNumber != strlen(optarg)){
                    fprintf(stderr, "Port is not a number!!!\n");
                    return 60;
                }
                break;
            case 'h':
                printHelp();
                exit(0);
            case '?':
                return 60;
		}

    }
    return 0;
}
/* Function that return lenght of the file string - USING FINAL STATE MACHINE, also checks file syntax */
int fileProcessing(FILE *inputFile, int *counter)
{

    int c = 0;
    konecnyAutomat TSM = S_START;
    /*processing the file */
    while(1)
    {
        //printf("Znak je: %c a counter je: %d\n", c, *counter);
        *counter = *counter + 1;
        if(TSM != S_END)
            c = fgetc(inputFile);

        if(TSM == S_START)
        {
            if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '@' || c == '.')
            {
                TSM = S_EMAIL;
            }
            else if(c == EOF)
            {
                TSM = S_END;
            }
            else
            {
                fprintf(stderr, "1 Error, bad format of file, char is: %d\n", c);
                return 1;
                break;
            }
        }
        else if(TSM == S_EMAIL)
        {
            if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '@' || c == '.')
            {
                TSM = S_EMAIL;
            }
            else if(c == ',')
                TSM = S_START;
            else if(c == ' ')
            {
                TSM = S_MSGSTART;
            }
            else
            {
                fprintf(stderr, "2 Error, bad format of file, char is: %d\n", c);
                return 1;
                break;
            }
        }
        else if(TSM == S_MSGSTART)
        {
            if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '@' || c == '.')
            {
                TSM = S_MSG;
            }
            else if(c == '\n')
            {
                TSM = S_START;
            }
            else
            {
                fprintf(stderr, "3 Error, bad format of file, char is: %d\n", c);
                return 1;
                break;
            }
        }
        else if(TSM == S_MSG)
        {
            if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '@' || c == '.' || c == ' ')
            {
                TSM = S_MSG;
            }
            else if(c == '\n')
            {
                TSM = S_START;
            }
            else if(c == EOF)
            {
                TSM = S_END;
            }
            else
            {
                fprintf(stderr, "4 Error, bad format of file, char is: %d\n", c);
                return 1;
                break;
            }
        }
        else if(TSM == S_END)
        {
            //something
            break;
        }

    }
    return 0;
}

/*this function will fill inputString with content of the inputFile */
int getFileContent(FILE *inputFile, int counter, char *inputString)
{
    /* reset FILE counter */
    int maxCounter = 0;
    //Reset pointer in the file
    rewind(inputFile);
    int i, c;
    //Byte by byte store the content of the file :)
    for (int i = 0; i < counter; i++)
    {
        c = fgetc(inputFile);
        if(c == EOF)
        {
            maxCounter = i;
            break;
        }
        inputString[i] = c;
        //printf("%d: char je: %c, inputString: %c\n", i, c, inputString[i]);
    }
    inputString[maxCounter-1] = '\0' ;
    //printf("%s\nKONEC a max counter je: %d\n", inputString, maxCounter);
    return maxCounter;
}

/* this function will count number of lines in string -- basicly count number of \n chars*/
int countLines(char *fileString, int counter)
{
    int i, numberOfLines = 0;
    for (int i = 0; i < counter; i++)
    {
        if(fileString[i] == '\n')
            numberOfLines++;
    }
    return numberOfLines+1;
}

/* fill array of lenghts with lenght of each line - basicly count all chars and reset when new line char comes */
void getLenghtOfLines(char *fileString, int *charPointersLenght)
{
    int i = 0;
    int counter = 0;
    int arrayPointer = 0;
    while(1)
    {
        if(fileString[i] == '\0'){
            charPointersLenght[arrayPointer] = counter;
            break;
        }
        if(fileString[i] != '\n')
        {
            counter++;
        }
        else
        {
            charPointersLenght[arrayPointer] = counter;
            counter = 0;
            arrayPointer++;
        }
        i++;
    }
}

/* function will fill each string with it's data - parse lines into strings basicly */
void fillStringsWithData(char *fileString, char **charPointers)
{
    /* pointer that points on acutal position in fileString */
    int fileStringIntPointer = 0;
    /* pointer that points on acutal position in charPointers */
    int charPointersPointer = 0;
    /* will be used as pointer into the current char of the current used string from charPointers */
    int i = 0;
    while(1)
    {
        //printf("%d: znak je: %c\n", fileStringIntPointer, fileString[fileStringIntPointer]);
        if(fileString[fileStringIntPointer] == '\n')
        {
            charPointers[charPointersPointer][i] = '\0';
            i = 0;
            charPointersPointer++;
            fileStringIntPointer++;
            //printf("String je: %s\n", charPointers[charPointersPointer-1]);
            continue;
        }
        else if(fileString[fileStringIntPointer] == '\0')
        {
            charPointers[charPointersPointer][i] = '\0';
            //printf("String je: %s\n", charPointers[charPointersPointer]);
            break;
        }
        charPointers[charPointersPointer][i] = fileString[fileStringIntPointer];

        i++;
        fileStringIntPointer++;
    }
}

/* function that will parse emails, start socket connection with server and send messages
    sendEmailControl(charPointers, numberOfLines, avalue, pvalue, ivalue, wvalue);
*/
int sendEmailControl(char **arrayOfStrings, int numberOfStrings, char *avalue, int pvalue, char *ivalue, int wvalue, int ipMode)
{
    //IPv4 declarations
    struct sockaddr_in sin;
    struct hostent *hptr;
    struct in_addr addr;
    //IPv6 declarations
    struct sockaddr_in6 sin6;
    struct hostent *hptr6;
    struct in6_addr addr6;
    //Declare char array where returned messages will be stored
    char returnedMsg[256];
    bzero(returnedMsg, 256);
    sentMessage = NULL;
    //ipv4 mode commands
    if(ipMode == 4)
    {
        //create socket in socket descriptor s
        if( (s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("error on socket");
            return 1;
        }
        //set proper sturcture stuff
        sin.sin_family = PF_INET;
        sin.sin_port = htons(pvalue);
        inet_aton(avalue, &addr);
        //get the proper form of address by gethostbyaddr
        if( (hptr = gethostbyaddr(&addr, sizeof(addr), PF_INET)) == NULL)
        {
            fprintf(stderr, "get host by addr error\n");
            return 1;
        }
        //set memory right
        memcpy(&sin.sin_addr, hptr->h_addr, hptr->h_length);
        //connect to the server
        if(connect (s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        {
            perror("error on connect");
            return 1;
        }
    }
    //ipv6 mode commands
    else if(ipMode == 6)
    {
        //create socket in socket descriptor s
        if( (s = socket(PF_INET6, SOCK_STREAM, 0)) < 0)
        {
            perror("error on socket");
            return 1;
        }
        //set proper sturcture stuff
        bzero((char *) &sin6, sizeof(sin6));
        sin6.sin6_flowinfo = 0;
        sin6.sin6_family = PF_INET6;
        sin6.sin6_port = htons(pvalue);
        sin6.sin6_addr = in6addr_any;
        inet_pton(AF_INET6, avalue, &addr6);
        //get the proper form of address by gethostbyaddr
        if( (hptr6 = gethostbyaddr(&addr6, sizeof(addr6), PF_INET6)) == NULL)
        {
            fprintf(stderr, "get host by addr error\n");
            return 1;
        }
        //set memory to be right
        memcpy(&sin6.sin6_addr, hptr6->h_addr, hptr6->h_length);
        //connect to the server
        if(connect (s, (struct sockaddr *)&sin6, sizeof(sin6)) < 0)
        {
            perror("error on connect");
            return 1;
        }
    }
    //Read response from server - now it doesn't matter if its ipv6 or ipv4
    memset(returnedMsg, 0, 255);
    if((n = read(s, returnedMsg, sizeof(returnedMsg))) < 0)
    {
        perror("error on read");
    }
    /* send EHLO message */
    // assign EHLO message to string
    sentMessage = malloc(sizeof("EHLO isa.local\n"));
    strcpy(sentMessage, "EHLO isa.local\n");

    if ( write(s, sentMessage, strlen(sentMessage)) < 0)
    {
        perror("error on write");
        return 1;
    }
    free(sentMessage);
    sentMessage = NULL;
    //Read response from server
    memset(returnedMsg, 0, 255);
    if((n = read(s, returnedMsg, sizeof(returnedMsg))) < 0)
    {
        perror("error on read");
    }

    /* in this cycle, little main function will be taking place                                *
     * for each string I will first parse it, get emails and message, then send it using SMTP  *
     * this will be done for each lane of the file - stored in arrayOfStrings                  */
    for(int i = 0; i < numberOfStrings; i++)
    {
        /* ------------------ */
        /* PARSING DUTY ENDS */
        /* ------------------ */
        //declare array where I can store emails and pointer that show size of the array
        char *arrayOfEmails[100];
        int arrayOfEmailsPointer = 0;
        //in the current string now I have one line
        char *currentString = arrayOfStrings[i];
        char *token  = NULL;
        char *token2  = NULL;
        //store message
        char *msg = NULL;
        /* main for each line start */
        //parse string based on delimiter
        while ((token = strsep(&currentString, ",")))
        {
            if(currentString == NULL)
            {
                //parse message and last email
                token2 = strsep(&token, " ");
                currentString = token;
                //add email into string array of emails
                arrayOfEmails[arrayOfEmailsPointer] = malloc(sizeof(char)*strlen(token2)+1);
                strcpy(arrayOfEmails[arrayOfEmailsPointer], token2);
                arrayOfEmailsPointer++;
                //create msg string
                msg = malloc(sizeof(char)*strlen(currentString)+1);
                strcpy(msg, currentString);
                break;
            }
            else
            {
                //add email into string array of emails
                arrayOfEmails[arrayOfEmailsPointer] = malloc(sizeof(char)*strlen(token)+1);
                strcpy(arrayOfEmails[arrayOfEmailsPointer], token);
                arrayOfEmailsPointer++;
            }
        }
        //set global variables for cleaning duty
        msgGlobal = msg;
        arrayOfEmailsGlobal = arrayOfEmails;
        arrayOfEmailsPointerGlobal = arrayOfEmailsPointer;
        /* ------------------ */
        /* PARSING DUTY ENDS */
        /* ------------------ */

        /* ------------------ */
        /* SOCKET STUFF START */
        /* ------------------ */
        //Read welcome message from smtp server
        /* SEND mail from message */
        sentMessage = malloc(sizeof("MAIL FROM:<xkacma03@isa.local>\r\n"));
        strcpy(sentMessage, "MAIL FROM:<xkacma03@isa.local>\r\n");
        if ( write(s, sentMessage, strlen(sentMessage)) < 0)
        {
            perror("error on write");
            freeEmails(arrayOfEmails, arrayOfEmailsPointer, msg);
            wasMemoryCleaned = 1;
            return 1;
        }
        free(sentMessage);
        sentMessage = NULL;
        //Read response from server
        memset(returnedMsg, 0, 255);
        if((n = read(s, returnedMsg, sizeof(returnedMsg))) < 0)
        {
            perror("error on read");
        }
        //correction need to be done, sometimes server just return CR LF so need to correct that
        if((strcmp(returnedMsg, "\r\n")) == 0)
        {
            memset(returnedMsg, 0, 255);
            if((n = read(s, returnedMsg, sizeof(returnedMsg))) < 0)
            {
                perror("error on read");
            }
        }
        /* RCVT mail TO */
        //need to do it in for cycle for all mails
        for (int g = 0; g < arrayOfEmailsPointer; g++)
        {
            char odesilatele [256];
            memset(odesilatele, 0, 255);
            strcpy(odesilatele, "RCPT TO:<");
            strcat(odesilatele, arrayOfEmails[g]);
            strcat(odesilatele, ">\r\n");
            if ( write(s, odesilatele, strlen(odesilatele)) < 0)
            {
                perror("error on write");
                freeEmails(arrayOfEmails, arrayOfEmailsPointer, msg);
                wasMemoryCleaned = 1;
                return 1;
            }
            //Read response from server
            memset(returnedMsg, 0, 255);
            if((n = read(s, returnedMsg, sizeof(returnedMsg))) < 0)
            {
                perror("error on read");
            }
            //check if recipient exist
            if (strstr(returnedMsg, "250") == NULL)
            {
                fprintf(stderr, "Error, recipient %s probably doesn't exist!\nReturned: %s\n", arrayOfEmails[g], returnedMsg);
            }
        }
        /* SEND DATA */
        sentMessage = malloc(sizeof("data\r\n"));
        strcpy(sentMessage, "data\r\n");
        if ( write(s, sentMessage, strlen(sentMessage)) < 0)
        {
            perror("error on write");
            freeEmails(arrayOfEmails, arrayOfEmailsPointer, msg);
            wasMemoryCleaned = 1;
            return 1;
        }
        //Let program now that we are in DATA section and u just can't use QUIT like that
        inDataSection = 1;
        free(sentMessage);
        sentMessage = NULL;
        //Read response from server
        memset(returnedMsg, 0, 255);
        if((n = read(s, returnedMsg, sizeof(returnedMsg))) < 0)
        {
            perror("error on read");
        }
        //Check if data return 354 and everything went ok
        if (strstr(returnedMsg, "354") == NULL)
        {
            fprintf(stderr, "Error, cannot send the message, error: %s\n", returnedMsg);
            freeEmails(arrayOfEmails, arrayOfEmailsPointer, msg);
            wasMemoryCleaned = 1;
            continue;
        }
        /* SEND THE ACTUAL MESSAGE */
        if ( write(s, msg, strlen(msg)) < 0)
        {
            perror("error on write");
            freeEmails(arrayOfEmails, arrayOfEmailsPointer, msg);
            wasMemoryCleaned = 1;
            return 1;
        }
        /* end the transaction with dot character */
        sentMessage = malloc(sizeof("\r\n.\r\n"));
        strcpy(sentMessage, "\r\n.\r\n");
        if ( write(s, sentMessage, strlen(sentMessage)) < 0)
        {
            perror("error on write");
            freeEmails(arrayOfEmails, arrayOfEmailsPointer, msg);
            wasMemoryCleaned = 1;
            return 1;
        }
        //Let program know that data section is over and you can use safely QUIT
        inDataSection = 0;
        free(sentMessage);
        sentMessage = NULL;
        //Read response from server
        memset(returnedMsg, 0, 255);
        if((n = read(s, returnedMsg, sizeof(returnedMsg))) < 0)
        {
            perror("error on read");
        }

        /* ------------------ */
        /* SOCKET STUFF ENDS  */
        /* ------------------ */

        /* ------------------ */
        /* CLEANING DUTY START*/
        /* ------------------ */
        freeEmails(arrayOfEmails, arrayOfEmailsPointer, msg);
        wasMemoryCleaned = 1;
        /* ------------------ */
        /* CLEANING DUTY ENDS */
        /* ------------------ */

        /* main for each line ends */

    }
    /* end the connection with QUIT message*/
    char *quitMsg;
    quitMsg = malloc(sizeof("QUIT\r\n"));
    strcpy(quitMsg, "QUIT\r\n");
    if ( write(s, quitMsg, strlen(quitMsg)) < 0)
    {
        perror("error on write");
        return 1;
    }
    free(quitMsg);
    //Read response from server
    memset(returnedMsg, 0, 255);
    if((n = read(s, returnedMsg, sizeof(returnedMsg))) < 0)
    {
        perror("error on read");
    }
    //Check if server return 221 and everything went ok
    if (strstr(returnedMsg, "221") == NULL)
    {
        fprintf(stderr, "Error while closing connection, something went wrong!\n");
    }
    //if w param was set, sleep before closing connection
    if( wvalue > 0)
        sleep(wvalue);
    close(s);

    return 0;
}

/* clear memory after sending emails */
void freeEmails(char **arrayOfEmails, int arrayOfEmailsPointer, char *msg)
{
    for (int j = 0; j < arrayOfEmailsPointer; j++)
    {
        //printf("Email cislo: %d je: %s\n", j+1, arrayOfEmails[j]);
        free(arrayOfEmails[j]);
    }
    //printf("msg je: %s\n", msg);
    free(msg);
}
int main (int argc, char **argv){
    /* signal blog */
    if(signal(SIGINT, sig_handler) == SIG_ERR)
            printf("Can't catch SIGKILL\n");
    if(signal(SIGTERM, sig_handler) == SIG_ERR)
            printf("Can't catch SIGTERM\n");
    if(signal(SIGQUIT, sig_handler) == SIG_ERR)
            printf("Can't catch SIGQUIT\n");
    /* declaration blog */
    inputFile = NULL;
    int lenghtOfFilecounter = 0;
    /*---------------------------
       parametr processing blog
    ---------------------------*/
    //variable with arguments
	//IP adresa
	char *avalue = "127.0.0.1";
	//port
	int pvalue = 25;
	//soubor
	char *ivalue = "";
	//sekundy
	int wvalue = 0;
	//clear memory of argumentArray
	memset(argumentArray, 0, 4);
    //ip mode, can be 4 or 6 depending on IPvX version, 4 is default
	int ipMode = 4;
	/* declaration blog ends*/

	//apiw, [0] - a - IP, [1] - p - port, [2] - i - soubor, [3] - w - sekundy
    int returnFromArgumentValue = argumentProcessing(argc, argv, argumentArray, &avalue, &pvalue, &ivalue, &wvalue);
    ivalueGlobal = ivalue;
    avalueGlobal = avalue;
    //if 60 is returned, there was argument error
    if(returnFromArgumentValue == 60)
    {
        fprintf(stderr, "Bad params, usage: ./smtpklient [-h] [-a IP] [-p port] -i file [-w seconds]\n");
        relaseArgumentArray(avalue, ivalue);
        return 60;
    }

    //check if file is presented
    if(argumentArray[2] != 1)
    {
        fprintf(stderr, "Error, you need to give the file param\n");
        relaseArgumentArray(avalue, ivalue);
        return 4;
    }

    /* opening a file */
    inputFile = fopen(ivalue, "r");
    if(inputFile == NULL)
    {
        perror("Error, cannot open the file:");
        relaseArgumentArray(avalue, ivalue);
        exit(3);
    }
    //check w value
    if( wvalue > 3600 || wvalue < 0)
    {
        fprintf(stderr, "Error, w param need to be less then 1 hour!\n");
        relaseArgumentArray(avalue, ivalue);
        fclose(inputFile);
        return -1;
    }
    /*process file and get lenght of the file :)*/
    if((fileProcessing(inputFile, &lenghtOfFilecounter)) == 1)
    {
        /* cleaning memory */
        relaseArgumentArray(avalue, ivalue);
        fclose(inputFile);
        return 2;
    }
    /* alocate string for the file content */
    char fileString[lenghtOfFilecounter];
    memset(fileString, 0, lenghtOfFilecounter-1);
    /* get file content into string */
    int sizeOfString = getFileContent(inputFile, lenghtOfFilecounter, fileString);
    /* count number of lines in the file */
    int numberOfLines = countLines(fileString, sizeOfString);
    numberOfLinesGlobal = numberOfLines;
    /* alocate array of chars, for each line one pointer and array that will keep their lenght*/
    char *charPointers[numberOfLines];
    int charPointersLenght[numberOfLines];
    /* get lenght of each lines */
    getLenghtOfLines(fileString, charPointersLenght);
    /* create strings based on lenght of each line */
    for (int x = 0; x < numberOfLines; x++)
    {
        charPointers[x] = malloc(sizeof(char)*charPointersLenght[x]+1);
    }
    charPointersGlobal = charPointers;
    /* fill strings with data */
    fillStringsWithData(fileString, charPointers);
    //Check if we have IPv4 or IPv6 address
    if(strstr(avalue, ".") != NULL)
        ipMode = 4;
    else
        ipMode = 6;
    //sendEmailControl will send emails based on data that we got from upper functions
    if (sendEmailControl(charPointers, numberOfLines, avalue, pvalue, ivalue, wvalue, ipMode) != 0)
    {
        fprintf(stderr, "Something went WRONG :(, couldn't send email/s!\n");
    }


    /* cleaning memory */
    relaseArgumentArray(avalue, ivalue);
    fclose(inputFile);
    for (int x = 0; x < numberOfLines; x++)
    {
        free(charPointers[x]);
    }


	return 0;
}

void relaseArgumentArray(char *avalue, char *ivalue)
{
    if(argumentArray[0])
        free(avalue);
    if(argumentArray[2])
        free(ivalue);
}
