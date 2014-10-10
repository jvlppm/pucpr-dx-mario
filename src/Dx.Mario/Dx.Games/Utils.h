#pragma once
#include <memory>
#include <vector>

class Utils
{
public:
	template <typename T>
	static std::shared_ptr<std::vector<T>> add(std::shared_ptr<std::vector<T>> items, T item)
	{
		auto newItems = new std::vector<T>(items->begin(), items->end());
		newItems->push_back(item);
		return std::shared_ptr<std::vector<T>>(newItems);
	}

	template <typename T>
	static std::shared_ptr<std::vector<T>> remove(std::shared_ptr<std::vector<T>> items, T item)
	{
		auto newItems = new std::vector<T>(items->begin(), items->end());
		newItems->erase(std::find(newItems->begin(), newItems->end(), item));
		return std::shared_ptr<std::vector<T>>(newItems);
	}
};

