//--------------------------------------------------
// Robot Simulator
// userInterface.cpp
// Date: 2020-07-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "userInterface.h"
#include "simulator/helpers/log.h"

UserInterface::UserInterface(
		std::shared_ptr<Device> device, 
		std::shared_ptr<Window> window, 
		std::shared_ptr<SwapChain> swapChain, 
		Scene* scene):
	_device(device),
	_window(window),
	_scene(scene),
	_swapChain(swapChain),
	// Toggle variables
   	_enableRayTracing(nullptr), _splitRender(nullptr),
	_showPhysicsDebugger(false), _rootWidget(nullptr)
{
	//---------- Create gui objects ----------//
	_guiCommandPool = std::make_shared<CommandPool>(_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	_guiCommandBuffers = new CommandBuffers(_device, _guiCommandPool, _swapChain->getImageViews().size());

	_guiUniformBuffers.resize(_swapChain->getImageViews().size());
	for(size_t i = 0; i < _guiUniformBuffers.size(); i++) 
	{
		_guiUniformBuffers[i] = new GuiUniformBuffer(_device, sizeof(GuiUniformBufferObject));

		VkExtent2D extent = _swapChain->getExtent();
		GuiUniformBufferObject ubo;
		ubo.ratio = extent.width/extent.height;
		ubo.debug = false;
		_guiUniformBuffers[i]->setValue(ubo);
	}

	// Load font to texture
	_fontLoader = new guib::FontLoader(_device, _guiCommandPool, "assets/fonts/Ubuntu-Medium.ttf");

	_guiPipeline = new GuiPipeline(_device, _swapChain, _guiUniformBuffers, _fontLoader);
	_guiRender = new guib::GuiRender(_swapChain->getExtent(), _guiPipeline->getPipelineLayout(), _window->handle(), _fontLoader);
	createWidgetTree();
}

UserInterface::~UserInterface()
{
	delete _guiRender;
	_guiRender = nullptr;

	delete _guiPipeline;
	_guiPipeline = nullptr;

	for(auto& uniformBuffer : _guiUniformBuffers)
	{
		delete uniformBuffer;
		uniformBuffer = nullptr;
	}

	delete _guiCommandBuffers;
	_guiCommandBuffers = nullptr;

	//delete _guiCommandPool;
	//_guiCommandPool = nullptr;

	if(_rootWidget != nullptr)
	{
		delete _rootWidget;
		_rootWidget = nullptr;
	}
}

void UserInterface::createWidgetTree()
{
	_rootWidget = 
		new guib::Box(
		{
			.color = {.2,.2,.2,1},
			.size  = {300, 1, guib::UNIT_PIXEL, guib::UNIT_PERCENT},
			.child = new guib::Align({
				.hAlignment = guib::ALIGN_CENTER,
				.vAlignment = guib::ALIGN_END,
				.child = new guib::Box({
					.color = {.8, .5, .3},
					.size = {1, .8},
					.child = new guib::Align({
						.hAlignment = guib::ALIGN_CENTER,
						.vAlignment = guib::ALIGN_CENTER,
						.child = new guib::Visibility({
							.visible = true,
							.child = new guib::Column(
							{
								.hAlignment = guib::ALIGN_CENTER,
								.vAlignment = guib::ALIGN_END,
								.children = {
									new guib::Box({
										.color = {.1,.8,.3,1},
										.size = {.3,.9},
									}),
									new guib::Box({
										.color = {.8,.1,.3,1},
										.size = {.99,.1},
									})
								}
							})
						})
					})
				})
			})
		}); 

	_windows.push_back(
		new guib::Window(
		{
			.name = "Main",
			.closable = false,
			.minimizable = true,
			.movable = false,
			.offset = {0, 0},
			.size = {200, 1, guib::UNIT_PIXEL, guib::UNIT_PERCENT},
			.child = new guib::Align(
				{
					.hAlignment = guib::ALIGN_CENTER,
					.vAlignment = guib::ALIGN_START,
					.child = new guib::Button(
					{
						.onClick = [&](){
						},
						.size = {100, 20, guib::UNIT_PIXEL, guib::UNIT_PIXEL},
						.child = new guib::Text(
						{
							.color = {1,1,1,1},
							.text = "abcdefghi",
							.textSize = 16
						})
					})
				})
		})	
	);

	_windows.push_back(
		new guib::Window(
		{
			.name = "Draggable",
			.closable = true,
			.minimizable = true,
			.movable = true,
			.offset = {0.8, 0},
			.size = {.2, 200, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
			/*.child = new guib::Box(
				{
					.color = {1,0,1,1},
					.size  = {1,.7}
				})*/
		})	
	);

	//_guiRender->setRootWidget(_rootWidget);
	_guiRender->setWindowWidgets(_windows);
}

void UserInterface::checkResult(VkResult result)
{
	//if(result != VK_SUCCESS)
	//{
	//	Log::error("UserInterface", "Failed to setup Imggui info!");
	//	exit(1);
	//}
}


void UserInterface::render(int i)
{
	VkCommandBuffer commandBuffer = _guiCommandBuffers->begin(i);
	{
		_guiPipeline->beginRender(commandBuffer, i);
		_guiRender->render(commandBuffer);
		_guiPipeline->endRender(commandBuffer);
	}
	_guiCommandBuffers->end(i);
}

void UserInterface::draw()
{

}


//---------------------------------------------//
//-------------- DRAW FUNCTIONS ---------------//
//---------------------------------------------//

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
}

void UserInterface::onScroll(double xoffset, double yoffset)
{
	_guiRender->onScroll(xoffset, yoffset);
}