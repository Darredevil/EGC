//-------------------------------------------------------------------------------------------------
// Descriere: fisier main
//
// Autor: student
// Data: today
//-------------------------------------------------------------------------------------------------

//incarcator de meshe
#include "lab_mesh_loader.hpp"

//geometrie: drawSolidCube, drawWireTeapot...
#include "lab_geometry.hpp"

//incarcator de shadere
#include "lab_shader_loader.hpp"

//interfata cu glut, ne ofera fereastra, input, context opengl
#include "lab_glut.hpp"

//texturi
#include "lab_texture_loader.hpp"

//time
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>


class Laborator : public lab::glut::WindowListener{

//variabile
private:
	glm::mat4 model_matrix, view_matrix, projection_matrix;								//matrici 4x4 pt modelare vizualizare proiectie
	glm::mat4 model1_matrix;
	unsigned int gl_program_shader_phong;												//id obiect shader phong
	unsigned int gl_program_shader_curent;												//id obiect shader curent

	unsigned int gl_particle_shader;

	unsigned int first_vbo, first_ibo, first_vao, first_num_indices;						//containere opengl pentru vertecsi, indecsi si stare
	unsigned int second_vbo, second_ibo, second_vao, second_num_indices;
	unsigned int third_vbo, third_ibo, third_vao, third_num_indices;
	unsigned int sleep_vbo, sleep_ibo, sleep_vao, sleep_num_indices;
	unsigned int particle_vao, particle_vbo, particle_ibo, particle_num_indices;
	unsigned int face_texture, particle_texture;


	float N, K, start, time, anim_start, anim_time;
	int animation;

	glm::vec3 light_position;
	glm::vec3 eye_position;
	glm::vec3 start_position, final_position;
	unsigned int material_shininess;
	float material_kd;
	float material_ks;

	unsigned int earth_texture;
	std::vector<glm::vec3> culori;


//metode
public:
	
	//constructor .. e apelat cand e instantiata clasa
	Laborator(){
		//setari pentru desenare, clear color seteaza culoarea de clear pentru ecran (format R,G,B,A)
		glClearColor(0.5,0.5,0.5,1);
		glClearDepth(1);			//clear depth si depth test (nu le studiem momentan, dar avem nevoie de ele!)
		glEnable(GL_DEPTH_TEST);	//sunt folosite pentru a determina obiectele cele mai apropiate de camera (la curs: algoritmul pictorului, algoritmul zbuffer)
		
		//culori
		culori.push_back(glm::vec3(0, 0, 1));
		culori.push_back(glm::vec3(0, 1, 0));
		culori.push_back(glm::vec3(1, 0, 0));
		culori.push_back(glm::vec3(1, 1, 0));
		culori.push_back(glm::vec3(0, 1, 1));

		//incarca un shader din fisiere si gaseste locatiile matricilor relativ la programul creat
		gl_program_shader_phong = lab::loadShader("shadere\\shader_phong_vertex.glsl", "shadere\\shader_phong_fragment.glsl");
		gl_program_shader_curent = gl_program_shader_phong;
		gl_particle_shader = lab::loadShader("shadere\\shader_vertex.glsl", "shadere\\shader_geometry.glsl", "shadere\\shader_fragment.glsl");
		
		//incarca un mesh
		std::vector<lab::VertexFormat> vertices1, vertices2, vertices3;
		std::vector<unsigned int> indices1, indices2, indices3;

		// incarcare vertices si indices pentru cele 3 obiecte asociate fiecarei stari necesare animatiei
		lab::_loadObjFile("resurse\\girl_sleep.obj", vertices1, indices1); 
		lab::_loadObjFile("resurse\\girl_surprise.obj", vertices2, indices2); 
		lab::_loadObjFile("resurse\\girl_annoyed.obj", vertices3, indices3); 

		// initializare obiecte cu proprietatea ca pe langa pozitiile, normalele si coordonatele de textura proprie
		// mai au si pozitiile si normalele starii urmatoare.
		lab::myLoadObj(vertices1, indices1, vertices2, indices2, first_vao, first_vbo, first_ibo, first_num_indices);
		lab::myLoadObj(vertices2, indices2, vertices3, indices3, second_vao, second_vbo, second_ibo, second_num_indices);
		lab::myLoadObj(vertices3, indices3, vertices1, indices1, third_vao, third_vbo, third_ibo, third_num_indices);
		lab::myLoadObj(vertices1, indices1, vertices1, indices1, sleep_vao, sleep_vbo, sleep_ibo, sleep_num_indices);
		face_texture = lab::loadTextureBMP("resurse\\girl_texture.bmp");
		particle_texture = lab::loadTextureBMP("resurse\\music.bmp");

		// lumina & material
		// initializare pozitii camera, timpi de inceput pentru animatii, durata unui ciclu de animatie
		// initializare stare a animatiei si distanta maxima folosita la translatarea particulelor
		// dintr-o parte in alta a ecranului.
		eye_position = glm::vec3(0,7,3);
		light_position = glm::vec3(1,8,3);
		material_shininess = 100;
		material_kd = 0.75;
		material_ks = 0.25;
		start = GetTickCount();
		anim_start = GetTickCount();
		N = 5000.0f;
		animation = 1;
		start_position = glm::vec3(0, 0, 0);
		final_position = glm::vec3(17, -1.5, -0.5);


		// creare sistem particule
		
		std::vector<unsigned int> indices;
		std::vector<lab::VertexFormat> vertices;
		int k = 0, p = 0;
		for(int i = 0; i < 41; i++) {
			for(int j = 0; j < 41; j++) {
				if (k % 5 == 0) {
					glm::vec3 c = culori.at(p % 5);
					vertices.push_back(lab::VertexFormat(-5.5 - i * 0.14, 11.5 - j * 0.23, 0, c.x, c.y, c.z));
					indices.push_back(i * 6 + j);
					p++;
				}
				k++;
			}
		}

		//vertex array object -> un obiect ce reprezinta un container pentru starea de desenare
		glGenVertexArrays(1, &particle_vao);
		glBindVertexArray(particle_vao);

		//vertex buffer object -> un obiect in care tinem vertecsii
		glGenBuffers(1,&particle_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, particle_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(lab::VertexFormat), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1,&particle_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particle_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)0);					//trimite pozitii pe pipe 0
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(lab::VertexFormat),(void*)(sizeof(float)*3));	//trimite pozitii pe pipe 0
		
		particle_num_indices = indices.size();;

		//matrici de modelare si vizualizare
		model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		model1_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		view_matrix = glm::lookAt(eye_position, glm::vec3(0,7,0), glm::vec3(0,1,0));

		//desenare wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//destructor .. e apelat cand e distrusa clasa
	~Laborator(){
		//distruge shadere
		glDeleteProgram(gl_program_shader_phong);
		glDeleteProgram(gl_particle_shader);

		//distruge mesh incarcat
		glDeleteBuffers(1,&first_vbo);
		glDeleteBuffers(1,&first_ibo);
		glDeleteVertexArrays(1,&first_vao);

		glDeleteBuffers(1,&second_vbo);
		glDeleteBuffers(1,&second_ibo);
		glDeleteVertexArrays(1,&second_vao);

		glDeleteBuffers(1,&third_vbo);
		glDeleteBuffers(1,&third_ibo);
		glDeleteVertexArrays(1,&third_vao);

		glDeleteBuffers(1,&sleep_vbo);
		glDeleteBuffers(1,&sleep_ibo);
		glDeleteVertexArrays(1,&sleep_vao);

		glDeleteBuffers(1,&particle_vbo);
		glDeleteBuffers(1,&particle_ibo);
		glDeleteVertexArrays(1,&particle_vao);
	}


	//--------------------------------------------------------------------------------------------
	//functii de cadru ---------------------------------------------------------------------------

	//functie chemata inainte de a incepe cadrul de desenare, o folosim ca sa updatam situatia scenei ( modelam/simulam scena)
	void notifyBeginFrame(){
		//rotatie lumina Oy
	}
	//functia de afisare (lucram cu banda grafica)
	void notifyDisplayFrame(){
		// bufferele din framebuffer sunt aduse la valorile initiale (setate de clear color si cleardepth)
		// adica se sterge ecranul si se pune culoare (si alte propietati) initiala
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		K = GetTickCount();
		// calcul timp pentru animatia dintre stari
		if (K - start < N) {
			time = (K - start) / N;
		} else {
			// daca s-a depasit timpul de animatie intre 2 stari
			// se trece la urmatoarea stare
			start = K;
			time = (K - start) / N;
			if (animation == 4) {
				animation = 1;
			} else {
				animation++;
			}
		}
		// calcul timp pentru animatia zgomotului
		if (K - anim_start < (2 * N)) {
			anim_time = (K - anim_start) / (2 * N);
		} else {
			if (K - anim_start >= (4 * N)) {
				anim_start = K;
				anim_time = 0.0f;
			} else {
				anim_time = 1.0f;
			}
		}
		

		//foloseste shaderul
		glUseProgram(gl_program_shader_curent);
				
		GLuint locatie1 = glGetUniformLocation(gl_program_shader_curent, "textura1");

		//trimite variabile uniforme la shader
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "model_matrix"),1,false,glm::value_ptr(model_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "view_matrix"),1,false,glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader_curent, "projection_matrix"),1,false,glm::value_ptr(projection_matrix));
		glUniform3f(glGetUniformLocation(gl_program_shader_curent, "light_position"),light_position.x, light_position.y, light_position.z);
		glUniform3f(glGetUniformLocation(gl_program_shader_curent, "eye_position"),eye_position.x, eye_position.y, eye_position.z);
		glUniform1i(glGetUniformLocation(gl_program_shader_curent, "material_shininess"),material_shininess);
		glUniform1f(glGetUniformLocation(gl_program_shader_curent, "material_kd"),material_kd);
		glUniform1f(glGetUniformLocation(gl_program_shader_curent, "material_ks"),material_ks);
		glUniform1f(glGetUniformLocation(gl_program_shader_curent, "time"), time);
		
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, face_texture);
		glUniform1i(locatie1, 1);
		
		// bind obiect
		// se afiseaza obiectul din starea in care a ajuns animatia
		if (animation == 1) {
			glBindVertexArray(first_vao);
			glDrawElements(GL_TRIANGLES, first_num_indices, GL_UNSIGNED_INT, 0);
		} else if (animation == 2) {
			glBindVertexArray(second_vao);
			glDrawElements(GL_TRIANGLES, second_num_indices, GL_UNSIGNED_INT, 0);
		} else if (animation == 3) {
			glBindVertexArray(third_vao);
			glDrawElements(GL_TRIANGLES, third_num_indices, GL_UNSIGNED_INT, 0);
		} else {
			glBindVertexArray(sleep_vao);
			glDrawElements(GL_TRIANGLES, third_num_indices, GL_UNSIGNED_INT, 0);
		}
		
		glUseProgram(gl_particle_shader);
		
		// calcul distanta ce trebuie translatata la un moment de timp pentru animatia zgomotului
		glm::vec3 dist = (1 - anim_time) * start_position + anim_time * final_position;
		model1_matrix = glm::translate(model_matrix, dist);

		glUniform3f(glGetUniformLocation(gl_particle_shader, "cameraPos"),eye_position.x, eye_position.y, eye_position.z);
		glUniformMatrix4fv(glGetUniformLocation(gl_particle_shader, "view_matrix"),1,false,glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_particle_shader, "model_matrix"),1,false,glm::value_ptr(model1_matrix));
		glUniformMatrix4fv(glGetUniformLocation(gl_particle_shader, "projection_matrix"),1,false,glm::value_ptr(projection_matrix));
		
		// bind textura
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, particle_texture);
		glUniform1i(glGetUniformLocation(gl_particle_shader, "textura1"), 2);

		// bind particule
		glBindVertexArray(particle_vao);
		glDrawArrays(GL_POINTS, 0, particle_num_indices);

		
	}
	//functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
	void notifyEndFrame(){}
	//functei care e chemata cand se schimba dimensiunea ferestrei initiale
	void notifyReshape(int width, int height, int previos_width, int previous_height){
		//reshape
		if(height == 0) height=1;
		glViewport(0,0,width,height);
		projection_matrix = glm::perspective(90.0f, (float)width/(float)height,0.1f, 10000.0f);
	}


	//--------------------------------------------------------------------------------------------
	//functii de input output --------------------------------------------------------------------
	
	//tasta apasata
	void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == 27) lab::glut::close();	//ESC inchide glut si 
		if(key_pressed == 32) {
			//SPACE reincarca shaderul si recalculeaza locatiile (offseti/pointeri)
			glDeleteProgram(gl_program_shader_phong);
			glDeleteProgram(gl_particle_shader);
			gl_program_shader_phong = lab::loadShader("shadere\\shader_phong_vertex.glsl", "shadere\\shader_phong_fragment.glsl");
			gl_particle_shader = lab::loadShader("shadere\\shader_vertex.glsl", "shadere\\shader_geometry.glsl", "shadere\\shader_fragment.glsl");
			gl_program_shader_curent = gl_program_shader_phong;
		}
		if(key_pressed == 'w'){
			static bool wire =true;
			wire=!wire;
			glPolygonMode(GL_FRONT_AND_BACK, (wire?GL_LINE:GL_FILL));
		}

	}
	//tasta ridicata
	void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y){	}
	//tasta speciala (up/down/F1/F2..) apasata
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == GLUT_KEY_F1) lab::glut::enterFullscreen();
		if(key_pressed == GLUT_KEY_F2) lab::glut::exitFullscreen();
	}
	//tasta speciala ridicata
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y){}
	//drag cu mouse-ul
	void notifyMouseDrag(int mouse_x, int mouse_y){ }
	//am miscat mouseul (fara sa apas vreun buton)
	void notifyMouseMove(int mouse_x, int mouse_y){ }
	//am apasat pe un boton
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y){ }
	//scroll cu mouse-ul
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y){ std::cout<<"Mouse scroll"<<std::endl;}

};

int main(){
	//initializeaza GLUT (fereastra + input + context OpenGL)
	lab::glut::WindowInfo window(std::string("lab shadere 1"),800,600,100,100,true);
	lab::glut::ContextInfo context(3,3,false);
	lab::glut::FramebufferInfo framebuffer(true,true,true,true);
	lab::glut::init(window,context, framebuffer);

	//initializeaza GLEW (ne incarca functiile openGL, altfel ar trebui sa facem asta manual!)
	glewExperimental = true;
	glewInit();
	std::cout<<"GLEW:initializare"<<std::endl;

	//creem clasa noastra si o punem sa asculte evenimentele de la GLUT
	//DUPA GLEW!!! ca sa avem functiile de OpenGL incarcate inainte sa ii fie apelat constructorul (care creeaza obiecte OpenGL)
	Laborator mylab;
	lab::glut::setListener(&mylab);

	//taste
	std::cout<<"Taste:"<<std::endl<<"\tESC ... iesire"<<std::endl<<"\tSPACE ... reincarca shadere"<<std::endl<<"\tw ... toggle wireframe"<<std::endl;

	//run
	lab::glut::run();

	return 0;
}