#pragma once
#include <functional>

namespace FrokEngine
{
	using CallbackType = std::function<void()>;
	using CallbackType2 = std::function<void(int)>;

	/*---------
		Job
	----------*/
	class Job
	{
	public:
		Job(CallbackType&& callback) : _callback(std::move(callback))
		{
		}

		Job(CallbackType2&& callback) : _callback2(std::move(callback))
		{
		}

		template<typename T, typename Ret, typename... Args>
		Job(shared_ptr<T> owner, Ret(T::* memFunc)(Args...), Args&&... args)
		{
			_callback = [owner, memFunc, args...]()
			{
				(owner.get()->*memFunc)(args...);
			};
		}

		void Execute()
		{
			_callback();
		}

	private:
		CallbackType _callback;
		CallbackType2 _callback2;
	};
}

