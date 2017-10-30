# Extra stuff I found I needed for compiling under macos/qt3 32 bit
find_library(Qt3 REQUIRED)
add_definitions(${QT_DEFINITIONS}) 
include_directories(${QT_INCLUDE_DIR} /usr/local/qt3/include)
link_directories(${QT_LIBRARY_DIR} /usr/local/qt3/lib)
set( CMAKE_EXE_LINKER_FLAGS "-m32 ${CMAKE_EXE_LINKER_FLAGS}") 
add_definitions (-m32)
