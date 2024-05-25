#version 330 core
in vec2 UV;
layout(location = 0) out vec3 outputColor;
uniform sampler2D unit;
void main(){
	outputColor = texture(unit, UV).rgb;
}