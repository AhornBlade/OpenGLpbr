#pragma once

#include <memory>

#include "Geometry.h"
#include "MaterialParams.h"
#include "Transform.h"

class Drawable
{
public:	
	explicit Drawable(std::shared_ptr<Geometry> mesh);	
	explicit Drawable();

	virtual void draw() = 0;	
	virtual glm::mat4 modelMatrix() const;

	Transform				  transform;	
	MaterialParams			  materialParams;
	std::shared_ptr<Geometry> geometry;
};
