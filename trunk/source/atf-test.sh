#!/home/finnerty/local/bin/atf-sh 

atf_test_case single_jam_gen
single_jam_gen_head() {
  atf_set "descr" "Test Standard Jam generator on single directory project."
}
single_jam_gen_body() {
  pushd ../test/single_test
  #----------------------- 
  # Standard (jam) variant 
  #----------------------- 
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file single_test.prj\n" patch <patch/jam.patch
  atf_check -s exit:0 -o empty bouml single_test.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamfile output/Jamfile.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamrules output/Jamrules.canon
  atf_check -s exit:0 -o empty rm output/Jamfile output/Jamrules
  atf_check -s exit:0 -o empty rm *.session
  atf_check -s exit:0 -o empty rm -f output/butter.log
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
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
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file single_test.prj\n" patch <patch/boost.patch
  atf_check -s exit:0 -o empty bouml single_test.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamroot output/Jamroot.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/local.jam output/local.jam.canon
  atf_check -s exit:0 -o empty rm output/local.jam output/Jamroot
  atf_check -s exit:0 -o empty rm *.session
  atf_check -s exit:0 -o empty rm -f output/butter.log
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
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
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file single_test.prj\n" patch <patch/cmake.patch
  atf_check -s exit:0 -o empty bouml single_test.prj -exec ../../source/src/butter/butter_exe -exit
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/CMakeLists.txt output/CMakeLists.txt.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/local.cmake output/local.cmake.canon
  atf_check -s exit:0 -o empty rm output/CMakeLists.txt output/local.cmake
  atf_check -s exit:0 -o empty rm *.session
  atf_check -s exit:0 -o empty rm -f output/butter.log
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  popd
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
  atf_check -s exit:0 -o empty git checkout HEAD -- .
  atf_check -s exit:0 -o inline:"# On branch master\nnothing to commit, working directory clean\n" git status .
  atf_check -s exit:0 -o inline:"patching file single_test.prj\n" patch <patch/make.patch
  atf_check -s exit:0 -o empty bouml single_test.prj -exec ../../source/src/butter/butter_exe -exit
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
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/Jamrules output/Jamrules.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/src/Jamfile output/src/Jamfile.canon
  atf_check -o empty diff output/src/fortran.f output/src/fortran.f.canon
  atf_check -o empty diff --ignore-matching-lines="#[MTWFS][aouehr][neduit] [JFMASOND][aepuco][nbrylgptvc] [0-9][0-9]* [0-9][0-9]:[0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9] *" output/doc/Jamfile output/doc/Jamfile.canon
  atf_check -o empty diff output/doc/program.t2t output/doc/program.t2t.canon
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
#    atf_add_test_case proplang_boost_gen
#    atf_add_test_case proplang_cmake_gen
#    atf_add_test_case proplang_make_gen
}

