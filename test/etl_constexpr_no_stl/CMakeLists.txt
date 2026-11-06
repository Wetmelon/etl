cmake_minimum_required(VERSION 3.5.0)
project(etl_constexpr_no_stl_unit_tests)

add_definitions(-DETL_DEBUG)
add_definitions(-DETL_NO_STL)

include_directories(${PROJECT_SOURCE_DIR}/../../include)

# std::construct_at needs C++20, so older standards from run-tests.sh still build this as C++20.
if (ETL_CXX_STANDARD MATCHES "23")
	set(TEST_CXX_STANDARD 23)
elseif (ETL_CXX_STANDARD MATCHES "26" AND CMAKE_VERSION VERSION_GREATER_EQUAL "3.25")
	set(TEST_CXX_STANDARD 26)
else()
	set(TEST_CXX_STANDARD 20)
endif()
message(STATUS "Compiling for C++${TEST_CXX_STANDARD}")

# No C++ standard header can be included, which is what this test is for.
add_executable(etl_tests test_no_standard_headers.cpp)
target_compile_options(etl_tests PRIVATE -nostdinc++)

# ETL_NO_STL with the standard headers there. Compile only: it must not redefine them.
add_library(etl_standard_headers_present OBJECT test_standard_headers_present.cpp)
target_compile_definitions(etl_standard_headers_present PRIVATE ETL_FORCE_STD_INITIALIZER_LIST)

# The same toolchain, opting in. Compile only.
add_library(etl_override_with_standard_headers OBJECT test_override_with_standard_headers.cpp)
target_compile_definitions(etl_override_with_standard_headers PRIVATE ETL_FORCE_STD_INITIALIZER_LIST ETL_USING_STD_CONSTRUCT_AT=1)

foreach (target etl_tests etl_standard_headers_present etl_override_with_standard_headers)
	set_property(TARGET ${target} PROPERTY CXX_STANDARD ${TEST_CXX_STANDARD})
	target_compile_options(${target}
			PRIVATE
			-Wall
			-Wextra
			-Werror
			-Wfloat-equal
			-Wshadow
			-Wnull-dereference
			)
endforeach()

# Enable the 'make test' CMake target using the executable defined above
add_test(etl_constexpr_no_stl_unit_tests etl_tests)

# Since ctest will only show you the results of the single executable
# define a target that will output all of the failing or passing tests
# as they appear from UnitTest++
add_custom_target(test_verbose COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
