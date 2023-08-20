#pragma once

#include <vector>
#include <string>

#define DECLARE_DELEGATE(DelegateName) typedef Delegate<void()> DelegateName;
#define DECLARE_DELEGATE_OneParam(DelegateName, val0) typedef Delegate<void(val0)> DelegateName;
#define DECLARE_DELEGATE_TwoParams(DelegateName, val0, val1) typedef Delegate<void(val0, val1)> DelegateName;

#define DECLARE_DELEGATE_RET(DelegateName, ret) typedef Delegate<ret()> DelegateName;
#define DECLARE_DELEGATE_RET_OneParam(DelegateName, ret, val0) typedef Delegate<ret(val0)> DelegateName;
#define DECLARE_DELEGATE_RET_TwoParams(DelegateName, ret, val0, val1) typedef Delegate<ret(val0, val1)> DelegateName;

#define DECLARE_MULTICAST_DELEGATE(DelegateName) typedef MulticastDelegate<void()> DelegateName;
#define DECLARE_MULTICAST_DELEGATE_OneParam(DelegateName, val0) typedef MulticastDelegate<void(val0)> DelegateName;
#define DECLARE_MULTICAST_DELEGATE_TwoParams(DelegateName, val0, val1) typedef MulticastDelegate<void(val0, val1)> DelegateName;
#define DECLARE_MULTICAST_DELEGATE_ThreeParams(DelegateName, val0, val1, val2) typedef MulticastDelegate<void(val0, val1, val2)> DelegateName;

#define DECLARE_MULTICAST_DELEGATE_RET(DelegateName, ret) typedef MulticastDelegate<ret()> DelegateName;
#define DECLARE_MULTICAST_DELEGATE_RET_OneParam(DelegateName, ret, val0) typedef MulticastDelegate<ret(val0)> DelegateName;
#define DECLARE_MULTICAST_DELEGATE_RET_TwoParams(DelegateName, ret, val0, val1) typedef MulticastDelegate<ret(val0, val1)> DelegateName;

class DelegateNotBound : public std::exception
{
public:
	const char* what() const noexcept override
	{
		return "Delegate is not bound";
	}
};

template<typename T>
class Delegate;

template<typename T>
class MulticastDelegate;

template<typename Ret, typename ...Args>
class Delegate<Ret(Args...)>
{
	friend class MulticastDelegate<Ret(Args...)>;
public:
	Delegate()
	{
		ClearBind();
	}

	// グローバル関数の保存
	template<auto Function, typename = typename std::enable_if_t<std::is_function_v<typename std::remove_pointer_t<decltype(Function)>>&& std::is_invocable_r_v<Ret, decltype(Function), Args...>>>
	void Bind()
	{
		new (&mData) std::nullptr_t(nullptr);

		/* 関数をラムダ式で保存する！ */
		mFunction = [](Storage, Args... args)
		{
			return Function(std::forward<Args>(args)...);
		};
	}

	// メンバ関数の保存
	template<auto Function, typename Type, typename = typename std::enable_if_t<std::is_member_function_pointer_v<decltype(Function)>&& std::is_invocable_r_v<Ret, decltype(Function), Type, Args...>>>
	void Bind(Type& instance)
	{
		new (&mData) Type* (&instance);

		mFunction = [](Storage data, Args... args)
		{
			Type* instance = *reinterpret_cast<Type**>(data);
			return std::invoke(Function, *instance, std::forward<Args>(args)...);
		};
	};

	// lvalueのラムダ式の保存
	template<typename Type>
	void Bind(Type& funcPtr)
	{
		new (&mData) Type* (&funcPtr);

		mFunction = [](Storage data, Args... args)
		{
			Type* instance = *reinterpret_cast<Type**>(data);
			return std::invoke(*instance, std::forward<Args>(args)...);
		};
	}

	// rvalueのラムダ式の保存
	template<typename Type>
	void Bind(Type&& funcPtr)
	{
		new (&mData) Type(std::move(funcPtr));

		mFunction = [](Storage data, Args... args)
		{
			Type* instance = reinterpret_cast<Type*>(data);
			return std::invoke(*instance, std::forward<Args>(args)...);
		};
	}

	// 削除
	void ClearBind()
	{
		new (&mData) std::nullptr_t(nullptr);
		mFunction = nullptr;
	}

	explicit operator bool() const { return mFunction; }

	template<typename ...Args>
	Ret Broadcast(Args&& ...args)
	{
		if (!*this)
		{
			throw DelegateNotBound();
		}
		/* 関数ポインタに値(args)を渡して呼び出す！ */
		return mFunction(&mData, std::forward<Args>(args)...);
	}

	bool IsBound() const
	{
		return mFunction;
	}
private:
	using Storage = std::byte(*)[sizeof(void*)];
	using Function = Ret(*)(Storage, Args...);

	/* ラムダ式などを保存する */
	alignas(void*) std::byte mData[sizeof(void*)];

	/* 関数ポインタ */
	Function mFunction;
};

template<typename Ret, typename ...Args>
class MulticastDelegate<Ret(Args...)>
{
private:
	using DelegateType = Delegate<Ret(Args...)>;
	using MulticastDelegateType = MulticastDelegate<Ret(Args...)>;
public:
	// グローバル関数の保存
	template<auto Function, typename = typename std::enable_if_t<std::is_function_v<typename std::remove_pointer_t<decltype(Function)>>&& std::is_invocable_r_v<Ret, decltype(Function), Args...>>>
	void Bind(std::string tag)
	{
		DelegateType _delegate;
		mDelegates.push_back(_delegate);
		mDelegates.back().Bind<Function>();
		mFunctionTags.push_back(tag);
	}

	// メンバ関数の保存
	template<auto Function, typename Type, typename = typename std::enable_if_t<std::is_member_function_pointer_v<decltype(Function)>&& std::is_invocable_r_v<Ret, decltype(Function), Type, Args...>>>
	void Bind(Type& instance, std::string tag)
	{
		DelegateType _delegate;
		mDelegates.push_back(_delegate);
		mDelegates.back().Bind<Function>(instance);
		mFunctionTags.push_back(tag);
	};

	// lvalueのラムダ式の保存
	template<typename Type>
	void Bind(Type& funcPtr, std::string tag)
	{
		DelegateType _delegate;
		mDelegates.push_back(_delegate);
		mDelegates.back().Bind<Type>(funcPtr);
		mFunctionTags.push_back(tag);
	}

	// rvalueのラムダ式の保存
	template<typename Type>
	void Bind(Type&& funcPtr, std::string tag)
	{
		DelegateType _delegate;
		mDelegates.push_back(_delegate);
		mDelegates.back().Bind<Type>(funcPtr);
		mFunctionTags.push_back(tag);
	}

	void Add(const DelegateType& inDelegate, std::string tag)
	{
		mDelegates.push_back(inDelegate);
		mFunctionTags.push_back(tag);
	}

	void Append(const MulticastDelegateType& inDelegate, std::string tag)
	{
		for (auto&& _delegate : inDelegate.mDelegates)
		{
			mDelegates.push_back(_delegate);
		}
		mFunctionTags.push_back(tag);
	}

	void Unbind(std::string tag)
	{
		int i = 0;
		for (auto& _delegate : mDelegates)
		{
			if (mFunctionTags[i] == tag)
			{
				_delegate.ClearBind();

				mFunctionTags.erase(mFunctionTags.begin() + i);
				mDelegates.erase(mDelegates.begin() + i);
				break;
			}
			++i;
		}
	}

	void ClearBind()
	{
		for (auto& _delegate : mDelegates)
		{
			_delegate.ClearBind();
		}
		mDelegates.clear();
		mFunctionTags.clear();
	}

	explicit operator bool() const { return !mDelegates.empty(); }

	template<typename ...Args>
	void Broadcast(Args&& ...args)
	{
		for (auto& _delegate : mDelegates)
		{
			_delegate.Broadcast(std::forward<Args>(args)...);
		}
	}

	bool IsBound() const
	{
		return mDelegates.size() > 0;
	}
private:
	std::vector<DelegateType> mDelegates;
	std::vector<std::string> mFunctionTags;
};