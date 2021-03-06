//--------------------------------------------------
// Robot Simulator
// guiPipeline.h
// Date: 2020-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_PIPELINE_H
#define GUI_PIPELINE_H

#include <vector>
#include <string>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/imageView.h>
#include <atta/graphics/vulkan/depthBuffer.h>
#include <atta/graphics/vulkan/colorBuffer.h>
#include <atta/graphics/vulkan/shaderModule.h>
#include <atta/graphics/vulkan/descriptorSetManager.h>
#include <atta/graphics/gui/guiRenderPass.h>
#include <atta/graphics/gui/guiFrameBuffer.h>
#include <atta/graphics/gui/guiPipelineLayout.h>
#include <atta/graphics/gui/guiUniformBuffer.h>
#include <atta/graphics/gui/widgets/widgets.h>
#include <atta/graphics/gui/font/fontLoader.h>

namespace atta
{
	class GuiPipeline
	{
		public:
			GuiPipeline(std::shared_ptr<vk::Device> device, 
					std::shared_ptr<Window> window,
					std::shared_ptr<vk::SwapChain> swapChain,
					std::shared_ptr<GuiUniformBuffer> uniformBuffer,
					std::shared_ptr<guib::FontLoader> _fontLoader);
			~GuiPipeline();

			void beginRender(VkCommandBuffer commandBuffer, int imageIndex);
			void endRender(VkCommandBuffer commandBuffer);

			//---------- Getters and Setters ----------//
			VkPipeline handle() const { return _pipeline; }
			std::shared_ptr<GuiPipelineLayout> getPipelineLayout() const { return _pipelineLayout; }
			std::shared_ptr<vk::DescriptorSetManager> getDescriptorSetManager() const { return _descriptorSetManager; }
			std::shared_ptr<vk::DescriptorSets> getDescriptorSets() const { return _descriptorSetManager->getDescriptorSets(); }
			std::vector<std::shared_ptr<vk::ImageView>> getImageViews() const { return _imageViews; }
			std::vector<std::shared_ptr<GuiFrameBuffer>> getFrameBuffers() const { return _frameBuffers; }
			std::shared_ptr<GuiRenderPass> getRenderPass() const { return _renderPass; }
			std::vector<std::shared_ptr<vk::ColorBuffer>> getColorBuffers() const { return _colorBuffers; }

		private:
			VkPipeline _pipeline;
			std::shared_ptr<vk::Device> _device;
			std::shared_ptr<vk::SwapChain> _swapChain;
			std::vector<std::shared_ptr<vk::DepthBuffer>> _depthBuffers;
			std::vector<std::shared_ptr<vk::ColorBuffer>> _colorBuffers;
			std::vector<std::shared_ptr<GuiFrameBuffer>> _frameBuffers;
			std::shared_ptr<GuiRenderPass> _renderPass;

			std::vector<std::shared_ptr<vk::ImageView>> _imageViews;
			VkFormat _imageFormat;
			VkExtent2D _imageExtent;

			std::shared_ptr<vk::ShaderModule> _vertShaderModule;
			std::shared_ptr<vk::ShaderModule> _fragShaderModule;
			std::shared_ptr<vk::DescriptorSetManager> _descriptorSetManager;
			std::shared_ptr<GuiPipelineLayout> _pipelineLayout;

			//---------- GiuB widget handler ----------//
			std::vector<guib::ClickDetectorArea> _clickableAreas;
	};
}
#endif// GUI_PIPELINE_H
