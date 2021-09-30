
# cmake 最低版本要求
cmake_minimum_required(VERSION 3.1.0)

# 工程名
if(NOT DEFINED PROJECT_NAME)
    project("Qt")
else()
    project(${PROJECT_NAME})
endif()

# Qt
# moc
set(CMAKE_AUTOMOC ON)
# 编译 *.rc 文件
set(CMAKE_AUTORCC ON)
# 编译 *.ui 文件 生成 ui_*.h
set(CMAKE_AUTOUIC ON)

# 查找Qt模块
# 需要设置环境变量 Qt5_DIR 指向 Qt5Config.cmake
# 或者添加到 CMAKE_PREFIX_PATH (推荐)
find_package(
    Qt5
    COMPONENTS
    Core
    Gui
    Widgets
    Concurrent
    Qml
    Quick
    Multimedia
    MultimediaWidgets
    Charts
    REQUIRED
)

find_package(
    OpenCV
    REQUIRED
)

# 导入所在目录
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    # 编译目录包含到 include 目录
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# 工作目录
if(NOT DEFINED CWD)
    # 默认CMakeList.txt所在目录
    set(CWD ${CMAKE_CURRENT_LIST_DIR})
else()
    # 转换为CMake风格路径
    file(TO_CMAKE_PATH ${CWD} CWD)
endif()
message(STATUS "Current workspace is: ${CWD}")

# 默认构建类型 Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "Build type is: ${CMAKE_BUILD_TYPE}")

# Release模式下 关闭控制台
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(WIN32)
        set(GUI_TYPE WIN32)
    elseif(APPLE)
        set(GUI_TYPE MACOSX_BUNDLE)
    endif()
endif()

# C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 编译参数
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # 指定异常处理模型
    set(CMAKE_CXX_FLAGS "-EHsc")
    # 启用调试信息
    # 编译创建一个调试多线程 DLL，使用msvcrtd.lib
    set(CMAKE_CXX_FLAGS_DEBUG "-Zi -MDd")
    # 最大优化(优选速度)
    # 打开整个程序优化
    # 编译创建一个调试多线程 DLL使用msvcrtd.lib
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -GL -MD")
else()
    message(WARNING "Please set the compiler flags")
endif()

# 目标输出路径
set(EXECUTABLE_OUTPUT_PATH ${CWD}/bin)
# 头文件
include_directories(
    ${CWD}/include
    ${OpenCV_INCLUDE_DIRS}
)
# 库文件
link_directories(
    ${CWD}/lib
    ${OpenCV_LIBRARY_DIRS}
)

# 头文件保存到变量中
file(GLOB HEADERS ${CWD}/include/*)
# 源文件保存到变量中
aux_source_directory(${CWD}/src SOURCES)

# 构建目标
add_executable(
    ${PROJECT_NAME}
    ${GUI_TYPE}
    ${SOURCES}
    ${HEADERS}
    src/MainWindow.ui
    src/Settings.ui
)

# 链接库文件
target_link_libraries(
    ${PROJECT_NAME}
    ${OpenCV_LIBS}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Concurrent
    Qt5::Qml
    Qt5::Quick
    Qt5::Multimedia
    Qt5::MultimediaWidgets
    Qt5::Charts
)
