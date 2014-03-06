#version 330

layout(location = 0) in vec3 in_position;		
layout(location = 1) in vec3 in_normal;		
layout(location = 2) in vec2 in_texcoord;
layout(location = 3) in vec3 in_next_position;		
layout(location = 4) in vec3 in_next_normal;	

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform float time;

out vec3 world_pos;
out vec3 world_normal;

out vec2 texcoord;

void main(){

	texcoord = in_texcoord;

	vec3 new_position = in_position * (1 - time) + in_next_position * time;
	vec3 new_normal = in_normal * (1 - time) + in_next_normal * time;

	world_pos = (model_matrix * vec4(new_position, 1)).xyz;
	world_normal = normalize(mat3(model_matrix) * new_normal);

	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(new_position, 1); 
}
