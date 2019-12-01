cmake_minimum_required(VERSION 3.6)

project(lifeGameFLTK)

set(FLTK_DIR C:/FLTK/fltk-1.3.5)
list(APPEND CMAKE_PREFIX_PATH ${FLTK_DIR})

find_package(FLTK REQUIRED NO_MODULE)

include_directories(${FLTK_INCLUDE_DIRS})
set(INIH_FILE ${CMAKE_SOURCE_DIR}/inih/INIReader.cpp ${CMAKE_SOURCE_DIR}/inih/ini.c)

add_executable(lifeGameFLTK WIN32 lifeGameFLTK.cpp lifeGame.cpp ${INIH_FILE})

target_link_libraries(lifeGameFLTK fltk)


set(FILE ${CMAKE_CURRENT_SOURCE_DIR}/lifeGameFLTK.ini)
execute_process( 	
	COMMAND ${CMAKE_COMMAND} -E copy ${FILE} ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E 
		echo "${CMAKE_COMMAND} -E copy ${FILE} ${CMAKE_CURRENT_BINARY_DIR}"
)

set(COPY_FOLDERS_LIST lgfiles)
foreach(FOLDER IN LISTS COPY_FOLDERS_LIST)
	execute_process( 	
		COMMAND ${CMAKE_COMMAND} -E copy_directory 
			${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER} ${CMAKE_CURRENT_BINARY_DIR}/${FOLDER}
		COMMAND ${CMAKE_COMMAND} -E 
			echo "${CMAKE_COMMAND} -E copy_directory
			 ${CMAKE_CURRENT_SOURCE_DIR}/${FOLDER} ${CMAKE_CURRENT_BINARY_DIR}/${FOLDER}"
	)
endforeach()