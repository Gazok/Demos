file(GLOB sources "*.cpp")

find_package(SFML 2.0 REQUIRED system window graphics)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${SFML_INCLUDE_DIRS})
include_directories(${OpenGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

add_executable("${PROJECT_NAME}" ${sources})

target_link_libraries("${PROJECT_NAME}" ${OPENGL_LIBRARIES} ${SFML_LIBRARIES} ${GLEW_LIBRARY}) 
