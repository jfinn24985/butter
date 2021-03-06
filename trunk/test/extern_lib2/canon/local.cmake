#
# local.cmake
#
set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG DEBUG=1)
set_directory_properties(PROPERTIES COMPILE_DEFINITIONS DEBUG=0)
#############################
##  Default install locations
#############################
set (CMAKE_INSTALL_PREFIX installdir)
set (BINDIR bin)
set (DATADIR share)
set (DOCDIR share/doc)
set (HTMLDIR share/html)
set (INCLUDEDIR include)
set (LIBDIR lib)
set (MANDIR share/man)

