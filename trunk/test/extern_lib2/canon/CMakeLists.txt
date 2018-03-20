##START:version
#Butter version 1.2beta.175
##END:version
##START:date
#Tue Mar 20 21:27:39 2018
##END:date
##START:preamble
cmake_minimum_required(VERSION 2.6)

project (EXTERN_LIB2)

##END:preamble
##START:target EXTERN_LIB2
include (${CMAKE_SOURCE_DIR}/local.cmake)

##END:target EXTERN_LIB2

include (${CMAKE_SOURCE_DIR}/qt.cmake)

##START:target program
set (__PROGRAM program)

set (__PROGRAM_SRC 
	example_exe.cc)
add_executable (${__PROGRAM}
	${__PROGRAM_SRC})
target_link_libraries (${__PROGRAM}
	${QT_LINK_LIBS}
	${QTLIB})
set_target_properties(${__PROGRAM}
	PROPERTIES
	COMPILE_FLAGS ${QTFLAGS})
install (TARGETS ${__PROGRAM}
	RUNTIME DESTINATION ${BINDIR}
	CONFIGURATIONS Release)

##END:target program
