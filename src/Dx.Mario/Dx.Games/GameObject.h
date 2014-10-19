#pragma once
#include <memory>
#include <d3dx9.h>
#include <queue>
#include "ImmutableList.h"

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

		D3DXVECTOR3 globalPosition();

		template<class T, class... TArgs>
		std::shared_ptr<T> add(TArgs&&... args) {
			std::shared_ptr<T> item = std::make_shared<T>(std::forward<TArgs>(args)...);
			add(item);
			return item;
		}

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

		template <typename T>
		std::vector<std::shared_ptr<T>> findChildren()
		{
			std::vector<std::shared_ptr<T>> found;

			std::queue<std::shared_ptr<GameObject>> toProcess;
			toProcess.push(shared_from_this());

			std::shared_ptr<GameObject> current;

			while (toProcess.size() > 0) {
				current = toProcess.front();
				toProcess.pop();

				if (auto possible = std::dynamic_pointer_cast<T>(current))
					found.push_back(possible);
				else {
					if (auto children = current->children)
						for (auto child : *children)
							toProcess.push(child);
				}
			}

			return found;
		}

	private:
		ImmutableList<std::shared_ptr<GameObject>> children;
	};
}