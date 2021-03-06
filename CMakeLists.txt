#-------------------------------------------------
#  Copyright 2020 ESRI
#
#  All rights reserved under the copyright laws of the United States
#  and applicable international laws, treaties, and conventions.
#
#  You may freely redistribute and use this sample code, with or
#  without modification, provided you include the original copyright
#  notice and use restrictions.
#
#  See the Sample code usage restrictions document for further information.
#-------------------------------------------------
cmake_minimum_required(VERSION 3.5)

project(display_a_scene LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS REQUIRED Core Quick Multimedia Positioning Sensors)
find_package(ArcGISRuntime 100.11 COMPONENTS REQUIRED Cpp)

set(SOURCE_FILES
  main.cpp
  Display_a_scene.cpp
  qml/qml.qrc
  Resources/Resources.qrc
  $<$<BOOL:${WIN32}>:Win/Resources.rc>
  $<$<BOOL:${APPLE}>:Mac/AppIcon.icns>)

if(ANDROID)
  add_library(display_a_scene SHARED ${SOURCE_FILES})
else()
  add_executable(display_a_scene MACOSX_BUNDLE ${SOURCE_FILES})

  # On MacOSX add icon to app bundle.
  set(MACOSX_BUNDLE_ICON_FILE AppIcon.icns)
  set_source_files_properties(Mac/AppIcon.icns
    PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

  # Copy required dynamic libraries to the build folder as a post-build step.
  if(DEFINED ArcGISRuntime_LIBRARIES)
    add_custom_command(TARGET display_a_scene POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${ArcGISRuntime_LIBRARIES}
      $<TARGET_FILE_DIR:display_a_scene>)
  endif()
endif()

target_compile_definitions(display_a_scene
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(display_a_scene PRIVATE
  Qt5::Core
  Qt5::Quick
  Qt5::Multimedia
  Qt5::Positioning
  Qt5::Sensors
  ArcGISRuntime::Cpp)

if(ANDROID)
  find_package(Qt5 COMPONENTS REQUIRED AndroidExtras)
  target_link_libraries(display_a_scene PRIVATE Qt5::AndroidExtras)

  set(PROJECT_DEPLOYABLE_LIBS
    ${ArcGISRuntime_LIBRARIES})

  list(JOIN PROJECT_DEPLOYABLE_LIBS , PROJECT_DEPLOYABLE_LIBS_STRING)

  # QtCreator supports the following variables for Android, which are identical
  # to qmake Android variables.
  # Check http://doc.qt.io/qt-5/deployment-android.html for more information.
  # These variables must use CACHE, otherwise QtCreator won't see them.
  set(ANDROID_EXTRA_LIBS ${PROJECT_DEPLOYABLE_LIBS_STRING} CACHE INTERNAL "")
endif()
