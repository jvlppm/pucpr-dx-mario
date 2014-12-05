#pragma once
#include "BaseObject.h"

namespace games {

	template <typename Type>
	class GameObject : public BaseObject
	{
	public:
		GameObject() {
		}
		~GameObject() {
		}

		template <typename T>
		std::shared_ptr<Type> transform(T func) {
			BaseObject::transform(func);
			return shared_from_this();
		}

		std::shared_ptr<Type> translate(float x, float y, float z) {
			return transform([x, y, z](D3DXMATRIX& m) { D3DXMatrixTranslation(&m, x, y, z); });
		}
		std::shared_ptr<Type> rotateX(float angle) {
			return transform([angle](D3DXMATRIX& m) { D3DXMatrixRotationX(&m, angle); });
		}
		std::shared_ptr<Type> rotateY(float angle) {
			return transform([angle](D3DXMATRIX& m) { D3DXMatrixRotationY(&m, angle); });
		}
		std::shared_ptr<Type> rotateZ(float angle) {
			return transform([angle](D3DXMATRIX& m) { D3DXMatrixRotationZ(&m, angle); });
		}
		std::shared_ptr<Type> scale(float x, float y, float z) {
			return transform([x, y, z](D3DXMATRIX& m) { D3DXMatrixScaling(&m, x, y, z); });
		}
		std::shared_ptr<Type> scale(float xyz) { return scale(xyz, xyz, xyz); };
		std::shared_ptr<Type> scaleX(float scaleX) { return scale(scaleX, 1.0f, 1.0f); };
		std::shared_ptr<Type> scaleY(float scaleY) { return scale(1.0f, scaleY, 1.0f); };
		std::shared_ptr<Type> scaleZ(float scaleZ) { return scale(1.0f, 1.0f, scaleZ); };

	protected:
		std::shared_ptr<Type> shared_from_this() {
			return std::static_pointer_cast<Type>(BaseObject::shared_from_this());
		}
	};
}