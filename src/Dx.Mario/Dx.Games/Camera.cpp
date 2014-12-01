#include "Camera.h"
#include "mage/GameWindow.h"

using namespace games;

Camera::Camera()
{
	auto window = &mage::GameWindow::get();
	viewport = { 0, 0, window->getWidth(), window->getHeight(), 0.0f, 1.0f };
}


Camera::~Camera()
{
}

void games::Camera::setPerspective(float fov, float nearDist, float farDist)
{
	D3DXMatrixPerspectiveFovLH(
		&projection,
		D3DXToRadian(fov),					 // Campo de visão horizontal
		mage::GameWindow::get().getAspect(), // aspect ratio
		nearDist,							 // Plano near
		farDist);							 // Plano far'
}

void games::Camera::lookAt(D3DXVECTOR3 target)
{
	auto position = worldPosition();
	D3DXMatrixLookAtLH(&view,
		&position, // posição da câmera
		&target,  // local para onde olha
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f)); // topo do mundo
}
