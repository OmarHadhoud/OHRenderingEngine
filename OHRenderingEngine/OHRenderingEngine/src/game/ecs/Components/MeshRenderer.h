#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include <glm/glm.hpp>
#include "game/ecs/Components/Component.h"
#include "renderer/Model.h"

enum class Transparency
{
	kNonTransparent,
	GL_TEXTURE_WRAP_SemiTransparent,
	GL_TEXTURE_WRAP_Transparent
};

struct MeshRenderer : Component
{
	std::unique_ptr<Model> m_Model;
	glm::vec3 m_BorderColor;
	bool m_BorderEnabled;
	float m_ExplodeStartTime;
	Transparency m_Transparency;
	bool m_IsSolid;
	static unsigned int m_Count;
	static int m_Indices[MAX_ENTITY_COUNT];
};

#endif // !MESH_RENDERER_H

