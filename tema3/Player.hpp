#pragma once
#include "lab_mesh_loader.hpp"
#include "lab_camera.hpp"
#include <iostream>

#define PI 3.14159265359
#define CAMERA_HEIGHT 75
#define CAMERA_DISTANCE 110

class Player 
{
public:
	unsigned int mesh_vbo, mesh_ibo, mesh_vao, mesh_num_indices;
	unsigned int cam_vbo, cam_ibo, cam_vao, cam_num_indices;
	unsigned int gl_program_shader, following_cam;	
	glm::mat4 model_matrix, identity_matrix;
	glm::mat4 rotate_matrix, translate_matrix, scale_matrix;
	glm::vec3 position, color, TPcam_position, cam_position, cam_center;
	float dir_angle, scale_factor, center_dist;
	lab::Camera *cam;
	bool halfLap;

public:
	Player (std::string path, unsigned int _gl_program_shader, glm::vec3 init_pos, glm::vec3 color, int _TPS) {
		lab::loadObj(path, mesh_vao, mesh_vbo, mesh_ibo, mesh_num_indices, color);
		this->gl_program_shader = _gl_program_shader;

		scale_factor = 3;	dir_angle = 0;	center_dist = init_pos.x;	halfLap = false;	following_cam = _TPS;

		identity_matrix = rotate_matrix = translate_matrix, scale_matrix = glm::mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);
		position = glm::vec3(0, 0, 0);		color = glm::vec3(1, 1, 0);
		cam_position = init_pos + glm::vec3(0, CAMERA_HEIGHT, 0);
		TPcam_position = glm::vec3(CAMERA_DISTANCE, CAMERA_HEIGHT, init_pos.z);
		
		scale_matrix = glm::scale(scale_matrix, scale_factor * glm::vec3(1, 1, 1));
		rotate_matrix = glm::rotate(rotate_matrix, dir_angle + 180, glm::vec3(0, 1, 0));
		translate(init_pos);
	}

	void rotate(float _angle) {
		// metoda de rotire/schimbare de directie a playerului
		dir_angle += _angle;
		rotate_matrix = glm::rotate(rotate_matrix, _angle, glm::vec3(0, 1, 0));
	}

	void translate(glm::vec3 _pos) {
		// translatare la un punct 3D dat ca parametru
		position = _pos;
		cam_position = _pos + glm::vec3(0, 18, 0);
		TPcam_position = glm::vec3(CAMERA_DISTANCE, CAMERA_HEIGHT, _pos.z);
		translate_matrix = glm::translate(identity_matrix, _pos);
	}

	void translate(float radius, float distance) {
		// metoda de translatare cu distance unitati in directia in care este orientat jucatorul
		float z = distance * cos(PI * dir_angle/180);
		float x = distance * sin(PI * dir_angle/180);
		float x1, z1, dist;

		if (position.z > -radius * ratio  && position.z < 0) {
			if (position.x > 0) {
				x1 = std::min(position.x + x, radius);
				x1 = std::max(x1, radius / 2);
			} else {
				x1 = std::min(position.x + x, -radius / 2);
				x1 = std::max(x1, -radius);
			}
			x = x1 - position.x;
		} else if (position.z <= -radius * ratio) {
			dist = sqrt((position.x + x) * (position.x + x) + (position.z + z + radius * ratio) * (position.z + z + radius * ratio));
			if (dist > radius || dist < radius / 2) {
				x = 0; z = 0;
			}
		} else {
			dist = sqrt((position.x + x) * (position.x + x) + (position.z + z) * (position.z + z));
			if (dist > radius || dist < radius / 2) {
				x = 0; z = 0;
			}
		}
		position += glm::vec3(x, 0, z);
		cam_position += glm::vec3(x, 0, z);
		if (position.z > -radius * ratio / 2 && position.x < 0) halfLap = true;

		if (position.z <= -radius * ratio || position.z >= 0) {
			float zn, xn, angle, zc, xc;
			if (position.z <= -radius * ratio) {
				zn = radius * ratio + position.z;
				xn = position.x;
				if (position.x > 0) {
					angle = std::atan(zn/xn);
				} else {
					angle = std::atan(zn/xn) + PI;
				}
			} else {
				zn = position.z;
				xn = position.x;
				if (position.x < 0) {
					angle = std::atan(zn/xn) + PI;
				} else {
					angle = std::atan(zn/xn) + 2 * PI;
				}
			}
			zc = CAMERA_DISTANCE * sin(angle);
			xc = CAMERA_DISTANCE * cos(angle);
			if (position.z <= - radius * ratio) {
				TPcam_position = glm::vec3(0, CAMERA_HEIGHT, -radius * ratio) + glm::vec3(xc, 0, zc);
			} else {
				TPcam_position = glm::vec3(0, CAMERA_HEIGHT, 0) + glm::vec3(xc, 0, zc);
			}
		} else {
			if (position. x > 0) {
				TPcam_position = glm::vec3(CAMERA_DISTANCE, CAMERA_HEIGHT, position.z);
			} else {
				TPcam_position = glm::vec3(-CAMERA_DISTANCE, CAMERA_HEIGHT, position.z);
			}
		}
		
		translate_matrix = glm::translate(translate_matrix, glm::vec3(x, 0, z));
	}

	bool victory(float size) {
		// metoda de verificare a conditiei de victorie
		if (halfLap == true && position.x > 0 && position.z < -size / 2 + 15)
			return true;
		else
			return false;
	}

	void activateFPSCam(lab::Camera *_cam) {
		// metoda de calcul a punctului catre care priveste camera
		// si de activare si setare a camerei FPS
		// pozitia acesteia este pe umarul stang al jucatorului
		cam_center = cam_position + glm::vec3(-50 * sin(PI * dir_angle/180), 0, -50 * cos(PI * dir_angle/180));
		cam = _cam;
		_cam->set(cam_position, cam_center, glm::vec3(0, 1, 0));
	}

	void activateTPSCam(float radius, lab::Camera *_cam) {
		// metoda de calcul a punctului catre care priveste camera
		// si de activare si setare a camerei TPS
		// pozitia acesteia este in exteriorul stadionului
		if (TPcam_position.z <= 0 && TPcam_position.z >= -radius * ratio) {
			cam_center = glm::vec3(0, 0, TPcam_position.z);
		} else if (TPcam_position.z > 0) {
			cam_center = glm::vec3(0, 0, 0);
		} else if (TPcam_position.z < -radius * ratio) {
			cam_center = glm::vec3(0, 0, -radius * ratio);
		}
		cam = _cam;
		_cam->set(TPcam_position, cam_center, glm::vec3(0, 1, 0));
	}

	void automaticMovement(float radius, float distance) {
		// metoda ce determina deplasarea automata a jucatorului cu distance unitati
		// pe directia pe care este orientat
		if (position.z > -radius * ratio && position.z < 0) {
			if (position.x >= 0) {
				distance = std::min(position.z + radius * ratio, (double) distance);
				position += glm::vec3(0, 0, -distance);
				cam_position += glm::vec3(0, 0, -distance);
				translate_matrix = glm::translate(translate_matrix, glm::vec3(0, 0, -distance));
			} else {
				distance = std::min(-position.z, distance);
				position += glm::vec3(0, 0, distance);
				cam_position += glm::vec3(0, 0, distance);
				translate_matrix = glm::translate(translate_matrix, glm::vec3(0, 0, distance));
			}
		} else if (position.z > 0) { 
			float rotAngle = distance * 2;
			rotate(rotAngle / 2);
			double d = 2 * center_dist * sin(PI * rotAngle / 360);
			double x = -d * sin(PI * dir_angle/180.0) / 2;
			double z = -d * cos(PI * dir_angle/180.0) / 2;
			translate_matrix = glm::translate(translate_matrix, glm::vec3(x, 0, z));
			position += glm::vec3(x, 0, z);
			cam_position += glm::vec3(x, 0, z);
		} else if (position.z < -radius * ratio) {
			float rotAngle = distance * 2;
			rotate(rotAngle / 2);
			double d = 2 * center_dist * sin(PI * rotAngle / 360);
			double x = -d * sin(PI * dir_angle/180.0) / 2;
			double z = -d * cos(PI * dir_angle/180.0) / 2;
			translate_matrix = glm::translate(translate_matrix, glm::vec3(x, 0, z));
			position += glm::vec3(x, 0, z);
			cam_position += glm::vec3(x, 0, z);
		} else if (position.z == 0) {
			if (position.x > 0) {
				distance = std::min(position.z + radius * ratio, (double)distance);
				position += glm::vec3(0, 0, -distance);
				cam_position += glm::vec3(0, 0, -distance);
				translate_matrix = glm::translate(translate_matrix, glm::vec3(0, 0, -distance));
			} else {
				float rotAngle = distance * 2;
				rotate(rotAngle / 2);
				double d = 2 * center_dist * sin(PI * rotAngle / 360);
				double x = -d * sin(PI * dir_angle/180.0) / 2;
				double z = -d * cos(PI * dir_angle/180.0) / 2;
				translate_matrix = glm::translate(translate_matrix, glm::vec3(x, 0, z));
				position += glm::vec3(x, 0, z);
				cam_position += glm::vec3(x, 0, z);
			}
		} else if (position.z == -radius * ratio) {
			if (position.x > 0) {
				float rotAngle = distance * 2;
				rotate(rotAngle / 2);
				double d = 2 * center_dist * sin(PI * rotAngle / 360);
				double x = -d * sin(PI * dir_angle/180.0) / 2;
				double z = -d * cos(PI * dir_angle/180.0) / 2;
				translate_matrix = glm::translate(translate_matrix, glm::vec3(x, 0, z));
				position += glm::vec3(x, 0, z);
				cam_position += glm::vec3(x, 0, z);
			} else {
				distance = std::min(-position.z, distance);
				position += glm::vec3(0, 0, distance);
				cam_position += glm::vec3(0, 0, distance);
				translate_matrix = glm::translate(translate_matrix, glm::vec3(0, 0, distance));
			}
		}

		if (position.z > -radius * ratio / 2 && position.x < 0) halfLap = true;

		// calculul pozitiei unde trebuie plasata camera TPS in urma unei miscari
		// bucata aceasta de cod nu este utilizata in varianta finala a jocului
		// a fost scrisa cu scopul de a testa camera TPS pe jucatorii cu comportament automat
		if (position.z <= -radius * ratio || position.z >= 0) {
			float zn, xn, angle, zc, xc;
			if (position.z <= -radius * ratio) {
				zn = radius * ratio + position.z;
				xn = position.x;
				if (position.x > 0) {
					angle = std::atan(zn/xn);
				} else {
					angle = std::atan(zn/xn) + PI;
				}
			} else {
				zn = position.z;
				xn = position.x;
				if (position.x < 0) {
					angle = std::atan(zn/xn) + PI;
				} else {
					angle = std::atan(zn/xn) + 2 * PI;
				}
			}
			zc = CAMERA_DISTANCE * sin(angle);
			xc = CAMERA_DISTANCE * cos(angle);
			if (position.z <= - radius * ratio) {
				TPcam_position = glm::vec3(0, CAMERA_HEIGHT, -radius * ratio) + glm::vec3(xc, 0, zc);
			} else {
				TPcam_position = glm::vec3(0, CAMERA_HEIGHT, 0) + glm::vec3(xc, 0, zc);
			}
		} else {
			if (position. x > 0) {
				TPcam_position = glm::vec3(CAMERA_DISTANCE, CAMERA_HEIGHT, position.z);
			} else {
				TPcam_position = glm::vec3(-CAMERA_DISTANCE, CAMERA_HEIGHT, position.z);
			}
		}
	
	}
	
	void draw() {
		// metoda de redesenare a obiectului
		model_matrix = translate_matrix * rotate_matrix * scale_matrix;
		glUniformMatrix4fv(glGetUniformLocation(gl_program_shader, "model_matrix"), 1, false, glm::value_ptr(model_matrix));
		glBindVertexArray(mesh_vao);
		glDrawElements(GL_TRIANGLES, mesh_num_indices, GL_UNSIGNED_INT, 0);

	}

	~Player() {
		glDeleteBuffers(1,&mesh_vbo);
		glDeleteBuffers(1,&mesh_ibo);
		glDeleteVertexArrays(1,&mesh_vao);
	};

};