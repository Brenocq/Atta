//--------------------------------------------------
// Atta Ray Tracing Vulkan
// uniformBuffer.h
// Date: 2021-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RT_VK_UNIFORM_BUFFER_H
#define ATTA_RT_VK_UNIFORM_BUFFER_H

#include <iostream>
#include <string.h>
#include <vector>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/buffer.h>
#include <atta/math/math.h>

namespace atta::rt::vk
{
	struct UniformBufferObject
	{
		alignas(16) mat4 viewMat;
		alignas(16) mat4 projMat;
		alignas(16) mat4 viewMatInverse;
		alignas(16) mat4 projMatInverse;

		alignas(4) unsigned samplesPerPixel;
		alignas(4) unsigned nAccSamples;
		alignas(4) unsigned maxDepth;

		alignas(4) unsigned nLights;

		alignas(4) unsigned seed;
	};


	class UniformBuffer : public atta::vk::Buffer
	{
		public:
			UniformBuffer(std::shared_ptr<atta::vk::Device> device);
			~UniformBuffer();

			void setValue(UniformBufferObject ubo);
			UniformBufferObject getValue() const { return _ubo; }
			
		private:
			UniformBufferObject _ubo;
	};
}

#endif// ATTA_RT_VK_UNIFORM_BUFFER_H
