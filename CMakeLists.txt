cmake_minimum_required(VERSION 3.0.0)
project(Test VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB_RECURSE TARGET_SRC "src/*.cpp" "src/*.c" "include/*.cpp" "./dependencies/src/*.cpp" "./dependencies/src/*.c")

include(/home/laperex/.local/share/vcpkg/scripts/buildsystems/vcpkg.cmake)
# include_directories("./dependencies/include/" src)
include_directories("./dependencies/include/" include src /home/laperex/.local/share/vcpkg/installed/x64-linux/include)

include_directories("C:/libs/Lucy Graphics Library/include")

set(CMAKE_CXX_STANDARD 20)

add_executable(main ${TARGET_SRC})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(main PRIVATE SDL2::SDL2 SDL2::SDL2main)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(main PRIVATE imgui::imgui)

find_package(imguizmo CONFIG REQUIRED)
target_link_libraries(main PRIVATE imguizmo::imguizmo)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(main PRIVATE assimp::assimp)

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(main PRIVATE yaml-cpp)

find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
target_include_directories(main PRIVATE ${STB_INCLUDE_DIRS})

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(main PRIVATE EnTT::EnTT)

find_package(glm CONFIG REQUIRED)
target_link_libraries(main PRIVATE glm::glm)

find_package(glad CONFIG REQUIRED)
target_link_libraries(main PRIVATE glad::glad)

find_package(stduuid CONFIG REQUIRED)
target_link_libraries(main PRIVATE stduuid)

# find_package(OpenCV CONFIG REQUIRED)
set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-linux/share/opencv2")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(main PRIVATE ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
