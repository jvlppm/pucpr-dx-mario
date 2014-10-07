#include <DxErr.h>
#include "TString.h"

#pragma once
// Adicionamos informa��o extra de debug, caso estejamos usando as builds de Debug do DirectX.
// Tamb�m permite que os objetos fucionem bem na janela de debug, durante o passo-a-passo, mas 
// torna a execu��o direta ligeiramente mais lenta.
#if defined(DEBUG) | defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

// Trata o retorno das fun��es do DirectX, logando seus erros caso estejamos em modo debug.
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                       \
	{                                                   \
		HRESULT hr = x;                                 \
		if(FAILED(hr))                                  \
			DXTrace(__FILE__, __LINE__, hr, _T(#x), FALSE); \
	}
	#endif
#else
	#ifndef HR
	#define HR(x) x;
	#endif
#endif