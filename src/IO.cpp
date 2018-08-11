#include "IO.h"
#include "Brep.h"
#include "HalfEdge.h"
#include <fstream>
#include <vector>
#include <string>
#include <assert.h>

int solidNum, ringNum[MAX_SOLID_NUM];
std::vector<Coordinate> vtxData[MAX_SOLID_NUM][MAX_RING_NUM];


void readFile(const char *fileName) {
	std::ifstream infile;
	infile.open(fileName);  
	assert(infile.is_open()); 
	infile >> solidNum;
	if (solidNum >= MAX_SOLID_NUM - 1) {
		perror("ring num is to large\n");
		exit(0);
	}
	for (int i = 0; i < solidNum; i++) {
		infile >> ringNum[i];
		if (ringNum[i] >= MAX_RING_NUM - 1) {
			perror("ring num is to large\n");
			exit(0);
		}
	}
	
	int vtxNum = 0, flagLoop, flagSolid;	
	//flagLoop 标记数据的类型：0 外loop点， 1-ringNum, 内loop点
	//flagSolid 标记体号
	Coordinate t;
	while (infile>>t.x>>t.y>>t.z>> flagLoop>>flagSolid){
		if (flagLoop >= 0 && flagLoop < MAX_RING_NUM && flagSolid >=0 && flagSolid < MAX_SOLID_NUM) {
			vtxData[flagSolid][flagLoop].push_back(t);
		}else {
			perror("Error input data\n");
			exit(0);
		}
		vtxNum++;
	}
	if (vtxNum > MAX_VTX_NUM) {
		perror("vertex num is too large \n");
		exit(0);
	}
	infile.close();
}