#include <GL/glut.h>
#include <memory>
#include <type_traits>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>

#include "Texture.h"
#include "Camera.h"
#include "Basin.h"
#include "SkyBox.h"
#include "Ground.h"
#include "Pool.h"
#include "Fountain.h"

// Sky & ground 
const float SKY_BOX_SIZE = 300.0f;
const float GROUND_SIZE = 300.0f;
const float GROUND_TEX_REPEAT = 40.0f;

// Pool Configuration
const int OSCILLATORS_NUM_X = 110;
const int OSCILLATORS_NUM_Z = 110;
const float OSCILLATOR_DISTANCE = 0.04f;
const float OSCILLATOR_WEIGHT = 0.0002f;
const float OSCILLATOR_SPLASH = -0.015f;
const float OSCILLATOR_DAMPING = 0.005f;
const float POOL_HEIGHT = 0.3f;
const float POOL_TEX_REPEAT_X = 3.0f;
const float POOL_TEX_REPEAT_Z = 3.0f;

// Basin Configuration
const float BASIN_BORDER_WIDTH = 0.4f;
const float BASIN_BORDER_HEIGHT = 0.2f;
const float BASIN_INNER_X = (OSCILLATORS_NUM_X * OSCILLATOR_DISTANCE);
const float BASIN_INNER_Z = (OSCILLATORS_NUM_Z * OSCILLATOR_DISTANCE);

// Fountain Configuration
const float DROP_SIZE = 4.0f;

const float WATER_COLOR[] = { 0.9f, 0.9f, 0.9f, 0.4f };
const float TIME_DELTA = 0.004f;
const int FPS = 50;

// Lighting configuration
const float LIGHT_AMBIENT_1[] = { 0.1f, 0.1f, 0.1f, 0.0f };
const float LIGHT_DIFFUSE_1[] = {
    211.0f / 255.0f, 183.0f / 255.0f, 133.0f / 255.0f, 0.0f
};
const float LIGHT_POSITION_1[] = { 0.8f, 0.4f, -0.5f, 0.0f };

const float LIGHT_AMBIENT_2[] = { 0.2f, 0.2f, 0.2f, 0.0f };
const float LIGHT_DIFFUSE_2[] = {
    211.0f / 255.0f, 183.0f / 255.0f, 133.0f / 255.0f, 0.0f
};
const float LIGHT_POSITION_2[] = { 0.8f, -0.2f, -0.5f, 0.0f };



// Camera Configuration
const float MOVE_SPEED = 0.2f;
const float ROTATE_SPEED = 1.0f;
const float ROTATE_FACTOR = 0.25f;
const float CAMERA_POSITION[] = {
    BASIN_INNER_X / 2.0f, 1.8f, BASIN_INNER_Z + 3.5f
};
const float CAMERA_ROTATION[] = {
    -5.0f, 0.0f, 0.0f
};


// Viewport & Window Configuration
const double FIELD_OF_VIEW = 45.0;
const double CLIP_NEAR = 1.0;
const double CLIP_FAR = 10000.0;
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;
int windowWidth = WINDOW_WIDTH;
int windowHeight = WINDOW_HEIGHT;

const float INFO_POSITION_Z = 0.0f;
const float FPS_INFO_POSITION[] = { 0.02f, 0.05f, INFO_POSITION_Z };
const float CAMPOS_INFO_POSITION[] = { 0.02f, 0.85f, INFO_POSITION_Z };
const float CAMROT_INFO_POSITION[] = { 0.02f, 0.90f, INFO_POSITION_Z };
const float CAMDIR_INFO_POSITION[] = { 0.02f, 0.95f, INFO_POSITION_Z };
const float CONTROL_INFO_POSITION[] = { 0.82f, 0.95f, INFO_POSITION_Z };
void *INFO_FONT = GLUT_BITMAP_TIMES_ROMAN_24;


// Objects in the scene
Camera camera;

// Water and the floor in the basin
Pool pool;

// Water in the air
Fountain fountain;
Fountain fountains[5];

// Basin of the fountain
Basin basin;

// Sky
Skybox skybox;

// The ground
Ground ground;

bool isFullScreen = false;
bool mouseControl = false;

int mouseX = 0, mouseY = 0;
void mouseMove(int x, int y) {
    if (mouseControl) {
        camera.rotateX((mouseY - y) * ROTATE_FACTOR);
        camera.rotateY((mouseX - x) * ROTATE_FACTOR);
    }
    mouseX = x;
    mouseY = y;
}

void keyDown(unsigned char key, int x, int y) {
    switch (key) {
    case 27:  // ESC
        exit(0);
        break;
    case 'f':
    case 'F': // full screen
        if (!isFullScreen) {
            glutFullScreen();
            isFullScreen = true;
            glutWarpPointer(mouseX, mouseY);
        } else {
            glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
            isFullScreen = false;
            glutWarpPointer(mouseX, mouseY);
        }
        break;
    case 'c':
    case 'C':
        mouseControl = !mouseControl;
        break;
    }

     // Camera controls
    switch (key) {
        case 'a':
        case 'A':
            camera.moveX(-MOVE_SPEED);
            break;
        case 'd':
        case 'D':
            camera.moveX(MOVE_SPEED);
            break;
        case 's':
        case 'S':
            camera.moveZ(MOVE_SPEED);
            break;
        case 'w':
        case 'W':
            camera.moveZ(-MOVE_SPEED);
            break;
		case 'q':
		case 'Q':
			camera.moveY(MOVE_SPEED);
			break;
		case 'z':
		case 'Z':
			camera.moveY(-MOVE_SPEED);
			break;
    }
}

void drawScene(void) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glEnable(GL_LIGHTING);
    // set up the scene
    pool.render();
    basin.render();
    ground.render();
    // sky
    glDisable(GL_LIGHTING);
    skybox.render();

    // water in the air
    glDisable(GL_TEXTURE_2D);
    glColor4fv(WATER_COLOR);
    fountain.render();
	for (int i = 0; i < 4; i++) {
		fountains[i].render();
	}
    glEnable(GL_LIGHTING);

    glDisable(GL_BLEND);
}

void renderBitmapString(const float position[3],
                        void *font, const char *string) {
    const char *c;
    glRasterPos3fv(position);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void vecToString(char *buffer, const char *format, glm::vec3 vec) {
    sprintf(buffer, format, vec.x, vec.y, vec.z);
}

int lastTime = 0;
int thisTime = 0;
void showText() {
    char frameInfo[30], positionInfo[50],
        rotationInfo[50], directionInfo[50];

    sprintf(frameInfo, "fps: %4.2f", 1000.0 / (thisTime - lastTime));
    camera.updateDirection();
    vecToString(positionInfo,
                "Camera position %3.2f, %3.2f, %3.2f", camera.getPosition());
    vecToString(rotationInfo,
                "Camera rotation %3.2f, %3.2f, %3.2f", camera.getRotation());
    vecToString(directionInfo,
                "Camera direction %3.2f, %3.2f, %3.2f", camera.getDirection());

    // draw status text
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    // render the string
    renderBitmapString(FPS_INFO_POSITION, INFO_FONT, frameInfo);
    renderBitmapString(CAMPOS_INFO_POSITION, INFO_FONT, positionInfo);
    renderBitmapString(CAMROT_INFO_POSITION, INFO_FONT, rotationInfo);
    renderBitmapString(CAMDIR_INFO_POSITION, INFO_FONT, directionInfo);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camera.render();

    glLightfv(GL_LIGHT1, GL_POSITION, LIGHT_POSITION_1);
    glLightfv(GL_LIGHT2, GL_POSITION, LIGHT_POSITION_2);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    drawScene();
    showText();
    glutSwapBuffers();
}

void idle(void) {
    thisTime = glutGet(GLUT_ELAPSED_TIME);
    if (thisTime - lastTime > 1000 / FPS) {
        // update the fountain and the pool
        fountain.update(TIME_DELTA, pool);
		for (int i = 0; i < 4; i++) {
			fountains[i].update(TIME_DELTA, pool);
		}
        pool.update(TIME_DELTA);

        //render the scene:
        display();
        lastTime = thisTime;
    }
}

void reshape(int x, int y) {
    windowWidth = x;
    windowHeight = y;
    if (y == 0 || x == 0) return;  // invisible

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FIELD_OF_VIEW, (GLdouble)x / (GLdouble)y,
                   CLIP_NEAR, CLIP_FAR);
    glViewport(0, 0, x, y);
    glMatrixMode(GL_MODELVIEW);
}

void mouseButton(int button, int state, int x, int y) {
    // scroll up
    if (button == 3) {
        camera.moveZ(-MOVE_SPEED);
    // scroll down
    } else if (button == 4) {
        camera.moveZ(MOVE_SPEED);
    }
}

int main(int argc, char **argv) {
    //static_assert(std::is_pod<glm::vec3>::value, "FVector must be a POD!");
    static_assert(std::is_pod<Oscillator>::value, "Oscillator must be a POD!");

    // initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Fountain");

    // textures
    std::unique_ptr<Texture> pebbleTexture(new Texture);
    std::unique_ptr<Texture> basinTexture(new Texture);
    std::unique_ptr<Texture> groundTexture(new Texture);
    std::unique_ptr<Texture[]> skyTextures(new Texture[SKY_BOX_FACES]);

    pebbleTexture->load("resource/pebbles.bmp");
    basinTexture->load("resource/wall.bmp");
    groundTexture->load("resource/grass.bmp");

    skyTextures[SKY_FRONT].load("resource/skybox/front.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_RIGHT].load("resource/skybox/right.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_LEFT].load("resource/skybox/left.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_BACK].load("resource/skybox/back.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_UP].load("resource/skybox/up.bmp", GL_CLAMP_TO_EDGE);
    skyTextures[SKY_DOWN].load("resource/skybox/down.bmp", GL_CLAMP_TO_EDGE);

    // initialize the scene
    skybox.initialize(-SKY_BOX_SIZE, SKY_BOX_SIZE,
                      -SKY_BOX_SIZE, SKY_BOX_SIZE,
                      -SKY_BOX_SIZE, SKY_BOX_SIZE, std::move(skyTextures));

    pool.initialize(OSCILLATORS_NUM_X, OSCILLATORS_NUM_Z, POOL_HEIGHT,
                    OSCILLATOR_DISTANCE, OSCILLATOR_WEIGHT,
                    OSCILLATOR_DAMPING, OSCILLATOR_SPLASH,
                    POOL_TEX_REPEAT_X, POOL_TEX_REPEAT_Z,
                    std::move(pebbleTexture));

    fountain.initialize(4, 30, 30, DROP_SIZE, 75.0f, 90.0f, 0.2f, 0.10f);
	for (int i = 0; i < 4; i++) {
		fountains[i].initialize(6, 20, 30, DROP_SIZE, 90.0f, 90.0f, 1.0f, 0.12f);
	}


    basin.initialize(BASIN_BORDER_HEIGHT + POOL_HEIGHT, BASIN_BORDER_WIDTH,
                     BASIN_INNER_X, BASIN_INNER_Z, std::move(basinTexture));

    ground.initialize(-GROUND_SIZE, GROUND_SIZE,
                      -GROUND_SIZE, GROUND_SIZE,
                      std::move(groundTexture), GROUND_TEX_REPEAT);

    // place fountains 
	fountain.center = glm::vec3(BASIN_INNER_X / 2.0f, POOL_HEIGHT, BASIN_INNER_Z / 2.0f);
	fountains[0].center = glm::vec3(BASIN_INNER_X / 8.0f * 7.0f, POOL_HEIGHT, BASIN_INNER_Z / 8.0f * 7.0f);
	fountains[1].center = glm::vec3(BASIN_INNER_X / 8.0f * 7.0f, POOL_HEIGHT, BASIN_INNER_Z / 8.0f * 1.0f);
	fountains[2].center = glm::vec3(BASIN_INNER_X / 8.0f * 1.0f, POOL_HEIGHT, BASIN_INNER_Z / 8.0f * 7.0f);
	fountains[3].center = glm::vec3(BASIN_INNER_X / 8.0f * 1.0f, POOL_HEIGHT, BASIN_INNER_Z / 8.0f * 1.0f);

    // initialize camera:
	glm::vec3 cposition, crotation;
    cposition = glm::vec3(CAMERA_POSITION[0], CAMERA_POSITION[1], CAMERA_POSITION[2]);
    camera.move(cposition);
    crotation = glm::vec3(CAMERA_ROTATION[0], CAMERA_ROTATION[1], CAMERA_ROTATION[2]);
    camera.rotate(crotation);

    // enable vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // solid rendering
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    // lighting
    glLightfv(GL_LIGHT1, GL_AMBIENT, LIGHT_AMBIENT_1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LIGHT_DIFFUSE_1);
    glLightfv(GL_LIGHT1, GL_POSITION, LIGHT_POSITION_1);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT2, GL_AMBIENT, LIGHT_AMBIENT_2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, LIGHT_DIFFUSE_2);
    glLightfv(GL_LIGHT2, GL_POSITION, LIGHT_POSITION_2);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    // settings
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glFrontFace(GL_CCW);   // orientation should be the front face
    glShadeModel(GL_SMOOTH);

    // blending
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // seed
    srand((unsigned)time(NULL));

    // register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);
    glutPassiveMotionFunc(mouseMove);
    glutMouseFunc(mouseButton);
    glutIdleFunc(idle);

    // hide cursor
    glutSetCursor(GLUT_CURSOR_NONE);

    // start
    glutMainLoop();

    return 0;
}
