#pragma once
#include "common.h"
#include "array.h"

#if COMPILER_MSVC
#pragma warning(push, 0)
#endif

#include <float.h>
#include <math.h>
#include <memory.h>
#include <utility>

#if COMPILER_MSVC
#pragma warning(pop)
#endif


#if COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 5045) // spectre
#pragma warning(disable : 4146) // unsigned unary minus
#pragma warning(disable : 4201) // unnamed struct
#pragma warning(disable : 4820) // padding
#endif

namespace TL {
/*
#define s32x8_eq(a, b) _mm256_castsi256_ps(_mm256_cmpeq_epi32(_mm256_castps_si256(a), _mm256_castps_si256(b)))
#define s32x8_gt(a, b) _mm256_castsi256_ps(_mm256_cmpgt_epi32(_mm256_castps_si256(a), _mm256_castps_si256(b)))
#define s32x8_lt(a, b) s32x8_gt(b, a)
#define s32x8_le(a, b) m32_not(s32x8_gt(a, b))
#define s32x8_ge(a, b) m32_not(s32x8_lt(a, b))
#define s32x8_ne(a, b) m32_not(_mm256_cmpeq_epi32(b, a))

#define u32x8_eq s32x8_eq
#define u32x8_ne s32x8_ne
#define u32x8_gt(a, b) s32x8_gt(_mm256_xor_pi(a, _mm256_set1_epi32(0x80000000)), _mm256_xor_pi(b, _mm256_set1_epi32(0x80000000)))
#define u32x8_lt(a, b) u32x8_gt(b, a)
#define u32x8_le(a, b) m32_not(u32x8_gt(a, b))
#define u32x8_ge(a, b) m32_not(u32x8_lt(a, b))

#define _mm256_slliv_epi32(a, b) _mm256_setr_epi32(a << _mm256_extract_epi32(b, 0), a << _mm256_extract_epi32(b, 1), a << _mm256_extract_epi32(b, 2), a << _mm256_extract_epi32(b, 3), a << _mm256_extract_epi32(b, 4), a << _mm256_extract_epi32(b, 5), a << _mm256_extract_epi32(b, 6), a << _mm256_extract_epi32(b, 7))
#define _mm256_sraiv_epi32(a, b) _mm256_setr_epi32(a >> _mm256_extract_epi32(b, 0), a >> _mm256_extract_epi32(b, 1), a >> _mm256_extract_epi32(b, 2), a >> _mm256_extract_epi32(b, 3), a >> _mm256_extract_epi32(b, 4), a >> _mm256_extract_epi32(b, 5), a >> _mm256_extract_epi32(b, 6), a >> _mm256_extract_epi32(b, 7))
#define _mm256_srliv_epi32(a, b) _mm256_setr_epi32((u32)a >> (u32)_mm256_extract_epi32(b, 0), (u32)a >> (u32)_mm256_extract_epi32(b, 1), (u32)a >> (u32)_mm256_extract_epi32(b, 2), (u32)a >> (u32)_mm256_extract_epi32(b, 3), (u32)a >> (u32)_mm256_extract_epi32(b, 4), (u32)a >> (u32)_mm256_extract_epi32(b, 5), (u32)a >> (u32)_mm256_extract_epi32(b, 6), (u32)a >> (u32)_mm256_extract_epi32(b, 7))

#if ARCH_AVX

#define m32_xor _mm256_xor_ps
#define m32_and _mm256_and_ps
#define m32_or  _mm256_or_ps

#define combine_m16(a, b) _mm256_setr_m128(a, b)
#define m32_get_half(a, n) _mm256_extractf128_ps(a, n)
#define m32_get_lower(a) _mm256_extractf128_ps(a, 0)
#define m32_get_upper(a) _mm256_extractf128_ps(a, 1)

#define f32x8_set _mm256_setr_ps
#define s32x8_set(a,b,c,d,e,f,g,h) _mm256_castsi256_ps(_mm256_setr_epi32(a,b,c,d,e,f,g,h))
#define u32x8_set _mm256_setr_epi32

#define f32x8_set1(a) _mm256_set1_ps(a)
#define s32x8_set1(a) _mm256_castsi256_ps(_mm256_set1_epi32(a))
#define u32x8_set1(v) _mm256_castsi256_ps(_mm256_set1_epi32((s32)v))

#define f64x4_set1 _mm256_set1_pd
#define s64x4_set1 _mm256_set1_epi64x
#define u64x4_set1(v) _mm256_set1_epi64x((s32)v)

#define s32x8_get _mm256_extract_epi32
#define u32x8_get(v, i) ((u32)_mm256_extract_epi32(v, i))

#define f32x8_add(a, b) _mm256_add_ps(a, b)
#define f32x8_sub(a, b) _mm256_sub_ps(a, b)
#define f32x8_mul(a, b) _mm256_mul_ps(a, b)
#define f32x8_div(a, b) _mm256_div_ps(a, b)

#define f32x8_reciprocal(a) _mm256_rcp_ps(a)

#define f32x8_and_not(a, b) _mm256_andnot_ps(b, a)

#define b32x8_get_mask(a)  _mm256_movemask_ps(a.ps)
#define b64x4_get_mask(a)  _mm256_movemask_pd(a.pd)

#define b64x4_all_true(v) (b64x4_get_mask(v) == 0xF)
#define b32x8_all_true(v) (b32x8_get_mask(v) == 0xFF)
#define b64x4_all_false(v) (b64x4_get_mask(v) == 0)
#define b32x8_all_false(v) (b32x8_get_mask(v) == 0)

#define b64x4_any_false(v) (b64x4_get_mask(v) != 0xF)
#define b32x8_any_false(v) (b32x8_get_mask(v) != 0xFF)
#define b64x4_any_true(v) (b64x4_get_mask(v) != 0)
#define b32x8_any_true(v) (b32x8_get_mask(v) != 0)

#define select_32x8(mask, a, b) _mm256_blendv_ps(b, a, mask)

#else
#define b32x8_get_mask(a)  ((_mm_movemask_ps(a.lo.ps) << 4) | _mm_movemask_ps(a.hi.ps))
#define b64x4_get_mask(a)  ((_mm_movemask_pd(a.lo.pd) << 2) | _mm_movemask_pd(a.hi.pd))

#endif

#if ARCH_AVX2
#define s32x8_to_f32x8(a) _mm256_cvtepi32_ps(_mm256_castps_si256(a))
#define u32x8_to_f32x8(a) _mm256_cvtepi32_ps(_mm256_castps_si256(a))

#endif

#define f32x4_abs(v) and_pi_16(v, s32x4_set1(max_value<s32>))
#define f64x2_abs(v) and_pi_16(v, s64x2_set1(max_value<s64>))

#define f32x8_abs(v) _mm256_and_ps(v, _mm256_castsi256_ps(s32x8_set1(max_value<s32>)))
#define f64x4_abs(v) _mm256_and_ps(v, _mm256_castsi256_ps(s64x4_set1(max_value<s64>)))

#define i32x4_sli _mm_slli_epi32
#define s32x4_sri _mm_srai_epi32
#define u32x4_sri _mm_srli_epi32
#define i32x4_sil(a, b) i32x4_sl(s32x4_set1(a), b)
#define s32x4_sir(a, b) s32x4_sr(s32x4_set1(a), b)
#define u32x4_sir(a, b) u32x4_sr(u32x4_set1(a), b)

#if ARCH_AVX2
#define i32x8_sli _mm256_slli_epi32
#define s32x8_sri _mm256_srai_epi32
#define u32x8_sri _mm256_srli_epi32

#define b256_cmpeq(a, b) _mm256_castsi256_ps(_mm256_cmpeq_epi8(_mm256_castps_pi(a), _mm256_castps_pi(b)))
#define i32x4_sl _mm_sllv_epi32
#define s32x4_sr _mm_srav_epi32
#define u32x4_sr _mm_srlv_epi32
#define i64x2_sl _mm_sllv_epi64
#define u64x2_sr _mm_srlv_epi64
#define i32x8_sl _mm256_sllv_epi32
#define s32x8_sr _mm256_srav_epi32
#define u32x8_sr _mm256_srlv_epi32
#define i32x8_sli _mm256_slli_epi32
#define s32x8_sri _mm256_srai_epi32
#define u32x8_sri _mm256_srli_epi32
#define i32x8_sil(a, b) i32x8_sl(s32x8_set1(a), b)
#define s32x8_sir(a, b) s32x8_sr(s32x8_set1(a), b)
#define u32x8_sir(a, b) u32x8_sr(u32x8_set1(a), b)
#else
#define b256_cmpeq(a, b) (m32_not_ps(_mm256_xor_ps(a, b)))
#define i32x4_sl(a, b) s32x4_set(s32x4_get(a, 0) << s32x4_get(b, 0), s32x4_get(a, 1) << s32x4_get(b, 1), s32x4_get(a, 2) << s32x4_get(b, 2), s32x4_get(a, 3) << s32x4_get(b, 3))
#define s32x4_sr(a, b) s32x4_set(s32x4_get(a, 0) >> s32x4_get(b, 0), s32x4_get(a, 1) >> s32x4_get(b, 1), s32x4_get(a, 2) >> s32x4_get(b, 2), s32x4_get(a, 3) >> s32x4_get(b, 3))
#define u32x4_sr(a, b) s32x4_set(u32x4_get(a, 0) >> u32x4_get(b, 0), u32x4_get(a, 1) >> u32x4_get(b, 1), u32x4_get(a, 2) >> u32x4_get(b, 2), u32x4_get(a, 3) >> u32x4_get(b, 3))
#define i64x2_sl(a, b) s64x2_set(s64x2_get(a, 0) << s64x2_get(b, 0), s64x2_get(a, 1) << s64x2_get(b, 1))
#define u64x2_sr(a, b) s64x2_set(u64x2_get(a, 0) >> u64x2_get(b, 0), u64x2_get(a, 1) >> u64x2_get(b, 1))
#define i32x8_sl(a, b) _mm256_setr_epi32(s32x8_get(a, 0) << s32x8_get(b, 0), s32x8_get(a, 1) << s32x8_get(b, 1), s32x8_get(a, 2) << s32x8_get(b, 2), s32x8_get(a, 3) << s32x8_get(b, 3), s32x8_get(a, 4) << s32x8_get(b, 4), s32x8_get(a, 5) << s32x8_get(b, 5), s32x8_get(a, 6) << s32x8_get(b, 6), s32x8_get(a, 7) << s32x8_get(b, 7))
#define s32x8_sr(a, b) _mm256_setr_epi32(s32x8_get(a, 0) >> s32x8_get(b, 0), s32x8_get(a, 1) >> s32x8_get(b, 1), s32x8_get(a, 2) >> s32x8_get(b, 2), s32x8_get(a, 3) >> s32x8_get(b, 3), s32x8_get(a, 4) >> s32x8_get(b, 4), s32x8_get(a, 5) >> s32x8_get(b, 5), s32x8_get(a, 6) >> s32x8_get(b, 6), s32x8_get(a, 7) >> s32x8_get(b, 7))
#define u32x8_sr(a, b) _mm256_setr_epi32(u32x8_get(a, 0) >> u32x8_get(b, 0), u32x8_get(a, 1) >> u32x8_get(b, 1), u32x8_get(a, 2) >> u32x8_get(b, 2), u32x8_get(a, 3) >> u32x8_get(b, 3), u32x8_get(a, 4) >> u32x8_get(b, 4), u32x8_get(a, 5) >> u32x8_get(b, 5), u32x8_get(a, 6) >> u32x8_get(b, 6), u32x8_get(a, 7) >> u32x8_get(b, 7))
#endif

#if ARCH_AVX512DQ && ARCH_AVX512VL
#define s64x2_mul _mm_mullo_epi64
#else
#define s64x2_mul(a, b) s64x2_set(s64x2_get(a, 0) * s64x2_get(b, 0), s64x2_get(a, 1) * s64x2_get(b, 1))
#endif

#if ARCH_AVX512F && ARCH_AVX512VL
#define s64x2_sr _mm_srav_epi64
#define i64x2_sli _mm_slli_epi64
#define s64x2_sri _mm_srai_epi64
#define u64x2_sri _mm_srli_epi64
#define i64x2_sil(a, b) i64x2_sl(s64x2_set1(a), b)
#define s64x2_sir(a, b) s64x2_sr(s64x2_set1(a), b)
#define u64x2_sir(a, b) u64x2_sr(u64x2_set1(a), b)
#else
#define s64x2_sr(a, b) s64x2_set(s64x2_get(a, 0) >> s64x2_get(b, 0), s64x2_get(a, 1) >> s64x2_get(b, 1))
#define i64x2_sli(a, b) u64x2_set(u64x2_get(a, 0) << ((u64)b), u64x2_get(a, 1) << ((u64)b))
#define s64x2_sri(a, b) s64x2_set(s64x2_get(a, 0) >> ((s64)b), s64x2_get(a, 1) >> ((s64)b))
#define u64x2_sri(a, b) u64x2_set(u64x2_get(a, 0) >> ((u64)b), u64x2_get(a, 1) >> ((u64)b))
#define i64x2_sil(a, b) u64x2_set(((u64)a) << u64x2_get(b, 0), ((u64)a) << u64x2_get(b, 1))
#define s64x2_sir(a, b) s64x2_set(((s64)a) >> s64x2_get(b, 0), ((s64)a) >> s64x2_get(b, 1))
#define u64x2_sir(a, b) u64x2_set(((u64)a) >> u64x2_get(b, 0), ((u64)a) >> u64x2_get(b, 1))
#endif

#if COMPILER_GCC
#define s32x4_div(a,b)                             \
_mm_setr_epi32(                                        \
	_mm_extract_epi32(a, 0) / _mm_extract_epi32(b, 0), \
	_mm_extract_epi32(a, 1) / _mm_extract_epi32(b, 1), \
	_mm_extract_epi32(a, 2) / _mm_extract_epi32(b, 2), \
	_mm_extract_epi32(a, 3) / _mm_extract_epi32(b, 3)  \
)
#define s32x4_rem(a,b)                             \
_mm_setr_epi32(                                        \
	_mm_extract_epi32(a, 0) % _mm_extract_epi32(b, 0), \
	_mm_extract_epi32(a, 1) % _mm_extract_epi32(b, 1), \
	_mm_extract_epi32(a, 2) % _mm_extract_epi32(b, 2), \
	_mm_extract_epi32(a, 3) % _mm_extract_epi32(b, 3)  \
)
#define u32x4_div(a,b)                                       \
_mm_setr_epi32(                                                  \
	(u32)_mm_extract_epi32(a, 0) / (u32)_mm_extract_epi32(b, 0), \
	(u32)_mm_extract_epi32(a, 1) / (u32)_mm_extract_epi32(b, 1), \
	(u32)_mm_extract_epi32(a, 2) / (u32)_mm_extract_epi32(b, 2), \
	(u32)_mm_extract_epi32(a, 3) / (u32)_mm_extract_epi32(b, 3)  \
)
#define u32x4_rem(a,b)                                       \
_mm_setr_epi32(                                                  \
	(u32)_mm_extract_epi32(a, 0) % (u32)_mm_extract_epi32(b, 0), \
	(u32)_mm_extract_epi32(a, 1) % (u32)_mm_extract_epi32(b, 1), \
	(u32)_mm_extract_epi32(a, 2) % (u32)_mm_extract_epi32(b, 2), \
	(u32)_mm_extract_epi32(a, 3) % (u32)_mm_extract_epi32(b, 3)  \
)
#else
#define s32x4_div _mm_div_epi32
#define s32x4_rem _mm_rem_epi32
#define u32x4_div _mm_div_epu32
#define u32x4_rem _mm_rem_epu32
#define s64x2_div _mm_div_epi64
#define s64x2_rem _mm_rem_epi64
#define u64x2_div _mm_div_epu64
#define u64x2_rem _mm_rem_epu64
#endif


#define MM256_PERM128(a, b) ((a) | ((b) << 4))
#define MM_SHUFFLE(a,b,c,d) _MM_SHUFFLE(d,c,b,a)

#define DECLARE_M16 __m128 ps; __m128i pi; __m128d pd
#define DECLARE_M32 __m256 ps; __m256i pi; __m256d pd
*/

// clang-format off

constexpr f32 pi    = f32(3.1415926535897932384626433832795L);
constexpr f32 tau   = f32(6.283185307179586476925286766559L);
constexpr f32 invPi = f32(0.31830988618379067153776752674503L);
constexpr f32 sqrt2 = f32(1.4142135623730950488016887242097L);
constexpr f32 sqrt3 = f32(1.7320508075688772935274463415059L);
constexpr f32 sqrt5 = f32(2.2360679774997896964091736687313L);

template <class T> forceinline constexpr auto radians(T deg) { return deg * (pi / 180.0f); }
template <class T> forceinline constexpr auto degrees(T rad) { return rad * (180.0f / pi); }
template <class T, class U, class V> forceinline constexpr auto clamp(T a, U mi, V ma) { return min(max(a, mi), ma); }
template <class T, class SN, class SX, class DN, class DX> forceinline constexpr auto map(T v, SN sn, SX sx, DN dn, DX dx) { return (v - sn) / (sx - sn) * (dx - dn) + dn; }
template <class T, class SN, class SX, class DN, class DX> forceinline constexpr auto map_clamped(T v, SN sn, SX sx, DN dn, DX dx) { return (clamp(v, sn, sx) - sn) / (sx - sn) * (dx - dn) + dn; }
template <class A, class B, class T> forceinline constexpr auto lerp(A a, B b, T t) { return a + (b - a) * t; }
template <class M, class T> forceinline T &maskAssign(M mask, T &dst, T src) { return dst = select(mask, src, dst); }
template <class T> forceinline constexpr auto pow2(T v) { return v * v; }
template <class T> forceinline constexpr auto pow3(T v) { return v * v * v; }
template <class T> forceinline constexpr auto pow4(T v) { return pow2(v * v); }
// clang-format on

template <class T>
inline constexpr T smooth_min(T a, T b, f32 k) {
	f32 h = max(0, min(1, (b - a) / k + .5f));
	f32 m = h * (1 - h) * k;
	return lerp(b, a, h) - m * .5f; 
}

template <class T>
inline constexpr T smooth_max(T a, T b, f32 k) {
	return smooth_min(a, b, -k); 
}

template <class T>
forceinline constexpr T easeInOut2(T t) {
	if (t < 0.5f) return 0.5f * pow2(2 * t);
	else          return 1 - 0.5f * pow2(2 - 2 * t);
	//return lerp(pow2(t), 1 - pow2(1 - t), t);
}

namespace CE {

template <class To, class From>
forceinline constexpr To cvt(From v) {
	return (To)v;
}

} // namespace CE

union v2f;
union v2s;
union v2u;
template <class T> union v3;
union v4f;
union v4s;
union v4u;
union m3;
union m4;

using v3f = v3<f32>;
//using v3s = v3<s32>;
//using v3u = v3<u32>;
union v3s;
union v3u;

template<class T> inline static constexpr bool is_vector = false;
template<> inline static constexpr bool is_vector<v2f> = true;
template<> inline static constexpr bool is_vector<v3f> = true;
template<> inline static constexpr bool is_vector<v4f> = true;
template<> inline static constexpr bool is_vector<v2s> = true;
template<> inline static constexpr bool is_vector<v3s> = true;
template<> inline static constexpr bool is_vector<v4s> = true;
template<> inline static constexpr bool is_vector<v2u> = true;
template<> inline static constexpr bool is_vector<v3u> = true;
template<> inline static constexpr bool is_vector<v4u> = true;

template <class T> 
inline static constexpr u32 _dimension_of = 0;
template <class T>
inline static constexpr u32 dimension_of = _dimension_of<RemoveCVRef<T>>;

template <> inline static constexpr u32 _dimension_of<v2f> = 2;
template <> inline static constexpr u32 _dimension_of<v2s> = 2;
template <> inline static constexpr u32 _dimension_of<v2u> = 2;
template <> inline static constexpr u32 _dimension_of<v3f> = 3;
template <> inline static constexpr u32 _dimension_of<v3s> = 3;
template <> inline static constexpr u32 _dimension_of<v3u> = 3;
template <> inline static constexpr u32 _dimension_of<v4f> = 4;
template <> inline static constexpr u32 _dimension_of<v4s> = 4;
template <> inline static constexpr u32 _dimension_of<v4u> = 4;

#define ConV2\
	forceinline constexpr v2f V2f(f32 = 0);\
	forceinline constexpr v2f V2f(f32 x, f32 y);\
	forceinline v2fx4 V2fx4(f32 = 0); forceinline constexpr v2fx4 V2fx4(f32x4);\
	forceinline v2fx8 V2fx8(f32 = 0); forceinline constexpr v2fx8 V2fx8(f32x8 v);\
	template<umm ps> forceinline v2fx<ps> V2fx(f32 = 0);\
	template<umm ps> forceinline v2fx<ps> V2fx(f32, f32);\
	template<umm ps> forceinline v2fx<ps> V2fx(f32x<ps>);\
	template<umm ps> forceinline v2fx<ps> V2fx(f32x<ps>, f32x<ps>);\
	template<umm ps> forceinline v2fx<ps> V2fx(v2f);

forceinline constexpr v2f V2f(f32 = 0);
forceinline constexpr v2s V2s(s32 = 0);
forceinline constexpr v2u V2u(u32 = 0);

forceinline constexpr v3f V3f(f32 = 0);
forceinline constexpr v3s V3s(s32 = 0);
forceinline constexpr v3u V3u(u32 = 0);

forceinline constexpr v4f V4f(f32 = 0);
forceinline constexpr v4s V4s(s32 = 0);
forceinline constexpr v4u V4u(u32 = 0);

forceinline constexpr v2f V2f(f32 x, f32 y);
forceinline constexpr v2s V2s(s32 x, s32 y);
forceinline constexpr v2u V2u(u32 x, u32 y);

forceinline constexpr v3f V3f(f32 x, f32 y, f32 z);
forceinline constexpr v3s V3s(s32 x, s32 y, s32 z);
forceinline constexpr v3u V3u(u32 x, u32 y, u32 z);

forceinline constexpr m4 M4();
forceinline constexpr m4 M4(v4f i, v4f j, v4f k, v4f l);
forceinline constexpr m4 M4(f32 ix, f32 iy, f32 iz, f32 iw, f32 jx, f32 jy, f32 jz, f32 jw, f32 kx, f32 ky, f32 kz, f32 kw, f32 lx,
				f32 ly, f32 lz, f32 lw);

forceinline void sincos(f32 v, f32& sinOut, f32& cosOut);
forceinline void sincos(v2f v, v2f& sinOut, v2f& cosOut);
forceinline void sincos(v3f v, v3f& sinOut, v3f& cosOut);
forceinline void sincos(v4f v, v4f& sinOut, v4f& cosOut);

#define fice forceinline constexpr

#define MOP(op, vec)			fice vec& operator op##=(vec b) { return *this = *this op b; }
#define MOPS(op, vec, scl, con)	fice vec& operator op##=(scl b) { return *this op##= con(b); }
#define OPS(op, vec, scl, con)	fice vec operator op(scl b) const { return *this op con(b); }
#define SOP(op, vec, scl, con)	fice friend vec operator op(scl a, vec b) { return con(a) op b; }

#define MEMFUNS_BASIC_SCL(vec, scl, con) \
	SOP(+, vec, scl, con)                \
	SOP(-, vec, scl, con)                \
	SOP(*, vec, scl, con)                \
	SOP(/, vec, scl, con)                \
	OPS(+, vec, scl, con)                \
	OPS(-, vec, scl, con)                \
	OPS(*, vec, scl, con)                \
	OPS(/, vec, scl, con)                \
	MOPS(+, vec, scl, con)               \
	MOPS(-, vec, scl, con)               \
	MOPS(*, vec, scl, con)               \
	MOPS(/, vec, scl, con)

#define MEMFUNS_BASIC(vec, scl, con) \
	MOP(+, vec)                      \
	MOP(-, vec)                      \
	MOP(*, vec)                      \
	MOP(/, vec)                      \
	MEMFUNS_BASIC_SCL(vec, scl, con)

#define NEQ(vec) \
	fice bool operator!=(vec b) const { return !(*this == b); }

#define MEMFUNS_DATA(scl)               \
	fice scl* data() { return s; } \
	fice scl const* data() const { return s; }

#define INDEX_S(scl)                                          \
	fice scl operator[](umm i) const { return s[i]; } \
	fice scl& operator[](umm i) { return s[i]; }

#define MEMFUNS_INT(vec, scl, con)                              \
	fice vec& operator%=(vec b) { return *this = *this % b; }        \
	fice vec& operator^=(vec b) { return *this = *this ^ b; }        \
	fice vec& operator|=(vec b) { return *this = *this | b; }        \
	fice vec& operator&=(vec b) { return *this = *this & b; }        \
	fice vec& operator<<=(vec b) { return *this = *this << b; }      \
	fice vec& operator>>=(vec b) { return *this = *this >> b; }      \
	fice vec operator%(scl b) const { return *this % con(b); }       \
	fice vec operator^(scl b) const { return *this ^ con(b); }       \
	fice vec operator|(scl b) const { return *this | con(b); }       \
	fice vec operator&(scl b) const { return *this & con(b); }       \
	fice vec& operator%=(scl b) { return *this %= con(b); }          \
	fice vec& operator^=(scl b) { return *this ^= con(b); }          \
	fice vec& operator|=(scl b) { return *this |= con(b); }          \
	fice vec& operator&=(scl b) { return *this &= con(b); }          \
	fice vec& operator<<=(scl b) { return *this = *this << b; }      \
	fice vec& operator>>=(scl b) { return *this = *this >> b; }      \
	fice friend vec operator%(scl a, vec b) { return con(a) % b; }   \
	fice friend vec operator^(scl a, vec b) { return con(a) ^ b; }   \
	fice friend vec operator|(scl a, vec b) { return con(a) | b; }   \
	fice friend vec operator&(scl a, vec b) { return con(a) & b; }   \
	fice friend vec operator<<(scl a, vec b) { return con(a) << b; } \
	fice friend vec operator>>(scl a, vec b) { return con(a) >> b; }

#define MEMFUNS_SHIFT_SCL(vec, scl, con)                    \
	fice vec operator<<(scl b) const { return *this << con(b); } \
	fice vec operator>>(scl b) const { return *this >> con(b); }

#define MEMFUNS_SHIFT_SCL_M(vec, scl, con)              \
	fice vec operator<<(scl b) const { return con(m << b); } \
	fice vec operator>>(scl b) const { return con(m >> b); }

#define MEMFUNS_SHIFT_SCL_2(vec, scl)                        \
	fice vec operator<<(scl b) const { return {x << b, y << b}; } \
	fice vec operator>>(scl b) const { return {x >> b, y >> b}; }

#define MEMFUNS_SHIFT_SCL_3(vec, scl)                                \
	fice vec operator<<(scl b) const { return {x << b, y << b, z << b}; } \
	fice vec operator>>(scl b) const { return {x >> b, y >> b, z >> b}; }

#define MEMFUNS_SHIFT_SCL_4(vec, scl)                                        \
	fice vec operator<<(scl b) const { return {x << b, y << b, z << b, w << b}; } \
	fice vec operator>>(scl b) const { return {x >> b, y >> b, z >> b, w >> b}; }


#define RETURN_T(t,x)    t _result{}; _result = x; return _result
#define RETURN_T2(t,x,y) t _result{}; _result.lo = x; _result.hi = y; return _result
#define RETURN(x)        RETURN_T(Pack,x)
#define RETURN_2(x,y)    RETURN_T2(Pack,x,y)

#define RETURN_MT(t,m,x)    t _result{}; _result.m = x; return _result
#define RETURN_MT2(t,m,x,y) t _result{}; _result.lo.m = x; _result.hi.m = y; return _result
#define RETURN_M(m,x)       RETURN_MT(Pack,m,x)
#define RETURN_M2(m,x,y)    RETURN_MT2(Pack,m,x,y)

#define RETURN_MASK(m,x)       RETURN_MT(Mask,m,x)


#define OPERATOR_SCALAR(op, extend) \
	forceinline Pack operator op(Scalar b) const { return *this op extend(b); } \
	forceinline friend Pack operator op(Scalar a, Pack b) { return extend(a) op b; }

// is first is not friend, s32x4's operator!= refuses to compile
#define OPERATOR_COMPARE(op, extend) \
	forceinline friend Mask operator op(Pack a, Scalar b) { return a op extend(b); } \
	forceinline friend Mask operator op(Scalar a, Pack b) { return extend(a) op b; }

#define OPERATOR_MODIFY(op, extend) \
	forceinline Pack &operator op##=(Pack b) { return *this = *this op b; } \
	forceinline Pack &operator op##=(Scalar b) { return *this = *this op extend(b); }


#define OPERATOR_ALL_VER(op, extend) \
	OPERATOR_MODIFY(op, extend) \
	OPERATOR_SCALAR(op, extend)

#define OPERATOR_XCREMENT(extend) \
	forceinline Pack &operator++()   { return *this = *this + extend(1); } \
	forceinline Pack &operator--()   { return *this = *this - extend(1); } \
	forceinline Pack  operator++(int){ auto result = *this; *this = *this + extend(1); return result; }\
	forceinline Pack  operator--(int){ auto result = *this; *this = *this - extend(1); return result; }

#define OPERATORS_BOOL(extend)                                           \
	forceinline Pack operator!() const { return ~*this; }               \
	forceinline Pack operator&&(Pack b) const { return *this & b; }     \
	forceinline Pack operator||(Pack b) const { return *this | b; }     \
	forceinline Scalar operator[](umm index) const { return s[index]; } \
	forceinline Scalar &operator[](umm index) { return s[index]; } \
	OPERATOR_COMPARE(==, extend) \
	OPERATOR_COMPARE(!=, extend) \
	OPERATOR_ALL_VER(^, extend) \
	OPERATOR_ALL_VER(&, extend) \
	OPERATOR_ALL_VER(|, extend)\
	OPERATOR_SCALAR(&&, extend) \
	OPERATOR_SCALAR(||, extend)

#define FALLBACK_BOOL                                                                  \
	forceinline Pack operator~() const { RETURN_2(~lo, ~hi); }                        \
	forceinline Pack operator==(Pack b) const { RETURN_2(lo == b.lo, hi == b.hi); } \
	forceinline Pack operator!=(Pack b) const { RETURN_2(lo != b.lo, hi != b.hi); } \
	forceinline Pack operator^ (Pack b) const { RETURN_2(lo ^  b.lo, hi ^  b.hi); } \
	forceinline Pack operator& (Pack b) const { RETURN_2(lo &  b.lo, hi &  b.hi); } \
	forceinline Pack operator| (Pack b) const { RETURN_2(lo |  b.lo, hi |  b.hi); }

#define B1_IMPL(extend) \
	Scalar s[1];\
	Scalar m;\
	forceinline Pack operator~() const { return {~m}; }\
	forceinline Pack operator==(Pack b) const { return {~(m ^ b.m)}; }\
	forceinline Pack operator!=(Pack b) const { return {m ^ b.m}; }\
	forceinline Pack operator^ (Pack b) const { return {m ^ b.m}; }\
	forceinline Pack operator& (Pack b) const { return {m & b.m}; }\
	forceinline Pack operator| (Pack b) const { return {m | b.m}; }\
	OPERATORS_BOOL(extend)

#define B128_IMPL(extend) \
	DECLARE_M16;\
	forceinline Pack operator~() const { RETURN_M(ps, _mm_xor_ps(ps, _mm_setone_ps())); }\
	forceinline Pack operator==(Pack b) const { RETURN_M(pi, s32x4_eq(pi, b.pi)); }\
	forceinline Pack operator!=(Pack b) const { RETURN_M(pi, s32x4_ne(pi, b.pi)); }\
	forceinline Pack operator^ (Pack b) const { RETURN_M(ps, _mm_xor_ps(ps, b.ps)); }\
	forceinline Pack operator& (Pack b) const { RETURN_M(ps, _mm_and_ps(ps, b.ps)); }\
	forceinline Pack operator| (Pack b) const { RETURN_M(ps, _mm_or_ps (ps, b.ps)); }\
	OPERATORS_BOOL(extend)

#if ARCH_AVX
#define B256_IMPL(extend) \
	DECLARE_M32;\
	forceinline Pack operator~() const { RETURN_M(ps, _mm256_xor_ps(ps, _mm256_setone_ps())); }\
	forceinline Pack operator==(Pack b) const { RETURN_M(ps, b256_cmpeq(ps, b.ps)); }\
	forceinline Pack operator!=(Pack b) const { RETURN_M(ps, _mm256_xor_ps(ps, b.ps)); }\
	forceinline Pack operator^ (Pack b) const { RETURN_M(ps, _mm256_xor_ps(ps, b.ps)); }\
	forceinline Pack operator& (Pack b) const { RETURN_M(ps, _mm256_and_ps(ps, b.ps)); }\
	forceinline Pack operator| (Pack b) const { RETURN_M(ps, _mm256_or_ps (ps, b.ps)); }\
	OPERATORS_BOOL(extend)
#else
#define B256_IMPL(extend) \
	FALLBACK_BOOL; \
	OPERATORS_BOOL(extend)
#endif

#define OPERATORS_FLOAT(extend) \
	OPERATOR_COMPARE(>, extend) \
	OPERATOR_COMPARE(<, extend) \
	OPERATOR_COMPARE(>=, extend) \
	OPERATOR_COMPARE(<=, extend) \
	OPERATOR_COMPARE(==, extend) \
	OPERATOR_COMPARE(!=, extend) \
	OPERATOR_ALL_VER(+, extend) \
	OPERATOR_ALL_VER(-, extend) \
	OPERATOR_ALL_VER(*, extend) \
	OPERATOR_ALL_VER(/, extend) \
	OPERATOR_XCREMENT(extend)

#define OPERATORS_INT(extend) \
	OPERATOR_COMPARE(>, extend) \
	OPERATOR_COMPARE(<, extend) \
	OPERATOR_COMPARE(>=, extend) \
	OPERATOR_COMPARE(<=, extend) \
	OPERATOR_COMPARE(==, extend) \
	OPERATOR_COMPARE(!=, extend) \
	OPERATOR_ALL_VER(+, extend) \
	OPERATOR_ALL_VER(-, extend) \
	OPERATOR_ALL_VER(*, extend) \
	OPERATOR_ALL_VER(/, extend) \
	OPERATOR_ALL_VER(%, extend) \
	OPERATOR_ALL_VER(^, extend) \
	OPERATOR_ALL_VER(&, extend) \
	OPERATOR_ALL_VER(|, extend) \
	OPERATOR_MODIFY(>>, extend) \
	OPERATOR_MODIFY(<<, extend) \
	forceinline Mask operator!() const { return *this == 0; } \
	OPERATOR_XCREMENT(extend)


#define FALLBACK_FLOAT                                                                  \
	forceinline Pack operator-() const { RETURN_2(-lo, -hi); }                        \
	forceinline Pack operator+ (Pack b) const { RETURN_2(lo +  b.lo, hi +  b.hi); } \
	forceinline Pack operator- (Pack b) const { RETURN_2(lo -  b.lo, hi -  b.hi); } \
	forceinline Pack operator* (Pack b) const { RETURN_2(lo *  b.lo, hi *  b.hi); } \
	forceinline Pack operator/ (Pack b) const { RETURN_2(lo /  b.lo, hi /  b.hi); } \
	forceinline Mask operator> (Pack b) const { RETURN_T2(Mask, lo >  b.lo, hi >  b.hi); } \
	forceinline Mask operator< (Pack b) const { RETURN_T2(Mask, lo <  b.lo, hi <  b.hi); } \
	forceinline Mask operator>=(Pack b) const { RETURN_T2(Mask, lo >= b.lo, hi >= b.hi); } \
	forceinline Mask operator<=(Pack b) const { RETURN_T2(Mask, lo <= b.lo, hi <= b.hi); } \
	forceinline Mask operator==(Pack b) const { RETURN_T2(Mask, lo == b.lo, hi == b.hi); } \
	forceinline Mask operator!=(Pack b) const { RETURN_T2(Mask, lo != b.lo, hi != b.hi); }

#define FALLBACK_INT_NO_BITWISE                                                                  \
	forceinline Pack operator-() const { RETURN_2(-lo, -hi); }                                 \
	forceinline Pack operator+(Pack b) const { RETURN_2(lo + b.lo, hi + b.hi); }             \
	forceinline Pack operator-(Pack b) const { RETURN_2(lo - b.lo, hi - b.hi); }             \
	forceinline Pack operator*(Pack b) const { RETURN_2(lo * b.lo, hi * b.hi); }             \
	forceinline Pack operator/(Pack b) const { RETURN_2(lo / b.lo, hi / b.hi); }             \
	forceinline Pack operator%(Pack b) const { RETURN_2(lo % b.lo, hi % b.hi); }             \
	forceinline Pack operator<<(Pack b) const { RETURN_2(lo << b.lo, hi << b.hi); }          \
	forceinline Pack operator>>(Pack b) const { RETURN_2(lo >> b.lo, hi >> b.hi); }          \
	forceinline Pack operator<<(Scalar b) const { RETURN_2(lo << b, hi << b); }                \
	forceinline Pack operator>>(Scalar b) const { RETURN_2(lo >> b, hi >> b); }                \
	forceinline friend Pack operator<<(Scalar a, Pack b) { RETURN_T2(decltype(b), a << b.lo, a << b.hi); } \
	forceinline friend Pack operator>>(Scalar a, Pack b) { RETURN_T2(decltype(b), a >> b.lo, a >> b.hi); } \
	forceinline Mask operator> (Pack b) const { RETURN_T2(Mask, lo >  b.lo, hi >  b.hi); }     \
	forceinline Mask operator< (Pack b) const { RETURN_T2(Mask, lo <  b.lo, hi <  b.hi); }     \
	forceinline Mask operator>=(Pack b) const { RETURN_T2(Mask, lo >= b.lo, hi >= b.hi); }     \
	forceinline Mask operator<=(Pack b) const { RETURN_T2(Mask, lo <= b.lo, hi <= b.hi); }     \
	forceinline Mask operator==(Pack b) const { RETURN_T2(Mask, lo == b.lo, hi == b.hi); }     \
	forceinline Mask operator!=(Pack b) const { RETURN_T2(Mask, lo != b.lo, hi != b.hi); }

#define FALLBACK_INT                                                                 \
	FALLBACK_INT_NO_BITWISE                                                          \
	forceinline Pack operator~() const { RETURN_2(~lo, ~hi); }                     \
	forceinline Pack operator^(Pack b) const { RETURN_2(lo ^ b.lo, hi ^ b.hi); } \
	forceinline Pack operator&(Pack b) const { RETURN_2(lo & b.lo, hi & b.hi); } \
	forceinline Pack operator|(Pack b) const { RETURN_2(lo | b.lo, hi | b.hi); }                   

#define TOMASK(expr) {(expr) ? -1 : 0}

#define CVT(other) explicit forceinline operator other() const

// clang-format on

#define UOP2(op, ty, con) fice ty operator op() const { return {op x, op y}; }
#define UOP3(op, ty, con) fice ty operator op() const { return {op x, op y, op z}; }
#define UOP4(op, ty, con) fice ty operator op() const { return {op x, op y, op z, op w}; }

#define BOP2(op, ty, con) fice ty operator op(ty b) const { return {x op b.x, y op b.y}; }
#define BOP3(op, ty, con) fice ty operator op(ty b) const { return {x op b.x, y op b.y, z op b.z}; }
#define BOP4(op, ty, con) fice ty operator op(ty b) const { return {x op b.x, y op b.y, z op b.z, w op b.w}; }

#define BOP2CMP(op, ty, con) fice ty operator op(ty b) const { return {_mask<ty>(x op b.x), _mask<ty>(y op b.y)}; }
#define BOP3CMP(op, ty, con) fice ty operator op(ty b) const { return {_mask<ty>(x op b.x), _mask<ty>(y op b.y), _mask<ty>(z op b.z)}; }
#define BOP4CMP(op, ty, con) fice ty operator op(ty b) const { return {_mask<ty>(x op b.x), _mask<ty>(y op b.y), _mask<ty>(z op b.z), _mask<ty>(w op b.w)}; }

#define BOPSF32_(bop, uop, cmp, ty, con) \
uop(-, ty, con) \
bop(+, ty, con) \
bop(-, ty, con) \
bop(*, ty, con) \
bop(/, ty, con)

#define BOPSU32_(bop, uop, ty, con) \
bop(+, ty, con) \
bop(-, ty, con) \
bop(*, ty, con) \
bop(/, ty, con) \
bop(%, ty, con) \
bop(^, ty, con) \
bop(|, ty, con) \
bop(&, ty, con) \
bop(<<, ty, con) \
bop(>>, ty, con) \
uop(~, ty, con)

#define BOPSS32_(bop, uop, ty, con) \
BOPSU32_(bop, uop, ty, con) \
uop(-, ty, con)

#define BOPSF32(op, ty, con) BOPSF32_(CONCAT(B, op), CONCAT(U, op), CONCAT(CONCAT(B, op), CMP), ty, con)
#define BOPSS32(op, ty, con) BOPSS32_(CONCAT(B, op), CONCAT(U, op), ty, con)
#define BOPSU32(op, ty, con) BOPSU32_(CONCAT(B, op), CONCAT(U, op), ty, con)

#define VECCMP2(bool, v2) fice bool operator==(v2 b) const { return x == b.x && y == b.y; }
#define VECCMP3(bool, v3) fice bool operator==(v3 b) const { return x == b.x && y == b.y && z == b.z; }
#define VECCMP4(bool, v4) fice bool operator==(v4 b) const { return x == b.x && y == b.y && z == b.z && w == b.w; }

#define MEMBERS2(width, bool, f32, v2, v3, v4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...) \
	struct {                                                                          \
		f32 x, y;                                                                     \
	};                                                                                \
	fn(__VA_ARGS__) VECCMP2(bool, v2) CVT(v2s);                                       \
	CVT(v2u);                                                                         \
	f32 s[(width)*2]

#define MEMBERS3(width, bool, f32, v2, v3, v4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...) \
	struct {                                                                          \
		f32 x, y, z;                                                                  \
	};                                                                                \
	v2 xy;                                                                            \
	struct {                                                                          \
		f32 _pad0;                                                                    \
		v2 yz;                                                                        \
	};                                                                                \
	fn(__VA_ARGS__) VECCMP3(bool, v3) CVT(v3s);                                       \
	CVT(v3u);                                                                         \
	f32 s[(width)*3]

#define MEMBERS4(width, bool, f32, v2, v3, v4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...) \
	struct {                                                                          \
		f32 x, y, z, w;                                                               \
	};                                                                                \
	struct {                                                                          \
		v2 xy;                                                                        \
		v2 zw;                                                                        \
	};                                                                                \
	v3 xyz;                                                                           \
	struct {                                                                          \
		f32 _pad0;                                                                    \
		union {                                                                       \
			v2 yz;                                                                    \
			v3 yzw;                                                                   \
		};                                                                            \
	};                                                                                \
	fn(__VA_ARGS__) VECCMP4(bool, v4) CVT(v4s);                                       \
	CVT(v4u);                                                                         \
	f32 s[(width)*4]

#define MEMBERS2S(width, bool, f32, v2, v3, v4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...)     \
	MEMBERS2(width, bool, f32, v2, v3, v4, v2s, v3s, v4s, v2u, v3u, v4u, fn, __VA_ARGS__); \
	fice f32 operator[](umm i) const { return s[i]; }                               \
	fice f32 &operator[](umm i) { return s[i]; }
#define MEMBERS3S(width, bool, f32, v2, v3, v4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...)     \
	MEMBERS3(width, bool, f32, v2, v3, v4, v2s, v3s, v4s, v2u, v3u, v4u, fn, __VA_ARGS__); \
	fice f32 operator[](umm i) const { return s[i]; }                               \
	fice f32 &operator[](umm i) { return s[i]; }
#define MEMBERS4S(width, bool, f32, v2, v3, v4, v2s, v3s, v4s, v2u, v3u, v4u, fn, ...)     \
	MEMBERS4(width, bool, f32, v2, v3, v4, v2s, v3s, v4s, v2u, v3u, v4u, fn, __VA_ARGS__); \
	fice f32 operator[](umm i) const { return s[i]; }                               \
	fice f32 &operator[](umm i) { return s[i]; }

// clang-format off
#define IMPL_v2f32x1() MEMFUNS_DATA(f32) MEMFUNS_BASIC(v2f, f32, V2f) BOPSF32(OP2, v2f, V2f)
#define IMPL_v3f32x1() MEMFUNS_DATA(f32) MEMFUNS_BASIC(v3f, f32, V3f) BOPSF32(OP3, v3f, V3f)
#define IMPL_v4f32x1() MEMFUNS_DATA(f32) MEMFUNS_BASIC(v4f, f32, V4f) BOPSF32(OP4, v4f, V4f)
#define IMPL_v2s32x1() MEMFUNS_DATA(s32) MEMFUNS_BASIC(v2s, s32, V2s) MEMFUNS_INT(v2s, s32, V2s) MEMFUNS_SHIFT_SCL(v2s, s32, V2s) BOPSS32(OP2, v2s, V2s)
#define IMPL_v3s32x1() MEMFUNS_DATA(s32) MEMFUNS_BASIC(v3s, s32, V3s) MEMFUNS_INT(v3s, s32, V3s) MEMFUNS_SHIFT_SCL(v3s, s32, V3s) BOPSS32(OP3, v3s, V3s)
#define IMPL_v4s32x1() MEMFUNS_DATA(s32) MEMFUNS_BASIC(v4s, s32, V4s) MEMFUNS_INT(v4s, s32, V4s) MEMFUNS_SHIFT_SCL(v4s, s32, V4s) BOPSS32(OP4, v4s, V4s)
#define IMPL_v2u32x1() MEMFUNS_DATA(u32) MEMFUNS_BASIC(v2u, u32, V2u) MEMFUNS_INT(v2u, u32, V2u) MEMFUNS_SHIFT_SCL(v2u, u32, V2u) BOPSU32(OP2, v2u, V2u)
#define IMPL_v3u32x1() MEMFUNS_DATA(u32) MEMFUNS_BASIC(v3u, u32, V3u) MEMFUNS_INT(v3u, u32, V3u) MEMFUNS_SHIFT_SCL(v3u, u32, V3u) BOPSU32(OP3, v3u, V3u)
#define IMPL_v4u32x1() MEMFUNS_DATA(u32) MEMFUNS_BASIC(v4u, u32, V4u) MEMFUNS_INT(v4u, u32, V4u) MEMFUNS_SHIFT_SCL(v4u, u32, V4u) BOPSU32(OP4, v4u, V4u)

#define TYPES_f32x1(fn, packSize, dim) fn(packSize, bool, f32, v2f, v3f, v4f, v2s, v3s, v4s, v2u, v3u, v4u, IMPL_v##dim##f32x1)
#define TYPES_s32x1(fn, packSize, dim) fn(packSize, bool, s32, v2s, v3s, v4s, v2f, v3f, v4f, v2u, v3u, v4u, IMPL_v##dim##s32x1)
#define TYPES_u32x1(fn, packSize, dim) fn(packSize, bool, u32, v2u, v3u, v4u, v2f, v3f, v4f, v2s, v3s, v4s, IMPL_v##dim##u32x1)

#define VECIMPL(dim, SorP, packSize, type) TYPES_##type##x##packSize(MEMBERS##dim##SorP, packSize, dim)

union v3s64;

union v2f { VECIMPL(2, S, 1, f32); v2f yx() const { return {y, x}; } };
template <>
union v3<f32> {
	using v2 = v2f;

	struct { f32 x, y, z; };
	v2 xy;
	struct { f32 _pad; v2 yz; };
	f32 s[3];

	fice v3f yzx() const { return {y, z, x}; }
	fice v3f zxy() const { return {z, x, y}; }

	fice v3f operator+() const { return *this; }
	fice v3f operator-() const { return {-x, -y, -z}; }
	fice v3f operator+(v3f b) const { return {x + b.x, y + b.y, z + b.z}; }
	fice v3f operator-(v3f b) const { return {x - b.x, y - b.y, z - b.z}; }
	fice v3f operator*(v3f b) const { return {x * b.x, y * b.y, z * b.z}; }
	fice v3f operator/(v3f b) const { return {x / b.x, y / b.y, z / b.z}; }
	fice v3f operator+(f32 b) const { return {x + b, y + b, z + b}; }
	fice v3f operator-(f32 b) const { return {x - b, y - b, z - b}; }
	fice v3f operator*(f32 b) const { return {x * b, y * b, z * b}; }
	fice v3f operator/(f32 b) const { return {x / b, y / b, z / b}; }
	fice friend v3f operator+(f32 a, v3f b) { return {a + b.x, a + b.y, a + b.z};}
	fice friend v3f operator-(f32 a, v3f b) { return {a - b.x, a - b.y, a - b.z};}
	fice friend v3f operator*(f32 a, v3f b) { return {a * b.x, a * b.y, a * b.z};}
	fice friend v3f operator/(f32 a, v3f b) { return {a / b.x, a / b.y, a / b.z};}
	fice v3f &operator+=(v3f b) { return x += b.x, y += b.y, z += b.z, *this;}
	fice v3f &operator-=(v3f b) { return x -= b.x, y -= b.y, z -= b.z, *this;}
	fice v3f &operator*=(v3f b) { return x *= b.x, y *= b.y, z *= b.z, *this;}
	fice v3f &operator/=(v3f b) { return x /= b.x, y /= b.y, z /= b.z, *this;}
	fice v3f &operator+=(f32 b) { return x += b, y += b, z += b, *this;}
	fice v3f &operator-=(f32 b) { return x -= b, y -= b, z -= b, *this;}
	fice v3f &operator*=(f32 b) { return x *= b, y *= b, z *= b, *this;}
	fice v3f &operator/=(f32 b) { return x /= b, y /= b, z /= b, *this;}

	forceinline explicit operator v3s() const;
	forceinline explicit operator v3u() const;
};
union v4f { VECIMPL(4, S, 1, f32); };

union v2s { VECIMPL(2, S, 1, s32); };
union v3s { 
	VECIMPL(3, S, 1, s32); 
	fice v2s xz() const { return {x, z}; } 
	fice v3s zxy() const { return {z,x,y}; }
	forceinline explicit operator v3s64() const;
};
union v4s { VECIMPL(4, S, 1, s32); };
union v2u { VECIMPL(2, S, 1, u32); };
union v3u { VECIMPL(3, S, 1, u32); };
union v4u { VECIMPL(4, S, 1, u32); };

#undef CVT

v3f::operator v3s()const{return{(s32)x,(s32)y,(s32)z};}
v3f::operator v3u()const{return{(u32)x,(u32)y,(u32)z};}

// clang-format on

union v3s64 {
	struct {
		s64 x, y, z;
	};
	fice v3s64 operator-(v3s64 b) const { return {x - b.x, y - b.y, z - b.z}; }
	fice v3s64 operator*(v3s64 b) const { return {x * b.x, y * b.y, z * b.z}; }
	fice v3s64 operator*(s64   b) const { return {x * b, y * b, z * b}; }
	fice v3s64 &operator*=(v3s64 b) { return x *= b.x, y *= b.y, z *= b.z, *this; }
};
forceinline v3s::operator v3s64() const {
	return {x, y, z};
}

union m2 {
	struct {
		v2f i, j;
	};
	f32 s[4];
	forceinline v2f operator*(v2f b) const {
		v2f x = V2f(b.x) * i;
		v2f y = V2f(b.y) * j;
		return x + y;
	}
	forceinline m2 operator*(m2 b) const { return {*this * b.i, *this * b.j}; }
	forceinline m2& operator*=(m2 b) { return *this = *this * b; }
	static forceinline m2 identity() { 
		return {
			1, 0,
			0, 1,
		};
	}
	static forceinline m2 scale(f32 x, f32 y) {
		// clang-format off
		return {
			x, 0,
			0, y,
		};
		// clang-format on
	}
	static forceinline m2 scale(v2f v) { return scale(v.x, v.y); }
	static forceinline m2 scale(f32 v) { return scale(v, v); }
	static forceinline m2 rotation(f32 a) {
		f32 s, c;
		sincos(a, s, c);
		// clang-format off
		return {
			c, s,
		   -s, c,
		};
		// clang-format on
	}
};

union m3 {
	struct {
		v3f i, j, k;
	};
	f32 s[9];
	forceinline v3f operator*(v3f b) const {
		v3f x = V3f(b.x) * i;
		v3f y = V3f(b.y) * j;
		v3f z = V3f(b.z) * k;
		return x + y + z;
	}
	forceinline m3 operator*(m3 b) const { return {*this * b.i, *this * b.j, *this * b.k}; }
	forceinline m3& operator*=(m3 b) { return *this = *this * b; }
	static forceinline m3 identity() { 
		return {
			1, 0, 0,
			0, 1, 0,
			0, 0, 1,
		};
	}
	static forceinline m3 scale(f32 x, f32 y, f32 z) {
		// clang-format off
		return {
			x, 0, 0,
			0, y, 0,
			0, 0, z,
		};
		// clang-format on
	}
	static forceinline m3 scale(v3f v) { return scale(v.x, v.y, v.z); }
	static forceinline m3 scale(v2f xy, f32 z) { return scale(xy.x, xy.y, z); }
	static forceinline m3 scale(f32 v) { return scale(v, v, v); }
	static forceinline m3 translation(f32 x, f32 y) {
		// clang-format off
		return {
			1, 0, 0,
			0, 1, 0,
			x, y, 1,
		};
		// clang-format on
	}
	static forceinline m3 translation(v2f v) { return translation(v.x, v.y); }
	static forceinline m3 translation(f32 v) { return translation(v, v); }
	static forceinline m3 rotationX(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			1, 0, 0,
			0, c, s,
			0,-s, c,
		};
		// clang-format on
	}
	static forceinline m3 rotationY(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			c, 0, s,
			0, 1, 0,
		   -s, 0, c,
		};
		// clang-format on
	}
	static forceinline m3 rotationZ(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			c, s, 0,
		   -s, c, 0,
			0, 0, 1,
		};
		// clang-format on
	}
};

#define V2(f32, v2f, V2f)                                     \
	forceinline constexpr v2f V2f(f32 x, f32 y) { return {x, y}; } \
	forceinline constexpr v2f V2f(f32 v) { return {v, v}; }
V2(f32, v2f, V2f);
V2(s32, v2s, V2s);
V2(u32, v2u, V2u);
#undef V2

#define V3(f32, v2f, v3f, V3f)                                          \
	forceinline constexpr v3f V3f(f32 x, f32 y, f32 z) { return {x, y, z}; } \
	forceinline constexpr v3f V3f(f32 v) { return {v, v, v}; }               \
	forceinline constexpr v3f V3f(v2f xy, f32 z) { return {xy.x, xy.y, z}; } \
	forceinline constexpr v3f V3f(f32 x, v2f yz) { return {x, yz.x, yz.y}; }
V3(f32, v2f, v3f, V3f);
V3(s32, v2s, v3s, V3s);
V3(u32, v2u, v3u, V3u);
#undef V3

#define V4(f32, v2f, v3f, v4f, V4f)                                        \
	forceinline constexpr v4f V4f(f32 x, f32 y, f32 z, f32 w) { return {x, y, z, w}; } \
	forceinline constexpr v4f V4f(f32 v) { return {v, v, v, v}; }                      \
	forceinline constexpr v4f V4f(v2f xy, v2f zw) { return {xy.x, xy.y, zw.x, zw.y}; } \
	forceinline constexpr v4f V4f(v2f xy, f32 z, f32 w) { return {xy.x, xy.y, z, w}; } \
	forceinline constexpr v4f V4f(f32 x, f32 y, v2f zw) { return {x, y, zw.x, zw.y}; } \
	forceinline constexpr v4f V4f(v3f xyz, f32 w) { return {xyz.x, xyz.y, xyz.z, w}; } \
	forceinline constexpr v4f V4f(f32 x, v3f yzw) { return {x, yzw.x, yzw.y, yzw.z}; }
V4(f32, v2f, v3f, v4f, V4f);
V4(s32, v2s, v3s, v4s, V4s);
V4(u32, v2u, v3u, v4u, V4u);
#undef V4

#define C(v4f, V4f) forceinline constexpr v4f V4f(v4f v) { return v; }
C(v2f, V2f) C(v2s, V2s) C(v2u, V2u);
C(v3f, V3f) C(v3s, V3s) C(v3u, V3u);
C(v4f, V4f) C(v4s, V4s) C(v4u, V4u);
#undef C

// clang-format off
#define CVT(v2f, v2s, V2s, s32, v2u, V2u, u32)                \
	forceinline v2f::operator v2s() const { return V2s((s32)x, (s32)y); } \
	forceinline v2f::operator v2u() const { return V2u((u32)x, (u32)y); }
CVT(v2f, v2s, V2s, s32, v2u, V2u, u32)
CVT(v2s, v2f, V2f, f32, v2u, V2u, u32)
CVT(v2u, v2f, V2f, f32, v2s, V2s, s32)
#undef CVT

#define CVT(v3f, v3s, V3s, s32, v3u, V3u, u32)                        \
	forceinline v3f::operator v3s() const { return V3s((s32)x, (s32)y, (s32)z); } \
	forceinline v3f::operator v3u() const { return V3u((u32)x, (u32)y, (u32)z); }
CVT(v3s, v3f, V3f, f32, v3u, V3u, u32)
CVT(v3u, v3f, V3f, f32, v3s, V3s, s32)
#undef CVT

#define CVT(v4f, v4s, V4s, s32, v4u, V4u, u32)                                \
	forceinline v4f::operator v4s() const { return V4s((s32)x, (s32)y, (s32)z, (s32)w); } \
	forceinline v4f::operator v4u() const { return V4u((u32)x, (u32)y, (u32)z, (u32)w); }
CVT(v4f, v4s, V4s, s32, v4u, V4u, u32)
CVT(v4s, v4f, V4f, f32, v4u, V4u, u32)
CVT(v4u, v4f, V4f, f32, v4s, V4s, s32)
#undef CVT
// clang-format on

namespace CE {

template <class T>
constexpr T sqrt_newton_raphson(T x, T curr, T prev) {
    return curr == prev ? curr : sqrt_newton_raphson(x, (T)(0.5 * (curr + x / curr)), curr);
};

template <class T>
constexpr auto sqrt(T x) {
	return sqrt_newton_raphson(x, x, T{});
}

template <class T>
constexpr auto length(T a) {
	return CE::sqrt(dot(a, a));
}
template <class T>
constexpr auto normalize(T a) {
	return a * (1.0f / CE::length(a));
}

} // namespace CE

namespace CE {

#define CVT(v2f, v2s, s32) template<> constexpr v2s cvt(v2f v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y)}; }
CVT(v2f, v2s, s32) CVT(v2f, v2u, u32)
CVT(v2s, v2f, f32) CVT(v2s, v2u, u32)
CVT(v2u, v2f, f32) CVT(v2u, v2s, s32)
#undef CVT

#define CVT(v3f, v3s, s32) template<> constexpr v3s cvt(v3f v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y), CE::cvt<s32>(v.z)}; }
CVT(v3f, v3s, s32) CVT(v3f, v3u, u32)
CVT(v3s, v3f, f32) CVT(v3s, v3u, u32)
CVT(v3u, v3f, f32) CVT(v3u, v3s, s32)
#undef CVT

#define CVT(v4f, v4s, s32) template<> constexpr v4s cvt(v4f v) { return {CE::cvt<s32>(v.x), CE::cvt<s32>(v.y), CE::cvt<s32>(v.z), CE::cvt<s32>(v.w)}; }
CVT(v4f, v4s, s32) CVT(v4f, v4u, u32)
CVT(v4s, v4f, f32) CVT(v4s, v4u, u32)
CVT(v4u, v4f, f32) CVT(v4u, v4s, s32)
#undef CVT
} // namespace CE

// clang-format on

#define SHUFFLE(a, s0, s1, b, s2, s3) _mm_shuffle_ps(a, b, _MM_SHUFFLE(s3, s2, s1, s0))

// clang-format off
forceinline v2f pack(v2f v) { return v; }
forceinline v3f pack(v3f v) { return v; }
forceinline v4f pack(v4f v) { return v; }
forceinline v2f unpack(v2f v) { return v; }
forceinline v3f unpack(v3f v) { return v; }
forceinline v4f unpack(v4f v) { return v; }

#if 0

#if ARCH_AVX2
forceinline void pack_32x4(any32x4 &x, any32x4 &y) { 
	any32x8 c = combine_m16(x, y);
    c = _mm256_permutevar8x32_ps(c, _mm256_setr_epi32(0,2,4,6,1,3,5,7));
	x = m32_get_half(c, 0);
	y = m32_get_half(c, 1);
}
#else
forceinline void pack_32x4(any32x4 &x, any32x4 &y) { 
	v2fx4 r;
	for (u32 i = 0; i < 2; ++i) {
		for (u32 j = 0; j < 4; ++j) {
			r.s[i * 4 + j]  = v.s[i + j * 2];
		}
	}
}
#endif

forceinline void pack_32x4(any32x4 &_x, any32x4 &_y, any32x4 &_z) { 
	s32 src[12];
	s32 dst[12];

	memcpy(src +  0, &_x, sizeof(_x));
	memcpy(src + 16, &_y, sizeof(_y));
	memcpy(src + 32, &_z, sizeof(_z));

	for (u32 i = 0; i < 3; ++i) {
		for (u32 j = 0; j < 4; ++j) {
			r.s[i * 4 + j]  = v.s[i + j * 3];
		}
	}
}
forceinline v4fx4 pack(v4fx4 v) { 
	v4fx4 r;
#if ARCH_AVX2
	__m256 &v0 = ((__m256 *)&v)[0];
	__m256 &v1 = ((__m256 *)&v)[1];
	__m256 &r0 = ((__m256 *)&r)[0];
	__m256 &r1 = ((__m256 *)&r)[1];
	__m256i idx = _mm256_setr_epi32(0,4,1,5,2,6,3,7);

    r1 = _mm256_unpacklo_ps(v0, v1);
    r0 = _mm256_unpackhi_ps(v0, v1);
    v1 = _mm256_permutevar8x32_ps(r1, idx);
    v0 = _mm256_permutevar8x32_ps(r0, idx);
    r0 = v1;
    r1 = v0;

#else
	for (u32 i = 0; i < 4; ++i) {
		for (u32 j = 0; j < 4; ++j) {
			r.s[i * 4 + j]  = v.s[i + j * 4]; 
			r.s[i * 4 + j]  = v.s[i + j * 4];	 
			r.s[i * 4 + j]  = v.s[i + j * 4];
			r.s[i * 4 + j]  = v.s[i + j * 4];
		}
	}
#endif
	return r;
}
forceinline v2fx8 pack(v2fx8 v) {
	v2fx8 r;
	for (u32 i = 0; i < 2; ++i) {
		for (u32 j = 0; j < 8; ++j) {
			r.s[i * 8 + j]  = v.s[i + j * 2];
		}
	}
	return r;
}
forceinline v3fx8 pack(v3fx8 v) { 
	v3fx8 r;
	for (u32 i = 0; i < 3; ++i) {
		for (u32 j = 0; j < 8; ++j) {
			r.s[i * 8 + j]  = v.s[i + j * 3];
		}
	}
	return r;
}
forceinline v4fx8 pack(v4fx8 v) { 
	v4fx8 r;
	for (u32 i = 0; i < 4; ++i) {
		for (u32 j = 0; j < 8; ++j) {
			r.s[i * 8 + j]  = v.s[i + j * 4];
		}
	}
	return r;
}
forceinline v2fx4 unpack(v2fx4 v) { 
	v2fx4 r;
	r.x.ps = _mm_unpacklo_ps(v.x.ps, v.y.ps);
	r.y.ps = _mm_unpackhi_ps(v.x.ps, v.y.ps);
	return r;
}
forceinline v3fx4 unpack(v3fx4 v) { 
	auto xy = SHUFFLE(v.x.ps, 0, 2, v.y.ps, 0, 2);
	auto yz = SHUFFLE(v.y.ps, 1, 3, v.z.ps, 1, 3);
	auto xz = SHUFFLE(v.x.ps, 1, 3, v.z.ps, 0, 2);
	v.x.ps = SHUFFLE(xy, 0, 2, xz, 2, 0);
	v.y.ps = SHUFFLE(yz, 0, 2, xy, 1, 3);
	v.z.ps = SHUFFLE(xz, 3, 1, yz, 1, 3);
	return v;
}
forceinline v4fx4 unpack(v4fx4 v) {
	return pack(v);
}
forceinline v2fx8 unpack(v2fx8 v) { 
	v2fx8 r;
#if ARCH_AVX
	__m256 m02 = _mm256_unpacklo_ps(v.x.ps, v.y.ps);
	__m256 m13 = _mm256_unpackhi_ps(v.x.ps, v.y.ps);
	r.x.ps = _mm256_setr_m128(_mm256_extractf128_ps(m02, 0), _mm256_extractf128_ps(m13, 0));
	r.y.ps = _mm256_setr_m128(_mm256_extractf128_ps(m02, 1), _mm256_extractf128_ps(m13, 1));
#else
	for (u32 i = 0; i < 8; ++i){
		for (u32 j = 0; j < 2; ++j){
			r.s[i * 2 + j] = v.s[i + j * 8];
		}
	}
#endif
	return r;
}
#if ARCH_AVX
#define UNPACK(v3fx8)													 \
forceinline v3fx8 unpack(v3fx8 v) {										 \
	auto xy = _mm256_shuffle_ps(v.x.ps, v.y.ps, MM_SHUFFLE(0, 2, 0, 2)); \
	auto yz = _mm256_shuffle_ps(v.y.ps, v.z.ps, MM_SHUFFLE(1, 3, 1, 3)); \
	auto xz = _mm256_shuffle_ps(v.x.ps, v.z.ps, MM_SHUFFLE(1, 3, 0, 2)); \
	auto rx = _mm256_shuffle_ps(xy, xz, MM_SHUFFLE(0, 2, 2, 0));		 \
	auto ry = _mm256_shuffle_ps(yz, xy, MM_SHUFFLE(0, 2, 1, 3));		 \
	auto rz = _mm256_shuffle_ps(xz, yz, MM_SHUFFLE(3, 1, 1, 3));		 \
	v.x.ps = _mm256_permute2f128_ps(rx, ry, MM256_PERM128(0, 2));		 \
	v.y.ps = _mm256_permute2f128_ps(rz, rx, MM256_PERM128(0, 3));		 \
	v.z.ps = _mm256_permute2f128_ps(ry, rz, MM256_PERM128(1, 3));		 \
	return v;															 \
}
#else
#define UNPACK(v3fx8)						 \
forceinline v3fx8 unpack(v3fx8 v) {			 \
	v3fx8 r;								 \
	for (u32 i = 0; i < 8; ++i) {			 \
		for (u32 j = 0; j < 3; ++j){		 \
			r.s[i * 3 + j] = v.s[i + j * 8]; \
		}									 \
	}										 \
	return r;								 \
}
#endif
UNPACK(v3fx8) UNPACK(v3sx8) UNPACK(v3ux8)
#undef UNPACK
forceinline v4fx8 unpack(v4fx8 v) {
	v4fx8 r;
	for (u32 i = 0; i < 8; ++i) {
		for (u32 j = 0; j < 4; ++j){
			r.s[i * 4 + j] = v.s[i + j * 8];
		}
	}
	return r;
}
// clang-format on

#endif

// clang-format off
#define MINMAX(v2f, V2f)						  \
	forceinline auto min(v2f a) { return min(a.x, a.y); }\
	forceinline auto max(v2f a) { return max(a.x, a.y); }\
	forceinline auto min(v2f a, v2f b) { return V2f(min(a.x, b.x), min(a.y, b.y)); }\
	forceinline auto max(v2f a, v2f b) { return V2f(max(a.x, b.x), max(a.y, b.y)); }\
	forceinline void minmax(v2f a, v2f b, v2f& mn, v2f& mx) { \
		minmax(a.x, b.x, mn.x, mx.x);		  \
		minmax(a.y, b.y, mn.y, mx.y); 		  \
	}
MINMAX(v2f, V2f)
MINMAX(v2s, V2s)
MINMAX(v2u, V2u)
#undef MINMAX

#define MINMAX(v3f, V3f)						  \
	forceinline auto min(v3f a) { return min(a.x, a.y, a.z); }\
	forceinline auto max(v3f a) { return max(a.x, a.y, a.z); }\
	forceinline auto min(v3f a, v3f b) { return V3f(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }\
	forceinline auto max(v3f a, v3f b) { return V3f(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)); }\
	forceinline void minmax(v3f a, v3f b, v3f& mn, v3f& mx) { \
		minmax(a.x, b.x, mn.x, mx.x);		  \
		minmax(a.y, b.y, mn.y, mx.y); 		  \
		minmax(a.z, b.z, mn.z, mx.z); 		  \
	}
MINMAX(v3f, V3f)
MINMAX(v3s, V3s)
MINMAX(v3u, V3u)
#undef MINMAX

#define MINMAX(v4f, V4f)						  \
	forceinline auto min(v4f a) { return min(a.x, a.y, a.z, a.w); }\
	forceinline auto max(v4f a) { return max(a.x, a.y, a.z, a.w); }\
	forceinline auto min(v4f a, v4f b) { return V4f(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w)); }\
	forceinline auto max(v4f a, v4f b) { return V4f(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w)); }\
	forceinline void minmax(v4f a, v4f b, v4f& mn, v4f& mx) { \
		minmax(a.x, b.x, mn.x, mx.x);		  \
		minmax(a.y, b.y, mn.y, mx.y); 		  \
		minmax(a.z, b.z, mn.z, mx.z); 		  \
		minmax(a.w, b.w, mn.w, mx.w); 		  \
	}
MINMAX(v4f, V4f)
MINMAX(v4s, V4s)
MINMAX(v4u, V4u)
#undef MINMAX
// clang-format on

#define HALF(f32) forceinline f32 half(f32 v) { return v * 0.5f; }
HALF(f32)
HALF(v2f)
HALF(v3f)
HALF(v4f)
#undef HALF

#define HALF(s32) forceinline s32 half(s32 v) { return v / 2; }
HALF(s32)
HALF(v2s)
HALF(v3s)
HALF(v4s)
#undef HALF

#define HALF(u32) forceinline u32 half(u32 v) { return v >> 1; }
HALF(u32)
HALF(v2u)
HALF(v3u)
HALF(v4u)
#undef HALF

forceinline f32 floor(f32 v) { return ::floorf(v); }
forceinline f64 floor(f64 v) { return ::floor(v); }
forceinline v2f floor(v2f v) { return { floor(v.x), floor(v.y) }; }
forceinline v3f floor(v3f v) { return { floor(v.x), floor(v.y), floor(v.z) }; }
forceinline v4f floor(v4f v) { return { floor(v.x), floor(v.y), floor(v.z), floor(v.w) }; }

#define FRAC(f32) forceinline f32 frac(f32 v) { return v - floor(v); }
FRAC(f32)
FRAC(f64)
#undef FRAC

forceinline v2f frac(v2f v) { return { frac(v.x), frac(v.y) }; }
forceinline v3f frac(v3f v) { return { frac(v.x), frac(v.y), frac(v.z) }; }
forceinline v4f frac(v4f v) { return { frac(v.x), frac(v.y), frac(v.z), frac(v.w) }; }

forceinline v2s frac(v2s v, v2s s) {
	__m128i vm = _mm_setr_epi32(v.x, v.y, 0, 0);
	__m128i sm = _mm_setr_epi32(s.x, s.y, 0, 0);
	__m128d vlo = _mm_cvtepi32_pd(vm);
	__m128d slo = _mm_cvtepi32_pd(sm);
	__m128i result = _mm_cvttpd_epi32(_mm_floor_pd(_mm_div_pd(vlo, slo)));
	result = _mm_sub_epi32(vm, _mm_mullo_epi32(result, sm));
	return *(v2s *)&result;
}
forceinline v3s frac(v3s v, v3s s) {
	return {
		frac(v.x, s.x),
		frac(v.y, s.y),
		frac(v.z, s.z),
	};
}
forceinline v4s frac(v4s v, v4s step) {
	return {
		frac(v.x, step.x),
		frac(v.y, step.y),
		frac(v.z, step.z),
		frac(v.w, step.w),
	};
}
forceinline v2s frac(v2s v, s32 s) { return frac(v, V2s(s)); }
forceinline v3s frac(v3s v, s32 s) { return frac(v, V3s(s)); }
forceinline v4s frac(v4s v, s32 s) { return frac(v, V4s(s)); }

#define FLOOR(v2s, s32, V2s) 		   \
forceinline v2s floor(v2s v, v2s s) {  \
	return { 						   \
		floor(v.x, s.x), 			   \
		floor(v.y, s.y), 			   \
	}; 								   \
} 									   \
forceinline v2s floor(v2s v, s32 s) { return floor(v, V2s(s)); }
FLOOR(v2s, s32, V2s)
FLOOR(v2u, u32, V2u)
#undef FLOOR

#define FLOOR(v3s, s32, V3s) 		  \
forceinline v3s floor(v3s v, v3s s) { \
	return { 						  \
		floor(v.x, s.x), 			  \
		floor(v.y, s.y), 			  \
		floor(v.z, s.z), 			  \
	}; 								  \
} 									  \
forceinline v3s floor(v3s v, s32 s) { return floor(v, V3s(s)); }
FLOOR(v3s, s32, V3s)
FLOOR(v3u, u32, V3u)
#undef FLOOR

#define FLOOR(v4s, s32, V4s) 		  \
forceinline v4s floor(v4s v, v4s s) { \
	return {						  \
		floor(v.x, s.x),			  \
		floor(v.y, s.y),			  \
		floor(v.z, s.z),			  \
		floor(v.w, s.w),			  \
	};								  \
}									  \
forceinline v4s floor(v4s v, s32 s) { return floor(v, V4s(s)); }
FLOOR(v4s, s32, V4s)
FLOOR(v4u, u32, V4u)
#undef FLOOR

forceinline s32 floor_to_int(f32 v) { return (s32)floor(v); }
forceinline s64 floor_to_int(f64 v) { return (s64)floor(v); }
forceinline v2s floor_to_int(v2f v) {
	return {
		floor_to_int(v.x),
		floor_to_int(v.y),
	};
}
forceinline v3s floor_to_int(v3f v) {
	return {
		floor_to_int(v.x),
		floor_to_int(v.y),
		floor_to_int(v.z),
	};
}
forceinline v4s floor_to_int(v4f v) {
	return {
		floor_to_int(v.x),
		floor_to_int(v.y),
		floor_to_int(v.z),
		floor_to_int(v.w),
	};
}

forceinline v2f ceil(v2f v) { return {ceil(v.x), ceil(v.y)}; }
forceinline v3f ceil(v3f v) { return {ceil(v.x), ceil(v.y), ceil(v.z)}; }
forceinline v4f ceil(v4f v) { return {ceil(v.x), ceil(v.y), ceil(v.z), ceil(v.w)}; }

#define CEIL(t) forceinline t ceil(t v, t s) { return floor(v + s - 1, s); }
CEIL(v2s) CEIL(v3s) CEIL(v4s);
#undef CEIL

#define CEIL(s32, f32) forceinline s32 ceil_to_int(f32 v) { return (s32)ceil(v); }
CEIL(v2s, v2f)
CEIL(v3s, v3f)
CEIL(v4s, v4f)
#undef CEIL

forceinline f32 round(f32 v) { return ::roundf(v); }
forceinline v2f round(v2f v) { return {round(v.x), round(v.y)}; }
forceinline v3f round(v3f v) { return {round(v.x), round(v.y), round(v.z)}; }
forceinline v4f round(v4f v) { return {round(v.x), round(v.y), round(v.z), round(v.w)}; }

forceinline s32 round_to_int(f32 v) { return (s32)lroundf(v); }
forceinline s64 round_to_int(f64 v) { return llround(v); }
forceinline v2s round_to_int(v2f v) { return {round_to_int(v.x), round_to_int(v.y)}; }
forceinline v3s round_to_int(v3f v) { return {round_to_int(v.x), round_to_int(v.y), round_to_int(v.z)}; }
forceinline v4s round_to_int(v4f v) { return {round_to_int(v.x), round_to_int(v.y), round_to_int(v.z), round_to_int(v.w)}; }

forceinline f32 absolute(f32 v) { return *(u32*)&v &= 0x7FFFFFFF, v; }
forceinline v2f absolute(v2f v) { return {absolute(v.x), absolute(v.y)}; }
forceinline v3f absolute(v3f v) { return {absolute(v.x), absolute(v.y), absolute(v.z)}; }
forceinline v4f absolute(v4f v) { return {absolute(v.x), absolute(v.y), absolute(v.z), absolute(v.w)}; }

forceinline s32 absolute(s32 v) { return v < 0 ? -v :v; }
forceinline v2s absolute(v2s a) { return {absolute(a.x), absolute(a.y)}; }
forceinline v3s absolute(v3s a) { return {absolute(a.x), absolute(a.y), absolute(a.z)}; }
forceinline v4s absolute(v4s a) { return {absolute(a.x), absolute(a.y), absolute(a.z), absolute(a.w)}; }

forceinline u32 absolute(u32 v) { return v; }

forceinline f64 absolute(f64 v) { return *(u64*)&v &= 0x7FFFFFFFFFFFFFFF, v; }

forceinline f32 set_sign(f32 dst, f32 src) {
    *(u32*)&dst &= 0x7FFFFFFF;
    *(u32*)&dst |= *(u32*)&src & 0x80000000;
    return dst;
}
forceinline f64 set_sign(f64 dst, f64 src) {
    *(u64*)&dst &= 0x7FFFFFFFFFFFFFFF;
    *(u64*)&dst |= *(u64*)&src & 0x8000000000000000;
    return dst;
}

forceinline s8 sign(s8  v) { return ((v > 0) ? 1 : ((v < 0) ? -1 : 0)); }
forceinline s8 sign(s16 v) { return ((v > 0) ? 1 : ((v < 0) ? -1 : 0)); }
forceinline s8 sign(s32 v) { return ((v > 0) ? 1 : ((v < 0) ? -1 : 0)); }
forceinline s8 sign(s64 v) { return ((v > 0) ? 1 : ((v < 0) ? -1 : 0)); }

forceinline f32 sign(f32 v) { return set_sign(1.0f, v); }
forceinline v2f sign(v2f v) { return {sign(v.x), sign(v.y)}; }
forceinline v3f sign(v3f v) { return {sign(v.x), sign(v.y), sign(v.z)}; }
forceinline v4f sign(v4f v) { return {sign(v.x), sign(v.y), sign(v.z), sign(v.w)}; }

forceinline f32 trunc(f32 v) { return _mm_cvtss_f32(_mm_round_ps(_mm_set_ss(v), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)); };
forceinline v2f trunc(v2f v) { return {trunc(v.x), trunc(v.y)}; }
forceinline v3f trunc(v3f v) { return {trunc(v.x), trunc(v.y), trunc(v.z)}; }
forceinline v4f trunc(v4f v) { return {trunc(v.x), trunc(v.y), trunc(v.z), trunc(v.w)}; }

forceinline f64 trunc(f64 v) { return _mm_cvtsd_f64(_mm_round_pd(_mm_set_sd(v), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)); };

#define MOD(f32) \
forceinline f32 modulo(f32 a, f32 b) { return set_sign(a - trunc(a / b) * b, a); } \
forceinline f32 positive_modulo(f32 a, f32 b) { return frac(a / b) * b; }
MOD(f32)
MOD(f64)
#undef MOD

forceinline v2f modulo(v2f a, v2f b) { return {modulo(a.x, b.x), modulo(a.y, b.y)}; }
forceinline v3f modulo(v3f a, v3f b) { return {modulo(a.x, b.x), modulo(a.y, b.y), modulo(a.z, b.z)}; }
forceinline v4f modulo(v4f a, v4f b) { return {modulo(a.x, b.x), modulo(a.y, b.y), modulo(a.z, b.z), modulo(a.w, b.w)}; }

forceinline v2f positive_modulo(v2f a, v2f b) { return {positive_modulo(a.x, b.x), positive_modulo(a.y, b.y)}; }
forceinline v3f positive_modulo(v3f a, v3f b) { return {positive_modulo(a.x, b.x), positive_modulo(a.y, b.y), positive_modulo(a.z, b.z)}; }
forceinline v4f positive_modulo(v4f a, v4f b) { return {positive_modulo(a.x, b.x), positive_modulo(a.y, b.y), positive_modulo(a.z, b.z), positive_modulo(a.w, b.w)}; }

forceinline f32 sqrt(f32 v) { return sqrtf(v); }
forceinline v2f sqrt(v2f v) { return {sqrtf(v.x), sqrtf(v.y)}; }
forceinline v3f sqrt(v3f v) { return {sqrtf(v.x), sqrtf(v.y), sqrtf(v.z)}; }
forceinline v4f sqrt(v4f v) { return {sqrtf(v.x), sqrtf(v.y), sqrtf(v.z), sqrtf(v.w)}; }

forceinline f32 reciprocal(f32 v) { return _mm_cvtss_f32(_mm_rcp_ss(_mm_set_ss(v))); }
forceinline v2f reciprocal(v2f v) { return {reciprocal(v.x), reciprocal(v.y)}; }
forceinline v3f reciprocal(v3f v) { return {reciprocal(v.x), reciprocal(v.y), reciprocal(v.z)}; }
forceinline v4f reciprocal(v4f v) { return {reciprocal(v.x), reciprocal(v.y), reciprocal(v.z), reciprocal(v.w)}; }

forceinline f32 sin(f32 v) { return ::sinf(v); }
forceinline v2f sin(v2f v) { return {sin(v.x), sin(v.y)}; }
forceinline v3f sin(v3f v) { return {sin(v.x), sin(v.y), sin(v.z)}; }
forceinline v4f sin(v4f v) { return {sin(v.x), sin(v.y), sin(v.z), sin(v.w)}; }

forceinline f32 cos(f32 v) { return ::cosf(v); }
forceinline v2f cos(v2f v) { return {cos(v.x), cos(v.y)}; }
forceinline v3f cos(v3f v) { return {cos(v.x), cos(v.y), cos(v.z)}; }
forceinline v4f cos(v4f v) { return {cos(v.x), cos(v.y), cos(v.z), cos(v.w)}; }

forceinline f32 tan(f32 v) { return ::tanf(v); }
forceinline v2f tan(v2f v) { return {tan(v.x), tan(v.y)}; }
forceinline v3f tan(v3f v) { return {tan(v.x), tan(v.y), tan(v.z)}; }
forceinline v4f tan(v4f v) { return {tan(v.x), tan(v.y), tan(v.z), tan(v.w)}; }

forceinline void sincos(f32 v, f32 &_sin, f32 &_cos) { _sin = sin(v); _cos = cos(v); }
forceinline void sincos(v2f v, v2f &_sin, v2f &_cos) { _sin = sin(v); _cos = cos(v); }
forceinline void sincos(v3f v, v3f &_sin, v3f &_cos) { _sin = sin(v); _cos = cos(v); }
forceinline void sincos(v4f v, v4f &_sin, v4f &_cos) { _sin = sin(v); _cos = cos(v); }

forceinline void sincos(f32 v, v2f &result) { result.y = sin(v); result.x = cos(v); }

forceinline v2f sincos(f32 v) { return {cos(v), sin(v)}; }

forceinline f32 sin_bhaskara(f32 v) {
	v = positive_modulo(v, pi * 2);
	auto mask = v >= pi;
	v = select(mask, v - pi, v);
	return (16 * v * (pi - v)) * reciprocal(5 * pi * pi - 4 * v * (pi - v)) * select(mask, -1.0f, 1.0f);
}
forceinline f32 cos_bhaskara(f32 v) { return sin_bhaskara(v + pi*0.5f); }

forceinline v2f sincos_bhaskara(f32 v) { return {cos_bhaskara(v), sin_bhaskara(v)}; }

forceinline f32 atan2(f32 y, f32 x) { return ::atan2f(y, x); }
forceinline f32 atan2(v2f v) { return atan2(v.y, v.x); }

forceinline f32 atan2_approx(f32 y, f32 x) {
	auto ay = absolute(y) + 1e-10f;
	auto mask = x < 0;
	auto r = select(mask, (x + ay) / (ay - x), (x - ay) / (ay + x));
	auto angle = select(mask, pi * 0.75f, pi * 0.25f);
	angle += (0.1963f * r * r - 0.9817f) * r;
	return select(y < 0, -angle, angle);
}
forceinline f32 atan2_approx(v2f v) { return atan2_approx(v.y, v.x); }

template <class A, class B, class T, class S>
forceinline constexpr auto lerpWrap(A a, B b, T t, S s) {
	a = positiveModulo(a, s);
	b = positiveModulo(b, s);
	auto d = a - b;
	return select(absolute(d) > half(s), 
				  positiveModulo(lerp(a, b+sign(d)*s, t), s), 
				  lerp(a, b, t));
}

forceinline constexpr f32 dot(f32 a, f32 b) { return a * b; }

#define DOT(f32, v2f) forceinline constexpr f32 dot(v2f a, v2f b) { return a *= b, a.x + a.y; }
DOT(f32, v2f)
DOT(s32, v2s)
DOT(u32, v2u)
DOT(s64, v3s64)
#undef DOT

#define DOT(f32, v3f) forceinline constexpr f32 dot(v3f a, v3f b) { return a *= b, a.x + a.y + a.z; }
DOT(f32, v3f)
DOT(s32, v3s)
DOT(u32, v3u)
#undef DOT

#define DOT(f32, v4f) forceinline constexpr f32 dot(v4f a, v4f b) { return a *= b, a.x + a.y + a.z + a.w; }
DOT(f32, v4f)
DOT(s32, v4s)
DOT(u32, v4u)
#undef DOT

template <class T>
forceinline auto reflect(T v, T n) {
	return v - dot(v, n) * n * 2;
}

forceinline v2f cross(v2f a) { return {a.y, -a.x}; }
forceinline v2s cross(v2s a) { return {a.y, -a.x}; }
forceinline v3f cross(v3f a, v3f b) {
	return {
		a.y * b.z - a.z * b.y, 
		a.z * b.x - a.x * b.z, 
		a.x * b.y - a.y * b.x
	};
}

forceinline f32 sum(v2f v) { return v.x + v.y; }

forceinline f32 rsqrt(f32 v) { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(v))); }

template <class T>
forceinline auto length_squared(T a) {
	return dot(a, a);
}
template <class T>
forceinline auto length(T a) {
	return sqrt(dot(a, a));
}
template <class T>
forceinline auto normalize(T a) {
	return a * rsqrt(dot(a, a));
}
template <class T>
forceinline auto normalize(T a, T fallback) {
	auto lsq = length_squared(a);
	if (lsq == decltype(lsq){})
		return fallback;
	return a * (1.0f / sqrt(lsq));
}
template <class T>
forceinline auto distance_squared(T a, T b) {
	return length_squared(a - b);
}
template <class T>
forceinline auto distance(T a, T b) {
	return sqrt(distance_squared(a, b));
}
template <> forceinline auto distance(f32 a, f32 b) { return absolute(a - b); }
template <> forceinline auto distance(f64 a, f64 b) { return absolute(a - b); }
template <class T>
forceinline auto manhattan(T a, T b) {
	return sum(abs(a - b));
}
forceinline s32 maxDistance(v3s a, v3s b) {
	a = absolute(a - b);
	return max(max(a.x, a.y), a.z);
}

template <class T>
forceinline constexpr auto move_toward(T a, T b, f32 t) {
	auto d = b - a;
	if (length_squared(d) > t * t) {
		return a + normalize(d) * t;
	} else {
		return b;
	}
}

template <class T>
forceinline constexpr auto moveAway(T a, T b, f32 t) {
	return a + normalize(a - b) * t;
}

template <class T>
forceinline constexpr T project(T vector, T normal) {
	return normal * dot(normal, vector);
}

forceinline f32 cos01(f32 t) { return 0.5f - cosf(t * pi) * 0.5f; }

forceinline v3f hsv_to_rgb(f32 h, f32 s, f32 v) {
	h = frac(h);
	f32 c = v * s;
	f32 x = c * (1 - absolute(modulo(h * 6, 2) - 1));
	v3f m = V3f(v - c);
	     if (h < ( 60 / 360.0f)) { m += v3f{c, x, 0}; }
	else if (h < (120 / 360.0f)) { m += v3f{x, c, 0}; }
	else if (h < (180 / 360.0f)) { m += v3f{0, c, x}; }
	else if (h < (240 / 360.0f)) { m += v3f{0, x, c}; }
	else if (h < (300 / 360.0f)) { m += v3f{x, 0, c}; }
	else                         { m += v3f{c, 0, x}; }

	return m;
}
forceinline v3f hsv_to_rgb(v3f hsv) { return hsv_to_rgb(hsv.x, hsv.y, hsv.z); }

inline v3f rgb_to_hsv(f32 r, f32 g, f32 b) {
	f32 cMax = max(max(r, g), b);
	f32 cMin = min(min(r, g), b);
	f32 delta = cMax - cMin;
	v3f result;

    result.z = cMax;
    if (delta == 0) {
        result.x = result.y = 0;
        return result;
    }
    if (cMax == 0) {
        result.x = result.y = 0;
        return result;
    } else {
        result.y = (delta / cMax);
    }
         if (r >= cMax) result.x = (g - b) / delta;
    else if (g >= cMax) result.x = (b - r) / delta + 2.0f;
    else                result.x = (r - g) / delta + 4.0f;

    result.x = frac(result.x * 1.0f / 6.0f);

    return result;
}
forceinline v3f rgb_to_hsv(v3f rgb) { return rgb_to_hsv(rgb.x, rgb.y, rgb.z); }

template<class T>
struct line {
	T a, b;
};

template <class T>
forceinline line<T> line_begin_end(T begin, T end) {
	return {begin, end};
}

template <class T>
forceinline line<T> line_begin_dir(T begin, T dir) {
	return {begin, begin + dir};
}

template<class T>
struct ray {
	T begin, dir;
};

template <class T>
forceinline ray<T> ray_begin_end(T begin, T end) {
	return {begin, end - begin};
}

template <class T>
forceinline ray<T> ray_begin_dir(T begin, T dir) {
	return {begin, dir};
}

template <class T>
struct triangle {
	T a, b, c;
};

template<class T>
struct aabb {
	T min, max;
	T size() const { return max - min; }
	T middle() const { 
		T sum = max + min;
		sum *= 0.5f;
		return sum;
	}
	template <class U>
	aabb<T> operator*(U const &b) { return {min * b, max * b}; }
	bool operator==(aabb const &that) const { return min == that.min && max == that.max; }
	bool operator!=(aabb const &that) const { return min != that.min || max != that.max; }
	template <class U>
	explicit operator aabb<U>() const { return { (U)min, (U)max }; }
};

template <class T>
forceinline aabb<T> aabb_min_max(T min, T max) {
	return {min, max};
}
template <class T>
forceinline aabb<T> aabbMinDim(T min, T dim) {
	return {min, min + dim};
}
template <class T>
forceinline aabb<T> aabbCenterDim(T center, T dim) {
	dim = half(dim);
	return {center - dim, center + dim};
}
template <class T>
forceinline aabb<T> aabbCenterRadius(T center, T radius) {
	return {center - radius, center + radius};
}

#define for_aabb3(x, y, z, condition, box)           \
for (auto z = box.min.z; z condition box.max.z; ++z) \
for (auto y = box.min.y; y condition box.max.y; ++y) \
for (auto x = box.min.x; x condition box.max.x; ++x)

#define IN_BOUNDS2(bool, v2fA, v2fB) 			       \
	forceinline bool in_bounds(v2fA v, aabb<v2fB> b) { \
		return 								           \
			(v.x >= b.min.x) && 			           \
			(v.y >= b.min.y) && 			           \
			(v.x < b.max.x) && 				           \
			(v.y < b.max.y); 				           \
	}
IN_BOUNDS2(bool, v2f, v2f)
IN_BOUNDS2(bool, v2s, v2s)
IN_BOUNDS2(bool, v2u, v2u)
#undef IN_BOUNDS2

#define IN_BOUNDS3(bool, v3fA, v3fB) 			       \
	forceinline bool in_bounds(v3fA v, aabb<v3fB> b) { \
		return 								           \
			(v.x >= b.min.x) && 			           \
			(v.y >= b.min.y) &&				           \
			(v.z >= b.min.z) &&				           \
			(v.x < b.max.x) &&				           \
			(v.y < b.max.y) &&				           \
			(v.z < b.max.z);				           \
	}
IN_BOUNDS3(bool, v3f, v3f)
IN_BOUNDS3(bool, v3s, v3s)
IN_BOUNDS3(bool, v3u, v3u)
#undef IN_BOUNDS3

#define INTERSECTS2(bool, v2f)						        \
	forceinline bool intersects(aabb<v2f> a, aabb<v2f> b) { \
		return 										        \
			(a.min.x < b.max.x) && 					        \
			(a.min.y < b.max.y) && 					        \
			(a.max.x > b.min.x) && 					        \
			(a.max.y > b.min.y); 					        \
	}
INTERSECTS2(bool, v2f)
INTERSECTS2(bool, v2s)
INTERSECTS2(bool, v2u)
#undef INTERSECTS2

#define INTERSECTS3(bool, v3f)						        \
	forceinline bool intersects(aabb<v3f> a, aabb<v3f> b) { \
		return 										        \
			(a.min.x < b.max.x) && 					        \
			(a.min.y < b.max.y) && 					        \
			(a.min.z < b.max.z) && 					        \
			(a.max.x > b.min.x) && 					        \
			(a.max.y > b.min.y) && 					        \
			(a.max.z > b.min.z); 					        \
	}
INTERSECTS3(bool, v3f)
INTERSECTS3(bool, v3s)
INTERSECTS3(bool, v3u)
#undef INTERSECTS3

template <class T>
forceinline aabb<T> operator&(aabb<T> const &a, aabb<T> const &b) {
	aabb<T> result;
	result.min = max(a.min, b.min);
	result.max = min(a.max, b.max);
	return result;
}

forceinline s32 volume(aabb<v2s> const &box) {
	v2s diameter = box.max - box.min;
	return diameter.x * diameter.y;
}
forceinline s32 volume(aabb<v3s> const &box) {
	v3s diameter = box.max - box.min;
	return diameter.x * diameter.y * diameter.z;
}

// Axis aligned bounding box subrtacion routines
// a - b
// subtract b from a
//
// subtract_volume treats a and b as volumes
//
// subtract_points treats a and b as sets of points; 
// Every aabb in result contains unique set of points.
//
// In both cases result's max point is inclusive
//

inline StaticList<aabb<v2s>, 8> subtract_volumes(aabb<v2s> a, aabb<v2s> b) {
	b.min = clamp(b.min, a.min, a.max);
	b.max = clamp(b.max, a.min, a.max);
	StaticList<aabb<v2s>, 8> boxes = {
		aabb_min_max(a.min, b.min),
		aabb_min_max(b.max, a.max),
		aabb_min_max(v2s{a.min.x, b.max.y}, v2s{b.min.x, a.max.y}),
		aabb_min_max(v2s{b.max.x, a.min.y}, v2s{a.max.x, b.min.y}),
		aabb_min_max(v2s{b.min.x, a.min.y}, v2s{b.max.x, b.min.y}),
		aabb_min_max(v2s{b.min.x, b.max.y}, v2s{b.max.x, a.max.y}),
		aabb_min_max(v2s{a.min.x, b.min.y}, v2s{b.min.x, b.max.y}),
		aabb_min_max(v2s{b.max.x, b.min.y}, v2s{a.max.x, b.max.y}),
	};
	for (u32 i = 0; i < boxes.size; ++i) {
		if (volume(boxes[i]) <= 0) {
			boxes.erase_at_unordered(i--);
		}
	}
	return boxes;
}
inline StaticList<aabb<v3s>, 26> subtract_volumes(aabb<v3s> a, aabb<v3s> b) {
	b.min = clamp(b.min, a.min, a.max);
	b.max = clamp(b.max, a.min, a.max);
	StaticList<aabb<v3s>, 26> boxes = {
		// Corners
		aabb_min_max(a.min, b.min),                                                   // left bottom back
		aabb_min_max(b.max, a.max),                                                   // right top front
		aabb_min_max(v3s{a.min.x, b.max.y, a.min.z}, v3s{b.min.x, a.max.y, b.min.z}), // left top back
		aabb_min_max(v3s{b.max.x, b.max.y, a.min.z}, v3s{a.max.x, a.max.y, b.min.z}), // right top back
		aabb_min_max(v3s{a.min.x, a.min.y, b.max.z}, v3s{b.min.x, b.min.y, a.max.z}), // left bottom front
		aabb_min_max(v3s{b.max.x, a.min.y, b.max.z}, v3s{a.max.x, b.min.y, a.max.z}), // right bottom front
		aabb_min_max(v3s{b.max.x, a.min.y, a.min.z}, v3s{a.max.x, b.min.y, b.min.z}), // right bottom back
		aabb_min_max(v3s{a.min.x, b.max.y, b.max.z}, v3s{b.min.x, a.max.y, a.max.z}), // left top front

		// Edges
		aabb_min_max(v3s{b.min.x, a.min.y, a.min.z}, v3s{b.max.x, b.min.y, b.min.z}), // bottom back
		aabb_min_max(v3s{b.min.x, b.max.y, a.min.z}, v3s{b.max.x, a.max.y, b.min.z}), // top back
		aabb_min_max(v3s{b.min.x, a.min.y, b.max.z}, v3s{b.max.x, b.min.y, a.max.z}), // bottom front
		aabb_min_max(v3s{b.min.x, b.max.y, b.max.z}, v3s{b.max.x, a.max.y, a.max.z}), // top front
		aabb_min_max(v3s{a.min.x, b.min.y, a.min.z}, v3s{b.min.x, b.max.y, b.min.z}), // right back
		aabb_min_max(v3s{b.max.x, b.min.y, a.min.z}, v3s{a.max.x, b.max.y, b.min.z}), // left back
		aabb_min_max(v3s{a.min.x, b.min.y, b.max.z}, v3s{b.min.x, b.max.y, a.max.z}), // right front
		aabb_min_max(v3s{b.max.x, b.min.y, b.max.z}, v3s{a.max.x, b.max.y, a.max.z}), // left front
		aabb_min_max(v3s{a.min.x, b.max.y, b.min.z}, v3s{b.min.x, a.max.y, b.max.z}), // left top
		aabb_min_max(v3s{b.max.x, b.max.y, b.min.z}, v3s{a.max.x, a.max.y, b.max.z}), // right top
		aabb_min_max(v3s{a.min.x, a.min.y, b.min.z}, v3s{b.min.x, b.min.y, b.max.z}), // left bottom
		aabb_min_max(v3s{b.max.x, a.min.y, b.min.z}, v3s{a.max.x, b.min.y, b.max.z}), // right bottom

		// Faces
		aabb_min_max(v3s{b.min.x, b.min.y, a.min.z}, v3s{b.max.x, b.max.y, b.min.z}), // back
		aabb_min_max(v3s{b.min.x, b.min.y, b.max.z}, v3s{b.max.x, b.max.y, a.max.z}), // front
		aabb_min_max(v3s{a.min.x, b.min.y, b.min.z}, v3s{b.min.x, b.max.y, b.max.z}), // left
		aabb_min_max(v3s{b.max.x, b.min.y, b.min.z}, v3s{a.max.x, b.max.y, b.max.z}), // right
		aabb_min_max(v3s{b.min.x, b.max.y, b.min.z}, v3s{b.max.x, a.max.y, b.max.z}), // top
		aabb_min_max(v3s{b.min.x, a.min.y, b.min.z}, v3s{b.max.x, b.min.y, b.max.z}), // bottom
	};
	for (u32 i = 0; i < boxes.size; ++i) {
		if (volume(boxes[i]) <= 0) {
			boxes.erase_at_unordered(i--);
		}
	}
	return boxes;
}
inline StaticList<aabb<v2s>, 8> subtract_points(aabb<v2s> a, aabb<v2s> b) {
	aabb<v2s> original_b = b;
	b.min = clamp(b.min, a.min, a.max);
	b.max = clamp(b.max, a.min, a.max);
	auto boxes = subtract_volumes(a, b);
	for (auto &box : boxes) {
		box.min.x += (original_b.max.x == b.max.x && box.min.x == b.max.x);
		box.min.y += (original_b.max.y == b.max.y && box.min.y == b.max.y);
		box.max.x -= (original_b.min.x == b.min.x && box.max.x == b.min.x);
		box.max.y -= (original_b.min.y == b.min.y && box.max.y == b.min.y);
	}
	return boxes;
}
inline StaticList<aabb<v3s>, 26> subtract_points(aabb<v3s> a, aabb<v3s> b) {
	aabb<v3s> original_b = b;
	b.min = clamp(b.min, a.min, a.max);
	b.max = clamp(b.max, a.min, a.max);
	auto boxes = subtract_volumes(a, b);
	for (auto &box : boxes) {
		box.min.x += (original_b.max.x == b.max.x && box.min.x == b.max.x);
		box.min.y += (original_b.max.y == b.max.y && box.min.y == b.max.y);
		box.min.z += (original_b.max.z == b.max.z && box.min.z == b.max.z);
		box.max.x -= (original_b.min.x == b.min.x && box.max.x == b.min.x);
		box.max.y -= (original_b.min.y == b.min.y && box.max.y == b.min.y);
		box.max.z -= (original_b.min.z == b.min.z && box.max.z == b.min.z);
	}
	return boxes;
}

inline StaticList<aabb<v2s>, 9> combine_volumes(aabb<v2s> const &a, aabb<v2s> const &b) {
	StaticList<aabb<v2s>, 9> result = { a };
	result += subtract_volumes(b, a);
	return result;
}
inline StaticList<aabb<v3s>, 27> combine_volumes(aabb<v3s> const &a, aabb<v3s> const &b) {
	StaticList<aabb<v3s>, 27> result = { a };
	result += subtract_volumes(b, a);
	return result;
}
inline StaticList<aabb<v2s>, 9> combine_points(aabb<v2s> const &a, aabb<v2s> const &b) {
	StaticList<aabb<v2s>, 9> result = { a };
	result += subtract_points(b, a);
	return result;
}
inline StaticList<aabb<v3s>, 27> combine_points(aabb<v3s> const &a, aabb<v3s> const &b) {
	StaticList<aabb<v3s>, 27> result = { a };
	result += subtract_points(b, a);
	return result;
}

forceinline bool intersects(line<v2f> line, aabb<v2f> aabb) {
	v2f dir = normalize(line.b - line.a);
	v2f rdir = 1.0f / dir;
	v2f vMin = (aabb.min - line.a) * rdir;
	v2f vMax = (aabb.max - line.a) * rdir;

	f32 tMin = max(min(vMin.x, vMax.x), min(vMin.y, vMax.y));
	f32 tMax = min(max(vMin.x, vMax.x), max(vMin.y, vMax.y));

	return tMax > 0 && tMin < tMax;
}

forceinline bool intersects(line<v3f> line, aabb<v3f> aabb) {
	v3f dir = normalize(line.b - line.a);
	v3f rdir = 1.0f / dir;
	v3f vMin = (aabb.min - line.a) * rdir;
	v3f vMax = (aabb.max - line.a) * rdir;

	f32 tMin = max(min(vMin.x, vMax.x), min(vMin.y, vMax.y), min(vMin.z, vMax.z));
	f32 tMax = min(max(vMin.x, vMax.x), max(vMin.y, vMax.y), max(vMin.z, vMax.z));

	return tMax > 0 && tMin < tMax;
}
#if 0
forceinline bool raycastLine(v2f a, v2f b, v2f c, v2f d, v2f& point, v2f& normal) {
	v2f ba = b - a;
	v2f dc = d - c;
	v2f ac = a - c;

	v2f s = v2f{ba.x, dc.x} * ac.y - v2f{ba.y, dc.y} * ac.x;
	s /= ba.x * dc.y - dc.x * ba.y;

	if (s.x >= 0 && s.x <= 1 && s.y >= 0 && s.y <= 1) {
		point  = a + (s.y * ba);
		normal = cross(normalize(c - d));
		if (dot(b - a, normal) > 0)
			normal *= -1;
		return true;
	}
	return false;
}

forceinline bool raycastPlane(v3f a, v3f b, v3f p1, v3f p2, v3f p3, v3f& point, v3f& normal) {
	auto p21 = p2 - p1;
	auto p31 = p3 - p1;
	normal	 = normalize(cross(p21, p31));

	auto r = a - b;

	f32 nr = dot(normal, r);

	if (nr <= 1e-5f) {
		return false;
	}

	f32 t = dot(-normal, a - p1) / nr;
	point = a + r * t;

	auto dp1 = point - p1;
	f32 u	 = dot(dp1, p21);
	f32 v	 = dot(dp1, p31);

	if (length_squared(a - point) > length_squared(a - b) || dot(a - point, a - b) <= 0) {
		return false;
	}

	return u >= 0.0f && u <= dot(p21, p21) && v >= 0.0f && v <= dot(p31, p31);
}
#endif
struct RaycastHit {
	bool hit = false;
	v3f position = {};
	v3f normal = {};
};

inline RaycastHit raycast_triangle(ray<v3f> ray, triangle<v3f> tri) {
    v3f e1 = tri.b - tri.a;
    v3f e2 = tri.c - tri.a;
    // ���������� ������� ������� � ���������
    v3f pvec = cross(ray.dir, e2);
    f32 det = dot(e1, pvec);

    // ��� ���������� ���������
    if (det < 1e-8 && det > -1e-8) {
		return {};
    }

    f32 inv_det = 1 / det;
    v3f tvec = ray.begin - tri.a;
    f32 u = dot(tvec, pvec) * inv_det;
    if (u < 0 || u > 1) {
		return {};
    }

    v3f qvec = cross(tvec, e1);
    f32 v = dot(ray.dir, qvec) * inv_det;
    if (v < 0 || u + v > 1) {
		return {};
    }
    f32 t = dot(e2, qvec) * inv_det;

	if (t < 0)
		return {};

	RaycastHit hit;
	hit.hit = true;
	hit.position = ray.begin + ray.dir* t;
	hit.normal = normalize(cross(e1, e2));
	return hit;
}

inline RaycastHit raycast(ray<v3f> ray, aabb<v3f> box) {
	v3f dirfrac = 1.0f / ray.dir;
	v3f t1 = (box.min - ray.begin)*dirfrac;
	v3f t2 = (box.max - ray.begin)*dirfrac;
	
	f32 tmin = max(min(t1.x, t2.x), min(t1.y, t2.y), min(t1.z, t2.z));
	f32 tmax = min(max(t1.x, t2.x), max(t1.y, t2.y), max(t1.z, t2.z));

	if (tmin < 0 || tmax < 0 || tmin > tmax) {
		return {};
	}

	RaycastHit hit = {};
	hit.hit = true;
	hit.position = ray.begin + ray.dir * tmin;
	assert(dot(ray.dir, hit.position - ray.begin) > 0);
	return hit;
}

template <class T, umm size>
forceinline constexpr T linearSample(const T (&arr)[size], f32 t) noexcept {
	f32 f = frac(t) * size;
	s32 a = (s32)f;
	s32 b = a + 1;
	if (b == size)
		b = 0;
	return lerp(arr[a], arr[b], frac(f));
}

union m4 {
	struct {
		v4f i, j, k, l;
	};
	struct {
		__m128 im, jm, km, lm;
	};
	__m128 m[4];
	v4f vectors[4];
	f32 s[16];
	forceinline v4f operator*(v4f const &b) const {
		v4f x = V4f(b.x) * i;
		v4f y = V4f(b.y) * j;
		v4f z = V4f(b.z) * k;
		v4f w = V4f(b.w) * l;
		return x + y + z + w;
	}
	forceinline v3f operator*(v3f const &b) const {
		auto x = V4f(b.x) * i;
		auto y = V4f(b.y) * j;
		auto z = V4f(b.z) * k;
		auto r = x + y + z;
		v3f result;
		memcpy(&result, &r, 12);
		return result;
	}
	forceinline m4 operator*(m4 const &b) const { return {*this * b.i, *this * b.j, *this * b.k, *this * b.l}; }
	forceinline m4& operator*=(m4 const &b) { return *this = *this * b; }
	static forceinline m4 scale(f32 x, f32 y, f32 z) {
		// clang-format off
		return {
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1
		};
		// clang-format on
	}
	static forceinline m4 identity() { return {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}; }
	static forceinline m4 scale(v3f v) { return scale(v.x, v.y, v.z); }
	static forceinline m4 scale(v2f xy, f32 z) { return scale(xy.x, xy.y, z); }
	static forceinline m4 scale(f32 v) { return scale(v, v, v); }
	static forceinline m4 translation(f32 x, f32 y, f32 z) {
		// clang-format off
		return {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1,
		};
		// clang-format on
	}
	static forceinline m4 translation(v3f v) { return translation(v.x, v.y, v.z); }
	static forceinline m4 translation(v2f xy, f32 z) { return translation(xy.x, xy.y, z); }
	static forceinline m4 translation(f32 v) { return translation(v, v, v); }
	static forceinline m4 perspectiveLH(f32 aspect, f32 fov, f32 nz, f32 fz) {
		f32 h	   = 1.0f / tanf(fov * 0.5f);
		f32 w	   = h / aspect;
		f32 fzdfmn = fz / (fz - nz);
		// clang-format off
		return {
			w, 0, 0, 0, 
			0, h, 0, 0, 
			0, 0, fzdfmn, 1, 
			0, 0, -fzdfmn * nz, 0
		};
		// clang-format on
	}
	static forceinline m4 perspectiveRH(f32 aspect, f32 fov, f32 nz, f32 fz) {
		f32 xymax = nz * tan(fov * 0.5f);
		f32 depth = fz - nz;
		f32 q = -(fz + nz) / depth;
		f32 qn = -2 * (fz * nz) / depth;
		f32 h = nz / xymax;
		f32 w = h / aspect;

		return {
			w, 0, 0, 0,
			0, h, 0, 0,
			0, 0, q, -1,
			0, 0, qn, 0,
		};
	}
	static forceinline m4 ortho_right_handed(f32 height, f32 aspect, f32 nz, f32 fz) {
		f32 h = 1.0f / (height * 0.5f);
		f32 rz = fz - nz;
		return {
			h / aspect, 0, 0, 0,
			0, h, 0, 0,
			0, 0, 1.0f / rz, 0,
			0, 0, -nz / rz, 1,
		};
	}
	static forceinline m4 rotationX(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			1, 0, 0, 0,
			0, c, s, 0,
			0,-s, c, 0,
			0, 0, 0, 1
		};
		// clang-format on
	}
	static forceinline m4 rotationY(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			 c, 0, s, 0,
			 0, 1, 0, 0,
			-s, 0, c, 0,
			 0, 0, 0, 1
		};
		// clang-format on
	}
	static forceinline m4 rotationZ(f32 a) {
		f32 s = sinf(a), c = cosf(a);
		// clang-format off
		return {
			 c, s, 0, 0,
			-s, c, 0, 0,
			 0, 0, 1, 0,
			 0, 0, 0, 1
		};
		// clang-format on
	}
	// Roll, Pitch, Yaw (ZXY)
	static forceinline m4 rotationZXY(v3f v) {
		v3f sin;
		v3f cos;
		sincos(v, sin, cos);
		f32 a = sin.x;
		f32 c = sin.y;
		f32 e = sin.z;
		f32 b = cos.x;
		f32 d = cos.y;
		f32 f = cos.z;
		// clang-format off
		return {
			 d * f - a * c * e,  b * e, c * f + a * d * e, 0,
			-a * c * f - d * e,  b * f, a * d * f - c * e, 0,
			            -b * c,     -a,             b * d, 0,
			                 0,      0,                 0, 1,
		};
		// clang-format on
	}
	static forceinline m4 rotationZXY(f32 x, f32 y, f32 z) { return rotationZXY({x, y, z}); }
	// Yaw, Pitch, Roll (YXZ)
	static forceinline m4 rotationYXZ(v3f v) {
		v3f sin;
		v3f cos;
		sincos(v, sin, cos);
		f32 a = sin.x;
		f32 c = sin.y;
		f32 e = sin.z;
		f32 b = cos.x;
		f32 d = cos.y;
		f32 f = cos.z;
		// clang-format off
		return {
			 d * f + a * c * e,  -a * c * f + d * e, b * c, 0,
			            -b * e,               b * f,     a, 0,
			-c * f + a * d * e,  -a * d * f - c * e, b * d, 0,
			                 0,                   0,     0, 1,
		};
		// clang-format on
	}
	static forceinline m4 rotationYXZ(f32 x, f32 y, f32 z) { return rotationYXZ({x, y, z}); }
};

forceinline constexpr m3 transpose(m3 const& m) {
	return {
		m.i.x, m.j.x, m.k.x, m.i.y, m.j.y, m.k.y, m.i.z, m.j.z, m.k.z,
	};
}
forceinline m4 transpose(m4 const& m) {
	__m128 tmp0 = _mm_unpacklo_ps(m.im, m.jm);
	__m128 tmp1 = _mm_unpackhi_ps(m.im, m.jm);
	__m128 tmp2 = _mm_unpacklo_ps(m.km, m.lm);
	__m128 tmp3 = _mm_unpackhi_ps(m.km, m.lm);

	m4 result;
	result.im = _mm_movelh_ps(tmp0, tmp2);
	result.jm = _mm_movehl_ps(tmp2, tmp0);
	result.km = _mm_movelh_ps(tmp1, tmp3);
	result.lm = _mm_movehl_ps(tmp3, tmp1);
	return result;
}

inline m4 inverse(m4 const &m) {
	f32 A2323 = m.k.z * m.l.w - m.k.w * m.l.z;
	f32 A1323 = m.k.y * m.l.w - m.k.w * m.l.y;
	f32 A1223 = m.k.y * m.l.z - m.k.z * m.l.y;
	f32 A0323 = m.k.x * m.l.w - m.k.w * m.l.x;
	f32 A0223 = m.k.x * m.l.z - m.k.z * m.l.x;
	f32 A0123 = m.k.x * m.l.y - m.k.y * m.l.x;
	f32 A2313 = m.j.z * m.l.w - m.j.w * m.l.z;
	f32 A1313 = m.j.y * m.l.w - m.j.w * m.l.y;
	f32 A1213 = m.j.y * m.l.z - m.j.z * m.l.y;
	f32 A2312 = m.j.z * m.k.w - m.j.w * m.k.z;
	f32 A1312 = m.j.y * m.k.w - m.j.w * m.k.y;
	f32 A1212 = m.j.y * m.k.z - m.j.z * m.k.y;
	f32 A0313 = m.j.x * m.l.w - m.j.w * m.l.x;
	f32 A0213 = m.j.x * m.l.z - m.j.z * m.l.x;
	f32 A0312 = m.j.x * m.k.w - m.j.w * m.k.x;
	f32 A0212 = m.j.x * m.k.z - m.j.z * m.k.x;
	f32 A0113 = m.j.x * m.l.y - m.j.y * m.l.x;
	f32 A0112 = m.j.x * m.k.y - m.j.y * m.k.x;

	f32 det = m.i.x * (m.j.y * A2323 - m.j.z * A1323 + m.j.w * A1223) 
		    - m.i.y * (m.j.x * A2323 - m.j.z * A0323 + m.j.w * A0223) 
		    + m.i.z * (m.j.x * A1323 - m.j.y * A0323 + m.j.w * A0123) 
		    - m.i.w * (m.j.x * A1223 - m.j.y * A0223 + m.j.z * A0123);

	if (det == 0)
		return {};

	det = 1 / det;

	return m4 {
	   det *  (m.j.y * A2323 - m.j.z * A1323 + m.j.w * A1223),
	   det * -(m.i.y * A2323 - m.i.z * A1323 + m.i.w * A1223),
	   det *  (m.i.y * A2313 - m.i.z * A1313 + m.i.w * A1213),
	   det * -(m.i.y * A2312 - m.i.z * A1312 + m.i.w * A1212),
	   det * -(m.j.x * A2323 - m.j.z * A0323 + m.j.w * A0223),
	   det *  (m.i.x * A2323 - m.i.z * A0323 + m.i.w * A0223),
	   det * -(m.i.x * A2313 - m.i.z * A0313 + m.i.w * A0213),
	   det *  (m.i.x * A2312 - m.i.z * A0312 + m.i.w * A0212),
	   det *  (m.j.x * A1323 - m.j.y * A0323 + m.j.w * A0123),
	   det * -(m.i.x * A1323 - m.i.y * A0323 + m.i.w * A0123),
	   det *  (m.i.x * A1313 - m.i.y * A0313 + m.i.w * A0113),
	   det * -(m.i.x * A1312 - m.i.y * A0312 + m.i.w * A0112),
	   det * -(m.j.x * A1223 - m.j.y * A0223 + m.j.z * A0123),
	   det *  (m.i.x * A1223 - m.i.y * A0223 + m.i.z * A0123),
	   det * -(m.i.x * A1213 - m.i.y * A0213 + m.i.z * A0113),
	   det *  (m.i.x * A1212 - m.i.y * A0212 + m.i.z * A0112),
	};
}

forceinline constexpr m4 M4(f32 v = 0.0f) { return m4{v, v, v, v, v, v, v, v, v, v, v, v, v, v, v, v}; }
forceinline constexpr m4 M4(v4f i, v4f j, v4f k, v4f l) { return m4{i, j, k, l}; }
forceinline constexpr m4 M4(__m128 i, __m128 j, __m128 k, __m128 l) {
	m4 r{};
	r.m[0] = i;
	r.m[1] = j;
	r.m[2] = k;
	r.m[3] = l;
	return r;
}
forceinline constexpr m4 M4(f32 ix, f32 iy, f32 iz, f32 iw, f32 jx, f32 jy, f32 jz, f32 jw, f32 kx, f32 ky, f32 kz, f32 kw, f32 lx,
				f32 ly, f32 lz, f32 lw) {
	return {ix, iy, iz, iw, jx, jy, jz, jw, kx, ky, kz, kw, lx, ly, lz, lw};
}

forceinline m4 M4(m3 v) {
	return {
		V4f(v.i, 0),
		V4f(v.j, 0),
		V4f(v.k, 0),
		V4f(0, 0, 0, 1),
	};
}

using FrustumPlanes = Array<v4f, 6>;

forceinline FrustumPlanes create_frustum_planes_d3d(m4 m) {
	FrustumPlanes planes;
	
	// http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf
	planes[0].x = m.i.w + m.i.x;
	planes[0].y = m.j.w + m.j.x;
	planes[0].z = m.k.w + m.k.x;
	planes[0].w = m.l.w + m.l.x;
	planes[1].x = m.i.w - m.i.x;
	planes[1].y = m.j.w - m.j.x;
	planes[1].z = m.k.w - m.k.x;
	planes[1].w = m.l.w - m.l.x;
	planes[2].x = m.i.w + m.i.y;
	planes[2].y = m.j.w + m.j.y;
	planes[2].z = m.k.w + m.k.y;
	planes[2].w = m.l.w + m.l.y;
	planes[3].x = m.i.w - m.i.y;
	planes[3].y = m.j.w - m.j.y;
	planes[3].z = m.k.w - m.k.y;
	planes[3].w = m.l.w - m.l.y;
	planes[4].x = m.i.z;
	planes[4].y = m.j.z;
	planes[4].z = m.k.z;
	planes[4].w = m.l.z;
	planes[5].x = m.i.w - m.i.z;
	planes[5].y = m.j.w - m.j.z;
	planes[5].z = m.k.w - m.k.z;
	planes[5].w = m.l.w - m.l.z;

	for (auto& p : planes) {
		p /= length(v3f{p.x, p.y, p.z});
	}
	return planes;
}
forceinline FrustumPlanes create_frustum_planes_gl(m4 m) {
	FrustumPlanes planes;
	
	// http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf
	planes[0].x = m.i.w + m.i.x;
	planes[0].y = m.j.w + m.j.x;
	planes[0].z = m.k.w + m.k.x;
	planes[0].w = m.l.w + m.l.x;
	planes[1].x = m.i.w - m.i.x;
	planes[1].y = m.j.w - m.j.x;
	planes[1].z = m.k.w - m.k.x;
	planes[1].w = m.l.w - m.l.x;
	planes[2].x = m.i.w + m.i.y;
	planes[2].y = m.j.w + m.j.y;
	planes[2].z = m.k.w + m.k.y;
	planes[2].w = m.l.w + m.l.y;
	planes[3].x = m.i.w - m.i.y;
	planes[3].y = m.j.w - m.j.y;
	planes[3].z = m.k.w - m.k.y;
	planes[3].w = m.l.w - m.l.y;
	planes[4].x = m.i.w + m.i.z;
	planes[4].y = m.j.w + m.j.z;
	planes[4].z = m.k.w + m.k.z;
	planes[4].w = m.l.w + m.l.z;
	planes[5].x = m.i.w - m.i.z;
	planes[5].y = m.j.w - m.j.z;
	planes[5].z = m.k.w - m.k.z;
	planes[5].w = m.l.w - m.l.z;

	for (auto& p : planes) {
		p /= length(v3f{p.x, p.y, p.z});
	}
	return planes;
}
forceinline bool contains_sphere(FrustumPlanes const &planes, v3f position, f32 radius) {
	for (auto p : planes) {
		if (dot(v3f{p.x, p.y, p.z}, position) + p.w + radius < 0) {
			return false;
		}
	}
	return true;
}

namespace CE {

forceinline constexpr u32 count_bits(u32 v) {
	u32 s = 0;
	u32 r = v;
	for (u32 i = 0; i < 32; ++i) {
		s += r & 1;
		r >>= 1;
	}
	return s;
}
forceinline constexpr v4s frac(v4s v, s32 step) {
	return {
		TL::frac(v.x, step),
		TL::frac(v.y, step),
		TL::frac(v.z, step),
		TL::frac(v.w, step),
	};
}

} // namespace CE

#define TO_STRING_V3(v3f)													  \
template <class Char, class CopyFn, class = EnableIf<is_copy_fn<CopyFn, Char>>> \
CopyFnRet<CopyFn, Char> to_string(v3f v, CopyFn &&copyFn) {					  \
	StaticList<Char, 256> buffer;											  \
	buffer += '{';															  \
	to_string<Char>(v.x, [&](Span<Char const> span) { buffer += span; });	  \
	buffer += as_span(", ");												  \
	to_string<Char>(v.y, [&](Span<Char const> span) { buffer += span; });	  \
	buffer += as_span(", ");												  \
	to_string<Char>(v.z, [&](Span<Char const> span) { buffer += span; });	  \
	buffer += '}';															  \
	return copyFn(as_span(buffer));											  \
}

TO_STRING_V3(v3f)
TO_STRING_V3(v3u)
TO_STRING_V3(v3s)

#undef TO_STRING_V3

template <class Char, class T, class CopyFn, class = EnableIf<is_copy_fn<CopyFn, Char>>> 
CopyFnRet<CopyFn, Char> to_string(aabb<T> v, CopyFn &&copyFn) {					  
	StaticList<Char, 256> buffer;											  
	buffer += as_span("{ min: ");															  
	to_string<Char>(v.min, [&](Span<Char const> span) { buffer += span; });	  
	buffer += as_span(", max: ");												  
	to_string<Char>(v.max, [&](Span<Char const> span) { buffer += span; });	  
	buffer += as_span(" }");															  
	return copyFn(as_span(buffer));											  
}

template <class To, class From> forceinline To cvt(From v) { return (To)v; }
template <> forceinline v2f cvt(f32 v) { return V2f(v); }
template <> forceinline v3f cvt(f32 v) { return V3f(v); }
template <> forceinline v2f cvt(s32 v) { return V2f((f32)v); }
template <> forceinline v3f cvt(s32 v) { return V3f((f32)v); }
template <> forceinline v2f cvt(u32 v) { return V2f((f32)v); }
template <> forceinline v3f cvt(u32 v) { return V3f((f32)v); }

template <class T>
forceinline T saturate(T t) {
	return clamp(t, cvt<T>(0), cvt<T>(1));
}

template <class T>
forceinline T smoothstep(T t) {
	t = saturate(t);
	return t * t * t * (t * (t * 6 - 15) + 10);
}

} // namespace TL

#undef MOP
#undef MOPS
#undef OPS
#undef SOP

#undef MM_SHUFFLE
#undef MM256_PERM128

#undef f32x4_lt
#undef f32x4_gt
#undef f32x4_le
#undef f32x4_ge
#undef f32x4_eq
#undef f32x4_ne
#undef f64x2_lt
#undef f64x2_gt
#undef f64x2_le
#undef f64x2_ge
#undef f64x2_eq
#undef f64x2_ne

#if COMPILER_MSVC
#pragma warning(pop)
#endif
