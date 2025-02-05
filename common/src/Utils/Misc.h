#pragma once
#include <coroutine>
#include <cstdint>
#include <memory>

namespace Misc
{

template <typename T>
struct Generator
{
	struct promise_type
	{
		T current_value;

		auto get_return_object() { return Generator{*this}; }
		std::suspend_always initial_suspend() { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		void return_void() {}
		void unhandled_exception() { std::terminate(); }

		std::suspend_always yield_value(T value)
		{
			current_value = value;
			return {};
		}
	};

	using Handle = std::coroutine_handle<promise_type>;
	Handle handle;

	Generator(promise_type& p) : handle(Handle::from_promise(p)) {}
	~Generator() { if (handle) handle.destroy(); }

	bool next() const
	{
		if (!handle.done())
		{
			handle.resume();
			return true;
		}
		return false;
	}

	T value() const { return handle.promise().current_value; }
};

Generator<unsigned> FibonacciGen();

uint16_t GenerateUUID();

} // namespace Misc
