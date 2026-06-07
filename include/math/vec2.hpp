#pragma once

#include <immintrin.h>

namespace clz::math
{
	struct alignas(16) vec2
	{
		union
		{
			__m128 xmm;
			struct {
				float x, y, _pad1, _pad2;
			};
		};

		vec2() : xmm(_mm_setzero_ps()) {}

		vec2(const float x, const float y)
		    : xmm(_mm_set_ps(0, 0, y, x)) {}

		vec2(const __m128 xmm) : xmm(xmm) {}

	};

	inline vec2 add(const vec2& lhs, const vec2& rhs)
	{
		return vec2(_mm_add_ps(lhs.xmm, rhs.xmm));
	}

	inline vec2 subtract(const vec2& lhs, const vec2& rhs)
	{
		return vec2(_mm_sub_ps(lhs.xmm, rhs.xmm));
	}

	inline vec2 scalar_product(const vec2& lhs, const float scalar)
	{
		return vec2(_mm_mul_ps(lhs.xmm, _mm_set1_ps(scalar)));
	}
	inline vec2 component_product(const vec2& lhs, const vec2& rhs)
	{
		return vec2(_mm_mul_ps(lhs.xmm, rhs.xmm));
	}
	inline float dot_product(const vec2& lhs, const vec2& rhs)
	{
		const __m128 res = _mm_dp_ps(lhs.xmm, rhs.xmm, 0x31);
		return _mm_cvtss_f32(res);
	}

	inline float getLength(const vec2& v)
	{
		const __m128 sLength = _mm_dp_ps(v.xmm, v.xmm, 0x31);
		return _mm_cvtss_f32(_mm_sqrt_ss(sLength));
	}
	inline vec2 normalize(const vec2& v)
	{
		const __m128 sLength = _mm_dp_ps(v.xmm, v.xmm, 0x33);
		return vec2(_mm_mul_ps(v.xmm, _mm_rsqrt_ps(sLength)));
	}
}