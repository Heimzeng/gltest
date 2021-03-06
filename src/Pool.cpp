#include <vector>
#include "Pool.h"

void Pool::initialize(int oNumX, int oNumZ, float height,
                      float oDistance, float oWeight,
                      float damping, float splash,
                      float texRepeatX, float texRepeatZ,
                      std::unique_ptr<Texture> floorTexture) {
    // initializes the members
    this->oNumX = oNumX;
    this->oNumZ = oNumZ;
    this->height = height;
    this->oNum = oNumX * oNumZ;
    this->oDistance = oDistance;
    this->oWeight = oWeight;
    this->damping = damping;
    this->splash = splash;
    this->floorTexture = std::move(floorTexture);

    oscillators.clear();
    oscillators.resize(oNum);

    for (int i = 0; i < oNumX; i++) {
        for (int j = 0; j < oNumZ; j++) {
            int idx = i + j * oNumX;
            // positions
            oscillators[idx].x = oDistance * float(i);
            oscillators[idx].y = 0.0f;  // on the plane initially
            oscillators[idx].z = oDistance * float(j);

            // normals. pointing up initially
            oscillators[idx].nx = 0.0f;
            oscillators[idx].ny = 1.0f;
            oscillators[idx].nz = 0.0f;

            // texture coordinates
            oscillators[idx].texX = (float)i / (float)oNumX * texRepeatX;
            oscillators[idx].texY = 1.0f - (float)j / (float)oNumZ * texRepeatZ;

            // initial speed
            oscillators[idx].speedY = 0;

            // create a peek for it. that's two triangles
            if ((i < oNumX - 1) && (j < oNumZ - 1)) {
                indices.push_back(i + j * oNumX);
                indices.push_back((i + 1) + j * oNumX);
                indices.push_back((i + 1) + (j + 1) * oNumX);

                indices.push_back(i + j * oNumX);
                indices.push_back((i + 1) + (j + 1) * oNumX);
                indices.push_back(i + (j + 1) * oNumX);
            }

        }
    }

    indicesNum = indices.size();
}

void Pool::reset() {
    for (int i = 0; i < oNumX; i++) {
        for (int j = 0; j < oNumZ; j++) {
            int idx = i + j * oNumX;
            // normal points up
            oscillators[idx].nx = 0.0f;
            oscillators[idx].ny = 1.0f;
            oscillators[idx].nz = 0.0f;
            // back to the plane
            oscillators[idx].y = 0.0f;
            // initial speed
            oscillators[idx].speedY = 0.0f;
        }
    }
}

void Pool::splashOscillator(int posX, int posZ) {
    // if in the range.
    // this is needed by fountains with drops out of range
    if ((posX >= 0) && (posX < oNumX) && (posZ >= 0) && (posZ < oNumZ)) {
        int idx = posX + posZ * oNumX;
        if (oscillators[idx].y > -height)
            oscillators[idx].y += splash;
    }
}

void Pool::update(float deltaTime) {
    for (int i = 0; i < oNumX; i++) {
        for (int j = 0; j < oNumZ; j++) {
            int idx = i + j * oNumX;
            // 储存y方向的位置
            oscillators[idx].newY = oscillators[idx].y;

            if ((i == 0) || (i == oNumX - 1) || (j == 0) || (j == oNumZ - 1)) {
				
            } else {
				// 计算平均下降，保持一个平衡
                float avgdiff = oscillators[idx - 1].y   //left
                    + oscillators[idx + 1].y // right
                    + oscillators[idx - oNumX].y //upper
                    + oscillators[idx + oNumX].y  // lower
                    - 4 * oscillators[idx].y;  // subtract itself all at one

                oscillators[idx].speedY += avgdiff * (deltaTime / oWeight);
                oscillators[idx].speedY *= (1.0f - damping);
				
                oscillators[idx].newY += oscillators[idx].speedY * deltaTime;
            }
        }
    }
	// 从缓存更新到真正的位置
    for (int i = 0; i < oNumX; i++) {
        for (int j = 0; j < oNumZ; j++) {
            int idx = i + j * oNumX;
            oscillators[idx].y = oscillators[idx].newY;
        }
    }

	
    // update normals using the newly positioned neighbors
    for (int i = 0; i < oNumX; i++) {
        for (int j = 0; j < oNumZ; j++) {
            // the new normal is orthogonal to
            // 1. the vector from the left to the right neighbor
            // 2. the vector from the upper to the lower neighbor
            int idx = i + j * oNumX,
                ileft = i - 1 + j * oNumX, iright = i + 1 + j * oNumX,
                iup = i + (j + 1) * oNumX, idown = i + (j - 1) * oNumX;
            glm::vec3 p1, p2; // store the points for calculating u and v
                             // needed because of the boundaries.
			glm::vec3 u, v;  // direction vectors
            int ip1 = idx, ip2 = idx;  // indices for p1 and p2

            // calculate left-to-right direction vector
            ip1 = i > 0 ? ileft : idx;
            ip2 = i < oNumX - 1 ? iright : idx;
            p1 = glm::vec3(oscillators[ip1].x,
                   oscillators[ip1].y,
                   oscillators[ip1].z);
            p2 = glm::vec3(oscillators[ip2].x,
                   oscillators[ip2].y,
                   oscillators[ip2].z);
            u = p2 - p1;

            // calculate upper-to-lower direction vector
            ip1 = j > 0 ? idown : idx;
            ip2 = j < oNumZ - 1 ? iup : idx;
            p1 = glm::vec3(oscillators[ip1].x,
                   oscillators[ip1].y,
                   oscillators[ip1].z);
            p2 = glm::vec3(oscillators[ip2].x,
                   oscillators[ip2].y,
                   oscillators[ip2].z);
            v = p2 - p1;

            // cross product to get the orthogonal vector
			glm::vec3 normal =glm::normalize(glm::cross(u, v));
            // the normal should always points up
            float sign = normal.y > 0.0 ? 1.0f : -1.0f;
            oscillators[idx].nx = normal.x * sign;
            oscillators[idx].ny = normal.y * sign;
            oscillators[idx].nz = normal.z * sign;
        }
    }
}



void Pool::render() const {
    // set up pointers
    glVertexPointer(3, GL_FLOAT, sizeof(Oscillator), oscillators.data());
    glTexCoordPointer(2, GL_FLOAT, sizeof(Oscillator), &(oscillators.data()[0].texX));
    glNormalPointer(GL_FLOAT, sizeof(Oscillator), &(oscillators.data()[0].nx));

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // draw
    glPushMatrix();
    glTranslatef(4.0f, height - 3.6f, -8.0f);
    floorTexture->bind();
    // fill it. NOTE: this will affect the whole scene!!!
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glDrawElements(GL_TRIANGLES, indicesNum,  GL_UNSIGNED_INT, indices.data());
    glPopMatrix();
}
