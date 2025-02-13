cmake_minimum_required (VERSION 3.10.0)

project(SimpleHasher)

# Version Number
set (SIMPLE_HASHER_VERSION_MAJOR 1)
set (SIMPLE_HASHER_VERSION_MINOR 5)
set (SIMPLE_HASHER_VERSION_PATCH 0)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
							 
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

string(TIMESTAMP CURRENT_YEAR "%Y")

# Find the QtWidgets library
find_package(Qt6 COMPONENTS Widgets Multimedia)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  set(CORE_EXTERNAL_LIBS ${CORE_EXTERNAL_LIBS} ${QT_QTTEST_LIBRARY})
endif (CMAKE_BUILD_TYPE MATCHES Debug)

if(DEFINED MINGW)
  configure_file("${PROJECT_SOURCE_DIR}/resources.rc.in" "${PROJECT_BINARY_DIR}/resources.rc")
  configure_file("${PROJECT_SOURCE_DIR}/installer/script.iss.in" "${PROJECT_BINARY_DIR}/script.iss")
  set(CORE_SOURCES ${CORE_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/resources.rc)
  set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff -o <OBJECT> -i <SOURCE>")
  enable_language(RC)
endif(DEFINED MINGW)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}          # Generated .h files
  ${CMAKE_CURRENT_BINARY_DIR}  # For wrap/ui files
  )

set(CMAKE_CXX_FLAGS " -Wall -Wno-deprecated -std=c++17 -mwindows -m64")

# Add Qt Resource files
qt6_add_resources(RESOURCES
  rsc/resources.qrc
)

qt6_wrap_ui(CORE_UI
  # .ui for Qt
  AboutDialog.ui
  SimpleHasher.ui
  ConfigurationDialog.ui
)
	
set (CORE_SOURCES
  # project files
  ${CORE_SOURCES}
  ${RESOURCES}
  ${CORE_MOC}
  ${CORE_UI}
  main.cpp
  AboutDialog.cpp
  ComputerThread.cpp
  Hash.cpp
  SimpleHasher.cpp
  ConfigurationDialog.cpp
  Utils.cpp
  hash/MD5.cpp
  hash/SHA1.cpp
  hash/SHA224.cpp
  hash/SHA256.cpp
  hash/SHA384.cpp
  hash/SHA512.cpp
  hash/Tiger.cpp
  external/QTaskBarButton.cpp
)

set(CORE_EXTERNAL_LIBS
  Qt6::Widgets
  Qt6::Multimedia
)

add_executable(SimpleHasher ${CORE_SOURCES})
target_link_libraries (SimpleHasher ${CORE_EXTERNAL_LIBS})
