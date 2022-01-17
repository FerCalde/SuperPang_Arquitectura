#pragma once

#include "Components.h"
#include "stdafx.h"
#include "vector2d.h"
#include "sys.h"

class Bullet : public Entity
{
	Bullet() { AddComponent(new CMP_Transform); }


};