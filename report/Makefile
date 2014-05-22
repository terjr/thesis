# The main latex-file
TEXFILE = main
DOTFILES=$(subst .gv,-gv.pdf, $(wildcard figs/*.gv))
GAFILES=$(wildcard ~/ga-results.*)
TRAINING_FIGS=figs/training/add-add.pdf figs/training/pi-pi.pdf figs/training/sha2-sha2.pdf figs/training/trend-trend.pdf figs/training/trend-submul.pdf figs/training/dhrystone-dhry.pdf
SVG_FIGS=figs/ga_genome.pdf

# Fix reference file and compile source
default: full

full: ${DOTFILES} ${TRAINING_FIGS} ${SVG_FIGS}
	#latexmk -latexoption='-interaction nonstopmode -shell-escape' -use-make -pdf ${TEXFILE}
	pdflatex -shell-escape $(TEXFILE) && \
	bibtex $(TEXFILE) && \
	makeglossaries $(TEXFILE) && \
	pdflatex -shell-escape $(TEXFILE) && \
	pdflatex -shell-escape $(TEXFILE)


# Removes TeX-output files
clean:
	latexmk -CA
	rm -f *.aux $(TEXFILE).bbl $(TEXFILE).blg *.log *.out $(TEXFILE).toc $(TEXFILE).lot $(TEXFILE).lof $(TEXFILE).glg $(TEXFILE).glo $(TEXFILE).gls $(TEXFILE).ist $(TEXFILE).acn $(TEXFILE).acr $(TEXFILE).alg $(TEXFILE).xdy $(TEXFILE).loa
	rm -f figs/*-gv.pdf
	rm -f figs/training/*.pdf
	rm -f figs/ga_genome.pdf*
	rm -r figs/*ps-converted-to.pdf

%.pdf: %.svg
	inkscape -D -z --file=$< --export-pdf=$@ --export-text-to-path

figs/%-gv.pdf: figs/%.gv
	dot figs/$*.gv -Tpdf -ofigs/$*-gv.pdf

figs/training/%.pdf: figs/training/%-training.txt gnuplot_power.sh
	./gnuplot_power.sh -y '200:340' -n $* -f figs/training -r ../powerlogs/root/m5bins/$(subst -,/,$*)/pet-log-cut -t $*-training.txt -g xy -o eps > $@

figs/training/%-training.txt: ../workloads/m5out-stable/%/trace.out ../pet/weights.conf ../pet/pet ../pet/weights.conf
	mkdir -p figs/training
	./apply_pet.sh $* > $@
	#../pet/pet -w ../pet/weights.conf -o $@ -f plain -b $$(wc -l ../powerlogs/root/m5bins/$(subst -,/,$*)/pet-log-cut | awk '{print $$1}' ) -i ../workloads/m5out-stable/$*/trace.out -s -v

figs/training/%-noscale.pdf: figs/training/%-noscale-training.txt gnuplot_power.sh
	./gnuplot_power.sh -n $* -f figs/training -r ../powerlogs/root/m5bins/$(subst -,/,$*)/pet-log-cut -t $*-noscale-training.txt -g xy -o eps > $@

figs/training/%-noscale-training.txt: ../workloads/m5out-stable/%/trace.out ../pet/weights.conf ../pet/pet ../pet/weights.conf
	mkdir -p figs/training
	../pet/pet -w ../pet/weights.conf -o $@ -f plain -B 100000000 -i ../workloads/m5out-stable/$*/trace.out -s -v

../pet/weights.conf: ${GAFILES}
	../pet/scripts/update_weights.sh > $@
