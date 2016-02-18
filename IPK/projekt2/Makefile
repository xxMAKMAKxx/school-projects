# Makefile pro 2. projekt do IPK
# by Martin Kacmarcik - xkacm03
# FIT VUTBR

SERVER = ipkhttpserver
CLIENT = ipkhttpclient
AUTHOR = xkacma03
INTERPRET = python3
FILES = $(SERVER) $(CLIENT) testScript Makefile www test_folder README

.PHONY: clean test pack

all: 
	chmod +x $(CLIENT)
	chmod +x $(SERVER)

clean: 
	rm -rf *.header *.log *.payload 
	
test:
	chmod +x testScript
	./testScript
	
pack: $(FILES) clean
	tar -cvzf $(AUTHOR).tgz $(FILES)

	