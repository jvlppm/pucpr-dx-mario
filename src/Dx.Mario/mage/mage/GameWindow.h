#pragma once

#include <windows.h>
#include <d3d9.h>
#include "TString.h"

namespace mage
{
	class IGameLoop;
	struct WindowsEvent;
	//Representa a janela principal do jogo.
	class GameWindow
	{
		private:		
			HWND handle;
			IDirect3D9* direct3D;
			IDirect3DDevice9* device;
			D3DPRESENT_PARAMETERS d3params;

			IGameLoop *game;
			
			TString title;
			bool cursorVisible;

			float frequency;
			
			GameWindow();
			GameWindow(const GameWindow& gw);
			GameWindow operator =(GameWindow& gw);				

			bool isDeviceLost();

			bool InitWindowsApp(HINSTANCE instanceHandle, int show, int x, int y, int w, int h, bool fullscreen);
			bool InitDirectX(D3DDEVTYPE deviceType, DWORD verterProcessingMode, 
				int w, int h, bool fullscreen, bool vsync);			
			LONGLONG QueryCounter() const;
		public:
			static GameWindow& get();

			bool setup(HINSTANCE hInstance, int nShowCmd, 
				const TString& title, int x, int y, int w, int h, bool fullscreen = false,
				bool vsync=false, D3DDEVTYPE deviceType=D3DDEVTYPE_HAL, DWORD verterProcessingMode=D3DCREATE_HARDWARE_VERTEXPROCESSING);

			void run(IGameLoop* loop);
			void shutDown();

			HWND getHandle() const;

			int getWidth() const;
			int getHeight() const;
			float getAspect() const;

			bool isFullScreen() const;

			bool isShowingCursor() const;
			void showCursor(bool show);

			void postEvent(const WindowsEvent& evt);

			~GameWindow();
	};
}