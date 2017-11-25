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
  atf_check -s exit:0 -o save:canon/cpp.log bouml ${example}.prj -execnogui ${BOUML_LOC}/cpp_generator -exit
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

  # Flags for a library should apply to the using class?

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
    atf_check -s exit:0 -o save:symbol objdump -t program
    atf_check -s exit:1 -o ignore diff ../../../../canon/symbol symbol
    popd

    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default installdir/bin/program
    pushd installdir/bin
    atf_check -s exit:0 -o save:symbol objdump -t program
    atf_check -s exit:1 -o ignore diff ../../../canon/symbol symbol
    popd

    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default installdir/bin/program
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
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
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

  # Flags for a library should apply to the using class?

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
    atf_check -s exit:0 -o save:symbol objdump -t program
    atf_check -s exit:1 -o ignore diff ../../../../canon/symbol symbol
    popd

    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default installdir/bin/program
    pushd installdir/bin
    atf_check -s exit:0 -o save:symbol objdump -t program
    atf_check -s exit:1 -o ignore diff ../../../canon/symbol symbol
    popd

    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default installdir/bin/program
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
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
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

  # Flags for a library should apply to the using class?

  build_test(){
    local variant=$1
    local builddir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ -e ${builddir}/src/Library/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default ${builddir}/src/Executable/program
    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default installdir/bin/program
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
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
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

  build_test(){
    local variant=$1
    local builddir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ -e ${builddir}/src/Library/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default ${builddir}/src/Executable/program
    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default installdir/bin/program
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
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
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

  build_test(){
    local variant=$1
    local builddir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ -e ${builddir}/src/Library/library.a ]
    atf_check -s exit:0 -o save:../canon/output_default ${builddir}/src/Executable/program
    pushd ${builddir}/src/Executable
      atf_check -s exit:0 -o save:symbol.txt objdump -t program
      atf_check -s exit:1 -o ignore diff symbol.txt ../../../../canon/symbol
    popd
    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default installdir/bin/program
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
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
}


atf_test_case property_top_ldflags_jam_gen
property_top_ldflags_jam_gen_head() {
  atf_set "descr" "Property (*top*)Package(butter ldflags) with jam generator."
}
property_top_ldflags_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Property: PROJECT.butter flags + -DPROGRAM_MESSAGE='\"proj1 message\"' -DLIBRARY_MESSAGE='\"proj2 message\"' 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam-top" "top-ldflags"
  run_plugouts "property_test" "jam" "top-ldflags"

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

  setup_example "property_test" "jam-top" "top-ldflags-guard"
  run_plugouts "property_test" "jam" "top-ldflags-guard"

  build_test(){
    local variant=$1
    local builddir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ -e ${builddir}/src/Library/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default ${builddir}/src/Executable/program
    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default installdir/bin/program
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


  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
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

  build_test(){
    local variant=$1
    local builddir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ -e ${builddir}/src/Library/library.a ]
    atf_check -s exit:0 -o file:../canon/output_top_include ${builddir}/src/Executable/program
    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_top_include installdir/bin/program
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
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
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
 
  build_test(){
    local variant=$1
    local builddir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ -e ${builddir}/src/Library/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default ${builddir}/src/Executable/program
    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default installdir/bin/program
    # no distclean target
    # test clean target
    atf_check -s exit:0 -o save:jam3.log -e save:jam3.err jam clean ${variant}
    atf_check -s exit:0 [ ! -e ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ ! -e ${builddir}/src/Library/library.a ]
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
 
  build_test(){
    local variant=$1
    local builddir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ -e ${builddir}/src/Library/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default ${builddir}/src/Executable/program
    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default installdir/bin/program
    # no distclean target
    # test clean target
    atf_check -s exit:0 -o save:jam3.log -e save:jam3.err jam clean ${variant}
    atf_check -s exit:0 [ ! -e ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ ! -e ${builddir}/src/Library/library.a ]
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

  build_test(){
    local variant=$1
    local builddir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ -e ${builddir}/src/Library/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default ${builddir}/src/Executable/program
    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default installdir/bin/program
    # no distclean target
    # test clean target
    atf_check -s exit:0 -o save:jam3.log -e save:jam3.err jam clean ${variant}
    atf_check -s exit:0 [ ! -e ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ ! -e ${builddir}/src/Library/library.a ]
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

  # default (DEBUG) VARIANT
  build_test "" DEBUG

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

  # default (DEBUG) VARIANT
  build_test "" DEBUG

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output



  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
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

  build_test(){
    local variant=$1
    local builddir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ -e ${builddir}/src/Library/library.a ]
    atf_check -s exit:0 -o file:../canon/output_top_flags ${builddir}/src/Executable/program
    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_top_flags installdir/bin/program
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
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
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

  build_test(){
    local variant=$1
    local builddir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ -e ${builddir}/src/Library/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default ${builddir}/src/Executable/program
    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 [ -e installdir/lib/library.a ]
    atf_check -s exit:0 -o file:../canon/output_default installdir/bin/program
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
    atf_check -s exit:0 -o empty rm -rf BUILD
    atf_check -s exit:0 -o empty rm -rf installdir
    popd
  }

  # default (DEBUG) VARIANT
  build_test "" BUILD/DEBUG
  # specific DEBUG VARIANT
  build_test -sVARIANT=DEBUG BUILD/DEBUG
  # RELEASE VARIANT
  build_test -sVARIANT=RELEASE BUILD/RELEASE

  # Clean up
  atf_check -s exit:0 -o empty rm -rf output
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
  unset build_test
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
##
atf_test_case property_test_jam_gen
property_test_jam_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
property_test_jam_gen_body() {
  pushd ../test/property_test
  #----------------------- 
  # Standard (jam) variant 
  #----------------------- 
  setup_example "property_test" "jam"

  build_test(){
    local variant=$1
    local builddir=$2
    pushd output
    # test base build target
    atf_check -s exit:0 -o save:jam1.log -e save:jam1.err jam ${variant}
    atf_check -s exit:0 [ -x ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ -e ${builddir}/src/Library/library.a ]
    atf_check -s exit:0 -o save:../canon/output ${builddir}/src/Executable/program
    # test install target
    atf_check -s exit:0 -o save:jam2.log -e save:jam2.err jam install ${variant}
    atf_check -s exit:0 [ -x installdir/bin/program ]
    atf_check -s exit:0 -o file:../canon/output installdir/bin/program
    # no distclean target
    # test clean target
    atf_check -s exit:0 -o save:jam3.log -e save:jam3.err jam clean ${variant}
    atf_check -s exit:0 [ ! -e ${builddir}/src/Executable/program ]
    atf_check -s exit:0 [ ! -e ${builddir}/src/Library/library.a ]
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

atf_init_test_cases() {
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
#    atf_add_test_case property_lib_description_jam_gen
#    atf_add_test_case property_lib_libtype_static_jam_gen
#    atf_add_test_case property_lib_libtype_shared_jam_gen

}

