#pragma once

#include <Windows.h>

namespace mage
{
	//Representa um evento de janela.
	struct WindowsEvent
	{
		HWND windowHandle;
		UINT type;		 
		LPARAM lParam;
		WPARAM wParam;
	};
}