if(APPLE)
	set(BASE "darwin")
endif(APPLE)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(BASE "linux")
endif()

if(UNIX)
	EXECUTE_PROCESS(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)
	set(PLATFORM "${BASE}_${ARCHITECTURE}")
	set(TEST_RUNNER_NAME "test${PROJECT_NAME}_${PLATFORM}")
endif(UNIX)

if(WIN32)
	set(TEST_RUNNER_NAME "test${PROJECT_NAME}_retardedOS.exe")
endif(WIN32)
if(APPLE)
	set(BASE "darwin")
endif(APPLE)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(BASE "linux")
endif()

set(TEST_RUNNER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/test/binaries/${TEST_RUNNER_NAME})

math(EXPR UNLIMITED_MS 60*1000)
math(EXPR UNLIMITED_KB 1024*1024)
option(UNLIMITED "Disable time/memory limits" OFF)

include(valgrind)

include (CTest)
enable_testing()

if (UNLIMITED)
	add_test(
		NAME ${TEST_RUNNER_NAME}
		COMMAND ${TEST_RUNNER_PATH} -m ${UNLIMITED_KB} -t ${UNLIMITED_MS} ./${PROJECT_NAME}
	)
else()
	add_test(
		NAME ${TEST_RUNNER_NAME}
		COMMAND ${TEST_RUNNER_PATH} ./${PROJECT_NAME}
	)
endif()

set_tests_properties(${TEST_RUNNER_NAME} PROPERTIES DEPENDS ${PROJECT_NAME})
