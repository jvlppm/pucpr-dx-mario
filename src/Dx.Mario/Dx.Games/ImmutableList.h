#pragma once
#include <memory>
#include <vector>

template<typename T>
class ImmutableList :
	public std::shared_ptr<std::vector<T>>
{
public:
	ImmutableList()
		: std::shared_ptr<std::vector<T>>(std::make_shared<std::vector<T>>()) {

		}

	~ImmutableList() {
	}

	ImmutableList<T> add(T item) {
		auto newItems = new std::vector<T>((*this)->begin(), (*this)->end());
		newItems->push_back(item);
		return ImmutableList<T>(newItems);
	}

	ImmutableList<T> remove(T item) {
		auto newItems = new std::vector<T>((*this)->begin(), (*this)->end());
		newItems->erase(std::find(newItems->begin(), newItems->end(), item));
		return ImmutableList<T>(newItems);
	}

	private:
		ImmutableList(std::vector<T>* data)
		: std::shared_ptr<std::vector<T>>(data) {
		}
};
