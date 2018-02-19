all: manual

manual: modules
	pdflatex manual.tex

modules: $(wildcard inc/modules/*.tex)
	ls inc/modules/*.tex | awk '{printf "\\input{%s}\n", $$1}' > inc/modules.tex

example:
	pdflatex example.tex

clean:
	find . -regextype posix-awk -regex "(.*.aux|.*.out|.*.pdf|.*.log|.*.cmx|.*.enx|.*.exx|.*.idx|.*.toc)" -type f -delete
