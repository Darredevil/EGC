#pragma once
#include "lab_mesh_loader.hpp"

#define PI 3.14159265359
#define ratio 2.312
#define CAM_RADIUS 5

class TPS
{
public:
	unsigned int mesh_vbo, mesh_ibo, mesh_vao, mesh_num_indices;
	unsigned int gl_program_shader;
	glm::mat4 model_matrix, translate_matrix, rotate_matrix;
	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;

public:
	TPS(int _gl_program_shader) {
		// desenare camera TPS
		gl_program_shader = _gl_program_shader;
		
		float x = 0, y = 0, z = 0;
		vertex v[10000];
		// punctele ce desemneaza centrele cercurilor pe baza carora construiesc cilindrul
		v[0].position = glm::vec3(x, y, z);
		v[0].color = glm::vec3(1, 1, 1);
		vertices.push_back(v[0]);
		v[1].position = glm::vec3(x, y + 5, z);
		v[1].color = glm::vec3(1, 1, 1);
		vertices.push_back(v[1]);

		int k = 2;
		// formarea punctelor ce formeaza conturul celor 2 cercuri ale cilindrului
		// formarea triunghiurilor ce compun fetele cilindrului, cele 2 baza si fata laterala
		for (float angle = 0; angle <= 2 * PI; angle += PI/180 ) {
			// punctele cercului de jos
			v[k].position = glm::vec3(x + cos(angle) * CAM_RADIUS, y, z + sin(angle) * CAM_RADIUS);
			v[k].color = glm::vec3(124/255.0, 10/255.0, 2/255.0);
			vertices.push_back(v[k++]);
			// punctele cercului de sus
			v[k].position = glm::vec3(x + cos(angle) * CAM_RADIUS, y + 10, z + sin(angle) * CAM_RADIUS);
			v[k].color = glm::vec3(124/255.0, 10/255.0, 2/255.0);
			vertices.push_back(v[k++]);
			// triunghiurile
			if (k > 4) {
				indices.push_back(0); indices.push_back(k - 2); indices.push_back(k - 4);
				indices.push_back(1); indices.push_back(k - 1); indices.push_back(k - 3);

				indices.push_back(k - 2); indices.push_back(k - 4); indices.push_back(k - 3);
				indices.push_back(k - 5); indices.push_back(k - 4); indices.push_back(k - 3); 
			}
		}

		indices.push_back(2); indices.push_back(3); indices.push_back(k - 1);
		indices.push_back(2); indices.push_back(k - 2); indices.push_back(k - 1);

		// legarile vao si vbo asociate cu vertices
		glGenVertexArrays(1, &mesh_vao);
		glBindVertexArray(mesh_vao);

		glGenBuffers(1, &mesh_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, mesh_vao);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

		// index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1, &mesh_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// legatura intre atributele vertecsilor si pipeline, datele noastre sunt INTERLEAVED.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vertex),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1

		mesh_num_indices = indices.size();

	}

	void draw(glm::vec3 _pos) {
		// metoda de redesenare la un punct 3D dat ca argument
		translate_matrix = glm::translate(glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1), _pos + glm::vec3(0, 5, 0));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader, "model_matrix"), 1, false, glm::value_ptr(translate_matrix));
		glBindVertexArray(mesh_vao);
		glDrawElements(GL_TRIANGLES, mesh_num_indices, GL_UNSIGNED_INT, 0);
	}

	~TPS() {
		glDeleteBuffers(1,&mesh_vbo);
		glDeleteBuffers(1,&mesh_ibo);
		glDeleteVertexArrays(1,&mesh_vao);
	};

};