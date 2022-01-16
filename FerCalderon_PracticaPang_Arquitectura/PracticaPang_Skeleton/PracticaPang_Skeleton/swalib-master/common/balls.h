#pragma once

#include "Components.h"
#include "stdafx.h"
#include "vector2d.h"
#include "sys.h"

class Ball : public Entity
{

public:
	Ball()
	{
		AddComponent(new CMP_Transform);
		AddComponent(new CMP_Collider);
		AddComponent(new CMP_Render);
	}

	~Ball() {};

};
