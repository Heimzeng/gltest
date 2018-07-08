#ifndef POOL_H
#define POOL_H

#include <memory>
#include <vector>
#include <GL/glut.h>

#include "Texture.h"

//glm
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

struct Oscillator {
    float x, y, z;  // position
    float nx, ny, nz;  // normal
    float texX, texY;  // texture coordinates
    float speedY;  // speed along the y direction
    float newY;  // for storing calculated y temperorily
};

class Pool {
public:
    // default
    Pool() {}
    // explicit arguments
    Pool(int oNumX, int oNumZ, float height,
         float oDistance, float oWeight, float damping, float splash,
         float texRepeatX, float texRepeatZ,
         std::unique_ptr<Texture> floorTexture) {
        initialize(oNumX, oNumZ, height,
                   oDistance, oWeight, damping, splash,
                   texRepeatX, texRepeatZ,
                   std::move(floorTexture));
    }

    // explicit arguments
    void initialize(int oNumX, int oNumZ, float height,
                    float oDistance, float oWeight,
                    float damping, float splash,
                    float texRepeatX, float texRepeatZ,
                    std::unique_ptr<Texture> floorTexture);

    // get the distance of the oscillators
    float getODistance() const {
        return oDistance;
    }

    // render the pool
    void render() const;

    // update the oscillator at a position
    // this should be caused by a drop
    void splashOscillator(int idxX, int idxZ);
    void update(float deltaTime);
    void reset();

private:
    std::vector<Oscillator> oscillators;  
    std::vector<int> indices;  
    std::unique_ptr<Texture> floorTexture; 

    float oDistance; 
    float oWeight; 
    int oNumX, oNumZ; 
    int oNum;  // sizeX * sizeZ
    float splash, damping;
    float height;  
    int indicesNum;  
};

#endif
