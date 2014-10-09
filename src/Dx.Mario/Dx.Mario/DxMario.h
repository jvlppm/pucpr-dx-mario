#pragma once

#include "mage/AbstractGameLoop.h"

#include <memory>

namespace mario {
	class DxMario : public mage::AbstractGameLoop
	{
	public:
		DxMario();
		virtual void setup(IDirect3DDevice9* device);
		virtual bool process(float time);
		virtual void paint(IDirect3DDevice9* device);
		virtual void shutDown(IDirect3DDevice9* device);
		virtual void processEvent(const mage::WindowsEvent& evt);
	private:
		struct private_implementation;
		std::unique_ptr<private_implementation> pImpl;
	};
}
