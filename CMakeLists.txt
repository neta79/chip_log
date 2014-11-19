add_library(chip_log src/clog.c include/clog.h)
add_executable(test_init tests/test_init.c)
target_link_libraries(chip_log)

