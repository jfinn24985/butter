T2T=txt2tags
T2TFLAGS=-toc -t html -o
.html.t2t :
	$(T2T) $(T2TFLAGS) $< $>
 