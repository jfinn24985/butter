#!/home/finnerty/local/bin/atf-sh 

# Location of bouml standard installation.
BOUML_LOC=/usr/lib/bouml
BUTTER_EXE=../../source/src/butter/butter_exe

setup_example(){
  local example=$1
  local genr=$2
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file ${example}.prj\n" patch <patch/${genr}.patch
  atf_check -s exit:0 -o save:butter.log.${genr}.canon bouml ${example}.prj -execnogui ${BUTTER_EXE} -test:ok -exit
  atf_check -s exit:0 -o file:cpp.log.canon bouml ${example}.prj -execnogui ${BOUML_LOC}/cpp_generator -exit
}

atf_test_case single_jam_gen
single_jam_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
single_jam_gen_body() {
  pushd ../test/single_test
  #----------------------- 
  # Standard (jam) variant 
  #-----------------------
  setup_example "single_test" "jam"

  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamfile output/Jamfile.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamrules output/Jamrules.canon

  build_single_test(){
    local variant=$1
    local builddir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/test ]
    atf_check -s exit:0 [ -e ${builddir}/single_test.o ]
    atf_check -s exit:0 -o file:output.canon ${builddir}/test
    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/test ]
    atf_check -s exit:0 -o file:output.canon installdir/bin/test
    # no distclean target
    # test clean target
    atf_check -s exit:0 -o save:jam3.log -e save:jam3.err jam clean ${variant}
    atf_check -s exit:0 [ ! -e ${builddir}/test ]
    atf_check -s exit:0 [ ! -e ${builddir}/single_test.o ]
    atf_check -s exit:0 [ -e installdir/bin/test ]

    atf_check -s exit:0 -o empty rm jam1.log jam1.err
    atf_check -s exit:0 -o empty rm jam2.log jam2.err
    atf_check -s exit:0 -o empty rm jam3.log jam3.err
    atf_check -s exit:0 -o empty rm -rf ${builddir}
    atf_check -s exit:0 -o empty rm -rf installdir
    popd
  }

  # default (DEBUG) VARIANT
  build_single_test "" DEBUG
  # specific DEBUG VARIANT
  build_single_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  build_single_test -sVARIANT=RELEASE RELEASE
  
  # remove source
  pushd output
  atf_check -s exit:0 -o empty rm single_test.cc single_test.hh
  popd

  # Clean up
  atf_check -s exit:0 -o empty rm output/Jamfile output/Jamrules
  atf_check -s exit:0 -o empty rm -f output/butter.log
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_single_test
}

atf_test_case single_boost_gen
single_boost_gen_head() {
  atf_set "descr" "Test Boost Jam generator on single directory project."
}
single_boost_gen_body() {
  pushd ../test/single_test
  #-------------- 
  # Boost variant 
  #-------------- 
  setup_example "single_test" "boost"

  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamroot output/Jamroot.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/local.jam output/local.jam.canon

  build_single_test(){
    local variant=$1
    local builddir=$2
    local installdir=$3
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:build1.log -e save:build1.err bjam ${variant}
    atf_check -s exit:0 [ -x build/${builddir}/test ]
    atf_check -s exit:0 [ -e build/${builddir}/single_test.o ]
    atf_check -s exit:0 -o file:output.canon build/${builddir}/test
    # no separate install target
    atf_check -s exit:0 [ -x ${installdir}/test ]
    atf_check -s exit:0 -o file:output.canon ${installdir}/test
    # no distclean target
    # test clean target
    atf_check -s exit:0 -o save:build2.log -e save:build2.err bjam clean ${variant}
    atf_check -s exit:0 [ ! -e build/${builddir}/test ]
    atf_check -s exit:0 [ ! -e build/${builddir}/single_test.o ]
    atf_check -s exit:0 [ ! -e ${installdir}/test ]

    atf_check -s exit:0 -o empty rm build1.log build1.err
    atf_check -s exit:0 -o empty rm build2.log build2.err
    atf_check -s exit:0 -o empty rm -rf build
    atf_check -s exit:0 -o empty rm -rf ${installdir}
    if [ `basename ${installdir}` != ${installdir} ]; then
       echo "deleting " `dirname ${installdir}`
       atf_check -s exit:0 -o empty rm -rf `dirname ${installdir}`
    fi
    popd
  }

  # default (DEBUG) VARIANT
  build_single_test "" "*/debug" install_test
  # specific DEBUG VARIANT
  build_single_test debug "*/debug" install_test
  # RELEASE VARIANT
  build_single_test release "*/release" installdir/bin
   
  # remove source
  pushd output
  atf_check -s exit:0 -o empty rm single_test.cc single_test.hh
  popd

  # Clean up
  atf_check -s exit:0 -o empty rm output/local.jam output/Jamroot
  atf_check -s exit:0 -o empty rm -f output/butter.log
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_single_test
}

atf_test_case single_cmake_gen
single_cmake_gen_head() {
  atf_set "descr" "Test CMake generator on single directory project."
}
single_cmake_gen_body() {
  pushd ../test/single_test
  #-------------- 
  # CMake variant 
  #-------------- 
  setup_example "single_test" "cmake"

  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/CMakeLists.txt output/CMakeLists.txt.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/local.cmake output/local.cmake.canon

  build_single_test(){
    local variant=$1
    local installdir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:build1.log -e save:build1.err cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=${variant} --build .
    atf_check -s exit:0 -o save:build2.log -e save:build2.err make -f Makefile VERBOSE=1
    atf_check -s exit:0 [ -x test ]
    atf_check -s exit:0 [ -e CMakeFiles/test.dir/single_test.cc.o ]
    atf_check -s exit:0 -o file:output.canon ./test
    # test install target
    atf_check -s exit:0 -o save:build2.log -e save:build2.err make -f Makefile install VERBOSE=1
    if [ "X" != "X${installdir}" ]; then
      atf_check -s exit:0 [ -e install_manifest.txt ]
      atf_check -s exit:0 [ -s install_manifest.txt ]
      atf_check -s exit:0 [ -e ${installdir}/bin/test ]
      atf_check -s exit:0 -o file:output.canon ${installdir}/bin/test
    else
      atf_check -s exit:0 [ -e install_manifest.txt ]
      atf_check -s exit:0 [ ! -s install_manifest.txt ]
    fi
    # no distclean target
    # test clean target
    atf_check -s exit:0 -o save:build3.log -e save:build3.err make -f Makefile clean VERBOSE=1
    atf_check -s exit:0 [ ! -e ./test ]
    atf_check -s exit:0 [ ! -e CMakeFiles/test.dir/single_test.o ]
    if [ "X" != "X${installdir}" ]; then
      atf_check -s exit:0 [ -e ${installdir}/bin/test ]
    fi

    # cleanup
    atf_check -s exit:0 -o empty rm build1.log build1.err
    atf_check -s exit:0 -o empty rm build2.log build2.err
    atf_check -s exit:0 -o empty rm build3.log build3.err
    atf_check -s exit:0 -o empty rm CMakeCache.txt Makefile cmake_install.cmake install_manifest.txt

    atf_check -s exit:0 -o empty rm -rf CMakeFiles
    atf_check -s exit:0 -o empty rm -rf ${installdir}
    popd
  }

  # default (DEBUG) VARIANT
  build_single_test "" ""
  # specific DEBUG VARIANT
  build_single_test Debug ""
  # RELEASE VARIANT
  build_single_test Release "installdir"
 
  # remove source
  pushd output
  atf_check -s exit:0 -o empty rm single_test.cc single_test.hh
  popd

  # Clean up
  atf_check -s exit:0 -o empty rm output/CMakeLists.txt output/local.cmake
  atf_check -s exit:0 -o empty rm -f output/butter.log
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_single_test
}

atf_test_case single_make_gen
single_make_gen_head() {
  atf_set "descr" "Test Makefile generator on single directory project."
}
single_make_gen_body() {
  pushd ../test/single_test
  #-------------- 
  # Make variant 
  #-------------- 
  setup_example "single_test" "make"

  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/makefile output/makefile.canon
  atf_check -o empty diff output/M_sys.mk output/M_sys.mk.canon
  atf_check -o empty diff output/M_cl.mk output/M_cl.mk.canon
  atf_check -o empty diff output/M_gcc.mk output/M_gcc.mk.canon
  atf_check -o empty diff output/M_unix.mk output/M_unix.mk.canon
  atf_check -o empty diff output/M_Windows_NT.mk output/M_Windows_NT.mk.canon
 
  build_single_test(){
    local variant=$1
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:build1.log -e save:build1.err make -k VARIANT=${variant}
    atf_check -s exit:0 [ -x test ]
    atf_check -s exit:0 [ -e single_test.o ]
    atf_check -s exit:0 [ ! -e installdir/bin/test ]
    atf_check -s exit:0 -o file:output.canon ./test
    # test clean target
    atf_check -s exit:0 -o save:build2.log -e save:build2.err make clean
    atf_check -s exit:0 [ -e ./test ]
    atf_check -s exit:0 [ ! -e single_test.o ]
    atf_check -s exit:0 [ ! -e installdir/bin/test ]

    # test install target
    atf_check -s exit:0 -o save:build3.log -e save:build3.err make install
    atf_check -s exit:0 [ -x test ]
    atf_check -s exit:0 [ -e single_test.o ]
    atf_check -s exit:0 [ -e installdir/bin/test ]
    atf_check -s exit:0 -o file:output.canon installdir/bin/test
    # test diszclean target
    atf_check -s exit:0 -o save:build4.log -e save:build4.err make distclean
    atf_check -s exit:0 [ ! -e ./test ]
    atf_check -s exit:0 [ ! -e single_test.o ]
    atf_check -s exit:0 [ -e installdir/bin/test ]

    # cleanup
    atf_check -s exit:0 -o empty rm build1.log build1.err
    atf_check -s exit:0 -o empty rm build2.log build2.err
    atf_check -s exit:0 -o empty rm build3.log build3.err
    atf_check -s exit:0 -o empty rm build4.log build4.err
    atf_check -s exit:0 -o empty rm -rf installdir
    popd
  }

  # default (DEBUG) VARIANT
  build_single_test ""
  # specific DEBUG VARIANT
  build_single_test DEBUG
  # RELEASE VARIANT
  build_single_test RELEASE

  # remove source
  pushd output
  atf_check -s exit:0 -o empty rm single_test.cc single_test.hh
  popd

  # Clean up
  atf_check -s exit:0 -o empty rm output/makefile output/M_sys.mk output/M_cl.mk output/M_gcc.mk output/M_unix.mk output/M_Windows_NT.mk
  atf_check -s exit:0 -o empty rm -f output/butter.log
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .

  popd
  unset build_single_test
}

atf_test_case multidir_jam_gen
multidir_jam_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
multidir_jam_gen_body() {
  pushd ../test/multidir
  #----------------------- 
  # Standard (jam) variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file multidir.prj\n" patch <patch/jam.patch
  atf_check -s exit:0 -o empty bouml multidir.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamfile canon.jam/Jamfile.canon
  atf_check -o empty diff output/Jamrules canon.jam/Jamrules.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Executable/Jamfile canon.jam/src/Executable/Jamfile.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Library/Jamfile canon.jam/src/Library/Jamfile.canon
  atf_check -s exit:0 -o empty rm output/Jamfile output/Jamrules
  atf_check -s exit:0 -o empty rm -rf output/src output/include
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case multidir_boost_gen
multidir_boost_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
multidir_boost_gen_body() {
  pushd ../test/multidir
  #----------------------- 
  # Boost (bjam) variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file multidir.prj\n" patch <patch/boost.patch
  atf_check -s exit:0 -o empty bouml multidir.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamroot canon.bjam/Jamroot.canon
  atf_check -o empty diff output/local.jam canon.bjam/local.jam.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Executable/Jamfile canon.bjam/src/Executable/Jamfile.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Library/Jamfile canon.bjam/src/Library/Jamfile.canon
  atf_check -s exit:0 -o empty rm output/Jamroot output/local.jam
  atf_check -s exit:0 -o empty rm -rf output/src output/include
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case multidir_cmake_gen
multidir_cmake_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
multidir_cmake_gen_body() {
  pushd ../test/multidir
  #----------------------- 
  # CMake variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file multidir.prj\n" patch <patch/cmake.patch
  atf_check -s exit:0 -o empty bouml multidir.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/CMakeLists.txt canon.cmake/CMakeLists.txt.canon
  atf_check -o empty diff output/local.cmake canon.cmake/local.cmake.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Executable/CMakeLists.txt canon.cmake/src/Executable/CMakeLists.txt.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Library/CMakeLists.txt canon.cmake/src/Library/CMakeLists.txt.canon
  atf_check -s exit:0 -o empty rm output/CMakeLists.txt output/local.cmake
  atf_check -s exit:0 -o empty rm -rf output/src output/include
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case multidir_make_gen
multidir_make_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
multidir_make_gen_body() {
  pushd ../test/multidir
  #----------------------- 
  # Make variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file multidir.prj\n" patch <patch/make.patch
  atf_check -s exit:0 -o empty bouml multidir.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/makefile canon.make/makefile.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Executable/makefile canon.make/src/Executable/makefile.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Library/makefile canon.make/src/Library/makefile.canon
  atf_check -o empty diff output/M_sys.mk canon.make/M_sys.mk.canon
  atf_check -o empty diff output/M_cl.mk canon.make/M_cl.mk.canon
  atf_check -o empty diff output/M_gcc.mk canon.make/M_gcc.mk.canon
  atf_check -o empty diff output/M_unix.mk canon.make/M_unix.mk.canon
  atf_check -o empty diff output/M_Windows_NT.mk canon.make/M_Windows_NT.mk.canon
  atf_check -s exit:0 -o empty rm output/makefile output/M_sys.mk output/M_cl.mk output/M_gcc.mk output/M_unix.mk output/M_Windows_NT.mk
  atf_check -s exit:0 -o empty rm -rf output/src output/include
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}



atf_test_case multilang_jam_gen
multilang_jam_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
multilang_jam_gen_body() {
  pushd ../test/multilang
  #----------------------- 
  # Standard (jam) variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file multilang.prj\n" patch <patch/jam.patch
  atf_check -s exit:0 -o empty bouml multilang.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamfile canon.jam/Jamfile.canon
  atf_check -o empty diff output/Jamrules canon.jam/Jamrules.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Executable/Jamfile canon.jam/src/Executable/Jamfile.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Library/Jamfile canon.jam/src/Library/Jamfile.canon
  atf_check -o empty diff output/src/Library/example_c.c canon.jam/src/Library/example_c.c.canon
  atf_check -s exit:0 -o empty rm output/Jamfile output/Jamrules
  atf_check -s exit:0 -o empty rm -rf output/src output/include
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case multilang_boost_gen
multilang_boost_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
multilang_boost_gen_body() {
  pushd ../test/multilang
  #----------------------- 
  # Boost (bjam) variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file multilang.prj\n" patch <patch/boost.patch
  atf_check -s exit:0 -o empty bouml multilang.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamroot canon.bjam/Jamroot.canon
  atf_check -o empty diff output/local.jam canon.bjam/local.jam.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Executable/Jamfile canon.bjam/src/Executable/Jamfile.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Library/Jamfile canon.bjam/src/Library/Jamfile.canon
  atf_check -o empty diff output/src/Library/example_c.c canon.bjam/src/Library/example_c.c.canon
  atf_check -s exit:0 -o empty rm output/Jamroot output/local.jam
  atf_check -s exit:0 -o empty rm -rf output/src output/include
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case multilang_cmake_gen
multilang_cmake_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
multilang_cmake_gen_body() {
  pushd ../test/multilang
  #----------------------- 
  # CMake variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file multilang.prj\n" patch <patch/cmake.patch
  atf_check -s exit:0 -o empty bouml multilang.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/CMakeLists.txt canon.cmake/CMakeLists.txt.canon
  atf_check -o empty diff output/local.cmake canon.cmake/local.cmake.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Executable/CMakeLists.txt canon.cmake/src/Executable/CMakeLists.txt.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Library/CMakeLists.txt canon.cmake/src/Library/CMakeLists.txt.canon
  atf_check -o empty diff output/src/Library/example_c.c canon.cmake/src/Library/example_c.c.canon
  atf_check -s exit:0 -o empty rm output/CMakeLists.txt output/local.cmake
  atf_check -s exit:0 -o empty rm -rf output/src output/include
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case multilang_make_gen
multilang_make_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
multilang_make_gen_body() {
  pushd ../test/multilang
  #----------------------- 
  # Make variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file multilang.prj\n" patch <patch/make.patch
  atf_check -s exit:0 -o empty bouml multilang.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/makefile canon.make/makefile.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Executable/makefile canon.make/src/Executable/makefile.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Library/makefile canon.make/src/Library/makefile.canon
  atf_check -o empty diff output/src/Library/example_c.c canon.make/src/Library/example_c.c.canon
  atf_check -o empty diff output/M_sys.mk canon.make/M_sys.mk.canon
  atf_check -o empty diff output/M_cl.mk canon.make/M_cl.mk.canon
  atf_check -o empty diff output/M_gcc.mk canon.make/M_gcc.mk.canon
  atf_check -o empty diff output/M_unix.mk canon.make/M_unix.mk.canon
  atf_check -o empty diff output/M_Windows_NT.mk canon.make/M_Windows_NT.mk.canon
  atf_check -s exit:0 -o empty rm output/makefile output/M_sys.mk output/M_cl.mk output/M_gcc.mk output/M_unix.mk output/M_Windows_NT.mk
  atf_check -s exit:0 -o empty rm -rf output/src output/include
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}





atf_test_case multitarget_jam_gen
multitarget_jam_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
multitarget_jam_gen_body() {
  pushd ../test/multitarget
  #----------------------- 
  # Standard (jam) variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file multitarget.prj\n" patch <patch/jam.patch
  atf_check -s exit:0 -o empty bouml multitarget.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamfile output/Jamfile.canon
  atf_check -o empty diff output/Jamrules output/Jamrules.canon
  atf_check -s exit:0 -o empty rm output/Jamfile output/Jamrules
  atf_check -s exit:0 -o empty rm -f output/butter.log
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case multitarget_boost_gen
multitarget_boost_gen_head() {
  atf_set "descr" "Test Boost generator on single directory project."
}
multitarget_boost_gen_body() {
  pushd ../test/multitarget
  #----------------------- 
  # Boost variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file multitarget.prj\n" patch <patch/boost.patch
  atf_check -s exit:0 -o empty bouml multitarget.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamroot output/Jamroot.canon
  atf_check -o empty diff output/local.jam output/local.jam.canon
  atf_check -s exit:0 -o empty rm output/Jamroot output/local.jam
  atf_check -s exit:0 -o empty rm -f output/butter.log
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case multitarget_cmake_gen
multitarget_cmake_gen_head() {
  atf_set "descr" "Test CMake generator on single directory project."
}
multitarget_cmake_gen_body() {
  pushd ../test/multitarget
  #----------------------- 
  # CMake variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file multitarget.prj\n" patch <patch/cmake.patch
  atf_check -s exit:0 -o empty bouml multitarget.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/CMakeLists.txt output/CMakeLists.txt.canon
  atf_check -o empty diff output/local.cmake output/local.cmake.canon
  atf_check -s exit:0 -o empty rm output/CMakeLists.txt output/local.cmake
  atf_check -s exit:0 -o empty rm -f output/butter.log
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case multitarget_make_gen
multitarget_make_gen_head() {
  atf_set "descr" "Test Make generator on single directory project."
}
multitarget_make_gen_body() {
  pushd ../test/multitarget
  #----------------------- 
  # Make variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file multitarget.prj\n" patch <patch/make.patch
  atf_check -s exit:0 -o empty bouml multitarget.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/makefile output/makefile.canon
  atf_check -o empty diff output/M_sys.mk output/M_sys.mk.canon
  atf_check -o empty diff output/M_cl.mk output/M_cl.mk.canon
  atf_check -o empty diff output/M_gcc.mk output/M_gcc.mk.canon
  atf_check -o empty diff output/M_unix.mk output/M_unix.mk.canon
  atf_check -o empty diff output/M_Windows_NT.mk output/M_Windows_NT.mk.canon
  atf_check -s exit:0 -o empty rm output/makefile output/M_sys.mk output/M_cl.mk output/M_gcc.mk output/M_unix.mk output/M_Windows_NT.mk
  atf_check -s exit:0 -o empty rm -f output/butter.log
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}









atf_test_case proplang_jam_gen
proplang_jam_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
proplang_jam_gen_body() {
  pushd ../test/proplang
  #----------------------- 
  # Standard (jam) variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file proplang.prj\n" patch <patch/jam.patch
  atf_check -s exit:0 -o empty bouml proplang.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamfile output/Jamfile.canon
  atf_check -o empty diff output/Jamrules output/Jamrules.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Jamfile output/src/Jamfile.canon
  atf_check -o empty diff output/src/fortran.f output/src/fortran.f.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/doc/Jamfile output/doc/Jamfile.canon
  atf_check -o empty diff output/doc/program.t2t output/doc/program.t2t.canon
  atf_check -o empty diff output/Jamroot output/Jamroot.base.canon
  atf_check -o empty diff output/CMakeLists.txt output/CMakeLists.txt.base.canon
  atf_check -o empty diff output/t2t.bjam output/t2t.bjam.canon
  atf_check -o empty diff output/t2t.jam output/t2t.jam.canon
  atf_check -o empty diff output/t2t.cmake output/t2t.cmake.canon
  atf_check -o empty diff output/t2t.make output/t2t.make.canon
  atf_check -s exit:0 -o empty rm output/Jamfile output/Jamrules
  atf_check -s exit:0 -o empty rm output/src/Jamfile output/src/fortran.f
  atf_check -s exit:0 -o empty rm output/doc/Jamfile output/doc/program.t2t
  atf_check -s exit:0 -o empty rm output/CMakeLists.txt output/Jamroot
  atf_check -s exit:0 -o empty rm output/t2t.bjam output/t2t.cmake output/t2t.jam output/t2t.make
  atf_check -s exit:0 -o empty rm -rf output/include
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case proplang_boost_gen
proplang_boost_gen_head() {
  atf_set "descr" "Test Boost generator on single directory project."
}
proplang_boost_gen_body() {
  pushd ../test/proplang
  #----------------------- 
  # Boost variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file proplang.prj\n" patch <patch/boost.patch
  atf_check -s exit:0 -o empty bouml proplang.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamroot output/Jamroot.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Jamfile output/src/Jamfile.bjam.canon
  atf_check -o empty diff output/src/fortran.f output/src/fortran.f.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/doc/Jamfile output/doc/Jamfile.bjam.canon
  atf_check -o empty diff output/local.jam output/local.jam.canon
  atf_check -o empty diff output/Jamfile output/Jamfile.base.canon
  atf_check -o empty diff output/CMakeLists.txt output/CMakeLists.txt.base.canon
  atf_check -o empty diff output/doc/program.t2t output/doc/program.t2t.canon
  atf_check -o empty diff output/t2t.bjam output/t2t.bjam.canon
  atf_check -o empty diff output/t2t.jam output/t2t.jam.canon
  atf_check -o empty diff output/t2t.cmake output/t2t.cmake.canon
  atf_check -o empty diff output/t2t.make output/t2t.make.canon
  atf_check -s exit:0 -o empty rm output/Jamroot output/local.jam
  atf_check -s exit:0 -o empty rm output/src/Jamfile output/src/fortran.f
  atf_check -s exit:0 -o empty rm output/doc/Jamfile output/doc/program.t2t
  atf_check -s exit:0 -o empty rm output/CMakeLists.txt output/Jamfile
  atf_check -s exit:0 -o empty rm output/t2t.bjam output/t2t.cmake output/t2t.jam output/t2t.make
  atf_check -s exit:0 -o empty rm -rf output/include
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case proplang_cmake_gen
proplang_cmake_gen_head() {
  atf_set "descr" "Test CMake generator on single directory project."
}
proplang_cmake_gen_body() {
  pushd ../test/proplang
  #----------------------- 
  # CMake variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file proplang.prj\n" patch <patch/cmake.patch
  atf_check -s exit:0 -o empty bouml proplang.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/CMakeLists.txt output/CMakeLists.txt.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/CMakeLists.txt output/src/CMakeLists.txt.canon
  atf_check -o empty diff output/src/fortran.f output/src/fortran.f.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/doc/CMakeLists.txt output/doc/CMakeLists.txt.canon
  atf_check -o empty diff output/local.cmake output/local.cmake.canon
  atf_check -o empty diff output/Jamfile output/Jamfile.base.canon
  atf_check -o empty diff output/Jamroot output/Jamroot.base.canon
  atf_check -o empty diff output/doc/program.t2t output/doc/program.t2t.canon
  atf_check -o empty diff output/t2t.bjam output/t2t.bjam.canon
  atf_check -o empty diff output/t2t.jam output/t2t.jam.canon
  atf_check -o empty diff output/t2t.cmake output/t2t.cmake.canon
  atf_check -o empty diff output/t2t.make output/t2t.make.canon
  atf_check -s exit:0 -o empty rm output/Jamroot output/local.cmake
  atf_check -s exit:0 -o empty rm output/src/CMakeLists.txt output/src/fortran.f
  atf_check -s exit:0 -o empty rm output/doc/CMakeLists.txt output/doc/program.t2t
  atf_check -s exit:0 -o empty rm output/CMakeLists.txt output/Jamfile
  atf_check -s exit:0 -o empty rm output/t2t.bjam output/t2t.cmake output/t2t.jam output/t2t.make
  atf_check -s exit:0 -o empty rm -rf output/include
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case proplang_make_gen
proplang_make_gen_head() {
  atf_set "descr" "Test CMake generator on single directory project."
}
proplang_make_gen_body() {
  pushd ../test/proplang
  #----------------------- 
  # CMake variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file proplang.prj\n" patch <patch/make.patch
  atf_check -s exit:0 -o empty bouml proplang.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/makefile output/makefile.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/makefile output/src/makefile.canon
  atf_check -o empty diff output/src/fortran.f output/src/fortran.f.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/doc/makefile output/doc/makefile.canon
  atf_check -o empty diff output/Jamfile output/Jamfile.base.canon
  atf_check -o empty diff output/Jamroot output/Jamroot.base.canon
  atf_check -o empty diff output/CMakeLists.txt output/CMakeLists.txt.base.canon
  atf_check -o empty diff output/doc/program.t2t output/doc/program.t2t.canon
  atf_check -o empty diff output/t2t.bjam output/t2t.bjam.canon
  atf_check -o empty diff output/t2t.jam output/t2t.jam.canon
  atf_check -o empty diff output/t2t.cmake output/t2t.cmake.canon
  atf_check -o empty diff output/t2t.make output/t2t.make.canon
  atf_check -o empty diff output/M_sys.mk output/M_sys.mk.canon
  atf_check -o empty diff output/M_cl.mk output/M_cl.mk.canon
  atf_check -o empty diff output/M_gcc.mk output/M_gcc.mk.canon
  atf_check -o empty diff output/M_unix.mk output/M_unix.mk.canon
  atf_check -o empty diff output/M_Windows_NT.mk output/M_Windows_NT.mk.canon
  atf_check -s exit:0 -o empty rm output/M_sys.mk output/M_cl.mk output/M_gcc.mk output/M_unix.mk output/M_Windows_NT.mk
  atf_check -s exit:0 -o empty rm output/Jamroot output/makefile
  atf_check -s exit:0 -o empty rm output/src/makefile output/src/fortran.f
  atf_check -s exit:0 -o empty rm output/doc/makefile output/doc/program.t2t
  atf_check -s exit:0 -o empty rm output/CMakeLists.txt output/Jamfile
  atf_check -s exit:0 -o empty rm output/t2t.bjam output/t2t.cmake output/t2t.jam output/t2t.make
  atf_check -s exit:0 -o empty rm -rf output/include
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}








atf_init_test_cases() {
    atf_add_test_case single_jam_gen
    atf_add_test_case single_boost_gen
    atf_add_test_case single_cmake_gen
    atf_add_test_case single_make_gen
    atf_add_test_case multidir_jam_gen
    atf_add_test_case multidir_boost_gen
    atf_add_test_case multidir_cmake_gen
    atf_add_test_case multidir_make_gen
    atf_add_test_case multilang_jam_gen
    atf_add_test_case multilang_boost_gen
    atf_add_test_case multilang_cmake_gen
    atf_add_test_case multilang_make_gen
    atf_add_test_case multitarget_jam_gen
    atf_add_test_case multitarget_boost_gen
    atf_add_test_case multitarget_cmake_gen
    atf_add_test_case multitarget_make_gen
    atf_add_test_case proplang_jam_gen
    atf_add_test_case proplang_boost_gen
    atf_add_test_case proplang_cmake_gen
    atf_add_test_case proplang_make_gen
}

