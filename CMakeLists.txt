cmake_minimum_required(VERSION 3.0.0)
project(snap2text VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -lstdc++fs -luuid -rdynamic")

set(SW_BUILD_SHARED_LIBS 1)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTKMM gtkmm-3.0)
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
ADD_DEFINITIONS(${GTKMM_CFLAGS_OTHER})

pkg_check_modules(LEPT lept)
include_directories(${LEPT_INCLUDE_DIRS})
link_directories(${LEPT_LIBRARY_DIRS})
ADD_DEFINITIONS(${LEPT_CFLAGS_OTHER})

pkg_check_modules(TESS tesseract)
include_directories(${TESS_INCLUDE_DIRS})
link_directories(${TESS_LIBRARY_DIRS})
ADD_DEFINITIONS(${TESS_CFLAGS_OTHER})

pkg_check_modules(CURL libcurl)
include_directories(${CURL_INCLUDE_DIRS})
link_directories(${CURL_LIBRARY_DIRS})
ADD_DEFINITIONS(${CURL_CFLAGS_OTHER})

pkg_check_modules(CURLPP curlpp)
include_directories(${CURLPP_INCLUDE_DIRS})
link_directories(${CURLPP_LIBRARY_DIRS})
ADD_DEFINITIONS(${CURLPP_CFLAGS_OTHER})

pkg_check_modules(SQLITE3 sqlite3)
include_directories(${SQLITE3_INCLUDE_DIRS})
link_directories(${SQLITE3_LIBRARY_DIRS})
ADD_DEFINITIONS(${SQLITE3_CFLAGS_OTHER})

pkg_check_modules(JSONCPP jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})
link_directories(${JSONCPP_LIBRARY_DIRS})
ADD_DEFINITIONS(${JSONCPP_CFLAGS_OTHER})

PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

pkg_search_module(UUID REQUIRED uuid)
INCLUDE_DIRECTORIES(${UUID_INCLUDE_DIRS})
LINK_DIRECTORIES(${UUID_LIBRARY_DIRS})
ADD_DEFINITIONS(${UUID_CFLAGS_OTHER})

add_executable(snap2text main.cc 
                apihelper.cc 
                apihelper.h 
                appintegrity.h 
                appintegrity.cc 
                areacapturewindow.cc 
                areacapturewindow.h
                document.cc
                document.h
                drawingareawindow.cc
                drawingareawindow.h
                globals.cc
                globals.h
                googletranslator.cc
                googletranslator.h
                gridmodel.h
                languagehelper.cc
                languagehelper.h
                menuappwindow.cc
                menuappwindow.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


target_include_directories(snap2text PUBLIC ${GTK3_INCLUDE_DIRS} 
                                ${GTKMM_INCLUDE_DIRS}
                                ${LEPT_INCLUDE_DIRS}
                                ${TESS_INCLUDE_DIRS}
                                ${CURL_INCLUDE_DIRS}
                                ${CURLPP_INCLUDE_DIRS}
                                ${SQLITE3_INCLUDE_DIRS}
                                ${JSONCPP_INCLUDE_DIRS}
                                ${GTK3_INCLUDE_DIRS}
                                ${UUID_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(snap2text ${GTK3_LIBRARIES} 
                                ${GTKMM_LIBRARIES}
                                ${LEPT_LIBRARIES}
                                ${TESS_LIBRARIES}
                                ${CURL_LIBRARIES}
                                ${CURLPP_LIBRARIES}
                                ${SQLITE3_LIBRARIES}
                                ${JSONCPP_LIBRARIES}
                                ${GTK3_LIBRARIES}
                                ${UUID_LIBRARIES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/static $<TARGET_FILE_DIR:${PROJECT_NAME}>/static)
