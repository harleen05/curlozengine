#pragma once

#include <immintrin.h>

namespace clz::math
{
	struct alignas(16) vec3
	{
		union
		{
			__m128 xmm;
			struct {
				float x, y, z, _pad;
			};
		};

		vec3() : xmm(_mm_setzero_ps()) {}

		vec3(const float x, const float y, const float z)
		    : xmm(_mm_set_ps(0, z, y, x)) {}

		vec3(const __m128 xmm) : xmm(xmm) {}

	};

	inline vec3 add(const vec3& lhs, const vec3& rhs)
	{
		return vec3(_mm_add_ps(lhs.xmm, rhs.xmm));
	}

	inline vec3 subtract(const vec3& lhs, const vec3& rhs)
	{
		return vec3(_mm_sub_ps(lhs.xmm, rhs.xmm));
	}

	inline vec3 scalar_product(const vec3& lhs, const float scalar)
	{
		return vec3(_mm_mul_ps(lhs.xmm, _mm_set1_ps(scalar)));
	}
	inline vec3 component_product(const vec3& lhs, const vec3& rhs)
	{
		return vec3(_mm_mul_ps(lhs.xmm, rhs.xmm));
	}
	inline float dot_product(const vec3& lhs, const vec3& rhs)
	{
		const __m128 res = _mm_dp_ps(lhs.xmm, rhs.xmm, 0x71);
		return _mm_cvtss_f32(res);
	}

	inline float getLength(const vec3& lhs)
	{
		const __m128 lSquare = _mm_dp_ps(lhs.xmm, lhs.xmm, 0x71);
		return _mm_cvtss_f32(_mm_sqrt_ss(lSquare));
	}

	inline vec3 normalize(const vec3& lhs)
	{
		const __m128 length = _mm_dp_ps(lhs.xmm, lhs.xmm, 0x77);
		return vec3(_mm_mul_ps(lhs.xmm, _mm_rsqrt_ps(length)));
	}
}