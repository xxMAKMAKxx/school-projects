#makefile pro projekt do IMS by Martin Kacmarcik (xkacma03) a Klara Mihalikova (xmihal05)

LFLAGS=-g -O2 
RFLAGS=-lsimlib -lm
 
CC = g++
AUTHOR = xkacma03_xmihal05
FILES = ZelecnicniStanice.cpp Makefile dokumentace.pdf

all: ZelecnicniStanice

ZelecnicniStanice: ZelecnicniStanice.cpp
	$(CC) $(LFLAGS) -o $@ ZelecnicniStanice.cpp $(RFLAGS)

rebuild: clean all

#               0.        1.             2.            3.          4.        5.         6.         7.            8.                9.             10.         argument
#argumenty: ./model PocetNastupist SpozdeniRychliku SpozdeniEC SpozdeniOS Runtime Generovani_R Generovani_EC Generovani_OS   spozdeni_bool   nazev_outputu
#if false (f) then default
#                       1 2 3 4  5 6 7 8 9       10
run:
	./ZelecnicniStanice f f f f 20 f f f 1 experiment1.out			#Experiment1, zda-li nadrazi zvlada bezny provoz bez velkych spozdeni.
	
	./ZelecnicniStanice f 30 60 15 20 f f f 1 experiment2.out		        	#Experiment2, pridat vlakum nejake vyssi spozdeni, pozorovat jestli nadrazi stale zvlada a jaky to ma vliv na nadrazi

	./ZelecnicniStanice f f f f 20 3 2 10 1 experiment3_bez_spozdeni.out		#Experiment3, pridani vlaku (simulace svatecnich posil) a pozorovani vliv na nadrazi, bez velkych spozdeni
	
	./ZelecnicniStanice f 30 60 15 20 3 2 10 1 experiment3_spozdeni.out			#Experiment3, pridani vlaku (simulace svatecnich posil) a pozorovani vliv na nadrazi, s velkym spozdenim

	./ZelecnicniStanice 7 30 60 15 20 3 2 10 1 experiment4.out		        	#Experiment4, pridani vlaku (simulace svatecnich posil) a pozorovani vliv na nadrazi, se spozdenim, po pridani jednoho nastupiste
	
	
	

	
clean:
	rm ZelecnicniStanice *.out
