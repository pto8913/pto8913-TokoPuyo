#pragma once

#include <vector>

#define DECLARE_DELEGATE(DelegateName) \
	typedef Delegate<void()> DelegateName;
#define DECLARE_DELEGATE_OneParam(DelegateName, val0) \
	typedef Delegate<void(val0)> DelegateName;
#define DECLARE_DELEGATE_TwoParams(DelegateName, val0, val1) \
	typedef Delegate<void(val0, val1)> DelegateName;

#define DECLARE_DELEGATE_RET(DelegateName, ret) \
	typedef Delegate<ret()> DelegateName;
#define DECLARE_DELEGATE_RET_OneParam(DelegateName, ret, val0) \
	typedef Delegate<ret(val0)> DelegateName;
#define DECLARE_DELEGATE_RET_TwoParams(DelegateName, ret, val0, val1) \
	typedef Delegate<ret(val0, val1)> DelegateName;

#define DECLARE_MULTICAST_DELEGATE(DelegateName) \
	typedef MulticastDelegate<void()> DelegateName;
#define DECLARE_MULTICAST_DELEGATE_OneParam(DelegateName, val0) \
	typedef MulticastDelegate<void(val0)> DelegateName;
#define DECLARE_MULTICAST_DELEGATE_TwoParams(DelegateName, val0, val1) \
	typedef MulticastDelegate<void(val0, val1)> DelegateName;
#define DECLARE_MULTICAST_DELEGATE_ThreeParams(DelegateName, val0, val1, val2) \
	typedef MulticastDelegate<void(val0, val1, val2)> DelegateName;

#define DECLARE_MULTICAST_DELEGATE_RET(DelegateName, ret) \
	typedef MulticastDelegate<ret()> DelegateName;
#define DECLARE_MULTICAST_DELEGATE_RET_OneParam(DelegateName, ret, val0) \
	typedef MulticastDelegate<ret(val0)> DelegateName;
#define DECLARE_MULTICAST_DELEGATE_RET_TwoParams(DelegateName, ret, val0, val1) \
	typedef MulticastDelegate<ret(val0, val1)> DelegateName;


class DelegateNotBound : public std::exception
{
public:
	const char* what() const noexcept override
	{
		return "Delegate is Not Bound.";
	}
};

template<typename T>
class Delegate;

template<typename Ret, typename ...Args>
class Delegate<Ret(Args...)>
{
public:
	Delegate()
	{
		new(&mData) std::nullptr_t(nullptr);
		mFunction = nullptr;
	}

	template<auto Function, typename = typename std::enable_if<std::is_function<typename std::remove_pointer<decltype(Function)>::type>::value&& std::is_invocable_r<Ret, decltype(Function), Args...>::value>::type>
	void Bind()
	{
		new(&mData) std::nullptr_t(nullptr);

		mFunction = +[](Storage*, Args... args) -> Ret
		{
			return Function(std::forward<Args>(args)...);
		};
	}

	template<auto Function, typename Type, typename = typename std::enable_if<std::is_member_function_pointer<decltype(Function)>::value&& std::is_invocable_r<Ret, decltype(Function), Type, Args...>::value>::type>
	void Bind(Type& instance)
	{
		new(&mData) Type* (&instance);

		mFunction = +[](Storage* data, Args... args) -> Ret
		{
			Type* instance = *reinterpret_cast<Type**>(data);
			return std::invoke(Function, instance, std::forward<Args>(args)...);
		};
	}

	template<typename Type>
	void Bind(Type&& funcPtr)
	{
		static_assert(sizeof(Type) <= sizeof(void*));

		if constexpr (std::is_lvalue_reference<Type>::value)
		{
			new (&mData) std::remove_reference_t<Type>* (&funcPtr);

			mFunction = +[](Storage* data, Args... args) -> Ret
			{
				std::remove_reference_t<Type>* instance = *reinterpret_cast<std::remove_reference_t<Type>**>(data);
				return std::invoke(*instance, std::forward<Args>(args)...);
			};
		}
		else
		{
			new (&mData) Type(std::move(funcPtr));

			mFunction = +[](Storage* data, Args... args) -> Ret
			{
				std::remove_reference_t<Type>* instance = reinterpret_cast<std::remove_reference_t<Type>*>(data);
				return std::invoke(*instance, std::forward<Args>(args)...);
			};
		}
	}

	explicit operator bool() const { return mFunction; }

	template<typename ...Args>
	Ret Invoke(Args&& ...args)
	{
		return (*this).Broadcast(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	Ret Broadcast(Args&& ...args)
	{
		if (!*this)
		{
			throw DelegateNotBound();
		}
		return mFunction(&mData, std::forward<Args>(args)...);
	}

	bool IsBound()
	{
		return mFunction;
	}
private:
	using Storage = std::aligned_storage_t<sizeof(void*), alignof(void*)>;
	// like using Func = void(Class::*)(Args)
	using Function = Ret(*)(Storage*, Args...);

	Storage mData;
	Function mFunction;
};

template<typename T>
class MulticastDelegate;

template<typename Ret, typename... Args>
class MulticastDelegate<Ret(Args...)>
{
public:
	MulticastDelegate(size_t size = 10u)
	{
		mDelegates.reserve(size);
	}

	template<auto Function>
	void Bind()
	{
		Delegate<Ret(Args...)> _delegate;
		mDelegates.push_back(_delegate);
		mDelegates.back().Bind<Function>();
	}

	template<auto Function, typename Type>
	void Bind(Type& instance)
	{
		Delegate<Ret(Args...)> _delegate;
		mDelegates.push_back(_delegate);
		mDelegates.back().Bind<Function>(instance);
	}

	template<typename Type>
	void Bind(Type&& funcPtr)
	{
		Delegate<Ret(Args...)> _delegate;
		mDelegates.push_back(_delegate);
		mDelegates.back().Bind<Type>(funcPtr);
	}

	explicit operator bool() const { return !mDelegates.empty(); }

	void Broadcast(Args ...args)
	{
		for (auto& _delegate : mDelegates)
		{
			_delegate.Broadcast(std::forward<Args>(args)...);
		}
	}

	bool IsBound()
	{
		return mDelegates.size() > 0;
	}
private:
	void Invoke(Args ...args)
	{
		for (auto& _delegate : mDelegates)
		{
			_delegate.Invoke(std::forward<Args>(args)...);
		}
	}

	std::vector<Delegate<Ret(Args...)>> mDelegates;
};