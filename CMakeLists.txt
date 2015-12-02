cmake_minimum_required (VERSION 2.6)
set(target "SimPHITSROOTrakcer.exe")

# include ROOT
set(libs)
set(CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT REQUIRED)
list(APPEND libs ${ROOT_LIBRARIES})
list(APPEND libs $ENV{ROOTSYS}/lib/root/libEG.so)
include_directories(${ROOT_INCLUDE_DIR})
#message ("${libs}")

set(CMAKE_CXX_FLAGS "-O3")
set(main SimPHITSRooTracker.cc PHITStoRooTracker.cc)
add_executable(${target} ${main})
target_link_libraries(${target} ${libs})

