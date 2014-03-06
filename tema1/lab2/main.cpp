#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Polygon2D.h"
#include "Framework/Ship.h"
#include "Framework/Enemies.h"
#include <iostream>
#include <windows.h>
#include <math.h>

using namespace std;

#define PI 3.14159265358979323846

#define SCREEN_HEIGHT 650
#define SCREEN_WIDTH 1300
#define SHIP_RADIUS 30
#define ROTATION_ANGLE PI/24

#define CENTRE_X 650
#define CENTRE_Y 320

int contor = 0, randType, randLoc, time = 600;
int score = 0;
float shipAngle = 0;
bool leftFlag = false, rightFlag = false, upFlag = false;

vector<Point2D> spawnLoc;
Visual2D *v2d1, *v2d2;
Text *showScore;
Ship *ship;
Enemy1 *first1;
Enemy2 *first2;
Enemy3 *first3;

vector<Circle2D*> life;
vector<Enemy1*> v1;
vector<Enemy2*> v2;
vector<Enemy3*> v3;

//functia care permite adaugarea de obiecte
void DrawingWindow::init()
{
	// initializare context vizual pentru spatiul de joc
	v2d1 = new Visual2D(0, 10, DrawingWindow::width - 20, DrawingWindow::height, 0, 0, DrawingWindow::width - 20, DrawingWindow::height - 10);  
	addVisual2D(v2d1);
	v2d1->cadruPoarta(Color(0,0,1));

	// initializare context vizual pentru afisarea vietilor si a scorului
	v2d2 = new Visual2D(0, 0, DrawingWindow::width - 20, 50, 0, 0, DrawingWindow::width - 20, 50);  
	addVisual2D(v2d2);
	v2d2->cadruPoarta(Color(1,0,0));

	// initializare text scor
	showScore = new Text(std::to_string(score));
	showScore->color = Color(0, 0, 1);
	showScore->pos = Point2D(635, 13);
	showScore->font = BITMAP_TIMES_ROMAN_24;
	addText_to_Visual2D(showScore, v2d2);

	// initializare vieti
	for(int i = 0; i < 5; i++) {
		Circle2D *newLife = new Circle2D(Point2D(1200 - i * 40, 22), 15, Color(1, 0, 0), true);
		addObject2D_to_Visual2D(newLife, v2d2);
		life.push_back(newLife);
	}

	// initalizare nava jucator
	ship = new Ship(CENTRE_X, CENTRE_Y, SHIP_RADIUS);
	ship->drawShip(v2d1);

	// initializare inamic tip 1
	first1 = new Enemy1(Point2D(500,100));
	first1->draw(v2d1);
	v1.push_back(first1);

	// initializare inamic tip 2
	first2 = new Enemy2(Point2D(100,100));
	first2->draw(v2d1);
	v2.push_back(first2);

	// initializare inamic tip 3
	first3 = new Enemy3(Point2D(1200,100));
	first3->draw(v2d1);
	v3.push_back(first3);

	// initializare puncte de respawn pentru inamici
	spawnLoc.push_back(Point2D(100,100));
	spawnLoc.push_back(Point2D(1200,100));
	spawnLoc.push_back(Point2D(100,500));
	spawnLoc.push_back(Point2D(1200,500));

}


//functia care permite animatia
void DrawingWindow::onIdle()
{
	if(life.size() != 0){

		// Keyboard input:
		if(upFlag)
			ship->moveForward();
		if(rightFlag)
			ship->rotate("right", ROTATION_ANGLE);
		if(leftFlag)
			ship->rotate("left", ROTATION_ANGLE);
		/* ---------------------- */

		contor++;

		// animatie, miscare si verificarea coliziunilor cu inamici de tip 1
		for(int i = 0; i < v1.size(); i++){
			v1[i]->rotateAnimation(PI/20);
			v1[i]->moveForward(3);
			int collisionType = ship->collision(v1[i], v2d1);
			if(collisionType != 0){
				score += v1[i]->points;
				if(collisionType == 1){
					removeObject2D_from_Visual2D(life.at(life.size() - 1), v2d2);
					life.erase(life.begin() + life.size() - 1);
					score -= v1[i]->points;
				}
				v1.erase(v1.begin() + i);
				showScore->text = std::to_string(score);
			}
		}

		// animatie, miscare si verificarea coliziunilor cu inamici de tip 2
		for(int i = 0; i < v2.size(); i++){
			v2[i]->rotateAnimation(PI/10);
			v2[i]->moveForward(3);
			int collisionType = ship->collision(v2[i], v2d1);
			if(collisionType != 0){
				score += v2[i]->points;
				if(collisionType == 1){
					removeObject2D_from_Visual2D(life.at(life.size() - 1), v2d2);
					life.erase(life.begin() + life.size() - 1);
					score -= v2[i]->points;
				}
				v2.erase(v2.begin() + i);
				showScore->text = std::to_string(score);
			}
		}

		// animatie, miscare si verificarea coliziunilor cu inamici de tip 3
		for(int i = 0; i < v3.size(); i++){
			v3[i]->rotateAnimation(PI/3);
			v3[i]->moveForward(3);
			int collisionType = ship->collision(v3[i], v2d1);
			if(collisionType != 0){
				score += v3[i]->points;
				if(collisionType == 1){
					removeObject2D_from_Visual2D(life.at(life.size() - 1), v2d2);
					life.erase(life.begin() + life.size() - 1);
					score -= v3[i]->points;
				}
				v3.erase(v3.begin() + i);
				showScore->text = std::to_string(score);
			}
		}

		// generare de noi inamici cu tip si locatie random
		if(contor == time){
			contor = 0;
			if(time > 100) {
				time -= 100;
			} else if (time > 50){
				time -= 50;
			}
			randType = rand() % 3 + 1;
			randLoc = rand() % 4;
			if(randType == 1){
				Enemy1 *newEnemy1 = new Enemy1(spawnLoc.at(randLoc));
				newEnemy1->draw(v2d1);
				v1.push_back(newEnemy1);
			} else if(randType == 2){
				Enemy2 *newEnemy2 = new Enemy2(spawnLoc.at(randLoc));
				newEnemy2->draw(v2d1);
				v2.push_back(newEnemy2);
			} else {
				Enemy3 *newEnemy3 = new Enemy3(spawnLoc.at(randLoc));
				newEnemy3->draw(v2d1);
				v3.push_back(newEnemy3);
			}
		
		}
	} else {
		Text *gameOver = new Text("GAME OVER!", Point2D(590, 320), Color(1, 0, 0), BITMAP_TIMES_ROMAN_24);
		addText_to_Visual2D(gameOver, v2d1);
	}
}

//functia care se apeleaza la redimensionarea ferestrei
void DrawingWindow::onReshape(int width,int height)
{

}

//functia care defineste ce se intampla cand se apasa pe tastatura
void DrawingWindow::onKey(unsigned char key)
{
	// functia care este apelata cand se tine apasata o tasta
	switch(key)
	{
		case 27 : 
			exit(0);
		case GLUT_KEY_LEFT :
			leftFlag = true;
			break;
		case GLUT_KEY_RIGHT :
			rightFlag = true;
			break;
		case KEY_SPACE :
			ship->toggleDrill(v2d1);
			break;
		case GLUT_KEY_UP :
			upFlag = true;
			break;
		
	}
}

void DrawingWindow::onKeyUp(unsigned char key)
{
	// functia care este apelata cand se da release la tasta ce era apasata
	switch(key)
	{
		case GLUT_KEY_LEFT :
			leftFlag = false;
			break;
		case GLUT_KEY_RIGHT :
			rightFlag = false;
			break;
		case GLUT_KEY_UP :
			upFlag = false;
			break;
	}
}

//functia care defineste ce se intampla cand se da click pe mouse
void DrawingWindow::onMouse(int button,int state,int x, int y)
{

}


int main(int argc, char** argv)
{
	//creare fereastra
	DrawingWindow dw(argc, argv, SCREEN_WIDTH, SCREEN_HEIGHT, 30, 30, "Geometry Wars");
	//se apeleaza functia init() - in care s-au adaugat obiecte
	dw.init();
	//se intra in bucla principala de desenare - care face posibila desenarea, animatia si procesarea evenimentelor
	dw.run();
	return 0;

}