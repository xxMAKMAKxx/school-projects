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
#include <pwd.h>
#include <signal.h>
/* function addToString add to msg the string given, based on topIndex */
void addToString (char *msg, char *string,int *topIndex)
{
    int lenghtOfString = strlen(string);
    //printf("lenght: %d top: %d\n", lenghtOfString, (*topIndex));
    (*topIndex)--;
    int i;
    for(i = 0; i < (lenghtOfString+1); i++)
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

void createReturnString(struct passwd *result, int valueArray[6], char *theMessage) //based on return and valueArray return string with propper informations
{
    char *first, *second, *third, *fourth, *fifth, *sixth;
    first = second = third = fourth = fifth = sixth = NULL;
    int firstN, secondN, thirdN, fourthN, fifthN, sixthN;
    firstN = secondN = thirdN = fourthN = fifthN = sixthN = -1;
    char buffer[32];
    memset(buffer, 0, 31);
    int numberOf = 0;
    int top = 0;
    int topIndex = 1;
    /* Go trough whole array and find first second etc record and make char that you will return (so can server return it to client so it can be printed) */
    int i;
    for(i = 0; i < 6; i++)
    {
        if(valueArray[i] == 1) // first
        {
            top++;
            if(i == 0)
            {
                first = result->pw_name;
            }
            else if(i == 1)
            {
                firstN = result->pw_uid;
            }
            else if(i == 2)
            {
                firstN = result->pw_gid;
            }
            else if(i == 3)
            {
                first = result->pw_gecos;
            }
            else if(i == 4)
            {
                first = result->pw_dir;
            }
            else if(i == 5)
            {
                first = result->pw_shell;
            }
        }
        else if(valueArray[i] == 2) //second
        {
            top++;
            if(i == 0)
            {
                second = result->pw_name;
            }
            else if(i == 1)
            {
                secondN = result->pw_uid;
            }
            else if(i == 2)
            {
                secondN = result->pw_gid;
            }
            else if(i == 3)
            {
                second = result->pw_gecos;
            }
            else if(i == 4)
            {
                second = result->pw_dir;
            }
            else if(i == 5)
            {
                second = result->pw_shell;
            }
        }
        else if(valueArray[i] == 3) //third
        {
            top++;
            if(i == 0)
            {
                third = result->pw_name;
            }
            else if(i == 1)
            {
                thirdN = result->pw_uid;
            }
            else if(i == 2)
            {
                thirdN = result->pw_gid;
            }
            else if(i == 3)
            {
                third = result->pw_gecos;
            }
            else if(i == 4)
            {
                third = result->pw_dir;
            }
            else if(i == 5)
            {
                third = result->pw_shell;
            }
        }
        else if(valueArray[i] == 4) //fourth
        {
            top++;
            if(i == 0)
            {
                fourth = result->pw_name;
            }
            else if(i == 1)
            {
                fourthN = result->pw_uid;
            }
            else if(i == 2)
            {
                fourthN = result->pw_gid;
            }
            else if(i == 3)
            {
                fourth = result->pw_gecos;
            }
            else if(i == 4)
            {
                fourth = result->pw_dir;
            }
            else if(i == 5)
            {
                fourth = result->pw_shell;
            }
        }
        else if(valueArray[i] == 5) //fifth
        {
            top++;
            if(i == 0)
            {
                fifth = result->pw_name;
            }
            else if(i == 1)
            {
                fifthN = result->pw_uid;
            }
            else if(i == 2)
            {
                fifthN = result->pw_gid;
            }
            else if(i == 3)
            {
                fifth = result->pw_gecos;
            }
            else if(i == 4)
            {
                fifth = result->pw_dir;
            }
            else if(i == 5)
            {
                fifth = result->pw_shell;
            }
        }
        else if(valueArray[i] == 6) //sixth
        {
            top++;
            if(i == 0)
            {
                sixth = result->pw_name;
            }
            else if(i == 1)
            {
                sixthN = result->pw_uid;
            }
            else if(i == 2)
            {
                sixthN = result->pw_gid;
            }
            else if(i == 3)
            {
                sixth = result->pw_gecos;
            }
            else if(i == 4)
            {
                sixth = result->pw_dir;
            }
            else if(i == 5)
            {
                sixth = result->pw_shell;
            }
        }
    }
    if(first == NULL && firstN != -1)
    {
        sprintf(buffer, "%d", firstN);
        addToString(theMessage, buffer, &topIndex);
    }
    else if(first != NULL && firstN == -1)
    {
        addToString(theMessage, first, &topIndex);
    }
    if(second == NULL && secondN != -1)
    {
        sprintf(buffer, "%d", secondN);
        addToString(theMessage, " ", (&topIndex));
        addToString(theMessage, buffer, &topIndex);
    }
    else if(second != NULL && secondN == -1)
    {
        addToString(theMessage, " ", (&topIndex));
        addToString(theMessage, second, &topIndex);
    }
    if(third == NULL && thirdN != -1)
    {
        addToString(theMessage, " ", (&topIndex));
        sprintf(buffer, "%d", thirdN);
        addToString(theMessage, buffer, &topIndex);
    }
    else if(third != NULL && thirdN == -1)
    {
        addToString(theMessage, " ", (&topIndex));
        addToString(theMessage, third, &topIndex);
    }
    if(fourth == NULL && fourthN != -1)
    {
        addToString(theMessage, " ", (&topIndex));
        sprintf(buffer, "%d", fourthN);
        addToString(theMessage, buffer, &topIndex);
    }
    else if(fourth != NULL && fourthN == -1)
    {
        addToString(theMessage, " ", (&topIndex));
        addToString(theMessage, fourth, &topIndex);
    }
    if(fifth == NULL && fifthN != -1)
    {
        addToString(theMessage, " ", (&topIndex));
        sprintf(buffer, "%d", fifthN);
        addToString(theMessage, buffer, &topIndex);
    }
    else if(fifth != NULL && fifthN == -1)
    {
        addToString(theMessage, " ", (&topIndex));
        addToString(theMessage, fifth, &topIndex);
    }
    if(sixth == NULL && sixthN != -1)
    {
        addToString(theMessage, " ", (&topIndex));
        sprintf(buffer, "%d", sixthN);
        addToString(theMessage, buffer, &topIndex);
    }
    else if(sixth != NULL && sixthN == -1)
    {
        addToString(theMessage, " ", (&topIndex));
        addToString(theMessage, sixth, &topIndex);
    }


}

void searchPwd(char *msg, char *theMessage){
    int valueArray[6] = {0,0,0,0,0,0};
    struct passwd *result;
    int numberOfDoubleDots = 0;
    int isLogin = -1;
    int topIndex = 1;
    int index = 0; //index how long is name so I can put \0 at the end of the string
    char name[32]; //can be both login and uid, we will see
    memset(name, 0, 31);
    /* result = getpwnam(tolower_login); or getpwuid
    if (result == NULL){
        fprintf(stderr, "Error, login or UID not found!\n");
    }*/
    int lenght = (int)(strlen(msg)+1);
    int i;
    for(i = 0; i < lenght ; i++)
    {
        if(msg[i] == ':') //inc numberofdots when found one - determine where in string ure
        {
            numberOfDoubleDots++;
            continue;
        }

        if(numberOfDoubleDots == 0) //looking for uid / login
        {
            if(isdigit(msg[0])) //is digit - uid
            {
                isLogin = 0;
            }
            else //is string - login
            {
                isLogin = 1;
            }
            name[i] = tolower(msg[i]);
            index++;
        }
        if(numberOfDoubleDots == 1)
        {
            if(msg[i] != '0')
                valueArray[0] = (msg[i] - '0');
        }
        if(numberOfDoubleDots == 2)
        {
            if(msg[i] != '0')
                valueArray[1] = (msg[i] - '0');
        }
        if(numberOfDoubleDots == 3)
        {
            if(msg[i] != '0')
                valueArray[2] = (msg[i] - '0');
        }
        if(numberOfDoubleDots == 4)
        {
            if(msg[i] != '0')
                valueArray[3] = (msg[i] - '0');
        }
        if(numberOfDoubleDots == 5)
        {
            if(msg[i] != '0')
                valueArray[4] = (msg[i] - '0');
        }
        if(numberOfDoubleDots == 6)
        {
            if(msg[i] != '0')
                valueArray[5] = (msg[i] - '0');
        }
    }
    name[index+1] = '\0'; //end of the string


    if(isLogin == 1) //getpwnam
    {
        result = getpwnam(name);
        if (result == NULL){
            addToString(theMessage, "Chyba: neznamy login ", &topIndex);
            addToString(theMessage, name, &topIndex);
        }
        else
        {
            createReturnString(result, valueArray, theMessage);
        }
    }
    else //getpwuid
    {
        int uid = atoi(name);
        result = getpwuid(uid);
        if (result == NULL){
            addToString(theMessage, "Chyba: nezname uid ", &topIndex);
            addToString(theMessage, name, &topIndex);
        }
        else
        {
            createReturnString(result, valueArray, theMessage);
        }
    }
}

int main (int argc, char **argv){

    //variable declaration
    int c;
    pid_t pid;
    int pflag = 0;
    char *pvalue;
    int pIntValue = -99;
    opterr = 0;
    char msg[160];
    char theMessage[128];
    memset(theMessage, 0, 127);

    //argument processing
    while ((c = getopt (argc, argv, "p:")) != -1){
        switch (c)
          {
          case 'p':
            pflag = 1;
            pvalue = optarg;
            break;
          case '?':
            if (optopt == 'p')
              fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
              fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
              fprintf (stderr,
                       "Unknown option character `\\x%x'.\n",
                       optopt);
            return 1;
          default:
            abort ();
          }
    }

    //argument check
    pIntValue = atoi(pvalue);
    if(pflag == 0)
    {
        fprintf(stderr, "0 parameters given. Type -p port");
        return 1;
    }
    else if(pIntValue == -99)
    {
        fprintf(stderr, "Error! Cannot convert port parametr to int! Something is wrong!");
        return 1;
    }

    //REAL MAIN START
    int s, sinlen, t;
    struct sockaddr_in sin;
    struct hostent * hp;

    if ( (s = socket(PF_INET, SOCK_STREAM, 0 ) ) < 0) { /* create socket*/
        fprintf(stderr, "error on socket\n");  /* socket error */
        return -1;
    }
    sin.sin_family = PF_INET;              /*set protocol family to Internet */
    sin.sin_port = htons(atoi(pvalue));    /* set port no. */
    sin.sin_addr.s_addr  = INADDR_ANY;     /* set IP addr to any interface */
    if (bind(s, (struct sockaddr *)&sin, sizeof(sin) ) < 0 ) {
        fprintf(stderr, "error on bind\n"); return -1;  /* bind error */
    }
    if (listen(s, 5)) {
        fprintf (stderr, "error on listen\n"); /* listen error*/
        return -1;
    }
    signal(SIGCHLD, SIG_IGN);

    sinlen = sizeof(sin);
    //konec pripravy pro server

    /*searchPwd("1000:4:1:0:3:0:2:", theMessage); //"xkacma03:4:1:0:3:0:2:

    printf("%s\n", theMessage);
    memset(theMessage, 0, 159);
    searchPwd("xkacma03:4:1:0:3:0:2:", theMessage); //"xkacma03:4:1:0:3:0:2:

    printf("%s\n", theMessage);*/

    while(1)
    {
        /* accepting new connection request from client,
        socket id for the new connection is returned in t*/
        if ( (t = accept(s, (struct sockaddr *) &sin, &sinlen) ) < 0 ) {
          fprintf(stderr, "error on accept\n");  /* accept error*/
          return -1;
        }

        pid = fork();
        if (pid == 0) //child
        {
            bzero(msg, sizeof(msg));
            if( read(t,msg, sizeof(msg)) < 0)
            {
                fprintf(stderr, "error on read!\n");
                return -1;
            }

            searchPwd(msg, theMessage);
            if ( write(t, theMessage, strlen(theMessage)+1) < 0 ) {  /* echo message back*/
                fprintf(stderr, "error on write\n");    return -1; /*  write error*/
            }
            if (close(t) < 0) { fprintf(stderr, "error on close\n"); return -1;}
            if (close(s) < 0) { fprintf(stderr, "error on close\n"); return -1;}
            exit(0);
        }
        else if(pid < 0) //chyba
        {
            fprintf(stderr, "Chyba! Nepodarilo se vytvorit novy proces :(\n");
        }
        else //parrent
        {
            /* close connection, clean up sockets*/
            if (close(t) < 0) { fprintf(stderr, "error on close\n"); return -1;}
        }


    }

    //uzavreni socketu
    if (close(s) < 0) { fprintf(stderr, "close"); return -1;}
    return 0;
}
