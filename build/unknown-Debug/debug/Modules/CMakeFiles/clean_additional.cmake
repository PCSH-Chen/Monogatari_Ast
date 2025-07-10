# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "BaseInfo_autogen"
  "CMakeFiles\\BaseInfo_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\BaseInfo_autogen.dir\\ParseCache.txt"
  )
endif()
