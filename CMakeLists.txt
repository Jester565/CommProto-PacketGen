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

option(release "Compile the release version of PacketGen" OFF)
option(diag "Allows for generation of byte diagrams (requires allegro)" OFF)
option(alleg "On mean that directory of allegro's lib files will be specified" OFF)

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
		
if(diag)
	set(alleg "-" CACHE STRING "The path to allegro's static library")
	if ("-" STREQUAL alleg)

  else()
    
	endif()
	add_definitions(-DDIAGRAM)
	set(PACKETGEN_HEADERS 
		${PACKETGEN_HEADERS}
		${HEADER_DIR}/DiagramManager.h
		${HEADER_DIR}/DiagramRunner.h)
	set(PACKETGEN_SOURCES
		${PACKETGEN_SOURCES}
		${SOURCE_DIR}/DiagramManager.cpp
		${SOURCE_DIR}/DiagramRunner.cpp)
	set(DEP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/)
	configure_file(${DEP_DIR}/Calibri.ttf Calibri.ttf COPYONLY)
endif()

set (NGCP_GLOB
  ${PACKETGEN_HEADERS}
  ${PACKETGEN_SOURCES})

if (WIN32)
	if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
		add_definitions(-DPAUSE)
		source_group("Header" FILES ${PACKETGEN_HEADERS})
		source_group("Source" FILES ${PACKETGEN_SOURCES})
		set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT PacketGen)
	endif()
endif()

add_executable(PacketGen ${NGCP_GLOB})

if(diag)
	if(alleg)
	  set(allegdir "/usr/lib/x86_64-linux-gnu/" CACHE STRING "The directories of allegro's libraries")
	  set(alleglib "liballegro.so" CACHE STRING "The path of the main allegro library relative to allegdir")
	  set(ttflib "liballegro_ttf.so" CACHE STRING "The path of the ttf allegro extension relative to allegdir")
	  set(imglib "liballegro_image.so" CACHE STRING "The path of the img allegro extension relative to allegdir")
	  set(primlib "liballegro_primitives.so" CACHE STRING "The path of the primtive allegro extension relative to allegdir")
	  set(fontlib "liballegro_font.so" CACHE STRING "The path of the font allegro extension relative to allegdir")
		target_link_libraries(PacketGen ${allegdir}${alleglib})
		target_link_libraries(PacketGen ${allegdir}${ttflib})
		target_link_libraries(PacketGen ${allegdir}${imglib})
		target_link_libraries(PacketGen ${allegdir}${primlib})
		target_link_libraries(PacketGen ${allegdir}${fontlib})
	endif()
endif()

set_property(TARGET PacketGen PROPERTY CXX_STANDARD 11)
set_property(TARGET PacketGen PROPERTY CXX_STANDARD_REQUIRED 11)
