#version 330
layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 100) out;

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform vec3 cameraPos;

in vec3 v_to_g_color[];
out vec2 gf_texcoord;
out vec3 g_to_f_color;


void main(){

	g_to_f_color = v_to_g_color[0];

	vec3 Pos = (model_matrix * view_matrix * gl_in[0].gl_Position).xyz;
	vec3 toCamera = normalize(Pos);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up);
	float a = 0.3f;

	Pos -= (right * a/2);
    gl_Position = projection_matrix * vec4(Pos, 1.0);
    gf_texcoord = vec2(0.0, 0.0);
    EmitVertex();

	Pos.y += a;
    gl_Position = projection_matrix * vec4(Pos, 1.0);
    gf_texcoord = vec2(0.0, 1.0);
    EmitVertex();

	Pos.y -= a;
    Pos += right * a;
    gl_Position = projection_matrix * vec4(Pos, 1.0);
    gf_texcoord = vec2(1.0, 0.0);
    EmitVertex();

	Pos.y += a;
    gl_Position = projection_matrix * vec4(Pos, 1.0);
    gf_texcoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
