#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.hpp>

#include <iostream>
#include "base.hpp"

int main()
{
  Test app;
  app.run();
}
