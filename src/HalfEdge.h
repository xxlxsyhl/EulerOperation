#ifndef HALFEDGE_H
#define HALFEDGE_H

#include <iostream>

//给相关结构体赋一个唯一的ID
static int vertexIdx = 0;
static int hedgeIdx = 0;

//half edge data structure
struct Solid;
struct Face;
struct Loop;
struct HalfEdge;
struct Vertex;
struct Edge;

struct Coordinate {
	double x, y, z;
	Coordinate() {};
	Coordinate(double xx, double yy, double zz):x(xx), y(yy), z(zz) {};
	friend Coordinate operator-(Coordinate &a, Coordinate &b) {
		return Coordinate(a.x-b.x,a.y-b.y, a.z-b.z);
	}
};
//Solid

struct Solid {
	Solid *pres, *nexts;
	Face *face;
	Edge *edge;
	Vertex *vtx;

	Solid(): pres(NULL), nexts(NULL), face(NULL), edge(NULL), vtx(NULL){};
};
//Face
struct Face {
	Face *pref, *nextf;
	Solid *solid;
	Loop *in_loop, *out_loop;//inner loop and out loop

	Face(): pref(NULL), nextf(NULL), solid(NULL), in_loop(NULL), out_loop(NULL) {};
};

//Loop
struct Loop {
	Loop *prel, *nextl;
	Face *face;
	HalfEdge *hedge;

	Loop(): prel(NULL), nextl(NULL), face(NULL), hedge(NULL) {};
};

//HalfEdge
struct HalfEdge {
	HalfEdge *preh, *nexth;
	Loop *loop;
	Vertex *svtx, *evtx;
	Edge *edge;
	int id;

	HalfEdge(Vertex *svtx, Vertex *evtx): preh(NULL), nexth(NULL), loop(NULL), edge(NULL) {
		this->svtx = svtx;
		this->evtx = evtx;
		id = hedgeIdx++;
	}

	friend bool operator==(HalfEdge &a, HalfEdge &b) {
		if (a.id == b.id)	return true;
		else   return false;
	}
};

//Edge
struct Edge {
	Edge *pree, *nexte;
	HalfEdge *left_edge, *right_edge;
	Vertex *vtx1, *vtx2;

	Edge(Vertex *vtx1, Vertex *vtx2, HalfEdge *left_edge, HalfEdge *right_edge): pree(NULL), nexte(NULL) {
		this->vtx1 = vtx1, this->vtx2 = vtx2;
		this->left_edge = left_edge, this->right_edge = right_edge;
	}
};

//Vertex
struct Vertex {
	Vertex *prev, *nextv;
	int id;

	Coordinate cdt;//几何信息 

	Vertex(double x, double y, double z): prev(NULL), nextv(NULL){
		cdt.x = x, cdt.y = y, cdt.z = z;
		this->id = vertexIdx ++;
	}

	friend bool operator==(Vertex &a, Vertex &b) {
		if (a.id == b.id)	return true;
		else   return false;
	}
};

#endif
