PROJECT(chip_log C)
CMAKE_MINIMUM_REQUIRED( VERSION 2.6.0 )
OPTION(DEVEL "enable debug, full compiler warnings, unit tests" OFF)
OPTION(CODE_COVERAGE "produces test coverage report (requires gcc, gcov, lcov)" OFF)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
INCLUDE_DIRECTORIES(.)

IF (CODE_COVERAGE OR DEVEL)
    INCLUDE(DevelopmentStuff)
ENDIF()

ADD_LIBRARY( chip_log
    clog.c
    clog.h
    clog_str.h
    clog_str.c
    clog_bas.h
    clog_lex_fwd.h
    clog_lex.h
    clog_lex.c
    clog_prs_fwd.h
    clog_prs.h
    clog_prs.c
    clog_cfg.h
)

