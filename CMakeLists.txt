####################################################################################
#  Main CMake script for which to run the build                                    #
#                                                                                  #
#  Copyright(C) 2016  Alex Craig, Michael Wallace, Mario Garcia.                   #
#                                                                                  #
#  This program is free software : you can redistribute it and / or modify         #
#  it under the terms of the GNU General Public License as published by            #
#  the Free Software Foundation, either version 3 of the License, or               #
#  (At your option) any later version.                                             #
#                                                                                  #
#  This program is distributed in the hope that it will be useful,                 #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of                  #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                    #
#  GNU General Public License for more details.                                    #
#                                                                                  #
#  You should have received a copy of the GNU General Public License               #
#  along with this program.If not, see <http://www.gnu.org/licenses/>.             #
#                                                                                  #
####################################################################################
cmake_minimum_required( VERSION 2.6 )

project( "PacketGen" )

option(release "Compile the release version of PacketGen")

if (release)
  set (CMAKE_BUILD_TYPE Release)
else()
  set (CMAKE_BUILD_TYPE Debug)
endif()

include_directories(SYSTEM PacketGen/include/)

set(TEMPLATE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/PacketGen/template/)

configure_file(${TEMPLATE_DIR}/template.h template.h COPYONLY)
configure_file(${TEMPLATE_DIR}/template.cs template.cs COPYONLY)

set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/PacketGen/include/)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/PacketGen/src/)

set(PACKETGEN_HEADERS
  ${HEADER_DIR}/CommandArgument.h
		${HEADER_DIR}/CommandArgumentManager.h
		${HEADER_DIR}/CppRunner.h
		${HEADER_DIR}/CsRunner.h
		${HEADER_DIR}/FileManager.h
		${HEADER_DIR}/Message.h
		${HEADER_DIR}/Parser.h
		${HEADER_DIR}/Runner.h
		${HEADER_DIR}/TypeMap.h)

set(PACKETGEN_SOURCES
  ${SOURCE_DIR}/CommandArgument.cpp
  ${SOURCE_DIR}/CommandArgumentManager.cpp
  ${SOURCE_DIR}/CppRunner.cpp
		${SOURCE_DIR}/CsRunner.cpp
		${SOURCE_DIR}/Message.cpp
		${SOURCE_DIR}/Parser.cpp
		${SOURCE_DIR}/Runner.cpp
		${SOURCE_DIR}/Source.cpp
		${SOURCE_DIR}/TypeMap.cpp)

set (NGCP_GLOB
  ${PACKETGEN_HEADERS}
  ${PACKETGEN_SOURCES})

if (WIN32)
		if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
						source_group("Header" FILES ${PACKETGEN_HEADERS})
						source_group("Source" FILES ${PACKETGEN_SOURCES})
						set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT PacketGen)
		endif()
endif()

add_executable(PacketGen ${NGCP_GLOB})


set_property(TARGET PacketGen PROPERTY CXX_STANDARD 11)
set_property(TARGET PacketGen PROPERTY CXX_STANDARD_REQUIRED 11)