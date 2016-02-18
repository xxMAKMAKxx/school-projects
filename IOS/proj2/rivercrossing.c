/*
Projekt 3. do predmetu IOS
Implementace tzv. rivercrossing problemu.
Martin Kacmarcik - xkacma03 - (c) 2014
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>


typedef struct
{
    int arg[4];
} Arguments;

Arguments a; //pole s argumenty, 1. parametr - pocet H a S, 2. doba tvorby H, 3. doba tvorby S, 4. doba plavby

typedef struct
{
   int hackerCount;
   int serfsCount;
   int radek;
   int moloHackers;
   int moloSerfs;
   int inBoat;
   int predFinished;
   FILE *vypis;
} SharedMemoryData;


int shm_fd, readcount;
SharedMemoryData *data = NULL; //struktura obsahujici data potrebna pro synchronizaci (sdilena pamet)

sem_t *s_zapis, *s_cteni, *s_molo, *s_usleep, *s_hackersQueue, *s_serfsQueue, *s_bariera, *s_plavba, *s_vylodeni, *s_uspani, *s_finished; //semafory

int randomGenerator(int maximum) //funkce vygeneruje nahodne cislo pomoci rand. Je nutne osetrit rozsah, proto modulo ktere cislo redukuje pro rozsah.
{
    int cislo = (rand() % (maximum-1));
    return cislo;
}

void errMsg() //funkce pro error message
{
        fprintf(stderr, "Chyba argumentu!\n");
        exit(1);
}



void relaseThemAll(int shm_fd)
{
    //semafor dealoakce start
    if (sem_destroy(s_zapis) == -1 ) { fprintf(stderr ,"Nepodarilo se uvolnit zdroje!\n"); exit(2);}
    if (sem_destroy(s_cteni) == -1 ) { fprintf(stderr ,"Nepodarilo se uvolnit zdroje!\n"); exit(2);}
    if (sem_destroy(s_molo) == -1 ) { fprintf(stderr ,"Nepodarilo se uvolnit zdroje!\n"); exit(2);}
    if (sem_destroy(s_usleep) == -1 ) { fprintf(stderr ,"Nepodarilo se uvolnit zdroje!\n"); exit(2);}
    if (sem_destroy(s_hackersQueue) == -1 ) { fprintf(stderr ,"Nepodarilo se uvolnit zdroje!\n"); exit(2);}
    if (sem_destroy(s_serfsQueue) == -1 ) { fprintf(stderr ,"Nepodarilo se uvolnit zdroje!\n"); exit(2);}
    if (sem_destroy(s_bariera) == -1 ) { fprintf(stderr ,"Nepodarilo se uvolnit zdroje!\n"); exit(2);}
    if (sem_destroy(s_plavba) == -1 ) { fprintf(stderr ,"Nepodarilo se uvolnit zdroje!\n"); exit(2);}
    if (sem_destroy(s_vylodeni) == -1 ) { fprintf(stderr ,"Nepodarilo se uvolnit zdroje!\n"); exit(2);}
    if (sem_destroy(s_uspani) == -1 ) { fprintf(stderr ,"Nepodarilo se uvolnit zdroje!\n"); exit(2);}
    if (sem_destroy(s_finished) == -1 ) { fprintf(stderr ,"Nepodarilo se uvolnit zdroje!\n"); exit(2);}
    //semafor dealokace end

    fclose(data->vypis); //uzavreni souboru
    if (munmap(data, sizeof(SharedMemoryData))  == -1 ) { fprintf(stderr ,"Nepodarilo se uvolnit zdroje!\n"); exit(2);} //odmapovani pameti

    if (shm_unlink("/sharedMemoryMak")  == -1 ) { fprintf(stderr ,"Nepodarilo se uvolnit zdroje!\n"); exit(2);} //odlinkovani pojmenovane pameti

    if (close(shm_fd) == -1 ) { fprintf(stderr ,"Nepodarilo se uvolnit zdroje!\n"); exit(2);} //zavreni filedescriptoru
}

void errMsg2(int shm_fd)
{
    fprintf(stderr, "Chyba pri syscall\n");
    relaseThemAll(shm_fd);
    exit(2);
}

void terminate()
{
    relaseThemAll(shm_fd);
    exit(2);
}

void argumentCheck(int argc,char **argv) //funkce pro check argumentu
{
    if(argc != 5)
    {
        errMsg(); //argumentu musi byt 5. 1. je cesta, 4 jsou cisla potrebna pro beh souboru.
    }
    int i;
    for(i = 1; i < argc; i++)
    {
        a.arg[i-1]= strtol (argv[i],NULL,0); //prevod argumentu ze string na cisla a prirazeni do pole argumentu
    }
    if((a.arg[0] > 0) &&  (a.arg[0] % 2 == 0)) //podminka 1 - tolik podminek je tu pro prehlednost, snaze se kontroluje jednotlive nez ve slozitem vyrazu
    {
        if(a.arg[1] >= 0 && a.arg[1] < 5001) //podminka 2
        {
            if(a.arg[2] >= 0 && a.arg[2] < 5001) //podminka 3
            {
                if(a.arg[3] >= 0 && a.arg[3] < 5001) //podminka 4
                {
                    return;
                }
                else
                    errMsg();
            }
            else
                errMsg();
        }
        else
            errMsg();
    }
    else
    {
        errMsg();
    }
}

void prichodNaMolo(int spoustec, int pomocna) //funkce simulujici prichod na molo
{
            char *procesName; //nazev procesu - hacker nebo serf
            int myIterator, pomocna2; //iterator + pomocna promena pro jakekoliv ucely (ale je pouzivana pro zjisteni cisla procesu hackera/serfa)
            sem_wait(s_molo); //nepust nikoho na molo dokud neni vhodna doba!!!
            int isCaptain = 0; // isCaptain == 0 false, isCaptain == 1 true

            if (spoustec == 0) //hacker ?
            {
                sem_wait(s_zapis);
                data->moloHackers++; //pribyl hacker
                data->radek++; //zvys radek budes zapisovat
                sem_post(s_zapis);
                procesName = "hacker";
            }
            else if(spoustec == 1) // serf?
            {
                sem_wait(s_zapis);
                data->moloSerfs++; //pribyl serf
                data->radek++; //zvys radek budes zapisovat
                sem_post(s_zapis);
                procesName = "serf";
            }

            sem_wait(s_zapis);
            fflush(data->vypis); //vypis, ze prislusny proces dosel na molo a ceka na nalodeni
            fprintf(data->vypis, "%i: %s: %i: waiting for boarding: %i: %i\n", data->radek, procesName, pomocna, data->moloHackers, data->moloSerfs);
            sem_post(s_zapis);


            if (data->moloHackers == 4) //jsou 4 hackeri a muzu udelat lod?
            {
                sem_wait(s_zapis);
                data->moloHackers = 0; //nyni ovsem je na molu 0 hackeru
                sem_post(s_zapis);
                isCaptain = 1; //ten ktery prisel posledni a trignul podminku je Cpt.

                for (myIterator = 0; myIterator < 4; myIterator++)
                {
                    sem_post(s_hackersQueue); //pokud ano, tak propust frontou 4 hackery.
                }
            }
            else if (data->moloSerfs == 4) //jsou 4 serfove a muzu udelat lod?
            {
                sem_wait(s_zapis);
                data->moloSerfs = 0; //nyni ovsem je na molu 0 serfu
                sem_post(s_zapis);
                isCaptain = 1; //ten ktery prisel posledni a trignul podminku je Cpt.

                for (myIterator = 0; myIterator < 4; myIterator++)
                {
                    sem_post(s_serfsQueue);  //pokud ano, tak propust frontou 4 serfy.
                }
            }
            else if (data->moloHackers == 2 && data->moloSerfs >= 2) // nebo jsou 2 hackeri a libovolny pocet serfu?
            {
                sem_wait(s_zapis);
                data->moloSerfs -= 2; //hackeru aj serfu je na molu o 2 min.
                data->moloHackers -= 2;
                sem_post(s_zapis);
                isCaptain = 1; //ten kdo spustil je Cpt.
                sem_post(s_serfsQueue); //propust 2 hackery a 2 serfy frontou
                sem_post(s_serfsQueue);
                sem_post(s_hackersQueue);
                sem_post(s_hackersQueue);
            }
            else if(data->moloHackers >= 2 && data->moloSerfs == 2) // nebo jsou 2 serfove a libovolny pocet hackeru?
            {

                sem_wait(s_zapis);
                data->moloSerfs -= 2; //hackeru aj serfu je na molu o 2 min.
                data->moloHackers -= 2;
                data->radek++;
                sem_post(s_zapis);
                isCaptain = 1; //ten kdo spustil je Cpt.
                sem_post(s_serfsQueue); //propust 2 hackery a 2 serfy frontou
                sem_post(s_serfsQueue);
                sem_post(s_hackersQueue);
                sem_post(s_hackersQueue);
            }
            else
            {
                sem_post(s_molo); //kdyz nemuzes udelat lod, pust dalsiho na molo, treba on je ten vyvoleny
            }

            if(spoustec == 0)
            {
                sem_wait(s_hackersQueue); //pokud si dosel az sem a si hacker tak cekej ve fronte s hackery
            }
            else if(spoustec == 1)
            {
                sem_wait(s_serfsQueue); //pokud si dosel az sem a si serf tak cekej ve fronte se serfy
            }

            sem_wait(s_zapis);
            data->radek++;
            sem_post(s_zapis);

            sem_wait(s_cteni);

            sem_wait(s_zapis);
            fflush(data->vypis); //muzes se nalodit, napis to, ze se nalodujes procese.
            fprintf(data->vypis, "%i: %s: %i: boarding: %i: %i\n", data->radek, procesName, pomocna, data->moloHackers, data->moloSerfs);
            sem_post(s_zapis);



            sem_wait(s_zapis);
            data->inBoat++; //no jo, ted je ale v lodi o 1 vic, tak to dej vedet do sdilene pameti.
            sem_post(s_zapis);


            //kriticka sekce cteni start
            sem_wait(s_zapis);
            pomocna2 = data->inBoat; //do pomocne2 si uloz pocet lidi na lodi
            sem_post(s_zapis);
            //kriticka sekce cteni end

            sem_post(s_cteni);

            if (pomocna2 == 4) //jsou uz vsichni na lodi? Pokud ano pust je barierou
            {
                sem_wait(s_zapis);
                data->inBoat = 0; //vynuluj pocet lidi na lodi
                sem_post(s_zapis);

                sem_post(s_bariera);
                sem_post(s_bariera); //propust je barierou
                sem_post(s_bariera);
            }
            else //nejsou vsichni jeste na lodi? Cekej na ne. (ceky neeeee xD)
            {
                sem_wait(s_bariera);
            }



            //vypis clenu posadky start
            if( isCaptain == 1) //je to kapitan?
            {
                sem_wait(s_zapis);
                data->radek++;
                sem_post(s_zapis);

                //kriticka sekce cteni start
                sem_wait(s_zapis);

                fflush(data->vypis); //vypis, ze je proces kapo
                fprintf(data->vypis,"%i: %s: %i: captain\n", data->radek, procesName, pomocna);

                sem_post(s_zapis);
                //kriticka sekce cteni end


            }
            else // neni kapitan :(
            {
                sem_wait(s_zapis);
                data->radek++;
                sem_post(s_zapis);

                sem_wait(s_zapis);

                fflush(data->vypis); //vypis ze si JEN member
                fprintf(data->vypis, "%i: %s: %i: member\n", data->radek, procesName, pomocna);

                sem_post(s_zapis);
                //kriticka sekce cteni end
                sem_post(s_uspani);
            }
            //vypis clenu posadky end

            //simulace plavby start
            if(isCaptain == 1)
            {
                sem_wait(s_uspani);
                sem_wait(s_uspani); //neuspavej dokud vsichni se nevyspali
                sem_wait(s_uspani);
                srand (time(NULL) & getpid());
                usleep(randomGenerator(a.arg[3]) * 1000); //simuluj plavbu
                sem_wait(s_zapis);
                data->radek++;
                fflush(data->vypis); //cpt. Landuje prvni
                fprintf(data->vypis, "%i: %s: %i: landing: %i: %i\n", data->radek, procesName, pomocna, data->moloHackers, data->moloSerfs);
                sem_post(s_zapis);

                sem_post(s_plavba);
                sem_post(s_plavba); //dej vedet ostatnim ze uz taky muzou landovat
                sem_post(s_plavba);
            }
            else
            {
                sem_wait(s_plavba);
                sem_wait(s_zapis);
                data->radek++;
                fflush(data->vypis); //cpt. nam dal prikaz, tak muzem landovat
                fprintf(data->vypis, "%i: %s: %i: landing: %i: %i\n", data->radek, procesName, pomocna, data->moloHackers, data->moloSerfs);
                sem_post(s_zapis);
                sem_post(s_vylodeni);
            }
            //simulace plavby end

            if(isCaptain == 1) //posledni kroky kapitana, ceka na vylodeni vsech clenu posadky
            {
                sem_wait(s_vylodeni);
                sem_wait(s_vylodeni); //pokud se vsichni uz vylodili, muzou dalsi na molo :)
                sem_wait(s_vylodeni);
                sem_post(s_molo);
            }


}

void procesGenerator(int spoustec) // spoustec - 0 pro hackery, 1 pro serfy
{
    pid_t procesNew;
    char *procesName; //nazev procesu
    int r, i, pomocna; //pomocna promene, r - nahodne cislo pro uspani, i - iterator, pomocna - pro pomocne ucely
    for (i = 0; i < a.arg[0]; i++)
    {
        procesNew = fork();
        if (procesNew == 0)
        {
        	srand (time(NULL) * getpid()); //nastaveni srand pro zajisteni opravdu nahodneho generovani cisel
            if (spoustec == 0) //je to hacker?
            {

                procesName = "hacker";
                sem_wait(s_usleep); //pockej na semafor uspani
                r = randomGenerator(a.arg[1]); //vygeneruj nahodne cislo pro uspani

                //kriticka sekce zapis start
                sem_wait(s_zapis);
                data->hackerCount++; //prisel na breh hacker, zvys pocet hackeru kteri jsou v programu
                sem_post(s_zapis);
                //kriticka sekce zapis end

                //kriticka sekce cteni start
                sem_wait(s_zapis);
                pomocna = data->hackerCount; //nastaveni pomocne promene na pocet hackeru
                sem_post(s_zapis);
                //kriticka sekce cteni end

            }
            else if(spoustec == 1) //je to serf?
            {
                sem_wait(s_usleep); //pockej na semafor uspani
                r = randomGenerator(a.arg[2]); //vygeneruj nahodne cislo pro uspani
                procesName = "serf";
                 //kriticka sekce zapis start
                sem_wait(s_zapis);
                data->serfsCount++; //1 novy serf
                sem_post(s_zapis);
                //kriticka sekce zapis end

                sem_wait(s_zapis);
                pomocna = data->serfsCount; //nastaveni pomocne promene na pocet serfu
                sem_post(s_zapis);

            }


            sem_wait(s_zapis);

            usleep(r*1000); //uspi na r milisekund
            data->radek++; //pred kazdym vypisem bude radek++
            fflush(data->vypis);
            fprintf(data->vypis, "%i: %s: %i: started\n", data->radek, procesName, pomocna); //prichod na breh
            sem_post(s_usleep);

            sem_post(s_zapis);


            prichodNaMolo(spoustec, pomocna); //spust prichod na molo

            sem_wait(s_zapis);
            data->predFinished++; //zapis, ze dalsi proces je pred finisem
            sem_post(s_zapis);

            if(data->predFinished == (2*a.arg[0])) //pokud si posledni pust vsechny do finise
            {
                int g;
                for (g = 1; g < (2*a.arg[0]); g++)
                {
                    sem_post(s_finished);
                }
            }
            else //pokud ne cekej na semaforu s_finished
            {
                sem_wait(s_finished);
            }
            sem_wait(s_zapis);
            data->radek++;
            fflush(data->vypis); //vypis, ze si finishnul
            fprintf(data->vypis, "%i: %s: %i: finished\n", data->radek, procesName, pomocna);

            sem_post(s_zapis);
            exit(0); //rekni, ze si uspense skoncil :)
        }
        else if (procesNew < 0)
        {
            fprintf(stderr, "Chyba pri vytvoreni procesu!\n");
            kill(0, SIGTERM); //pokud dojde k chybe vytvareni procesu, dej to vedet na stderr a killni vsechny deti
            exit(2);
        }
    }

}


int main(int argc,char **argv)
{

    argumentCheck(argc, argv); //prirazeni argumentu

    //handler pro signal start
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = terminate;
    sigaction(SIGTERM, &action, NULL);
    //handler pro signal end

    //SharedMemory - alokovani mapovani sdilene pameti
    if( (shm_fd = shm_open("/sharedMemoryMak", O_CREAT | O_EXCL | O_RDWR, 0644)) == -1) {errMsg2(shm_fd); }
    if( (ftruncate(shm_fd, sizeof(SharedMemoryData)) ) == -1) {errMsg2(shm_fd); }
    if( (data = mmap(NULL, sizeof(SharedMemoryData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0) ) == MAP_FAILED) {errMsg2(shm_fd);}
    //SharedMemory END


    //shared memory init start
    data->radek = 0; //pred kazdym vypisem radek++
    data->hackerCount = 0;
    data->serfsCount = 0;
    data->moloHackers = 0;
    data->moloSerfs = 0;
    data->inBoat = 0;
    readcount = 0;
    data->predFinished = 0;
    data->vypis = NULL;
    //shared memory init end

    //fopen start
    if((data->vypis=fopen("rivercrossing.out","w")) == NULL)
	{
		fprintf(stderr,"Soubor se nepodarilo otevrit!\n");
		relaseThemAll(shm_fd);
	}
    //fopen end

    //semaphore init start
    if( (s_zapis = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {errMsg2(shm_fd);}
    if( (sem_init(s_zapis, 1, 1)) == -1) { errMsg2(shm_fd);}
    if( (s_molo = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {errMsg2(shm_fd);}
    if( (sem_init(s_molo, 1, 1)) == -1) { errMsg2(shm_fd);}
    if( (s_cteni = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {errMsg2(shm_fd);}
    if( (sem_init(s_cteni, 1, 1)) == -1) { errMsg2(shm_fd);}
    if( (s_usleep = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {errMsg2(shm_fd);}
    if( (sem_init(s_usleep, 1, 1)) == -1) { errMsg2(shm_fd);}
    if( (s_serfsQueue = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {errMsg2(shm_fd);}
    if( (sem_init(s_serfsQueue, 1, 0)) == -1) { errMsg2(shm_fd);}
    if( (s_hackersQueue = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {errMsg2(shm_fd);}
    if( (sem_init(s_hackersQueue, 1, 0)) == -1) { errMsg2(shm_fd);}
    if( (s_bariera = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {errMsg2(shm_fd);}
    if( (sem_init(s_bariera, 1, 0)) == -1) { errMsg2(shm_fd);}
    if( (s_plavba = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {errMsg2(shm_fd);}
    if( (sem_init(s_plavba, 1, 0)) == -1) { errMsg2(shm_fd);}
    if( (s_vylodeni = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {errMsg2(shm_fd);}
    if( (sem_init(s_vylodeni, 1, 0)) == -1) { errMsg2(shm_fd);}
    if( (s_uspani = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {errMsg2(shm_fd);}
    if( (sem_init(s_uspani, 1, 0)) == -1) { errMsg2(shm_fd);}
    if( (s_finished = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {errMsg2(shm_fd);}
    if( (sem_init(s_finished, 1, 0)) == -1) { errMsg2(shm_fd);}
    //semaphore init end

    setbuf(data->vypis, NULL); //nastaveni bufferu

    pid_t serfs, hackers; //vytvoreni pid promene pro hackery a pro serfy
    hackers = fork(); //novy proces hacker

    if(hackers == 0) //parrent vsech hackeru
    {
            procesGenerator(0); //vytvor noveho hackera
            int z;
            for (z = 0; z < a.arg[0];z++)
            {
                wait(NULL); //cekej az vsichni hackeri skonci
            }
            exit(0);
    }
    else if (hackers > 0)
    {
        serfs  = fork(); //novy proces serf
        if (serfs == 0) //parrent vsech serfu
        {
            procesGenerator(1); //generuj serfy
            int y;
            for (y = 0; y < a.arg[0];y++)
            {
                wait(NULL); //cekej az vsichni serfove skonci
            }
            exit(0);
        }
        else if(serfs > 0)
        {
            wait(NULL); //pockej na vsechny
        }
        else if (serfs < 0)
        {
            fprintf(stderr, "Chyba! Nepodarilo se vytvorit proces!\n");
			kill(0, SIGTERM); //pokud dojde k chybe vytvareni procesu, dej to vedet na stderr a killni vsechny deti
            exit(2);
        }
        wait(NULL); //pockej na vsechny
    }
    else
    {
        fprintf(stderr, "Chyba pri vytvoreni procesu!\n");
        kill(0, SIGTERM); //pokud dojde k chybe vytvareni procesu, dej to vedet na stderr a killni vsechny deti
        exit(2);
    }


    relaseThemAll(shm_fd); //dealokuj vse

    exit(0);
}

















