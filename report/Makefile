# The main latex-file
TEXFILE = main

# Fix reference file and compile source
default: full

full:
	#latexmk -latexoption='-interaction nonstopmode' -use-make -pdf ${TEXFILE}
	pdflatex $(TEXFILE); \
	bibtex $(TEXFILE); \
	makeglossaries $(TEXFILE);\
	pdflatex $(TEXFILE);\
	pdflatex $(TEXFILE)


# Removes TeX-output files
clean:
	latexmk -CA
	rm -f *.aux $(TEXFILE).bbl $(TEXFILE).blg *.log *.out $(TEXFILE).toc $(TEXFILE).lot $(TEXFILE).lof $(TEXFILE).glg $(TEXFILE).glo $(TEXFILE).gls $(TEXFILE).ist $(TEXFILE).acn $(TEXFILE).acr $(TEXFILE).alg $(TEXFILE).xdy $(TEXFILE).loa
	rm -f figs/*-gv.pdf

figs/%-gv.pdf: figs/%.gv
	dot figs/$*.gv -Tpdf -ofigs/$*-gv.pdf
