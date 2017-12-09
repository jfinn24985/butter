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

check_build(){
  # Test if build files are similar or different to canonical version
  # usage check_build [0|!0] [0|!0] [0|!0] [0|!0]
  #   0 : asserts top, lib, exe and/or sys files match canon
  #   !0 : asserts top, lib, exe and/or sys files do not match canon
  #   anything else : do not check.
  local top=$1
  local lib=$2
  local exe=$3
  local sys=$4
  if [ 0 -eq ${top} ]; then
     atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/makefile output/makefile
  else
     atf_check -o ignore -s exit:1 diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/makefile output/makefile
  fi
  if [ 0 -eq ${lib} ]; then
     atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/makefile_lib output/src/Library/makefile 
  else
     atf_check -o ignore -s exit:1 diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/makefile_lib output/src/Library/makefile
  fi
  if [ 0 -eq ${exe} ]; then
     atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/makefile_exe output/src/Executable/makefile
  else
     atf_check -o ignore -s exit:1 diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/makefile_exe output/src/Executable/makefile
  fi
  atf_check -o empty diff output/M_sys.mk canon/M_sys.mk
  atf_check -o empty diff output/M_cl.mk canon/M_cl.mk
  atf_check -o empty diff output/M_gcc.mk canon/M_gcc.mk
  atf_check -o empty diff output/M_unix.mk canon/M_unix.mk
  atf_check -o empty diff output/M_Windows_NT.mk canon/M_Windows_NT.mk
}

standard_build_test(){
  local variant=$1
  local output_fname=${2:-output_default}
  local symbol_exit=${3:-1}
  local shared=${4:-0}
  local buildbase=${5:-.}
  pushd output
  # test base build target
  atf_check -s exit:0 -o save:build1.log -e save:build1.err make -f makefile VARIANT=${variant}
  atf_check -s exit:0 -o save:build2.log -e save:build2.err make -f makefile VARIANT=${variant}
  atf_check -s exit:0 [ -x ${buildbase}/src/Executable/program ]
  if [ "X${shared}" = "X1" ] ; then
    atf_check -s exit:0 [ -e ${buildbase}/src/Library/library.so ]
    LD_LIBRARY_PATH=${buildbase}/src/Library atf_check -s exit:0 -o file:../canon/${output_fname} ${buildbase}/src/Executable/program
  else
    atf_check -s exit:0 [ -e ${buildbase}/src/Library/library.a ]
    atf_check -s exit:0 -o file:../canon/${output_fname} ${buildbase}/src/Executable/program
  fi

  # separate install target
  atf_check -s exit:0 -o save:build3.log -e save:build3.err make -f makefile VARIANT=${variant} install
  atf_check -s exit:0 [ -d installdir ]
  
  pushd ${buildbase}/src/Executable
    atf_check -s exit:0 -o save:symbol objdump -t program
  popd
  atf_check -s exit:${symbol_exit} -o ignore diff ../canon/symbol ${buildbase}/src/Executable/symbol
  atf_check -s exit:0 -o empty rm ${buildbase}/src/Executable/symbol
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
  atf_check -s exit:${symbol_exit} -o ignore diff ../canon/symbol installdir/bin/symbol
  atf_check -s exit:0 -o empty rm installdir/bin/symbol

  # test clean target
  atf_check -s exit:0 [ -e ${buildbase}/src/Executable/example_exe.o ]
  atf_check -s exit:0 [ -e ${buildbase}/src/Executable/example_exe.d ]
  atf_check -s exit:0 [ -e ${buildbase}/src/Library/example_lib.o ]
  atf_check -s exit:0 [ -e ${buildbase}/src/Library/example_lib.d ]
  atf_check -s exit:0 [ -e ${buildbase}/src/Executable/program ]
  if [ "X${shared}" = "X1" ] ; then
    atf_check -s exit:0 [ -e ${buildbase}/src/Library/library.so ]
  else
    atf_check -s exit:0 [ -e ${buildbase}/src/Library/library.a ]
  fi
  atf_check -s exit:0 [ -e installdir/bin/program ]
  if [ "X${shared}" = "X1" ] ; then
    atf_check -s exit:0 [ -e installdir/lib/library.so ]
  else
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
  fi
  # perform clean
  atf_check -s exit:0 -o save:build4.log -e save:build4.err make -f makefile VARIANT=${variant} clean
  atf_check -s exit:0 [ ! -e ${buildbase}/src/Executable/example_exe.o ]
  atf_check -s exit:0 [ -e ${buildbase}/src/Executable/example_exe.d ]
  atf_check -s exit:0 [ ! -e ${buildbase}/src/Library/example_lib.o ]
  atf_check -s exit:0 [ -e ${buildbase}/src/Library/example_lib.d ]
  atf_check -s exit:0 [ -e ${buildbase}/src/Executable/program ]
  if [ "X${shared}" = "X1" ] ; then
    atf_check -s exit:0 [ -e ${buildbase}/src/Library/library.so ]
  else
    atf_check -s exit:0 [ ! -e ${buildbase}/src/Library/library.a ]
  fi
  atf_check -s exit:0 [ -e installdir/bin/program ]
  if [ "X${shared}" = "X1" ] ; then
    atf_check -s exit:0 [ -e installdir/lib/library.so ]
  else
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
  fi
  # distclean target
  atf_check -s exit:0 -o save:build5.log -e save:build5.err make -f makefile VARIANT=${variant} distclean
  atf_check -s exit:0 [ ! -e ${buildbase}/src/Executable/example_exe.o ]
  atf_check -s exit:0 [ ! -e ${buildbase}/src/Executable/example_exe.d ]
  atf_check -s exit:0 [ ! -e ${buildbase}/src/Library/example_lib.o ]
  atf_check -s exit:0 [ ! -e ${buildbase}/src/Library/example_lib.d ]
  atf_check -s exit:0 [ ! -e ${buildbase}/src/Executable/program ]
  if [ "X${shared}" = "X1" ] ; then
    atf_check -s exit:0 [ ! -e ${buildbase}/src/Library/library.so ]
  else
    atf_check -s exit:0 [ ! -e ${buildbase}/src/Library/library.a ]
  fi
  atf_check -s exit:0 [ -e installdir/bin/program ]
  if [ "X${shared}" = "X1" ] ; then
    atf_check -s exit:0 [ -e installdir/lib/library.so ]
  else
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
  fi
  atf_check -s exit:0 -o empty rm -rf installdir
  atf_check -s exit:0 -o empty rm build1.log build1.err
  atf_check -s exit:0 -o empty rm build2.log build2.err
  atf_check -s exit:0 -o empty rm build3.log build3.err
  atf_check -s exit:0 -o empty rm build4.log build4.err
  atf_check -s exit:0 -o empty rm build5.log build5.err
  popd
}


atf_test_case property_exeart_description_make_gen
property_exeart_description_make_gen_head() {
  atf_set "descr" "Description Executable.Target(HDR/FLAGS/LINK) with boost generator."
}
property_exeart_description_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Executable.Target.(HDR/FLAGS/LINK) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeart-description.patch
  run_plugouts "property_test" "make" "exeart-description"
  check_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exeart_libtype_shared_make_gen
property_exeart_libtype_shared_make_gen_head() {
  atf_set "descr" "Property Executable.Target(butter type=shared) with boost generator."
}
property_exeart_libtype_shared_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Target.butter type + shared 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeart-libtype-shared.patch
  run_plugouts "property_test" "make" "exeart-libtype-shared"
  check_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exeart_libtype_static_make_gen
property_exeart_libtype_static_make_gen_head() {
  atf_set "descr" "Property Executable.Target(butter type=static) with boost generator."
}
property_exeart_libtype_static_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Target.butter type + static 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeart-libtype-static.patch
  run_plugouts "property_test" "make" "exeart-libtype-static"
  check_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exeart_include_make_gen
property_exeart_include_make_gen_head() {
  atf_set "descr" "Property Executable.Target(butter include) with boost generator."
}
property_exeart_include_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Target.butter include + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeart-include.patch
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\npatching file output/include/other/exe_message.hpp\n" patch -p0 <patch/other_header.patch
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exe_source_include.patch
  run_plugouts "property_test" "make" "exeart-include"
  check_build 0 0 1 0
  # "include" for a lib.source file should be only to this source

  # default (DEBUG) VARIANT
  standard_build_test "" output_exe_include
  # specific DEBUG VARIANT
  standard_build_test debug output_exe_include
  # RELEASE VARIANT
  standard_build_test release output_exe_include

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exeart_ldflags_make_gen
property_exeart_ldflags_make_gen_head() {
  atf_set "descr" "Property Executable.Target(butter ldflags) with boost generator."
}
property_exeart_ldflags_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Target.butter ldflags + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeart-ldflags.patch
  run_plugouts "property_test" "make" "exeart-ldflags"
  check_build 0 0 1 0

  # LDFlags for a lib.source file should apply to the library users
  # default (DEBUG) VARIANT
  standard_build_test "" output_default 0
  # specific DEBUG VARIANT
  standard_build_test debug output_default 0
  # RELEASE VARIANT
  standard_build_test release output_default 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exeart_flags_make_gen
property_exeart_flags_make_gen_head() {
  atf_set "descr" "Property Executable.Target(butter flags) with boost generator."
}
property_exeart_flags_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Target.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeart-flags.patch
  run_plugouts "property_test" "make" "exeart-flags"
  check_build 0 0 1 0

# Flags for a lib.source file should apply to itself only

  # default (DEBUG) VARIANT
  standard_build_test "" output_exesrc_flags
  # specific DEBUG VARIANT
  standard_build_test debug output_exesrc_flags
  # RELEASE VARIANT
  standard_build_test release output_exesrc_flags

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_libart_description_make_gen
property_libart_description_make_gen_head() {
  atf_set "descr" "Description Library.Target(HDR/FLAGS/LINK) with boost generator."
}
property_libart_description_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Library.Target.(HDR/FLAGS/LINK) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libart-description.patch
  run_plugouts "property_test" "make" "libart-description"
  check_build 0 0 1 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "" 0
  # specific DEBUG VARIANT
  standard_build_test debug "" 0
  # RELEASE VARIANT
  standard_build_test release "" 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libart_libtype_shared_make_gen
property_libart_libtype_shared_make_gen_head() {
  atf_set "descr" "Property Library.Target(butter type=shared) with boost generator."
}
property_libart_libtype_shared_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Target.butter type + shared 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libart-libtype-shared.patch
  run_plugouts "property_test" "make" "libart-libtype-shared"
  check_build 0 1 1 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "" "" 1
  # specific DEBUG VARIANT
  standard_build_test debug "" "" 1
  # RELEASE VARIANT
  standard_build_test release "" "" 1

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libart_libtype_static_make_gen
property_libart_libtype_static_make_gen_head() {
  atf_set "descr" "Property Library.Target(butter type=static) with boost generator."
}
property_libart_libtype_static_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Target.butter type + static 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libart-libtype-static.patch
  run_plugouts "property_test" "make" "libart-libtype-static"
  check_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libart_include_make_gen
property_libart_include_make_gen_head() {
  atf_set "descr" "Property Library.Target(butter include) with boost generator."
}
property_libart_include_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Target.butter include + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libart-include.patch
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\npatching file output/include/other/exe_message.hpp\n" patch -p0 <patch/other_header.patch
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/lib_source_include.patch
  run_plugouts "property_test" "make" "libart-include"
  check_build 0 1 1 0
  # "include" for a lib.source file should be only to this source

  # default (DEBUG) VARIANT
  standard_build_test "" output_top_include
  # specific DEBUG VARIANT
  standard_build_test debug output_top_include
  # RELEASE VARIANT
  standard_build_test release output_top_include

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libart_ldflags_make_gen
property_libart_ldflags_make_gen_head() {
  atf_set "descr" "Property Library.Target(butter ldflags) with boost generator."
}
property_libart_ldflags_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Target.butter ldflags + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libart-ldflags.patch
  run_plugouts "property_test" "make" "libart-ldflags"
  check_build 0 1 1 0

  # LDFlags for a lib.source file should apply to the library users
  # default (DEBUG) VARIANT
  standard_build_test "" output_default 0
  # specific DEBUG VARIANT
  standard_build_test debug output_default 0
  # RELEASE VARIANT
  standard_build_test release output_default 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libart_flags_make_gen
property_libart_flags_make_gen_head() {
  atf_set "descr" "Property Library.Target(butter flags) with boost generator."
}
property_libart_flags_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Target.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libart-flags.patch
  run_plugouts "property_test" "make" "libart-flags"
  check_build 0 1 0 0

# Flags for a lib.source file should apply to itself only

  # default (DEBUG) VARIANT
  standard_build_test "" output_libsrc_flags
  # specific DEBUG VARIANT
  standard_build_test debug output_libsrc_flags
  # RELEASE VARIANT
  standard_build_test release output_libsrc_flags

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}



atf_test_case property_exesrc_description_make_gen
property_exesrc_description_make_gen_head() {
  atf_set "descr" "Description Executable.Source(HDR/FLAGS/LINK) with boost generator."
}
property_exesrc_description_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Executable.Source.(HDR/FLAGS/LINK) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exesrc-description.patch
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exe_source_include.patch
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\npatching file output/include/other/exe_message.hpp\n" patch -p0 <patch/other_header.patch
  run_plugouts "property_test" "make" "exesrc-description"
  check_build 0 0 1 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" output_exe_include 0
  # specific DEBUG VARIANT
  standard_build_test debug output_exe_include 0
  # RELEASE VARIANT
  standard_build_test release output_exe_include 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exesrc_libtype_shared_make_gen
property_exesrc_libtype_shared_make_gen_head() {
  atf_set "descr" "Property Executable.Source(butter type=shared) with boost generator."
}
property_exesrc_libtype_shared_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Source.butter type + shared 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exesrc-libtype-shared.patch
  run_plugouts "property_test" "make" "exesrc-libtype-shared"
  check_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exesrc_libtype_static_make_gen
property_exesrc_libtype_static_make_gen_head() {
  atf_set "descr" "Property Executable.Source(butter type=static) with boost generator."
}
property_exesrc_libtype_static_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Source.butter type + static 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exesrc-libtype-static.patch
  run_plugouts "property_test" "make" "exesrc-libtype-static"
  check_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exesrc_include_make_gen
property_exesrc_include_make_gen_head() {
  atf_set "descr" "Property Executable.Source(butter include) with boost generator."
}
property_exesrc_include_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Source.butter include + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exesrc-include.patch
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\npatching file output/include/other/exe_message.hpp\n" patch -p0 <patch/other_header.patch
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exe_source_include.patch
  run_plugouts "property_test" "make" "exesrc-include"
  check_build 0 0 1 0
  # "include" for a lib.source file should be only to this source

  # default (DEBUG) VARIANT
  standard_build_test "" output_exe_include
  # specific DEBUG VARIANT
  standard_build_test debug output_exe_include
  # RELEASE VARIANT
  standard_build_test release output_exe_include

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exesrc_ldflags_make_gen
property_exesrc_ldflags_make_gen_head() {
  atf_set "descr" "Property Executable.Source(butter ldflags) with boost generator."
}
property_exesrc_ldflags_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Source.butter ldflags + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exesrc-ldflags.patch
  run_plugouts "property_test" "make" "exesrc-ldflags"
  check_build 0 0 1 0

  # LDFlags for a exe.source file should apply to the exe target
  # default (DEBUG) VARIANT
  standard_build_test "" output_default 0
  # specific DEBUG VARIANT
  standard_build_test debug output_default 0
  # RELEASE VARIANT
  standard_build_test release output_default 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exesrc_flags_make_gen
property_exesrc_flags_make_gen_head() {
  atf_set "descr" "Property Executable.Source(butter flags) with boost generator."
}
property_exesrc_flags_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.Source.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exesrc-flags.patch
  run_plugouts "property_test" "make" "exesrc-flags"
  check_build 0 0 1 0

  # Flags for a exe.source file should apply to itself only

  # default (DEBUG) VARIANT
  standard_build_test "" output_exesrc_flags
  # specific DEBUG VARIANT
  standard_build_test debug output_exesrc_flags
  # RELEASE VARIANT
  standard_build_test release output_exesrc_flags

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_libsrc_description_make_gen
property_libsrc_description_make_gen_head() {
  atf_set "descr" "Description Library.Source(HDR/FLAGS/LINK) with boost generator."
}
property_libsrc_description_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Library.Source.(HDR/FLAGS/LINK) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-description.patch
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\npatching file output/include/other/exe_message.hpp\n" patch -p0 <patch/other_header.patch
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/lib_source_include.patch
  run_plugouts "property_test" "make" "libsrc-description"
  check_build 0 1 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" output_top_include 
  # specific DEBUG VARIANT
  standard_build_test debug output_top_include 
  # RELEASE VARIANT
  standard_build_test release output_top_include 

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libsrc_libtype_shared_make_gen
property_libsrc_libtype_shared_make_gen_head() {
  atf_set "descr" "Property Library.Source(butter type=shared) with boost generator."
}
property_libsrc_libtype_shared_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter type + shared 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-libtype-shared.patch
  run_plugouts "property_test" "make" "libsrc-libtype-shared"
  check_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libsrc_libtype_static_make_gen
property_libsrc_libtype_static_make_gen_head() {
  atf_set "descr" "Property Library.Source(butter type=static) with boost generator."
}
property_libsrc_libtype_static_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter type + static 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-libtype-static.patch
  run_plugouts "property_test" "make" "libsrc-libtype-static"
  check_build 0 1 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libsrc_include_make_gen
property_libsrc_include_make_gen_head() {
  atf_set "descr" "Property Library.Source(butter include) with boost generator."
}
property_libsrc_include_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter include + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-include.patch
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\npatching file output/include/other/exe_message.hpp\n" patch -p0 <patch/other_header.patch
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/lib_source_include.patch
  run_plugouts "property_test" "make" "libsrc-include"
  check_build 0 1 0 0
  # "include" for a lib.source file should be only to this source

  # default (DEBUG) VARIANT
  standard_build_test "" output_top_include
  # specific DEBUG VARIANT
  standard_build_test debug output_top_include
  # RELEASE VARIANT
  standard_build_test release output_top_include

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libsrc_ldflags_make_gen
property_libsrc_ldflags_make_gen_head() {
  atf_set "descr" "Property Library.Source(butter ldflags) with boost generator."
}
property_libsrc_ldflags_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter ldflags + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-ldflags.patch
  run_plugouts "property_test" "make" "libsrc-ldflags"
  check_build 0 1 1 0

  # LDFlags for a lib.source file should apply to the library users
  # default (DEBUG) VARIANT
  standard_build_test "" output_default 0
  # specific DEBUG VARIANT
  standard_build_test debug output_default 0
  # RELEASE VARIANT
  standard_build_test release output_default 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libsrc_flags_make_gen
property_libsrc_flags_make_gen_head() {
  atf_set "descr" "Property Library.Source(butter flags) with boost generator."
}
property_libsrc_flags_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-flags.patch
  run_plugouts "property_test" "make" "libsrc-flags"
  check_build 0 1 0 0

  # Flags for a lib.source file should apply to itself only

  # default (DEBUG) VARIANT
  standard_build_test "" output_libsrc_flags
  # specific DEBUG VARIANT
  standard_build_test debug output_libsrc_flags
  # RELEASE VARIANT
  standard_build_test release output_libsrc_flags

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exe_description_make_gen
property_exe_description_make_gen_head() {
  atf_set "descr" "Description Executable Package(HDR/FLAGS/LINK) with boost generator."
}
property_exe_description_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Executable.(HDR/FLAGS/LINK) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-description.patch
  run_plugouts "property_test" "make" "exeproj-description"
  check_build 0 0 0 0

  # Flags for a executable should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exe_libtype_shared_make_gen
property_exe_libtype_shared_make_gen_head() {
  atf_set "descr" "Property Executable Package(butter type=shared) with boost generator."
}
property_exe_libtype_shared_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter type + shared 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-libtype-shared.patch
  run_plugouts "property_test" "make" "exeproj-libtype-shared"
  check_build 0 0 0 0

  # Flags for a executable should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exe_libtype_static_make_gen
property_exe_libtype_static_make_gen_head() {
  atf_set "descr" "Property Executable Package(butter type=static) with boost generator."
}
property_exe_libtype_static_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter type + static 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-libtype-static.patch
  run_plugouts "property_test" "make" "exeproj-libtype-static"
  check_build 0 0 0 0

  # Flags for a executable should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exe_include_make_gen
property_exe_include_make_gen_head() {
  atf_set "descr" "Property Executable Package(butter include) with boost generator."
}
property_exe_include_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter include + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-include.patch
  run_plugouts "property_test" "make" "exeproj-include"
  check_build 0 0 0 0
  # "include" for a executable project should be included for the library
  # and its users?

  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exe_ldflags_make_gen
property_exe_ldflags_make_gen_head() {
  atf_set "descr" "Property Executable Package(butter ldflags) with boost generator."
}
property_exe_ldflags_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter ldflags + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-ldflags.patch
  run_plugouts "property_test" "make" "exeproj-ldflags"
  check_build 0 0 0 0

  # Flags for a executable should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exe_flags_make_gen
property_exe_flags_make_gen_head() {
  atf_set "descr" "Property Executable Package(butter flags) with boost generator."
}
property_exe_flags_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-flags.patch
  run_plugouts "property_test" "make" "exeproj-flags"
  check_build 0 0 0 0

  # Flags for a executable should apply to the using class?

  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_lib_description_make_gen
property_lib_description_make_gen_head() {
  atf_set "descr" "Description Library Package(HDR/FLAGS/LINK) with boost generator."
}
property_lib_description_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Library.(HDR/FLAGS/LINK) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-description.patch
  run_plugouts "property_test" "make" "libproj-description"
  check_build 0 0 0 0

  # Flags for a library should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_lib_libtype_shared_make_gen
property_lib_libtype_shared_make_gen_head() {
  atf_set "descr" "Property Library Package(butter type=shared) with boost generator."
}
property_lib_libtype_shared_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter type + shared 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-libtype-shared.patch
  run_plugouts "property_test" "make" "libproj-libtype-shared"
  check_build 0 1 1 0

  # Flags for a library should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" "" "" 1
  # specific DEBUG VARIANT
  standard_build_test debug "" "" 1
  # RELEASE VARIANT
  standard_build_test release "" "" 1

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_lib_libtype_static_make_gen
property_lib_libtype_static_make_gen_head() {
  atf_set "descr" "Property Library Package(butter type=static) with boost generator."
}
property_lib_libtype_static_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter type + static 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-libtype-static.patch
  run_plugouts "property_test" "make" "libproj-libtype-static"
  check_build 0 0 0 0

  # Flags for a library should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_lib_include_make_gen
property_lib_include_make_gen_head() {
  atf_set "descr" "Property Library Package(butter include) with boost generator."
}
property_lib_include_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter include + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-include.patch
  run_plugouts "property_test" "make" "libproj-include"
  check_build 0 0 0 0
  # "include" for a library project should be included for the library
  # and its users?

  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_lib_ldflags_make_gen
property_lib_ldflags_make_gen_head() {
  atf_set "descr" "Property Library Package(butter ldflags) with boost generator."
}
property_lib_ldflags_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter ldflags + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-ldflags.patch
  run_plugouts "property_test" "make" "libproj-ldflags"
  check_build 0 0 0 0

  # Flags for a library should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_lib_flags_make_gen
property_lib_flags_make_gen_head() {
  atf_set "descr" "Property Library Package(butter flags) with boost generator."
}
property_lib_flags_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-flags.patch
  run_plugouts "property_test" "make" "libproj-flags"
  check_build 0 0 0 0

  # Flags for a library should apply to the using class?

  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_top_description_make_gen
property_top_description_make_gen_head() {
  atf_set "descr" "Description (*top*)Package (FLAGS=..) with boost generator."
}
property_top_description_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # PROJECT.description : FLAGS=... \ HDR=... \ LINK=...
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make" "top_description"
  run_plugouts "property_test" "make" "top_description"
  check_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_top_ldflags_make_gen
property_top_ldflags_make_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter ldflags) with boost generator."
}
property_top_ldflags_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter ldflags + -s 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make-top" "top-ldflags"
  run_plugouts "property_test" "make" "top-ldflags"
  check_build 1 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" output_default 0
  # specific DEBUG VARIANT
  standard_build_test debug output_default 0
  # RELEASE VARIANT
  standard_build_test release output_default 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output

  # Test of ldflags that are really for flags to see
  # if the ldflags are used during compilation as well
  # as only during linking. 
  setup_example "property_test" "make-top" "top-ldflags-guard"
  run_plugouts "property_test" "make" "top-ldflags-guard"
  check_build 1 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output

  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_top_include_make_gen
property_top_include_make_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter include) with boost generator."
}
property_top_include_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter include + "other" 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make-top" "top-include"
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\npatching file output/include/other/exe_message.hpp\n" patch -p0 <patch/other_header.patch
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/lib_source_include.patch
  run_plugouts "property_test" "make" "top-include"
  check_build 1 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" "output_top_include"
  # specific DEBUG VARIANT
  standard_build_test debug "output_top_include"
  # RELEASE VARIANT
  standard_build_test release "output_top_include"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_top_libtype_shared_make_gen
property_top_libtype_shared_make_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter type = shared) with boost generator."
}
property_top_libtype_shared_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter type + static
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make-top" "top-libtype-shared"
  run_plugouts "property_test" "make" "top-libtype-shared"
  check_build 0 1 1 0
 
  # default (DEBUG) VARIANT
  standard_build_test "" "" "" 1
  # specific DEBUG VARIANT
  standard_build_test debug "" "" 1
  # RELEASE VARIANT
  standard_build_test release "" "" 1

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_top_libtype_static_make_gen
property_top_libtype_static_make_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter type = static) with boost generator."
}
property_top_libtype_static_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter type + static
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make-top" "top-libtype-static"
  run_plugouts "property_test" "make" "top-libtype-static"
  check_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_top_flags_make_gen
property_top_flags_make_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter flags) with boost generator."
}
property_top_flags_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make-top" "top-flags"
  run_plugouts "property_test" "make" "top-flags"
  check_build 1 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" "output_top_flags"
  # specific DEBUG VARIANT
  standard_build_test debug "output_top_flags"
  # RELEASE VARIANT
  standard_build_test release "output_top_flags"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_version_make_gen
property_version_make_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter version) with boost generator."
}
property_version_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter include + "other" 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make-top" "version"
  run_plugouts "property_test" "make" "version"
  check_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_log_level_make_gen
property_log_level_make_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter log-level) with boost generator."
}
property_log_level_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter log-level = 0 (default) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make-top" "loglevel0"
  run_plugouts "property_test" "make" "loglevel0"
  atf_check -o empty diff canon/butter.log.boost canon/butter.log.boost.loglevel0 
  atf_check -s exit:0 [ -e output/include/butter.log ]
  atf_check -s exit:1 [ -s output/include/butter.log ]
  check_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output

  #----------------------- 
  # Property: PROJECT.butter log-level = 1 (information) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make-top" "loglevel1"
  run_plugouts "property_test" "make" "loglevel1"
  atf_check -s exit:0 [ -e output/include/butter.log ]
  atf_check -s exit:1 [ -s output/include/butter.log ]
  check_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output

  #----------------------- 
  # Property: PROJECT.butter log-level = 2 (debug) 
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make-top" "loglevel2"
  run_plugouts "property_test" "make" "loglevel2"
  atf_check -s exit:0 [ -e output/include/butter.log ]
  atf_check -s exit:1 [ -s output/include/butter.log ]
  check_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output

  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_basedir_make_gen
property_basedir_make_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter base) with boost generator."
}
property_basedir_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter base + ../..
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make-top" "basedir"
  run_plugouts "property_test" "make" "basedir"
  atf_check -s exit:0 [ -e makefile ]
  atf_check -s exit:0 [ -e M_cl.mk ]
  atf_check -s exit:0 [ -e M_gcc.mk ]
  atf_check -s exit:0 [ -e M_sys.mk ]
  atf_check -s exit:0 [ -e M_unix.mk ]
  atf_check -s exit:0 [ -e M_Windows_NT.mk ]
  build_test(){
    local variant=$1
    local output_fname=${2:-output_default}
    local symbol_exit=${3:-1}
    local shared=${4:-0}
    local buildbase=${5:-.}
    # test base build target
    atf_check -s exit:0 -o save:build1.log -e save:build1.err make -f makefile VARIANT=${variant}
    atf_check -s exit:0 -o save:build2.log -e save:build2.err make -f makefile VARIANT=${variant}
    atf_check -s exit:0 [ -x output/src/Executable/program ]
    atf_check -s exit:0 [ -e output/src/Library/library.a ]
    atf_check -s exit:0 -o file:canon/output_default output/src/Executable/program
  
    # separate install target
    atf_check -s exit:0 -o save:build3.log -e save:build3.err make -f makefile VARIANT=${variant} install
    atf_check -s exit:0 [ -d installdir ]
    
    pushd output/src/Executable
      atf_check -s exit:0 -o save:symbol objdump -t program
    popd
    atf_check -s exit:1 -o ignore diff canon/symbol output/src/Executable/symbol
    atf_check -s exit:0 -o empty rm output/src/Executable/symbol
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:canon/output_default installdir/bin/program
    
    pushd installdir/bin
      atf_check -s exit:0 -o save:symbol objdump -t program
    popd
    atf_check -s exit:1 -o ignore diff canon/symbol installdir/bin/symbol
    atf_check -s exit:0 -o empty rm installdir/bin/symbol
  
    # test clean target
    atf_check -s exit:0 [ -e output/src/Executable/example_exe.o ]
    atf_check -s exit:0 [ -e output/src/Executable/example_exe.d ]
    atf_check -s exit:0 [ -e output/src/Library/example_lib.o ]
    atf_check -s exit:0 [ -e output/src/Library/example_lib.d ]
    atf_check -s exit:0 [ -e output/src/Executable/program ]
    atf_check -s exit:0 [ -e output/src/Library/library.a ]
    atf_check -s exit:0 [ -e installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    # perform clean
    atf_check -s exit:0 -o save:build4.log -e save:build4.err make -f makefile VARIANT=${variant} clean
    atf_check -s exit:0 [ ! -e output/src/Executable/example_exe.o ]
    atf_check -s exit:0 [ -e output/src/Executable/example_exe.d ]
    atf_check -s exit:0 [ ! -e output/src/Library/example_lib.o ]
    atf_check -s exit:0 [ -e output/src/Library/example_lib.d ]
    atf_check -s exit:0 [ -e output/src/Executable/program ]
    atf_check -s exit:0 [ ! -e output/src/Library/library.a ]
    atf_check -s exit:0 [ -e installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    # distclean target
    atf_check -s exit:0 -o save:build5.log -e save:build5.err make -f makefile VARIANT=${variant} distclean
    atf_check -s exit:0 [ ! -e output/src/Executable/example_exe.o ]
    atf_check -s exit:0 [ ! -e output/src/Executable/example_exe.d ]
    atf_check -s exit:0 [ ! -e output/src/Library/example_lib.o ]
    atf_check -s exit:0 [ ! -e output/src/Library/example_lib.d ]
    atf_check -s exit:0 [ ! -e output/src/Executable/program ]
    atf_check -s exit:0 [ ! -e output/src/Library/library.a ]
    atf_check -s exit:0 [ -e installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o empty rm -rf installdir
    atf_check -s exit:0 -o empty rm build1.log build1.err
    atf_check -s exit:0 -o empty rm build2.log build2.err
    atf_check -s exit:0 -o empty rm build3.log build3.err
    atf_check -s exit:0 -o empty rm build4.log build4.err
    atf_check -s exit:0 -o empty rm build5.log build5.err
  }

  # default (DEBUG) VARIANT
  build_test
  # specific DEBUG VARIANT
  build_test debug
  # RELEASE VARIANT
  build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty rm makefile
  atf_check -s exit:0 -o empty rm M_cl.mk  M_gcc.mk  M_sys.mk  M_unix.mk  M_Windows_NT.mk
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
}

atf_test_case property_builddir_make_gen
property_builddir_make_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter build-dir) with boost generator."
}
property_builddir_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter build-dir + BUILD
  # Standard (boost) variant 
  #----------------------- 
  setup_example "property_test" "make-top" "builddir"
  run_plugouts "property_test" "make" "builddir"
  check_build 0 0 0 0
  # check_build 1 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test # "" "" "" "" BUILD
  # specific DEBUG VARIANT
  standard_build_test debug # "" "" "" BUILD
  # RELEASE VARIANT
  standard_build_test release # "" "" "" BUILD

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_test_make_gen
property_test_make_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
property_test_make_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Standard (boost) variant without defined properties (nul test)
  #----------------------- 
  setup_example "property_test" "make"
  run_plugouts "property_test" "make" "nultest"
  check_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test
  # specific DEBUG VARIANT
  standard_build_test debug
  # RELEASE VARIANT
  standard_build_test release

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_init_test_cases() {
    atf_add_test_case property_test_make_gen
    atf_add_test_case property_builddir_make_gen
    atf_add_test_case property_basedir_make_gen
    atf_add_test_case property_version_make_gen
    atf_add_test_case property_log_level_make_gen
    atf_add_test_case property_top_flags_make_gen
    atf_add_test_case property_top_ldflags_make_gen
    atf_add_test_case property_top_include_make_gen
    atf_add_test_case property_top_description_make_gen
    atf_add_test_case property_top_libtype_static_make_gen
    atf_add_test_case property_top_libtype_shared_make_gen
    atf_add_test_case property_lib_flags_make_gen
    atf_add_test_case property_lib_ldflags_make_gen
    atf_add_test_case property_lib_include_make_gen
    atf_add_test_case property_lib_libtype_static_make_gen
    atf_add_test_case property_lib_libtype_shared_make_gen
    atf_add_test_case property_lib_description_make_gen
    atf_add_test_case property_exe_flags_make_gen
    atf_add_test_case property_exe_ldflags_make_gen
    atf_add_test_case property_exe_include_make_gen
    atf_add_test_case property_exe_libtype_static_make_gen
    atf_add_test_case property_exe_libtype_shared_make_gen
    atf_add_test_case property_exe_description_make_gen
    atf_add_test_case property_libsrc_flags_make_gen
    atf_add_test_case property_libsrc_ldflags_make_gen
    atf_add_test_case property_libsrc_include_make_gen
    atf_add_test_case property_libsrc_libtype_static_make_gen
    atf_add_test_case property_libsrc_libtype_shared_make_gen
    atf_add_test_case property_libsrc_description_make_gen
    atf_add_test_case property_exesrc_flags_make_gen
    atf_add_test_case property_exesrc_ldflags_make_gen
    atf_add_test_case property_exesrc_include_make_gen
    atf_add_test_case property_exesrc_libtype_static_make_gen
    atf_add_test_case property_exesrc_libtype_shared_make_gen
    atf_add_test_case property_exesrc_description_make_gen
    atf_add_test_case property_libart_flags_make_gen
    atf_add_test_case property_libart_ldflags_make_gen
    atf_add_test_case property_libart_include_make_gen
    atf_add_test_case property_libart_libtype_static_make_gen
    atf_add_test_case property_libart_libtype_shared_make_gen
    atf_add_test_case property_libart_description_make_gen
    atf_add_test_case property_exeart_flags_make_gen
    atf_add_test_case property_exeart_ldflags_make_gen
    atf_add_test_case property_exeart_include_make_gen
    atf_add_test_case property_exeart_libtype_static_make_gen
    atf_add_test_case property_exeart_libtype_shared_make_gen
    atf_add_test_case property_exeart_description_make_gen


}

