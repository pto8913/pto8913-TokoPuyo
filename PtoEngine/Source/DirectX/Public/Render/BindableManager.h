#pragma once

#include "DirectX/DirectXHead.h"
#include "Render/Bindable.h"
#include <unordered_map>

class BindableManager
{
public:
	virtual ~BindableManager()
	{
		auto iter = Binds.begin();
		while (iter != Binds.end())
		{
			auto& row = *iter;
			if (row.second != nullptr)
			{
				row.second.reset();
				row.second = nullptr;

				iter = Binds.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		Binds.clear();
	}
	static BindableManager& Get() {
		static BindableManager instance;
		return instance;
	}

	template<class T, typename... Args>
	static std::shared_ptr<T> Make(DirectX11& dx, Args&& ...args)
	{
		static_assert(std::is_base_of<Bindable, T>::value, "Can only make ptr class derivered from Bindable.");
		return Get()._Make<T>(dx, std::forward<Args>(args)...);
	}
private:
	template<class T, typename... Args>
	std::shared_ptr<T> _Make(DirectX11& dx, Args&& ...args)
	{
		const std::string key = T::GenerateID(std::forward<Args>(args)...);
		const auto findBind = Binds.find(key);
		if (findBind == Binds.end())
		{
			/* Not found */
			auto res = std::make_shared<T>(dx, std::forward<Args>(args)...);
			Binds[key] = res;
			return res;
		}
		else
		{
			return std::static_pointer_cast<T>(findBind->second);
		}
	}
private:
	std::unordered_map<std::string, std::shared_ptr<Bindable>> Binds;
};