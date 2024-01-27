cmake_minimum_required(VERSION 3.5)
project(my_vulkan
  LANGUAGES CXX
)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON ) # for clangd compile_commands.json

find_package( Vulkan REQUIRED )
find_package( glfw3 REQUIRED )
add_executable( my_vulkan
  src/main.cpp
)
set_target_properties( my_vulkan
  PROPERTIES
  LANGUAGE CXX
  CXX_STANDARD 17
)
target_link_libraries( my_vulkan PUBLIC
  glfw
  Vulkan::Vulkan
)
