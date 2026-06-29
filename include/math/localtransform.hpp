#pragma once

#include "mat4x4.hpp"
#include "vec3.hpp"
#include "quat.hpp"
#include <cmath>

namespace clz::math
{
	inline vec3 scale(const vec3& v, const vec3& scale)
	{
		return component_product(v, scale);
	}

	inline vec3 translate(const vec3& v, const vec3& translation)
	{
		return add(v, translation);
	}

	constexpr float DEG_TO_RAD = 3.14159265358979323846 / 180;

	inline float radians(const float degree)
	{
		return degree * DEG_TO_RAD;
	}

	inline quat rotate(const quat& q, const float rad, const Axis& axis)
	{
		const float half = rad * 0.5f;
		const float s = std::sin(half);

		const quat delta(
			std::cos(half),
			axis.x * s,
			axis.y * s,
			axis.z * s);

		return q * delta;
	}


} // namespace clz::math
