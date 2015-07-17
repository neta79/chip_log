# Development options, test suite execution and code coverage
# it's all left out of the main CMakeLists.txt, to leave it in its simplest form.

ADD_DEFINITIONS( -DCLOG_DEVELOPER=1 )
SET(DEVEL_CFLAGS
    "-Wall -pedantic -std=c89 -Werror -Wextra \
    -Wfloat-equal -Wundef -Wshadow -Wpointer-arith \
    -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 \
    -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default \
    -Wconversion -Wunreachable-code -Winit-self" )
SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEVEL_CFLAGS}" )
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEVEL_CFLAGS} -Weffc++" )

ENABLE_TESTING()
INCLUDE(CTest)

# build test fat library (contains all test suites)
FILE(GLOB TEST_SOURCES tests/test_*.c)
ADD_LIBRARY( test_lib STATIC ${TEST_SOURCES} )

# use an external script and scan the bulk of the test suite, extract test function declarations and mappings
ADD_CUSTOM_COMMAND(OUTPUT tests/tests_decl.h
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/scripts/mk_test_list.py
    -o ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests_decl.h
    ${TEST_SOURCES} --externals)
ADD_CUSTOM_COMMAND(OUTPUT tests/tests_mapping.inc
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/scripts/mk_test_list.py
    -o ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests_mapping.inc
    ${TEST_SOURCES} --mapping)

# build fat runner executable, embedding the whole test suite
SET_PROPERTY(SOURCE tests/run_tests.c APPEND PROPERTY OBJECT_DEPENDS tests/tests_decl.h tests/tests_mapping.inc)
ADD_EXECUTABLE( run_tests tests/run_tests.c )
TARGET_LINK_LIBRARIES( run_tests test_lib chip_log )

# add individual test calls to CTest, for better partitioning and reporting
EXECUTE_PROCESS(
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/scripts/mk_test_list.py
    ${TEST_SOURCES}
    OUTPUT_VARIABLE TEST_NAMES)
FOREACH(TEST_NAME ${TEST_NAMES})
    ADD_TEST(${TEST_NAME} run_tests ${TEST_NAME})
ENDFOREACH()

IF (CODE_COVERAGE)
    INCLUDE(CodeCoverage)
    SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage -g -O0")
    SET(GCC_COVERAGE_LINK_FLAGS    "-fprofile-arcs -ftest-coverage")
    SET( CMAKE_C_FLAGS           "${CMAKE_C_FLAGS}          ${GCC_COVERAGE_COMPILE_FLAGS}" )
    SET( CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}        ${GCC_COVERAGE_COMPILE_FLAGS}" )
    SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )
    # 'run_tests' w/o arguments just runs all tests, which is how we check code coverage
    SETUP_TARGET_FOR_COVERAGE(test_code_coverage run_tests coverage)
ENDIF()
