cmake_minimum_required(VERSION 3.15)

project(NFDRS4Py LANGUAGES CXX)

include(ExternalProject)

if (NOT SKBUILD)
  message(WARNING "\
  This CMake file is meant to be executed using 'scikit-build'. Running
  it directly will almost certainly not produce the desired result. If
  you are a user trying to install this package, please use the command
  below, which will install all necessary build dependencies, compile
  the package in an isolated environment, and then install it.
  =====================================================================
   $ pip install .
  =====================================================================
  If you are a software developer, and this is your own package, then
  it is usually much more efficient to install the build dependencies
  in your environment once and use the following command that avoids
  a costly creation of a new virtual environment at every compilation:
  =====================================================================
   $ pip install nanobind scikit-build-core[pyproject]
   $ pip install --no-build-isolation -ve .
  =====================================================================
  You may optionally add -Ceditable.rebuild=true to auto-rebuild when
  the package is imported. Otherwise, you need to re-run the above
  after editing C++ files.")
endif()

# Try to import all Python components potentially needed by nanobind
find_package(Python 3.12
  REQUIRED COMPONENTS Interpreter Development.Module
  OPTIONAL_COMPONENTS Development.SABIModule)

## NFDRS4 needs to build config4cpp 
set(CONFIG4CPP_SRC ${CMAKE_SOURCE_DIR}/external/NFDRS4/extern/config4cpp)
set(CONFIG4CPP_DIR ${CMAKE_SOURCE_DIR}/external/NFDRS4/extern/config4cpp/include)
set(CONFIG4CPP_LIB ${CMAKE_SOURCE_DIR}/external/NFDRS4/extern/config4cpp/lib/libconfig4cpp.a)
message(STATUS "CONFIG4CPP_DIR = ${CONFIG4CPP_DIR}")
message(STATUS "CONFIG4CPP_LIB= ${CONFIG4CPP_LIB}")

ExternalProject_Add(
    _config4cpp
    SOURCE_DIR ${CONFIG4CPP_SRC}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CONFIG4CPP_SRC}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make
    INSTALL_COMMAND "" 
    BUILD_IN_SOURCE 1
)

# Import nanobind through CMake's find_package mechanism
add_subdirectory(external/nanobind)

## import NFDRS4 
add_subdirectory(external/NFDRS4)

add_dependencies(NFDRS4 _config4cpp)

# We are now ready to compile the actual extension module
nanobind_add_module(
  # Name of the extension
  nfdrs4_wrap

  # Target the stable ABI for Python 3.12+, which reduces
  # the number of binary wheels that must be built. This
  # does nothing on older Python versions
  STABLE_ABI

  # Build libnanobind statically and merge it into the
  # extension (which itself remains a shared library)
  #
  # If your project builds multiple extensions, you can
  # replace this flag by NB_SHARED to conserve space by
  # reusing a shared libnanobind across libraries
  NB_STATIC

  # Source code goes here
  src/nanobind/nfdrs4_wrap.cpp
)

target_link_libraries(nfdrs4_wrap PRIVATE NFDRS4)

# Install directive for scikit-build-core
install(TARGETS nfdrs4_wrap LIBRARY DESTINATION NFDRS4Py)
