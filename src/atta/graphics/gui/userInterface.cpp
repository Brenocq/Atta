//--------------------------------------------------
// Robot Simulator
// userInterface.cpp
// Date: 2020-07-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/userInterface.h>
#include <atta/graphics/gui/guiState.h>
#include <atta/helpers/log.h>
#include <atta/graphics/vulkan/imageMemoryBarrier.h>

namespace atta
{
	UserInterface::UserInterface(CreateInfo info):
		_device(info.device),
		_window(info.window),
		_scene(info.scene),
		_swapChain(info.swapChain),
		// Toggle variables
		_rootWidget(nullptr),
		_shouldClose(false)
	{
		//---------- Create gui objects ----------//
		_guiCommandPool = std::make_shared<vk::CommandPool>(_device, vk::CommandPool::DEVICE_QUEUE_FAMILY_GRAPHICS, vk::CommandPool::QUEUE_GUI, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		_guiCommandBuffers = std::make_shared<vk::CommandBuffers>(_device, _guiCommandPool, 1);

		// Uniform buffer
		_guiUniformBuffer = std::make_shared<GuiUniformBuffer>(_device, sizeof(GuiUniformBufferObject));

		GuiUniformBufferObject ubo;
		ubo.ratio = _window->getWidth()/(float)_window->getHeight();
		ubo.debug = false;
		_guiUniformBuffer->setValue(ubo);

		// Load font to texture
		_fontLoader = std::make_shared<guib::FontLoader>(_device, _guiCommandPool, "/usr/include/atta/assets/fonts/Roboto/Roboto-Light.ttf");

		createTextures();
		_guiPipeline = std::make_shared<GuiPipeline>(_device, _window, _swapChain, _guiUniformBuffer, _fontLoader);
		_guiRender = std::make_shared<guib::GuiRender>(
				(VkExtent2D){_window->getWidth(),_window->getHeight()}, 
				_guiPipeline->getPipelineLayout(), 
				_window->handle(), 
				_fontLoader);

		createWidgetTree();
	}

	UserInterface::~UserInterface()
	{
		// TODO release textures
	}

	void UserInterface::createWidgetTree()
	{
		_rootWidget = 
			new guib::Column(
			{
				.children = {
					new guib::TopBar(
					{
						.color = guib::state::palette["background"],
						.buttons = {
							new guib::Box((guib::BoxInfo){.color = guib::state::palette["background"], .size = {5, 1, guib::UNIT_PIXEL}}),
							new guib::MenuButton(
							(guib::MenuButtonInfo){
								.image = "logo16",
								.imageColor = guib::state::palette["main"],
								.color = guib::state::palette["background"],
								.hoverColor = guib::state::palette["light"],
								.clickColor = guib::state::palette["lightLight"],
								.children = {
									new guib::MenuItem((guib::MenuItemInfo){.text = "Welcome!"}),
									new guib::MenuItem((guib::MenuItemInfo){.text = "Atta main menu"}),
									new guib::MenuItem((guib::MenuItemInfo){.text = "Test"})
								}
							}),
							new guib::MenuButton(
							(guib::MenuButtonInfo){
								.text = "Rendering",
								.color = guib::state::palette["background"],
								.hoverColor = guib::state::palette["light"],
								.clickColor = guib::state::palette["lightLight"],
							}),
							new guib::MenuButton(
							(guib::MenuButtonInfo){
								.text = "Physics",
								.color = guib::state::palette["background"],
								.hoverColor = guib::state::palette["light"],
								.clickColor = guib::state::palette["lightLight"],
							}),
							new guib::MenuButton(
							(guib::MenuButtonInfo){
								.text = "Help",
								.color = guib::state::palette["background"],
								.hoverColor = guib::state::palette["light"],
								.clickColor = guib::state::palette["lightLight"],
							})
						}
					}),
					new guib::Box({
						.color = {0,0,0,0},//guib::state::palette["background"],
						.size  = {1, 1, guib::UNIT_PERCENT, guib::UNIT_PERCENT},
						.child = new guib::Row({
							.children = {
								new guib::Box({.color = guib::state::palette["background"], .size={3,1,guib::UNIT_PIXEL,guib::UNIT_PERCENT}}),
								// Left menu
								new guib::Box({
									.color = guib::state::palette["background"],
									.size={35,1,guib::UNIT_PIXEL,guib::UNIT_PERCENT},
									.child = new guib::Box({
										.color = guib::state::palette["lightDark"],
										.radius = {.3,.3,.3,.3},
										.size  = {35, 210, guib::UNIT_PIXEL, guib::UNIT_PIXEL},
										.child = new guib::Column({
											.children = {
												new guib::ButtonImage((guib::ButtonImageInfo){
													.image = "moveIcon",
													.imageColor = guib::state::palette["main"],
													.color = guib::state::palette["lightDark"],
													.hoverColor = guib::state::palette["light"],
													.clickColor = guib::state::palette["lightLight"],
													.size = {35, 35, guib::UNIT_PIXEL, guib::UNIT_PIXEL},
												}),
												new guib::ButtonImage((guib::ButtonImageInfo){
													.image = "rotateIcon",
													.imageColor = guib::state::palette["main"],
													.color = guib::state::palette["lightDark"],
													.hoverColor = guib::state::palette["light"],
													.clickColor = guib::state::palette["lightLight"],
													.size = {35, 35, guib::UNIT_PIXEL, guib::UNIT_PIXEL},
												}),
												new guib::ButtonImage((guib::ButtonImageInfo){
													.image = "scaleIcon",
													.imageColor = guib::state::palette["main"],
													.color = guib::state::palette["lightDark"],
													.hoverColor = guib::state::palette["light"],
													.clickColor = guib::state::palette["lightLight"],
													.size = {35, 35, guib::UNIT_PIXEL, guib::UNIT_PIXEL},
												}),
											}
										})
									})
								}),
								new guib::Box({.color = guib::state::palette["background"], .size={3,1,guib::UNIT_PIXEL,guib::UNIT_PERCENT}}),
								// Center viewports
								new guib::Box({
									.color = {0,0,0,0}, 
									.size = {guib::state::screenSize.width-250-35-4*3,1,guib::UNIT_PIXEL,guib::UNIT_PERCENT},
									.child = new guib::Column({
										// Viewport buttons
										.children = {
											new guib::Box({
												.color = guib::state::palette["background"],
												.size = {1,22, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
												.child = new guib::Row({
													.children = {
														new guib::ButtonText((guib::ButtonTextInfo){
															.text = "Main",
															.color = guib::state::palette["lightDark"],
															.hoverColor = guib::state::palette["light"],
															.clickColor = guib::state::palette["lightLight"],
														}),
														new guib::ButtonText((guib::ButtonTextInfo){
															.text = "Explore",
															.color = guib::state::palette["lightDark"],
															.hoverColor = guib::state::palette["light"],
															.clickColor = guib::state::palette["lightLight"],
														}),
													}
												}),
											}),
											// Viewport
											new guib::Box({
												.color = {0,0,0,0},
												.size = {1,guib::state::screenSize.height-26-22-3, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
											}),
											new guib::Box({.color = guib::state::palette["background"], .size={1,3,guib::UNIT_PERCENT,guib::UNIT_PIXEL}}),
										},
									}),
								}),
								new guib::Box({.color = guib::state::palette["background"], .size={3,1,guib::UNIT_PIXEL,guib::UNIT_PERCENT}}),
								// Left scene tree
								new guib::Box({
										.color = guib::state::palette["background"],
										.size={250,1,guib::UNIT_PIXEL,guib::UNIT_PERCENT},
										.child = new guib::Column({

										}),
								}),
								new guib::Box({.color = guib::state::palette["background"], .size={3,1,guib::UNIT_PIXEL,guib::UNIT_PERCENT}}),
							}
						})
					})
				}
			}); 

		std::vector<guib::Widget*> sceneTreeWidgets;
		for(auto object : _scene->getObjects())
		{
			sceneTreeWidgets.push_back(
				new guib::Button(
				{
					.onClick = [&](){
					},
					.size = {1, 20, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
					.child = new guib::Padding({
						.child = new guib::Text(
						{
							.color = {1,1,1,1},
							.text = object->getName(),
							.textSize = 10
						})
					})
				})
			);
		}

		//_windows.push_back(
		//	new guib::Window(
		//	{
		//		.name = "Scene",
		//		.closable = false,
		//		.minimizable = true,
		//		.movable = false,
		//		.offset = {0, 28, guib::UNIT_PIXEL, guib::UNIT_PIXEL},
		//		.size = {200, 1, guib::UNIT_PIXEL, guib::UNIT_PERCENT},
		//		.child = new guib::Column(
		//			{
		//				.hAlignment = guib::ALIGN_CENTER,
		//				.vAlignment = guib::ALIGN_START,
		//				.children = sceneTreeWidgets
		//			})
		//	})	
		//);

		//_windows.push_back(
		//	new guib::Window(
		//	{
		//		.name = "Draggable",
		//		.closable = true,
		//		.minimizable = true,
		//		.movable = true,
		//		.offset = {0.8, 28, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
		//		.size = {.2, 200, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
		//		/*.child = new guib::Box(
		//			{
		//				.color = {1,0,1,1},
		//				.size  = {1,.7}
		//			})*/
		//	})	
		//);

		_guiRender->setRootWidget(_rootWidget);
		_guiRender->setWindowWidgets(_windows);
	}

	void UserInterface::createTextures()
	{
		std::vector<std::pair<std::string, std::string>> icons = {
			{"logo16", "/usr/include/atta/assets/icons/logo16.png"},
			{"logo50", "/usr/include/atta/assets/icons/logo50.png"},
			{"moveIcon", "/usr/include/atta/assets/icons/move100.png"},
			{"rotateIcon", "/usr/include/atta/assets/icons/rotate100.png"},
			{"scaleIcon", "/usr/include/atta/assets/icons/scale100.png"},
		};

		for(auto icon : icons)
		{
			guib::state::textureIndex[icon.first] = guib::state::textures.size();
			guib::state::textures.push_back(new atta::vk::Texture(_device, _guiCommandPool, icon.second));
		}
	}

	void UserInterface::render(int imageIndex)
	{
		VkCommandBuffer commandBuffer = _guiCommandBuffers->begin(0);
		{
			_guiPipeline->beginRender(commandBuffer, imageIndex);
			_guiRender->render(commandBuffer);
			_guiPipeline->endRender(commandBuffer);
		}
		_guiCommandBuffers->end(0);
	}

	void UserInterface::render(VkCommandBuffer commandBuffer, int imageIndex)
	{
		static bool firstResize = true;
		if(firstResize)
		{
			_renderers.begin()->second->resize(
					guib::state::screenSize.width-250-35-4*3,
					guib::state::screenSize.height-26-22-3);
			firstResize = false;
		}

		_renderers.begin()->second->render(commandBuffer);

		copyRendererImage(commandBuffer, imageIndex, _renderers.begin()->first, {35+2*3,26+22});
		_guiPipeline->beginRender(commandBuffer, imageIndex);
		_guiRender->render(commandBuffer);
		_guiPipeline->endRender(commandBuffer);
	}

	void UserInterface::copyRendererImage(VkCommandBuffer commandBuffer, int imageIndex, std::string rendererName, VkOffset2D dstOffset)
	{
		VkImageSubresourceRange subresourceRange;
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;
		VkImage dstImage = _swapChain->getImages()[imageIndex];
		VkImage srcImage = _renderers[rendererName]->getImage()->handle();

		vk::ImageMemoryBarrier::insert(commandBuffer, dstImage, subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
			0, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		vk::ImageMemoryBarrier::insert(commandBuffer, srcImage, subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
			0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

		// Copy src image to dst image
		VkExtent2D srcExtent = _renderers[rendererName]->getImage()->getExtent();
		VkImageCopy region;
		region.srcOffset = {0, 0, 0};
		//region.srcOffsets = { static_cast<int32_t>(srcExtent.width), static_cast<int32_t>(srcExtent.height), 1 };
		region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.srcSubresource.mipLevel = 0;
		region.srcSubresource.baseArrayLayer = 0;
		region.srcSubresource.layerCount = 1;
		region.dstOffset = {dstOffset.x, dstOffset.y, 0};
		//region.dstOffsets[1] = { dstExtent.width, dstExtent.height, 1 };
		region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.dstSubresource.mipLevel = 0;
		region.dstSubresource.baseArrayLayer = 0;
		region.dstSubresource.layerCount = 1;
		region.extent = { srcExtent.width, srcExtent.height, 1 };

		vkCmdCopyImage(commandBuffer,
			srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, 
			&region);

		vk::ImageMemoryBarrier::insert(commandBuffer, dstImage, subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
			0, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	}

	//---------------------------------------------//
	//---------------- CALLBACKS ------------------//
	//---------------------------------------------//
	void UserInterface::onWindowResized(int width, int height)
	{
		_guiRender ->onWindowResized(width, height);
	}

	// Window callbacks
	void UserInterface::onKey(int key, int scancode, int action, int mods)
	{
		_guiRender->onKey(key, scancode, action, mods);
	}

	void UserInterface::onCursorPosition(double xpos, double ypos)
	{
		_guiRender->onCursorPosition(xpos, ypos);
	}

	void UserInterface::onMouseButton(int button, int action, int mods)
	{
		_guiRender->onMouseButton(button, action, mods);

		// Check if window closed by UI
		if(guib::state::shouldClose)
		{
			// This signal is propagated to userInterface->workerGui->threadManager
			// to finish execution
			_shouldClose = true;
		}
	}

	void UserInterface::onScroll(double xoffset, double yoffset)
	{
		_guiRender->onScroll(xoffset, yoffset);
	}
}
