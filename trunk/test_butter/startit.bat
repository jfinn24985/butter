@ECHO OFF
REM run test_suite.py
REM
set _time_=%TIME:.=%
set _timestamp_=%DATE:~4,2%%_time_:~0,2%%_time_:~3,2%%_time_:~6,4%
start /B /WAIT test_suite.py --file test.xml %* > "out%_timestamp_: =0%.html"

