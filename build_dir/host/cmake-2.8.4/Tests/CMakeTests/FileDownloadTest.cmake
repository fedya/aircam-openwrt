set(url "file:///home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/host/cmake-2.8.4/Tests/CMakeTests/FileDownloadInput.png")
set(dir "/home/fdkit/GPL-AirCam-v1.2/openwrt/build_dir/host/cmake-2.8.4/Tests/CMakeTests/downloads")

message(STATUS "FileDownload:1")
file(DOWNLOAD
  ${url}
  ${dir}/file1.png
  TIMEOUT 2
  )

message(STATUS "FileDownload:2")
file(DOWNLOAD
  ${url}
  ${dir}/file2.png
  TIMEOUT 2
  SHOW_PROGRESS
  )

# Two calls in a row, exactly the same arguments.
# Since downloaded file should exist already for 2nd call,
# the 2nd call will short-circuit and return early...
#
if(EXISTS ${dir}/file3.png)
  file(REMOVE ${dir}/file3.png)
endif()

message(STATUS "FileDownload:3")
file(DOWNLOAD
  ${url}
  ${dir}/file3.png
  TIMEOUT 2
  EXPECTED_MD5 d16778650db435bda3a8c3435c3ff5d1
  )

message(STATUS "FileDownload:4")
file(DOWNLOAD
  ${url}
  ${dir}/file3.png
  TIMEOUT 2
  EXPECTED_MD5 d16778650db435bda3a8c3435c3ff5d1
  )
