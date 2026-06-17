/**
 * @file mat4x4.hpp
 * @brief SIMD accelerated 4x4 matrix
 * @author curl0z
 *
 * Uses SSE4.1 and FMA instrinsics
 */
#pragma once

#include <immintrin.h>

namespace clz::math
{
	/**
	 * @brief 16-component float vector backed by 4 128-bit SSE register.
	 *
	 * It's not recommented to access matrix data directly	
	 * All instances are 16-byte aligned for safe use with aligned SSE loads/stores.
	 */
        struct alignas(16) mat4
        {
                union {
			struct 
			{
				__m128 r0;
				__m128 r1;
				__m128 r2;
				__m128 r3;
			};

                        struct
                        {
                                float xx=0, xy=0, xz=0, xw=0;
                                float yx=0, yy=0, yz=0, yw=0;
                                float zx=0, zy=0, zz=0, zw=0;
                                float wx=0, wy=0, wz=0, ww=0;
                        };
			float data[16];
                };

                mat4() {}

		/// @brief Initializes Matrix in an identity format
                mat4(float value): 
			xx(value), yy(value), zz(value), ww(value) {}

		/// @brief Initializes the 4 __m128 registers
                mat4(__m128 r0, __m128 r1, __m128 r2, __m128 r3): 
			r0(r0), r1(r1), r2(r2), r3(r3) {}
        };

	/**
	 * @brief Returns Addition of two matrices
	 * @param m1 matrix 1
	 * @param m2 matrix 2
	 * @return Sum of m1 and m1 matrices
	 * Takes roughly ~15-20 cycles
	 */
        inline mat4 add(const mat4& m1, const mat4& m2)
        {
                return mat4(_mm_add_ps(m1.r0, m2.r0),
				_mm_add_ps(m1.r1, m2.r1),
				_mm_add_ps(m1.r2, m2.r2),
				_mm_add_ps(m1.r3, m2.r3));
        }

	/**
	 * @brief Multiplies two given matrices
	 * @param m1 matrix 1
	 * @param m2 matrix 2
	 * @return M1 X M2
	 *
	 * Takes around ~20-30 cycles
	 */
	inline mat4 multiply(const mat4& m1, const mat4& m2)
	{
		mat4 result;
		__m128 row;

		// First row
		row = _mm_mul_ps(m2.r0, _mm_set1_ps(m1.data[0]));
		row = _mm_fmadd_ps(m2.r1, _mm_set1_ps(m1.data[1]), row);
		row = _mm_fmadd_ps(m2.r2, _mm_set1_ps(m1.data[2]), row);
		row = _mm_fmadd_ps(m2.r3, _mm_set1_ps(m1.data[3]), row);
		result.r0 = row;

		// second row
		row = _mm_mul_ps(m2.r0, _mm_set1_ps(m1.data[4]));
		row = _mm_fmadd_ps(m2.r1, _mm_set1_ps(m1.data[5]), row);
		row = _mm_fmadd_ps(m2.r2, _mm_set1_ps(m1.data[6]), row);
		row = _mm_fmadd_ps(m2.r3, _mm_set1_ps(m1.data[7]), row);
		result.r1 = row;

		// third row
		row = _mm_mul_ps(m2.r0, _mm_set1_ps(m1.data[8]));
		row = _mm_fmadd_ps(m2.r1, _mm_set1_ps(m1.data[9]), row);
		row = _mm_fmadd_ps(m2.r2, _mm_set1_ps(m1.data[10]), row);
		row = _mm_fmadd_ps(m2.r3, _mm_set1_ps(m1.data[11]), row);
		result.r2 = row;

		// fourth row
		row = _mm_mul_ps(m2.r0, _mm_set1_ps(m1.data[12]));
		row = _mm_fmadd_ps(m2.r1, _mm_set1_ps(m1.data[13]), row);
		row = _mm_fmadd_ps(m2.r2, _mm_set1_ps(m1.data[14]), row);
		row = _mm_fmadd_ps(m2.r3, _mm_set1_ps(m1.data[15]), row);
		result.r3 = row;

		return result;
	}
}
