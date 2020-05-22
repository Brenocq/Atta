#include "fence.h"

Fence::Fence(Device* device, const bool signaled):
	_device(device)
{
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

	if(vkCreateFence(_device->handle(), &fenceInfo, nullptr, &_fence) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Fence] Failed to create fence!" << RESET << std::endl;
		exit(1);
	}
}

Fence::Fence(Fence&& other) noexcept :
	_device(other._device),
	_fence(other._fence)
{
	other._fence = nullptr;
}

Fence::~Fence()
{
	if(_fence != nullptr)
		vkDestroyFence(_device->handle(), _fence, nullptr);
}

void Fence::reset()
{
	if(vkResetFences(_device->handle(), 1, &_fence) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Fence] Failed to reset fence!" << RESET << std::endl;
		exit(1);
	}
}

void Fence::wait(const uint64_t timeout) const
{
	if(vkWaitForFences(_device->handle(), 1, &_fence, VK_TRUE, timeout) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Fence] Failed to wait for fence!" << RESET << std::endl;
		exit(1);
	}
}
