#!/bin/bash
#Authors Martin Kacmarcik a Martin Kocour

DIRECTORY="images"

if ! [ -d "$DIRECTORY" ]; then
	wget http://www.maksp.xf.cz/images.zip
	unzip images.zip -d ./
	rm images.zip
fi

ulimit -v unlimited
