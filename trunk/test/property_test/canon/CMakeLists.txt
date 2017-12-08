##START:version
#Butter version 1.2beta.175
##END:version
##START:date
#Fri Dec 8 16:08:53 2017
##END:date
##START:preamble
cmake_minimum_required(VERSION 2.6)

project (PROPERTY_TEST)

##END:preamble
##START:target PROPERTY_TEST
include (${CMAKE_SOURCE_DIR}/local.cmake)


include_directories (${CMAKE_SOURCE_DIR}/include )
##END:target PROPERTY_TEST
##START:close
add_subdirectory (src/Library)
add_subdirectory (src/Executable)
##END:close
