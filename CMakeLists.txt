cmake_minimum_required(VERSION 3.18) 
project(ZMath LANGUAGES CXX)

option(USE_FP64 "Enable Double Precision" ON)
add_definitions(-D_FP64)

# 设置安装路径（默认在 build/install 目录，可自定义）
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "安装路径" FORCE)
endif()


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES 
    "src/*.cpp"    # C++ 源文件
)
# 自动扫描头文件目录
file(GLOB_RECURSE HEADER_FILES "include/*.h" "include/*.hpp")


# 创建库
add_executable(ZMathTest test/test_zmath.cpp)
add_library(ZMath SHARED ${SOURCE_FILES})

# 源文件中的导出宏
target_compile_definitions(ZMath PRIVATE ZMATH_EXPORTS) 

# 包含头文件目录
target_include_directories(ZMath PRIVATE include)

# 链接测试可执行文件到你的数学库
target_link_libraries(ZMathTest PRIVATE ZMath)

# 包含头文件目录（如果头文件在 include/ 下）
target_include_directories(ZMathTest PRIVATE include)


# IDE中显示头文件
target_sources(ZMath PRIVATE
    ${HEADER_FILES}
)

# 安装规则
install(
    TARGETS ZMath
    ARCHIVE   # 导入库 .lib（静态库或动态库的导入库）
        DESTINATION lib
        COMPONENT Runtime
    LIBRARY   # Linux/macOS 动态库，Windows 忽略
        DESTINATION lib
    RUNTIME   # Windows 动态库 .dll
        DESTINATION lib
)

# 安装头文件
install(
    DIRECTORY include/    # 源头文件目录（末尾必须有 `/`）
    DESTINATION include   # 目标安装目录
    FILES_MATCHING
    PATTERN "*.h"        # 仅安装 .h 文件
    PATTERN "*.hpp"
    EXCLUDE              # 排除临时文件（如 .h~）
)