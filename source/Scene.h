#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "Drawable.h"
#include "Light.h"
#include "Camera.h"

class Scene
{
public:

	explicit Scene();

	void AddDrawable(std::shared_ptr<Drawable> drawable);

	std::shared_ptr<Camera> camera() const;

	void Draw();
	void Update(float deltaTime);

	std::shared_ptr<Light> TheLight;

private:
	std::vector<std::shared_ptr<Drawable>> m_Drawables;	
	std::shared_ptr<Camera>                m_Camera;
	float								   m_CurrentTime	= 0.0f;
	float								   m_Gamma			= 1.2f;
};