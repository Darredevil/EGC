#version 330

layout(location = 0) in vec3 in_position;		
layout(location = 1) in vec3 in_color;		
layout(location = 2) in vec2 first_line;

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform float count;

out vec3 vertex_to_fragment_color;

void main(){

	vertex_to_fragment_color = in_color;
	int amp = 8;

	gl_Position = projection_matrix*view_matrix*model_matrix*vec4(in_position,1);
	if (count != 0 && first_line.x != 1) { 
		gl_Position.x = gl_Position.x + cos(0.8 * count) * amp;
		gl_Position.y = gl_Position.y + sin(gl_Position.x + gl_Position.y + count) * amp;		
	}
}
