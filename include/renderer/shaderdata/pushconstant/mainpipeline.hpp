/**
 * @file mainpipeline.hpp
 * @author curl0z
 * @brief Defines push constants of the main pipeline
 */
#pragma once

#include "renderer/shaderdata/descriptor/texture.hpp"
#include "math/mat4x4.hpp"

namespace clz::renderer::MainPC
{
	struct VertexData
	{
		math::mat4 model;
	};
	struct FragmentData
	{
		TextureID textureID;
	};

	/// Vertex Shader data
	inline VertexData vertexData;
	/// Fragment Shader data
	inline FragmentData fragmentData;
}
