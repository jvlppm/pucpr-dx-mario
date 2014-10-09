#include "mage/GameWindow.h"
#include "DxMario.h"

using namespace mage;
using namespace mario;

// Equivalente windows do "Main"
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR      pCmdLine,
	int       nShowCmd)
{
	//Obtemos o subsistema da janela
	mage::GameWindow& window = mage::GameWindow::get();
	//Tentamos inicializa-lo.
	if (window.setup(hInstance, nShowCmd, _T("Dx.Mario"),
		100, 100, 800, 600))
	{
		//Se deu certo, roda o game loop
		window.run(new DxMario());
	}

	//Finalizamos o sistema.
	window.shutDown();
}