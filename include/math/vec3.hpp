/**
 * @file vec3.hpp
 * @brief SIMD accelerated 3-component float vector.
 * @author curl0z
 *
 * Uses SSE4.1 intrinsics via __m128 for all operations.
 * x86-64 only.
 */
#pragma once

#include <immintrin.h>

namespace clz::math
{
	/**
	 * @brief 3-component float vector backed by a 128-bit SSE register.
	 *
	 * Members x, y, z are accessible directly
	 * Not recommended to extract values via the underlying xmm register.
	 * All instances are 16-byte aligned for safe use with aligned SSE loads/stores.
	 */
	struct vec3
	{
		float x, y, z;

		/// @brief Initializes all components to zero.
		vec3() : x(0.0f), y(0.0f), z(0.0f) {}

		/**
		 * @brief Constructs a vec3 from three float values.
		 * @param x X component.
		 * @param y Y component.
		 * @param z Z component.
		 */
		vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

		/**
		 * @brief Constructs a vec3 
		 * with x y z all set to a single float value
		 * @param x X component.
		 * @param y Y component.
		 * @param z Z component.
		 */
		vec3(const float value) : x(value), y(value), z(value) {}


		/**
		 * @brief Constructs a vec3 directly from a __m128 register.
		 * @param xmm Source SSE register.
		 */
		vec3(const __m128& xmm)
		{
			x = _mm_cvtss_f32(xmm);
			__m128 res = _mm_shuffle_ps(xmm, xmm, _MM_SHUFFLE(1, 1, 1, 1));
			y = _mm_cvtss_f32(res);
			res = _mm_shuffle_ps(xmm, xmm, _MM_SHUFFLE(2, 2, 2, 2));
			z = _mm_cvtss_f32(res);
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
		__m128 vec1 = _mm_set_ps(0, lhs.z, lhs.y, lhs.x);
		__m128 vec2 = _mm_set_ps(0, rhs.z, rhs.y, rhs.x);
		return vec3(_mm_add_ps(vec1, vec2));
	}

	/**
	 * @brief Component-wise subtraction of two vec3s.
	 * @param lhs Left operand.
	 * @param rhs Right operand.
	 * @return lhs - rhs
	 */
	inline vec3 subtract(const vec3& lhs, const vec3& rhs)
	{
		__m128 vec1 = _mm_set_ps(0, lhs.z, lhs.y, lhs.x);
		__m128 vec2 = _mm_set_ps(0, rhs.z, rhs.y, rhs.x);
		return vec3(_mm_sub_ps(vec1, vec2));
	}

	/**
	 * @brief Multiplies all components of a vec3 by a scalar.
	 * @param lhs Source vector.
	 * @param scalar Scalar multiplier.
	 * @return lhs * scalar
	 */
	inline vec3 scalar_product(const vec3& lhs, const float scalar)
	{
		return vec3(lhs.x * scalar, lhs.y * scalar, lhs.z * scalar);
	}

	/**
	 * @brief Component-wise multiplication of two vec3s.
	 * @param lhs Left operand.
	 * @param rhs Right operand.
	 * @return lhs * rhs (per component)
	 */
	inline vec3 component_product(const vec3& lhs, const vec3& rhs)
	{
		return vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
	}

	/**
	 * @brief Computes the dot product of two vec3s.
	 * @param lhs Left operand.
	 * @param rhs Right operand.
	 * @return Scalar dot product of lhs and rhs.
	 */
	inline float dot_product(const vec3& lhs, const vec3& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	/**
	 * @brief Computes the length (magnitude) of a vec3.
	 * @param lhs Source vector.
	 * @return Scalar length of lhs.
	 */
	inline float getLength(const vec3& lhs)
	{
		__m128 vec = _mm_set_ps(0, lhs.z, lhs.y, lhs.x);
		// dot(v, v) = squared length, then sqrt
		const __m128 lSquare = _mm_dp_ps(vec, vec, 0x71);
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
		__m128 vec = _mm_set_ps(0, lhs.z, lhs.y, lhs.x);
		// 0x77 = 0111 0111 — broadcast squared length into xyz lanes for division
		const __m128 length = _mm_dp_ps(vec, vec, 0x77);
		return vec3(_mm_mul_ps(vec, _mm_rsqrt_ps(length)));
	}

} // namespace clz::math
