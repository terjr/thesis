# The main latex-file
TEXFILE = main
DOTFILES=$(subst .gv,-gv.pdf, $(wildcard figs/*.gv))

# Fix reference file and compile source
default: full

full: ${DOTFILES}
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
	rm -f figs/training/*.pdf

figs/%-gv.pdf: figs/%.gv
	dot figs/$*.gv -Tpdf -ofigs/$*-gv.pdf

figs/training/%.pdf: figs/training/%-training.txt
	./gnuplot_power.sh -n $* -f figs/training -r ../powerlogs/root/m5bins/$(subst -,/,$*)/pet-log-cut -t $*-training.txt -g xy -o eps > $@

figs/training/%-training.txt: ../workloads/m5out-stable/%/trace.out ../pet/weights.conf ../pet/pet
	mkdir -p figs/training
	../pet/pet -w ../pet/weights.conf -o $@ -f plain -b $$(wc -l ../powerlogs/root/m5bins/$(subst -,/,$*)/pet-log-cut | awk '{print $$1}' ) -i ../workloads/m5out-stable/$*/trace.out -s -v

figs/training/%-noscale.pdf: figs/training/%-noscale-training.txt
	./gnuplot_power.sh -n $* -f figs/training -r ../powerlogs/root/m5bins/$(subst -,/,$*)/pet-log-cut -t $*-noscale-training.txt -g xy -o eps > $@

figs/training/%-noscale-training.txt: ../workloads/m5out-stable/%/trace.out ../pet/weights.conf ../pet/pet
	mkdir -p figs/training
	../pet/pet -w ../pet/weights.conf -o $@ -f plain -B 100000000 -i ../workloads/m5out-stable/$*/trace.out -s -v
