#pragma once

namespace games {
	class IUpdateable {
	public:
		virtual void update(float time) = 0;
	};
}