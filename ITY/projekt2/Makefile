projekt=proj2
all:	
	latex $(projekt).tex
	latex $(projekt).tex
	dvips -t a4 $(projekt).dvi
	gs -sDEVICE=pdfwrite -o $(projekt).pdf $(projekt).ps

clean:
	rm -f $(projekt).aux $(projekt).log $(projekt).ps $(projekt).dvi