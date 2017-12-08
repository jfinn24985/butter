#!/home/finnerty/local/bin/atf-sh 

# Location of bouml standard installation.
BOUML_LOC=/usr/lib/bouml
BUTTER_EXE=../../source/src/butter/butter_exe

setup_example(){
  local example=$1
  local genr=$2
  local prop=$3
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  if [ "X${prop}" != "X" ]; then
    atf_check -s exit:0 -o inline:"patching file ${example}.prj\n" patch <patch/${prop}.patch
  fi
  atf_check -s exit:0 -o inline:"patching file ${example}.prj\n" patch <patch/${genr}.patch
}

run_plugouts(){
  local example=$1
  local genr=$2
  local prop=$3
  atf_check -s exit:0 -o save:canon/butter.log.${genr}.${prop} bouml ${example}.prj -execnogui ${BUTTER_EXE} -test:ok -exit
  atf_check -s exit:0 -o file:canon/cpp.log bouml ${example}.prj -execnogui ${BOUML_LOC}/cpp_generator -exit
}

check_boost_build(){
  # Test if build files are similar or different to canonical version
  # usage check_boost_build [0|!0] [0|!0] [0|!0] [0|!0]
  #   0 : asserts top, lib, exe and/or sys files match canon
  #   !0 : asserts top, lib, exe and/or sys files do not match canon
  #   anything else : do not check.
  local top=$1
  local lib=$2
  local exe=$3
  local sys=$4
  if [ 0 -eq ${top} ]; then
     atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/Jamroot output/Jamroot
  else
     atf_check -o ignore -s exit:1 diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/Jamroot output/Jamroot
  fi
  if [ 0 -eq ${lib} ]; then
     atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/Jamfile_boost_lib output/src/Library/Jamfile
  else
     atf_check -o ignore -s exit:1 diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/Jamfile_boost_lib output/src/Library/Jamfile
  fi
  if [ 0 -eq ${exe} ]; then
     atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/Jamfile_boost_exe output/src/Executable/Jamfile
  else
     atf_check -o ignore -s exit:1 diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/Jamfile_boost_exe output/src/Executable/Jamfile
  fi
  if [ 0 -eq ${sys} ]; then
     atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/local.jam output/local.jam
  else
     atf_check -o ignore -s exit:1 diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/local.jam output/local.jam
  fi
}

standard_build_test(){
  local variant=$1
  local builddir=$2
  local output_fname=${3:-output_default}
  local symbol_exit=${4:-1}
  local shared=${5:-0}
  local buildbase=${6:-build}
  pushd output
  # test base build target
  atf_check -s exit:0 -o save:boost1.log -e save:boost1.err bjam -d2 ${variant}
  atf_check -s exit:0 [ -x ${buildbase}/src/Executable/${builddir}/program ]
  if [ "X${shared}" = "X1" ] ; then
    atf_check -s exit:0 [ -e ${buildbase}/src/Library/${builddir}/library.so ]
  else
    atf_check -s exit:0 [ -e ${buildbase}/src/Library/${builddir}/link-static/library.a ]
  fi
  atf_check -s exit:0 -o file:../canon/${output_fname} ${buildbase}/src/Executable/${builddir}/program

  # no separate install target
  if [ -d installdir ]; then
    # release version.
    pushd ${buildbase}/src/Executable/${builddir}
      atf_check -s exit:0 -o save:symbol objdump -t program
    popd
    atf_check -s exit:0 -o ignore diff ../canon/symbol ${buildbase}/src/Executable/${builddir}/symbol
    atf_check -s exit:0 [ -x installdir/bin/program ]
    if [ "X${shared}" = "X1" ] ; then
      atf_check -s exit:0 [ -e installdir/lib/library.so ]
      LD_LIBRARY_PATH=installdir/lib atf_check -s exit:0 -o file:../canon/${output_fname} installdir/bin/program
    else
      atf_check -s exit:0 [ -e installdir/lib/library.a ]
      atf_check -s exit:0 -o file:../canon/${output_fname} installdir/bin/program
    fi
    
    pushd installdir/bin
      atf_check -s exit:0 -o save:symbol objdump -t program
    popd
    atf_check -s exit:0 -o ignore diff ../canon/symbol installdir/bin/symbol
  else
    # debug version
    pushd ${buildbase}/src/Executable/${builddir}
      atf_check -s exit:0 -o save:symbol objdump -t program
    popd
    atf_check -s exit:${symbol_exit} -o ignore diff ../canon/symbol ${buildbase}/src/Executable/${builddir}/symbol
    atf_check -s exit:0 [ -x src/Executable/install_program/program ]
    if [ "X${shared}" = "X1" ] ; then
      atf_check -s exit:0 [ -e src/Library/install_library/library.so ]
      LD_LIBRARY_PATH=src/Library/install_library atf_check -s exit:0 -o file:../canon/${output_fname} src/Executable/install_program/program
    else
      atf_check -s exit:0 [ -e src/Library/install_library/library.a ]
      atf_check -s exit:0 -o file:../canon/${output_fname} src/Executable/install_program/program
    fi
    pushd src/Executable/install_program
      atf_check -s exit:0 -o save:symbol objdump -t program
    popd
    atf_check -s exit:${symbol_exit} -o ignore diff ../canon/symbol src/Executable/install_program/symbol
  fi

  # no distclean target
  # test clean target
  atf_check -s exit:0 -o save:boost2.log -e save:boost2.err bjam clean ${variant}
  atf_check -s exit:0 [ ! -e ${buildbase}/src/Executable/${builddir}/program ]
  atf_check -s exit:0 [ ! -e ${buildbase}/src/Library/${builddir}/link-static/library.a ]
  if [ -d installdir ]; then
    # release version.
    atf_check -s exit:0 [ ! -e installdir/bin/program ]
    if [ "X${shared}" = "X1" ] ; then
      atf_check -s exit:0 [ ! -e installdir/lib/library.so ]
    else
      atf_check -s exit:0 [ ! -e installdir/lib/library.a ]
    fi
    atf_check -s exit:0 -o empty rm -rf installdir
  else
    # debug version
    atf_check -s exit:0 [ ! -e src/Executable/install_program/program ]
    if [ "X${shared}" = "X1" ] ; then
      atf_check -s exit:0 [ ! -e src/Library/install_library/library.so ]
    else
      atf_check -s exit:0 [ ! -e src/Library/install_library/library.a ]
    fi
    atf_check -s exit:0 -o empty rm -rf src/Executable/install_program
    atf_check -s exit:0 -o empty rm -rf src/Library/install_library
  fi 
  atf_check -s exit:0 -o empty rm boost1.log boost1.err
  atf_check -s exit:0 -o empty rm boost2.log boost2.err
  atf_check -s exit:0 -o empty rm -rf build
  popd
}


atf_test_case property_exeart_description_boost_gen
property_exeart_description_boost_gen_head() {
  atf_set "descr" "Description Executable.Target(HDR/FLAGS/LINK) with boost generator."
}
property_exeart_description_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Executable.Target.(HDR/FLAGS/LINK) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeart-description.patch
  run_plugouts "property_test" "boost" "exeart-description"
  check_boost_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exeart_libtype_shared_boost_gen
property_exeart_libtype_shared_boost_gen_head() {
  atf_set "descr" "Property Executable.Target(butter type=shared) with boost generator."
}
property_exeart_libtype_shared_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Target.butter type + shared 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeart-libtype-shared.patch
  run_plugouts "property_test" "boost" "exeart-libtype-shared"
  check_boost_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exeart_libtype_static_boost_gen
property_exeart_libtype_static_boost_gen_head() {
  atf_set "descr" "Property Executable.Target(butter type=static) with boost generator."
}
property_exeart_libtype_static_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Target.butter type + static 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeart-libtype-static.patch
  run_plugouts "property_test" "boost" "exeart-libtype-static"
  check_boost_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exeart_include_boost_gen
property_exeart_include_boost_gen_head() {
  atf_set "descr" "Property Executable.Target(butter include) with boost generator."
}
property_exeart_include_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Target.butter include + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeart-include.patch
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\npatching file output/include/other/exe_message.hpp\n" patch -p0 <patch/other_header.patch
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exe_source_include.patch
  run_plugouts "property_test" "boost" "exeart-include"
  check_boost_build 0 0 1 0
  # "include" for a lib.source file should be only to this source

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" output_exe_include
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" output_exe_include
  # RELEASE VARIANT
  standard_build_test release "*/release" output_exe_include

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exeart_ldflags_boost_gen
property_exeart_ldflags_boost_gen_head() {
  atf_set "descr" "Property Executable.Target(butter ldflags) with boost generator."
}
property_exeart_ldflags_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Target.butter ldflags + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeart-ldflags.patch
  run_plugouts "property_test" "boost" "exeart-ldflags"
  check_boost_build 0 0 1 0

  # LDFlags for a lib.source file should apply to the library users
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" output_default 0
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" output_default 0
  # RELEASE VARIANT
  standard_build_test release "*/release" output_default 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exeart_flags_boost_gen
property_exeart_flags_boost_gen_head() {
  atf_set "descr" "Property Executable.Target(butter flags) with boost generator."
}
property_exeart_flags_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Target.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeart-flags.patch
  run_plugouts "property_test" "boost" "exeart-flags"
  check_boost_build 0 0 1 0

# Flags for a lib.source file should apply to itself only

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" output_exesrc_flags
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" output_exesrc_flags
  # RELEASE VARIANT
  standard_build_test release "*/release" output_exesrc_flags

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_libart_description_boost_gen
property_libart_description_boost_gen_head() {
  atf_set "descr" "Description Library.Target(HDR/FLAGS/LINK) with boost generator."
}
property_libart_description_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Library.Target.(HDR/FLAGS/LINK) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libart-description.patch
  run_plugouts "property_test" "boost" "libart-description"
  check_boost_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libart_libtype_shared_boost_gen
property_libart_libtype_shared_boost_gen_head() {
  atf_set "descr" "Property Library.Target(butter type=shared) with boost generator."
}
property_libart_libtype_shared_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Target.butter type + shared 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libart-libtype-shared.patch
  run_plugouts "property_test" "boost" "libart-libtype-shared"
  check_boost_build 0 1 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" "" "" 1
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" "" "" 1
  # RELEASE VARIANT
  standard_build_test release "*/release" "" "" 1

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libart_libtype_static_boost_gen
property_libart_libtype_static_boost_gen_head() {
  atf_set "descr" "Property Library.Target(butter type=static) with boost generator."
}
property_libart_libtype_static_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Target.butter type + static 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libart-libtype-static.patch
  run_plugouts "property_test" "boost" "libart-libtype-static"
  check_boost_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libart_include_boost_gen
property_libart_include_boost_gen_head() {
  atf_set "descr" "Property Library.Target(butter include) with boost generator."
}
property_libart_include_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Target.butter include + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libart-include.patch
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\npatching file output/include/other/exe_message.hpp\n" patch -p0 <patch/other_header.patch
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/lib_source_include.patch
  run_plugouts "property_test" "boost" "libart-include"
  check_boost_build 0 1 0 0
  # "include" for a lib.source file should be only to this source

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" output_top_include
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" output_top_include
  # RELEASE VARIANT
  standard_build_test release "*/release" output_top_include

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libart_ldflags_boost_gen
property_libart_ldflags_boost_gen_head() {
  atf_set "descr" "Property Library.Target(butter ldflags) with boost generator."
}
property_libart_ldflags_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Target.butter ldflags + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libart-ldflags.patch
  run_plugouts "property_test" "boost" "libart-ldflags"
  check_boost_build 0 1 0 0

  # LDFlags for a lib.source file should apply to the library users
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" output_default 0
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" output_default 0
  # RELEASE VARIANT
  standard_build_test release "*/release" output_default 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libart_flags_boost_gen
property_libart_flags_boost_gen_head() {
  atf_set "descr" "Property Library.Target(butter flags) with boost generator."
}
property_libart_flags_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Target.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libart-flags.patch
  run_plugouts "property_test" "boost" "libart-flags"
  check_boost_build 0 1 0 0

# Flags for a lib.source file should apply to itself only

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" output_libsrc_flags
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" output_libsrc_flags
  # RELEASE VARIANT
  standard_build_test release "*/release" output_libsrc_flags

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}



atf_test_case property_exesrc_description_boost_gen
property_exesrc_description_boost_gen_head() {
  atf_set "descr" "Description Executable.Source(HDR/FLAGS/LINK) with boost generator."
}
property_exesrc_description_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Executable.Source.(HDR/FLAGS/LINK) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exesrc-description.patch
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exe_source_include.patch
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\npatching file output/include/other/exe_message.hpp\n" patch -p0 <patch/other_header.patch
  run_plugouts "property_test" "boost" "exesrc-description"
  check_boost_build 0 0 1 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" output_exe_include 0
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" output_exe_include 0
  # RELEASE VARIANT
  standard_build_test release "*/release" output_exe_include 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exesrc_libtype_shared_boost_gen
property_exesrc_libtype_shared_boost_gen_head() {
  atf_set "descr" "Property Executable.Source(butter type=shared) with boost generator."
}
property_exesrc_libtype_shared_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Source.butter type + shared 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exesrc-libtype-shared.patch
  run_plugouts "property_test" "boost" "exesrc-libtype-shared"
  check_boost_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exesrc_libtype_static_boost_gen
property_exesrc_libtype_static_boost_gen_head() {
  atf_set "descr" "Property Executable.Source(butter type=static) with boost generator."
}
property_exesrc_libtype_static_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Source.butter type + static 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exesrc-libtype-static.patch
  run_plugouts "property_test" "boost" "exesrc-libtype-static"
  check_boost_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exesrc_include_boost_gen
property_exesrc_include_boost_gen_head() {
  atf_set "descr" "Property Executable.Source(butter include) with boost generator."
}
property_exesrc_include_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Source.butter include + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exesrc-include.patch
  #atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\npatching file output/include/other/exe_message.hpp\n" patch -p0 <patch/other_header.patch
  #atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exe_source_include.patch
  run_plugouts "property_test" "boost" "exesrc-include"
  check_boost_build 0 0 0 0
  # check_boost_build 0 0 1 0
  # "include" for a lib.source file should be only to this source

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" # output_exe_include
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" # output_exe_include
  # RELEASE VARIANT
  standard_build_test release "*/release" # output_exe_include

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exesrc_ldflags_boost_gen
property_exesrc_ldflags_boost_gen_head() {
  atf_set "descr" "Property Executable.Source(butter ldflags) with boost generator."
}
property_exesrc_ldflags_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Source.butter ldflags + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exesrc-ldflags.patch
  run_plugouts "property_test" "boost" "exesrc-ldflags"
  check_boost_build 0 0 1 0

  # LDFlags for a exe.source file should apply to the exe target
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" output_default 0
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" output_default 0
  # RELEASE VARIANT
  standard_build_test release "*/release" output_default 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exesrc_flags_boost_gen
property_exesrc_flags_boost_gen_head() {
  atf_set "descr" "Property Executable.Source(butter flags) with boost generator."
}
property_exesrc_flags_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Source.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exesrc-flags.patch
  run_plugouts "property_test" "boost" "exesrc-flags"
  check_boost_build 0 0 0 0
  # check_boost_build 0 0 1 0

  # Flags for a exe.source file should apply to itself only

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" # output_exesrc_flags
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" # output_exesrc_flags
  # RELEASE VARIANT
  standard_build_test release "*/release" # output_exesrc_flags

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_libsrc_description_boost_gen
property_libsrc_description_boost_gen_head() {
  atf_set "descr" "Description Library.Source(HDR/FLAGS/LINK) with boost generator."
}
property_libsrc_description_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Library.Source.(HDR/FLAGS/LINK) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-description.patch
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\npatching file output/include/other/exe_message.hpp\n" patch -p0 <patch/other_header.patch
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/lib_source_include.patch
  run_plugouts "property_test" "boost" "libsrc-description"
  check_boost_build 0 1 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" output_top_include 0
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" output_top_include 0
  # RELEASE VARIANT
  standard_build_test release "*/release" output_top_include 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libsrc_libtype_shared_boost_gen
property_libsrc_libtype_shared_boost_gen_head() {
  atf_set "descr" "Property Library.Source(butter type=shared) with boost generator."
}
property_libsrc_libtype_shared_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter type + shared 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-libtype-shared.patch
  run_plugouts "property_test" "boost" "libsrc-libtype-shared"
  check_boost_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libsrc_libtype_static_boost_gen
property_libsrc_libtype_static_boost_gen_head() {
  atf_set "descr" "Property Library.Source(butter type=static) with boost generator."
}
property_libsrc_libtype_static_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter type + static 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-libtype-static.patch
  run_plugouts "property_test" "boost" "libsrc-libtype-static"
  check_boost_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libsrc_include_boost_gen
property_libsrc_include_boost_gen_head() {
  atf_set "descr" "Property Library.Source(butter include) with boost generator."
}
property_libsrc_include_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter include + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-include.patch
  #atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\npatching file output/include/other/exe_message.hpp\n" patch -p0 <patch/other_header.patch
  #atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/lib_source_include.patch
  run_plugouts "property_test" "boost" "libsrc-include"
  check_boost_build 0 0 0 0
  #check_boost_build 0 1 0 0
  # "include" for a lib.source file should be only to this source

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" #output_top_include
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" #output_top_include
  # RELEASE VARIANT
  standard_build_test release "*/release" #output_top_include

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libsrc_ldflags_boost_gen
property_libsrc_ldflags_boost_gen_head() {
  atf_set "descr" "Property Library.Source(butter ldflags) with boost generator."
}
property_libsrc_ldflags_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter ldflags + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-ldflags.patch
  run_plugouts "property_test" "boost" "libsrc-ldflags"
  check_boost_build 0 1 0 0

  # LDFlags for a lib.source file should apply to the library users
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" output_default 0
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" output_default 0
  # RELEASE VARIANT
  standard_build_test release "*/release" output_default 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libsrc_flags_boost_gen
property_libsrc_flags_boost_gen_head() {
  atf_set "descr" "Property Library.Source(butter flags) with boost generator."
}
property_libsrc_flags_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-flags.patch
  run_plugouts "property_test" "boost" "libsrc-flags"
  check_boost_build 0 0 0 0
  # check_boost_build 0 1 0 0

  # Flags for a lib.source file should apply to itself only

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" # output_libsrc_flags
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" # output_libsrc_flags
  # RELEASE VARIANT
  standard_build_test release "*/release" # output_libsrc_flags

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}



atf_test_case property_exe_description_boost_gen
property_exe_description_boost_gen_head() {
  atf_set "descr" "Description Executable Package(HDR/FLAGS/LINK) with boost generator."
}
property_exe_description_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Executable.(HDR/FLAGS/LINK) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-description.patch
  run_plugouts "property_test" "boost" "exeproj-description"
  check_boost_build 0 0 0 0

  # Flags for a executable should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exe_libtype_shared_boost_gen
property_exe_libtype_shared_boost_gen_head() {
  atf_set "descr" "Property Executable Package(butter type=shared) with boost generator."
}
property_exe_libtype_shared_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter type + shared 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-libtype-shared.patch
  run_plugouts "property_test" "boost" "exeproj-libtype-shared"
  check_boost_build 0 0 0 0

  # Flags for a executable should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exe_libtype_static_boost_gen
property_exe_libtype_static_boost_gen_head() {
  atf_set "descr" "Property Executable Package(butter type=static) with boost generator."
}
property_exe_libtype_static_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter type + static 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-libtype-static.patch
  run_plugouts "property_test" "boost" "exeproj-libtype-static"
  check_boost_build 0 0 0 0

  # Flags for a executable should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exe_include_boost_gen
property_exe_include_boost_gen_head() {
  atf_set "descr" "Property Executable Package(butter include) with boost generator."
}
property_exe_include_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter include + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-include.patch
  run_plugouts "property_test" "boost" "exeproj-include"
  check_boost_build 0 0 0 0
  # "include" for a executable project should be included for the library
  # and its users?

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exe_ldflags_boost_gen
property_exe_ldflags_boost_gen_head() {
  atf_set "descr" "Property Executable Package(butter ldflags) with boost generator."
}
property_exe_ldflags_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter ldflags + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-ldflags.patch
  run_plugouts "property_test" "boost" "exeproj-ldflags"
  check_boost_build 0 0 0 0

  # Flags for a executable should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_exe_flags_boost_gen
property_exe_flags_boost_gen_head() {
  atf_set "descr" "Property Executable Package(butter flags) with boost generator."
}
property_exe_flags_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-flags.patch
  run_plugouts "property_test" "boost" "exeproj-flags"
  check_boost_build 0 0 0 0

  # Flags for a executable should apply to the using class?

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}



atf_test_case property_lib_description_boost_gen
property_lib_description_boost_gen_head() {
  atf_set "descr" "Description Library Package(HDR/FLAGS/LINK) with boost generator."
}
property_lib_description_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Library.(HDR/FLAGS/LINK) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-description.patch
  run_plugouts "property_test" "boost" "libproj-description"
  check_boost_build 0 0 0 0

  # Flags for a library should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_lib_libtype_shared_boost_gen
property_lib_libtype_shared_boost_gen_head() {
  atf_set "descr" "Property Library Package(butter type=shared) with boost generator."
}
property_lib_libtype_shared_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter type + shared 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-libtype-shared.patch
  run_plugouts "property_test" "boost" "libproj-libtype-shared"
  check_boost_build 0 1 0 0

  # Flags for a library should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" "" "" 1
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" "" "" 1
  # RELEASE VARIANT
  standard_build_test release "*/release" "" "" 1

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_lib_libtype_static_boost_gen
property_lib_libtype_static_boost_gen_head() {
  atf_set "descr" "Property Library Package(butter type=static) with boost generator."
}
property_lib_libtype_static_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter type + static 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-libtype-static.patch
  run_plugouts "property_test" "boost" "libproj-libtype-static"
  check_boost_build 0 0 0 0

  # Flags for a library should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_lib_include_boost_gen
property_lib_include_boost_gen_head() {
  atf_set "descr" "Property Library Package(butter include) with boost generator."
}
property_lib_include_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter include + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-include.patch
  run_plugouts "property_test" "boost" "libproj-include"
  check_boost_build 0 0 0 0
  # "include" for a library project should be included for the library
  # and its users?

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_lib_ldflags_boost_gen
property_lib_ldflags_boost_gen_head() {
  atf_set "descr" "Property Library Package(butter ldflags) with boost generator."
}
property_lib_ldflags_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter ldflags + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-ldflags.patch
  run_plugouts "property_test" "boost" "libproj-ldflags"
  check_boost_build 0 0 0 0

  # Flags for a library should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_lib_flags_boost_gen
property_lib_flags_boost_gen_head() {
  atf_set "descr" "Property Library Package(butter flags) with boost generator."
}
property_lib_flags_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-flags.patch
  run_plugouts "property_test" "boost" "libproj-flags"
  check_boost_build 0 0 0 0

  # Flags for a library should apply to the using class?

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_version_boost_gen
property_version_boost_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter version) with boost generator."
}
property_version_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter include + "other" 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost-top" "version"
  run_plugouts "property_test" "boost" "version"
  check_boost_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_top_description_boost_gen
property_top_description_boost_gen_head() {
  atf_set "descr" "Description (*top*)Package (FLAGS=..) with boost generator."
}
property_top_description_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # PROJECT.description : FLAGS=... \ HDR=... \ LINK=...
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost" "top_description"
  run_plugouts "property_test" "boost" "top_description"
  check_boost_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_top_ldflags_boost_gen
property_top_ldflags_boost_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter ldflags) with boost generator."
}
property_top_ldflags_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter ldflags + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost-top" "top-ldflags"
  run_plugouts "property_test" "boost" "top-ldflags"
  check_boost_build 1 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" output_default 0
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" output_default 0
  # RELEASE VARIANT
  standard_build_test release "*/release" output_default 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output

  # Test of ldflags that are really for flags to see
  # if the ldflags are used during compilation as well
  # as only during linking. 
  setup_example "property_test" "boost-top" "top-ldflags-guard"
  run_plugouts "property_test" "boost" "top-ldflags-guard"
  check_boost_build 1 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output

  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_top_include_boost_gen
property_top_include_boost_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter include) with boost generator."
}
property_top_include_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter include + "other" 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost-top" "top-include"
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\npatching file output/include/other/exe_message.hpp\n" patch -p0 <patch/other_header.patch
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/lib_source_include.patch
  run_plugouts "property_test" "boost" "top-include"
  check_boost_build 1 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" "output_top_include"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" "output_top_include"
  # RELEASE VARIANT
  standard_build_test release "*/release" "output_top_include"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_top_libtype_shared_boost_gen
property_top_libtype_shared_boost_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter type = shared) with boost generator."
}
property_top_libtype_shared_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter type + static
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost-top" "top-libtype-shared"
  run_plugouts "property_test" "boost" "top-libtype-shared"
  check_boost_build 0 1 0 0
 
  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" "" "" 1
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" "" "" 1
  # RELEASE VARIANT
  standard_build_test release "*/release" "" "" 1

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_top_libtype_static_boost_gen
property_top_libtype_static_boost_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter type = static) with boost generator."
}
property_top_libtype_static_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter type + static
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost-top" "top-libtype-static"
  run_plugouts "property_test" "boost" "top-libtype-static"
  check_boost_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_log_level_boost_gen
property_log_level_boost_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter log-level) with boost generator."
}
property_log_level_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter log-level = 0 (default) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost-top" "loglevel0"
  run_plugouts "property_test" "boost" "loglevel0"
  atf_check -o empty diff canon/butter.log.boost canon/butter.log.boost.loglevel0 
  atf_check -s exit:0 [ -e output/include/butter.log ]
  atf_check -s exit:1 [ -s output/include/butter.log ]
  check_boost_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output

  #----------------------- 
  # Property: PROJECT.butter log-level = 1 (information) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost-top" "loglevel1"
  run_plugouts "property_test" "boost" "loglevel1"
  atf_check -s exit:0 [ -e output/include/butter.log ]
  atf_check -s exit:1 [ -s output/include/butter.log ]
  check_boost_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output



  #----------------------- 
  # Property: PROJECT.butter log-level = 2 (debug) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost-top" "loglevel2"
  run_plugouts "property_test" "boost" "loglevel2"
  atf_check -s exit:0 [ -e output/include/butter.log ]
  atf_check -s exit:1 [ -s output/include/butter.log ]
  check_boost_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output



  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_top_flags_boost_gen
property_top_flags_boost_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter flags) with boost generator."
}
property_top_flags_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost-top" "top-flags"
  run_plugouts "property_test" "boost" "top-flags"
  check_boost_build 1 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" "output_top_flags"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" "output_top_flags"
  # RELEASE VARIANT
  standard_build_test release "*/release" "output_top_flags"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_basedir_boost_gen
property_basedir_boost_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter base) with boost generator."
}
property_basedir_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter base + ../..
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost-top" "basedir"
  run_plugouts "property_test" "boost" "basedir"
  atf_check -s exit:0 [ -e Jamroot ]
  atf_check -s exit:0 [ -e local.jam ]
 
  build_test(){
    local variant=$1
    local builddir=$2
    # test base build target
    atf_check -s exit:0 -o save:boost1.log -e save:boost1.err bjam ${variant}
    atf_check -s exit:0 [ -x build/output/src/Executable/${builddir}/program ]
    atf_check -s exit:0 [ -e build/output/src/Library/${builddir}/link-static/library.a ]
    atf_check -s exit:0 -o file:canon/output_default build/output/src/Executable/${builddir}/program
    # no separate install target
    if [ -d installdir ]; then
      atf_check -s exit:0 [ -x installdir/bin/program ]
      atf_check -s exit:0 [ -e installdir/lib/library.a ]
      atf_check -s exit:0 -o file:canon/output_default installdir/bin/program
    else
      atf_check -s exit:0 [ -x output/src/Executable/install_program/program ]
      atf_check -s exit:0 [ -e output/src/Library/install_library/library.a ]
      atf_check -s exit:0 -o file:canon/output_default output/src/Executable/install_program/program
    fi
    # no distclean target
    # test clean target
    atf_check -s exit:0 -o save:boost2.log -e save:boost2.err bjam clean ${variant}
    atf_check -s exit:0 [ ! -e build/output/src/Executable/${builddir}/program ]
    atf_check -s exit:0 [ ! -e build/output/src/Library/${builddir}/link-static/library.a ]
    if [ -d installdir ]; then
      atf_check -s exit:0 [ ! -e installdir/bin/program ]
      atf_check -s exit:0 [ ! -e installdir/lib/library.a ]
      atf_check -s exit:0 -o empty rm -rf installdir
    else
      atf_check -s exit:0 [ ! -e output/src/Executable/install_program/program ]
      atf_check -s exit:0 [ ! -e output/src/Library/install_library/library.a ]
      atf_check -s exit:0 -o empty rm -rf output/src/Executable/install_program
      atf_check -s exit:0 -o empty rm -rf output/src/Library/install_library
    fi

    atf_check -s exit:0 -o empty rm boost1.log boost1.err
    atf_check -s exit:0 -o empty rm boost2.log boost2.err
    atf_check -s exit:0 -o empty rm -rf build
  }

  # default (DEBUG) VARIANT
  build_test "" "*/debug"
  # specific DEBUG VARIANT
  build_test debug "*/debug"
  # RELEASE VARIANT
  build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty rm Jamroot local.jam
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
}


atf_test_case property_builddir_boost_gen
property_builddir_boost_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter build-dir) with boost generator."
}
property_builddir_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter build-dir + BUILD
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "boost-top" "builddir"
  run_plugouts "property_test" "boost" "builddir"
  check_boost_build 1 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug" "" "" "" BUILD
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug" "" "" "" BUILD
  # RELEASE VARIANT
  standard_build_test release "*/release" "" "" "" BUILD

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_test_boost_gen
property_test_boost_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
property_test_boost_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Standard (boost) variant without defined properties (nul test)
  #----------------------- 
  setup_example "property_test" "boost"
  run_plugouts "property_test" "boost" "nultest"
  check_boost_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" "*/debug"
  # specific DEBUG VARIANT
  standard_build_test debug "*/debug"
  # RELEASE VARIANT
  standard_build_test release "*/release"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_init_test_cases() {
    atf_add_test_case property_test_boost_gen
    atf_add_test_case property_builddir_boost_gen
    atf_add_test_case property_basedir_boost_gen
    atf_add_test_case property_version_boost_gen
    atf_add_test_case property_log_level_boost_gen
    atf_add_test_case property_top_flags_boost_gen
    atf_add_test_case property_top_ldflags_boost_gen
    atf_add_test_case property_top_include_boost_gen
    atf_add_test_case property_top_description_boost_gen
    atf_add_test_case property_top_libtype_static_boost_gen
    atf_add_test_case property_top_libtype_shared_boost_gen
    atf_add_test_case property_lib_flags_boost_gen
    atf_add_test_case property_lib_ldflags_boost_gen
    atf_add_test_case property_lib_include_boost_gen
    atf_add_test_case property_lib_libtype_static_boost_gen
    atf_add_test_case property_lib_libtype_shared_boost_gen
    atf_add_test_case property_lib_description_boost_gen
    atf_add_test_case property_exe_flags_boost_gen
    atf_add_test_case property_exe_ldflags_boost_gen
    atf_add_test_case property_exe_include_boost_gen
    atf_add_test_case property_exe_libtype_static_boost_gen
    atf_add_test_case property_exe_libtype_shared_boost_gen
    atf_add_test_case property_exe_description_boost_gen
    atf_add_test_case property_libsrc_flags_boost_gen
    atf_add_test_case property_libsrc_ldflags_boost_gen
    atf_add_test_case property_libsrc_include_boost_gen
    atf_add_test_case property_libsrc_libtype_static_boost_gen
    atf_add_test_case property_libsrc_libtype_shared_boost_gen
    atf_add_test_case property_libsrc_description_boost_gen
    atf_add_test_case property_exesrc_flags_boost_gen
    atf_add_test_case property_exesrc_ldflags_boost_gen
    atf_add_test_case property_exesrc_include_boost_gen
    atf_add_test_case property_exesrc_libtype_static_boost_gen
    atf_add_test_case property_exesrc_libtype_shared_boost_gen
    atf_add_test_case property_exesrc_description_boost_gen
    atf_add_test_case property_libart_flags_boost_gen
    atf_add_test_case property_libart_ldflags_boost_gen
    atf_add_test_case property_libart_include_boost_gen
    atf_add_test_case property_libart_libtype_static_boost_gen
    atf_add_test_case property_libart_libtype_shared_boost_gen
    atf_add_test_case property_libart_description_boost_gen
    atf_add_test_case property_exeart_flags_boost_gen
    atf_add_test_case property_exeart_ldflags_boost_gen
    atf_add_test_case property_exeart_include_boost_gen
    atf_add_test_case property_exeart_libtype_static_boost_gen
    atf_add_test_case property_exeart_libtype_shared_boost_gen
    atf_add_test_case property_exeart_description_boost_gen


}

