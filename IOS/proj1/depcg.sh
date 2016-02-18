#!/bin/bash
#Spuštění: depcg.sh [-g] [-p] [-r FUNCTION_ID|-d FUNCTION_ID] FILE
#skript napsal Martin Kacmarcik - xkacma03 - pro FIT VUTBR. Vsechna prava vyhrazena :D. 


TEMP=`mktemp /tmp/$USER.XXXXXX`  
#sekce slouzici pro deklaraci promennych (nepovinne, ale pro prehlednost vhodne)
prepinacP=0
prepinacR=0
prepinacD=0
muzesProjit=1
parametrD=
parametrR=
prepinacG=0
#zde zacina kod programu
while getopts gpr:d: ridici #funkce getops pro jednoduche zpracovani argumentu prikazove radky
do	case "$ridici" in
	g)	prepinacG=1;;
	p)	prepinacP=1;;
	d)	prepinacD=1; parametrD=$OPTARG ;;
	r)	prepinacR=1; parametrR=$OPTARG ;;
	esac
done

((OPTIND--))
shift $OPTIND
FILE=$*; #v promene FILE mam nazev souboru pro objdump 

for i in `objdump -d -j .text $FILE | cut -c 18- | grep -e '<.*>:' -e'callq' | grep '<.*>' | sed 's/.*<//;s/>//'` #vhodne upraveny vystup pro dalsi zpracovani	
do
if [[ "$i" =~ (.*)":"$ ]] #nalezl jsi funkce: ? Pokud ano jedna se o CALLERa
then
	if [[ $prepinacD = 1 ]]; #pokud je zadan prepinac -d porovnej, jestli caller je roven parametru prepinace -d od uzivatele
	then 
		if [[ "`echo $parametrD`:" = $i ]]
		then 
			muzesProjit=1 #pokud ano muzes zpracovavat
			CALLER=$i
		else
			muzesProjit=0 #pokud ne nezpracovavej, uzivatel to nechce
			continue
		fi
	else
		CALLER=$i #jestli je prepinac vyplej uloz si callera
	fi
else
	if [[ $prepinacD = 1 ]]; #jestlize je -d zaple, zkontroluj, jestli muzes prochazet a tudiz je caller roven parametru -dcka
	then
		if [[ $muzesProjit = 1 ]];
		then echo $CALLER" -> "$i >>$TEMP #pokud jo vypis zavislost
		else continue
		fi
	elif [[ $prepinacR = 1 ]]; #pokud je prepinac -r zaplej zkontroluj jestli CALLEE = parametru prepinace -r
	then
		if [[ "`echo $parametrR`" = $i ]]
		then
			echo $CALLER" -> "$i >>$TEMP #pokud ano vypis zavislost
		else
			continue #pokud ne proste jdi na noveho CALLEE
		fi
	else
		echo $CALLER" -> "$i >>$TEMP #pokud neni nic vyple proste jen vypis zavislost
	fi
fi
done

if [[ $prepinacP = 0 ]]; #je zaplej prepinac -p? Pokud ano na konci se zbav vsech zavislosti se zavinacem
then 
	if [[ $prepinacG = 0 ]] #je zaplej prepinac -g ? Pokud ano tak dodelej header a tail
	then
		sort -u $TEMP | sed 's/://;s/+.*//' | grep -v '@' | sort -u
	else
		echo "digraph CG {"
		sort -u $TEMP | cat $TEMP | sed 's/://;s/+.*//;s/$/;/'  | grep -v '@' 
		echo "}"
	fi
else
	if [[ $prepinacG = 0 ]]  #pokud je prepinac -p zaplej nech zavislosti se zavinacem 
	then
		cat $TEMP | sed 's/://;s/+.*//' | sort -u
	else
		echo "digraph CG {"
		cat $TEMP | sed 's/://;s/+.*//;s/@plt/_PLT/;s/$/;/' | sort -u #a v pripade -g prepinace je jeste nahrad za _PLT
		echo "}"
	fi

fi


trap "rm $TEMP" SIGHUP SIGTERM SIGINT  #smaz docasnej soubor
#konec
