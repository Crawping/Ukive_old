#pragma once
#include "UGraphCreator.h"

#define ALTITUDE_MAP_SIZE 1024


class QTreeNode;
struct TerrainVertexData;

class ULodGenerator
{
private:
	int mMaxLevel;
	int mVertexCount;
	int mRowVertexCount;

	float COE_ROUGH;
	float COE_DISTANCE;
	QTreeNode *mRootQueue;

	char *mFlag;
	char *mAltitude;
	TerrainVertexData *mVertices;

	int *mIndices;
	int mIndexCount;
	int mMaxIndexCount;

	void enqueue(QTreeNode *&queue, QTreeNode *node);
	QTreeNode *dequeue(QTreeNode *&queue);
	void clearQueue(QTreeNode *&queue);

	int calInnerStep(QTreeNode *node);
	int calNeighborStep(QTreeNode *node);
	int calChildStep(QTreeNode *node);

	void generateQuadTree();
	void generateRootNodeData(QTreeNode *root);
	void generateChildNodeData(
		QTreeNode *parent, QTreeNode *child, int level, int position);

	void determineRoughAndBound(QTreeNode *node);

	bool checkNodeCanDivide(QTreeNode *node);
	bool assessNodeCanDivide(
		QTreeNode *node, XMFLOAT3 viewPosition);

	void drawNode(QTreeNode *node, int *indexBuffer);

	bool cullNodeWithBound(QTreeNode *node, XMFLOAT4X4 wvpMatrix);
	void constructNodeBound(QTreeNode *node, XMFLOAT3 *bound);

public:
	ULodGenerator(float edgeLength, int maxLevel);
	~ULodGenerator();

	void setCoefficient(float c1, float c2);

	int getLevel();
	float getCoef1();
	float getCoef2();

	void renderLodTerrain(
		XMFLOAT3 viewPosition, XMFLOAT4X4 wvpMatrix, int *indexBuffer);

	TerrainVertexData *getVertices();
	int getVertexCount();
	int getRowVertexCount();

	int *getIndices();
	int getIndexCount();
	int getMaxIndexCount();
};

