#!/bin/bash
#Spuštění: depsym.sh [-g] [-r|-d OBJECT_ID] FILEs
#________________________________________________________________#
#zacatek
TEMP=`mktemp /tmp/$USER.XXXXXX`
#promene
ridiciPromena=0
ridiciPromena2=0
zavisla=
nakom=
cim=
pomocneCim=
parametrG=0
prepinacR=
parametrR=
prepinacD=
parametrD=
muzesProchazet=1
#telo skriptu

while getopts gr:d: ridici #funkce getops pro nastaveni parametru
do	case "$ridici" in
	g)	parametrG=1;;
	r)	parametrR=$OPTARG;prepinacR=1;;
	d)	parametrD=$OPTARG;prepinacD=1;;
	esac
done
 
((OPTIND--))
shift $OPTIND
FILES=$*; #v promene FILES mam vsechny fily ktere chce uzivatel porovnavat

if [[ $parametrG = 1 ]] #pokud je gcko zaple, udelej hlavicku
then
echo "digraph GSYM {"
fi

nm $FILES | grep -v '^$' | sed 's/ *//' | sed -r -e 's/^(.*)(T|B|C|D|G) (.*)$/\2 \3/g' >>$TEMP #do tempu si uprav vystup z programu nm

for i in `cat $TEMP` #projed temp radek po radku
do
	if [[ "$i" =~ (.*)":"$ ]] #pokud naleznes identifikator s dvojteckou uloz si ho jako zavislaka
	then
		zavisla=$i
		if [[ $prepinacD == 1 ]] #jestlize je prepinac D aktivni zkontroluj jestli se zavislak shoduje s parametrem D
		then
			if [[ `basename $i` =~ $parametrD":" ]]
			then
				muzesProchazet=1 #pokud ano muzes pokracovat
			else
				muzesProchazet=0 #pokud ne dal nepokracuj uzivatel to nechce
			fi
		fi
	fi
if [[ $muzesProchazet = 1 ]] #muzu projit?
then
	if [[ $ridiciPromena = 1 ]] #pokud jsi v predchozi iteraci nalezl symbol U dalsi bude nasledovat nazev zavisejici funkce, zpracuj ji
	then
		cim=$i #uloz si tu funkci
		ridciPromena=0 #resetuj ridiciPromenou
		for q in `cat $TEMP` #znova projdi temp 
		do
			if [[ "$q" =~ (.*)":"$ ]] #pokud najdes nazev programu (dvojtecka na konci)
			then
				nakom=$q #uloz si ho pro pozdejsi pouziti
			fi

			if [[ $ridiciPromena2 = 1 ]] #pokud jsi v predchozi iteraci nasel symbol oznacujici, ze zde funkce vzinkla tak pokracuj
			then
				pomocneCim=$q #uloz si co si nasel
				#echo Toto je echo po T a nasledujici hodnota je: $q
				if [[ $pomocneCim = $cim ]] #porovnej funkce, pokud jsou stejne muzes vypsat zavislosti
				then
					if [[ $prepinacR = 1 ]] #pokud je prepinac R zaplej udelej jeste filtr 
					then 
						if [[ `basename $nakom` = `echo $parametrR:` ]] #pokud je nakom shodne s parametrem Rka muzes vypsat
						then
							if [[ $parametrG = 1 ]] #ocekuj si jestli tam je parametr G, ten ma specialni pozadavky
							then
								echo `basename $zavisla`" -> "`basename $nakom`" [label=\""$cim"\"];" | sed 's/://;s/: / /;s/-/_/g;s/\./D/g;s/+/P/g;s/_>/->/g'						
							else #kdyz ne vypis obyc zavislost
								echo $zavisla" -> "$nakom" ("$cim")" | sed 's/://;s/: / /'
							fi
						fi
					else #pokud neni zaplej normalne vypis zavislosti
						if [[ $parametrG = 1 ]] #zase si pohlidej gcko
						then
							echo `basename $zavisla`" -> "`basename $nakom`" [label=\""$cim"\"];" | sed 's/://;s/: / /;s/-/_/g;s/\./D/g;s/+/P/g;s/_>/->/g'						
						else
							echo $zavisla" -> "$nakom" ("$cim")" | sed 's/://;s/: / /'
						fi
					fi
					pomocneCim= #nastav vsechny pomocne promene na prazdny retezec, aby nedoslo k zamenene
					cim=
					nakom=
				fi
				ridiciPromena2=0 #resetuj ridici promenou
			fi
			if [[ "$nakom" != "$zavisla" ]]
			then
				if [[ "$q" = T ]] || [[ "$q" = B ]] || [[ "$q" = C ]] || [[ "$q" = D ]] || [[ "$q" = G ]] #cekni jestli nahodou nenajdes symbol T nebo B nebo C nebo D nebo G, protoze pokud ano nasel si vznik funkce a muzes vypsat zavislost
				then
					ridiciPromena2=1	#nastav ridici promenou2 na 1 aby mohlo dojit k porovnani a vypsani zavislosti
				fi
			fi
		done
		continue
	fi

	if [[ "$i" = U ]] #mas symbol U ? Pokud ano v dalsi iteraci te ceka zavisla funkce
	then
		ridiciPromena=1
	fi
fi
done

if [[ $parametrG = 1 ]] #vypis tail graphical vystupu pri prepinace -g
then
echo "}"
fi


#nad temito radky se nachazi vystup




#konec skriptu
trap "rm $TEMP" SIGHUP SIGTERM SIGINT  # TEMP musi zemrit. 
#konec
#________________________________________________________________#
