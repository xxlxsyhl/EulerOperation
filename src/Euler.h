#ifndef EULER_H
#define EULER_H

#include <vector>
#include "HalfEdge.h"
using namespace std;

struct Euler{
	//static std::vector<Vertex *> vlist;

	//5个基本欧拉操作 
	static Solid *mvsf(Vertex *vtx);
	static Loop *mev(Loop *&loop, Vertex *vtx1, Vertex *vtx2);
	static Face *mef(Loop *&loop, Vertex *vtx1, Vertex *vtx2);
	static Loop *kemr(Loop *&loop, Vertex *vtx1, Vertex *vtx2);
	static Solid *kfmrh(Face *f1, Face *f2);

	//辅助操作
	static Solid *addEdge(Solid *&solid, Edge *&edge);
	static Face *addLoop(Face *&face, Loop *&loop);
	static Solid *addFace(Solid *&solid, Face *&face);
	static Solid *addSolid(Solid *&solid, Solid *&pSolid);
	static Solid *addVertex(Solid *&solid, Vertex *vtx);
};

#endif