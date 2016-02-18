
PRJ=IFJ/IAL
#
.PHONY: clean

PROJECT = projekt
SOURCES = main.c scanner.c parser.c keywords.c precedence.c ial.c instruction_list.c interpret.c int_valuetable.c
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -g

all: $(SOURCES)
	$(CC) $(CFLAGS) -o $(PROJECT) $(SOURCES)

clean:
	rm -f *~ *.o ./tests/*.real $(PROJECT)
#

TESTFILES=$(patsubst tests/%.pas, tests/%, $(shell ls tests/*.pas));


tests: $(PROJECT) $(TESTFILES)

tests/%: tests/%.pas
	-$(shell ./projekt $< > $@.stdout.real 2> $@.stderr.real < $@.stdin;echo "$$?" > $@.exit.real;diff $@.stdout.real $@.stdout.correct > /dev/null;if [ "$$?" = "0" ];  then  diff $@.exit.real $@.exit.correct > /dev/null;  if [ "$$?" = "0" ];   then    echo "# OK ... $@";   rm $@.stdout.real $@.stderr.real $@.exit.real;  else    echo "# ERROR ... $@";  fi;  else echo "# ERROR ... $@";fi)
