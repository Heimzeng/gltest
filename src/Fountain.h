#ifndef FOUNTAIN_H
#define FOUNTAIN_H

#define DEGREE 3.1415926 / 180

#include <vector>
#include "Pool.h"

class Fountain;

class Drop {
public:
    void setSpeed(glm::vec3 newSpeed);
    void setAcceleration(float newAcc);
    void setTime(float newTime);
    void updatePosition(glm::vec3 & position, float dtime,
                        Pool & pool, Fountain & fountain);
private:
    float time;  // time after the drop take off
	glm::vec3 speed;
    float acceleration;
};

class Fountain {
public:
    Fountain() {}
    Fountain(int levels, int raysPerStep, int dropsPerRay,
             float dropSize, float angleMin, float angleMax,
             float randomAngle, float acceleration){
        initialize(levels, raysPerStep, dropsPerRay,
                   dropSize, angleMin, angleMax,
                   randomAngle, acceleration);
    }

    void initialize(int levels, int raysPerStep, int dropsPerRay,
                    float dropSize, float angleMin, float angleMax,
                    float randomAngle, float acceleration);

    void render() const;
    void update(float dtime, Pool & pool);

	glm::vec3 center;  // center of this fountain
private:
    int numDrops;  // number of drops
    float dropSize;  // size of drops
    std::vector<glm::vec3> dropPositions;  // positions of the drops this fountain emits
    std::vector<Drop> drops;  // drop info(speed, accelerations, etc.)
};

#endif
