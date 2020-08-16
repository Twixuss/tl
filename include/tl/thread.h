#pragma once
#include "common.h"
#include "optional.h"

#ifdef TL_NO_SYSTEM_HEADER
namespace TL {
#if OS_WINDOWS
extern "C" __declspec(dllimport) void __stdcall *CreateThread(void *security, umm stackSize, u32 (*func)(void *), void *param, u32 flags, u32 *id);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void *);
extern "C" __declspec(dllimport) int __stdcall SwitchToThread();
extern "C" __declspec(dllimport) void __stdcall Sleep(u32);
#elif OS_LINUX
extern "C" int pthread_create(void *newthread, void const  *attr, void *(*func) (void *), void *arg);
#endif
}
#else
#if OS_WINDOWS
#include <Windows.h>
#elif OS_LINUX
#include <pthread.h>
#endif
#endif

#pragma warning(push)
#pragma warning(disable : 4582)
#pragma warning(disable : 4583)
#pragma warning(disable : 4625)

namespace TL {

#if OS_WINDOWS
FORCEINLINE void switchThread() { SwitchToThread(); }
FORCEINLINE void spinIteration() { _mm_pause(); }
FORCEINLINE void sleepMilliseconds(u32 milliseconds) { Sleep(milliseconds); }
FORCEINLINE void sleepSeconds(u32 seconds) { Sleep(seconds * 1000); }

FORCEINLINE s8  lockAdd(s8  volatile *a, s8  b) { return _InterlockedExchangeAdd8(a, b); }
FORCEINLINE s16 lockAdd(s16 volatile *a, s16 b) { return _InterlockedExchangeAdd16(a, b); }
FORCEINLINE s32 lockAdd(s32 volatile *a, s32 b) { return (s32)_InterlockedExchangeAdd((long *)a, (long)b); }
FORCEINLINE s64 lockAdd(s64 volatile *a, s64 b) { return _InterlockedExchangeAdd64(a, b); }

FORCEINLINE s8  lockIncrement(s8  volatile *v) { return lockAdd(v, 1); }
FORCEINLINE s16 lockIncrement(s16 volatile *v) { return _InterlockedIncrement16(v); }
FORCEINLINE s32 lockIncrement(s32 volatile *v) { return (s32)_InterlockedIncrement((long *)v); }
FORCEINLINE s64 lockIncrement(s64 volatile *v) { return _InterlockedIncrement64(v); }

FORCEINLINE s8  lockDecrement(s8  volatile *v) { return lockAdd(v, -1); }
FORCEINLINE s16 lockDecrement(s16 volatile *v) { return _InterlockedDecrement16(v); }
FORCEINLINE s32 lockDecrement(s32 volatile *v) { return (s32)_InterlockedDecrement((long *)v); }
FORCEINLINE s64 lockDecrement(s64 volatile *v) { return _InterlockedDecrement64(v); }

template <class T>
FORCEINLINE T lockSet(T volatile *dst, T src) {
	       if constexpr (sizeof(T) == 8) { s64 result = _InterlockedExchange64((s64 *)dst, *(s64 *)&src); return *(T *)&result;
	} else if constexpr (sizeof(T) == 4) { s32 result = _InterlockedExchange  ((s32 *)dst, *(s32 *)&src); return *(T *)&result;
	} else if constexpr (sizeof(T) == 2) { s16 result = _InterlockedExchange16((s16 *)dst, *(s16 *)&src); return *(T *)&result;
	} else if constexpr (sizeof(T) == 1) { s8  result = _InterlockedExchange8 ((s8  *)dst, *(s8  *)&src); return *(T *)&result;
	} else {
		static_assert(false);
	}
}

template <class T>
FORCEINLINE T lockSetIfEquals(T volatile *dst, T newValue, T comparand) {
	       if constexpr (sizeof(T) == 8) { s64 result = _InterlockedCompareExchange64((s64 *)dst, *(s64 *)&newValue, *(s64 *)&comparand); return *(T *)&result;
	} else if constexpr (sizeof(T) == 4) { s32 result = _InterlockedCompareExchange  ((s32 *)dst, *(s32 *)&newValue, *(s32 *)&comparand); return *(T *)&result;
	} else if constexpr (sizeof(T) == 2) { s16 result = _InterlockedCompareExchange16((s16 *)dst, *(s16 *)&newValue, *(s16 *)&comparand); return *(T *)&result;
	} else if constexpr (sizeof(T) == 1) { s8  result = _InterlockedCompareExchange8 ((s8  *)dst, *(s8  *)&newValue, *(s8  *)&comparand); return *(T *)&result;
	} else {
		static_assert(false);
	}
}

struct ThreadHandle;
FORCEINLINE ThreadHandle *createThread(void (*fn)(void *), void *param) {
	bool volatile gotData = false;
	struct Data {
		void (*fn)(void *);
		void *param;
		bool volatile *gotData;
	};
	Data data;
	data.fn = fn;
	data.param = param;
	data.gotData = &gotData;
	void *result = CreateThread(0, 0, [](void *_data) -> u32 {
		Data data = *(Data *)_data;
		*data.gotData = true;
		data.fn(data.param);
	}, &data, 0, 0);
	while (!gotData);
	return (ThreadHandle *)result;
}
FORCEINLINE void destroyThread(ThreadHandle *handle) {
	CloseHandle((void *)handle);
}
#else
FORCEINLINE s8  lockAdd(s8  volatile *a, s8  b) { return __sync_fetch_and_add(a, b); }
FORCEINLINE s16 lockAdd(s16 volatile *a, s16 b) { return __sync_fetch_and_add(a, b); }
FORCEINLINE s32 lockAdd(s32 volatile *a, s32 b) { return __sync_fetch_and_add(a, b); }
FORCEINLINE s64 lockAdd(s64 volatile *a, s64 b) { return __sync_fetch_and_add(a, b); }

FORCEINLINE s8  lockIncrement(s8  volatile *v) { return lockAdd(v, 1); }
FORCEINLINE s16 lockIncrement(s16 volatile *v) { return lockAdd(v, 1); }
FORCEINLINE s32 lockIncrement(s32 volatile *v) { return lockAdd(v, 1); }
FORCEINLINE s64 lockIncrement(s64 volatile *v) { return lockAdd(v, 1); }

FORCEINLINE s8  lockDecrement(s8  volatile *v) { return lockAdd(v, -1); }
FORCEINLINE s16 lockDecrement(s16 volatile *v) { return lockAdd(v, -1); }
FORCEINLINE s32 lockDecrement(s32 volatile *v) { return lockAdd(v, -1); }
FORCEINLINE s64 lockDecrement(s64 volatile *v) { return lockAdd(v, -1); }

template <class T>
FORCEINLINE T lockSetIfEquals(T volatile *dst, T newValue, T comparand) {
	return __sync_val_compare_and_swap(dst, comparand, newValue);
}
#endif

FORCEINLINE u16 lockIncrement(u16 volatile *v) { return (u16)lockIncrement((s16 *)v); }
FORCEINLINE u32 lockIncrement(u32 volatile *v) { return (u32)lockIncrement((s32 *)v); }
FORCEINLINE u64 lockIncrement(u64 volatile *v) { return (u64)lockIncrement((s64 *)v); }

FORCEINLINE u16 lockDecrement(u16 volatile *v) { return (u16)lockDecrement((s16 *)v); }
FORCEINLINE u32 lockDecrement(u32 volatile *v) { return (u32)lockDecrement((s32 *)v); }
FORCEINLINE u64 lockDecrement(u64 volatile *v) { return (u64)lockDecrement((s64 *)v); }

FORCEINLINE u8  lockAdd(u8  volatile *a, u8  b) { return (u8 )lockAdd((s8  *)a, (s8 )b); }
FORCEINLINE u16 lockAdd(u16 volatile *a, u16 b) { return (u16)lockAdd((s16 *)a, (s16)b); }
FORCEINLINE u32 lockAdd(u32 volatile *a, u32 b) { return (u32)lockAdd((s32 *)a, (s32)b); }
FORCEINLINE u64 lockAdd(u64 volatile *a, u64 b) { return (u64)lockAdd((s64 *)a, (s64)b); }

FORCEINLINE s16 lockSubtract(s16 volatile *a, s16 b) { return lockAdd(a, -b); }
FORCEINLINE s32 lockSubtract(s32 volatile *a, s32 b) { return lockAdd(a, -b); }
FORCEINLINE s64 lockSubtract(s64 volatile *a, s64 b) { return lockAdd(a, -b); }
FORCEINLINE u16 lockSubtract(u16 volatile *a, u16 b) { return lockAdd(a, -b); }
FORCEINLINE u32 lockSubtract(u32 volatile *a, u32 b) { return lockAdd(a, -b); }
FORCEINLINE u64 lockSubtract(u64 volatile *a, u64 b) { return lockAdd(a, -b); }

FORCEINLINE s16 lockIncrement(s16 volatile &v) { return lockIncrement(&v); }
FORCEINLINE s32 lockIncrement(s32 volatile &v) { return lockIncrement(&v); }
FORCEINLINE s64 lockIncrement(s64 volatile &v) { return lockIncrement(&v); }
FORCEINLINE u16 lockIncrement(u16 volatile &v) { return lockIncrement(&v); }
FORCEINLINE u32 lockIncrement(u32 volatile &v) { return lockIncrement(&v); }
FORCEINLINE u64 lockIncrement(u64 volatile &v) { return lockIncrement(&v); }

FORCEINLINE s16 lockDecrement(s16 volatile &v) { return lockDecrement(&v); }
FORCEINLINE s32 lockDecrement(s32 volatile &v) { return lockDecrement(&v); }
FORCEINLINE s64 lockDecrement(s64 volatile &v) { return lockDecrement(&v); }
FORCEINLINE u16 lockDecrement(u16 volatile &v) { return lockDecrement(&v); }
FORCEINLINE u32 lockDecrement(u32 volatile &v) { return lockDecrement(&v); }
FORCEINLINE u64 lockDecrement(u64 volatile &v) { return lockDecrement(&v); }

FORCEINLINE s16 lockAdd(s16 volatile &a, s16 b) { return lockAdd(&a, b); }
FORCEINLINE s32 lockAdd(s32 volatile &a, s32 b) { return lockAdd(&a, b); }
FORCEINLINE s64 lockAdd(s64 volatile &a, s64 b) { return lockAdd(&a, b); }
FORCEINLINE u16 lockAdd(u16 volatile &a, u16 b) { return lockAdd(&a, b); }
FORCEINLINE u32 lockAdd(u32 volatile &a, u32 b) { return lockAdd(&a, b); }
FORCEINLINE u64 lockAdd(u64 volatile &a, u64 b) { return lockAdd(&a, b); }

FORCEINLINE s16 lockSubtract(s16 volatile &a, s16 b) { return lockSubtract(&a, b); }
FORCEINLINE s32 lockSubtract(s32 volatile &a, s32 b) { return lockSubtract(&a, b); }
FORCEINLINE s64 lockSubtract(s64 volatile &a, s64 b) { return lockSubtract(&a, b); }
FORCEINLINE u16 lockSubtract(u16 volatile &a, u16 b) { return lockSubtract(&a, b); }
FORCEINLINE u32 lockSubtract(u32 volatile &a, u32 b) { return lockSubtract(&a, b); }
FORCEINLINE u64 lockSubtract(u64 volatile &a, u64 b) { return lockSubtract(&a, b); }

#if 0
template <class T>
struct AtomicBase {
	T volatile value;

	AtomicBase() = default;
	AtomicBase(AtomicBase const &) = delete;
	AtomicBase(AtomicBase &&) = delete;
	AtomicBase(T that) : value(that) {}
	AtomicBase &operator=(AtomicBase const &) = delete;
	AtomicBase &operator=(AtomicBase &&) = delete;
	T store(T that) { return lockSet(&value, that), that; }
	T load() const { READ_WRITE_BARRIER; return value; }
	T operator=(T that) { return store(that); }
	operator T() const { return load(); }
};

template <class Int>
struct AtomicIntBase : AtomicBase<Int> {
	using Base = AtomicBase;
	using Base::Base;
	using Base::operator=;
	AtomicIntBase &operator++() { return lockIncrement(&value), *this; }
	AtomicIntBase &operator--() { return lockDecrement(&value), *this; }
	AtomicIntBase operator++(int) { return lockIncrement(&value) - 1; }
	AtomicIntBase operator--(int) { return lockDecrement(&value) + 1; }
	Int operator+=(Int that) { return lockAdd(value, that) + that; }
	Int operator-=(Int that) { return lockSubtract(value, that) - that; }
};

template <class T>
using SelectAtomicBase = std::conditional_t<std::is_integral_v<T>, AtomicIntBase<T>, AtomicBase<T>>;

template <class T>
struct Atomic : SelectAtomicBase<T> {
	using Base = SelectAtomicBase<T>;
	using Base::Base;
	using Base::operator=;
};

// single producer, single consumer
namespace SPSC {
template <class T, size_t capacity>
struct CircularQueue {
	CircularQueue()						 = default;
	CircularQueue(CircularQueue const &) = delete;
	template <class... Args>
	void emplace(Args &&... args) {
		getSpace();
		new (std::addressof(end->val)) T(std::forward<Args>(args)...);
		++poppableCount;
		increment(end);
	}
	void push(T &&val) { emplace(std::move(val)); }
	void push(T const &val) { emplace(val); }
	T pop() {
		ASSERT (poppableCount);
		--poppableCount;
		T result = std::move(begin->val);
		begin->val.~T();
		++pushableCount;
		increment(begin);
		return result;
	}
	Optional<T> try_pop() {
		Optional<T> result{};
		if (poppableCount) {
			--poppableCount;
			result.emplace(std::move(begin->val));
			begin->val.~T();
			++pushableCount;
			increment(begin);
		}
		return result;
	}
	umm size() const { return poppableCount; }

private:
	union Entry {
		T val;
		Entry() {}
		~Entry() {}
	};
	Entry entries[capacity];
	Entry *begin			  = entries;
	Entry *end				  = begin;
	Atomic<umm> poppableCount = 0;
	Atomic<umm> pushableCount = capacity;
	void increment(Entry *&e) {
		if (++e == entries + capacity) {
			e = entries;
		}
	}
	void getSpace() {
		while (pushableCount == 0) {
#if !COMPILER_GCC
			std::this_thread::sleep_for(std::chrono::microseconds(1));
#endif
		}
		--pushableCount;
	}
};
} // namespace SPSC
#endif
} // namespace TL
#pragma warning(pop)
