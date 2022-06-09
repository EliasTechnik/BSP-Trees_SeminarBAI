# CMakeList.txt: CMake-Projekt für "draft2". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

project ("draft2")

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (draft2 "draft2.cpp" "draft2.h" "bsptree.h" "bsptree.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET draft2 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
