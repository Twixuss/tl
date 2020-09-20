#include "../include/tl/tl.h"
using namespace TL;

#pragma warning(push, 0)
#include <stdio.h>
#include <assert.h>
#include <excpt.h>
#if COMPILER_GCC
#include <cxxabi.h>
#endif
#include <random>
#include <chrono>
#include <thread>
#include <typeinfo>
#pragma warning(pop)

using namespace TL;

void clearLine() {
	printf("\r                                                                                \r");
}

template <class Mask>
void _assert(Mask mask, char const *expression, u32 line) {
	printf("test %s at line %u... ", expression, line);
	if (!allTrue(mask)) {
		puts("\nAssertion failed");
		puts(expression);
		DEBUG_BREAK;
		exit(-1);
	}
	puts("ok");
}

template <class T>
auto equals(T a, T b) { return a == b; }
b64x2 equals(f64x2 a, f64x2 b) { return absolute(a - b) < 0.000001f; }
b32x4 equals(f32x4 a, f32x4 b) { return absolute(a - b) < 0.000001f; }
b64x4 equals(f64x4 a, f64x4 b) { return absolute(a - b) < 0.000001f; }
b32x8 equals(f32x8 a, f32x8 b) { return absolute(a - b) < 0.000001f; }

void print(b32x1 v) { printf("b32x1{%d}", v.m); }
void print(b32x4 v) { printf("b32x4{%d %d %d %d}", v[0], v[1], v[2], v[3]); }
void print(b32x8 v) { printf("b32x8{%d %d %d %d %d %d %d %d}", v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]); }
void print(b64x1 v) { printf("b64x1{%lld}", v.m); }
void print(b64x2 v) { printf("b64x2{%lld %lld}", v[0], v[1]); }
void print(b64x4 v) { printf("b64x4{%lld %lld %lld %lld}", v[0], v[1], v[2], v[3]); }

void print(f32 v) { printf("f32{%.20f}", v); }
void print(s32 v) { printf("s32{%d}", v); }
void print(u32 v) { printf("u32{%u}", v); }

void print(f64 v) { printf("f64{%.20f}", v); }
void print(s64 v) { printf("s64{%lld}", v); }
void print(u64 v) { printf("u64{%llu}", v); }

void print(f32x1 v) { print(v.m); }
void print(s32x1 v) { print(v.m); }
void print(u32x1 v) { print(v.m); }

void print(f64x1 v) { print(v.m); }
void print(s64x1 v) { print(v.m); }
void print(u64x1 v) { print(v.m); }

void print(f64x2 v) { printf("f64x2{%.20f %.20f}", v[0], v[1]); }
void print(s64x2 v) { printf("s64x2{%lld %lld}", v[0], v[1]); }

void print(f32x4 v) { printf("f32x4{%.20f %.20f %.20f %.20f}", v[0], v[1], v[2], v[3]); }
void print(s32x4 v) { printf("s32x4{%d %d %d %d}", v[0], v[1], v[2], v[3]); }
void print(u32x4 v) { printf("u32x4{%u %u %u %u}", v[0], v[1], v[2], v[3]); }

void print(f64x4 v) { printf("f64x4{%.20f %.20f %.20f %.20f}", v[0], v[1], v[2], v[3]); }
//void print(s64x4 v) { printf("s64x4{%lld %lld %lld %lld}", v[0], v[1], v[2], v[3]); }
//void print(u64x4 v) { printf("u64x4{%llu %llu %llu %llu}", v[0], v[1], v[2], v[3]); }

void print(f32x8 v) { printf("f32x8{%.20f %.20f %.20f %.20f %.20f %.20f %.20f %.20f}", v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]); }
void print(s32x8 v) { printf("s32x8{%d %d %d %d %d %d %d %d}", v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]); }
void print(u32x8 v) { printf("u32x8{%u %u %u %u %u %u %u %u}", v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]); }

template <class T, class ...Args>
void _assertEqual(T result, T check, char const *expression, u32 line, char const *message = 0, Args const &...args) {
	printf("%u: %s", line, expression);
	auto mask = equals(result, check);
	if (!allTrue(mask)) {
		puts("\nAssertion failed");
		puts(expression);
		printf("result: "); print(result); puts("");
		printf("check: "); print(check); puts("");
		printf("mask: "); print(mask); puts("");
		if (message) {
			printf("message: ");
#pragma warning(suppress: 4774)
			printf(message, args...);
		}
		DEBUG_BREAK;
		exit(-1);
	}
	clearLine();
}

inline constexpr void noop() {}

template <class T>
void printType() {
#if COMPILER_GCC
	int status;
	auto realName = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
	puts(realName);
	free(realName);
#else
	puts(typeid(T).name());
#endif
}

#undef assert
#define assert(...) _assert(__VA_ARGS__, #__VA_ARGS__, __LINE__)
#define assertEqual(a, b, ...) _assertEqual(a, b, #a " == " #b, __LINE__, __VA_ARGS__)

template <bool isMask, class T, class U>
void set(T &dst, U src) {
	if constexpr (isMask)
		dst = TOMASK(src);
	else
		dst = src;
}

#define TEST_UNARY(op) \
	do {\
		char const *opStr = #op;\
		printf("%s a ... ", opStr);\
		auto result = op a;\
		decltype(result) check;\
		for (u32 i = 0; i < width; ++i) {\
			set<isMask>(check.s[i], op data[i]);\
		}\
		assertEqual(result, check, "%s a == %s data[i]",opStr,opStr);\
		noop();\
	} while(0)

#define TEST_BINARY(op) \
	do {\
		char const *opStr = #op;\
		printf("a %s b ... ", opStr);\
		auto result = a op b;\
		using Result = decltype(result);\
		Result check;\
		constexpr bool resultIsMask = isSame<Result::Pack, Result::Mask>; \
		for (u32 i = 0; i < width; ++i) {\
			set<resultIsMask>(check.s[i], data[i] op data[i+width]);\
		}\
		assertEqual(result, check, "a %s b == data[i] %s data[i+w]",opStr,opStr);\
		result = a op scalar;\
		for (u32 i = 0; i < width; ++i) {\
			set<resultIsMask>(check.s[i], data[i] op scalar);\
		}\
		assertEqual(result, check, "a %s scalar == data[i] %s scalar",opStr,opStr);\
		result = scalar op b;\
		for (u32 i = 0; i < width; ++i) {\
			set<resultIsMask>(check.s[i], scalar op data[i+width]);\
		}\
		assertEqual(result, check, "scalar %s b == scalar %s data[i+w]",opStr,opStr);\
		noop();\
		printf("\r");\
	} while(0)

#define TEST_PREFIX							   \
	printType<T>();                            \
	using Scalar = typename T::Scalar;		   \
	using Pack = typename T::Pack;			   \
	using Mask = typename T::Mask;			   \
	constexpr bool isMask = isSame<Mask, Pack>;\
	constexpr u32 width = widthOf<Pack>;	   \
	alignas(64) Scalar data[width*2];		   \
	for (u32 i = 0; i < _countof(data); ++i) { \
		set<isMask>(data[i], (Scalar)((i == 2) ? (~0) : (i - 2)));\
	}										   \
	alignas(64) Scalar scalar;                 \
	set<isMask>(scalar, (Scalar)1);\
	alignas(64) Pack a = *(Pack *)(data + 0);	   \
	alignas(64) Pack b = *(Pack *)(data + width); \
	for (u32 i = 0; i < width; ++i) {		   \
		assertEqual(a[i], data[i], "a == data");			   \
		assertEqual(b[i], data[i + width], "b == data");	   \
	}

template <class T>
void test_bool() {
	static_assert(widthOf<T> != 0);
	
	ASSERT(allFalse((T)falseMask));
	ASSERT(allTrue((T)trueMask));
	
	ASSERT(!anyTrue((T)falseMask));
	ASSERT(!anyFalse((T)trueMask));

	ASSERT(compressMask((T)falseMask) == 0);
	ASSERT(compressMask((T)trueMask) == ((1 << widthOf<T>) - 1));
	
	ASSERT(allTrue(andNot((T)trueMask, (T)falseMask)));

	TEST_PREFIX;
	TEST_UNARY(~);
	TEST_UNARY(!);
	TEST_BINARY(^);
	TEST_BINARY(&);
	TEST_BINARY(|);
	TEST_BINARY(&&);
	TEST_BINARY(||);
	TEST_BINARY(==);
	TEST_BINARY(!=);
}

template <class T>
void test_float() {
	static_assert(widthOf<T> != 0);
	TEST_PREFIX;
	TEST_UNARY(-);
	TEST_BINARY(+);
	TEST_BINARY(-);
	TEST_BINARY(*);
	TEST_BINARY(/);
	TEST_BINARY(>);
	TEST_BINARY(<);
	TEST_BINARY(>=);
	TEST_BINARY(<=);
	TEST_BINARY(==);
	TEST_BINARY(!=);
}

template <class T>
void test_int() {
	static_assert(widthOf<T> != 0);
	TEST_PREFIX;
	TEST_UNARY(!);
	TEST_UNARY(~);
#pragma warning(suppress: 4146)
	TEST_UNARY(-);
	TEST_BINARY(+);
	TEST_BINARY(-);
	TEST_BINARY(*);
	TEST_BINARY(/);
	TEST_BINARY(%);
	TEST_BINARY(^);
	TEST_BINARY(&);
	TEST_BINARY(|);
	TEST_BINARY(<<);
	TEST_BINARY(>>);
	TEST_BINARY(>);
	TEST_BINARY(<);
	TEST_BINARY(>=);
	TEST_BINARY(<=);
	TEST_BINARY(==);
	TEST_BINARY(!=);
}

void string_test() {
	printf("%s ... ", "string_test");
	StringBuilder<OsAllocator, 16> builder;
	memset(builder.first.buffer, 'A', 16);
	for (u32 i = 0; i < 8; ++i)
		builder.append('B');

	ASSERT(memcmp(builder.first.buffer, "BBBBBBBBAAAAAAAA", 16) == 0);
	ASSERT(&builder.first == builder.last);
	ASSERT(&builder.first == builder.allocLast);

	builder.ensureConsecutiveSpace(16);
	memset(builder.last->buffer, 'C', 16);
	for (u32 i = 0; i < 8; ++i)
		builder.append('D');
	
	ASSERT(builder.first.next == builder.last);
	ASSERT(builder.first.next == builder.allocLast);
	ASSERT(memcmp(builder.last->buffer, "DDDDDDDDCCCCCCCC", 16) == 0);

	puts("ok");
}
void math_test() {
	printf("%s ... ", "math_test");
#ifdef TEST_SSE
	static_assert(!ARCH_AVX);
	static_assert(!ARCH_AVX2);
	static_assert(!ARCH_AVX512F);
	static_assert(!ARCH_AVX512DQ);
#endif
#ifdef TEST_AVX
	static_assert(ARCH_AVX);
	static_assert(!ARCH_AVX2);
	static_assert(!ARCH_AVX512F);
	static_assert(!ARCH_AVX512DQ);
#endif
#ifdef TEST_AVX2
	static_assert(ARCH_AVX);
	static_assert(ARCH_AVX2);
	static_assert(!ARCH_AVX512F);
	static_assert(!ARCH_AVX512DQ);
#endif
	
#define CONx1(f32x1, F32x1) \
	assertEqual((f32x1{1}), F32x1(1));

#define CONx2(f32x4, F32x4) \
	assertEqual((f32x4{1,1}), F32x4(1)); \
	assertEqual((f32x4{1,2}), F32x4(1,2)); \

#define CONx4(f32x4, F32x4) \
	assertEqual((f32x4{1,1,1,1}), F32x4(1)); \
	assertEqual((f32x4{1,2,3,4}), F32x4(1,2,3,4)); \

#define CONx8(f32x4, F32x4) \
	assertEqual((f32x4{1,1,1,1,1,1,1,1}), F32x4(1)); \
	assertEqual((f32x4{1,2,3,4,5,6,7,8}), F32x4(1,2,3,4,5,6,7,8)); \

	CONx1(f32x1, F32x1);
	CONx1(s32x1, S32x1);
	CONx1(u32x1, U32x1);

	CONx1(f64x1, F64x1);
	CONx1(s64x1, S64x1);
	CONx1(u64x1, U64x1);

	CONx2(f64x2, F64x2);
	//CONx2(s64x2, S64x2);
	//CONx2(u64x2, U64x2);

	CONx4(f32x4, F32x4);
	CONx4(s32x4, S32x4);
	CONx4(u32x4, U32x4);
	CONx4(f64x4, F64x4);
	//CON32x4(s64x4, S64x4);
	//CON32x4(u64x4, U64x4);

	CONx8(f32x8, F32x8);
	CONx8(s32x8, S32x8);
	CONx8(u32x8, U32x8);
	//CONx8(f64x8, F64x8);
	//CONx8(s64x8, S64x8);
	//CONx8(u64x8, U64x8);

	static_assert(isSame<b32x1::Scalar, b32>);
	static_assert(isSame<b32x4::Scalar, b32>);
	static_assert(isSame<b32x8::Scalar, b32>);
	static_assert(isSame<b64x1::Scalar, b64>);
	static_assert(isSame<b64x2::Scalar, b64>);
	static_assert(isSame<b64x4::Scalar, b64>);
	
	test_bool<b32x1>();
	test_bool<b32x4>();
	test_bool<b32x8>();

	test_bool<b64x1>();
	test_bool<b64x2>();
	test_bool<b64x4>();
	
	test_float<f32x1>();
	test_float<f32x4>();
	test_float<f32x8>();

	test_float<f64x1>();
	test_float<f64x2>();
	test_float<f64x4>();
	
	test_int<s32x1>();
	test_int<s32x4>();
	test_int<s32x8>();

	test_int<s64x1>();
	test_int<s64x2>();
	//test_int<s64x4>();

	test_int<u32x1>();
	test_int<u32x4>();
	test_int<u32x8>();
	
	test_int<u64x1>();
	//test_int<u64x2>();
	//test_int<u64x4>();
}




template <class T>
void bubbleSort(Span<T> span) {
	if (span.size() < 2)
		return;

	bool doSwap = true;

	while (doSwap) {
		doSwap = false;
		for (auto cursor = span.begin() + 1; 
			 cursor != span.end(); 
			 ++cursor)
		{
			if (cursor[0] < cursor[-1]) {
				std::swap(cursor[0], cursor[-1]);
				doSwap = true;
			}
		}
	}
}
// Fucking... nope, FUCKING FUCKING FUCKING FUCKING FUCKING c++!!!
template <template <class T, class ...Args> class List, class T, class ...Args> void bubbleSort(List<T, Args...> &list) { bubbleSort((Span<T>)list); }

template <class T, class Predicate>
void quickSort(Span<T> span, Predicate &&predicate) {
	if (span.size() < 2)
		return;

	auto partition = [&](T *begin, T *end) {
		//T mid = *midpoint(begin, end - 1);
		T mid = midpoint(begin[0], end[-1]);
		//T mid = (begin[0] + end[-1] + *midpoint(begin, end - 1)) / 3;
		--begin;
		for (;;) {
			while(predicate(*++begin, mid)) {}
			while(predicate(mid, *--end)) {}
			if (begin >= end)
				return end + 1;
			std::swap(*begin, *end);
		}
	};

	T *p = partition(span.begin(), span.end());
	quickSort(Span<T>{span.begin(), p});
	quickSort(Span<T>{p, span.end()});
}
template <class T>
void quickSort(Span<T> span) {
	quickSort(span, [](T &a, T &b) { return a < b; });
}

// Fucking... nope, FUCKING FUCKING FUCKING FUCKING FUCKING c++!!!
template <template <class T, class ...Args> class List, class T, class ...Args>
void quickSort(List<T, Args...> &list) {
	quickSort((Span<T>)list);
}
template <template <class T, class ...Args> class List, class T, class ...Args, class Predicate>
void quickSort(List<T, Args...> &list, Predicate &&predicate) {
	quickSort((Span<T>)list, predicate);
}

template<typename T>
void mergeSort(Span<T> span) {
	List<T> temp;
	temp.reserve(span.size());

	for (umm blockSize = 1; blockSize < span.size(); blockSize *= 2) {
		for (umm block = 0; block < span.size() - blockSize; block += 2 * blockSize) {
			umm left = 0;
			umm right = 0;
			umm begin = block;
			umm mid = block + blockSize;
			umm end = min(block + 2 * blockSize, span.size());
			umm size = end - begin;

			while (begin + left < mid && mid + right < end) {
				if (span[begin + left] < span[mid + right]) {
					temp.push_back(span[begin + left]);
					left += 1;
				} else {
					temp.push_back(span[mid + right]);
					right += 1;
				}
			}
			while (begin + left < mid) {
				temp.push_back(span[begin + left]);
				left += 1;
			}
			while (mid + right < end) {
				temp.push_back(span[mid + right]);
				right += 1;
			}
			for (umm i = 0; i < size; i++) {
				span[begin + i] = temp[i];
			}
			temp.clear();
		}
	}
}

#if 0

template<typename T>
void mergeSort(Span<T> span, Allocator al = osAllocator) {
	Span<T> temp {
		al.allocate<T>(span.size()),
		span.size()
	};
	DEFER { al.deallocate(temp.data()); }

	Span<T> *src = &span;
	Span<T> *dst = &temp;

	for (umm blockSize = 1; blockSize < src->size(); blockSize *= 2) {
		for (umm block = 0; block < src->size() - blockSize; block += 2 * blockSize) {
			umm left = 0;
			umm right = 0;
			umm begin = block;
			umm mid = block + blockSize;
			umm end = min(block + 2 * blockSize, src->size());
			umm size = end - begin;

			while (begin + left < mid && mid + right < end) {
				if (src->at(begin + left) < src->at(mid + right)) {
					dst->at(left + right) = std::move(src->at(begin + left));
					left += 1;
				} else {
					dst->at(left + right) = std::move(src->at(mid + right));
					right += 1;
				}
			}
			while (begin + left < mid) {
				dst->at(left + right) = std::move(src->at(begin + left));
				left += 1;
			}
			while (mid + right < end) {
				dst->at(left + right) = std::move(src->at(mid + right));
				right += 1;
			}
			//for (umm i = 0; i < size; i++) {
			//	src[begin + i] = dst[i];
			//}
			std::swap(src, dst);
		}
	}
	if (dst == &span) {
		for (umm i = 0; i < span.size(); i++) {
			span[i] = std::move(dst->at(i));
		}
	}
}

#endif

// Fucking... nope, FUCKING FUCKING FUCKING FUCKING FUCKING c++!!!
template <template <class T, class ...Args> class List, class T, class ...Args> void mergeSort(List<T, Args...> &list) { mergeSort((Span<T>)list); }

void radixSort(Span<u32> span) {
	List<u32> temp;
	temp.resize(span.size());

	Span<u32> src = span;
	Span<u32> dst = temp;

	u32 mask = 0xFF;
	u32 shift = 0;

	for (u32 r = 0; r < 4; ++r) {
		u32 counters[256]{};
		for (umm i = 0; i < src.size(); ++i) {
			++counters[(src[i] & mask) >> shift];
		}
		for (u32 i = 1; i < 256; ++i) {
			counters[i] += counters[i - 1];
		}
		for (auto it = src.end() - 1; it >= src.begin(); --it) {
			dst[--counters[(*it & mask) >> shift]] = *it;
		}
		mask <<= 8;
		shift += 8;
		std::swap(src, dst);
	}
}

u32 xorshift(u32 &x) {
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return x;
}

struct Timer {
	char const *name;
	List<u32> *origList;
	std::thread thread;

	Timer(char const *name, List<u32> *origList) : origList(origList) {
		this->name = name;
	}
	Timer(const Timer &) = delete;
	Timer &operator=(const Timer &) = delete;

	template <class Fn>
	void operator+=(Fn &&fn) {
		thread = std::thread([&] {
			StringBuilder<> builder;
			builder.appendFormat("{}:\n", name);
			for (u32 size = 2; size <= origList->size(); size *= 2) {
				List<u32> origList2;
				origList2.resize(size);
				memcpy(origList2.data(), origList->data(), size * sizeof(origList2[0]));

				u64 minTime = (u64)~0;
				for (u32 i = 0; i < 0x100; ++i) {
					List<u32> list = origList2;
					auto begin = std::chrono::high_resolution_clock::now();
					fn(list);
					minTime = min(minTime, (u64)(std::chrono::high_resolution_clock::now() - begin).count());
				}
				if (minTime)
					builder.appendFormat("({},{}),", log2(size), log2(minTime));
				else
					builder.appendFormat("({},0),", log2(size));
			}
			puts(builder.getTerminated().data());
		});
	}
};

#define TIMER(name) Timer CONCAT(_timer_, __LINE__)(name, &origList); DEFER { CONCAT(_timer_, __LINE__).thread.join(); }; CONCAT(_timer_, __LINE__) += [&](auto &list)

void sort_test() {
	List<u32> origList;
	origList.resize(0x1000);
	u32 const scale = 0x100000000 / origList.size();
	for (u32 i = 0; i < origList.size(); ++i)
		origList[i] = i * scale;
	std::shuffle(origList.begin(), origList.end(), std::mt19937{std::random_device{}()});

	List<u32> list;
	
#define SORT_TEST(name)						  \
	{										  \
		printf("%s ... ", #name);             \
		list = origList;				      \
		name(list);                           \
		for (u32 i = 0; i < list.size(); ++i) \
			ASSERT(list[i] == i * scale);	  \
		puts("ok");                           \
	}										  \

	SORT_TEST(bubbleSort);
	SORT_TEST(quickSort);
	SORT_TEST(mergeSort);
	SORT_TEST(radixSort);
}
void sort_perf() {
	List<u32> origList;
	origList.resize(0x100000);
	for (u32 i = 0; i < origList.size(); ++i)
		origList[i] = i;
	std::shuffle(origList.begin(), origList.end(), std::mt19937{std::random_device{}()});
	
	//TIMER("std::sort") { std::sort(list.begin(), list.end()); };
	//TIMER("quickSort") { quickSort(list); };
	//TIMER("mergeSort") { mergeSort(list); };
}

void common_test() {
	static_assert(TL::midpoint(0, 0) == 0);
	static_assert(TL::midpoint(0, 1) == 0);
	static_assert(TL::midpoint(0, 2) == 1);
	static_assert(TL::midpoint(0, 3) == 1);
	static_assert(TL::midpoint(0, 4) == 2);
	
	static_assert(TL::midpoint(1, 0) == 0);
	static_assert(TL::midpoint(1, 1) == 1);
	static_assert(TL::midpoint(1, 2) == 1);
	static_assert(TL::midpoint(1, 3) == 2);
	static_assert(TL::midpoint(1, 4) == 2);
	
	static_assert(TL::midpoint((u32)0xFFFFFFFF, (u32)0) == (u32)0x7FFFFFFF);
	static_assert(TL::midpoint((u32)0, (u32)0xFFFFFFFF) == (u32)0x7FFFFFFF);
	static_assert(TL::midpoint((u32)0xFFFFFFFD, (u32)0xFFFFFFFF) == (u32)0xFFFFFFFE);
	static_assert(TL::midpoint((u32)0xFFFFFFFF, (u32)0xFFFFFFFD) == (u32)0xFFFFFFFE);
	static_assert(TL::midpoint((s32)0xFFFFFFFD, (s32)0xFFFFFFFF) == (s32)0xFFFFFFFE);
	static_assert(TL::midpoint((s32)0xFFFFFFFF, (s32)0xFFFFFFFD) == (s32)0xFFFFFFFE);
}

int main() {
	common_test();
	sort_test();
	//sort_perf();

	string_test();

	// TODO: fix this shit
	 math_test();
}