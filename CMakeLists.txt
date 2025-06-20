﻿# CMakeList.txt : CMake project for cams, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (cams LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source to this project's executable.
add_executable (
	cams "src/cams.cpp"
	"src/tools.h" "src/tools.cpp"
	 "src/fsmanager.h" "src/fsmanager.cpp"
	"src/logger.h" "src/logger.cpp"
	"src/parameters.h" "src/parameters.cpp"
	 "src/dispatcher.h" "src/dispatcher.cpp"
	 "src/file.h" "src/file.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cams PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
