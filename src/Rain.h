#ifndef RAIN_H
#define RAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define GLUT_DISABLE_ATEXIT_HACK 
#define GLUT_DISABLE_ATEXIT_HACK 
#include <GL\glut.h>
#include <GL/gl.h>
#include <glm\glm.hpp>
#include <vector>
#include "Particles.h"



typedef struct {
	// Life
	bool alive;	// is the particle alive?
	float life;	// particle lifespan
	float fade; // decay
				// color
	float red;
	float green;
	float blue;
	// Position/direction
	float xpos;
	float ypos;
	float zpos;
	// Velocity/Direction, only goes down in y dir
	float vel;
	// Gravity
	float gravity;
}particles;


class Rain
{
public:
	Rain(int MAX_PARTICLES = 1000) {
		max_particles = MAX_PARTICLES;
		particles p;
		for (int i = 0; i < MAX_PARTICLES; i++)
			par_sys.push_back(p);
	}

	void initialize(int i, glm::vec3 camPos, glm::vec3 camDir);

	void setSlow(float sd) { slowdown = sd; }
	void setVel(float vel) { velocity = vel; }

	//render the rain
	void render(float zoom, glm::vec3 camPos, glm::vec3 camDir);


private:
	int max_particles;
	std::vector<particles> par_sys;
	float slowdown = 2.0;
	float velocity = 0.0;


};


#endif