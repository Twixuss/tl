#pragma once
#include "function.h"
#include "optional.h"
#include "list.h"

#pragma warning(push)
#pragma warning(disable: TL_DISABLED_WARNINGS)
#pragma warning(disable: 4582)
#pragma warning(disable: 4583)
#pragma warning(disable: 4625)
#pragma warning(disable: 4820)
#pragma warning(disable: 5220)

namespace TL {

TL_API void sleep_milliseconds(u32 milliseconds);
TL_API void sleep_seconds(u32 seconds);
#if OS_WINDOWS
TL_API void switch_thread();
forceinline void spin_iteration() { _mm_pause(); }

forceinline s8  atomic_add(s8  volatile *a, s8  b) { return _InterlockedExchangeAdd8((char *)a, (char)b); }
forceinline s16 atomic_add(s16 volatile *a, s16 b) { return _InterlockedExchangeAdd16(a, b); }
forceinline s32 atomic_add(s32 volatile *a, s32 b) { return (s32)_InterlockedExchangeAdd((long *)a, (long)b); }
forceinline s64 atomic_add(s64 volatile *a, s64 b) { return _InterlockedExchangeAdd64(a, b); }

template <class T>
forceinline T atomic_set(T volatile *dst, T src) {
	s64 result;
	     if constexpr (sizeof(T) == 8) result = _InterlockedExchange64((long long*)dst, *(long long*)&src); 
	else if constexpr (sizeof(T) == 4) result = _InterlockedExchange  ((long     *)dst, *(long     *)&src);
	else if constexpr (sizeof(T) == 2) result = _InterlockedExchange16((short    *)dst, *(short    *)&src);
	else if constexpr (sizeof(T) == 1) result = _InterlockedExchange8 ((char     *)dst, *(char     *)&src);
	else static_assert(false, "lockSet is not available for this size");
	return *(T *)&result;
}
template <class T>
forceinline T atomic_set(T volatile &dst, T src) { return atomic_set(&dst, src); }

template <class T>
forceinline T atomic_set_if_equals(T volatile *dst, T newValue, T comparand) {
	s64 result;
	     if constexpr (sizeof(T) == 8) result = _InterlockedCompareExchange64((long long*)dst, *(long long*)&newValue, *(long long*)&comparand);
	else if constexpr (sizeof(T) == 4) result = _InterlockedCompareExchange  ((long     *)dst, *(long     *)&newValue, *(long     *)&comparand);
	else if constexpr (sizeof(T) == 2) result = _InterlockedCompareExchange16((short    *)dst, *(short    *)&newValue, *(short    *)&comparand);
	else if constexpr (sizeof(T) == 1) result = _InterlockedCompareExchange8 ((char     *)dst, *(char     *)&newValue, *(char     *)&comparand);
	else static_assert(false, "atomic_set_if_equals is not available for this size");
	return *(T *)&result;
}
template <class T>
forceinline T atomic_set_if_equals(T volatile &dst, T newValue, T comparand) {
	return atomic_set_if_equals(std::addressof(dst), newValue, comparand);
}

TL_DECLARE_HANDLE(Thread);
TL_API Thread create_thread(void (*fn)(void *), void *param);
TL_API void destroy_thread(Thread thread);
TL_API u32 get_current_thread_id();
TL_API u32 get_thread_id(Thread thread);
TL_API void join(Thread thread);

inline Thread create_thread(Function function) {
	return create_thread(function.function, function.param);
}

template <class Fn, class ...Args>
Thread create_thread(Fn &&fn, Args &&...args) {
	return create_thread(create_function(std::forward<Fn>(fn), std::forward<Args>(args)...));
}

#else
forceinline s8  atomic_add(s8  volatile *a, s8  b) { return __sync_fetch_and_add(a, b); }
forceinline s16 atomic_add(s16 volatile *a, s16 b) { return __sync_fetch_and_add(a, b); }
forceinline s32 atomic_add(s32 volatile *a, s32 b) { return __sync_fetch_and_add(a, b); }
forceinline s64 atomic_add(s64 volatile *a, s64 b) { return __sync_fetch_and_add(a, b); }

template <class T>
forceinline T atomic_set_if_equals(T volatile *dst, T newValue, T comparand) {
	return __sync_val_compare_and_swap(dst, comparand, newValue);
}
#endif

forceinline u8  atomic_add(u8  volatile *a, u8  b) { return (u8 )atomic_add((s8  *)a, (s8 )b); }
forceinline u16 atomic_add(u16 volatile *a, u16 b) { return (u16)atomic_add((s16 *)a, (s16)b); }
forceinline u32 atomic_add(u32 volatile *a, u32 b) { return (u32)atomic_add((s32 *)a, (s32)b); }
forceinline u64 atomic_add(u64 volatile *a, u64 b) { return (u64)atomic_add((s64 *)a, (s64)b); }

forceinline s16 atomic_add(s16 volatile &a, s16 b) { return atomic_add(&a, b); }
forceinline s32 atomic_add(s32 volatile &a, s32 b) { return atomic_add(&a, b); }
forceinline s64 atomic_add(s64 volatile &a, s64 b) { return atomic_add(&a, b); }
forceinline u16 atomic_add(u16 volatile &a, u16 b) { return atomic_add(&a, b); }
forceinline u32 atomic_add(u32 volatile &a, u32 b) { return atomic_add(&a, b); }
forceinline u64 atomic_add(u64 volatile &a, u64 b) { return atomic_add(&a, b); }

#ifdef TL_IMPL

#if OS_WINDOWS

void sleep_milliseconds(u32 milliseconds) { Sleep(milliseconds); }
void sleep_seconds(u32 seconds) { Sleep(seconds * 1000); }
void switch_thread() { SwitchToThread(); }

Thread create_thread(void (*fn)(void *), void *param) {
	struct Data {
		void (*fn)(void *);
		void *param;
		bool volatile acquired;
	};
	Data data = {};
	data.fn = fn;
	data.param = param;
	data.acquired = false;
	HANDLE result = CreateThread(0, 0, [](void *param) noexcept -> DWORD {
		Data *pData = (Data *)param;
		Data data = *pData;
		pData->acquired = true;
		data.fn(data.param);
		return 0;
	}, &data, 0, 0);
	while (!data.acquired);
	return (Thread)result;
}
void destroy_thread(Thread thread) {
	CloseHandle((HANDLE)thread);
}
u32 get_current_thread_id() {
	return GetCurrentThreadId();
}
u32 get_thread_id(Thread thread) {
	return GetThreadId((HANDLE)thread);
}
void join(Thread thread) {
	WaitForSingleObjectEx((HANDLE)thread, INFINITE, false);
}

#else // ^^^ OS_WINDOWS ^^^ vvvvvv
#endif
#endif // TL_IMPL

template <class Predicate>
void loop_until(Predicate &&predicate) {
	u32 try_count = 0;
	while (!predicate()) {
		spin_iteration();
		if (try_count >= 64)
			switch_thread();
		if (try_count >= 4096)
			sleep_milliseconds(1);
		++try_count;
	}
}

struct SyncPoint {
	u32 volatile current_counter;
	u32 target_counter;
};

inline SyncPoint create_sync_point(u32 target_counter) {
	SyncPoint result;
	result.current_counter = 0;
	result.target_counter = target_counter;
	return result;
}

inline void sync(SyncPoint &point) {
	atomic_add(point.current_counter, 1);
	loop_until([&] { return point.current_counter == point.target_counter; });
}

struct Mutex {
	bool volatile in_use = false;
};

inline bool try_lock(Mutex &m) {
	return !atomic_set_if_equals(&m.in_use, true, false);
}
inline void lock(Mutex &m) {
	loop_until([&] {
		return try_lock(m);
	});
}
inline void unlock(Mutex &m) {
	m.in_use = false;
}
inline void wait_for_unlock(Mutex &m) {
	loop_until([&] {
		return !m.in_use;
	});
}

struct RecursiveMutex {
	u32 volatile thread_id = 0;
	u32 counter = 0;
};

inline bool try_lock(RecursiveMutex &m) {
	u32 thread_id = get_current_thread_id();
	if (thread_id == m.thread_id) {
		++m.counter;
		return true;
	} else {
		return !atomic_set_if_equals(m.thread_id, thread_id, (u32)0);
	}
}
inline void lock(RecursiveMutex &m) {
	u32 thread_id = get_current_thread_id();
	if (thread_id == m.thread_id) {
		++m.counter;
	} else {
		loop_until([&] {
			return !atomic_set_if_equals(m.thread_id, thread_id, (u32)0);
		});
	}
}
inline void unlock(RecursiveMutex &m) {
	if (m.counter == 0) {
		m.thread_id = 0;
	} else {
		--m.counter;
	}
}

#define scoped_lock(mutex) lock(mutex); defer { unlock(mutex); }
#define scoped_unlock(mutex) unlock(mutex); defer { lock(mutex); }

template <class T, class Mutex = Mutex>
struct MutexQueue {
	Queue<T> base;
	Mutex mutex;
	
	void push_front_unordered_no_lock(T const &value) { base.push_front_unordered(value); }
	void push_front_unordered(T const &value) { scoped_lock(mutex); base.push_front_unordered(value); }
	void push_no_lock(T const &value) { base.push(value); }
	void push(T const &value) { scoped_lock(mutex); base.push(value); }

	Optional<T> try_pop() {
		Optional<T> result;
		scoped_lock(mutex);
		if (base.size) {
			result = base.front();
			base.pop();
		}
		return result;
	}
	template <class Fn>
	Optional<T> try_pop_and(Fn &&fn) {
		Optional<T> result;
		scoped_lock(mutex);
		if (base.size) {
			result = base.front();
			base.pop();
			fn(result.value);
		}
		return result;
	}
	T pop() {
		Optional<T> opt;
		loop_until([&] {
			opt = try_pop();
			return opt.has_value();
		});
		return opt.value();
	}
	void clear() {
		scoped_lock(mutex);
		base.clear();
	}
	void erase_no_lock(T *ptr) {
		base.erase(ptr);
	}
	template <class Fn>
	void pop_all_nolock(Fn &&fn) {
		for (auto &value : base) {
			fn(value);
		}
		base.clear();
	}
	template <class Fn>
	void pop_all(Fn &&fn) {
		scoped_lock(mutex);
		pop_all_nolock(std::forward<Fn>(fn));
	}
	umm size() { return base.size; }
	
	void append_no_lock(T const &v)                 { base += v; }
	void append_no_lock(T &&v)                      { base += v; }
	void append_no_lock(Span<T const> v)            { base += v; }
	void append_no_lock(std::initializer_list<T> v) { base += v; }
	void append(T const &v)                 { scoped_lock(mutex); base += v; }
	void append(T &&v)                      { scoped_lock(mutex); base += v; }
	void append(Span<T const> v)            { scoped_lock(mutex); base += v; }
	void append(std::initializer_list<T> v) { scoped_lock(mutex); base += v; }
};

template <class T, class Mutex>
Span<T> as_span(MutexQueue<T, Mutex> &queue) { return as_span(queue.base); }

template <class T, umm capacity, class Mutex = Mutex>
struct StaticMutexCircularQueue {
	void push(T &&value) {
		scoped_lock(mutex);
		base.push(std::move(value));
	}
	void push(T const &value) {
		scoped_lock(mutex);
		base.push(value);
	}
	Optional<T> try_pop() {
		Optional<T> result;
		scoped_lock(mutex);
		if (base.size()) {
			result.emplace(std::move(base.front()));
			base.pop();
		}
		return result;
	}
	T pop() {
		Optional<T> opt;
		loop_until([&] {
			opt = try_pop();
			return opt.has_value();
		});
		return opt.value();
	}
	void clear() {
		scoped_lock(mutex);
		base.clear();
	}

private:
	StaticCircularQueue<T, capacity> base;
	Mutex mutex;
};

struct WorkQueue;

struct ThreadWork {
	WorkQueue *queue;
	void (*fn)(void *param);
	void *param;
};

struct ThreadPool {
	Allocator allocator = current_allocator;
	List<Thread> threads;
	u32 thread_count = 0;
	u32 volatile initialized_thread_count = 0;
	u32 volatile dead_thread_count = 0;
	bool volatile running = false;
	bool volatile stopping = false;
	MutexQueue<ThreadWork> all_work;
};
bool try_do_work(ThreadPool *pool);

struct WorkQueue {
	ThreadPool *pool = 0;
	u32 volatile work_to_do = 0;
	bool important = false;
	inline void push(void (*fn)(void *param), void *param) {
		if (pool->thread_count) {
			ThreadWork work;
			work.fn = fn;
			work.param = param;
			work.queue = this;
			if (important) {
				pool->all_work.push_front_unordered(work);
			} else {
				pool->all_work.push(work);
			}
		} else {
			fn(param);
		}
	}
	template <class Fn, class ...Args>
	void push(Fn &&fn, Args &&...args) {
		if (pool->thread_count) {
			using Tuple = std::tuple<std::decay_t<Fn>, std::decay_t<Args>...>;
			auto fnParams = ALLOCATE(Tuple, pool->allocator);
			new(fnParams) Tuple(std::forward<Fn>(fn), std::forward<Args>(args)...);
			constexpr auto invokerProc = Detail::get_invoke<Tuple>(std::make_index_sequence<1 + sizeof...(Args)>{});
			atomic_add(work_to_do, 1);
			push((void (*)(void *))invokerProc, (void *)fnParams);
		} else {
			std::invoke(fn, std::forward<Args>(args)...);
		}
	}
	template <class Fn>
	WorkQueue &operator+=(Fn &&fn) {
		push(std::forward<Fn>(fn));
		return *this;
	}
	inline void wait_for_completion() {
		if (pool->thread_count) {
			loop_until([&] {
				try_do_work(pool);
				return work_to_do == 0;
			});
		}
	}

};

inline WorkQueue make_work_queue(ThreadPool &pool, bool important = false) {
	WorkQueue result = {};
	result.pool = &pool;
	result.important = important;
	return result;
}

inline void do_work(ThreadWork work) {
	work.fn(work.param);
	FREE(work.queue->pool->allocator, work.param);
	atomic_add(work.queue->work_to_do, (u32)-1);
}
inline bool try_do_work(ThreadPool *pool) {
	if (auto popped = pool->all_work.try_pop()) {
		do_work(popped.value);
		return true;
	}
	return false;
}
inline bool do_work(ThreadPool *pool) {
	ThreadWork work = {};
	loop_until([&] {
		if (pool->stopping) {
			return true;
		}
		if (auto popped = pool->all_work.try_pop()) {
			work = popped.value;
			return true;
		}
		return false;
	});
	if (work.fn) {
		do_work(work);
		return true;
	} else {
		return false;
	}
}

inline void default_thread_pool_proc(ThreadPool *pool) {
	atomic_add(pool->initialized_thread_count, 1);
	loop_until([&] { return pool->running || pool->stopping; });
	while (1) {
		if (!do_work(pool))
			break;
	}
	atomic_add(pool->dead_thread_count, 1);
}
template <class ThreadProc = decltype(default_thread_pool_proc)>
bool init_thread_pool(ThreadPool &pool, u32 thread_count, ThreadProc &&thread_proc = default_thread_pool_proc) {
	pool.initialized_thread_count = 0;
	pool.dead_thread_count = 0;
	pool.running = false;
	pool.stopping = false;
	pool.thread_count = thread_count;
	if (thread_count) {
		pool.threads.reserve(thread_count);
		
		struct StartParams {
			ThreadPool *pool;
			ThreadProc *proc;
		};
		StartParams params;
		params.pool = &pool;
		params.proc = std::addressof(thread_proc);
		
		auto start_proc = [](void *param) {
			StartParams *info = (StartParams *)param;
			(*info->proc)(info->pool);
		};

		for (u32 i = 0; i < thread_count; ++i) {
			auto thread = create_thread(start_proc, &params);
			if (!thread) {
				pool.stopping = true;
				for (auto t : pool.threads) {
					destroy_thread(t);
				}
				pool.threads.clear();
				return false;
			}
			pool.threads.add(thread);
		}

		loop_until([&] {
			return pool.initialized_thread_count == pool.thread_count;
		});
		pool.running = true;
	} else {
		pool.threads.clear();
	}

	return true;
}
inline void deinit_thread_pool(ThreadPool *pool, bool waitForThreads = true) {
	pool->stopping = true;
	if (waitForThreads) {
		loop_until([&] { return pool->dead_thread_count == pool->thread_count; });
	}
	pool->threads.clear();
}

} // namespace TL
#pragma warning(pop)
