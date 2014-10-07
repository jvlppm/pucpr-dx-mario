#pragma once

#include <d3dx9.h>
#include <vector>

#include "HandleError.h"
#include "TString.h"

namespace mage
{
	class Effect
	{
		private:
			ID3DXEffect* shader;
			LPDIRECT3DDEVICE9 device;
			TString file;

			Effect(const Effect& other);
			Effect operator =(const Effect& other);

		public:
			Effect(const TString& filename);
			std::string compile(LPDIRECT3DDEVICE9 device);

			Effect& setTechnique(const std::string& name);
			Effect& setFloat(const std::string& name, const float value);			
			
			Effect& setMatrix(const std::string& name, const D3DXMATRIX& matrix);
			Effect& setVector(const std::string& name, const D3DXVECTOR4& vector);
			Effect& setVector(const std::string& name, const D3DXVECTOR3& vector);
			Effect& setVector(const std::string& name, const D3DXVECTOR2& vector);
			Effect& setTexture(const std::string& name, const LPDIRECT3DBASETEXTURE9& texture);
			Effect& setColor(const std::string& name, const D3DXCOLOR& value);
			Effect& commit();

			template<typename Function>
			Effect& execute(Function drawFunction)
			{
				UINT passes = 0;				
				HR(shader->Begin(&passes, 0));
				for (UINT i = 0; i < passes; ++i) 
				{
					HR(shader->BeginPass(i));
						drawFunction(device);
					HR(shader->EndPass());
				}
				HR(shader->End());
				return *this;
			}

			~Effect();
	};	
}
