#pragma once 

#include "mat4x4.hpp"
#include "vec3.hpp"

#ifdef CLZ_DEBUG
#include "core/assert.hpp"				
#endif


namespace clz::math 
{
	inline void makeIdentity(mat4& mat)
	{
		mat = mat4(1.0f);
	}
	inline void scale(mat4& mat, const vec3& vec)
	{

		#ifdef CLZ_DEBUG
		clz::CLZ_ASSERT(mat.xx == 1.0f && mat.yy == 1.0f &&
				mat.zz == 1.0f && mat.ww == 1.0f,
				"matrix not made identity before applying local transforms");				
		#endif


		mat.xx = vec.x;
		mat.yy = vec.y;
		mat.zz = vec.z;
	}

	inline void translate(mat4& mat, const vec3& vec)
	{
		mat.wx = vec.x;
		mat.wy = vec.y;
		mat.wz = vec.z;
	}

	struct Axis 
	{
		vec3 vector;

		Axis(float x, float y, float z):
			vector(vec3(x, y, z)) {}

		static Axis X() 
		{
			return Axis(1.0f, 0.0f, 0.0f);
		}
		static Axis Y()
		{
			return Axis(0.0f, 1.0f, 0.0f);
		}
		static Axis Z()
		{
			return Axis(0.0f, 0.0f, 1.0f);
		}
	};
}
