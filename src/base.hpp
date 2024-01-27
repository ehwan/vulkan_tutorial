#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>
#include <cstring>

template < typename CRTP >
struct GLFWApplication
{
  int width = 800;
  int height = 800;
  std::string title = "Hello Vulkan and GLFW";
  GLFWwindow *window;

  CRTP& as_base() &
  {
    return *static_cast<CRTP*>(this);
  }
  CRTP const& as_base() const&
  {
    return *static_cast<CRTP const*>(this);
  }
  GLFWApplication()
  {
    glfwInit();
    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
    window = glfwCreateWindow( width, height, title.c_str(), nullptr, nullptr );
  }
  ~GLFWApplication()
  {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void run()
  {
    while( !glfwWindowShouldClose(window) )
    {
      glfwPollEvents();
    }
  }
};


template < typename CRTP >
struct VulkanApplication 
  : GLFWApplication<CRTP>
{
  vk::Instance instance;
  vk::PhysicalDevice physical_device;

  void init_instance()
  {
    // init instance
    vk::ApplicationInfo appInfo;
    appInfo.sType = vk::StructureType::eApplicationInfo;
    appInfo.pApplicationName = "Hello Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1,2,0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1,2,0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    vk::InstanceCreateInfo createInfo;
    createInfo.sType = vk::StructureType::eInstanceCreateInfo;
    createInfo.pApplicationInfo = &appInfo;


    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );

    std::cout << "GLFW Extenstions :\n";
    std::vector<const char*> extensions;
    for( int i=0; i<glfwExtensionCount; ++i )
    {
      extensions.push_back( glfwExtensions[i] );
      std::cout << glfwExtensions[i] << std::endl;
    }

    // for mac OSX; add below flag
    createInfo.flags |= vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
    extensions.push_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );

    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    createInfo.enabledLayerCount = 0;


    // validation layer
    std::vector<const char*> use_layer =
    {
      "VK_LAYER_KHRONOS_validation"
    };


    auto layers = vk::enumerateInstanceLayerProperties();
    std::vector<const char*> layer_names( layers.size() );
    std::cout << "Layers found :\n";
    for( int i=0; i<layers.size(); ++i )
    {
      layer_names[i] = layers[i].layerName;
      std::cout << "Validation Layer " << i << " :\n";
      std::cout << layers[i].layerName << "\n";
      std::cout << layers[i].description << "\n";
      std::cout << layers[i].specVersion << "\n";
    }
    use_layer.erase(
    std::remove_if( use_layer.begin(), use_layer.end(),
        [&]( const char* l )
        {
          bool found = false;
          for( int i=0; i<layers.size(); ++i )
          {
            if( std::strcmp( l, layer_names[i] ) ){ found = true; break; }
          }
          if( found == false )
          {
            std::cout << "Layer : " << l << " not found;\n";
          }
          return found==false;
        } ),
      use_layer.end() );
    std::cout << "Use Layers :\n";
    for( int i=0; i<use_layer.size(); ++i )
    {
      std::cout << use_layer[i] << "\n";
    }
    createInfo.enabledLayerCount = use_layer.size();
    createInfo.ppEnabledLayerNames = use_layer.data();

    instance = vk::createInstance(createInfo);
  }
  void init_device()
  {
    auto devices = instance.enumeratePhysicalDevices();
    std::cout << devices.size() << " devices found :\n";
    for( auto& d : devices )
    {
      auto ps = d.getProperties();
      std::cout << "ID : " << ps.deviceID << "\n";
      std::cout << "Name : " << ps.deviceName << "\n";
      std::cout << "API : " << ps.apiVersion << "\n";
      std::cout << "DriverVersion : " << ps.driverVersion << "\n";
    }
    physical_device = std::move(devices[0]);
  }
  void init_logical_device()
  {
  }
  VulkanApplication()
  {
    init_instance();
    init_device();
  }
};

struct Test
  : VulkanApplication<Test>
{
};
