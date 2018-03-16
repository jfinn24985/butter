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
  atf_check -s exit:0 -o save:canon/butter.log.${genr} bouml ${example}.prj -execnogui ${BUTTER_EXE} -test:ok -exit
  atf_check -s exit:0 -o file:canon/cpp.log bouml ${example}.prj -execnogui ${BOUML_LOC}/cpp_generator -exit
}

atf_test_case extern_lib2_jam_gen
extern_lib2_jam_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
extern_lib2_jam_gen_body() {
  pushd ../test/extern_lib2
  #----------------------- 
  # Standard (jam) variant 
  #-----------------------
  setup_example "extern_lib2" "jam"

  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamfile canon/Jamfile
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamrules canon/Jamrules

  build_test(){
    local variant=$1
    local builddir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/program ]
    atf_check -s exit:0 [ -e ${builddir}/example_exe.o ]
    atf_check -s exit:0 -o file:../canon/output ${builddir}/program ../input/lorem.zip
    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 -o file:../canon/output installdir/bin/program ../input/lorem.zip
    # no distclean target
    # test clean target
    atf_check -s exit:0 -o save:jam3.log -e save:jam3.err jam clean ${variant}
    atf_check -s exit:0 [ ! -e ${builddir}/program ]
    atf_check -s exit:0 [ ! -e ${builddir}/example_exe.o ]
    atf_check -s exit:0 [ -e installdir/bin/program ]

    atf_check -s exit:0 -o empty rm jam1.log jam1.err
    atf_check -s exit:0 -o empty rm jam2.log jam2.err
    atf_check -s exit:0 -o empty rm jam3.log jam3.err
    atf_check -s exit:0 -o empty rm -rf ${builddir}
    atf_check -s exit:0 -o empty rm -rf installdir
    popd
  }

  # default (DEBUG) VARIANT
  build_test "" DEBUG
  # specific DEBUG VARIANT
  build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  build_test -sVARIANT=RELEASE RELEASE
  
  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
}

atf_test_case extern_lib2_boost_gen
extern_lib2_boost_gen_head() {
  atf_set "descr" "Test Boost Jam generator on single directory project."
}
extern_lib2_boost_gen_body() {
  pushd ../test/extern_lib2
  #-------------- 
  # Boost variant 
  #-------------- 
  setup_example "extern_lib2" "boost"

  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamroot canon/Jamroot
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/local.jam canon/local.jam

  build_test(){
    local variant=$1
    local builddir=$2
    local installdir=$3
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:build1.log -e save:build1.err bjam ${variant}
    atf_check -s exit:0 [ -x build/${builddir}/program ]
    atf_check -s exit:0 [ -e build/${builddir}/example_exe.o ]
    atf_check -s exit:0 -o file:../canon/output build/${builddir}/program ../input/lorem.zip
    # no separate install target
    atf_check -s exit:0 [ -x ${installdir}/program ]
    atf_check -s exit:0 -o file:../canon/output ${installdir}/program ../input/lorem.zip
    # no distclean target
    # test clean target
    atf_check -s exit:0 -o save:build2.log -e save:build2.err bjam clean ${variant}
    atf_check -s exit:0 [ ! -e build/${builddir}/program ]
    atf_check -s exit:0 [ ! -e build/${builddir}/example_exe.o ]
    atf_check -s exit:0 [ ! -e ${installdir}/program ]

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
  build_test "" "*/debug" install_program
  # specific DEBUG VARIANT
  build_test debug "*/debug" install_program
  # RELEASE VARIANT
  build_test release "*/release" installdir/bin
   
  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
}

atf_test_case extern_lib2_cmake_gen
extern_lib2_cmake_gen_head() {
  atf_set "descr" "Test CMake generator on single directory project."
}
extern_lib2_cmake_gen_body() {
  pushd ../test/extern_lib2
  #-------------- 
  # CMake variant 
  #-------------- 
  setup_example "extern_lib2" "cmake"

  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/CMakeLists.txt canon/CMakeLists.txt
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/local.cmake canon/local.cmake

  build_test(){
    local variant=$1
    local installdir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:build1.log -e save:build1.err cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=${variant} -D QTDIR=${QTDIR} --build .
    atf_check -s exit:0 -o save:build2.log -e save:build2.err make -f Makefile VERBOSE=1
    atf_check -s exit:0 [ -x program ]
    atf_check -s exit:0 [ -e CMakeFiles/program.dir/example_exe.cc.o ]
    atf_check -s exit:0 -o file:../canon/output ./program ../input/lorem.zip
    # test install target
    atf_check -s exit:0 -o save:build2.log -e save:build2.err make -f Makefile install VERBOSE=1
    if [ "X" != "X${installdir}" ]; then
      atf_check -s exit:0 [ -e install_manifest.txt ]
      atf_check -s exit:0 [ -s install_manifest.txt ]
      atf_check -s exit:0 [ -e ${installdir}/bin/program ]
      atf_check -s exit:0 -o file:../canon/output ${installdir}/bin/program ../input/lorem.zip
    else
      atf_check -s exit:0 [ -e install_manifest.txt ]
      atf_check -s exit:0 [ ! -s install_manifest.txt ]
    fi
    # no distclean target
    # test clean target
    atf_check -s exit:0 -o save:build3.log -e save:build3.err make -f Makefile clean VERBOSE=1
    atf_check -s exit:0 [ ! -e ./program ]
    atf_check -s exit:0 [ ! -e CMakeFiles/program.dir/example_exe.o ]
    if [ "X" != "X${installdir}" ]; then
      atf_check -s exit:0 [ -e ${installdir}/bin/program ]
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
  build_test "" ""
  # specific DEBUG VARIANT
  build_test Debug ""
  # RELEASE VARIANT
  build_test Release "installdir"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
}

atf_test_case extern_lib2_make_gen
extern_lib2_make_gen_head() {
  atf_set "descr" "Test Makefile generator on single directory project."
}
extern_lib2_make_gen_body() {
  pushd ../test/extern_lib2
  #-------------- 
  # Make variant 
  #-------------- 
  setup_example "extern_lib2" "make"

  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/makefile canon/makefile
  atf_check -o empty diff output/M_sys.mk canon/M_sys.mk
  atf_check -o empty diff output/M_cl.mk canon/M_cl.mk
  atf_check -o empty diff output/M_gcc.mk canon/M_gcc.mk
  atf_check -o empty diff output/M_unix.mk canon/M_unix.mk
  atf_check -o empty diff output/M_Windows_NT.mk canon/M_Windows_NT.mk
 
  build_test(){
    local variant=$1
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:build1.log -e save:build1.err make -k VARIANT=${variant}
    atf_check -s exit:0 [ -x ./program ]
    atf_check -s exit:0 [ -e example_exe.o ]
    atf_check -s exit:0 [ ! -e installdir/bin/program ]
    atf_check -s exit:0 -o file:../canon/output ./program ../input/lorem.zip
    # test clean target
    atf_check -s exit:0 -o save:build2.log -e save:build2.err make clean
    atf_check -s exit:0 [ -e ./program ]
    atf_check -s exit:0 [ ! -e example_exe.o ]
    atf_check -s exit:0 [ ! -e installdir/bin/program ]

    # test install target
    atf_check -s exit:0 -o save:build3.log -e save:build3.err make install
    atf_check -s exit:0 [ -x ./program ]
    atf_check -s exit:0 [ -e example_exe.o ]
    atf_check -s exit:0 [ -e installdir/bin/program ]
    atf_check -s exit:0 -o file:../canon/output installdir/bin/program ../input/lorem.zip
    # test distclean target
    atf_check -s exit:0 -o save:build4.log -e save:build4.err make distclean
    atf_check -s exit:0 [ ! -e ./program ]
    atf_check -s exit:0 [ ! -e example_exe.o ]
    atf_check -s exit:0 [ -e installdir/bin/program ]

    # cleanup
    atf_check -s exit:0 -o empty rm build1.log build1.err
    atf_check -s exit:0 -o empty rm build2.log build2.err
    atf_check -s exit:0 -o empty rm build3.log build3.err
    atf_check -s exit:0 -o empty rm build4.log build4.err
    atf_check -s exit:0 -o empty rm -rf installdir
    popd
  }

  # default (DEBUG) VARIANT
  build_test ""
  # specific DEBUG VARIANT
  build_test DEBUG
  # RELEASE VARIANT
  build_test RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .

  popd
  unset build_test
}


atf_init_test_cases() {
    atf_add_test_case extern_lib2_jam_gen
    atf_add_test_case extern_lib2_boost_gen
    atf_add_test_case extern_lib2_cmake_gen
    atf_add_test_case extern_lib2_make_gen
}

