#pragma once
#include "lab_mesh_loader.hpp"

class Flag
{
public:
	unsigned int mesh_vbo, mesh_ibo, mesh_vao, mesh_num_indices;
	unsigned int gl_program_shader;
	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;

public:
	Flag(glm::vec3 _color, int _gl_program_shader){
		vertex v[300];
		for(int i = 0; i < 300; i++) v[i].first_line.x = 0;
		int k = 0, n = 10;
		float x_plus = 60, x_minus = -60;
		float z_plus = 60, z_minus = -60;
		gl_program_shader = _gl_program_shader;

		// Cele 6 puncte pentru batul steagului
		v[k].position = glm::vec3(65, 0, 60);
		v[k].color = glm::vec3(0, 0, 0);
		v[k].first_line.x = 1;
		vertices.push_back(v[k++]);

		v[k].position = glm::vec3(65, 0, -60);
		v[k].color = glm::vec3(0, 0, 0);
		v[k].first_line.x = 1;
		vertices.push_back(v[k++]);

		v[k].position = glm::vec3(65, 0, -240);
		v[k].color = glm::vec3(0, 0, 0);
		v[k].first_line.x = 1;
		vertices.push_back(v[k++]);

		v[k].position = glm::vec3(60, 0, 60);
		v[k].color = glm::vec3(0, 0, 0);
		v[k].first_line.x = 1;
		vertices.push_back(v[k++]);

		v[k].position = glm::vec3(60, 0, -60);
		v[k].color = glm::vec3(0, 0, 0);
		v[k].first_line.x = 1;
		vertices.push_back(v[k++]);

		v[k].position = glm::vec3(60, 0, -240);
		v[k].color = glm::vec3(0, 0, 0);
		v[k].first_line.x = 1;
		vertices.push_back(v[k++]);

		// triunghiurile asociate batului pentru desenarea corecta a acestuia
		indices.push_back(0); indices.push_back(1); indices.push_back(4);
		indices.push_back(0); indices.push_back(3); indices.push_back(4);

		indices.push_back(4); indices.push_back(1); indices.push_back(2);
		indices.push_back(5); indices.push_back(4); indices.push_back(2);

		int mod = -1;
		// punctele ce formeaza panza steagului
		// vor fi desenate 6 x 10 puncte
		// triunghurile asociate matricii de puncte
		for (int i = 0; i < n; i++) {
			mod = -mod;
			v[k].position = glm::vec3(x_plus - (x_plus - x_minus)/n * i, 0, z_plus);
			v[k].color = _color + glm::vec3(1.5 * mod, 1.5 * mod, 1.5 * mod) * glm::vec3(i * 0.003, i * 0.001, i * 0.003);
			if(i == 0) v[k].first_line.x = 1;
			vertices.push_back(v[k++]);

			
			v[k].position = glm::vec3(x_plus - (x_plus - x_minus)/n * i, 0, z_plus - (z_plus - z_minus) * 1/5);
			v[k].color = _color + glm::vec3(1.5 * mod, 1.5 * mod, 1.5 * mod) * glm::vec3(i * 0.006, i * 0.002, i * 0.006);
			if(i == 0) v[k].first_line.x = 1;
			vertices.push_back(v[k++]);

			v[k].position = glm::vec3(x_plus - (x_plus - x_minus)/n * i, 0,  z_plus - (z_plus - z_minus) * 2/5);
			v[k].color = _color + glm::vec3(1.5 * mod, 1.5 * mod, 1.5 * mod) * glm::vec3(i * 0.009, i * 0.003, i * 0.009);
			if(i == 0) v[k].first_line.x = 1;
			vertices.push_back(v[k++]);

			
			v[k].position = glm::vec3(x_plus - (x_plus - x_minus)/n * i, 0,  z_plus - (z_plus - z_minus) * 3/5);
			v[k].color = _color + glm::vec3(1.5 * mod, 1.5 * mod, 1.5 * mod) * glm::vec3(i * 0.009, i * 0.003, i * 0.009);
			if(i == 0) v[k].first_line.x = 1;
			vertices.push_back(v[k++]);

			v[k].position = glm::vec3(x_plus - (x_plus - x_minus)/n * i, 0,  z_plus - (z_plus - z_minus) * 4/5);
			v[k].color = _color + glm::vec3(1.5 * mod, 1.5 * mod, 1.5 * mod) * glm::vec3(i * 0.009, i * 0.003, i * 0.009);
			if(i == 0) v[k].first_line.x = 1;
			vertices.push_back(v[k++]);

			
			v[k].position = glm::vec3(x_plus - (x_plus - x_minus)/n * i, 0,  z_plus - (z_plus - z_minus) * 5/5);
			v[k].color = _color + glm::vec3(1.5 * mod, 1.5 * mod, 1.5 * mod) * glm::vec3(i * 0.009, i * 0.003, i * 0.009);
			if(i == 0) v[k].first_line.x = 1;
			vertices.push_back(v[k++]);

			if (i > 0) {
				indices.push_back(k - 12); indices.push_back(k - 6); indices.push_back(k - 11); 
				indices.push_back(k - 5); indices.push_back(k - 6); indices.push_back(k - 11); 

				indices.push_back(k - 11); indices.push_back(k - 5); indices.push_back(k - 10); 
				indices.push_back(k - 4); indices.push_back(k - 5); indices.push_back(k - 10); 

				indices.push_back(k - 10); indices.push_back(k - 4); indices.push_back(k - 9); 
				indices.push_back(k - 3); indices.push_back(k - 4); indices.push_back(k - 9); 

				indices.push_back(k - 9); indices.push_back(k - 3); indices.push_back(k - 8); 
				indices.push_back(k - 2); indices.push_back(k - 3); indices.push_back(k - 8); 

				indices.push_back(k - 8); indices.push_back(k - 2); indices.push_back(k - 7); 
				indices.push_back(k - 1); indices.push_back(k - 2); indices.push_back(k - 7); 
			}
		}

		indices.push_back(6); indices.push_back(3); indices.push_back(4); 
		indices.push_back(6); indices.push_back(11); indices.push_back(4); 

		// legari vao si vbo asociate cu vertices
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
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(vertex),(void*)(2*sizeof(float)*3));

		mesh_num_indices = indices.size();
	}

	void draw(float count) {
		// trimitere la shader a unei valori tip float numita "count"
		// variabila "count" indeplineste functia de ceas si va fi folosita in functia sinusoidala folosita
		// pentru obtinerea efectului de fluturare a steagului
		glUniform1f(glGetUniformLocation(gl_program_shader, "count"), count);
		// apelare metode de redesenare a obiectului
		glBindVertexArray(mesh_vao);
		glDrawElements(GL_TRIANGLES, mesh_num_indices, GL_UNSIGNED_INT, 0);
	}

	~Flag() {
		glDeleteBuffers(1,&mesh_vbo);
		glDeleteBuffers(1,&mesh_ibo);
		glDeleteVertexArrays(1,&mesh_vao);
	}
};