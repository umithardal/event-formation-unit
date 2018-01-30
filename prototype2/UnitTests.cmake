if(NOT ${GTEST_FOUND})
    message(WARNING "unable to build unit tests as GTest was not found.")
    return()
endif()

enable_testing()

find_program(VALGRIND_CMD valgrind)
if(EXISTS ${VALGRIND_CMD})
    message(STATUS "valgrind found.")
else()
    message(STATUS "valgrind not found. Unable to run memory check.")
endif()

mark_as_advanced(VALGRIND_CMD)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/multigrid/calib_data DESTINATION ${CMAKE_BINARY_DIR}/prototype2)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/memcheck_res/)

#
# EFU Tests
#
if(${CMAKE_COMPILER_IS_GNUCXX})
    set(ParserTest_LIB_SRC
        efu/ParserTest.cpp
        test/SyscallWrappers.cpp
        common/NewStats.cpp
        common/NewStats.h)
    add_library(ParserTest_LIB STATIC
        ${ParserTest_LIB_SRC})
    add_compile_flags(ParserTest_LIB
        "-Wl,--wrap=fstat,--wrap=read,--wrap=write,--wrap=open -Wno-error")

    set(ParserTest_SRC
        efu/Parser.cpp
        common/EFUArgs.cpp
        $<TARGET_OBJECTS:calibfile>)
    set(ParserTest_INC
        efu/Parser.h
        common/EFUArgs.h)
    create_test_executable(ParserTest "ParserTest_LIB")
    add_compile_flags(ParserTest
        "-Wl,--wrap=fstat,--wrap=read,--wrap=write,--wrap=open")
    add_linker_flags(ParserTest
        "-Wl,--wrap=fstat,--wrap=read,--wrap=write,--wrap=open")
  else()
    message(STATUS "skipping ParserTest - requires gcc/linux")
endif()



#
# common/ Tests
#
set(VersionTest_SRC
    common/VersionTest.cpp
    common/Version.h)
create_test_executable(VersionTest "")

set(ESSGeometryTest_SRC
    common/ESSGeometryTest.cpp
    common/ESSGeometry.h)
create_test_executable(ESSGeometryTest "")


set(RingBufferTest_SRC
    common/RingBufferTest.cpp
    common/RingBuffer.h)
create_test_executable(RingBufferTest "")


set(DetectorTest_SRC
    common/DetectorTest.cpp
    common/NewStats.cpp
    common/NewStats.h
    common/Detector.h)
create_test_executable(DetectorTest "")


set(MultiGridGeometryTest_SRC
    multigrid/mgcncs/MultigridGeometryTest.cpp
    multigrid/mgcncs/MultigridGeometry.h
    ../libs/include/TSCTimer.h)
create_test_executable(MultiGridGeometryTest "")


set(FBSerializerTest_SRC
    common/FBSerializerTest.cpp
    common/FBSerializer.cpp
    common/Producer.cpp)
set(FBSerializerTest_INC
    common/FBSerializer.h
    common/Producer.h)
create_test_executable(FBSerializerTest "")

if(${CMAKE_COMPILER_IS_GNUCXX})
 #AND (NOT ${SKIP_PRODUCER_TEST}))
  set(ProducerTest_SRC
      common/ProducerTest.cpp
      common/Producer.cpp)
  set(ProducerTest_INC
      common/Producer.h)
  create_test_executable(ProducerTest "")
else()
  message(WARNING "skipping ProducerTest - requires gcc/linux")
endif()

set(ReadoutSerializerTest_SRC
    common/ReadoutSerializerTest.cpp
    common/ReadoutSerializer.cpp
    common/Producer.cpp)
set(ReadoutSerializerTest_INC
    common/ReadoutSerializer.h
    common/Producer.cpp)
create_test_executable(ReadoutSerializerTest "")


set(EFUArgsTest_SRC
    common/EFUArgsTest.cpp
    common/EFUArgs.cpp
    common/EFUArgs.h)
create_test_executable(EFUArgsTest "")

set(NewStatsTest_SRC
    common/NewStatsTest.cpp
    common/NewStats.cpp
    common/NewStats.h)
create_test_executable(NewStatsTest "")



#
# ESS Readout System Tests
#
set(ReadoutTest_SRC
    readout/ReadoutTest.cpp
    readout/Readout.cpp)
set(ReadoutTest_INC
    test/TestBase.h
    readout/Readout.h
    readout/ReadoutTestData.h)
create_test_executable(ReadoutTest "")

set(ReadoutDummyTest_SRC
    readout/ReadoutDummyTest.cpp
    readout/Readout.cpp
    readout/ReadoutDummy.cpp)
set(ReadoutDummyTest_INC
    test/TestBase.h
    readout/Readout.h
    readout/ReadoutDummy.h
    readout/ReadoutTestData.h)
create_test_executable(ReadoutDummyTest "")





#
# Libs unit tests
#

include_directories(..)

set(CounterTest_SRC
    ../libs/test/Counter/testmain.cpp
    ../libs/test/Counter/CounterTest.cpp)
set(CounterTest_INC
    ../libs/include/Counter.h)
create_test_executable(CounterTest "")

set(StatCounterTest_SRC
    ../libs/test/Counter/testmain.cpp
    ../libs/test/Counter/StatCounterTest.cpp)
set(StatCounterTest_INC
    ../libs/include/StatCounter.h)
create_test_executable(StatCounterTest "")


#
#  These commands must be at the end of this file
#
add_custom_target(runtest
    COMMAND ${CMAKE_CTEST_COMMAND} -V -R regular_*
    DEPENDS ${unit_test_targets})
add_custom_target(valgrind
    COMMAND ${CMAKE_CTEST_COMMAND} -R memcheck_*
    DEPENDS ${unit_test_targets})
add_custom_target(benchmark
    DEPENDS ${benchmark_targets})
