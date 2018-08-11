#include "HalfEdge.h"
#include "Sweep.h"
#include "IO.h"
#include "Brep.h"
#include <vector>

extern int solidNum, ringNum[MAX_SOLID_NUM];
extern std::vector<Coordinate> vtxData[MAX_SOLID_NUM][MAX_RING_NUM];
Solid *solid = NULL;

void makeSolid() {
	//����������
	readFile("data.txt");
	Solid *pSolid = NULL;
	for (int solidIdx = 0; solidIdx < solidNum; solidIdx++) {
		//��һ��ƽ��
		Vertex *vtx1 = new Vertex(vtxData[solidIdx][0][0].x, vtxData[solidIdx][0][0].y, vtxData[solidIdx][0][0].z);
		Vertex *startv = vtx1, *vtx2 = NULL;
		pSolid = Euler::mvsf(vtx1);
		Euler::addVertex(pSolid, vtx1);
		for (int i = 1; i != vtxData[solidIdx][0].size(); i++) {
			vtx2 = new Vertex(vtxData[solidIdx][0][i].x, vtxData[solidIdx][0][i].y, vtxData[solidIdx][0][i].z);
			Euler::addVertex(pSolid, vtx2);
			Euler::mev(pSolid->face->out_loop, vtx1, vtx2);
			vtx1 = vtx2;
		}
		Euler::mef(pSolid->face->out_loop, vtx2, startv);
		//��ƽ���е��ڻ�

		for (int i = 1; i <= ringNum[solidIdx]; i++) {
			Vertex *inLoopStartv = new Vertex(vtxData[solidIdx][i][0].x, vtxData[solidIdx][i][0].y, vtxData[solidIdx][i][0].z);
			Euler::mev(pSolid->face->nextf->out_loop, startv, inLoopStartv);
			vtx1 = inLoopStartv;
			Euler::addVertex(pSolid, vtx1);
			for (int j = 1; j < vtxData[solidIdx][i].size(); j++) {
				vtx2 = new Vertex(vtxData[solidIdx][i][j].x, vtxData[solidIdx][i][j].y, vtxData[solidIdx][i][j].z);
				Euler::addVertex(pSolid, vtx2);
				Euler::mev(pSolid->face->nextf->out_loop, vtx1, vtx2);
				vtx1 = vtx2;
			}
			//����һ����
			Face *face = Euler::mef(pSolid->face->nextf->out_loop, vtx2, inLoopStartv);
			//ɾ����
			Euler::kemr(pSolid->face->nextf->out_loop, startv, inLoopStartv);
			//ɾ����face
			Euler::kfmrh(pSolid->face->nextf, face);
		}

		//�趨ƽ��ɨ�ɷ���
		double direc[3] = { 0, 0, 1 };
		//ɨ��
		Sweep::translation(pSolid, 5, direc);
		//���һ����
		Euler::addSolid(solid, pSolid);
	}
}