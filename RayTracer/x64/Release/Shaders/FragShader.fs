#version 410 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D srcTex;

void main()
{
  color = texture(srcTex, texCoord);
}