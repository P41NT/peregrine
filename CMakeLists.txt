# CMakeList.txt : CMake project for peregrine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if(MSVC)
  set(variables 
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
	CMAKE_CXX_FLAGS_RELWITHDEBINFO
	CMAKE_CXX_FLAGS_MINSIZEREL
  )
  foreach(variable ${variables})
	if(${variable} MATCHES "/MD")
	  string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
	endif()
  endforeach()
endif()

project ("peregrine")

include(FetchContent)
FetchContent_Declare(
	GoogleTest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(GoogleTest)

# Add source to this project's executable.
add_executable (peregrine "src/peregrine.cpp" "include/peregrine.hpp" "include/clauses.hpp"  "include/assignment.hpp" "include/types.hpp" "include/parser.hpp" "src/clauses/assignment.cpp" "src/parsers/parser.cpp" "src/propagators/propagator.cpp")

enable_testing()
add_subdirectory(tests)

