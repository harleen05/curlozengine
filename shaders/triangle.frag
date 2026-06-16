#version 460

layout(location = 0) in vec4 inFragColor;

layout(location = 0) out vec4 outColor;

void main() 
{
	outColor = inFragColor;
}
