#include <thread>

/* NOTE : must be use DefferedContext_n per Timer. */
#define DECLARE_TIMER(TimerName) \
	typedef Timer<void()> TimerName;

/* Invalid Timer declare. */
//#define DECLARE_TIMER_RET(TimerName, Ret) \
//	typedef Timer<Ret()> TimerName;

class InvalidTimer : public std::exception
{
public:
	const char* what() const noexcept override
	{
		return "Invalid Timer. Return value can not use.";
	}
};

template<typename T>
class Timer;

/* Ret value is void only. */
template<typename Ret>
class Timer<Ret()>
{
public:
	Timer()
	{
		if (!std::is_same<Ret, void>::value)
		{
			throw InvalidTimer();
			return;
		}

		ClearTimer();
	}
	~Timer()
	{
		ClearTimer();
	}

	template<auto Function, typename = typename std::enable_if<std::is_function<typename std::remove_pointer<decltype(Function)>::type>::value&& std::is_invocable_r<Ret, decltype(Function)>::value>::type>
	void SetTimer(int inMilliSeconds)
	{
		bGo = true;
		milliSeconds = inMilliSeconds;
		new(&mData) std::nullptr_t(nullptr);

		mFunction = +[](Storage*) -> Ret
		{
			return Function();
		};
		std::thread t1(&Timer::run, this);
		t1.detach();
	}

	template<auto Function, typename Type, typename = typename std::enable_if<std::is_member_function_pointer<decltype(Function)>::value&& std::is_invocable_r<Ret, decltype(Function), Type>::value>::type>
	void SetTimer(Type& instance, int inMilliSeconds)
	{
		bGo = true;
		milliSeconds = inMilliSeconds;

		new(&mData) Type* (&instance);
		mFunction = +[](Storage* data) -> Ret
		{
			Type* instance = *reinterpret_cast<Type**>(data);
			return std::invoke(Function, instance);
		};
		std::thread t1(&Timer::run, this);
		t1.detach();
	}

	template<typename Type>
	void SetTimer(Type&& funcPtr, int inMilliSeconds)
	{
		static_assert(sizeof(Type) <= sizeof(void*));

		bGo = true;
		milliSeconds = inMilliSeconds;

		if constexpr (std::is_lvalue_reference<Type>::value)
		{
			new (&mData) std::remove_reference_t<Type>* (&funcPtr);

			mFunction = +[](Storage* data) -> Ret
			{
				std::remove_reference_t<Type>* instance = *reinterpret_cast<std::remove_reference_t<Type>**>(data);
				return std::invoke(*instance);
			};
		}
		else
		{
			new (&mData) Type(std::move(funcPtr));

			mFunction = +[](Storage* data) -> Ret
			{
				std::remove_reference_t<Type>* instance = reinterpret_cast<std::remove_reference_t<Type>*>(data);
				return std::invoke(*instance);
			};
		}

		std::thread t1(&Timer::run, this);
		t1.detach();
	}

	void ClearTimer()
	{
		bGo = false;
		milliSeconds = -1;

		new(&mData) std::nullptr_t(nullptr);
		mFunction = nullptr;
	}

private:
	void run()
	{
		while (bGo)
		{
			mFunction(&mData);
			std::this_thread::sleep_for(std::chrono::milliseconds(milliSeconds));
		}
	}

	bool bGo;
	int milliSeconds;

	using Storage = std::aligned_storage_t<sizeof(void*), alignof(void*)>;
	using Function = Ret(*)(Storage*);

	Storage mData;
	Function mFunction;
};
