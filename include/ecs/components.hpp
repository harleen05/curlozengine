/**
 * @file components.hpp
 * @author curl0z
 * @brief ECS component definitions for Curloz Engine.
 *
 * All components are plain data structs — no logic, no methods.
 * Logic lives in systems. Components are attached to entities via
 * the EnTT registry in variables.hpp.
 */

#pragma once

#include "math/vec3.hpp"

namespace clz::ecs
{
	/**
	 * @brief Stores position, rotation and scale of an entity in world space.
	 *
	 * Rotation is stored as Euler angles in degrees.
	 * This is the handshake between ECS, physics, and the renderer.
	 *
	 * @warning Do not change the layout without consulting the renderer and physics owners.
	 */
	struct TransformComponent
	{
		clz::math::vec3 position = {0, 0, 0};	///< World space position.
		clz::math::vec3 rotation = {0, 0, 0};	///< Euler angles in degrees (pitch, yaw, roll).
		clz::math::vec3 scale	 = {1, 1, 1};	///< Non-uniform scale.

		TransformComponent(const clz::math::vec3 &position, const clz::math::vec3 &rotation,
					const clz::math::vec3 &scale) : position(position), rotation(rotation), scale(scale) {}

	};

} // namespace clz::ecs