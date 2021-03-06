//--------------------------------------------------
// Robot Simulator
// renderPass.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_RENDER_PASS_H
#define ATTA_GRAPHICS_VULKAN_RENDER_PASS_H

#include <string>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/colorBuffer.h>
#include <atta/graphics/vulkan/depthBuffer.h>

namespace atta::vk
{
	class RenderPass
	{
		public:
			RenderPass(std::shared_ptr<Device> device, 
					std::shared_ptr<ColorBuffer> colorBuffer, 
					std::shared_ptr<DepthBuffer> depthBuffer);
			~RenderPass();

			//---------- Getters and Setters ----------//
			VkRenderPass handle() const { return _renderPass; }
			std::shared_ptr<ColorBuffer> getColorBuffer() const { return _colorBuffer; }
			std::shared_ptr<DepthBuffer> getDepthBuffer() const { return _depthBuffer; }

		private:
			VkRenderPass _renderPass;
			std::shared_ptr<Device> _device;
			std::shared_ptr<ColorBuffer> _colorBuffer;
			std::shared_ptr<DepthBuffer> _depthBuffer;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_RENDER_PASS_H
