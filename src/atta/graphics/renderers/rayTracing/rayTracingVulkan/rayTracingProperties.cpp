//--------------------------------------------------
// Atta Ray Tracing Vulkan
// rayTracingProperties.h
// Date: 2021-02-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/rayTracingProperties.h>
#include <atta/helpers/log.h>

namespace atta::rt::vk
{
	RayTracingProperties::RayTracingProperties(std::shared_ptr<atta::vk::Device> device):
		_device(device)
	{
		_accelProps = {};
		_accelProps.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR;

		_pipelineProps = {};
		_pipelineProps.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
		_pipelineProps.pNext = &_accelProps;

		VkPhysicalDeviceProperties2 props = {};
		props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		props.pNext = &_pipelineProps;

		vkGetPhysicalDeviceProperties2(device->getPhysicalDevice()->handle(), &props);
	}
}
