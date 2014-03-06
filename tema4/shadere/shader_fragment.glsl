#version 330

uniform sampler2D textura1;


in vec3 g_to_f_color;
in vec2 gf_texcoord;
out vec4 out_color;



void main(){
	vec3 color = texture(textura1, gf_texcoord).xyz;

	if (color.r >= 0.75 && color.g >= 0.75 && color.b >= 0.75) discard;

	out_color = vec4(g_to_f_color, 1);
}