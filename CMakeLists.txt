# REVIEW THE FOLLOWING IN ORDER TO COMPILE
# 
# 1. if on windows, install vcpkg, use it to install qt5 and set VcpkgPath

cmake_minimum_required(VERSION 3.1.0)

project(rook)

set(CMAKE_CXX_STANDARD 14)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

# Auto handling of qrc resource files
set(CMAKE_AUTORCC ON)

# Set Qt Path
if (WIN32)
    set(VcpkgPath "E:/vcpkg/packages")
    set(CMAKE_PREFIX_PATH ${QtPath})
endif (WIN32)

# Find the Qt libraries
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Sql REQUIRED)

# Populate a CMake variable with the sources
set(rook_SRCS
        src/appearancePage.cpp
        src/appearancePage.h
        src/bidDialog.cpp
        src/bidDialog.h
        src/bidDialog.ui
        src/card.cpp
        src/card.h
        src/clickableCard.cpp
        src/clickableCard.h
        src/common.h
        src/cpu.cpp
        src/cpu.h
        src/deck.cpp
        src/deck.h
        src/gameData.cpp
        src/gameData.h
        src/gameDatabase.cpp
        src/gameDatabase.h
        src/gameInfoWidget.cpp
        src/gameInfoWidget.h
        src/gameMenuWidget.cpp
        src/gameMenuWidget.h
        src/gamePage.cpp
        src/gamePage.h
        src/handInfo.cpp
        src/handInfo.h
        src/main.cpp
        src/mainWidget.cpp
        src/mainWidget.h
        src/mainWindow.cpp
        src/mainWindow.h
        src/messageBox.cpp
        src/messageBox.h
        src/middleDialog.cpp
        src/middleDialog.h
        src/nestDialog.cpp
        src/nestDialog.h
        src/overallInfo.cpp
        src/overallInfo.h
        src/player.cpp
        src/player.h
        src/partnerDialog.cpp
        src/partnerDialog.h
        src/preferencesDialog.cpp
        src/preferencesDialog.h
        src/resources.qrc
        src/roundInfo.h
        src/roundInfo.cpp
        src/roundSummaryDialog.cpp
        src/roundSummaryDialog.h
        src/saveSlotDialog.cpp
        src/saveSlotDialog.h
        src/saveSlotPage.cpp
        src/saveSlotPage.h
        src/scaledWidgets.cpp
        src/scaledWidgets.h
        src/scoresDialog.cpp
        src/scoresDialog.h
        src/settings.cpp
        src/settings.h
        src/trumpDialog.cpp
        src/trumpDialog.h
        src/utils.cpp
        src/utils.h
)

if (WIN32)
    add_executable(rook WIN32 ${rook_SRCS})
endif (WIN32)

if (UNIX)
    add_executable(rook ${rook_SRCS})
endif (UNIX)

# Copy Qt debug dlls
if (WIN32)
    configure_file(${VcpkgPath}/qt5-base_x86-windows/debug/bin/Qt5Widgetsd.dll ${CMAKE_CURRENT_BINARY_DIR}/Qt5Widgetsd.dll COPYONLY)
    configure_file(${VcpkgPath}/qt5-base_x86-windows/debug/bin/Qt5Cored.dll ${CMAKE_CURRENT_BINARY_DIR}/Qt5Cored.dll COPYONLY)
    configure_file(${VcpkgPath}/qt5-base_x86-windows/debug/bin/Qt5Guid.dll ${CMAKE_CURRENT_BINARY_DIR}/Qt5Guid.dll COPYONLY)
    configure_file(${VcpkgPath}/qt5-base_x86-windows/debug/bin/Qt5Sqld.dll ${CMAKE_CURRENT_BINARY_DIR}/Qt5Sqld.dll COPYONLY)
    configure_file(${VcpkgPath}/harfbuzz_x86-windows/debug/bin/harfbuzz.dll ${CMAKE_CURRENT_BINARY_DIR}/harfbuzz.dll COPYONLY)
    configure_file(${VcpkgPath}/libpng_x86-windows/debug/bin/libpng16d.dll ${CMAKE_CURRENT_BINARY_DIR}/libpng16d.dll COPYONLY)
    configure_file(${VcpkgPath}/zlib_x86-windows/debug/bin/zlibd1.dll ${CMAKE_CURRENT_BINARY_DIR}/zlibd1.dll COPYONLY)
    configure_file(${VcpkgPath}/icu_x86-windows/debug/bin/icuucd65.dll ${CMAKE_CURRENT_BINARY_DIR}/icuucd65.dll COPYONLY)
    configure_file(${VcpkgPath}/icu_x86-windows/debug/bin/icuind65.dll ${CMAKE_CURRENT_BINARY_DIR}/icuind65.dll COPYONLY)
    configure_file(${VcpkgPath}/icu_x86-windows/debug/bin/icudtd65.dll ${CMAKE_CURRENT_BINARY_DIR}/icudtd65.dll COPYONLY)
    configure_file(${VcpkgPath}/pcre2_x86-windows/debug/bin/pcre2-16d.dll ${CMAKE_CURRENT_BINARY_DIR}/pcre2-16d.dll COPYONLY)
    configure_file(${VcpkgPath}/freetype_x86-windows/debug/bin/freetyped.dll ${CMAKE_CURRENT_BINARY_DIR}/freetyped.dll COPYONLY)
    configure_file(${VcpkgPath}/bzip2_x86-windows/debug/bin/bz2d.dll ${CMAKE_CURRENT_BINARY_DIR}/bz2d.dll COPYONLY)
endif (WIN32)

# link the Qt libraries
target_link_libraries(rook Qt5::Widgets Qt5::Core Qt5::Sql)
