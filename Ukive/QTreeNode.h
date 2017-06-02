#pragma once

class QTreeNode
{
public:
	int level;
	int indexX;
	int indexY;
	float rough;
	XMFLOAT3 mincoord;
	XMFLOAT3 maxcoord;
	QTreeNode *child[4];

	QTreeNode *next;

public:
	QTreeNode();
	~QTreeNode();
};

