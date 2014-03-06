#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

out vec3 v_to_g_color;

void main(){

	v_to_g_color = in_color;
	gl_Position = vec4(in_position,1); 
	
}
