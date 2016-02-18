/**************************************
*  First project for ISA course       *
*            SMTP server              *
*  by Martin Kacmarcik - xkacma03     *
*  Faculty of Information Technology  *
*  University of Technology Brno      *
**************************************/

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


typedef enum {
    S_START = 0, //a
    S_EMAIL, //b
    S_MSGSTART, //c
    S_MSG, //e
    S_END, //f
} konecnyAutomat;

void sig_handler(int signo);
void signalCleaning();
void printHelp();
int argumentProcessing(int argc, char **argv, int *argumentArray, char **avalue, int *pvalue, char **ivalue, int *wvalue);
int fileProcessing(FILE *inputFile, int *counter);
int getFileContent(FILE *inputFile, int counter, char *inputString);
int countLines(char *fileString, int counter);
void getLenghtOfLines(char *fileString, int *charPointersLenght);
void fillStringsWithData(char *fileString, char **charPointers);
int sendEmailControl(char **arrayOfStrings, int numberOfStrings, char *avalue, int pvalue, char *ivalue, int wvalue, int ipMode);
void freeEmails(char **arrayOfEmails, int arrayOfEmailsPointer, char *msg);
int getLine(int fd, char *line, int lim);
void relaseArgumentArray(char *avalue, char *ivalue);
