#pragma once
#include "lab_mesh_loader.hpp"


struct vertex {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 first_line;
};

#define PI 3.14159265359
#define ratio 2.312

class Track
{
public:
	float size;
	unsigned int vbo, ibo, vao, num_indices;
	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;
	glm::mat4 model_matrix;

public:
	Track(float _radius, glm::vec3 color) {
		float x = 0, y = 0, z = 0;
		size = _radius * ratio;
		float radius = _radius;
		std::vector<vertex> vertices;
		std::vector<unsigned int> indices;
		
		vertex v[10000];
		
		for (int i = 0; i < 10000; i++) {
			v[i].color = color;
			v[i].first_line.x = 0;
		}	


		// semicercul 1
		v[0].position = glm::vec3(x, y, z);
		vertices.push_back(v[0]);
		int o1 = 0;
		int k = 1;
		for (float angle = 0; angle <= PI; angle += PI/180 ) {
			// semicerc
			v[k].position = glm::vec3(x + cos(angle) * radius, y, z + sin(angle) * radius); // semicerc mare (1)
			v[k].color = color;
			vertices.push_back(v[k]);
			k++;
			
			// banda 1
			v[k].position = glm::vec3(x + cos(angle) * radius, y + 0.03, z + sin(angle) * radius); // banda 1 (2)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;

			v[k].position = glm::vec3(x + cos(angle) * (radius - 1), y + 0.1, z + sin(angle) * (radius - 1)); // banda 1 (3)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;

			// banda 2
			v[k].position = glm::vec3(x + cos(angle) * (radius - 13), y + 0.1, z + sin(angle) * (radius - 13)); // banda 2 (4)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;

			v[k].position = glm::vec3(x + cos(angle) * (radius - 14), y + 0.1, z + sin(angle) * (radius - 14)); // banda 2 (5)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;

			// banda 3
			v[k].position = glm::vec3(x + cos(angle) * (radius - 26), y + 0.1, z + sin(angle) * (radius - 26)); // banda 3 (6)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;

			v[k].position = glm::vec3(x + cos(angle) * (radius - 27), y + 0.1, z + sin(angle) * (radius - 27)); // banda 3 (7)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;

			// banda 4
			v[k].position = glm::vec3(x + cos(angle) * (radius - 39), y + 0.1, z + sin(angle) * (radius - 39)); // banda 4 (8)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;

			v[k].position = glm::vec3(x + cos(angle) * (radius - 40), y + 0.1, z + sin(angle) * (radius - 40)); // banda 4 (9)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;
			

			if (k > 10) {
				indices.push_back(0); indices.push_back(k - 18); indices.push_back(k - 9);

				indices.push_back(k - 17); indices.push_back(k - 16); indices.push_back(k - 7); 
				indices.push_back(k - 17); indices.push_back(k - 8); indices.push_back(k - 7);

				indices.push_back(k - 15); indices.push_back(k - 14); indices.push_back(k - 5);
				indices.push_back(k - 15); indices.push_back(k - 6); indices.push_back(k - 5);

				indices.push_back(k - 13); indices.push_back(k - 12); indices.push_back(k - 3);
				indices.push_back(k - 13); indices.push_back(k - 4); indices.push_back(k - 3);

				indices.push_back(k - 11); indices.push_back(k - 10); indices.push_back(k - 1);
				indices.push_back(k - 11); indices.push_back(k - 2); indices.push_back(k - 1);
			}
		}

		// semicercul 2
		float z1 = z - size;
		v[k].position = glm::vec3(x, y, z1);
		v[k].color = color;
		vertices.push_back(v[k]);
		int second = k++;
				
		for (float angle=0; angle >= -PI; angle -= PI/180) {
			// semicerc
			v[k].position = glm::vec3(x + cos(angle) * radius, y, z1 + sin(angle) * radius); // semicerc mare (1)
			v[k].color = color;
			vertices.push_back(v[k]);
			k++;
			
			// banda 1
			v[k].position = glm::vec3(x + cos(angle) * radius, y + 0.03, z1 + sin(angle) * radius); // banda 1 (2)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;

			v[k].position = glm::vec3(x + cos(angle) * (radius - 1), y + 0.1, z1 + sin(angle) * (radius - 1)); // banda 1 (3)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;

			// banda 2
			v[k].position = glm::vec3(x + cos(angle) * (radius - 13), y + 0.1, z1 + sin(angle) * (radius - 13)); // banda 2 (4)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;

			v[k].position = glm::vec3(x + cos(angle) * (radius - 14), y + 0.1, z1 + sin(angle) * (radius - 14)); // banda 2 (5)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;

			// banda 3
			v[k].position = glm::vec3(x + cos(angle) * (radius - 26), y + 0.1, z1 + sin(angle) * (radius - 26)); // banda 2 (6)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;

			v[k].position = glm::vec3(x + cos(angle) * (radius - 27), y + 0.1, z1 + sin(angle) * (radius - 27)); // banda 2 (7)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;

			// banda 4
			v[k].position = glm::vec3(x + cos(angle) * (radius - 39), y + 0.1, z1 + sin(angle) * (radius - 39)); // banda 2 (8)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;

			v[k].position = glm::vec3(x + cos(angle) * (radius - 40), y + 0.1, z1 + sin(angle) * (radius - 40)); // banda 2 (9)
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k]);
			k++;
			

			if (k > second + 10) {
				indices.push_back(second); indices.push_back(k - 18); indices.push_back(k - 9);
	
				indices.push_back(k - 17); indices.push_back(k - 16); indices.push_back(k - 7); 
				indices.push_back(k - 17); indices.push_back(k - 8); indices.push_back(k - 7);

				indices.push_back(k - 15); indices.push_back(k - 14); indices.push_back(k - 5);
				indices.push_back(k - 15); indices.push_back(k - 6); indices.push_back(k - 5);

				indices.push_back(k - 13); indices.push_back(k - 12); indices.push_back(k - 3);
				indices.push_back(k - 13); indices.push_back(k - 4); indices.push_back(k - 3);

				indices.push_back(k - 11); indices.push_back(k - 10); indices.push_back(k - 1);
				indices.push_back(k - 11); indices.push_back(k - 2); indices.push_back(k - 1);
			}
		}
		
		// desenare dreptunghi ce uneste semicercuri
		v[k].position = glm::vec3(x - radius, y, z + 1);
		v[k].color = color;
		vertices.push_back(v[k++]);

		v[k].position = glm::vec3(x + radius, y, z + 1);
		v[k].color = color;
		vertices.push_back(v[k++]);

		v[k].position = glm::vec3(x + radius, y, z1 - 1);
		v[k].color = color;
		vertices.push_back(v[k++]);

		v[k].position = glm::vec3(x - radius, y, z1 - 1);
		v[k].color = color;
		vertices.push_back(v[k++]);

		indices.push_back(k - 4); indices.push_back(k - 3); indices.push_back(k - 2);
		indices.push_back(k - 4); indices.push_back(k - 1); indices.push_back(k - 2);

		// desenare benzi delimitare culoare pe linia dreapta
		int flp = k;
		for (int i = 0; i < 4; i++) {
			v[k].position = glm::vec3(x - radius + i * 13, y + 0.1, z + 1);
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k++]);

			v[k].position = glm::vec3(x - radius + i * 13 + 1, y + 0.1, z + 1);
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k++]);

			v[k].position = glm::vec3(x - radius + i * 13, y + 0.1, z1 - 1);
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k++]);

			v[k].position = glm::vec3(x - radius + i * 13 + 1, y + 0.1, z1 - 1);
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k++]);

			v[k].position = glm::vec3(x + radius - i * 13, y + 0.1, z + 1);
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k++]);

			v[k].position = glm::vec3(x + radius - i * 13 - 1, y + 0.1, z + 1);
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k++]);

			v[k].position = glm::vec3(x + radius - i * 13, y + 0.1, z1 - 1);
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k++]);

			v[k].position = glm::vec3(x + radius - i * 13 - 1, y + 0.1, z1 - 1);
			v[k].color = glm::vec3(1, 1, 1);
			vertices.push_back(v[k++]);
		}

		indices.push_back(flp);		 indices.push_back(flp + 1);  indices.push_back(flp + 2);
		indices.push_back(flp + 3);  indices.push_back(flp + 1);  indices.push_back(flp + 2);
		indices.push_back(flp);		 indices.push_back(flp + 2);  indices.push_back(flp + 3);
		indices.push_back(flp);		 indices.push_back(flp + 1);  indices.push_back(flp + 3);

		indices.push_back(flp + 4);  indices.push_back(flp + 5);  indices.push_back(flp + 7);
		indices.push_back(flp + 6);  indices.push_back(flp + 5);  indices.push_back(flp + 4);
		indices.push_back(flp + 4);  indices.push_back(flp + 6);  indices.push_back(flp + 7);
		indices.push_back(flp + 6);  indices.push_back(flp + 5);  indices.push_back(flp + 7);

		indices.push_back(flp + 8);	 indices.push_back(flp + 9);  indices.push_back(flp + 10);
		indices.push_back(flp + 11); indices.push_back(flp + 9);  indices.push_back(flp + 10);

		indices.push_back(flp + 12); indices.push_back(flp + 13); indices.push_back(flp + 14);
		indices.push_back(flp + 15); indices.push_back(flp + 13); indices.push_back(flp + 14);

		indices.push_back(flp + 16); indices.push_back(flp + 17); indices.push_back(flp + 18);
		indices.push_back(flp + 19); indices.push_back(flp + 17); indices.push_back(flp + 18);

		indices.push_back(flp + 20); indices.push_back(flp + 21); indices.push_back(flp + 22);
		indices.push_back(flp + 23); indices.push_back(flp + 21); indices.push_back(flp + 22);

		indices.push_back(flp + 24); indices.push_back(flp + 25); indices.push_back(flp + 26);
		indices.push_back(flp + 27); indices.push_back(flp + 25); indices.push_back(flp + 26);

		indices.push_back(flp + 28); indices.push_back(flp + 29); indices.push_back(flp + 30);
		indices.push_back(flp + 31); indices.push_back(flp + 29); indices.push_back(flp + 30);

		flp = k;
		// desenare linie start sosire
		v[k].position = glm::vec3(x + radius / 2, y + 0.2, -size / 2 + 15);
		v[k].color = glm::vec3(1, 1, 1);
		vertices.push_back(v[k++]);

		v[k].position = glm::vec3(x + radius, y + 0.2, -size / 2 + 15);
		v[k].color = glm::vec3(1, 1, 1);
		vertices.push_back(v[k++]);

		v[k].position = glm::vec3(x + radius, y + 0.2, -size / 2 + 13);
		v[k].color = glm::vec3(1, 1, 1);
		vertices.push_back(v[k++]);

		v[k].position = glm::vec3(x + radius / 2, y + 0.2, -size / 2 + 13);
		v[k].color = glm::vec3(1, 1, 1);
		vertices.push_back(v[k++]);

		indices.push_back(flp);		indices.push_back(flp + 1);		indices.push_back(flp + 2);
		indices.push_back(flp + 3); indices.push_back(flp + 1);		indices.push_back(flp + 2); 
		indices.push_back(flp);		indices.push_back(flp + 3);		indices.push_back(flp + 2);
		indices.push_back(flp);		indices.push_back(flp + 1);		indices.push_back(flp + 1);

		// legari vao si vbo
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vao);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

		// index buffer object -> un obiect in care tinem indecsii
		glGenBuffers(1,&ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// legatura intre atributele vertecsilor si pipeline, datele noastre sunt INTERLEAVED.
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex),(void*)0);						//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vertex),(void*)(sizeof(float)*3));		//trimite normale pe pipe 1
	
		
		num_indices = indices.size();
	}

	void draw() {
		// metoda de redesenare
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
	}

	~Track() {
		glDeleteBuffers(1,&vbo);
		glDeleteBuffers(1,&ibo);
		glDeleteVertexArrays(1,&vao);
	};
};

