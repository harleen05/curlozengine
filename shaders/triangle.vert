#version 460

// In's
layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(binding = 0) uniform UniformBufferObject
{
	mat4 model;
} ubo;

// Out's
layout(location = 0) out vec4 outFragColor;



void main()
{
        gl_Position = ubo.model * vec4(inPosition, 0.0, 1.0);
        outFragColor = vec4(inColor, 0.0);
}
