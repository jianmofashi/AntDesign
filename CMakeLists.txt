cmake_minimum_required(VERSION 3.16)
project(QtAntDesign LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS
    Core Gui Widgets Multimedia MultimediaWidgets OpenGLWidgets Svg
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/QtAntDesign
    ${CMAKE_CURRENT_SOURCE_DIR}/QtAntDesign/ThirdParty/QR-Code-generator-master
    ${CMAKE_CURRENT_SOURCE_DIR}/QtAntDesign/ThirdParty/FastGaussianBlur-main
)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/QtAntDesign/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/QtAntDesign/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/QtAntDesign/QtAntDesign.qrc)

qt_standard_project_setup()
qt6_wrap_cpp(MOC_SOURCES ${HEADERS})
qt6_add_resources(QRC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/QtAntDesign/QtAntDesign.qrc)
target_sources(${PROJECT_NAME} PRIVATE ${MOC_SOURCES} ${QRC_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets Qt6::OpenGLWidgets Qt6::Svg
)

if(WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()
