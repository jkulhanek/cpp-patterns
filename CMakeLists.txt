cmake_minimum_required (VERSION 3.4)

project(cpp-patterns)

enable_testing()
#find_package(Boost COMPONENTS unit_test_framework REQUIRED)
#include_directories (${Boost_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} tests/*.cpp)

foreach(testSrc ${TEST_SRCS})
        #Extract the filename without an extension (NAME_WE)
        get_filename_component(testName ${testSrc} NAME_WE)


        #Add compile target
        add_executable(${testName} ${testSrc})

        #link to Boost libraries AND your targets and dependencies
       #target_link_libraries(${testName} ${Boost_LIBRARIES})

        #I like to move testing binaries into a testBin directory
        set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/bin)

        #Finally add it to test execution - 
        #Notice the WORKING_DIRECTORY and COMMAND
        add_test(NAME ${testName} 
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin 
                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/${testName} )
endforeach(testSrc)