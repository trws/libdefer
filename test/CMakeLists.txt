add_executable(basic basic.c)
target_link_libraries(basic defer)

add_test(deferBasic basic)
