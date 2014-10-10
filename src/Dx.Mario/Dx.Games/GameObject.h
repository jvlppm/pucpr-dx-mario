#pragma once
#include <memory>

namespace games {
	class GameObject : std::enable_shared_from_this<GameObject>
	{
	public:
		std::weak_ptr<GameObject> parent_ref;

		GameObject();
		~GameObject();

		virtual void init();
		virtual void destroy();

		void add(std::shared_ptr<GameObject> child);
		void remove(std::shared_ptr<GameObject> child);

		template <typename T>
		std::shared_ptr<T> getAncestor()
		{
			static_assert(std::is_base_of<GameObject, T>::value, "T must be a descendant of GameObject");
			auto current = shared_from_this();
			while (current) {
				auto res = std::dynamic_pointer_cast<T>(current);
				if (res)
					return res;

				current = current->parent_ref.lock();
			}
			return nullptr;
		}

	private:
		struct private_implementation;
		std::unique_ptr<private_implementation> pImpl;
	};
}