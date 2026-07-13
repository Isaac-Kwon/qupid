cmake_minimum_required(VERSION 3.16)
project(qupid)

#find_package(Sanitizers)

# Build the ROOT visualization module (libqupidVis + ROOT-dependent tests).
# When OFF, or when ROOT is not found, only the ROOT-free core is built.
option(QUPID_WITH_ROOT "Build the ROOT visualization module" ON)

# by default build optimized code with debug symbols
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  # set(CMAKE_BUILD_TYPE DEBUG)
  # set(CMAKE_BUILD_TYPE RELWITHDEBINFO)
  set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# require C++11-capable compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9")
    message(WARNING "Compiler with incomplete C++11 support detected: functionality limited")
    set(OLD_CXX true)
  endif()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  message("clang, undefined dynamic_lookup")
  set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
endif()

# ROOT is only looked for when the visualization module is requested.
if(QUPID_WITH_ROOT)
  find_package(ROOT QUIET)
endif()

# The visualization module is built only when it was requested AND ROOT exists.
set(QUPID_ROOT_ENABLED FALSE)
if(QUPID_WITH_ROOT AND ROOT_FOUND)
  set(QUPID_ROOT_ENABLED TRUE)
endif()

# Standalone core defaults to C++17; when ROOT is used, follow ROOT's standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(QUPID_ROOT_ENABLED)
  if(ROOT_cxx17_FOUND)
    set(CMAKE_CXX_STANDARD 17)
  elseif(ROOT_cxx14_FOUND)
    set(CMAKE_CXX_STANDARD 14)
  elseif(ROOT_cxx11_FOUND)
    set(CMAKE_CXX_STANDARD 11)
  endif()
endif()
message(STATUS "Using C++${CMAKE_CXX_STANDARD}")

# Report the visualization decision plainly.
if(NOT QUPID_WITH_ROOT)
  message(STATUS "qupid: ROOT visualization disabled (QUPID_WITH_ROOT=OFF); building core only")
elseif(ROOT_FOUND)
  message(STATUS "qupid: ROOT ${ROOT_VERSION} found; building qupidVis visualization module")
else()
  message(STATUS "qupid: ROOT not found; skipping visualization module, building core only")
endif()

add_compile_options("-Wall")
add_compile_options("-Wpedantic")
add_compile_options("-Wno-unused-variable")
add_compile_options("-Wunused-variable")
add_compile_options("-Wno-uninitialized")

# check for build in the source tree
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(STATUS "Building in source tree")
  set(BUILDINSOURCE true)
endif()

message(STATUS "In Project Directory: ${PROJECT_SOURCE_DIR}")
message(STATUS "ROOT Include Directory: ${ROOT_INCLUDE_DIRS}")

# ---------------------------------------------------------------------------
# Core library: ROOT-free, std C++ only. Sources listed explicitly (no GLOB).
# ---------------------------------------------------------------------------
set(qupid_sources
  ${PROJECT_SOURCE_DIR}/src/QPVector.cpp
  ${PROJECT_SOURCE_DIR}/src/QPMatrix.cpp
  ${PROJECT_SOURCE_DIR}/src/QPMath.cpp
  ${PROJECT_SOURCE_DIR}/src/QPSignalBase.cpp
  ${PROJECT_SOURCE_DIR}/src/QPSignalGaussian.cpp
  ${PROJECT_SOURCE_DIR}/src/QPDigitizerBase.cpp
  ${PROJECT_SOURCE_DIR}/src/QPDigitizerSquare.cpp
  ${PROJECT_SOURCE_DIR}/src/QPPixelBase.cpp
  ${PROJECT_SOURCE_DIR}/src/QPPixel.cpp
  ${PROJECT_SOURCE_DIR}/src/QPRandom.cpp
  ${PROJECT_SOURCE_DIR}/src/QPMonteCarlo.cpp
)

add_library(qupid SHARED ${qupid_sources})
target_include_directories(qupid PUBLIC ${PROJECT_SOURCE_DIR}/include)
# No ROOT link: the core is a plain C++ library.

# ---------------------------------------------------------------------------
# Visualization library: QPGraphFactory. Built only when ROOT is available.
# ---------------------------------------------------------------------------
if(QUPID_ROOT_ENABLED)
  add_library(qupidVis SHARED ${PROJECT_SOURCE_DIR}/vis/src/QPGraphFactory.cpp)
  target_include_directories(qupidVis PUBLIC ${PROJECT_SOURCE_DIR}/vis/include)
  target_link_libraries(qupidVis PUBLIC qupid ROOT::Core ROOT::Hist ROOT::Graf)
endif()

# ---------------------------------------------------------------------------
# Copy runtime helpers into the build dir so ROOT interactive sessions work.
# ---------------------------------------------------------------------------
file(COPY ${PROJECT_SOURCE_DIR}/include DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/rootlogon.C DESTINATION ${CMAKE_BINARY_DIR})
if(QUPID_ROOT_ENABLED)
  # QPStyle.cpp is a ROOT macro loaded by rootlogon.C via LoadMacro("./QPStyle.cpp").
  file(COPY ${PROJECT_SOURCE_DIR}/macro/QPStyle.cpp DESTINATION ${CMAKE_BINARY_DIR})
  # Make the vis headers reachable from the single build/include path used by rootlogon.C.
  file(GLOB vis_headers ${PROJECT_SOURCE_DIR}/vis/include/*.hpp)
  file(COPY ${vis_headers} DESTINATION ${CMAKE_BINARY_DIR}/include)
endif()

add_subdirectory(exe)
add_subdirectory(test)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Data)
