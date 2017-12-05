T2T=txt2tags
T2TFLAGS?=
%.html : %.t2t
	$(T2T) $(T2TFLAGS) -t html -o $@ $<
%.man : %.t2t
	$(T2T) $(T2TFLAGS) -t man -o $@ $<
 