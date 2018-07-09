#ifndef PARTICLES_H
#define PARTICLES_H

class Particles {
public:
	void setAlive(bool al) { alive = al; }
	void setLife(float li) { life = li; }
	void setFade(float fa) { fade = fa; }

	void setRGB(float r, float g, float b) {
		red = r;
		green = g;
		blue = b;
	}

	void setPostion(float x, float y, float z) {
		xpos = x;
		ypos = y;
		zpos = z;
	}

	void setVel(float vel) {
		vel = vel;
	}

	void setGra(float gra) { gravity = gra; }

	bool getAlive() { return alive; }
	float getLife() { return life; }
	float getfade() { return fade; }

	float getRed() { return red; }
	float getGreen() { return green; }
	float getBlue() { return blue; }

	float getX() { return xpos; }
	float getY() { return ypos; }
	float getZ() { return zpos; }

	float getVel() { return vel; }
	float getGra() { return gravity; }

private:
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
}; 

#endif