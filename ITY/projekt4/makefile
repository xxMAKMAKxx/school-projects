proj=proj4
all:	
	latex $(proj).tex
	bibtex $(proj)
	latex $(proj).tex
	latex $(proj).tex
	dvips $(proj).dvi
	ps2pdf -sPAPERSIZE=a4 $(proj).ps
