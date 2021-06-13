#!/bin/sh
# run test_suite.py
#
SCRIPT=test_suite.py
for dir_ in . /tmp/test_suite ; do
  if [ -f "${dir_}/${SCRIPT}" ]; then
    SCRIPT=${dir_}/${SCRIPT}
    break
  fi
done
DATESTAMP=`date +%m%d%H%M`
if [ "X--file" = "X$1" ] ; then
python ${SCRIPT} $* > out${DATESTAMP}$$.html
else
python ${SCRIPT} --file test.xml $* > out${DATESTAMP}$$.html
fi
