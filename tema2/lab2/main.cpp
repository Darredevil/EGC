#include "Framework/DrawingWindow.h"
#include "Framework/Visual2D.h"
#include "Framework/Transform2D.h"
#include "Framework/Transform3D.h"
#include "Framework/Line2D.h"
#include "Framework/Rectangle2D.h"
#include "Framework/Circle2D.h"
#include "Framework/Polygon2D.h"
#include "Framework/Line3D.h"
#include "Framework/Car.h"
#include "Framework/Pyramid3D.h"
#include "Framework/Landscape.h"
#include "Framework/Sphere3D.h"
#include <iostream>
#include <windows.h>
#include <ctime>

#define WIDTH 1200
#define HEIGHT 600

#define LIFE 5
#define LEVEL_TIME 80
#define SIDE_SPEED 9
#define MAX_SPEED 175
#define PI 3.14159265358979323846
#define INF 1000000
#define SPHERE_RADIUS 125
#define WIN_DISTANCE 3000
#define RAND_FACTOR1 4000
#define RAND_FACTOR2 2000
using namespace std;

Text *remTime, *totalDistanceText, *scoreText, *lifeText;
Visual2D *v2d1, *v2d2;
Object3D *cube11,*cube21,*cube31;
Point3D *laneEye, *carEye, *centreLaneEye, *leftLaneEye, *rightLaneEye;
Point3D centreLaneRespawn, leftLaneRespawn, rightLaneRespawn;
Line3D *myLine1, *myLine2, *myLine3, *myLine4;
Car *myCar, *carObst;
Grass *green1, *green2;
Road *road;
Pyramid3D *pyr;
Sphere3D *sph;
vector<Pyramid3D*> enemies;
vector<Sphere3D*> enemies1;
vector<Car*> carsObst;
vector<Point3D> spawnLocation;
vector<Point3D*> laneEyes;

float n = 100, pas = 0, angle;
float prop = 1 / 5.0;
float currSpeed = 0, totalDistance = 0;
float alternate = 1;

bool leftFlag = false, rightFlag = false, upFlag = false, downFlag = false;
bool add = false, start = true, ok = true;

int randomRespawnLocation, nextObj = 0, remainingSeconds = 0, countB = 0;
int type, score = 0, remLife = LIFE;

char timeFormat[10], distanceString[100];

time_t current, initial;


//functia care permite adaugarea de obiecte
void DrawingWindow::init()
{
	time(&initial);

	/* background */
	glClearColor(0, 191/255.0, 1, 1);

	/* contextul Visual2D asociat cerului */
	v2d2 = new Visual2D(0, 0, WIDTH, prop * HEIGHT, 0, 0, WIDTH - 18, prop * HEIGHT);
	v2d2->tipTran(false);
	addVisual2D(v2d2);

	/* contextul Visual2D asociat spatiului de joc */
	v2d1 = new Visual2D(0, 0, WIDTH, HEIGHT, 0, prop * HEIGHT, WIDTH - 18, HEIGHT - 10); 
	v2d1->tipTran(false);
	addVisual2D(v2d1);
	
	/* initializare ochi perspective */
	laneEye = new Point3D(WIDTH / 2, 1.2 * HEIGHT, -1000);
	carEye = new Point3D(WIDTH / 2, 0.6 * HEIGHT, 500);

	centreLaneEye = new Point3D(WIDTH / 2, 1.06 * HEIGHT, 1000);
	leftLaneEye = new Point3D(WIDTH / 2 - 34, 1.06 * HEIGHT, 1000);
	rightLaneEye = new Point3D(WIDTH / 2 + 34, 1.06 * HEIGHT, 1000);
	laneEyes.push_back(leftLaneEye);
	laneEyes.push_back(centreLaneEye);
	laneEyes.push_back(rightLaneEye);

	centreLaneRespawn = Point3D(600, 0, -8500);
	leftLaneRespawn = Point3D(300, 0, -8500);
	rightLaneRespawn = Point3D(900, 0, -8500);
	spawnLocation.push_back(leftLaneRespawn);
	spawnLocation.push_back(centreLaneRespawn);
	spawnLocation.push_back(rightLaneRespawn);

	/* initializare masina */
	myCar = new Car(Point3D(WIDTH / 2, 0, -20), carEye, Color(0, 0, 1), Color(0, 1, 0));
	myCar->draw(v2d1, 0);

	/* desenarea liniilor ce delimiteaza benzile */
	myLine1 = new Line3D(Point2D(150,0), laneEye);
	myLine1->draw(v2d1);
	myLine2 = new Line3D(Point2D(450,0), laneEye);
	myLine2->draw(v2d1);
	myLine3 = new Line3D(Point2D(750,0), laneEye);
	myLine3->draw(v2d1);
	myLine4 = new Line3D(Point2D(1050,0), laneEye);
	myLine4->draw(v2d1);
	
	/* desenare sosea */
	road = new Road(150, 1050, laneEye);
	road->draw(v2d1);

	/* desenare iarba */
	green1 = new Grass(150, 0, laneEye);
	green1->draw(v2d1);
	green2 = new Grass(1050, 1, laneEye);
	green2->draw(v2d1);

	/* adaugare text cu timp ramas */
	remTime = new Text("00:00", Point2D(575, 90), Color(0, 0, 0), BITMAP_TIMES_ROMAN_24); 
	addText_to_Visual2D(remTime, v2d2);	

	/* adaugare text cu distanta parcursa */
	totalDistanceText = new Text("Remaining Distance: " + to_string(WIN_DISTANCE), Point2D(20, 90), Color(0, 0, 0), BITMAP_TIMES_ROMAN_24); 
	addText_to_Visual2D(totalDistanceText, v2d2);

	/* adaugare text ce reprezinta scorul */
	scoreText = new Text("Score: 0", Point2D(20, 65), Color(0, 0, 0), BITMAP_TIMES_ROMAN_24); 
	addText_to_Visual2D(scoreText, v2d2);

	/* adaugare text cu numarul de vieti ramase */
	lifeText = new Text("Remaining Lifes: 5", Point2D(20, 40), Color(0, 0, 0), BITMAP_TIMES_ROMAN_24);
	addText_to_Visual2D(lifeText, v2d2);

}


//functia care permite animatia
void DrawingWindow::onIdle()
{
	if(remLife > 0 && (int) totalDistance / 300 <= WIN_DISTANCE) {
		/* update timp, distanta si scor */
		time(&current);
		remainingSeconds = (int) difftime(current, initial);

		int firstDigitMin = remainingSeconds / 60 / 10;
		int secondDigitMin = remainingSeconds / 60 % 10;
		int firstDigitSec = remainingSeconds % 60 / 10;
		int secondDigitSec = remainingSeconds % 60 % 10;
		sprintf(timeFormat, "%d%d:%d%d", firstDigitMin, secondDigitMin, firstDigitSec, secondDigitSec);
		remTime->text = string(timeFormat);
		
		countB = sprintf(distanceString, "Remaining Distance: %d", WIN_DISTANCE - (int) totalDistance / 300);
		totalDistanceText->text = string(distanceString, countB);

		scoreText->text = "Points: " + to_string(score);


		/* generarea primului obstacol */
		if(totalDistance > 2500 && start) {
			srand(time(NULL));
			randomRespawnLocation = rand() % 3;
			type = rand() % 20;
			if(type >= 0 && type < 10){
				pyr = new Pyramid3D(spawnLocation[randomRespawnLocation], laneEyes[randomRespawnLocation]);
				pyr->draw(v2d1, pas);
				enemies.push_back(pyr);
			} else if(type >= 10 && type < 19){
				sph = new Sphere3D(spawnLocation[randomRespawnLocation], SPHERE_RADIUS, laneEyes[randomRespawnLocation]);
				sph->draw(v2d1,pas);
				enemies1.push_back(sph);
			} else {
				carObst = new Car(spawnLocation[randomRespawnLocation], laneEyes[randomRespawnLocation], Color(0, 1, 0), Color(1, 0, 0));
				carObst->draw(v2d1, pas);
				carsObst.push_back(carObst);
			}
			start = false;

			myLine1->redraw(v2d1);
			myLine2->redraw(v2d1);
			myLine3->redraw(v2d1);
			myLine4->redraw(v2d1);
			road->redraw(v2d1);
			green1->redraw(v2d1);
			green2->redraw(v2d1);

			nextObj = rand() % 4000 + 2000;
		}

		/* deplasare spre stanga */
		if(leftFlag && pas < 370){
			if(myCar->angle < 0)
				myCar->rotate(-myCar->angle);

			pas += 1.2 * SIDE_SPEED + currSpeed / 15.0;
		
			myLine1->translate(pas);
			myLine2->translate(pas);
			myLine3->translate(pas);
			myLine4->translate(pas);
			road->translate(pas);
			green1->translate(pas);
			green2->translate(pas);
		
			centreLaneEye->x += SIDE_SPEED / 8.0 + currSpeed / 144.0;
			leftLaneEye->x += SIDE_SPEED / 8.0 + currSpeed / 144.0;
			rightLaneEye->x += SIDE_SPEED / 8.0 + currSpeed / 144.0;

			for(int i = 0; i < enemies.size(); i++){
				enemies.at(i)->move(pas, 0);
			}
			for(int i = 0; i < enemies1.size(); i++){
				enemies1.at(i)->move(pas, 0);
			}
			for(int i = 0; i < carsObst.size(); i++){
				carsObst.at(i)->move(pas, 0);
			}


			myCar->rotate(PI/500);
	
		}

		/* deplasare spre dreapta */
		if(rightFlag && pas > -370){
			if(myCar->angle > 0)
				myCar->rotate(-myCar->angle);

			pas -= 1.2 * SIDE_SPEED + currSpeed / 15.0;
		
			myLine1->translate(pas);
			myLine2->translate(pas);
			myLine3->translate(pas);
			myLine4->translate(pas);
			road->translate(pas);
			green1->translate(pas);
			green2->translate(pas);

			centreLaneEye->x -= SIDE_SPEED / 8.0 + currSpeed / 144.0;
			leftLaneEye->x -= SIDE_SPEED / 8.0 + currSpeed / 144.0;
			rightLaneEye->x -= SIDE_SPEED / 8.0 + currSpeed / 144.0;

			for(int i = 0; i < enemies.size(); i++){
				enemies.at(i)->move(pas, 0);
			}
			for(int i = 0; i < enemies1.size(); i++){
				enemies1.at(i)->move(pas, 0);
			}
			for(int i = 0; i < carsObst.size(); i++){
				carsObst.at(i)->move(pas, 0);
			}

			myCar->rotate(-PI/500);

		}

		/* deplasare inainte cu acceleratie */
		if(upFlag){
			if(currSpeed < MAX_SPEED){
				currSpeed += 2.5;
			}
			totalDistance += currSpeed;
			nextObj -= currSpeed;
			for(int i = 0; i < enemies.size(); i++){
				enemies.at(i)->move(pas, currSpeed);
			}
			for(int i = 0; i < enemies1.size(); i++){
				enemies1.at(i)->move(pas, currSpeed);
			}
			for(int i = 0; i < carsObst.size(); i++){
				carsObst.at(i)->move(pas, currSpeed);
			}

		} else {
			/* deccelerare */
			if(downFlag == false){
				if(currSpeed > 0){

					if(currSpeed - 7 < 0)
						currSpeed = 0;
					else
						currSpeed -= 1;

					totalDistance += currSpeed;
					nextObj -= currSpeed;

					for(int i = 0; i < enemies.size(); i++){
						enemies.at(i)->move(pas, currSpeed);
					}
					for(int i = 0; i < enemies1.size(); i++){
						enemies1.at(i)->move(pas, currSpeed);
					}
					for(int i = 0; i < carsObst.size(); i++){
						carsObst.at(i)->move(pas, currSpeed);
					}

				}
			} else {
				/* franare */
				currSpeed = 0;
			}
		}

		/* aplicare viteza constanta masinilor obstacol */
		for(int i = 0; i < carsObst.size(); i++){
				carsObst.at(i)->constantSpeed(pas);
		}

		/* rotirea de revenire a masinii */
		if(leftFlag == false && rightFlag == false){
			if(myCar->angle < 0){
				if(myCar->angle + PI/100 > 0)
					myCar->rotate(-myCar->angle);
				else {
					myCar->rotate(PI/100);
				}
			}
			if(myCar->angle > 0){
				if(myCar->angle - PI/100 < 0)
					myCar->rotate(-myCar->angle);
				else {
					myCar->rotate(-PI/100);
				}
			}
		}

		/* verificare coliziune sau daca obiectele mai sunt in ecran */
		for(int i = 0; i < enemies.size(); i++){
			if(enemies.at(i)->collision(myCar)){
				remLife--;
				lifeText->text = "Remaining Lifes: " + to_string(remLife);
				score -= enemies.at(i)->points * 3;
				enemies.at(i)->erase(v2d1);
				enemies.erase(enemies.begin() + i);
				add = true;
			} else if(enemies.at(i)->isInBoundaries() == false){
				score += enemies.at(i)->points;
				enemies.at(i)->erase(v2d1);
				enemies.erase(enemies.begin() + i);
				add = true;
			}
		}
		for(int i = 0; i < enemies1.size(); i++){
			if(enemies1.at(i)->collision(myCar)){
				remLife--;
				lifeText->text = "Remaining Lifes: " + to_string(remLife);
				score -= enemies1.at(i)->points * 3;
				enemies1.at(i)->erase(v2d1);
				enemies1.erase(enemies1.begin() + i);
				add = true;
			} else if(enemies1.at(i)->isInBoundaries() == false){
				score += enemies1.at(i)->points;
				enemies1.at(i)->erase(v2d1);
				enemies1.erase(enemies1.begin() + i);
				add = true;
			}
		}
		for(int i = 0; i < carsObst.size(); i++){
			if(carsObst.at(i)->collision(myCar)){
				remLife--;
				lifeText->text = "Remaining Lifes: " + to_string(remLife);
				score -= carsObst.at(i)->points * 3;
				carsObst.at(i)->erase(v2d1);
				carsObst.erase(carsObst.begin() + i);
				add = true;
			} else if(carsObst.at(i)->isInBoundaries() == false){
				score += carsObst.at(i)->points;
				carsObst.at(i)->erase(v2d1);
				carsObst.erase(carsObst.begin() + i);
				add = true;
			}
		}

		/* verificare coliziuni intre obstacole mobile si statice */
		for(int i = 0; i < carsObst.size(); i++){
			for(int j = 0; j < enemies.size(); j++){
				if(enemies.at(j)->collision(carsObst.at(i))){
					enemies.at(j)->erase(v2d1);
					carsObst.at(i)->points += enemies.at(j)->points; 
					enemies.erase(enemies.begin() + j);
				}
			}
			for(int j = 0; j < enemies1.size(); j++){
				if(enemies1.at(j)->collision(carsObst.at(i))){
					enemies1.at(j)->erase(v2d1);
					carsObst.at(i)->points += enemies1.at(j)->points; 
					enemies1.erase(enemies1.begin() + j);
				}
			}
		}


		/* generare random de inamici */
		if(nextObj < 0 && start == false){
			Pyramid3D *newPyr;

			randomRespawnLocation = rand() % 3;
			type = rand() % 20;
			if(type >= 0  && type < 10){
				newPyr = new Pyramid3D(spawnLocation[randomRespawnLocation], laneEyes[randomRespawnLocation]);	
				enemies.push_back(newPyr);
				newPyr->draw(v2d1, pas);
			} else if(type >= 10 && type < 19){
				sph = new Sphere3D(spawnLocation[randomRespawnLocation], SPHERE_RADIUS, laneEyes[randomRespawnLocation]);
				enemies1.push_back(sph);
				sph->draw(v2d1, pas);
			} else {
				carObst = new Car(spawnLocation[randomRespawnLocation], laneEyes[randomRespawnLocation], Color(0, 1, 0), Color(1, 0, 0));
				carObst->draw(v2d1, pas);
				carsObst.push_back(carObst);
			}

			myLine1->redraw(v2d1);
			myLine2->redraw(v2d1);
			myLine3->redraw(v2d1);
			myLine4->redraw(v2d1);
			road->redraw(v2d1);
			green1->redraw(v2d1);
			green2->redraw(v2d1);

			nextObj = rand() % RAND_FACTOR1 + RAND_FACTOR2;
		}
	} else if(remLife == 0) {
		Text *gOver = new Text("GAME OVER!", Point2D(528, 65), Color(1, 0, 0), BITMAP_TIMES_ROMAN_24);
		addText_to_Visual2D(gOver, v2d2);
	} else if((int) totalDistance > WIN_DISTANCE){
		Text *gOver = new Text("YOU WON!", Point2D(543, 65), Color(1, 0, 0), BITMAP_TIMES_ROMAN_24);
		addText_to_Visual2D(gOver, v2d2);
	}
}

//functia care se apeleaza la redimensionarea ferestrei
void DrawingWindow::onReshape(int width,int height)
{

}

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
		case GLUT_KEY_UP :
			upFlag = true;
			break;
		case GLUT_KEY_DOWN :
			downFlag = true;
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
		case GLUT_KEY_DOWN :
			downFlag = false;
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
	DrawingWindow dw(argc, argv, WIDTH, HEIGHT, 50, 25, "Crazy Driver");
	//se apeleaza functia init() - in care s-au adaugat obiecte
	dw.init();
	//se intra in bucla principala de desenare - care face posibila desenarea, animatia si procesarea evenimentelor
	dw.run();
	return 0;

}