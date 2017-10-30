set (T2T txt2tags)
macro (add_t2t_target outfile infile)
  if (outfile MATCHES ".*\.man")
    set (T2TFLAGS -t man)
  else ()
    set (T2TFLAGS -toc -t html)
  endif (outfile MATCHES ".*\.man")
  add_custom_command (OUTPUT ${outfile}
    COMMAND ${T2T}
    ARGS ${T2TFLAGS} -o ${outfile} ${infile}
    DEPENDS ${infile})
endmacro (add_t2t_target)

 