//--------------------------------------------------
// Atta Graphics
// renderer.cpp
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "renderer.h"

namespace atta
{
	Renderer::Renderer(CreateInfo info):
		_vkCore(info.vkCore), _extent({info.width, info.height})
	{
		createOutputImage();
	}

	void Renderer::createOutputImage()
	{
		_image = std::make_shared<vk::Image>(
			_vkCore->getDevice(), 
			_extent.width, _extent.height, 
			VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_STORAGE_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		_imageView = std::make_shared<vk::ImageView>(_vkCore->getDevice(), _image->handle(), VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_ASPECT_COLOR_BIT);
	}
}
