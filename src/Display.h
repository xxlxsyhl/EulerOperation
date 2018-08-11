#ifndef DISPLAY_H
#define DISPLAY_H
#include <GL/glut.h>
#include "HalfEdge.h"

void initRendering();

void handleResize(int w, int h);

void handleKeypress(unsigned char key, int x, int y);

void handleMousePress(int button, int state, int x, int y);

void handleMouseActiveMotion(int x, int y);

void render();

void CALLBACK vertexCallback(GLvoid* vertex);

void CALLBACK beginCallback(GLenum type);

void CALLBACK endCallback();

void CALLBACK errorCallback(GLenum errorCode);

#endif
