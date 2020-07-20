#version 410 core

in vec2 pos;
out vec2 texCoord;
void main()
{
	texCoord = pos * 0.5f + vec2(0.5, 0.5);
	gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
}