#pragma once
#include "ISingleton.h"
#include <Windows.h>

#define KEYBOARD_H 'H' //FIRE LEFT
#define KEYBOARD_K 'K' //FIRE RIGHT
#define KEYBOARD_A 'A' //MOVE LEFT
#define KEYBOARD_D 'D' //MOVE RIGHT

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


};