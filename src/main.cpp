#include "Display.h"
#include "Euler.h"
#include "Sweep.h"
#include "Brep.h"

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow("BrepCAD");
	initRendering();
	makeSolid();
	glutDisplayFunc(render);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMouseFunc(handleMousePress);
	glutMotionFunc(handleMouseActiveMotion);
	glutMainLoop();
	return 0;
}