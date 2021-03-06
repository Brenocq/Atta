//--------------------------------------------------
// Robot Simulator
// importedObject.cpp
// Date: 2020-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/objects/basics/importedObject.h>

namespace atta
{
	ImportedObject::ImportedObject(CreateInfo info):
		Object({info.name, info.position, info.rotation, info.scale, info.mass, std::move(info.children)})
	{
		Object::setType("ImportedObject");

		//----- Model -----//
		Model::CreateInfo modelInfo = {
			.meshName = info.fileName,
			.material = info.material,
			.materials = info.materials
		};
		_model = std::make_shared<Model>(modelInfo);

		//----- Physics -----//
		_bodyPhysics->addShape(std::make_shared<phy::BoxShape>(vec3(), quat(), vec3(1,1,1)));
	}

	ImportedObject::~ImportedObject()
	{
	}
}
