#pragma once
#include <memory>
#include <d3dx9.h>
#include <queue>
#include "ImmutableList.h"

namespace games {
	class BaseObject : protected std::enable_shared_from_this<BaseObject> {
	public:
		BaseObject();

		~BaseObject();

		virtual void init();
		virtual void destroy();

		void add(std::shared_ptr<BaseObject> child);
		void remove(std::shared_ptr<BaseObject> child);

		void setParent(std::weak_ptr<BaseObject> parent);

		std::shared_ptr<BaseObject> getParent();

		template <typename T>
		std::shared_ptr<T> getAncestor()
		{
			static_assert(std::is_base_of<BaseObject, T>::value, "T must be a descendant of BaseObject");
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

			std::queue<std::shared_ptr<BaseObject>> toProcess;
			toProcess.push(shared_from_this());

			std::shared_ptr<BaseObject> current;

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

		template<class T, class... TArgs>
		std::shared_ptr<T> add(TArgs&&... args) {
			std::shared_ptr<T> item = std::make_shared<T>(std::forward<TArgs>(args)...);
			add(item);
			return item;
		}

		D3DXVECTOR3 worldPosition();
		D3DXMATRIX world();

		template <typename T>
		std::shared_ptr<BaseObject> transform(T func) {
			func(tmpTransform);
			localTransform = tmpTransform * localTransform;
			invalidateTransform();
			return shared_from_this();
		}

	private:
		std::weak_ptr<BaseObject> parent_ref;
		ImmutableList<std::shared_ptr<BaseObject>> children;

		bool dirtyTransform;
		D3DXMATRIX tmpTransform;
		D3DXMATRIX localTransform;
		D3DXMATRIX globalTransform;

		void invalidateTransform();
		void updateTransform();
	};
}