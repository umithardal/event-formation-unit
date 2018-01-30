

function(add_linker_flags target flags)
    get_target_property(cur_link_flags ${target} LINK_FLAGS)
    if(NOT cur_link_flags)
        set(cur_link_flags "")
    endif()
    set(new_link_flags "${cur_link_flags} ${flags}")
    set_target_properties(${target} PROPERTIES LINK_FLAGS "${new_link_flags}")
endfunction(add_linker_flags)

function(add_compile_flags target flags)
    get_target_property(cur_compile_flags ${target} COMPILE_FLAGS)
    if(NOT cur_compile_flags)
        set(cur_compile_flags "")
    endif()
    set(new_compile_flags "${cur_compile_flags} ${flags}")
    set_target_properties(${target} PROPERTIES COMPILE_FLAGS "${new_compile_flags}")
endfunction(add_compile_flags)

#
# Generate a loadable detector module
#
function(create_module module_name link_libraries)
    add_library(${module_name} MODULE
        ${${module_name}_SRC}
        ${${module_name}_INC})
    set_target_properties(${module_name} PROPERTIES PREFIX "")
    set_target_properties(${module_name} PROPERTIES SUFFIX ".so")
    install(TARGETS ${module_name} DESTINATION bin)
    enable_coverage(${module_name})

    add_dependencies(${module_name}
        eventlib
        )
    target_link_libraries(${module_name}
        ${EFU_COMMON_LIBS}
        ${link_libraries}
        eventlib
        )
endfunction(create_module)

#
# Generate an executable program
#
function(create_executable exec_name link_libraries)
    add_executable(${exec_name}
        ${${exec_name}_SRC}
        ${${exec_name}_INC})
    install(TARGETS ${exec_name} DESTINATION bin)
    enable_coverage(${exec_name})

    add_dependencies(${exec_name}
        eventlib)
    target_link_libraries(${exec_name}
        ${EFU_COMMON_LIBS}
        ${link_libraries}
        eventlib
        )
endfunction(create_executable)



#
# To generate unit test targets
#

set(unit_test_targets "" CACHE INTERNAL "All targets")

function(create_test_executable exec_name link_libraries)
    # message(STATUS ${exec_name})
    # message(STATUS ${unit_test_targets})
    add_executable(${exec_name} EXCLUDE_FROM_ALL
        ${${exec_name}_SRC}
        ${${exec_name}_INC} )
    target_include_directories(${exec_name}
        PRIVATE ${GTEST_INCLUDE_DIRS})
    set_target_properties(${exec_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/unit_tests")
    enable_coverage(${exec_name})

    target_link_libraries(${exec_name}
        ${link_libraries}
        ${EFU_COMMON_LIBS}
        ${CMAKE_DL_LIBS}
        ${GTEST_LIBRARIES}
        )

#    set_target_properties(${exec_name} PROPERTIES
#        COMPILE_FLAGS "-Wno-error")

    add_test(NAME regular_${exec_name}
        COMMAND ${exec_name} "--gtest_output=xml:${CMAKE_BINARY_DIR}/test_results/${exec_name}test.xml")

    set(unit_test_targets ${unit_test_targets} ${exec_name} CACHE INTERNAL "All targets")

    if (EXISTS ${VALGRIND_CMD})
        add_test(NAME memcheck_${exec_name} COMMAND ${VALGRIND_CMD} --tool=memcheck --leak-check=full --verbose --xml=yes --xml-file=${CMAKE_BINARY_DIR}/memcheck_res/${exec_name}test.valgrind ${CMAKE_BINARY_DIR}/unit_tests/${exec_name})
    endif()
endfunction(create_test_executable)



#
# To generate benchmark targets
#

set(benchmark_targets "" CACHE INTERNAL "All targets")

function(create_benchmark_executable exec_name link_libraries)
  if (GOOGLE_BENCHMARK)
    add_executable(${exec_name} EXCLUDE_FROM_ALL ${${exec_name}_SRC} ${${exec_name}_INC} )
    target_include_directories(${exec_name} PRIVATE ${GTEST_INCLUDE_DIRS})
    set_target_properties(${exec_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/benchmarks")

    target_link_libraries(${exec_name}
        ${link_libraries}
        ${GTEST_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        -lbenchmark)
    set(benchmark_targets
        ${exec_name}
        ${benchmark_targets}
        CACHE INTERNAL "All targets")
  else ()
    message(STATUS "skipping benchmark for ${exec_name} (can be enabled by cmake -DGOOGLE_BENCHMARK=YES)")
  endif()
endfunction(create_benchmark_executable)
