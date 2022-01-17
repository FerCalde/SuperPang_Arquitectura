#pragma once
#include "ISingleton.h"
#include <Windows.h>

#define KEYBOARD_H 'H' //FIRE LEFT
#define KEYBOARD_K 'K' //FIRE RIGHT
#define KEYBOARD_SPACE ' ' //FIRE SPACE 
#define KEYBOARD_A 'A' //MOVE LEFT
#define KEYBOARD_D 'D' //MOVE RIGHT

#define MOVE_LEFT KEYBOARD_A
#define MOVE_RIGHT KEYBOARD_D
#define FIRE_KEY KEYBOARD_K 

typedef unsigned char Key;


class CInputManager :public ISingleton< CInputManager>
{
private:
	~CInputManager(){}

public :

	bool IsKeyPressed(Key _key)
	{
		return GetAsyncKeyState(_key) & 0x8000;
	}

	bool PressedMoveRight() 
	{
		return IsKeyPressed(MOVE_RIGHT);
	}
	bool PressedMoveLeft()
	{
		return IsKeyPressed(MOVE_LEFT);
	}
	bool PressedFire()
	{
		return IsKeyPressed(FIRE_KEY);
	}
};