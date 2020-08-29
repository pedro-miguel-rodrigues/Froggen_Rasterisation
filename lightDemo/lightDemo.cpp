//
// AVT demo light 
// based on demos from GLSL Core Tutorial in Lighthouse3D.com   
//
// This demo was built for learning purposes only.
// Some code could be severely optimised, but I tried to
// keep as simple and clear as possible.
//
// The code comes with no warranties, use it at your own risk.
// You may use it, or parts of it, wherever you want.
//

#include <math.h>
#include <iostream>
#include <sstream>

#include <string.h>
#include <stdio.h>

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

#include <GL/gl.h>

#include <GL/glu.h>

#include <GL/glut.h>


// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

// Use Very Simple Libs
#include "VSShaderLib.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"
#include "basic_geometry.h"
#include "CameraProperties.h"
#include "TGA.h"
#include "l3DBillboard.h"
//#include "Texture_Loader.h"
#include "flare.h"
#include <IL/ilu.h>
#include <IL/il.h>
#include <IL/ilut.h>

#define MAX_PARTICULAS  1500
#define frand()			((float)rand()/RAND_MAX)
#define M_PI			3.14159265
#define CAPTION "AVT Per Fragment Phong Lightning Demo"


inline double clamp(const double x, const double min, const double max) {
	return (x < min ? min : (x > max ? max : x));
}

inline int clampi(const int x, const int min, const int max) {
	return (x < min ? min : (x > max ? max : x));
}


bool flareEffect = false;
bool flag;

int WindowHandle = 0;
int WinX = 640, WinY = 640;
int cameraId = 1;
int points = 0;
bool game_pause = false;

bool enableFog = false;

unsigned int FrameCount = 0;

float logx_array[5] = { 60.0f, 60.0f, 60.f, 20.0f, 20.0f };
float logz_array[5] = { -70.0f, -170.0f, 70.0f, -60.0f, 50.0f };

float carX_array[5] = { -27.0f, -27.0f, -27.0f, -67.0f, -67.0f };
float carZ_array[5] = { -50.0f, -100.0f, 80.0f, 20.0f, -100.0f };

float truckX_array[5] = { -47.0f, -47.0f, -47.0f, -87.0f, -87.0f };
float truckZ_array[5] = { -70.0f, 30.0f, 100.0f, 20.0f, -100.0f };

float turtleX_array[5] = { 40.0f, 40.0f, 40.0f, 80.0f, 80.0f };
float turtleZ_array[5] = { -70.0f, 30.0f, 110.0f, 60.0f, -100.0f };


typedef struct v3 {
	float x;
	float y;
	float z;
}vec3;

struct LightProperties {
	int isEnabled; // true to apply this light in this invocation
	int isLocal;	// true for a point light or a spotlight,
					// false for a positional light
	int isSpot;	// true if the light is a spotlight
	vec3 color;		// color of light
	vec3 position;	// location of light, if is Local is true,
						// otherwise the direction toward the light
	vec3 coneDirection; // spotlight attributes
	float spotCosCutoff;
	// other properties you may desire
};
#define N_LIGHTS 8
LightProperties lightProperties[N_LIGHTS];

typedef struct {
	float	life;		// vida
	float	fade;		// fade
	float	r, g, b;    // color
	GLfloat x, y, z;    // posio
	GLfloat vx, vy, vz; // velocidade 
	GLfloat ax, ay, az; // acelerao
} Particle;

Particle particula[MAX_PARTICULAS];
int dead_num_particles = 0;

int fireworks = 0;

float modelview[16];  //To be used in "Cheating" Matrix reset Billboard technique
float pos[3], right[3], up[3];

//game
int game_points = 0;
bool pause = false;
//frog position
float frog_x = -108.0f;
float frog_y = 1.0f;
float frog_z = -8.0f;
int frog_lives = 3;


//frog position
float frog2_x = -8.0f;
float frog2_y = 1.0f;
float frog2_z = -8.0f;

//Speed
float log_spd = 15.0f;
float car_spd = 12.0f;
float truck_spd = 8.0f;
float turtle_spd = 16.0f;


bool frog_on_long_log = false;
bool frog_on_short_log = false;
bool frog_on_turtle = false;

float frog_size = 16.0f;

float car_sizeX = 14.0f;
float car_sizeZ = 20.0f;

float truck_sizeX = 14.0f;
float truck_sizeZ = 40.0f;

float turtle_sizeX = 0.5f;
float turtle_sizeZ = 30.0f;

float short_log_sizeX = 8.0f;
float short_log_sizeZ = 30.0f;

float long_log_sizeX = 8.0f;
float long_log_sizeZ = 60.0f;

float x = 20.0f, y = 20.0f, z = 20.0f;
float cam[3] = { x,y,z };
int type = 0;

FLARE_DEF AVTflare;
float lightScreenPos[3];  //Position of the light in Window Coordinates

//log position
//float log2_x = 20.0f;
//float log2_y = 0.0f;
//float log2_z = -70.0f;

//counter
float counter = 0.0f;

long lastFrameTime = 0;

VSShaderLib shader;

struct MyMesh mesh[100];
int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh


//External array storage defined in AVTmathLib.cpp

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint lPos_uniformId;
GLint tex_loc, tex_loc1, tex_loc2, tex_loc3, tex_loc4, tex_normal;
GLint texMode_uniformId, shadowMode_uniformId;

GLuint TextureArray[6];
GLuint FlareTextureArray[5];

// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 39.0f, beta = 51.0f;
float r = 15.0f;

// Frame counting and FPS computation
long myTime, timebase = 0, frame = 0;
char s[32];
float lightPos[4] = { 4.0f, 15.0f, 2.0f, 1.0f };

void timer(int value)
{
	std::string hudString;
	if (game_pause && frog_lives > 0) hudString = " | Pause |";
	else if (frog_lives > 0) hudString = " | " + std::to_string(frog_lives) + " Lives | " + std::to_string(points) + " Points | ";
	else hudString = " | Game Over |";

	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")" << hudString;
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

void refresh(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, refresh, 0);
}

// ------------------------------------------------------------
//
// Reshape Callback Function
//
void setCameraLookAt() {
	switch (cameraId) {
	case 1:
		lookAt(orthoProperties.posX, orthoProperties.posY, orthoProperties.posZ,
			orthoProperties.lookX, orthoProperties.lookY, orthoProperties.lookZ,
			orthoProperties.upX, orthoProperties.upY, orthoProperties.upZ);
		break;

	case 2:
		lookAt(perspectiveProperties.posX, perspectiveProperties.posY, perspectiveProperties.posZ,
			perspectiveProperties.lookX, perspectiveProperties.lookY, perspectiveProperties.lookZ,
			perspectiveProperties.upX, perspectiveProperties.upY, perspectiveProperties.upZ);
		break;

	case 3:
		lookAt(frog_x - 10, frog_y + 10, frog_z + 8,
			frog_x + 50, frog_y, frog_z + 8,
			perspectiveProperties.upX, perspectiveProperties.upY, perspectiveProperties.upZ);
		break;
	}
}

void setCameraProjection() {
	loadIdentity(PROJECTION);

	switch (cameraId) {
	case 1:
		ortho(-orthoProperties.width / 2, orthoProperties.width / 2,
			-orthoProperties.height / 2, orthoProperties.height / 2,
			orthoProperties.nearPlane, orthoProperties.farPlane);
		break;

	case 2:
		perspective(perspectiveProperties.fov, perspectiveProperties.ratio,
			perspectiveProperties.nearPlane, perspectiveProperties.farPlane);
		break;

	case 3:
		perspective(perspectiveProperties.fov, perspectiveProperties.ratio,
			perspectiveProperties.nearPlane, perspectiveProperties.farPlane);
		break;
	}
}

void initializeCameras() {
	// set the camera position based on its spherical coordinates

	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r * sin(beta * 3.14f / 180.0f);

	// initialize cameras

	// Ortho
	orthoProperties.posX = 0;
	orthoProperties.posY = 100;
	orthoProperties.posZ = 0;

	orthoProperties.lookX = 0;
	orthoProperties.lookY = 0;
	orthoProperties.lookZ = 0;

	orthoProperties.upX = 1;
	orthoProperties.upY = 0;
	orthoProperties.upZ = 0;

	orthoProperties.nearPlane = 1;
	orthoProperties.farPlane = 1500;

	// Perspective
	perspectiveProperties.posX = -260;
	perspectiveProperties.posY = 250;
	perspectiveProperties.posZ = 0;

	perspectiveProperties.lookX = 0;
	perspectiveProperties.lookY = 0;
	perspectiveProperties.lookZ = 0;

	perspectiveProperties.upX = 1;
	perspectiveProperties.upY = 0;
	perspectiveProperties.upZ = 0;

	perspectiveProperties.fov = 30;
	perspectiveProperties.ratio = WinX / WinY;

	perspectiveProperties.nearPlane = 10;
	perspectiveProperties.farPlane = 1500;
}

void iterate(int value)
{
	int i;
	float h;

	/* Método de Euler de integração de eq. diferenciais ordinárias
	h representa o step de tempo; dv/dt = a; dx/dt = v; e conhecem-se os valores iniciais de x e v */

	h = 0.125f;
	//	h = 0.033;
	if (fireworks) {

		for (i = 0; i < MAX_PARTICULAS; i++)
		{
			particula[i].x += (h * particula[i].vx);
			particula[i].y += (h * particula[i].vy);
			particula[i].z += (h * particula[i].vz);
			particula[i].vx += (h * particula[i].ax);
			particula[i].vy += (h * particula[i].ay);
			particula[i].vz += (h * particula[i].az);
			particula[i].life -= particula[i].fade;
		}
		glutPostRedisplay();
		glutTimerFunc(20, iterate, 0);
	}
}

void changeSize(int w, int h) {

	float ratio;
	// Prevent a divide by zero, when window is too short
	if (h == 0)
		h = 1;
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// set the projection matrix
	ratio = (1.0f * w) / h;

	// update cameras based on ratio
	perspectiveProperties.ratio = ratio;
	carCamProperties.ratio = ratio;
	orthoProperties.height = 220;
	orthoProperties.width = 220;

	setCameraProjection();


}

typedef GLfloat Matrix[16];

void updateLog(float deltaTime) {
	int time = glutGet(GLUT_ELAPSED_TIME);
	for (int i = 0; i < 5; i++) {
		if (logz_array[i] > 170) {
			logz_array[i] = -180;
		}
		logz_array[i] += deltaTime * (log_spd + time / 1000);
	}
	if (frog_on_short_log && frog_z + frog_size + deltaTime * (log_spd + time / 1000) <= 110
		|| frog_on_long_log && frog_z + frog_size + deltaTime * (log_spd + time / 1000) <= 110) {
		frog_z += deltaTime * (log_spd + time / 1000);
		lightProperties[1].position.z = frog_z + 8;
	}

}

int frog2_direction = -1;

void updateFrog2(float deltaTime) {
	int time = glutGet(GLUT_ELAPSED_TIME);

	if (frog2_z <= -110) {
		frog2_direction = 1;
	}

	else if (frog2_z >= 110 - 16) {
		frog2_direction = -1;
	}
	
	frog2_z += frog2_direction * deltaTime * (car_spd + time / 1000);
}

void updateCar(float deltaTime) {
	int time = glutGet(GLUT_ELAPSED_TIME);
	for (int i = 0; i < 5; i++) {
		if (carZ_array[i] > 140) {
			carZ_array[i] = -130;
		}
		carZ_array[i] += deltaTime * (car_spd + time / 1000);
	}
}

void updateTruck(float deltaTime) {
	int time = glutGet(GLUT_ELAPSED_TIME);
	for (int i = 0; i < 5; i++) {
		if (truckZ_array[i] < -160) {
			truckZ_array[i] = 130;
		}
		truckZ_array[i] -= deltaTime * (truck_spd + time / 1000);
	}
}

void updateTurtle(float deltaTime) {
	int time = glutGet(GLUT_ELAPSED_TIME);
	for (int i = 0; i < 5; i++) {
		if (turtleZ_array[i] < -160) {
			turtleZ_array[i] = 130;
		}
		turtleZ_array[i] -= deltaTime * (turtle_spd + time / 1000);
	}
	if (frog_on_turtle && frog_z - deltaTime * (turtle_spd + time / 1000) >= -110) {
		frog_z -= deltaTime * (turtle_spd + time / 1000);
		lightProperties[1].position.z = frog_z + 8;
	}
}

// ------------------------------------------------------------
//
// Render stufff
//

void setRestartFrog() {
	frog_x = -108.0f;
	frog_y = 1.0f;
	frog_z = -8.0f;
	lightProperties[1].position.x = frog_x + 8;
	lightProperties[1].position.z = frog_z + 8;
}

void restart() {
	frog_x = -108.0f;
	frog_y = 1.0f;
	frog_z = -8.0f;
	lightProperties[1].position.x = frog_x + 8;
	lightProperties[1].position.z = frog_z + 8;
	game_points = 0;
	points = 0;
	frog_lives = 3;
	game_pause = false;
}

void iniParticulas();

void win_game() {
	points += 1;
	setRestartFrog();
	fireworks = 1;
	iniParticulas();
	glutTimerFunc(0, iterate, 0);
}

void lose_game() {
	frog_lives -= 1;
	setRestartFrog();
	if (frog_lives <= 0)
		game_pause = true;
}
bool collision(std::string object);

void checkCollisions() {

	float frog_nose = frog_x + frog_size;

	//the frog is in an area without possible collisions
		//set the flags to false when the game starts / when the frog moves back to the south bank
	if (frog_nose < 10) {
		frog_on_long_log = false;
		frog_on_short_log = false;
		frog_on_turtle = false;
	}

	//check for collisions with trucks
	if (frog_nose >= -90 && frog_nose < -70 || frog_nose >= -50 && frog_nose < -30) {

		if (collision("trucks")) {
			lose_game();
		}
	}

	//check for collisions with cars
	else if (frog_nose >= -70 && frog_nose < -50 || frog_nose >= -30 && frog_nose < -10) {

		if (collision("cars")) {
			lose_game();
		}
	}

	else if (frog_nose >= -10 && frog_nose < 10) {
		if (collision("frog2")) {
			lose_game();
		}
	}

	//check for collisions with long logs
	else if (frog_nose >= 10 && frog_nose < 30) {

		if (!collision("long_logs")) {
			lose_game();
		}

		else {
			frog_on_long_log = true;

			frog_on_short_log = false;
			frog_on_turtle = false;
		}
		//update frog's position when you update logs'
	}

	//check for collisions with turtles
	else if (frog_nose >= 30 && frog_nose < 50 || frog_nose >= 70 && frog_nose < 90) {

		if (!collision("turtles")) {
			lose_game();
		}

		else {
			frog_on_turtle = true;

			frog_on_long_log = false;
			frog_on_short_log = false;
		}
		//update frog's position when you update logs'
	}

	//check for collisions with short logs
	else if (frog_nose >= 50 && frog_nose < 70) {

		if (!collision("short_logs")) {
			lose_game();
		}

		else {
			frog_on_short_log = true;

			frog_on_turtle = false;
			frog_on_long_log = false;
		}
		//update frog's position when you update logs'
	}

	//the frog has reached the north bank of the river
	else if (frog_nose >= 90)
	{
		win_game();
	}
}

bool collisionDetection(float arrayX[], float arrayZ[], float object_sizeX, float object_sizeZ);
bool collisionLogDetection(float arrayX[], float arrayZ[], float object_sizeX, float object_sizeZ);
bool collisionSLogDetection(float arrayX[], float arrayZ[], float object_sizeX, float object_sizeZ);
bool collisionDetectionFrog2();


bool collision(std::string object) {

	if (object == "trucks") {
		return collisionDetection(truckX_array, truckZ_array, truck_sizeX, truck_sizeZ);
	}

	else if (object == "cars") {
		return collisionDetection(carX_array, carZ_array, car_sizeX, car_sizeZ);
	}

	else if (object == "frog2") {
		return collisionDetectionFrog2();
	}

	else if (object == "long_logs") {
		return collisionLogDetection(logx_array, logz_array, long_log_sizeX, long_log_sizeZ);
	}

	else if (object == "short_logs") {
		return collisionSLogDetection(logx_array, logz_array, short_log_sizeX, short_log_sizeZ);
	}

	else if (object == "turtles") {
		return collisionDetection(turtleX_array, turtleZ_array, turtle_sizeX, turtle_sizeZ);
	}


	else {
		return false;
	}
}

int findLane(float pos_x) { return (int)pos_x / 20; }


bool collisionDetectionFrog2() {

	float frog_nose = frog_x + frog_size;
	float frog2_nose = frog2_x + frog_size;

	float frog_right = frog_z + frog_size;
	float frog2_right = frog2_z + frog_size;

	float frog_left = frog_z;
	float frog2_left = frog2_z;

	if ((frog_right >= frog2_left && frog_left <= frog2_right) ||
		(frog_left <= frog2_right && frog_right >= frog2_right))
	{
		return true;
	}
	return false;

}



bool collisionDetection(float arrayX[], float arrayZ[], float object_sizeX, float object_sizeZ) {

	float frog_nose = frog_x + frog_size;

	float frog_right = frog_z + frog_size;
	float frog_left = frog_z;

	for (int i = 0; i < 5; i++) {


		if (findLane(frog_nose) == findLane(arrayX[i] + object_sizeX)) {

			if ((frog_right >= arrayZ[i] && frog_left <= (arrayZ[i] + object_sizeZ)) ||
				((frog_left) <= arrayZ[i] && (frog_right) >= (arrayZ[i] + object_sizeZ))) {
				return true;
			}
		}
	}
	return false;
}

bool collisionLogDetection(float arrayX[], float arrayZ[], float object_sizeX, float object_sizeZ) {

	float frog_nose = frog_x + frog_size;

	float frog_right = frog_z + frog_size;
	float frog_left = frog_z;

	for (int i = 0; i < 5; i++) {


		if (findLane(frog_nose) == findLane(arrayX[i] + object_sizeX)) {

			if ((frog_right >= arrayZ[i] - 30 && frog_left <= (arrayZ[i] - 30 + object_sizeZ)) ||
				((frog_left) <= arrayZ[i] - 30 && (frog_right) >= (arrayZ[i] - 30 + object_sizeZ))) {
				return true;
			}
		}
	}
	return false;
}

bool collisionSLogDetection(float arrayX[], float arrayZ[], float object_sizeX, float object_sizeZ) {

	float frog_nose = frog_x + frog_size;

	float frog_right = frog_z + frog_size;
	float frog_left = frog_z;

	for (int i = 0; i < 5; i++) {


		if (findLane(frog_nose) == findLane(arrayX[i] + object_sizeX)) {

			if ((frog_right >= arrayZ[i] - 20 && frog_left <= (arrayZ[i] - 20 + object_sizeZ)) ||
				((frog_left) <= arrayZ[i] - 20 && (frog_right) >= (arrayZ[i] - 20 + object_sizeZ))) {
				return true;
			}
		}
	}
	return false;
}

void iniParticulas(void)
{
	GLfloat v, theta, phi;
	int i;

	for (i = 0; i < MAX_PARTICULAS; i++)
	{
		v = 0.8 * frand() + 0.2;
		phi = frand() * M_PI;
		theta = 2.0 * frand() * M_PI;

		particula[i].z = 0.0f;
		particula[i].y = 70.0f;
		particula[i].x = 0.0f;
		particula[i].vz = v * cos(theta) * sin(phi);
		particula[i].vy = v * cos(phi);
		particula[i].vx = v * sin(theta) * sin(phi);
		particula[i].az = 0.25f; /* simular um pouco de vento */
		particula[i].ay = -0.15f; /* simular a aceleração da gravidade */
		particula[i].ax = 0.0f;

		/* tom amarelado que vai ser multiplicado pela textura que varia entre branco e preto */
		particula[i].r = 0.882f;
		particula[i].g = 0.552f;
		particula[i].b = 0.211f;

		particula[i].life = 1.0f;		/* vida inicial */
		particula[i].fade = 0.007f;	    /* step de decréscimo da vida para cada iteração */
	}
}


void drawTurtles() {
	//TURTLES
	GLint loc;
	int objId = 62;
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0; i < 5; i++) {
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, turtleX_array[i], 2.0f, turtleZ_array[i]);
		scale(MODEL, 1.0f, 0.5f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, turtleX_array[i], 2.0f, turtleZ_array[i] + 12.0f);
		scale(MODEL, 1.0f, 0.5f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, turtleX_array[i], 2.0f, turtleZ_array[i] + 24.0f);
		scale(MODEL, 1.0f, 0.5f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
	}

}


void drawCars() {
	GLint loc;
	int objId = 12;
	//CARS
	for (int i = 0; i < 5; i++) {
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, carX_array[i], 1.0f, carZ_array[i]);
		scale(MODEL, 14.0f, 5.0f, 20.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		//WHEEL 1
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, carX_array[i] - 1.0f, 3.0f, carZ_array[i] + 4.0f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		//WHEEL 2
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, carX_array[i] + 15.0f, 3.0f, carZ_array[i] + 4.0f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		//WHEEL 3
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, carX_array[i] + 15.0f, 3.0f, carZ_array[i] + 16.0f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		//WHEEL 4
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, carX_array[i] - 1.0f, 3.0f, carZ_array[i] + 16.0f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
	}
}


void drawObjects() {
	GLint loc;
	float particle_color[4];
	objId = 4;

	//logs
	for (int i = 0; i < 5; i++) {
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, logx_array[i], -2.0f, logz_array[i]);
		rotate(MODEL, 90.0f, 90.0f, 0.0f, 0.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
	}

	//frog body
	objId++;

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	pushMatrix(MODEL);
	translate(MODEL, frog_x, frog_y, frog_z);
	scale(MODEL, 16.0f, 5.0f, 16.0f);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
	glUniform1i(texMode_uniformId, 1);
	// Render mesh
	glBindVertexArray(mesh[objId].vao);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);
	//frog left eye
	objId++;

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	pushMatrix(MODEL);
	translate(MODEL, frog_x + 12, frog_y + 5, frog_z + 5);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
	glUniform1i(texMode_uniformId, 1);

	// Render mesh
	glBindVertexArray(mesh[objId].vao);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);

	//frog right eye
	objId++;

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	pushMatrix(MODEL);
	translate(MODEL, frog_x + 12, frog_y + 5, frog_z + 11);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
	glUniform1i(texMode_uniformId, 1);

	// Render mesh
	glBindVertexArray(mesh[objId].vao);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);

	//CARS
	for (int i = 0; i < 5; i++) {
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, carX_array[i], 1.0f, carZ_array[i]);
		scale(MODEL, 14.0f, 5.0f, 20.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		//WHEEL 1
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, carX_array[i] - 1.0f, 3.0f, carZ_array[i] + 4.0f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		//WHEEL 2
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, carX_array[i] + 15.0f, 3.0f, carZ_array[i] + 4.0f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		//WHEEL 3
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, carX_array[i] + 15.0f, 3.0f, carZ_array[i] + 16.0f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		//WHEEL 4
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, carX_array[i] - 1.0f, 3.0f, carZ_array[i] + 16.0f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
	}

	//TRUCKS
	for (int i = 0; i < 5; i++) {
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, truckX_array[i], 1.0f, truckZ_array[i]);
		scale(MODEL, 14.0f, 5.0f, 40.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		//WHEEL 1
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, truckX_array[i] - 1.0f, 3.0f, truckZ_array[i] + 4.0f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		//WHEEL 2
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, truckX_array[i] + 15.0f, 3.0f, truckZ_array[i] + 4.0f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		//WHEEL 3
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, truckX_array[i] + 15.0f, 3.0f, truckZ_array[i] + 36.0f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		//WHEEL 4
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, truckX_array[i] - 1.0f, 3.0f, truckZ_array[i] + 36.0f);
		rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);
	}

	//TURTLES
	for (int i = 0; i < 5; i++) {
		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, turtleX_array[i], 2.0f, turtleZ_array[i]);
		scale(MODEL, 1.0f, 0.5f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, turtleX_array[i], 2.0f, turtleZ_array[i] + 12.0f);
		scale(MODEL, 1.0f, 0.5f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);

		objId++;

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[objId].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[objId].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[objId].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, turtleX_array[i], 2.0f, turtleZ_array[i] + 24.0f);
		scale(MODEL, 1.0f, 0.5f, 1.0f);

		// send matrices to OGL
		computeDerivedMatrix(PROJ_VIEW_MODEL);
		glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
		glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
		computeNormalMatrix3x3();
		glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
		glUniform1i(texMode_uniformId, 1);

		// Render mesh
		glBindVertexArray(mesh[objId].vao);

		if (!shader.isProgramValid()) {
			printf("Program Not Valid!\n");
			exit(1);
		}
		glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		popMatrix(MODEL);


	}

	//frog2 body
	objId++;

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	pushMatrix(MODEL);
	translate(MODEL, frog2_x, frog2_y, frog2_z);
	scale(MODEL, 16.0f, 5.0f, 16.0f);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
	glUniform1i(texMode_uniformId, 1);
	// Render mesh
	glBindVertexArray(mesh[objId].vao);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);
	//frog left eye
	objId++;

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	pushMatrix(MODEL);
	translate(MODEL, frog2_x + 4, frog2_y + 5, frog2_z + 5);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
	glUniform1i(texMode_uniformId, 1);

	// Render mesh
	glBindVertexArray(mesh[objId].vao);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);

	//frog right eye
	objId++;

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	pushMatrix(MODEL);
	translate(MODEL, frog2_x + 4, frog2_y + 5, frog2_z + 11);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
	glUniform1i(texMode_uniformId, 1);

	// Render mesh
	glBindVertexArray(mesh[objId].vao);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);

    glUniform1i(texMode_uniformId, 4); // draw textured quads
    objId++;
	//for (int i = -5; i < 5; i++)
		for (int j = -5; j < 5; j++) {
			pushMatrix(MODEL);
			translate(MODEL, 0, -9, 5 + j * 10.0);

			pos[0] = 0; pos[1] = 0; pos[2] = 5 + j * 10.0;

			type = 0;
			if (type == 2)
				l3dBillboardSphericalBegin(cam, pos);
			else if (type == 3)
				l3dBillboardCylindricalBegin(cam, pos);

			  //quad for tree

			//diffuse and ambient color are not used in the tree quads
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[objId].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc, mesh[objId].mat.shininess);

			pushMatrix(MODEL);
			translate(MODEL, 100.0, 5.0, 0.0f);

			// send matrices to OGL
			if (type == 0 || type == 1) {     //Cheating matrix reset billboard techniques
				computeDerivedMatrix(VIEW_MODEL);
				memcpy(modelview, mCompMatrix[VIEW_MODEL], sizeof(float) * 16);  //save VIEW_MODEL in modelview matrix

				//reset VIEW_MODEL
				if (type == 0) BillboardCheatSphericalBegin();
				else BillboardCheatCylindricalBegin();

				computeDerivedMatrix_PVM(); // calculate PROJ_VIEW_MODEL
			}
			else computeDerivedMatrix(PROJ_VIEW_MODEL);

			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			popMatrix(MODEL);

			//	if (type==0 || type==1) // restore matrix   
				//	BillboardEnd(modelview);   // não é necessário pois a PVM é sempre calculada a pArtir da MODEL e da VIEW que não são ALTERADAS

			popMatrix(MODEL);
		}


	if (fireworks) {
		// draw fireworks particles
		objId ++;  //quad for particle


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDepthMask(GL_FALSE);  //Depth Buffer Read Only

		glUniform1i(texMode_uniformId, 2); // draw modulated textured particles 

		for (int i = 0; i < MAX_PARTICULAS; i++)
		{
			if (particula[i].life > 0.0f) /* só desenha as que ainda estão vivas */
			{

				/* A vida da partícula representa o canal alpha da cor. Como o blend está activo a cor final é a soma da cor rgb do fragmento multiplicada pelo
				alpha com a cor do pixel destino */

				particle_color[0] = particula[i].r;
				particle_color[1] = particula[i].g;
				particle_color[2] = particula[i].b;
				particle_color[3] = particula[i].life;

				// send the material - diffuse color modulated with texture
				loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
				glUniform4fv(loc, 1, particle_color);

				pushMatrix(MODEL);

				translate(MODEL, particula[i].x, particula[i].y, particula[i].z);
				rotate(MODEL, -90, 0, 1, 0);
				scale(MODEL, 2, 2, 1);

				// send matrices to OGL
				computeDerivedMatrix(PROJ_VIEW_MODEL);
				glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
				glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
				computeNormalMatrix3x3();
				glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

				glBindVertexArray(mesh[objId].vao);
				glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
				popMatrix(MODEL);
			}
			else dead_num_particles++;
		}

		glDepthMask(GL_TRUE); //make depth buffer again writeable
		if (dead_num_particles == MAX_PARTICULAS) {
			fireworks = 0;
			dead_num_particles = 0;
			printf("All particles dead\n");
		}
	}

	


}

void mirrorLight() {
	for (int i = 0; i < 8; i++) {
		lightProperties[i].position.y *= (-1.0f);
	}
}

void sendLight() {
	GLint loc;
	float res[4];
	float aux[4];

	for (int i = 0; i < 8; i++) {

		aux[0] = lightProperties[i].position.x;
		aux[1] = lightProperties[i].position.y;
		aux[2] = lightProperties[i].position.z;
		aux[3] = 1.0f;
		multMatrixPoint(VIEW, aux, res);

		loc = glGetUniformLocation(shader.getProgramIndex(), (std::string("Lights[") + std::to_string(i) + std::string("].isEnabled")).c_str());
		glUniform1i(loc, lightProperties[i].isEnabled);
		loc = glGetUniformLocation(shader.getProgramIndex(), (std::string("Lights[") + std::to_string(i) + std::string("].isLocal")).c_str());
		glUniform1i(loc, lightProperties[i].isLocal);
		loc = glGetUniformLocation(shader.getProgramIndex(), (std::string("Lights[") + std::to_string(i) + std::string("].isSpot")).c_str());
		glUniform1i(loc, lightProperties[i].isSpot);
		loc = glGetUniformLocation(shader.getProgramIndex(), (std::string("Lights[") + std::to_string(i) + std::string("].position")).c_str());
		glUniform3f(loc, res[0], res[1], res[2]);
		loc = glGetUniformLocation(shader.getProgramIndex(), (std::string("Lights[") + std::to_string(i) + std::string("].coneDirection")).c_str());
		glUniform3f(loc, lightProperties[i].coneDirection.x, lightProperties[i].coneDirection.y, lightProperties[i].coneDirection.z);
		loc = glGetUniformLocation(shader.getProgramIndex(), (std::string("Lights[") + std::to_string(i) + std::string("].spotCosCutoff")).c_str());
		glUniform1f(loc, lightProperties[i].spotCosCutoff);
	}
}

void drawMirror() {
	GLint loc;
	objId = 0;
	//for (int i = 0 ; i < 2; ++i) {
	//	for (int j = 0; j < 2; ++j) {
			// send the material
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	pushMatrix(MODEL);
	translate(MODEL, -10.0f, 0.0f, -110.0f);
	scale(MODEL, 20.0f, 1.0f, 220.0f);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
	glUniform1i(texMode_uniformId, 1);


	// Render mesh
	glBindVertexArray(mesh[objId].vao);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);
	objId++;

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	pushMatrix(MODEL);
	translate(MODEL, -90.0f, 0.0f, -110.0f);
	scale(MODEL, 80.0f, 1.0f, 220.0f);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
	glUniform1i(texMode_uniformId, 3);

	// Render mesh
	glBindVertexArray(mesh[objId].vao);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);
	objId++;

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	pushMatrix(MODEL);
	translate(MODEL, -110.0f, 0.0f, -110.0f);
	scale(MODEL, 20.0f, 1.0f, 220.0f);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
	glUniform1i(texMode_uniformId, 1);

	// Render mesh
	glBindVertexArray(mesh[objId].vao);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);
	objId++;

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	pushMatrix(MODEL);
	translate(MODEL, 10.0f, 0.0f, -110.0f);
	scale(MODEL, 80.0f, 1.0f, 220.0f);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
	glUniform1i(texMode_uniformId, 0);

	// Render mesh
	glBindVertexArray(mesh[objId].vao);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);
	objId++;

	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
	pushMatrix(MODEL);
	translate(MODEL, 90.0f, 0.0f, -110.0f);
	scale(MODEL, 20.0f, 1.0f, 220.0f);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
	glUniform1i(texMode_uniformId, 1);

	// Render mesh
	glBindVertexArray(mesh[objId].vao);

	if (!shader.isProgramValid()) {
		printf("Program Not Valid!\n");
		exit(1);
	}
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	popMatrix(MODEL);
}


void render_flare(FLARE_DEF* flare, int lx, int ly, int* m_viewport) {  //lx, ly represent the projected position of light on viewport

	int     dx, dy;          // Screen coordinates of "destination"
	int     px, py;          // Screen coordinates of flare element
	int		cx, cy;
	float    maxflaredist, flaredist, flaremaxsize, flarescale, scaleDistance;
	int     width, height, alpha;    // Piece parameters;
	int     i;
	float	diffuse[4];

	GLint loc;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	objId = 83;

	int screenMaxCoordX = m_viewport[0] + m_viewport[2] - 1;
	int screenMaxCoordY = m_viewport[1] + m_viewport[3] - 1;

	//viewport center
	cx = m_viewport[0] + (int)(0.5f * (float)m_viewport[2]) - 1;
	cy = m_viewport[1] + (int)(0.5f * (float)m_viewport[3]) - 1;

	// Compute how far off-center the flare source is.
	maxflaredist = sqrt(cx * cx + cy * cy);
	flaredist = sqrt((lx - cx) * (lx - cx) + (ly - cy) * (ly - cy));
	scaleDistance = (maxflaredist - flaredist) / maxflaredist;
	flaremaxsize = (int)(m_viewport[2] * flare->fMaxSize);
	flarescale = (int)(m_viewport[2] * flare->fScale);

	// Destination is opposite side of centre from source
	dx = clampi(cx + (cx - lx), m_viewport[0], screenMaxCoordX);
	dy = clampi(cy + (cy - ly), m_viewport[1], screenMaxCoordY);

	// Render each element. To be used Texture Unit 0

	glUniform1i(texMode_uniformId, 5); // draw modulated textured particles 
	glUniform1i(tex_loc, 0);  //use TU 0

	for (i = 0; i < flare->nPieces; ++i)
	{
		// Position is interpolated along line between start and destination.
		px = (int)((1.0f - flare->element[i].fDistance) * lx + flare->element[i].fDistance * dx);
		py = (int)((1.0f - flare->element[i].fDistance) * ly + flare->element[i].fDistance * dy);
		px = clampi(px, m_viewport[0], screenMaxCoordX);
		py = clampi(py, m_viewport[1], screenMaxCoordY);

		// Piece size are 0 to 1; flare size is proportion of screen width; scale by flaredist/maxflaredist.
		width = (int)(scaleDistance * flarescale * flare->element[i].fSize);

		// Width gets clamped, to allows the off-axis flaresto keep a good size without letting the elements get big when centered.
		if (width > flaremaxsize)  width = flaremaxsize;

		height = (int)((float)m_viewport[3] / (float)m_viewport[2] * (float)width);
		memcpy(diffuse, flare->element[i].matDiffuse, 4 * sizeof(float));
		diffuse[3] *= scaleDistance;   //scale the alpha channel

		if (width > 1)
		{
			// send the material - diffuse color modulated with texture
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, diffuse);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, FlareTextureArray[flare->element[i].textureId]);
			pushMatrix(MODEL);
			translate(MODEL, (float)(px - width * 0.0f), (float)(py - height * 0.0f), 0.0f);
			scale(MODEL, (float)width, (float)height, 1);
			computeDerivedMatrix(PROJ_VIEW_MODEL);
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			glBindVertexArray(mesh[objId].vao);
			glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			popMatrix(MODEL);
		}
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void renderScene(void) {

	GLint loc;
	counter++;
	float deltaTime = 0;
	int time = glutGet(GLUT_ELAPSED_TIME);

	if (!game_pause) {
		deltaTime = (time - lastFrameTime) / 1000.0f;
	}
	lastFrameTime = time;


	FrameCount++;
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// load identity matrices
	loadIdentity(VIEW);
	loadIdentity(MODEL);


	float aux[4];
	float eyeLightPos[4];
	aux[0] = lightProperties[0].position.x;
	aux[1] = lightProperties[0].position.y;
	aux[2] = lightProperties[0].position.z;
	aux[3] = 1.0f;
	multMatrixPoint(VIEW, aux, eyeLightPos);


	// set the camera using a function similar to gluaAt
	setCameraLookAt();
	// use our shader
	glUseProgram(shader.getProgramIndex());

	//send the light position in eye coordinates

		//glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord 




	GLfloat plano_chao[4] = { 0,1,0,0 };
	float mat[16];

	loc = glGetUniformLocation(shader.getProgramIndex(), "enableFog");
	glUniform1i(loc, enableFog);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureArray[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextureArray[1]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TextureArray[2]);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, TextureArray[3]); //particle.bmp associated to TU0

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, TextureArray[4]); //tree

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, TextureArray[5]); //normal map


	//Indicar aos tres samplers do GLSL quais os Texture Units a serem usados
	glUniform1i(tex_loc, 0);
	glUniform1i(tex_loc1, 1);
	glUniform1i(tex_loc2, 2);
	glUniform1i(tex_loc3, 3);
	glUniform1i(tex_loc4, 4);
	glUniform1i(tex_normal, 5);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_STENCIL_TEST);        // Escrever 1 no stencil buffer onde se for desenhar a reflexão e a sombra
	glStencilFunc(GL_NEVER, 0x1, 0x1);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	// Fill stencil buffer with Ground shape; never rendered into color buffer
	drawMirror();

	glUniform1i(shadowMode_uniformId, 0);  //iluminação phong

	// Desenhar apenas onde o stencil buffer esta a 1
	glStencilFunc(GL_EQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	//REFLEXOES
	mirrorLight();
	sendLight();
	pushMatrix(MODEL);
	scale(MODEL, 1.0f, -1.0f, 1.0f);
	glCullFace(GL_FRONT);
	drawObjects();
	glCullFace(GL_BACK);
	popMatrix(MODEL);
	mirrorLight();
	sendLight();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		// Blend specular Ground with reflected geometry
	drawMirror();






	
	// Render the Shadows
	glUniform1i(shadowMode_uniformId, 1);  //Render with constant color
	float posit[4];
	posit[3] = 1.0f;
	for (int i = 0; i < 8; i++) {
		if (lightProperties[i].isEnabled == 1) {
			posit[0] = lightProperties[i].position.x;
			posit[1] = lightProperties[i].position.y;
			posit[2] = lightProperties[i].position.z;
			shadow_matrix(mat, plano_chao, posit);

			glDisable(GL_DEPTH_TEST); //To force the shadow geometry to be rendered even if behind the floor

			//Dark the color stored in color buffer
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
			glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);

			pushMatrix(MODEL);
			multMatrix(MODEL, mat);
			drawObjects();
			//drawTurtles();
			popMatrix(MODEL);
		}
	}

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glUniform1i(shadowMode_uniformId, 0);
	drawObjects();
	//sendLight();

	if (flareEffect) {
		int flarePos[2];
		int m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		if (!project(eyeLightPos, lightScreenPos, m_viewport))
			printf("Error in getting projected light in screen\n");  //Calculate the window Coordinates of the light position: the projected position of light on viewport
		flarePos[0] = clampi((int)lightScreenPos[0], m_viewport[0], m_viewport[0] + m_viewport[2] - 1);
		flarePos[1] = clampi((int)lightScreenPos[1], m_viewport[1], m_viewport[1] + m_viewport[3] - 1);

		//viewer looking down at  negative z direction
		pushMatrix(PROJECTION);
		loadIdentity(PROJECTION);
		pushMatrix(VIEW);
		loadIdentity(VIEW);
		ortho(m_viewport[0], m_viewport[0] + m_viewport[2] - 1, m_viewport[1], m_viewport[1] + m_viewport[3] - 1, -1, 1);
		render_flare(&AVTflare, flarePos[0], flarePos[1], m_viewport);
		popMatrix(PROJECTION);
		popMatrix(VIEW);
	}

	updateFrog2(deltaTime);
	updateLog(deltaTime);
	updateCar(deltaTime);
	updateTruck(deltaTime);
	updateTurtle(deltaTime);
	checkCollisions();
	glBindTexture(GL_TEXTURE_2D, 0);
	glutSwapBuffers();
}
// ------------------------------------------------------------
//
// Events from the Keyboard
//

void processKeys(unsigned char key, int xx, int yy)
{
	switch (key) {

	case 27:
		glutLeaveMainLoop();
		break;

	case 'c':
		printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
		break;
	case 'm':
		glEnable(GL_MULTISAMPLE);
		break;
	case '1':
		cameraId = 1;
		cam[0] = orthoProperties.posX;
		cam[1] = orthoProperties.posX;
		cam[2] = orthoProperties.posX;
		setCameraProjection();
		break;
	case '2':
		cameraId = 2;
		cam[0] = perspectiveProperties.posX;
		cam[1] = perspectiveProperties.posX;
		cam[2] = perspectiveProperties.posX;
		setCameraProjection();
		break;
	case '3':
		cameraId = 3;
		cam[0] = frog_x - 10;
		cam[1] = frog_y + 10;
		cam[2] = frog_z + 8;
		setCameraProjection();
		break;
	case 'n':
		glDisable(GL_MULTISAMPLE);
		break;
	case 'N':
		if (lightProperties[0].isEnabled == 0)
			lightProperties[0].isEnabled = 1;
		else
			lightProperties[0].isEnabled = 0;
		break;
	case 'H':
		if (lightProperties[1].isEnabled == 0)
			lightProperties[1].isEnabled = 1;
		else
			lightProperties[1].isEnabled = 0;
		break;
	case 'C':
		for (int i = 2; i < 8; i++) {
			if (lightProperties[i].isEnabled == 0)
				lightProperties[i].isEnabled = 1;
			else
				lightProperties[i].isEnabled = 0;
		}
		break;

	case 'R':
		restart();
		break;
	case 'r':
		restart();
		break;

		//frog movements
//backward    
	case 'S':
		if (!game_pause) {
			if (frog_x - 20.0 >= -110.0) {
				frog_x -= 20.0;
				lightProperties[1].position.x = frog_x + 8;
			}
		}
		break;
	case 's':
		if (!game_pause) {
			if (frog_x - 20.0 >= -110.0) {
				frog_x -= 20.0;
				lightProperties[1].position.x = frog_x + 8;
			}
		}
		break;
		//forward
	case 'W':
		if (!game_pause) {
			frog_x += 20.0;
			lightProperties[1].position.x = frog_x + 8;
		}
		break;
	case 'w':
		if (!game_pause) {
			frog_x += 20.0;
			lightProperties[1].position.x = frog_x + 8;
		}
		break;
		//right
	case 'D':
		if (!game_pause) {
			if (frog_z + 20.0 <= 110.0) {
				frog_z += 20.0;
				lightProperties[1].position.z = frog_z + 8;
			}
		}
		break;
	case 'd':
		if (!game_pause) {
			if (frog_z + 20.0 <= 110.0) {
				frog_z += 20.0;
				lightProperties[1].position.z = frog_z + 8;
			}
		}
		break;
		//left
	case 'A':
		if (!game_pause) {
			if (frog_z - 20.0 >= -110.0) {
				frog_z -= 20.0;
				lightProperties[1].position.z = frog_z + 8;
			}
		}
		break;
	case 'a':
		if (!game_pause) {
			if (frog_z - 20.0 >= -110.0) {
				frog_z -= 20.0;
				lightProperties[1].position.z = frog_z + 8;
			}
		}
		break;
	case 'P':
		if (game_pause && frog_lives > 0)
			game_pause = false;
		else
			game_pause = true;
		break;
	case 'p':
		if (game_pause && frog_lives > 0)
			game_pause = false;
		else
			game_pause = true;
		break;
	case 'f':
		enableFog = !enableFog;
		break;
	case 'F':
		enableFog = !enableFog;
		break;
	case 'l':
		flag = true;
		if (lightProperties[0].isEnabled == 0)
			flag = false;
		for (int i = 1; i < 8; i++) {
			if (lightProperties[i].isEnabled == 1)
				flag = false;
		}
		if (flag)
			flareEffect = !flareEffect;
		else
			flareEffect = false;
		break;
	}
}


// ------------------------------------------------------------
//
// Mouse Events
//

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{

	int deltaX, deltaY;
	float alphaAux = 0.0f, betaAux = 0.0f;
	float rAux = 0.0f;

	deltaX = -xx + startX;
	deltaY = yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}

	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux * sin(betaAux * 3.14f / 180.0f);

	//  uncomment this if not using an idle or refresh func
	//	glutPostRedisplay();
}


void mouseWheel(int wheel, int direction, int x, int y) {

	r += direction * 0.1f;
	if (r < 0.1f)
		r = 0.1f;

	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r * sin(beta * 3.14f / 180.0f);

	//  uncomment this if not using an idle or refresh func
	//	glutPostRedisplay();
}

// --------------------------------------------------------
//
// Shader Stuff
//


GLuint setupShaders() {

	// Shader for models
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/pointlight.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/pointlight.frag");

	// set semantics for the shader variables
	glBindFragDataLocation(shader.getProgramIndex(), 0, "colorOut");
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
	glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(shader.getProgramIndex());

	texMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "texMode"); // different modes of texturing
	shadowMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "shadowMode"); // different modes of texturing
	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");
	tex_loc = glGetUniformLocation(shader.getProgramIndex(), "texmap");
	tex_loc1 = glGetUniformLocation(shader.getProgramIndex(), "texmap1");
	tex_loc2 = glGetUniformLocation(shader.getProgramIndex(), "texmap2");
	tex_loc3 = glGetUniformLocation(shader.getProgramIndex(), "texmap3");
	tex_loc4 = glGetUniformLocation(shader.getProgramIndex(), "texmap4");
	tex_normal = glGetUniformLocation(shader.getProgramIndex(), "texnormal");

	printf("InfoLog for Per Fragment Phong Lightning Shader\n%s\n\n", shader.getAllInfoLogs().c_str());

	return(shader.isProgramLinked());
}

// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//

void init()
{



	lightProperties[0].isEnabled = 1;
	lightProperties[0].isLocal = 0;
	lightProperties[0].isSpot = 0;
	lightProperties[0].position = { 80.0f, 90.0f,80.0f };
	lightProperties[0].spotCosCutoff = 0.98f;
	lightProperties[0].coneDirection = { 0.0f, 1.0f, 0.0f };

	lightProperties[1].isEnabled = 0;
	lightProperties[1].isLocal = 1;
	lightProperties[1].isSpot = 1;
	lightProperties[1].position.x = frog_x + 8;
	lightProperties[1].position.y = frog_y + 3;
	lightProperties[1].position.z = frog_z + 8;
	lightProperties[1].spotCosCutoff = 0.98f;
	lightProperties[1].coneDirection = { 0.0f, 1.0f, 0.0f };

	lightProperties[2].isEnabled = 0;
	lightProperties[2].isLocal = 1;
	lightProperties[2].isSpot = 0;
	lightProperties[2].position = { -60.0f,10.0f, 60.0f };

	lightProperties[3].isEnabled = 0;
	lightProperties[3].isLocal = 1;
	lightProperties[3].isSpot = 0;
	lightProperties[3].position = { 60.0f, 20.0f, 60.0f };

	lightProperties[4].isEnabled = 0;
	lightProperties[4].isLocal = 1;
	lightProperties[4].isSpot = 0;
	lightProperties[4].position = { 0.0f, 20.0f, 60.0f };

	lightProperties[5].isEnabled = 0;
	lightProperties[5].isLocal = 1;
	lightProperties[5].isSpot = 0;
	lightProperties[5].position = { 0.0f, 20.0f, -60.0f };

	lightProperties[6].isEnabled = 0;
	lightProperties[6].isLocal = 1;
	lightProperties[6].isSpot = 0;
	lightProperties[6].position = { 60.0f, 20.0f, -60.0f };

	lightProperties[7].isEnabled = 0;
	lightProperties[7].isLocal = 1;
	lightProperties[7].isSpot = 0;
	lightProperties[7].position = { -60.0f, 20.0f, -60.0f };



	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r * sin(beta * 3.14f / 180.0f);

	const char* stone = "stone.tga";
	const char* asphalt = "Asphalt-1.tga";
	const char* river = "river.tga";

	glGenTextures(6, TextureArray);
	TGA_Texture(TextureArray, stone, 0);
	TGA_Texture(TextureArray, asphalt, 1);
	TGA_Texture(TextureArray, river, 2);
	TGA_Texture(TextureArray, "particle.tga", 3);
	TGA_Texture(TextureArray, "tree.tga", 4);
	TGA_Texture(TextureArray, "normal.tga", 5);

	
	glGenTextures(5, FlareTextureArray);
	TGA_Texture(FlareTextureArray, "crcl.tga", 0);
	TGA_Texture(FlareTextureArray, "flar.tga", 1);
	TGA_Texture(FlareTextureArray, "hxgn.tga", 2);
	TGA_Texture(FlareTextureArray, "ring.tga", 3);
	TGA_Texture(FlareTextureArray, "sun.tga", 4);


	float amb[] = { 0.0f, 0.3f, 0.0f, 1.0f };
	float diff[] = { 0.1f, 0.8f, 0.1f, 0.3f };
	float spec[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 20.0f;
	int texcount = 0;

	// create geometry and VAO of the pawn
	objId = 0;
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();

	objId = 2;
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();

	objId = 4;
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();

	float amb1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diff1[] = { 0.3f, 0.3f, 0.3f, 0.3f };
	float spec1[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	shininess = 5.0;

	objId = 1;
	memcpy(mesh[objId].mat.ambient, amb1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();

	float amb2[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float diff2[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	float spec2[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	shininess = 500.0;

	objId = 3;
	memcpy(mesh[objId].mat.ambient, amb2, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff2, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec2, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = 100;
	mesh[objId].mat.texCount = texcount;
	createCube();


	float amb3[] = { 0.5f, 0.3f, 0.1f, 1.0f };
	float diff3[] = { 0.4f, 0.2f, 0.1f, 1.0f };
	float spec3[] = { 0.6f, 0.4f, 0.2f, 1.0f };
	shininess = 5.0;

	//logs
	objId = 5;
	memcpy(mesh[objId].mat.ambient, amb3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCylinder(40, 8, 25);

	objId = 6;
	memcpy(mesh[objId].mat.ambient, amb3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCylinder(40, 8, 25);

	objId = 7;
	memcpy(mesh[objId].mat.ambient, amb3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCylinder(40, 8, 25);

	objId = 8;
	memcpy(mesh[objId].mat.ambient, amb3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCylinder(70, 8, 25);

	objId = 9;
	memcpy(mesh[objId].mat.ambient, amb3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCylinder(70, 8, 25);

	float amb4[] = { 0.8f, 0.8f, 0.2f, 1.0f };
	float diff4[] = { 0.1f, 0.1f, 0.0f, 1.0f };
	float spec4[] = { 0.2f, 0.2f, 0.0f, 1.0f };
	shininess = 100.0;

	//createFrog
	objId = 10;
	memcpy(mesh[objId].mat.ambient, amb4, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff4, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec4, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();

	float amb5[] = { 0.5f, 0.0f, 0.5f, 1.0f };
	float diff5[] = { 0.1f, 0.1f, 0.0f, 1.0f };
	float spec5[] = { 0.2f, 0.2f, 0.0f, 1.0f };
	shininess = 100.0;

	objId = 11;
	memcpy(mesh[objId].mat.ambient, amb5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createSphere(2, 10);

	objId = 12;
	memcpy(mesh[objId].mat.ambient, amb5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createSphere(2, 10);

	//CAR
	float amb6[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float diff6[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	float spec6[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	shininess = 100.0;

	float amb7[] = { 0.8f, 0.1f, 0.1f, 1.0f };
	float diff7[] = { 0.1f, 0.1f, 0.0f, 1.0f };
	float spec7[] = { 0.2f, 0.2f, 0.0f, 1.0f };
	//objID 13-37
	for (int i = 0; i < 5; i++) {
		//BODY
		objId++;
		memcpy(mesh[objId].mat.ambient, amb6, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff6, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec6, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createCube();

		//WHEELS
		objId++;
		memcpy(mesh[objId].mat.ambient, amb7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createTorus(1.0f, 3.0f, 100, 100);

		//WHEELS
		objId++;
		memcpy(mesh[objId].mat.ambient, amb7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createTorus(1.0f, 3.0f, 100, 100);

		//WHEELS
		objId++;
		memcpy(mesh[objId].mat.ambient, amb7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createTorus(1.0f, 3.0f, 100, 100);

		//WHEELS
		objId++;
		memcpy(mesh[objId].mat.ambient, amb7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createTorus(1.0f, 3.0f, 100, 100);
	}

	//TRUCK
	float amb8[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float diff8[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	float spec8[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	shininess = 100.0;
	//objid 38-62
	for (int i = 0; i < 5; i++) {
		//BODY
		objId++;
		memcpy(mesh[objId].mat.ambient, amb8, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff8, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec8, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createCube();

		//WHEELS
		objId++;
		memcpy(mesh[objId].mat.ambient, amb7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createTorus(1.0f, 3.0f, 100, 100);

		//WHEELS
		objId++;
		memcpy(mesh[objId].mat.ambient, amb7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createTorus(1.0f, 3.0f, 100, 100);

		//WHEELS
		objId++;
		memcpy(mesh[objId].mat.ambient, amb7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createTorus(1.0f, 3.0f, 100, 100);

		//WHEELS
		objId++;
		memcpy(mesh[objId].mat.ambient, amb7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec7, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createTorus(1.0f, 3.0f, 100, 100);
	}


	float diff9[] = { 0.1f, 0.1f, 0.99f, 1.0f };
	float spec9[] = { 0.3f, 0.1f, 0.1f, 1.0f };
	float ambiente[] = { 0.0f,0.0f,0.3f,1.0f };
	//TURTLE
	shininess = 5;
	//objid 63-77
	for (int i = 0; i < 5; i++) {
		//BODY1
		objId++;
		memcpy(mesh[objId].mat.ambient, ambiente, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff9, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec9, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createSphere(6, 10);

		//BODY2
		objId++;
		memcpy(mesh[objId].mat.ambient, ambiente, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff9, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec9, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createSphere(6, 10);

		//BODY3
		objId++;
		memcpy(mesh[objId].mat.ambient, ambiente, 4 * sizeof(float));
		memcpy(mesh[objId].mat.diffuse, diff9, 4 * sizeof(float));
		memcpy(mesh[objId].mat.specular, spec9, 4 * sizeof(float));
		memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
		mesh[objId].mat.shininess = shininess;
		mesh[objId].mat.texCount = texcount;
		createSphere(6, 10);
	}

	//Frog2
	float ambF[] = { 0.8f, 0.8f, 0.5f, 1.0f };
	float diffF[] = { 0.1f, 0.3f, 0.0f, 1.0f };
	float specF[] = { 0.2f, 0.2f, 0.1f, 1.0f };
	objId++;
	memcpy(mesh[objId].mat.ambient, ambF, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diffF, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, specF, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();

	objId++;
	memcpy(mesh[objId].mat.ambient, amb5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createSphere(2, 10);

	objId++;
	memcpy(mesh[objId].mat.ambient, amb5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createSphere(2, 10);



	float tree_spec[] = { 0.6f, 0.3f, 0.3f, 1.0f };
	float tree_shininess = 10.0f;
	//objid 78
	objId++;
	memcpy(mesh[objId].mat.specular, tree_spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = tree_shininess;
	mesh[objId].mat.texCount = texcount;
	createQuad(6, 6);


	//objid 82
	objId++;
	mesh[objId].mat.texCount = texcount;
	createQuad(2, 2);

	//objId = 83
	objId++;
	createQuad(1, 1);

	//Load flare from file
	loadFlareFile(&AVTflare, "flare.txt");


	//create

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}
// ------------------------------------------------------------
//
// Main function
//


int main(int argc, char** argv) {

	//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WinX, WinY);
	WindowHandle = glutCreateWindow(CAPTION);

	initializeCameras();

	//  Callback Registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutTimerFunc(0, timer, 0);
	//glutIdleFunc(renderScene);  // Use it for maximum performance
	glutTimerFunc(0, refresh, 0);    //use it to to get 60 FPS whatever

//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc(mouseWheel);



	//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (!setupShaders())
		return(1);

	init();

	//  GLUT main loop
	glutMainLoop();

	return(0);

}

unsigned int getTextureId(char* name) {
	int i;

	for (i = 0; i < NTEXTURES; ++i)
	{
		if (strncmp(name, flareTextureNames[i], strlen(name)) == 0)
			return i;
	}
	return -1;
}
void loadFlareFile(FLARE_DEF* flare, const char* filename)
{
	int     n = 0;
	FILE* f;
	char    buf[256];
	int fields;

	memset(flare, 0, sizeof(FLARE_DEF));

	f = fopen(filename, "r");
	if (f)
	{
		fgets(buf, sizeof(buf), f);
		sscanf(buf, "%f %f", &flare->fScale, &flare->fMaxSize);

		while (!feof(f))
		{
			char            name[8] = { '\0', };
			double          dDist = 0.0, dSize = 0.0;
			float			color[4];
			int				id;

			fgets(buf, sizeof(buf), f);
			fields = sscanf(buf, "%4s %lf %lf ( %f %f %f %f )", name, &dDist, &dSize, &color[3], &color[0], &color[1], &color[2]);
			if (fields == 7)
			{
				for (int i = 0; i < 4; ++i) color[i] = clamp(color[i] / 255.0f, 0.0f, 1.0f);
				id = getTextureId(name);
				if (id < 0) printf("Texture name not recognized\n");
				else
					flare->element[n].textureId = id;
				flare->element[n].fDistance = (float)dDist;
				flare->element[n].fSize = (float)dSize;
				memcpy(flare->element[n].matDiffuse, color, 4 * sizeof(float));
				++n;
			}
		}

		flare->nPieces = n;
		fclose(f);
	}
	else printf("Flare file opening error\n");
}

