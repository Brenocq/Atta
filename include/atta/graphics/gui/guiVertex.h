//--------------------------------------------------
// Robot Simulator
// guiVertex.h
// Date: 2020-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_VERTEX_H
#define GUI_VERTEX_H

#include <atta/graphics/vulkan/vulkan.h>
#include <atta/math/vector.h>
#include <array>

struct GuiVertex 
{
	atta::vec3 pos;
	atta::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription() 
	{
        VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(GuiVertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() 
	{
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(GuiVertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(GuiVertex, color);

        return attributeDescriptions;
    }

	bool operator==(const GuiVertex& other) const {
		return pos == other.pos && color == other.color;
	}
};

namespace std {
    template<> struct hash<GuiVertex> {
        size_t operator()(GuiVertex const& vertex) const {
            return ((hash<atta::vec3>()(vertex.pos) ^
                   (hash<atta::vec3>()(vertex.color) << 1)) >> 1);
        }
    };
}
#endif// GUI_VERTEX_H
