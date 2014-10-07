#pragma once

#include <d3d9.h>
#include "WindowsEvent.h"

namespace mage
{
	class IGameLoop 
	{
		public:
			virtual ~IGameLoop(){}

			//Inicializa o jogo. Roda imediatamente antes do game loop iniciar.
			virtual void setup(IDirect3DDevice9* device) = 0;

			//Ativado sempre que um evento do windows chega.
			virtual void processEvent(const WindowsEvent& msg) = 0;

			//Chamado todo game loop, para que o jogo atualize sua lógica.
			//O time representa o tempo transocorrido, em segundos, desse frame 
			//em relação ao anterior.
			//
			//Deve retornar falso para o jogo acabar.
			virtual bool process(float time) = 0;

			//Chamado todo game loop, para que o jogo desenhe a cena.
			virtual void paint(IDirect3DDevice9* device) = 0;

			//Chamado sempre que o Device é perdido.
			virtual void onLostDevice() = 0;

			//Chamado sempre que o Device é restaurado.
			virtual void onRestoreDevice(IDirect3DDevice9* device) = 0;

			//Finaliza o jogo. Roda após o game loop finalizar.
			virtual void shutDown(IDirect3DDevice9* device) = 0;

	};
}
