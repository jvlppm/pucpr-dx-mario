#include "GameWindow.h"
#include "IGameLoop.h"
#include "HandleError.h"

using namespace mage;

#pragma region Setup and Shutdown
GameWindow::GameWindow() 
	: handle(0), direct3D(nullptr), device(nullptr), d3params(), game(nullptr), title(), cursorVisible(false), frequency(0)
{
}

GameWindow::~GameWindow()
{
	get().shutDown();
}

bool GameWindow::setup(
	HINSTANCE hInstance, int nShowCmd, 
	const TString& title, int x, int y, int w, int h, bool fullscreen,
	bool vsync, D3DDEVTYPE deviceType, DWORD verterProcessingMode)
{
	//Inicializamos as vari�veis de classe
	this->title = title;

	//Primeiro criamos e inicializamos nossa aplica��o Windows.
	//Note que fornecemos o hInstance da aplica��o e o nShowCmd,
	//recebidos como par�metro na WinMain.
	if (!InitWindowsApp(hInstance, nShowCmd, x, y, w, h, fullscreen))
		return false;

	//Depois, inicializamos o DirectX
	if (!InitDirectX(deviceType, verterProcessingMode, w, h, fullscreen, vsync))
		return false;

	return true;

}

void GameWindow::shutDown()
{
	if (device) 
	{ 
		device->Release();
		device = nullptr;
	}

	if (direct3D) {
		direct3D->Release();
		direct3D = nullptr;
	}

	if (!cursorVisible)
		ShowCursor(true);
}

#pragma endregion

#pragma region Properties

GameWindow& GameWindow::get()
{
	static GameWindow instance;
	return instance;
}

HWND GameWindow::getHandle() const
{
	return handle;
}

int GameWindow::getWidth() const
{
	return d3params.BackBufferWidth;
}

int GameWindow::getHeight() const
{
	return d3params.BackBufferHeight;
}

float GameWindow::getAspect() const
{
	return static_cast<float>(getWidth()) / static_cast<float>(getHeight());
}

bool GameWindow::isFullScreen() const
{
	return !d3params.Windowed;
}

bool GameWindow::isShowingCursor() const
{
	return cursorVisible;
}

void GameWindow::showCursor(bool show)
{	
	if (show == cursorVisible)
		return;

	ShowCursor(show);
}
#pragma endregion

#pragma region Windows and DirectX handling
namespace mage 
{
	const DWORD WS_NONRESIZABLE = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	const DWORD WS_FULLSCREEN = WS_EX_TOPMOST | WS_POPUP;

	LRESULT CALLBACK eventHandler(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
}

bool GameWindow::InitWindowsApp(HINSTANCE hInstance, int nShowCmd, int x, int y, int w, int h, bool fullscreen)
{	
	LARGE_INTEGER freq;
	if (!QueryPerformanceFrequency(&freq))
	{
		MessageBox(0, L"N�o h� suporte � temporiza��o de alta precis�o", 0, 0);
		return false;
	}

	frequency = static_cast<float>(freq.QuadPart);
	// A primeira tarefa ao se criar uma janela � descrever quais s�o 
	//suas caracter�sticas. Fazemos isso preenchendo uma estrutura do 
	//tipo WNDCLASSEX.
	WNDCLASSEX wc;

	//Zeramos os dados da struct.
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	//Configura��es da janela
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = eventHandler;
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);	
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = _T("GameWindow");

	//Ent�o, registramos essa descri��o de janela no Windows
	//para que possamos criar uma janela baseada nessa descri��o.	
	if(!RegisterClassEx(&wc))
	{
		MessageBox(0, _T("Falha ao registrar a classe da janela"), 0, 0);
		return false;
	}
	
	//Com a nossa classe da janela registrada, podemos criar uma janela
	//com a fun��o CreateWindow. Note que essa fun��o reotrna um HWND
	//para a janela criada, o qual salvaremos em MainWindowHandle.	
	handle = CreateWindowEx(NULL,
							_T("GameWindow"),								//Nome da classe 
							title.c_str(),									//T�tulo
							fullscreen ? WS_FULLSCREEN : WS_NONRESIZABLE,	//Estilo 
							fullscreen ? 0 : x,								//Posi��o x
							fullscreen ? 0 : y,								//Posi��o y
							w,												//Largura
							h,												//Altura
							NULL,											//Janela pai
							NULL,											//Barra de menu
							hInstance,										//Handle da aplica��o
							NULL);											//Usado com m�ltiplas janelas, NULL

	if(handle == 0)
    {
        MessageBox(0, _T("Falha ao criar janela"), 0, 0);
        return false;
    }

    // Finalmente, n�s precisamos atualizar a janela que rec�m criamos.
	// Observe que passaremos o MainWindowHandle para essas fun��es,
	// assim essas fun��es podem saber que janela deve ser mostrada e 
	// atualizada.
    ShowWindow(handle, nShowCmd);
	ShowCursor(false);
    UpdateWindow(handle);	
	return true;
}

bool GameWindow::InitDirectX(D3DDEVTYPE deviceType, DWORD verterProcessingMode, 
	int w, int h, bool fullscreen, bool vsync)
{
	//1. Obter um ponteiro para uma interface IDirect3D9
	direct3D = Direct3DCreate9(D3D_SDK_VERSION);

	//2. Verificar o suporte de hardware para o modo de 
	//display atual (modo janela). 
	if (fullscreen) 
	{
		HR(direct3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
			D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false));
	}
	else
	{
		D3DDISPLAYMODE mode; 
		HR(direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)); 
		HR(direct3D->CheckDeviceType(D3DADAPTER_DEFAULT, deviceType, 
			mode.Format, mode.Format, true));
	}

	//3. Verificar as capacidades do dispositivo (D3DCAPS9) para ver se a placa de
	//v�deo prim�ria suporte processamento de v�rtices e um dispositivo puro
	D3DCAPS9 caps; 
	HR(direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps)); 

	//Requisitamos o modo de processamento de v�rtice
	DWORD devBehaviorFlags = 0;
	if( caps.DevCaps &  D3DDEVCAPS_HWTRANSFORMANDLIGHT ) 
		devBehaviorFlags |= verterProcessingMode; 
	else //Caso n�o possa ser atendido pela placa, software
		devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; 

	// Testamos se existe a possibilidade de um Device Puro 
	if (caps.DevCaps & D3DDEVCAPS_PUREDEVICE && devBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) 
		 devBehaviorFlags |= D3DCREATE_PUREDEVICE;

	//Verificamos suporte ao Vertex e Pixel Shader
	if (caps.VertexShaderVersion < D3DVS_VERSION(2, 0))
	{
		MessageBox(0, _T("Sem suporte ao Vertex Shader 2.0 ou superior"), 0, 0);
	}

	if (caps.PixelShaderVersion < D3DVS_VERSION(2, 0))
	{
		MessageBox(0, _T("Sem suporte ao Pixel Shader 2.0 ou superior"), 0, 0);
	}

	//4. Inicializar uma inst�ncia da estrutura D3DPRESENT_PARAMETERS.	
    ZeroMemory(&d3params, sizeof(d3params));
	d3params.hDeviceWindow = handle; 
    d3params.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3params.BackBufferWidth = fullscreen ? w : 0;
    d3params.BackBufferHeight = fullscreen ? h : 0;
	d3params.BackBufferCount = 1;
    d3params.Windowed = !fullscreen;
	d3params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3params.PresentationInterval = vsync ? 
		D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE; 
    d3params.SwapEffect = D3DSWAPEFFECT_DISCARD;    
	d3params.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3params.MultiSampleQuality = 0; 	 
	d3params.EnableAutoDepthStencil = true; 
	d3params.AutoDepthStencilFormat = D3DFMT_D24S8; 
	d3params.Flags = 0; 

	//5. Construir o IDirect3DDevice9 baseado nessa estrutura.
	HR(direct3D->CreateDevice(
      D3DADAPTER_DEFAULT, // adaptador prim�rio
      deviceType,           // tipo de dispositivo
      handle,   // janela
      devBehaviorFlags, 
      &d3params, 
      &device));	
	return device != nullptr;
}

bool GameWindow::isDeviceLost()
{
	// Retorna verdadeiro se o Device est� perdido
	// Ou falso se ele est�, ou foi recuperado.

	// Obt�m o estado do dispositivo gr�fico
	HRESULT hr = device->TestCooperativeLevel();

    //Se ele se perdeu e n�o podemos recuperar, dormimos um pouco e esperamos o
	//pr�ximo loop
	if( hr == D3DERR_DEVICELOST )
	{
		Sleep(20);
		return true;
	}
	
	//Se houve um error de driver, n�o h� muito o que fazer a n�o ser
	//encerrar a aplica��o.
	if( hr == D3DERR_DRIVERINTERNALERROR )
	{
		MessageBox(0, _T("Erro de driver interno... saindo"), 0, 0);
		PostQuitMessage(0);
		return true;
	}
	// Se o Device est� perdido, mas pode ser recuperado, resetamos e o 
	//restauramos.
	if( hr == D3DERR_DEVICENOTRESET )
	{
		game->onLostDevice();			//Avisamos o jogo que iremos resetar.
		HR(device->Reset(&d3params));	//Resetamos o dispositivo
		game->onRestoreDevice(device);		//Avisamos o jogo que o dispositivo foi restaurado.
		// E agora podemos retornar false, j� que ele n�o estar� mais perdido.
		return false;
	}
      
	//Se n�o tem erros, ent�o n�o est� perdido.
	return false;
}

void GameWindow::run(IGameLoop* loop)
{
	LONGLONG before = QueryCounter();
	game = loop;

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	game->setup(device);
	while(true)
	{
		// Verificamos se h� mensagens na fila
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//Se houver, traduzimos e enviamos � fila
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			// Se for um WM_QUIT, ca�mos fora.
			if(msg.message == WM_QUIT)
				break;
			continue;
		}

		//C�digo do jogo
		if (!isDeviceLost()) 
		{
			LONGLONG now = QueryCounter();
			float secs = static_cast<float>((now - before) / frequency);
			before = QueryCounter();

			if (game->process(secs))
				game->paint(device);
			else
				DestroyWindow(handle);
		}
	}

	game->shutDown(device);
	delete game;
	game = nullptr;
}

void GameWindow::postEvent(const WindowsEvent& evt)
{
	if (game) game->processEvent(evt);
}

LRESULT CALLBACK mage::eventHandler(HWND windowHandle,
                         UINT msg,
                         WPARAM wParam,
                         LPARAM lParam)
{	
	// Vamos gerenciar algumas das mensagens da janela
	switch( msg )
	{
		// Caso recebamos um evento de destrui��o, enviamos uma 
		//mensagem de QUIT, que ir� encerrar o loop de mensagens.
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			WindowsEvent evt = {windowHandle, msg, lParam, wParam};
			GameWindow::get().postEvent(evt);
	}
	// Passamos para frente todas as mensagens que n�o manipulamos
	// acima para a fun��o padr�o da janela.
	return DefWindowProc(windowHandle, msg, wParam, lParam);
}

LONGLONG GameWindow::QueryCounter() const
{
	LARGE_INTEGER time;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&time);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return time.QuadPart;
}
#pragma endregion
