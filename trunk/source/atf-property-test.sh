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

check_jam_build(){
  # Test if build files are similar or different to canonical version
  # usage check_jam_build [0|!0] [0|!0] [0|!0] [0|!0]
  #   0 : asserts top, lib, exe and/or sys files match canon
  #   !0 : asserts top, lib, exe and/or sys files do not match canon
  #   anything else : do not check.
  local top=$1
  local lib=$2
  local exe=$3
  local sys=$4
  if [ 0 -eq ${top} ]; then
     atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/Jamfile_jam_top output/Jamfile
  else
     atf_check -o ignore -s exit:1 diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/Jamfile_jam_top output/Jamfile
  fi
  if [ 0 -eq ${lib} ]; then
     atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/Jamfile_jam_lib output/src/Library/Jamfile
  else
     atf_check -o ignore -s exit:1 diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/Jamfile_jam_lib output/src/Library/Jamfile
  fi
  if [ 0 -eq ${exe} ]; then
     atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/Jamfile_jam_exe output/src/Executable/Jamfile
  else
     atf_check -o ignore -s exit:1 diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/Jamfile_jam_exe output/src/Executable/Jamfile
  fi
  if [ 0 -eq ${sys} ]; then
     atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/Jamrules_sys output/Jamrules
  else
     atf_check -o ignore -s exit:1 diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" canon/Jamrules_sys output/Jamrules
  fi
}

standard_build_test(){
  local variant=$1
  local builddir=$2
  local output_fname=${3:-output_default}
  local symbol_exit=${4:-1}
  pushd output
  # test base build target
  atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam -d2 ${variant}
  atf_check -s exit:0 [ -x ${builddir}/src/Executable/program ]
  atf_check -s exit:0 [ -e ${builddir}/src/Library/library.a ]
  atf_check -s exit:0 -o file:../canon/${output_fname} ${builddir}/src/Executable/program
  pushd ${builddir}/src/Executable
  atf_check -s exit:0 -o save:symbol objdump -t program
  popd
  atf_check -s exit:${symbol_exit} -o ignore diff ../canon/symbol ${builddir}/src/Executable/symbol

  # test install target
  atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam -d2 install ${variant}
  atf_check -s exit:0 [ -x installdir/bin/program ]
  atf_check -s exit:0 [ -e installdir/lib/library.a ]
  atf_check -s exit:0 -o file:../canon/${output_fname} installdir/bin/program
  pushd installdir/bin
  atf_check -s exit:0 -o save:symbol objdump -t program
  popd
  atf_check -s exit:${symbol_exit} -o ignore diff ../canon/symbol installdir/bin/symbol

  # no distclean target
  # test clean target
  atf_check -s exit:0 -o save:jam3.log -e save:jam3.err jam clean ${variant}
  atf_check -s exit:0 [ ! -e ${builddir}/src/Executable/program ]
  atf_check -s exit:0 [ ! -e ${builddir}/src/Library/library.a ]
  atf_check -s exit:0 [ -e installdir/bin/program ]
  atf_check -s exit:0 [ -e installdir/lib/library.a ]

  atf_check -s exit:0 -o empty rm jam1.log jam1.err
  atf_check -s exit:0 -o empty rm jam2.log jam2.err
  atf_check -s exit:0 -o empty rm jam3.log jam3.err
  atf_check -s exit:0 -o empty rm -rf ${builddir}
  atf_check -s exit:0 -o empty rm -rf installdir
  popd
}


atf_test_case property_libsrc_description_jam_gen
property_libsrc_description_jam_gen_head() {
  atf_set "descr" "Description Library.Source(HDR/FLAGS/LINK) with jam generator."
}
property_libsrc_description_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Library.Source.(HDR/FLAGS/LINK) 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-description.patch
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\n" patch -p0 <patch/other_header.patch
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/lib_source_include.patch
  run_plugouts "property_test" "jam" "libsrc-description"
  check_jam_build 0 1 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG output_top_include 0
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG output_top_include 0
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE output_top_include 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libsrc_libtype_shared_jam_gen
property_libsrc_libtype_shared_jam_gen_head() {
  atf_set "descr" "Property Library.Source(butter type=shared) with jam generator."
}
property_libsrc_libtype_shared_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter type + shared 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-libtype-shared.patch
  run_plugouts "property_test" "jam" "libsrc-libtype-shared"
  check_jam_build 0 0 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libsrc_libtype_static_jam_gen
property_libsrc_libtype_static_jam_gen_head() {
  atf_set "descr" "Property Library.Source(butter type=static) with jam generator."
}
property_libsrc_libtype_static_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter type + static 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-libtype-static.patch
  run_plugouts "property_test" "jam" "libsrc-libtype-static"
  check_jam_build 0 1 0 0

  # Flags for a lib.source file should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libsrc_include_jam_gen
property_libsrc_include_jam_gen_head() {
  atf_set "descr" "Property Library.Source(butter include) with jam generator."
}
property_libsrc_include_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter include + -s 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-include.patch
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\n" patch -p0 <patch/other_header.patch
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/lib_source_include.patch
  run_plugouts "property_test" "jam" "libsrc-include"
  check_jam_build 0 1 0 0
  # "include" for a lib.source file should be only to this source

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG output_top_include
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG output_top_include
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE output_top_include

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_libsrc_ldflags_jam_gen
property_libsrc_ldflags_jam_gen_head() {
  atf_set "descr" "Property Library.Source(butter ldflags) with jam generator."
}
property_libsrc_ldflags_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter ldflags + -s 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-ldflags.patch
  run_plugouts "property_test" "jam" "libsrc-ldflags"
  check_jam_build 0 1 0 0

  # LDFlags for a lib.source file should apply to the library users
  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG output_default 0
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG output_default 0
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE output_default 0

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_libsrc_flags_jam_gen
property_libsrc_flags_jam_gen_head() {
  atf_set "descr" "Property Library.Source(butter flags) with jam generator."
}
property_libsrc_flags_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.Source.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libsrc-flags.patch
  run_plugouts "property_test" "jam" "libsrc-flags"
  check_jam_build 0 1 0 0

# Flags for a lib.source file should apply to itself only

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG output_libsrc_flags
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG output_libsrc_flags
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE output_libsrc_flags

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}



atf_test_case property_exe_description_jam_gen
property_exe_description_jam_gen_head() {
  atf_set "descr" "Description Executable Package(HDR/FLAGS/LINK) with jam generator."
}
property_exe_description_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Executable.(HDR/FLAGS/LINK) 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-description.patch
  run_plugouts "property_test" "jam" "exeproj-description"
  check_jam_build 0 0 0 0

  # Flags for a executable should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exe_libtype_shared_jam_gen
property_exe_libtype_shared_jam_gen_head() {
  atf_set "descr" "Property Executable Package(butter type=shared) with jam generator."
}
property_exe_libtype_shared_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter type + shared 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-libtype-shared.patch
  run_plugouts "property_test" "jam" "exeproj-libtype-shared"
  check_jam_build 0 0 0 0

  # Flags for a executable should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exe_libtype_static_jam_gen
property_exe_libtype_static_jam_gen_head() {
  atf_set "descr" "Property Executable Package(butter type=static) with jam generator."
}
property_exe_libtype_static_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter type + static 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-libtype-static.patch
  run_plugouts "property_test" "jam" "exeproj-libtype-static"
  check_jam_build 0 0 0 0

  # Flags for a executable should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exe_include_jam_gen
property_exe_include_jam_gen_head() {
  atf_set "descr" "Property Executable Package(butter include) with jam generator."
}
property_exe_include_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter include + -s 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-include.patch
  run_plugouts "property_test" "jam" "exeproj-include"
  check_jam_build 0 0 0 0
  # "include" for a executable project should be included for the library
  # and its users?

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_exe_ldflags_jam_gen
property_exe_ldflags_jam_gen_head() {
  atf_set "descr" "Property Executable Package(butter ldflags) with jam generator."
}
property_exe_ldflags_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter ldflags + -s 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-ldflags.patch
  run_plugouts "property_test" "jam" "exeproj-ldflags"
  check_jam_build 0 0 0 0

  # Flags for a executable should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_exe_flags_jam_gen
property_exe_flags_jam_gen_head() {
  atf_set "descr" "Property Executable Package(butter flags) with jam generator."
}
property_exe_flags_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Executable.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128169\n" patch <patch/exeproj-flags.patch
  run_plugouts "property_test" "jam" "exeproj-flags"
  check_jam_build 0 0 0 0

# Flags for a executable should apply to the using class?

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG 
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG 
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE 

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}



atf_test_case property_lib_description_jam_gen
property_lib_description_jam_gen_head() {
  atf_set "descr" "Description Library Package(HDR/FLAGS/LINK) with jam generator."
}
property_lib_description_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Description: Library.(HDR/FLAGS/LINK) 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-description.patch
  run_plugouts "property_test" "jam" "libproj-description"
  check_jam_build 0 0 0 0

  # Flags for a library should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_lib_libtype_shared_jam_gen
property_lib_libtype_shared_jam_gen_head() {
  atf_set "descr" "Property Library Package(butter type=shared) with jam generator."
}
property_lib_libtype_shared_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter type + shared 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-libtype-shared.patch
  run_plugouts "property_test" "jam" "libproj-libtype-shared"
  check_jam_build 0 0 0 0

  # Flags for a library should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_lib_libtype_static_jam_gen
property_lib_libtype_static_jam_gen_head() {
  atf_set "descr" "Property Library Package(butter type=static) with jam generator."
}
property_lib_libtype_static_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter type + static 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-libtype-static.patch
  run_plugouts "property_test" "jam" "libproj-libtype-static"
  check_jam_build 0 0 0 0

  # Flags for a library should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_lib_include_jam_gen
property_lib_include_jam_gen_head() {
  atf_set "descr" "Property Library Package(butter include) with jam generator."
}
property_lib_include_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter include + -s 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-include.patch
  run_plugouts "property_test" "jam" "libproj-include"
  check_jam_build 0 0 1 0
  # "include" for a library project should be included for the library
  # and its users?

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_lib_ldflags_jam_gen
property_lib_ldflags_jam_gen_head() {
  atf_set "descr" "Property Library Package(butter ldflags) with jam generator."
}
property_lib_ldflags_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter ldflags + -s 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-ldflags.patch
  run_plugouts "property_test" "jam" "libproj-ldflags"
  check_jam_build 0 0 0 0

  # Flags for a library should apply to the using class?
  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_lib_flags_jam_gen
property_lib_flags_jam_gen_head() {
  atf_set "descr" "Property Library Package(butter flags) with jam generator."
}
property_lib_flags_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: Library.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/libproj-flags.patch
  run_plugouts "property_test" "jam" "libproj-flags"
  check_jam_build 0 0 0 0

  # Flags for a library should apply to the using class?

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_version_jam_gen
property_version_jam_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter version) with jam generator."
}
property_version_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter include + "other" 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam-top" "version"
  run_plugouts "property_test" "jam" "version"
  check_jam_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_top_description_jam_gen
property_top_description_jam_gen_head() {
  atf_set "descr" "Description (*top*)Package (FLAGS=..) with jam generator."
}
property_top_description_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # PROJECT.description : FLAGS=... \ HDR=... \ LINK=...
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam" "top_description"
  run_plugouts "property_test" "jam" "top_description"
  check_jam_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_top_ldflags_jam_gen
property_top_ldflags_jam_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter ldflags) with jam generator."
}
property_top_ldflags_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter ldflags + -s 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam-top" "top-ldflags"
  run_plugouts "property_test" "jam" "top-ldflags"
  check_jam_build 1 0 0 0

  build_test(){
    local variant=$1
    local builddir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ -e ${builddir}/src/Library/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default ${builddir}/src/Executable/program
    pushd ${builddir}/src/Executable
    atf_check -s exit:0 -o file:../../../../canon/symbol objdump -t program
    popd

    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default installdir/bin/program
    pushd installdir/bin
    atf_check -s exit:0 -o file:../../../canon/symbol objdump -t program
    popd
    # no distclean target
    # test clean target
    atf_check -s exit:0 -o save:jam3.log -e save:jam3.err jam clean ${variant}
    atf_check -s exit:0 [ ! -e ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ ! -e ${builddir}/src/Library/library.a ]
    atf_check -s exit:0 [ -e installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]

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

  unset build_test

  # Test of ldflags that are really for flags to see
  # if the ldflags are used during compilation as well
  # as only during linking. 
  setup_example "property_test" "jam-top" "top-ldflags-guard"
  run_plugouts "property_test" "jam" "top-ldflags-guard"
  check_jam_build 1 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output


  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_top_include_jam_gen
property_top_include_jam_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter include) with jam generator."
}
property_top_include_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter include + "other" 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam-top" "top-include"
  atf_check -s exit:0 -o inline:"patching file output/include/other/lib_message.hpp\n" patch -p0 <patch/other_header.patch
  atf_check -s exit:0 -o inline:"patching file 128041\n" patch <patch/lib_source_include.patch
  run_plugouts "property_test" "jam" "top-include"
  check_jam_build 1 0 1 0

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG "output_top_include"
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG "output_top_include"
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE "output_top_include"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_top_libtype_shared_jam_gen
property_top_libtype_shared_jam_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter type = shared) with jam generator."
}
property_top_libtype_shared_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter type + static
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam-top" "top-libtype-shared"
  run_plugouts "property_test" "jam" "top-libtype-shared"
  atf_check -o empty diff canon/butter.log.jam canon/butter.log.jam.top-libtype-shared 
  check_jam_build 0 0 0 0
 
  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_top_libtype_static_jam_gen
property_top_libtype_static_jam_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter type = static) with jam generator."
}
property_top_libtype_static_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter type + static
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam-top" "top-libtype-static"
  run_plugouts "property_test" "jam" "top-libtype-static"
  atf_check -o empty diff canon/butter.log.jam canon/butter.log.jam.top-libtype-static 
  check_jam_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_log_level_jam_gen
property_log_level_jam_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter log-level) with jam generator."
}
property_log_level_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter log-level = 0 (default) 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam-top" "loglevel0"
  run_plugouts "property_test" "jam" "loglevel0"
  atf_check -o empty diff canon/butter.log.jam canon/butter.log.jam.loglevel0 
  atf_check -s exit:0 [ -e output/include/butter.log ]
  atf_check -s exit:1 [ -s output/include/butter.log ]
  check_jam_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output

  #----------------------- 
  # Property: PROJECT.butter log-level = 1 (information) 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam-top" "loglevel1"
  run_plugouts "property_test" "jam" "loglevel1"
  atf_check -s exit:0 [ -e output/include/butter.log ]
  atf_check -s exit:1 [ -s output/include/butter.log ]
  check_jam_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output



  #----------------------- 
  # Property: PROJECT.butter log-level = 2 (debug) 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam-top" "loglevel1"
  run_plugouts "property_test" "jam" "loglevel1"
  atf_check -s exit:0 [ -e output/include/butter.log ]
  atf_check -s exit:1 [ -s output/include/butter.log ]
  check_jam_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output



  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


atf_test_case property_top_flags_jam_gen
property_top_flags_jam_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter flags) with jam generator."
}
property_top_flags_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam-top" "top-flags"
  run_plugouts "property_test" "jam" "top-flags"
  check_jam_build 1 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG "output_top_flags"
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG "output_top_flags"
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE "output_top_flags"

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_test_case property_basedir_jam_gen
property_basedir_jam_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter base) with jam generator."
}
property_basedir_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter base + ../..
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam-top" "basedir"
  run_plugouts "property_test" "jam" "basedir"
  atf_check -s exit:0 [ -e Jamrules ]
  atf_check -s exit:0 [ -e Jamfile ]
 
  build_test(){
    local variant=$1
    local builddir=$2
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/output/src/Executable/program ]
    atf_check -s exit:0 [ -e ${builddir}/output/src/Library/library.a ]
    atf_check -s exit:0 -o file:canon/output_default ${builddir}/output/src/Executable/program
    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:canon/output_default installdir/bin/program
    # no distclean target
    # test clean target
    atf_check -s exit:0 -o save:jam3.log -e save:jam3.err jam clean ${variant}
    atf_check -s exit:0 [ ! -e ${builddir}/output/src/Executable/program ]
    atf_check -s exit:0 [ ! -e ${builddir}/output/src/Library/library.a ]
    atf_check -s exit:0 [ -e installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]

    atf_check -s exit:0 -o empty rm jam1.log jam1.err
    atf_check -s exit:0 -o empty rm jam2.log jam2.err
    atf_check -s exit:0 -o empty rm jam3.log jam3.err
    atf_check -s exit:0 -o empty rm -rf ${builddir}
    atf_check -s exit:0 -o empty rm -rf installdir
  }

  # default (DEBUG) VARIANT
  build_test "" DEBUG
  # specific DEBUG VARIANT
  build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty rm Jamfile Jamrules
   atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
}


atf_test_case property_builddir_jam_gen
property_builddir_jam_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter build-dir) with jam generator."
}
property_builddir_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter build-dir + BUILD
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam-top" "builddir"
  run_plugouts "property_test" "jam" "builddir"
  check_jam_build 1 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" BUILD/DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG BUILD/DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE BUILD/RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}


##atf_test_case property_builddir_boost_gen
##property_builddir_boost_gen_head() {
##  atf_set "descr" "Property (*top*)Package(butter build-dir) with boost generator."
##}
##property_builddir_boost_gen_body() {
##  pushd ../test/property_test
##  #----------------------- 
##  # Boost (bjam) variant 
##  #----------------------- 
##  setup_example "property_test" "boost"
##
##  build_test(){
##    local variant=$1
##    local builddir=$2
##    local installdir=$3
##    pushd output
##    # test base build target
##    atf_check -s exit:0 -o save:build1.log -e save:build1.err bjam ${variant}
##    atf_check -s exit:0 [ -x build/src/Executable/${builddir}/program ]
##    atf_check -s exit:0 [ -e build/src/Library/${builddir}/link-static/library.a ]
##    atf_check -s exit:0 -o file:../canon/output_default build/src/Executable/${builddir}/program
##    # no separate install target
##    atf_check -s exit:0 [ -x ${installdir}/program ]
##    atf_check -s exit:0 -o file:../canon/output_default ${installdir}/program
##    # no distclean target
##    # test clean target
##    atf_check -s exit:0 -o save:build2.log -e save:build2.err bjam clean ${variant}
##    atf_check -s exit:0 [ ! -e build/src/Executable/${builddir}/program ]
##    atf_check -s exit:0 [ ! -e build/src/Library/${builddir}/link-static/library.a ]
##    atf_check -s exit:0 [ ! -e ${installdir}/program ]
##
##    atf_check -s exit:0 -o empty rm build1.log build1.err
##    atf_check -s exit:0 -o empty rm build2.log build2.err
##    atf_check -s exit:0 -o empty rm -rf build
##    atf_check -s exit:0 -o empty rm -rf ${installdir}
##    popd
##  }
##
##  # default (DEBUG) VARIANT
##  build_test "" "*/debug" src/Executable/install_program
##  # specific DEBUG VARIANT
##  build_test debug "*/debug" src/Executable/install_program
##  # RELEASE VARIANT
##  build_test release "*/release" installdir/bin
##
##  atf_check -s exit:0 -o empty rmdir output/installdir
##
##  atf_check -s exit:0 -o empty rm -rf output
##  atf_check -s exit:0 -o empty git checkout HEAD -- .
##  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
##  popd
##  unset build_test
##}
##
##atf_test_case property_builddir_cmake_gen
##property_builddir_cmake_gen_head() {
##  atf_set "descr" "Property (*top*)Package(butter build-dir) with cmake generator."
##}
##property_builddir_cmake_gen_body() {
##  pushd ../test/property_test
##  #----------------------- 
##  # CMake variant 
##  #----------------------- 
##  setup_example "property_test" "cmake"
##
##  build_test(){
##    local variant=$1
##    local installdir=$2
##    pushd output
##    # test base build target
##    atf_check -s exit:0 -o save:build1.log -e save:build1.err cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=${variant} --build .
##    atf_check -s exit:0 -o save:build2.log -e save:build2.err make -f Makefile VERBOSE=1
##    atf_check -s exit:0 [ -x src/Executable/program ]
##    atf_check -s exit:0 [ -e src/Executable/CMakeFiles/program.dir/example_exe.cpp.o ]
##    atf_check -s exit:0 [ -e src/Library/library.a ]
##    atf_check -s exit:0 [ -e src/Library/CMakeFiles/library.dir/example_lib.cpp.o ]
##    atf_check -s exit:0 -o file:../canon/output_default src/Executable/program
##    # test install target
##    atf_check -s exit:0 -o save:build2.log -e save:build2.err make -f Makefile install VERBOSE=1
##    if [ "X" != "X${installdir}" ]; then
##      atf_check -s exit:0 [ -e install_manifest.txt ]
##      atf_check -s exit:0 [ -s install_manifest.txt ]
##      atf_check -s exit:0 [ -e ${installdir}/bin/program ]
##      atf_check -s exit:0 -o file:../canon/output_default ${installdir}/bin/program
##    else
##      atf_check -s exit:0 [ -e install_manifest.txt ]
##      atf_check -s exit:0 [ ! -s install_manifest.txt ]
##    fi
##    # no distclean target
##    # test clean target
##    atf_check -s exit:0 -o save:build3.log -e save:build3.err make -f Makefile clean VERBOSE=1
##    atf_check -s exit:0 [ ! -e src/Executable/program ]
##    atf_check -s exit:0 [ ! -e src/Executable/CMakeFiles/program.dir/example_exe.cpp.o ]
##    atf_check -s exit:0 [ ! -e src/Library/library.a ]
##    atf_check -s exit:0 [ ! -e src/Library/CMakeFiles/library.dir/example_lib.cpp.o ]
##    if [ "X" != "X${installdir}" ]; then
##      atf_check -s exit:0 [ -e ${installdir}/bin/program ]
##    fi
##
##    # cleanup
##    atf_check -s exit:0 -o empty rm build1.log build1.err
##    atf_check -s exit:0 -o empty rm build2.log build2.err
##    atf_check -s exit:0 -o empty rm build3.log build3.err
##    atf_check -s exit:0 -o empty rm CMakeCache.txt Makefile cmake_install.cmake install_manifest.txt
##
##    atf_check -s exit:0 -o empty rm -rf CMakeFiles
##    atf_check -s exit:0 -o empty rm -rf ${installdir}
##    popd
##  }
##
##  # default (DEBUG) VARIANT
##  build_test "" ""
##  # specific DEBUG VARIANT
##  build_test Debug ""
##  # RELEASE VARIANT
##  build_test Release "installdir"
##
##  atf_check -s exit:0 -o empty rm -rf output
##  atf_check -s exit:0 -o empty git checkout HEAD -- .
##  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
##  popd
##  unset build_test
##}
##
##atf_test_case property_builddir_make_gen
##property_builddir_make_gen_head() {
##  atf_set "descr" "Property (*top*)Package(butter build-dir) with gmake generator."
##}
##property_builddir_make_gen_body() {
##  pushd ../test/property_test
##  #----------------------- 
##  # Make variant 
##  #----------------------- 
##  setup_example "property_test" "make"
##
##  build_test(){
##    local variant=$1
##    pushd output
##    # test base build target
##    atf_check -s exit:0 -o save:build1.1.log -e save:build1.1.err make -k VARIANT=${variant}
##    atf_check -s exit:0 -o save:build1.2.log -e save:build1.2.err make -k VARIANT=${variant}
##    atf_check -s exit:0 [ -x src/Executable/program ]
##    atf_check -s exit:0 [ -e src/Library/library.a ]
##    atf_check -s exit:0 [ -e src/Executable/example_exe.o ]
##    atf_check -s exit:0 [ -e src/Library/example_lib.o ]
##    atf_check -s exit:0 [ ! -e installdir/bin/program ]
##    atf_check -s exit:0 -o file:../canon/output_default src/Executable/program
##    # test clean target
##    atf_check -s exit:0 -o save:build2.log -e save:build2.err make clean
##    atf_check -s exit:0 [ -e src/Executable/program ]
##    atf_check -s exit:0 [ ! -e src/Library/library.a ]
##    atf_check -s exit:0 [ ! -e src/Executable/example_exe.o ]
##    atf_check -s exit:0 [ ! -e src/Library/example_lib.o ]
##    atf_check -s exit:0 [ ! -e installdir/bin/program ]
##
##    # test install target
##    atf_check -s exit:0 -o save:build3.1.log -e save:build3.1.err make install
##    atf_check -s exit:0 -o save:build3.2.log -e save:build3.2.err make install
##    atf_check -s exit:0 [ -x src/Executable/program ]
##    atf_check -s exit:0 [ -e src/Library/library.a ]
##    atf_check -s exit:0 [ -e src/Executable/example_exe.o ]
##    atf_check -s exit:0 [ -e src/Library/example_lib.o ]
##    atf_check -s exit:0 [ -e installdir/bin/program ]
##    atf_check -s exit:0 -o file:../canon/output_default installdir/bin/program
##    # test distclean target
##    atf_check -s exit:0 -o save:build4.log -e save:build4.err make distclean
##    atf_check -s exit:0 [ ! -e src/Executable/program ]
##    atf_check -s exit:0 [ ! -e src/Library/library.a ]
##    atf_check -s exit:0 [ ! -e src/Executable/example_exe.o ]
##    atf_check -s exit:0 [ ! -e src/Library/example_lib.o ]
##    atf_check -s exit:0 [ -e installdir/bin/program ]
##
##    # cleanup
##    atf_check -s exit:0 -o empty rm build1.1.log build1.1.err
##    atf_check -s exit:0 -o empty rm build1.2.log build1.2.err
##    atf_check -s exit:0 -o empty rm build2.log build2.err
##    atf_check -s exit:0 -o empty rm build3.1.log build3.1.err
##    atf_check -s exit:0 -o empty rm build3.2.log build3.2.err
##    atf_check -s exit:0 -o empty rm build4.log build4.err
##    atf_check -s exit:0 -o empty rm -rf installdir
##    popd
##  }
##
##  # default (DEBUG) VARIANT
##  build_test ""
##  # specific DEBUG VARIANT
##  build_test DEBUG
##  # RELEASE VARIANT
##  build_test RELEASE
##
##  # Clean up
##  atf_check -s exit:0 -o empty rm -rf output
##  atf_check -s exit:0 -o empty git checkout HEAD -- .
##  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
##  popd
##  unset build_test
##}
##

atf_test_case property_test_jam_gen
property_test_jam_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
property_test_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Standard (jam) variant without defined properties (nul test)
  #----------------------- 
  setup_example "property_test" "jam"
  run_plugouts "property_test" "jam" "nultest"
  check_jam_build 0 0 0 0

  # default (DEBUG) VARIANT
  standard_build_test "" DEBUG
  # specific DEBUG VARIANT
  standard_build_test -sVARIANT=DEBUG DEBUG
  # RELEASE VARIANT
  standard_build_test -sVARIANT=RELEASE RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
}

atf_init_test_cases() {
    atf_add_test_case property_test_jam_gen
    atf_add_test_case property_builddir_jam_gen
    atf_add_test_case property_basedir_jam_gen
    atf_add_test_case property_version_jam_gen
    atf_add_test_case property_log_level_jam_gen
    atf_add_test_case property_top_flags_jam_gen
    atf_add_test_case property_top_ldflags_jam_gen
    atf_add_test_case property_top_include_jam_gen
    atf_add_test_case property_top_description_jam_gen
    atf_add_test_case property_top_libtype_static_jam_gen
    atf_add_test_case property_top_libtype_shared_jam_gen
    atf_add_test_case property_lib_flags_jam_gen
    atf_add_test_case property_lib_ldflags_jam_gen
    atf_add_test_case property_lib_include_jam_gen
    atf_add_test_case property_lib_libtype_static_jam_gen
    atf_add_test_case property_lib_libtype_shared_jam_gen
    atf_add_test_case property_lib_description_jam_gen
    atf_add_test_case property_exe_flags_jam_gen
    atf_add_test_case property_exe_ldflags_jam_gen
    atf_add_test_case property_exe_include_jam_gen
    atf_add_test_case property_exe_libtype_static_jam_gen
    atf_add_test_case property_exe_libtype_shared_jam_gen
    atf_add_test_case property_exe_description_jam_gen
    atf_add_test_case property_libsrc_flags_jam_gen
    atf_add_test_case property_libsrc_ldflags_jam_gen
    atf_add_test_case property_libsrc_include_jam_gen
    atf_add_test_case property_libsrc_libtype_static_jam_gen
    atf_add_test_case property_libsrc_libtype_shared_jam_gen
    atf_add_test_case property_libsrc_description_jam_gen

}

