#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h>
#include <FreeImage.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif
#ifndef GL_BGR_EXT
#define GL_BGR_EXT 0x80E0
#endif

class Texture {
public:
	Texture(const char *filename, int repeat = GL_REPEAT,
		int type = JPEG_DEFAULT);
    int getWidth() const;
    int getHeight() const;
    unsigned int getID() const;
    // bind to the context
    void bind();
private:
    int width, height;
    unsigned int ID;
};

#endif
