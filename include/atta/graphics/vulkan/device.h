//--------------------------------------------------
// Robot Simulator
// device.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_DEVICE_H
#define ATTA_GRAPHICS_VULKAN_DEVICE_H

#include <atta/graphics/vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <memory>
#include <atta/graphics/vulkan/physicalDevice.h>

namespace atta::vk
{
	class Device
	{
		public:
			Device(std::shared_ptr<PhysicalDevice> physicalDevice);
			~Device();

			VkDevice handle() const { return _device; }
			std::shared_ptr<PhysicalDevice> getPhysicalDevice() const { return _physicalDevice; }
			VkQueue getGraphicsQueue() const { return _graphicsQueue; }
			VkQueue getTransferQueue() const { return _transferQueue; }
			VkQueue getComputeQueue() const { return _computeQueue; }
			VkQueue getPresentQueueGUI() const { return _presentQueueGUI; }
			VkQueue getGraphicsQueueGUI() const { return _graphicsQueueGUI; }
			VkQueue getTransferQueueGUI() const { return _transferQueueGUI; }

			VkSampleCountFlagBits getMsaaSamples() const { return _msaaSamples; }

		private:
			VkSampleCountFlagBits getMaxUsableSampleCount();

			VkDevice _device;
			VkQueue _graphicsQueue;
			VkQueue _transferQueue;
			VkQueue _computeQueue;
			VkQueue _presentQueueGUI;
			VkQueue _graphicsQueueGUI;
			VkQueue _transferQueueGUI;

			std::shared_ptr<PhysicalDevice> _physicalDevice;
			VkSampleCountFlagBits _msaaSamples;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_DEVICE_H
