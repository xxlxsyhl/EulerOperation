#include "Euler.h"

vector<Vertex *> vlist;

//mvsf
Solid* Euler::mvsf(Vertex *vtx) {
	Solid *solid = new Solid();
	Face *face = new Face();
	Loop *out_loop = new Loop();
	//face and solid
	solid->face = face;
	face->solid = solid;
	//loop and face 
	face->out_loop = out_loop;
	out_loop->face = face;
	//store vertex
	vlist.push_back(vtx);

	return solid;
}

//mev
Loop* Euler::mev(Loop *&loop, Vertex *vtx1, Vertex *vtx2) {
	vlist.push_back(vtx2);

	Solid *solid = loop->face->solid;
	HalfEdge *left_edge = new HalfEdge(vtx1, vtx2);
	HalfEdge *right_edge = new HalfEdge(vtx2, vtx1);
	Edge *edge = new Edge(vtx1, vtx2, left_edge, right_edge);
	//��ߺͱߵ�ָ����ĳ��Ի� 
	edge->left_edge = left_edge;
	edge->right_edge = right_edge;

	left_edge->edge = edge;
	right_edge->edge = edge;

	left_edge->loop = loop;
	right_edge->loop = loop;

	//��������߼���loop��
	if (loop->hedge == NULL) {//�����ޱ� 
		left_edge->nexth = right_edge;
		left_edge->preh = right_edge;

		right_edge->nexth = left_edge;
		right_edge->preh = left_edge;

		loop->hedge = left_edge;
	}
	else {//�����б� 
		HalfEdge *hedge = loop->hedge;
		while (hedge->evtx != vtx1)  hedge = hedge->nexth;

		left_edge->nexth = right_edge;
		right_edge->preh = left_edge;

		right_edge->nexth = hedge->nexth;
		hedge->nexth->preh = right_edge;

		left_edge->preh = hedge;
		hedge->nexth = left_edge;
	}

	//���߼���߱���
	Euler::addEdge(solid, edge);

	return loop;
}

//mef
Face* Euler::mef(Loop *&loop, Vertex *vtx1, Vertex *vtx2) {
	//�½��ߺ���
	Solid *solid = loop->face->solid;
	Face *face = new Face();
	Loop *newloop = new Loop();
	HalfEdge *left_edge = new HalfEdge(vtx1, vtx2);
	HalfEdge *right_edge = new HalfEdge(vtx2, vtx1);
	Edge *edge = new Edge(vtx1, vtx2, left_edge, right_edge);

	//���ӱߺͰ��
	edge->left_edge = left_edge;
	edge->right_edge = right_edge;

	left_edge->edge = edge;
	right_edge->edge = edge;

	HalfEdge *hedgev1, *hedgev2, *hedgev3, *hedge = loop->hedge;
	//��vtx1��ǰһ����
	while (hedge->evtx != vtx1) hedge = hedge->nexth;
	hedgev1 = hedge;
	//��vtx2��ǰһ����
	while (hedge->evtx != vtx2) hedge = hedge->nexth;
	hedgev2 = hedge;

	hedge = hedgev2->nexth;
	while (hedge->evtx != vtx2) hedge = hedge->nexth;
	hedgev2 = hedge;

	left_edge->nexth = hedgev2->nexth;
	right_edge->nexth = hedgev1->nexth;

	left_edge->preh = hedgev1;
	right_edge->preh = hedgev2;

	hedgev2->nexth = right_edge;
	hedgev1->nexth = left_edge;

	left_edge->nexth->preh = left_edge;
	right_edge->nexth->preh = right_edge;

	//loop
	left_edge->loop = newloop;
	right_edge->loop = loop;

	newloop->hedge = left_edge;
	loop->hedge = right_edge;

	//add edge to solid
	addEdge(solid, edge);

	//add face to solid
	addFace(solid, face);

	//add loop to face
	addLoop(face, newloop);

	return face;
}

//kemr
Loop* Euler::kemr(Loop *&loop, Vertex *vtx1, Vertex *vtx2) {
	Solid *solid = loop->face->solid;
	Face *face = loop->face;
	Loop *in_loop = new Loop();

	HalfEdge *hedge_v1_v2 = NULL, *hedge_v2_v1 = NULL, *hedge = loop->hedge;
	//��vtx1->vtx2�İ��
	while (hedge->svtx != vtx1 || hedge->evtx != vtx2) 
		hedge = hedge->nexth;
	hedge_v1_v2 = hedge;
	//ͨ��edge�ҵ���һ���
	//hedge_v2_v1 = hedge->edge->left_edge == hedge ? hedge->edge->right_edge : hedge->edge->left_edge;
	while (hedge->svtx != vtx2 || hedge->evtx != vtx1)
		hedge = hedge->nexth;
	hedge_v2_v1 = hedge;

	//�պ�����2��loop
	hedge_v1_v2->preh->nexth = hedge_v2_v1->nexth;
	hedge_v2_v1->nexth->preh = hedge_v1_v2->preh;

	hedge_v1_v2->nexth->preh = hedge_v2_v1->preh;
	hedge_v2_v1->preh->nexth = hedge_v1_v2->nexth;

	//ԭout loop
	face->out_loop->hedge = hedge_v1_v2->preh;

	//�µ�in loop
	in_loop->face = face;
	in_loop->hedge = hedge_v1_v2->nexth;

	addLoop(solid->face, in_loop);
	
	//ɾ���������
	delete hedge_v1_v2;
	delete hedge_v2_v1;
	return in_loop;
}

//kfmrh
//f1Ϊ�������--����, f2Ϊ�������--ɾ��
Solid* Euler::kfmrh(Face *f1, Face *f2) {
	Solid *solid = f2->solid;
	addLoop(f1, f2->out_loop);
	Face *face = solid->face;
	if (face == f2) {
		solid->face = face->nextf;
		solid->face->pref = face->pref;
		delete face;
	}else {
		Face *f = face->nextf;
		while (f != f2) f = f->nextf;
		if (f == NULL) return solid;
		else {
			f->pref->nextf = f->nextf;
			if (f->nextf != NULL)	f->nextf->pref = f->pref;
			delete f;
		}
	}
	return solid;
}

//��߱������һ����
Solid* Euler::addEdge(Solid *&solid, Edge *&edge) {
	if (solid->edge == NULL)	solid->edge = edge;
	else {
		//β��
		Edge *p = solid->edge;
		while (p->nexte != NULL) p = p->nexte;
		p->nexte = edge;
		edge->pree = p;
	}
	return solid;
}

//����������һ����
Solid* Euler::addFace(Solid *&solid, Face *&face) {
	face->solid = solid;
	if (solid->face == NULL)		solid->face = face;
	else {
		//ͷ��
		Face *p = solid->face;
		while (p->nextf != NULL) p = p->nextf;
		p->nextf = face;
		face->pref = p;
	}
	return solid;
}

//��loop �������һ��loop
Face* Euler::addLoop(Face *&face, Loop *&loop) {
	//out loop ֻ��һ�����ұ�����out loop
	loop->face = face;
	if (face->out_loop == NULL)		face->out_loop = loop;
	else {
		if (face->in_loop == NULL)	face->in_loop = loop;
		else {
			//ͷ��
			Loop *p = face->in_loop;
			while (p->nextl != NULL) p = p->nextl;
			p->nextl = loop;
			loop->prel = p;
		}
	}
	return face;
}

//��Solid �������һ��solid
Solid* Euler::addSolid(Solid *&solid, Solid *&pSolid) {
	if (solid == NULL) solid = pSolid;
	else {
		//β��
		Solid *p = solid;
		while (p->nexts != NULL) p = p->nexts;
		p->nexts = pSolid;
		pSolid->pres = p;
		pSolid->nexts = NULL;
	}
	return solid;
}

//��Solid�м����
Solid* Euler::addVertex(Solid *&solid, Vertex *vtx) {
	if (solid->vtx == NULL)	solid->vtx == vtx;
	else {
		//ͷ��
		vtx->nextv = solid->vtx->nextv;
		solid->vtx->nextv->prev = vtx;
		solid->vtx = vtx;
	}
	return solid;
}