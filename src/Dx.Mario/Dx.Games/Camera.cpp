#include "Camera.h"
#include "mage/GameWindow.h"
#include "CameraVertex.h"
#include "mage/HandleError.h"
#include "Resources.h"

using namespace games;
using namespace mage;
using namespace std;

struct Camera::private_implementation {
	private_implementation() {
		cameraRenderTarget = 0;
	}

	void setViewport(int left, int top, int width, int height) {
		viewport = { left, top, width, height, 0.0f, 1.0f };
		renderTexture = nullptr;
		cameraRenderTarget = nullptr;
	}

	float getAspect() {
		return viewport.Width / (float)viewport.Height;
	}

	void begin(IDirect3DDevice9* device, shared_ptr<Scene> scene) {
		HR(device->SetRenderTarget(0, getRenderTarget(device).get()));
		device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, scene->clearColor, 1.0f, 0);
		device->BeginScene();
	}

	void end(IDirect3DDevice9* device) {
		device->EndScene();
	}

	void render(IDirect3DDevice9* device) {
		device->SetViewport(&viewport);

		effect->setTechnique("Default");
		effect->setTexture("gTexture", renderTexture.get());
		effect->commit();
		effect->execute([this](IDirect3DDevice9* device)
		{
			HR(device->SetVertexDeclaration(CameraVertex::getDeclaration(device).get()));
			HR(device->SetStreamSource(0,						//Número do stream
				getVertexBuffer(device).get(),									//Dados
				0,												//Offset
				sizeof(CameraVertex)));								//Tamanho do dado
			HR(device->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//Primitiva
				0,												//Vértice inicial
				2));
		});
	}

private:
	static shared_ptr<IDirect3DVertexBuffer9> getVertexBuffer(IDirect3DDevice9* device) {
		if (vertexBuffer)
			return vertexBuffer;

		vector<CameraVertex> vertices =
		{
			{ -1, +1, 0, 0 },
			{ +1, +1, 1, 0 },
			{ -1, -1, 0, 1 },
			{ +1, -1, 1, 1 },
		};

		vertexBuffer = CameraVertex::createVertexBuffer(device, vertices);
		return vertexBuffer;
	}

	shared_ptr<IDirect3DSurface9> getRenderTarget(IDirect3DDevice9* device) {
		if (!effect)
			effect = Resources::getEffect(device, "camera.fx");

		if (!renderTexture) {
			IDirect3DTexture9* cameraOutputTexture;
			if (device->CreateTexture(viewport.Width, viewport.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &cameraOutputTexture, 0) != S_OK)
				throw runtime_error("Camera render texture could not be created");
			renderTexture = shared_ptr<IDirect3DTexture9>(cameraOutputTexture, [](IDirect3DTexture9* texture) { texture->Release(); });
		}

		if (!cameraRenderTarget) {
			IDirect3DSurface9* cameraOutputTextureSurface;
			if (renderTexture->GetSurfaceLevel(0, &cameraOutputTextureSurface) != S_OK)
				throw runtime_error("Could not get camera output texture surface");
			cameraRenderTarget = shared_ptr<IDirect3DSurface9>(cameraOutputTextureSurface, [](IDirect3DSurface9* surface) { surface->Release(); });
		}

		return cameraRenderTarget;
	}

	D3DVIEWPORT9 viewport;
	shared_ptr<IDirect3DTexture9> renderTexture;
	shared_ptr<IDirect3DSurface9> cameraRenderTarget;
	static shared_ptr<IDirect3DVertexBuffer9> vertexBuffer;
	shared_ptr<Effect> effect;
};

shared_ptr<IDirect3DVertexBuffer9> Camera::private_implementation::vertexBuffer = nullptr;

Camera::Camera() : pImpl(new Camera::private_implementation())
{
	auto window = &GameWindow::get();
	pImpl->setViewport(0, 0, window->getWidth(), window->getHeight());
}

Camera::~Camera()
{
}

shared_ptr<Camera> Camera::setPerspective(float fov, float nearDist, float farDist)
{
	D3DXMatrixPerspectiveFovLH(
		&projection,
		D3DXToRadian(fov),					 // Campo de visão horizontal
		pImpl->getAspect(),                  // aspect ratio
		nearDist,							 // Plano near
		farDist);							 // Plano far'
	return shared_from_this();
}

shared_ptr<Camera> Camera::lookAt(D3DXVECTOR3 target)
{
	auto position = worldPosition();
	D3DXMatrixLookAtLH(&view,
		&position, // posição da câmera
		&target,  // local para onde olha
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f)); // topo do mundo
	return shared_from_this();
}

void Camera::begin(IDirect3DDevice9* device, shared_ptr<Scene> scene)
{
	pImpl->begin(device, scene);
}

void Camera::end(IDirect3DDevice9* device) {
	pImpl->end(device);
}

void Camera::render(IDirect3DDevice9* device) {
	pImpl->render(device);
}
