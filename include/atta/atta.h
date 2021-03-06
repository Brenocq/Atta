//--------------------------------------------------
// Atta Robot Simulator
// atta.h
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_ATTA_H
#define ATTA_ATTA_H

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <atta/parallel/threadManager.h>
#include <atta/objects/object.h>
#include <atta/core/robot.h>
#include <atta/core/scene.h>
#include <atta/core/common.h>

namespace atta
{
	class Project;
	class Atta
	{
		public:
			struct CreateInfo
			{
				// Graphics
				DimMode dimensionMode = DIM_MODE_2D;
				GuiRenderer guiRenderer = GUI_RENDERER_RAST;
				PhysicsMode physicsMode = PHY_MODE_DISABLED;
				RobotProcessing robotProcessing = ROBOT_PROCESSING_SEQUENTIAL;
				bool createWindow = true;
				std::vector<std::shared_ptr<Object>> objects = {};
				std::vector<std::shared_ptr<Robot>> robots = {};

				std::function<void(void)> runAfterRobots;
				std::function<void(WorkerGui*)> runBeforeWorkerGuiRender;
				std::function<void(int key, int action)> handleKeyboard;
				std::function<void(double xpos, double ypos)> handleMousePosition;
				std::function<void(int button, int action)> handleMouseButton;
				std::function<void(double xoffset, double yoffset)> handleMouseScroll;
			};

			Atta(CreateInfo info);
			~Atta();
			static Atta createFromProject(Project project);

			void run();

		private:
			ThreadManager::GeneralConfig populateTMGeneralConfig();
			ThreadManager::PhysicsStage populateTMPhysicsStage();
			ThreadManager::SensorStage populateTMSensorStage();
			ThreadManager::RobotStage populateTMRobotStage();
			ThreadManager::UiConfig populateTMUiConfig();

			CreateInfo _info;
			std::shared_ptr<Scene> _scene;
			std::shared_ptr<ThreadManager> _threadManager;
	};
}

#endif// ATTA_ATTA_H
