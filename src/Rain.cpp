#include "Rain.h"

void Rain::initialize(int i, glm::vec3 camPos, glm::vec3 camDir) {
	par_sys[i].alive = true;
	par_sys[i].life = 1.0;
	par_sys[i].fade = float(rand() % 100) / 1000.0f + 0.003f;

	par_sys[i].xpos = (float)((2 * rand() - 1) % 20) - camPos.x + (camDir.x - 0.3f) * 20;
	par_sys[i].ypos = (float)((2 * rand() - 1) % 20) + camPos.y + camDir.y * 10;
	par_sys[i].zpos = (float)((2 * rand() - 1) % 20) + camPos.z + camDir.z * 10;

	par_sys[i].red = 0.5;
	par_sys[i].green = 0.5;
	par_sys[i].blue = 1.0;

	par_sys[i].vel = velocity;
	par_sys[i].gravity = -0.8;
}

//render the rain
void Rain::render(float zoom, glm::vec3 camPos, glm::vec3 camDir) {
	float x, y, z;
	for (int i = 0; i < max_particles; i = i + 2) {
		if (par_sys[i].alive == true) {
			x = par_sys[i].xpos;
			y = par_sys[i].ypos;
			z = par_sys[i].zpos + zoom;


			// Draw particles
			glColor4f(0.9, 0.9, 0.9, 0.4);
			//      glColor3f(0.5, 0.5, 1.0);
			glBegin(GL_LINES);
				glVertex3f(x, y, z);
				glVertex3f(x, y + 0.2, z);
			glEnd();

			// Update values
			//Move
			// Adjust slowdown for speed!
			par_sys[i].ypos += par_sys[i].vel / (slowdown * 10);
			par_sys[i].vel += par_sys[i].gravity;
			// Decay
			par_sys[i].life -= par_sys[i].fade;

			if (par_sys[i].ypos <= -5) {
				par_sys[i].life = -1.0;
			}
			//Revive
			if (par_sys[i].life < 0.0) {
				initialize(i,camPos, camDir);
			}
		}
	}
}
