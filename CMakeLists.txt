cmake_minimum_required(VERSION 2.8.4)

project(BoatSimulation)

include_directories(include)
file(GLOB boatSimulation_SRC
    "src/*.cpp"
    )
set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
  message("we found glew ${GLEW_INCLUDE_DIRS}, ${GLEW_LIBRARIES}")
    include_directories(${GLEW_INCLUDE_DIRS})
endif()

#link_directories("C:/vcpkg/installed/x86-windows/lib")
link_directories("shapelib/build/dll/Debug")
link_directories(${CMAKE_SOURCE_DIR}/deps/lib)

#add_subdirectory(${CMAKE_SOURCE_DIR}/deps/SDL2-2.0.10)
add_executable(BoatSimulation ${boatSimulation_SRC})

target_link_libraries(BoatSimulation SDL2 SDL2main)

include_directories(${OPENGL_INCLUDE_DIRS})

include_directories(deps/includes)
include_directories(deps)
target_link_libraries(BoatSimulation shp)
target_link_libraries(BoatSimulation ${OPENGL_LIBRARIES})
if (GLEW_FOUND)
  target_link_libraries(BoatSimulation ${GLEW_LIBRARIES})
else()
  target_link_libraries(BoatSimulation glew)
endif()


