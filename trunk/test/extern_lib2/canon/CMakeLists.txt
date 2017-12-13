##START:version
#Butter version 1.2beta.175
##END:version
##START:date
#Tue Dec 12 00:00:07 2017
##END:date
##START:preamble
cmake_minimum_required(VERSION 2.6)

project (EXTERN_LIB)

##END:preamble
##START:target EXTERN_LIB
include (${CMAKE_SOURCE_DIR}/local.cmake)

##END:target EXTERN_LIB
##START:target program
set (__PROGRAM program)

set (__PROGRAM_SRC 
	example_exe.cc)
add_executable (${__PROGRAM}
	${__PROGRAM_SRC})
target_link_libraries (${__PROGRAM}
	${QT_LINK_LIBS}
	-L${QTDIR}/lib64 -lqt-mt)
set_target_properties(${__PROGRAM}
	PROPERTIES
	COMPILE_FLAGS -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT)

include_directories (${QTDIR}/include )
install (TARGETS ${__PROGRAM}
	RUNTIME DESTINATION ${BINDIR}
	CONFIGURATIONS Release)

##END:target program
