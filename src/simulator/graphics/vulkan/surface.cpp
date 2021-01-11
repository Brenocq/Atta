//--------------------------------------------------
// Robot Simulator
// surface.cpp
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "surface.h"
#include "simulator/helpers/log.h"

namespace atta::vk
{
	Surface::Surface(std::shared_ptr<Instance> instance, std::shared_ptr<Window> window):
		_instance(instance), _window(window)
	{
		if(glfwCreateWindowSurface(_instance->handle(), _window->handle(), nullptr, &_surface) != VK_SUCCESS) 
		{
			Log::error("Surface", "Failed to create window surface!");
			exit(1);
		}	
	}

	Surface::~Surface()
	{
		if(_surface != nullptr)
		{
			vkDestroySurfaceKHR(_instance->handle(), _surface, nullptr);
			_surface = nullptr;
		}
	}
}