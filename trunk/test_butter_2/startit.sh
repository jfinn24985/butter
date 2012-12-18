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
python ${SCRIPT} --file test.xml $* > out${DATESTAMP}$$.html

