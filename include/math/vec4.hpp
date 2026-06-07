#pragma once

#include <immintrin.h>

namespace clz::math
{
	struct alignas(16) vec4
	{
		union
		{
			__m128 xmm;
			struct {
				float x, y, z, w;
			};
		};

		vec4() : xmm(_mm_setzero_ps()) {}

		vec4(const float x, const float y, const float z, const float w)
		    : xmm(_mm_set_ps(w, z, y, x)) {}

		vec4(const __m128 xmm) : xmm(xmm) {}

	};

	inline vec4 add(const vec4& lhs, const vec4& rhs)
	{
		return vec4(_mm_add_ps(lhs.xmm, rhs.xmm));
	}

	inline vec4 subtract(const vec4& lhs, const vec4& rhs)
	{
		return vec4(_mm_sub_ps(lhs.xmm, rhs.xmm));
	}

	inline vec4 scalar_product(const vec4& lhs, const float scalar)
	{
		return vec4(_mm_mul_ps(lhs.xmm, _mm_set1_ps(scalar)));
	}
	inline vec4 component_product(const vec4& lhs, const vec4& rhs)
	{
		return vec4(_mm_mul_ps(lhs.xmm, rhs.xmm));
	}
	inline float dot_product(const vec4& lhs, const vec4& rhs)
	{
		const auto res = _mm_dp_ps(lhs.xmm, rhs.xmm, 0xf1);
		return _mm_cvtss_f32(res);
	}

}