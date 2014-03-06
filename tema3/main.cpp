//-------------------------------------------------------------------------------------------------
// Descriere: fisier main
//
// Autor: student
// Data: today
//-------------------------------------------------------------------------------------------------

#include "Track.hpp"
#include "Player.hpp"
#include "TPS.hpp"
#include "Flag.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <ctime>

//incarcator de meshe
#include "lab_mesh_loader.hpp"

//geometrie: drawSolidCube, drawWireTeapot...
#include "lab_geometry.hpp"

//incarcator de shadere
#include "lab_shader_loader.hpp"

//interfata cu glut, ne ofera fereastra, input, context opengl
#include "lab_glut.hpp"

//camera
#include "lab_camera.hpp"

//time
#include <ctime>

#define PI 3.14159265359
#define RADIUS 80
#define TRANS 2.5
#define ratio 2.312
#define TPX 0
#define TPY 335
#define TPZ -RADIUS * ratio / 2
#define PLAYER_SPEED 0.15

class Laborator : public lab::glut::WindowListener{
	
public:
	Player *player, *opponent1, *opponent2;
	TPS *spider_cam;
//variabile
private:
	Track *track;
	glm::mat4 model_matrix, projection_matrix;											//matrici 4x4 pt modelare vizualizare proiectie
	lab::Camera cam;
	unsigned int gl_program_shader;														//id-ul de opengl al obiectului de tip program shader
	unsigned int screen_width, screen_height;
	unsigned int mesh_vbo, mesh_ibo, mesh_vao, mesh_num_indices;
	bool upFlag, leftFlag, rightFlag, downFlag, player_cam, op1_cam, op2_cam, TPSCam;
	glm::vec3 player_color;
	glm::vec3 op1_color;
	glm::vec3 op2_color;
	int start, spider, victory, winner;
	Flag *player_flag, *op1_flag, *op2_flag;
	float pspeed, speed1, speed2;
	FILE *f;

//metode
public:
	
	//constructor .. e apelat cand e instantiata clasa
	Laborator(){
		// initializari variabile asociate:
		// inputului de la tastatura
		// activarii camerelor
		// starii jocului(pauza sau finalul jocului)
		// culorilor jucatorilor
		upFlag = false;		downFlag = false;	rightFlag = false;	leftFlag = false;
		player_cam = false;	op1_cam = false;		op2_cam = false;		
		start = 0;		spider = 1;		victory = 0;
		player_color = glm::vec3(135/255.0, 206/255.0, 250/255.0);
		op1_color = glm::vec3(76/255.0, 187/255.0, 23/255.0);
		op2_color = glm::vec3(255/255.0, 216/255.0, 0/255.0);

		// citire viteze din fisier
		f = fopen("viteze.txt", "r");
		fscanf(f, "%f", &pspeed);
		fscanf(f, "%f", &speed1);
		fscanf(f, "%f", &speed2);
		fclose(f);
		printf("%f %f %f\n", pspeed, speed1, speed2);

		// setari pentru desenare, clear color seteaza culoarea de clear pentru ecran (format R,G,B,A)
		glClearColor(0.5,0.5,0.5,1);
		glClearDepth(1);			//clear depth si depth test (nu le studiem momentan, dar avem nevoie de ele!)
		glEnable(GL_DEPTH_TEST);	//sunt folosite pentru a determina obiectele cele mai apropiate de camera (la curs: algoritmul pictorului, algoritmul zbuffer)
		
		// incarca un shader din fisiere si gaseste locatiile matricilor relativ la programul creat
		gl_program_shader = lab::loadShader("shadere\\shader_vertex.glsl", "shadere\\shader_fragment.glsl");
		
		// initializare obiecte: pista, jucatori, camere, steaguri
		// reinitializarile au fost facute datoria unui comportament anormal dupa instantierea camerelor si a steagurilor:
		// disparitia jucatorilor dupa instantierile pomenite
		track		= new Track(RADIUS, glm::vec3(124/255.0, 10/255.0, 2/255.0));		
		opponent1	= new Player("resurse\\body.obj", gl_program_shader, glm::vec3(46, 0, -95), op1_color, 0);
		opponent2	= new Player("resurse\\body.obj", gl_program_shader, glm::vec3(72, 0, -135), op2_color, 0);
		player		= new Player("resurse\\body.obj", gl_program_shader, glm::vec3(59, 0, -115), player_color, 1);
		spider_cam	= new TPS(gl_program_shader);
		opponent1	= new Player("resurse\\body.obj", gl_program_shader, glm::vec3(46, 0, -95), op1_color, 0);

		player_flag = new Flag(player_color, gl_program_shader);
		op1_flag	= new Flag(op1_color, gl_program_shader);
		op2_flag	= new Flag(op2_color, gl_program_shader);
		opponent2	= new Player("resurse\\body.obj", gl_program_shader, glm::vec3(72, 0, -135), op2_color, 0);
		player		= new Player("resurse\\body.obj", gl_program_shader, glm::vec3(59, 0, -115), player_color, 1);

		//matrici de modelare si vizualizare
		model_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		cam.set(glm::vec3(TPX, TPY, TPZ), glm::vec3(0, 0, TPZ), glm::vec3(0, 0, 1));
		
		//desenare wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(10);
		glPointSize(10);
	}

	//destructor .. e apelat cand e distrusa clasa
	~Laborator(){
		//distruge shader
		glDeleteProgram(gl_program_shader);

		//distruge mesh incarcat
		delete player;
		delete spider_cam;
		delete opponent1;
		delete opponent2;
		delete track;
		delete player_flag;
		delete op1_flag;
		delete op2_flag;
	}
	
	//--------------------------------------------------------------------------------------------
	//functii de cadru ---------------------------------------------------------------------------

	//functie chemata inainte de a incepe cadrul de desenare, o folosim ca sa updatam situatia scenei ( modelam/simulam scena)
	void notifyBeginFrame() { }

	//functia de afisare (lucram cu banda grafica)
	void notifyDisplayFrame(){
		//pe tot ecranul
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//cadru normal
		{
			glViewport(0, 0, screen_width, screen_height);
			
			//foloseste shaderul
			glUseProgram(gl_program_shader);
				
			//trimite variabile uniforme la shader
			glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"model_matrix"),1,false,glm::value_ptr(model_matrix));
			glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"view_matrix"),1,false,glm::value_ptr(cam.getViewMatrix()));
			glUniformMatrix4fv(glGetUniformLocation(gl_program_shader,"projection_matrix"),1,false,glm::value_ptr(projection_matrix));
			glUniform1f(glGetUniformLocation(gl_program_shader, "count"), 0);
			
			// variabila start este 0 cand jocul este in pauza
			if (start == 1) {
				// verificare conditie de victorie a jucatorului
				if (player->victory(RADIUS * ratio)) {
					start = 0;  victory = 1;
					winner = 0;
				}
				// verificare daca este apasata tasta UP
				if (upFlag == true) {
					player->translate(RADIUS, -pspeed);
					if (player_cam == true) {
						player->activateFPSCam(&cam);
					} else if (TPSCam == true) {
						player->activateTPSCam(RADIUS, &cam);
					}	
				}
				// verificare daca este apasata tasta DOWN
				if (downFlag == true) {
					player->translate(RADIUS, pspeed / 3);
					if (player_cam == true) {
						player->activateFPSCam(&cam);
					} else if (TPSCam == true) {
						player->activateTPSCam(RADIUS, &cam);
					}	
				}
				// verificare daca este apasata tasta LEFT
				if (leftFlag == true) {
					player->rotate(0.9 * pspeed);
					if (player_cam == true) {
						player->activateFPSCam(&cam);
					} else if (TPSCam == true) {
						player->activateTPSCam(RADIUS, &cam);
					}
				}
				// verificare daca este apasata tasta RIGHT
				if (rightFlag == true) {
					player->rotate(-0.9 * pspeed);
					if (player_cam == true) {
						player->activateFPSCam(&cam);
					} else if (TPSCam == true) {
						player->activateTPSCam(RADIUS, &cam);
					}
				}
				// verificare conditie de victorie pentru oponentul 1
				if (opponent1->victory(RADIUS * ratio)) {
					start = 0; victory = 1;
					winner = 1;
				}
				// apelare metoda ce realizeaza miscarea automata a oponentului 1
				opponent1->automaticMovement(RADIUS, speed1);
				if (op1_cam == true) {
					opponent1->activateFPSCam(&cam);
				}
				// verificare conditie de victorie pentru oponentul 2
				if (opponent2->victory(RADIUS * ratio)) {
					start = 0; victory = 1;
					winner = 2;
				}
				// apelare metoda ce realizeaza miscarea automata a oponentului 2
				opponent2->automaticMovement(RADIUS, speed2);
				if (op2_cam == true) {
					opponent2->activateFPSCam(&cam);
				}

			}

			//desenare obiecte
			if (victory == 0) {
				// cazul in care jocul nu s-a terminat
				track->draw();
				player->draw();
				opponent1->draw();
				opponent2->draw();
				spider_cam->draw(player->TPcam_position);
			} else {
				// cazul in care jocul s-a terminat cu victoria unui jucator
				float millis = GetTickCount();
				cam.set(glm::vec3(TPX, TPY, TPZ), glm::vec3(0,0,TPZ), glm::vec3(0,0,1));
				if (winner == 0) player_flag->draw(millis/1000.0);
				if (winner == 1) op1_flag->draw(millis/1000.0);
				if (winner == 2) op2_flag->draw(millis/1000.0);
			}
		}
	}

	//functie chemata dupa ce am terminat cadrul de desenare (poate fi folosita pt modelare/simulare)
	void notifyEndFrame(){}

	//functei care e chemata cand se schimba dimensiunea ferestrei initiale
	void notifyReshape(int width, int height, int previos_width, int previous_height){
		//reshape
		if (height == 0) 
			height = 1;
		screen_width = width;
		screen_height = height;
		float aspect = width * 0.5f / height;
		projection_matrix = glm::perspective(75.0f, aspect, 0.1f, 10000.0f);
	}


	//--------------------------------------------------------------------------------------------
	//functii de input output --------------------------------------------------------------------
	
	//tasta apasata
	void notifyKeyPressed(unsigned char key_pressed, int mouse_x, int mouse_y){
		if(key_pressed == 27) lab::glut::close();	//ESC inchide glut si 
		if(key_pressed == 32) {
			//SPACE reincarca shaderul si recalculeaza locatiile (offseti/pointeri)
			glDeleteProgram(gl_program_shader);
			gl_program_shader = lab::loadShader("shadere\\shader_vertex.glsl", "shadere\\shader_fragment.glsl");
		}
		if(key_pressed == 'i'){
			static bool wire = true;
			wire=!wire;
			glPolygonMode(GL_FRONT_AND_BACK, (wire?GL_LINE:GL_FILL));
		}
		if(key_pressed == 'w' && spider == 1) { cam.translateForward(TRANS); }
		if(key_pressed == 'a' && spider == 1) { cam.translateRight(-TRANS); }
		if(key_pressed == 's' && spider == 1) { cam.translateForward(-TRANS); }
		if(key_pressed == 'd' && spider == 1) { cam.translateRight(TRANS); }
		if(key_pressed == 'r' && spider == 1) { cam.translateUpword(TRANS); }
		if(key_pressed == 'f' && spider == 1) { cam.translateUpword(-TRANS); }
		if(key_pressed == 'q' && spider == 1) { cam.rotateFPSoY( PI/45); }
		if(key_pressed == 'e' && spider == 1) { cam.rotateFPSoY(-PI/45); }
		if(key_pressed == 'z' && spider == 1) { cam.rotateFPSoZ(-PI/45); }
		if(key_pressed == 'c' && spider == 1) { cam.rotateFPSoZ( PI/45); }
		if(key_pressed == 't' && spider == 1) { cam.rotateFPSoX( PI/45); }
		if(key_pressed == 'g' && spider == 1) { cam.rotateFPSoX(-PI/45); }
		if(key_pressed == 'o') { cam.set(glm::vec3(TPX, TPY, TPZ), glm::vec3(0,0,TPZ), glm::vec3(0,0,1)); op1_cam = false; player_cam = false; op2_cam = false; TPSCam = false; spider = 1;}
		if(key_pressed == 'h' && victory == 0) { player->activateTPSCam(RADIUS, &cam);	op1_cam = false; player_cam = false; op2_cam = false; TPSCam = true;  spider = 0;}
		if(key_pressed == 'j' && victory == 0) { player->activateFPSCam(&cam);			op1_cam = false; player_cam = true;  op2_cam = false; TPSCam = false; spider = 0;}
		if(key_pressed == 'k' && victory == 0) { opponent1->activateFPSCam(&cam);		op1_cam = true;  player_cam = false; op2_cam = false; TPSCam = false; spider = 0;}
		if(key_pressed == 'l' && victory == 0) { opponent2->activateFPSCam(&cam);		op1_cam = false; player_cam = false; op2_cam = true;  TPSCam = false; spider = 0;}
		if(key_pressed == 'y') { start = start ^ 1; }
		//if(key_pressed == '=') { 
		//	opponent1	= new Player("resurse\\body.obj", gl_program_shader, glm::vec3(46, 0, -95), op1_color, 0);
		//	opponent2	= new Player("resurse\\body.obj", gl_program_shader, glm::vec3(72, 0, -135), op2_color, 0);
		//	player		= new Player("resurse\\body.obj", gl_program_shader, glm::vec3(59, 0, -115), player_color, 1);
		//	spider_cam	= new TPS(gl_program_shader);
		//	start = 0; victory = 0;
		//}
		if(key_pressed == ']') { victory = 1; winner = 0;}
		//if(key_pressed == '1') { cam.rotateTPSoX( PI/45,40.0f); }
		//if(key_pressed == '2') { cam.rotateTPSoX(-PI/45,40.0f); }
		//if(key_pressed == '3') { cam.rotateTPSoY( PI/45,40.0f); }
		//if(key_pressed == '4') { cam.rotateTPSoY(-PI/45,40.0f); }
		//if(key_pressed == '5') { cam.rotateTPSoZ( PI/45,40.0f); }
		//if(key_pressed == '6') { cam.rotateTPSoZ(-PI/45,40.0f); }
	}
	//tasta ridicata
	void notifyKeyReleased(unsigned char key_released, int mouse_x, int mouse_y){
	}

	//tasta speciala (up/down/F1/F2..) apasata
	void notifySpecialKeyPressed(int key_pressed, int mouse_x, int mouse_y){
		// modificare flaguri pentru tastele dorite cand tasta este tinuta apasata
		if(key_pressed == GLUT_KEY_F1) lab::glut::enterFullscreen();
		if(key_pressed == GLUT_KEY_F2) lab::glut::exitFullscreen();
		if(key_pressed == GLUT_KEY_UP) upFlag = true;
		if(key_pressed == GLUT_KEY_DOWN) downFlag = true;
		if(key_pressed == GLUT_KEY_LEFT) leftFlag = true;
		if(key_pressed == GLUT_KEY_RIGHT) rightFlag = true;
	}

	//tasta speciala ridicata
	void notifySpecialKeyReleased(int key_released, int mouse_x, int mouse_y){
		// modificare flaguri pentru tastele dorite cand se ia mana de pe tasta
		if(key_released == GLUT_KEY_UP) upFlag = false;
		if(key_released == GLUT_KEY_DOWN) downFlag = false;
		if(key_released == GLUT_KEY_LEFT) leftFlag = false;
		if(key_released == GLUT_KEY_RIGHT) rightFlag = false;
	}

	//drag cu mouse-ul
	void notifyMouseDrag(int mouse_x, int mouse_y){ }
	//am miscat mouseul (fara sa apas vreun buton)
	void notifyMouseMove(int mouse_x, int mouse_y){ }
	//am apasat pe un boton
	void notifyMouseClick(int button, int state, int mouse_x, int mouse_y){ }
	//scroll cu mouse-ul
	void notifyMouseScroll(int wheel, int direction, int mouse_x, int mouse_y){ }

};

int main(){
	//initializeaza GLUT (fereastra + input + context OpenGL)
	lab::glut::WindowInfo window(std::string("Tema 3 - Cursa Atletism"),800,600,100,100,true);
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
	std::cout<<"Taste:"<<std::endl<<"\tESC ... iesire"<<std::endl<<"\tSPACE ... reincarca shadere"<<std::endl<<"\ti ... toggle wireframe"<<std::endl<<"\to ... reseteaza camera"<<std::endl;
	std::cout<<"\tw ... translatie camera in fata"<<std::endl<<"\ts ... translatie camera inapoi"<<std::endl;
	std::cout<<"\ta ... translatie camera in stanga"<<std::endl<<"\td ... translatie camera in dreapta"<<std::endl;
	std::cout<<"\tr ... translatie camera in sus"<<std::endl<<"\tf ... translatie camera jos"<<std::endl;
	std::cout<<"\tq ... rotatie camera FPS pe Oy, minus"<<std::endl<<"\te ... rotatie camera FPS pe Oy, plus"<<std::endl;
	std::cout<<"\tz ... rotatie camera FPS pe Oz, minus"<<std::endl<<"\tc ... rotatie camera FPS pe Oz, plus"<<std::endl;

	//run
	lab::glut::run();

	return 0;
}