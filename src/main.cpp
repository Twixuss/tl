#pragma warning(disable: 4711)
#pragma warning(disable: 4710)
#pragma warning(disable: 4464)
#define ASSERTION_FAILURE(cause, expr, ...) do{puts(cause);puts(expr);print(__VA_ARGS__);DEBUG_BREAK;}while(0)
#include "../include/tl/math.h"
#pragma warning(push, 0)
#pragma warning(disable: 5045)
#include <time.h>
#include <stdio.h>
#include <vector>
#define NOMINMAX
#include <Windows.h>
using namespace TL;

struct PerfTimer {
	inline PerfTimer() { reset(); }
	inline s64 getElapsedCounter() { return getCounter() - begin; }
	inline void reset() { begin = getCounter(); }

	inline static s64 const frequency = []() {
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		return freq.QuadPart;
	}();
	inline static s64 getCounter() {
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		return counter.QuadPart;
	}
	template <class Ret = f32> inline static Ret getSeconds(s64 elapsed) { return (Ret)elapsed / (Ret)frequency; }
	template <class Ret = f32> inline static Ret getMilliseconds(s64 elapsed) { return getSeconds<Ret>(elapsed * 1000); }
	template <class Ret = f32> inline static Ret getMicroseconds(s64 elapsed) { return getSeconds<Ret>(elapsed * 1000000); }
	template <class Ret = f32> inline static Ret getNanoseconds (s64 elapsed) { return getSeconds<Ret>(elapsed * 1000000000); }
	template <class Ret = f32> inline static Ret getSeconds(s64 begin, s64 end) { return getSeconds<Ret>(end - begin); }
	template <class Ret = f32> inline static Ret getMilliseconds(s64 begin, s64 end) { return getMilliseconds<Ret>(end - begin); }
	template <class Ret = f32> inline static Ret getMicroseconds(s64 begin, s64 end) { return getMicroseconds<Ret>(end - begin); }
	template <class Ret = f32> inline static Ret getNanoseconds (s64 begin, s64 end) { return getNanoseconds <Ret>(end - begin); }
	template <class Ret = f32> inline Ret getSeconds() { return getSeconds<Ret>(getElapsedCounter()); }
	template <class Ret = f32> inline Ret getMilliseconds() { return getMilliseconds<Ret>(getElapsedCounter()); }
	template <class Ret = f32> inline Ret getMicroseconds() { return getMicroseconds<Ret>(getElapsedCounter()); }
	template <class Ret = f32> inline Ret getNanoseconds() { return getNanoseconds<Ret>(getElapsedCounter()); }

private:
	s64 begin;
};

void (*_dummy)(void const *) = [](void const *) {};
template <class T>
void dummy(T const &v) { _dummy(&v); }

template<class ...T>
void print(T const &...vals) {
	printf(vals...);
}
void print(v2fx4 v) { printf("%.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f", v.s[0], v.s[1], v.s[2], v.s[3], v.s[4], v.s[5], v.s[6], v.s[7]); }
void print(v3fx4 v) { printf("%.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f", v.s[0], v.s[1], v.s[2], v.s[3], v.s[4], v.s[5], v.s[6], v.s[7], v.s[8], v.s[9], v.s[10], v.s[11]); }
void print(v4fx4 v) { printf("%.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f", v.s[0], v.s[1], v.s[2], v.s[3], v.s[4], v.s[5], v.s[6], v.s[7], v.s[8], v.s[9], v.s[10], v.s[11], v.s[12], v.s[13], v.s[14], v.s[15]); }
void print(v2fx8 v) { printf("%.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f", v.s[0], v.s[1], v.s[2], v.s[3], v.s[4], v.s[5], v.s[6], v.s[7], v.s[8], v.s[9], v.s[10], v.s[11], v.s[12], v.s[13], v.s[14], v.s[15]); }
void print(v3fx8 v) { printf("%.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f", v.s[0], v.s[1], v.s[2], v.s[3], v.s[4], v.s[5], v.s[6], v.s[7], v.s[8], v.s[9], v.s[10], v.s[11], v.s[12], v.s[13], v.s[14], v.s[15], v.s[16], v.s[17], v.s[18], v.s[19], v.s[20], v.s[21], v.s[22], v.s[23]); }
void print(v4fx8 v) { printf("%.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f %.0f", v.s[0], v.s[1], v.s[2], v.s[3], v.s[4], v.s[5], v.s[6], v.s[7], v.s[8], v.s[9], v.s[10], v.s[11], v.s[12], v.s[13], v.s[14], v.s[15], v.s[16], v.s[17], v.s[18], v.s[19], v.s[20], v.s[21], v.s[22], v.s[23], v.s[24], v.s[25], v.s[26], v.s[27], v.s[28], v.s[29], v.s[30], v.s[31]); }
void print(){}

#define A2	V2f(1, 2)
#define A3	V3f(1, 2, 3)
#define A4	V4f(1, 2, 3, 4)
#define B2	V2f(5, 6)
#define B3	V3f(5, 6, 7)
#define B4	V4f(5, 6, 7, 8)
#define A2i V2s(1, 2)
#define A3i V3s(1, 2, 3)
#define A4i V4s(1, 2, 3, 4)
#define B2i V2s(5, 6)
#define B3i V3s(5, 6, 7)
#define B4i V4s(5, 6, 7, 8)
template <class T, class U>
void test(T t, U u) {
	ASSERT(allTrue(t == u));
}
void test(f32 a, f32 b) {
	ASSERT(absolute(a-b) < 0.0001f);
}
b32x4 approxEqual(f32x4 a, f32x4 b) { return absolute(a - b) < FLT_EPSILON; }
b32x4 approxEqual(v2fx4 a, v2fx4 b) { 
	auto v = absolute(a - b);
	return v.x < FLT_EPSILON && v.y < FLT_EPSILON; 
}
f32 _a;
template<class T>
void raycastTest() {
	struct Random {
		s32 seed = 0;
		s32 s32() { return seed = randomize(seed); }
		u32 u32() { return (::u32)s32(); }
		f32 f32() { return (s32() / 256) * (1.0f / 8388608.f); }
		bool u1() { return u32() & 0x10; }
		s32x4 s32x4() { return {s32(), s32(), s32(), s32()}; }
		s32x8 s32x8() { return {s32(), s32(), s32(), s32(), s32(), s32(), s32(), s32()}; }
		u32x4 u32x4() { return (::u32x4)s32x4(); }
		u32x8 u32x8() { return (::u32x8)s32x8(); }
		f32x4 f32x4() { return (::f32x4)(s32x4() / 256) * (1.0f / 8388608.f); }
		f32x8 f32x8() { return (::f32x8)(s32x8() / 256) * (1.0f / 8388608.f); }
	} random;
	for (u32 j = 0; j < 1024 * 16; ++j) {
		mask32x<widthOf<T>> result{};
		T a4{};
		T b4{};
		T c4{};
		T d4{};
		T point4{}, normal4{};
		for (u32 i = 0; i < widthOf<T>; ++i) {
			v2f a = {random.f32(), random.f32()};
			v2f b = {random.f32(), random.f32()};
			v2f c = {random.f32(), random.f32()};
			v2f d = {random.f32(), random.f32()};
			v2f point, normal;
			result[i]			= raycastLine(a, b, c, d, point, normal) ? ~0u : 0u;
			a4[i]		= a;
			b4[i]		= b;
			c4[i]		= c;
			d4[i]		= d;
			point4[i]	= point;
			normal4[i] = normal;
		}
		a4		= pack(a4);
		b4		= pack(b4);
		c4		= pack(c4);
		d4		= pack(d4);
		point4	= pack(point4);
		normal4 = pack(normal4);

		T point4new, normal4new;
		auto resultNew = raycastLine(a4, b4, c4, d4, point4new, normal4new);
		auto pdiff = abs(point4new - point4);
		auto ndiff = abs(normal4new - normal4);
		test(allTrue(resultNew == result), true);
		test(allTrue((pdiff.x < FLT_EPSILON && pdiff.y < FLT_EPSILON) == result), true);
		test(allTrue((ndiff.x < FLT_EPSILON && ndiff.y < FLT_EPSILON) == result), true);
		_a = 1;
	}
	for (u32 j = 0; j < 1024 * 16; ++j) {
		mask32x<widthOf<T>> result{};
		T a4{};
		T b4{};
		T c4{};
		T d4{};
		T point4{}, normal4{};
		for (u32 i = 0; i < widthOf<T>; ++i) {
			v2f a = {random.f32(), random.f32()};
			v2f b = {random.f32(), random.f32()};
			v2f c = {random.f32(), random.f32()};
			v2f d = {random.f32(), random.f32()};
			v2f point, normal;
			result[i]			= raycastRect(a, b, c, d, point, normal) ? ~0u : 0u;
			a4[i]		= a;
			b4[i]		= b;
			c4[i]		= c;
			d4[i]		= d;
			point4[i]	= point;
			normal4[i] = normal;
		}
		a4		= pack(a4);
		b4		= pack(b4);
		c4		= pack(c4);
		d4		= pack(d4);
		point4	= pack(point4);
		normal4 = pack(normal4);

		T point4new, normal4new;
		auto resultNew = raycastRect(a4, b4, c4, d4, point4new, normal4new);
		auto pdiff = abs(point4new - point4);
		auto ndiff = abs(normal4new - normal4);
		test(allTrue(resultNew == result), true);
		test(allTrue(((pdiff.x < FLT_EPSILON) && (pdiff.y < FLT_EPSILON)) == result), true);
		test(allTrue(((ndiff.x < FLT_EPSILON) && (ndiff.y < FLT_EPSILON)) == result), true);
		_a = 1;
	}
}
template<class T>
void funTest() {
	T t{};
	t.data();
	t.s;
#define OP(op) t op t; t op##= t; t op 1; t op##= 1;
	if constexpr(std::is_same_v<RemoveQualifiers<decltype(t.s[0])>::Type, f32>) {
		OP(+)OP(-)OP(*)OP(/)
	} else if constexpr(std::is_same_v<RemoveQualifiers<decltype(t.s[0])>::Type, s32>) {
		OP(+)OP(-)OP(*)OP(/)
		OP(|)OP(&)OP(^)OP(%)
	} else if constexpr(std::is_same_v<RemoveQualifiers<decltype(t.s[0])>::Type, u32>) {
		OP(+)OP(-)OP(*)OP(/)
		OP(|)OP(&)OP(^)OP(%)
	} else {
		static_assert(false);	
	}
}
template<class T, class Fn>
void perfTest(char const* name, Fn&& op) {
	std::vector<T> fracArr(1024*1024);
	auto begin = __rdtsc();
	for(auto& v : fracArr) {
		op(v);
	}
	printf("%s time: %llu cy\n", name, (__rdtsc() - begin) / fracArr.size());
}
struct v4 {
	f32 x,y,z,w;
};
v4fx4 fff;
__m128 ta, tb;
void mathTest() {
	fff = pack(fff);

	ta = _mm_cmp_ps(ta, tb, _CMP_LE_OQ);

#if 0
	u32 seed = 0;
	u32 test = seed;
	u32 count = 0;
	do {
		test = randomize(test);
		++count;
		printf("%x\n", test);
		if(test == seed){
			if(count) {
				printf("Unique values: %u\n", count);
			}
			ASSERT(!count);
		}
	} while(count);
#endif

#define PACK_UNPACK(a, b) 											 \
	do {															 \
		auto packedA = pack(a);										 \
		auto unpackedB = unpack(b);									 \
		ASSERT(allTrue(packedA == b)); 								 \
		ASSERT(allTrue(unpackedB == a));							 \
	} while(0)

	PACK_UNPACK(V2fx4(0,1,2,3,4,5,6,7), V2fx4(0,2,4,6,1,3,5,7));
	PACK_UNPACK(V3fx4(0,1,2,3,4,5,6,7,8,9,10,11), V3fx4(0,3,6,9,1,4,7,10,2,5,8,11));
	PACK_UNPACK(V4fx4(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15), V4fx4(0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15));
	PACK_UNPACK(V2fx8(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15), V2fx8(0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15));
	PACK_UNPACK(V3fx8(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23), V3fx8(0,3,6,9,12,15,18,21,1,4,7,10,13,16,19,22,2,5,8,11,14,17,20,23));
	PACK_UNPACK(V4fx8(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31), V4fx8(0,4,8,12,16,20,24,28,1,5,9,13,17,21,25,29,2,6,10,14,18,22,26,30,3,7,11,15,19,23,27,31));

	f32 _[8]{99,99,99,99,99,99,99,99};
	
	f32 data[]{0,1,2,3,4,5,6,7};

	f32 __[8]{99,99,99,99,99,99,99,99};
	
	f32x4 fx4;
	gather(fx4, data, s32x4{0,1,2,3} * sizeof(f32)); ASSERT(allTrue(fx4 == F32x4(0,1,2,3)));
	gather(fx4, data, s32x4{1,2,3,4} * sizeof(f32)); ASSERT(allTrue(fx4 == F32x4(1,2,3,4)));
	gather(fx4, data, s32x4{2,3,4,5} * sizeof(f32)); ASSERT(allTrue(fx4 == F32x4(2,3,4,5)));
	gather(fx4, data, s32x4{3,4,5,6} * sizeof(f32)); ASSERT(allTrue(fx4 == F32x4(3,4,5,6)));
	gather(fx4, data, s32x4{4,5,6,7} * sizeof(f32)); ASSERT(allTrue(fx4 == F32x4(4,5,6,7)));

	f32x8 fx8;
	gather(fx8, data, s32x8{0,2,4,6,1,3,5,7} * sizeof(f32)); ASSERT(allTrue(fx8 == F32x8(0,2,4,6,1,3,5,7)));

	//TEST(+);
	//TEST(-);
	//TEST(*);
	//TEST(/);
	//TEST(%);
	//TEST(^);
	//TEST(|);
	//TEST(&);

	//perfTest<v4sx4>("mod", [](auto& v){ v %= magic; });
	//perfTest<v4sx8>("mod", [](auto& v){ v %= magic; });
	//raycastTest<v2fx4>();
	//raycastTest<v2fx8>();
	
	f32 arr[4]{0, 1, 2, 3};
	test(linearSample(arr, 0), 0);
	test(linearSample(arr, 0.125f), 0.5f);
	test(linearSample(arr, 0.25f), 1);
	test(linearSample(arr, 0.375f), 1.5f);
	test(linearSample(arr, 0.5f), 2);
	test(linearSample(arr, 0.625f), 2.5);
	test(linearSample(arr, 0.75f), 3);
	test(linearSample(arr, 0.875f), 1.5f);
	test(linearSample(arr, 1), 0);

	test(frac(-1.0f), 0.0f);
	test(frac(-0.1f), 0.9f);
	test(frac(0.0f), 0.0f);
	test(frac(0.3f), 0.3f);
	test(frac(1.0f), 0.0f);
	test(frac(1.6f), 0.6f);
	test(frac(2.9f), 0.9f);
	test(frac(3.2f), 0.2f);

	test(frac(-6, 5), 4);
	test(frac(-5, 5), 0);
	test(frac(-4, 5), 1);
	test(frac(-3, 5), 2);
	test(frac(-2, 5), 3);
	test(frac(-1, 5), 4);
	test(frac(0, 5), 0);
	test(frac(1, 5), 1);
	test(frac(2, 5), 2);
	test(frac(3, 5), 3);
	test(frac(4, 5), 4);
	test(frac(5, 5), 0);
	test(frac(6, 5), 1);

	test(floor(-6, 3), -6);
	test(floor(-5, 3), -6);
	test(floor(-4, 3), -6);
	test(floor(-3, 3), -3);
	test(floor(-2, 3), -3);
	test(floor(-1, 3), -3);
	test(floor(0, 3), 0);
	test(floor(1, 3), 0);
	test(floor(2, 3), 0);
	test(floor(3, 3), 3);
	test(floor(4, 3), 3);
	test(floor(5, 3), 3);
	test(floor(6, 3), 6);
	test(floor(7, 3), 6);
	test(floor(8, 3), 6);

	test(roundToInt(-1.0), -1);
	test(roundToInt(-0.6), -1);
	test(roundToInt(-0.5), -1);
	test(roundToInt(-0.4), 0);
	test(roundToInt(0.0), 0);
	test(roundToInt(0.4), 0);
	test(roundToInt(0.5), 1);
	test(roundToInt(0.6), 1);
	test(roundToInt(1.0), 1);

	test(roundToInt(A2 + 0.5f), A2i + 1);
	test(roundToInt(A3 + 0.5f), A3i + 1);
	test(roundToInt(A4 + 0.5f), A4i + 1);
	
	{
		b32x4 a = B32x4(0,1,0,0);
		b32x4 b = B32x4(0,1,0,1);
		test(a & b, B32x4(0,1,0,0));
		test(a | b, B32x4(0,1,0,1));
		test(a && b, B32x4(0,1,0,0));
		test(a || b, B32x4(0,1,0,1));
		test(a ^ b, B32x4(0,0,0,1));
		test((u32)a, 0b0010);
		test((u32)b, 0b1010);
	}
	{
		b32x8 a = B32x8(0,1,0,0,0,1,0,0);
		b32x8 b = B32x8(0,1,0,1,0,1,0,1);
		test(a & b,  B32x8(0,1,0,0,0,1,0,0));
		test(a | b,  B32x8(0,1,0,1,0,1,0,1));
		test(a && b, B32x8(0,1,0,0,0,1,0,0));
		test(a || b, B32x8(0,1,0,1,0,1,0,1));
		test(a ^ b,  B32x8(0,0,0,1,0,0,0,1));
		test((u32)a, 0b00100010);
		test((u32)b, 0b10101010);
	}
	
	static constexpr umm sinTableSize = 1024*1024;
	static auto sinTableData = [] {
		f32 *arr = allocate<f32, OsAllocator>(sinTableSize, 64);
		for (u32 i = 0; i < sinTableSize; ++i) {
			arr[i] = TL::sin((f32)i / sinTableSize * pi * 2);
		}
		return arr;
	}();
	auto sinTable = [](f32 angle) {
		static_assert(isPowerOf2(sinTableSize));
		static constexpr u32 mask = sinTableSize - 1;

		return sinTableData[(s32)(angle * (1 / (pi * 2)) * sinTableSize) & mask];
	};

	auto perfTest = [](auto &&fn) {
		f32 const maxx = 0xFFFFFF;

		u64 tmin = ~0;
		u64 tmax = 0;
		u64 ttotal = 0;

		for (f32 a = 0; a < maxx; ++a) {
			auto begin = __rdtsc();
			dummy(fn(a));
			auto time = __rdtsc() - begin;
			ttotal += time;
			tmin = min(tmin, time);
			tmax = max(tmax, time);
		}
		printf("Min: %llu, Max: %llu, Avg: %f\n", tmin, tmax, ttotal / maxx);
	};

	perfTest([](auto x) { return sinBhaskara(F32x8(x)); });
}
#include <thread>
#include <mutex>
#include "..\include\tl\thread.h"
int main() { 
	mathTest(); 
	system("pause");
}