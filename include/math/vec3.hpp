/**
 * @file vec3.hpp
 * @brief SIMD accelerated 3-component float vector.
 * @author curl0z
 *
 * Uses SSE4.1 intrinsics via __m128 for all operations.
 * The fourth lane (_pad) is always zero and ignored in all math.
 * Requires 16-byte alignment — guaranteed by alignas(16).
 * x86-64 only.
 */
#pragma once

#include <immintrin.h>

namespace clz::math
{
	/**
	 * @brief 3-component float vector backed by a 128-bit SSE register.
	 *
	 * Internally uses a __m128 register with the fourth lane (_pad) zeroed.
	 * Members x, y, z are accessible directly
	 * Not recommended to extract values via the underlying xmm register.
	 * All instances are 16-byte aligned for safe use with aligned SSE loads/stores.
	 */
	struct alignas(16) vec3
	{
		union {
			__m128 xmm;
			struct
			{
				float x, y, z, _pad;
			};
		};

		/// @brief Initializes all components to zero.
		vec3() : xmm(_mm_setzero_ps())
		{
		}

		/**
		 * @brief Constructs a vec3 from three float values.
		 * @param x X component.
		 * @param y Y component.
		 * @param z Z component.
		 */
		vec3(const float x, const float y, const float z) : xmm(_mm_set_ps(0, z, y, x))
		{
		}

		/**
		 * @brief Constructs a vec3 directly from a __m128 register.
		 * @param xmm Source SSE register.
		 */
		vec3(const __m128 xmm) : xmm(xmm)
		{
		}
	};

	/**
	 * @brief Component-wise addition of two vec3s.
	 * @param lhs Left operand.
	 * @param rhs Right operand.
	 * @return lhs + rhs
	 */
	inline vec3 add(const vec3& lhs, const vec3& rhs)
	{
		return vec3(_mm_add_ps(lhs.xmm, rhs.xmm));
	}

	/**
	 * @brief Component-wise subtraction of two vec3s.
	 * @param lhs Left operand.
	 * @param rhs Right operand.
	 * @return lhs - rhs
	 */
	inline vec3 subtract(const vec3& lhs, const vec3& rhs)
	{
		return vec3(_mm_sub_ps(lhs.xmm, rhs.xmm));
	}

	/**
	 * @brief Multiplies all components of a vec3 by a scalar.
	 * @param lhs Source vector.
	 * @param scalar Scalar multiplier.
	 * @return lhs * scalar
	 */
	inline vec3 scalar_product(const vec3& lhs, const float scalar)
	{
		return vec3(_mm_mul_ps(lhs.xmm, _mm_set1_ps(scalar)));
	}

	/**
	 * @brief Component-wise multiplication of two vec3s.
	 * @param lhs Left operand.
	 * @param rhs Right operand.
	 * @return lhs * rhs (per component)
	 */
	inline vec3 component_product(const vec3& lhs, const vec3& rhs)
	{
		return vec3(_mm_mul_ps(lhs.xmm, rhs.xmm));
	}

	/**
	 * @brief Computes the dot product of two vec3s.
	 * @param lhs Left operand.
	 * @param rhs Right operand.
	 * @return Scalar dot product of lhs and rhs.
	 */
	inline float dot_product(const vec3& lhs, const vec3& rhs)
	{
		// 0x71 = 0111 0001 — multiply xyz only, store in slot 0
		const __m128 res = _mm_dp_ps(lhs.xmm, rhs.xmm, 0x71);
		return _mm_cvtss_f32(res);
	}

	/**
	 * @brief Computes the length (magnitude) of a vec3.
	 * @param lhs Source vector.
	 * @return Scalar length of lhs.
	 */
	inline float getLength(const vec3& lhs)
	{
		// dot(v, v) = squared length, then sqrt
		const __m128 lSquare = _mm_dp_ps(lhs.xmm, lhs.xmm, 0x71);
		return _mm_cvtss_f32(_mm_sqrt_ss(lSquare));
	}

	/**
	 * @brief Returns a normalized (unit length) copy of a vec3.
	 * @param lhs Source vector.
	 * @return lhs scaled to unit length.
	 * @note Uses _mm_rsqrt_ps — fast approximation with ~0.037% max error.
	 * @note Also, not recommended if you want high precision
	 */
	inline vec3 normalize(const vec3& lhs)
	{
		// 0x77 = 0111 0111 — broadcast squared length into xyz lanes for division
		const __m128 length = _mm_dp_ps(lhs.xmm, lhs.xmm, 0x77);
		return vec3(_mm_mul_ps(lhs.xmm, _mm_rsqrt_ps(length)));
	}

} // namespace clz::math