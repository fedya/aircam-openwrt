CMAKE_MINIMUM_REQUIRED(VERSION 2.1)

# Settings:
SET(CTEST_DASHBOARD_ROOT                "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/host/cmake-2.8.4/Tests/CTestTest")
SET(CTEST_SITE                          "discordy")
SET(CTEST_BUILD_NAME                    "CTestTest-Linux-g++-Scheduler")

SET(CTEST_SOURCE_DIRECTORY              "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/host/cmake-2.8.4/Tests/CTestTestScheduler")
SET(CTEST_BINARY_DIRECTORY              "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/host/cmake-2.8.4/Tests/CTestTestScheduler")
SET(CTEST_CVS_COMMAND                   "CVSCOMMAND-NOTFOUND")
SET(CTEST_CMAKE_GENERATOR               "Unix Makefiles")
SET(CTEST_BUILD_CONFIGURATION           "$ENV{CMAKE_CONFIG_TYPE}")
SET(CTEST_MEMORYCHECK_COMMAND           "MEMORYCHECK_COMMAND-NOTFOUND")
SET(CTEST_MEMORYCHECK_SUPPRESSIONS_FILE "")
SET(CTEST_MEMORYCHECK_COMMAND_OPTIONS   "")
SET(CTEST_COVERAGE_COMMAND              "/usr/bin/gcov")
SET(CTEST_NOTES_FILES                   "${CTEST_SCRIPT_DIRECTORY}/${CTEST_SCRIPT_NAME}")

#CTEST_EMPTY_BINARY_DIRECTORY(${CTEST_BINARY_DIRECTORY})

# Remove old cost data file if it exists
IF(EXISTS "${CTEST_BINARY_DIRECTORY}/Testing/Temporary/CTestCostData.txt")
  FILE(REMOVE "${CTEST_BINARY_DIRECTORY}/Testing/Temporary/CTestCostData.txt")
ENDIF(EXISTS "${CTEST_BINARY_DIRECTORY}/Testing/Temporary/CTestCostData.txt")

CTEST_START(Experimental)
CTEST_CONFIGURE(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)
CTEST_BUILD(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res)
CTEST_TEST(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res PARALLEL_LEVEL 5)
# Run test set a second time to make sure they run in reverse order
CTEST_TEST(BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE res PARALLEL_LEVEL 5)
