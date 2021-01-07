//--------------------------------------------------
// Robot Simulator
// semaphore.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"

class Semaphore
{
	public:
	Semaphore(std::shared_ptr<Device> device);
	~Semaphore();

	VkSemaphore handle() const { return _semaphore; }
	std::shared_ptr<Device> getDevice() const { return _device; }

	private:
    VkSemaphore _semaphore;
	std::shared_ptr<Device> _device;
};

#endif// SEMAPHORE_H
