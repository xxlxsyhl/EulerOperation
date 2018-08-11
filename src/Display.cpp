#include "Display.h"
#include "HalfEdge.h"
#include "Brep.h"
#include <vector>

extern Solid *solid;

GLdouble data[MAX_VTX_NUM][3];

GLfloat mouseX = 0.0, mouseY = 0.0;
GLfloat cameraAngleX = 0.0, cameraAngleY = 0.0;
GLfloat cameraDistance = -30.0;
bool mouseLeftDown = false, mouseRightDown = false;

//获取平面的法向量
void calNormal(Loop *loop, Coordinate &normal) {
	std::vector<Coordinate> cdt;
	HalfEdge * hedge = loop->hedge;
	int cnt = 3;
	while (cnt --) {//取三个点
		cdt.push_back(hedge->evtx->cdt);
		hedge = hedge->nexth;
	}
	Coordinate a = cdt[1] - cdt[0], b = cdt[2] - cdt[1];
	normal.x = a.y*b.z - a.z*b.y;
	normal.y = a.z*b.x - a.x*b.z;
	normal.z = a.x*b.y - a.y*b.z;
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1); //Enable light #1
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading
}


void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}
//处理按键
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: exit(0);
	}
}
//处理鼠标按下
void handleMousePress(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
			mouseLeftDown = true;
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
			mouseRightDown = true;
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}
}

//处理鼠标移动
void handleMouseActiveMotion(int x, int y)
{
	if (mouseLeftDown)
	{
		cameraAngleY += (x - mouseX);
		cameraAngleX += (y - mouseY);
		mouseX = x;
		mouseY = y;
	}

	if (mouseRightDown)
	{
		cameraDistance += (y - mouseY) * 0.2f;
		mouseY = y;
	}

	glutPostRedisplay();
}

//绘制
void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//双面
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//全局光
	GLfloat ambientColor[] = { 0.2, 0.2, 0.2, 0.2 }; 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	//点光
	GLfloat lightColor0[] = { 1, 1, 1, 1 }; 
	GLfloat lightPos0[] = { 2, 0, 2, 1 }; 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	
	glTranslatef(0, 0, cameraDistance);
	glRotatef(cameraAngleX, 1, 0, 0);
	glRotatef(cameraAngleY, 0, 1, 0);
	//绘制
	int idx = 0;
	Coordinate normal;
	GLUtesselator * tessobj = gluNewTess();
	//注册回调
	gluTessCallback(tessobj, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
	gluTessCallback(tessobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
	gluTessCallback(tessobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
	gluTessCallback(tessobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);
	//开始绘制
	Solid *pSolid = solid;
	while (pSolid != NULL) {
		Face *face = pSolid->face;
		while (face != NULL) {
			gluTessBeginPolygon(tessobj, NULL);
			//处理out loop, out loop 只有一个
			Loop *loop = face->out_loop;
			calNormal(loop, normal);
			glNormal3f(normal.x, normal.y, normal.z);
			gluTessBeginContour(tessobj);
			HalfEdge *starte = loop->hedge, *pedge = NULL, *p = starte;
			while (starte != pedge) {
				Vertex *v = p->evtx;
				data[idx][0] = v->cdt.x, data[idx][1] = v->cdt.y, data[idx][2] = v->cdt.z;
				gluTessVertex(tessobj, data[idx], data[idx]);
				idx++;
				p = p->nexth;
				pedge = p;
			}
			gluTessEndContour(tessobj);
			//处理in loop，in loop 可能有多个
			loop = face->in_loop;
			while (loop != NULL) {
				gluTessBeginContour(tessobj);
				starte = loop->hedge, pedge = NULL, p = starte;
				while (starte != pedge) {
					Vertex *v = p->evtx;
					data[idx][0] = v->cdt.x, data[idx][1] = v->cdt.y, data[idx][2] = v->cdt.z;
					gluTessVertex(tessobj, data[idx], data[idx]);
					p = p->nexth;
					pedge = p;
					idx++;
				}
				loop = loop->nextl;
				gluTessEndContour(tessobj);
			}
			face = face->nextf;
			gluTessEndPolygon(tessobj);
		}
		pSolid = pSolid->nexts;
	}
	glutSwapBuffers();
}


//相关回调函数
void CALLBACK vertexCallback(GLvoid* vertex)
{
	GLdouble* pt;

	int numb;
	pt = (GLdouble*)vertex;

	glColor3f(0, 1.0, 0.5);

	glVertex3d(pt[0], pt[1], pt[2]);
}

void CALLBACK beginCallback(GLenum type)
{
	glBegin(type);
}

void CALLBACK endCallback()
{
	glEnd();
}

void CALLBACK errorCallback(GLenum errorCode)
{
	perror("Tessellation Error/n");
	exit(0);
}