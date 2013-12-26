all : quilt.pdf

quilt.ps : quilt.dvi
	dvips -t letter -o $@ $<

quilt.pdf: main.tex
	pdflatex --jobname=quilt -t letter $<

quilt.dvi : main.tex
	latex $< && \
	mv main.dvi $@

clean:
	rm -f main.{dvi,log,aux}

distclean: clean
	rm -f quilt.{ps,pdf,dvi}
