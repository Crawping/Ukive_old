#include "UCommon.h"
#include "UMath.h"
#include "QTreeNode.h"
#include "UTerrainConfigure.h"
#include "ULodGenerator.h"


ULodGenerator::ULodGenerator(float edgeLength, int maxLevel)
{
	if (edgeLength <= 0 || maxLevel < 1)
		throw std::invalid_argument("ULodGenerator-Constructor(): invalid params.");

	COE_ROUGH = 2.f;
	COE_DISTANCE = 30.f;

	mMaxLevel = maxLevel;
	mRowVertexCount = UMath::pow(2, mMaxLevel) + 1;
	mVertexCount = mRowVertexCount*mRowVertexCount;

	mFlag = new char[mVertexCount];
	::memset(mFlag, 0, mVertexCount);

	std::wstring altitudeFileName(::_wgetcwd(nullptr, 0));
	altitudeFileName.append(L"\\altitude.raw");

	std::ifstream reader(altitudeFileName, std::ios::binary);
	if (reader.fail())
		throw std::runtime_error("ULodGenerator-Constructor(): read file failed.");
	auto cpos = reader.tellg();
	reader.seekg(0, std::ios_base::end);
	size_t charSize = (size_t)reader.tellg();
	reader.seekg(cpos);

	mAltitude = new char[charSize];
	reader.read(mAltitude, charSize);

	mVertices = new TerrainVertexData[mVertexCount];
	for (int i = 0; i < mVertexCount; ++i)
	{
		int row = i / mRowVertexCount;
		int column = i % mRowVertexCount;

		int altitudeRow = (int)((ALTITUDE_MAP_SIZE / (float)mRowVertexCount)*row);
		int altitudeColumn = (int)((ALTITUDE_MAP_SIZE / (float)mRowVertexCount)*column);

		int altitude = mAltitude[
			(ALTITUDE_MAP_SIZE - 1 - altitudeRow) * ALTITUDE_MAP_SIZE + altitudeColumn];
		if (altitude < 0)
			altitude += 255;

		mVertices[i].position = XMFLOAT3(
			edgeLength*column / (mRowVertexCount - 1),
			(float)altitude * 2, edgeLength - edgeLength*row / (mRowVertexCount - 1));
	}

	mIndexCount = 0;
	mMaxIndexCount = (mRowVertexCount - 1)*(mRowVertexCount - 1) * 2 * 3;

	mIndices = new int[mMaxIndexCount];

	generateQuadTree();
	determineRoughAndBound(mRootQueue);
}

ULodGenerator::~ULodGenerator()
{
	if (mRootQueue)
	{
		QTreeNode* curQueue = mRootQueue;
		QTreeNode* nextQueue = nullptr;

		do
		{
			while (curQueue)
			{
				QTreeNode* node = curQueue;
				curQueue = curQueue->next;

				for (int i = 0; i < 4; ++i)
				{
					if (node->child[i])
						enqueue(nextQueue, node->child[i]);
				}

				delete node;
			}

			curQueue = nextQueue;
			nextQueue = nullptr;
		} while (curQueue);
	}

	delete[] mFlag;
	delete[] mIndices;
	delete[] mVertices;
	delete[] mAltitude;
}


inline void ULodGenerator::enqueue(QTreeNode*& queue, QTreeNode* node)
{
	node->next = queue;
	queue = node;
}

inline QTreeNode* ULodGenerator::dequeue(QTreeNode*& queue)
{
	if (queue)
	{
		QTreeNode* node = queue;
		queue = queue->next;
		return node;
	}

	return nullptr;
}

inline void ULodGenerator::clearQueue(QTreeNode*& queue)
{
	while (queue)
	{
		QTreeNode* node = queue;
		queue = queue->next;
		delete node;
	}
}


inline int ULodGenerator::calInnerStep(QTreeNode* node)
{
	return (mRowVertexCount - 1) / UMath::pow(2, node->level + 1);
}

inline int ULodGenerator::calNeighborStep(QTreeNode* node)
{
	return (mRowVertexCount - 1) / UMath::pow(2, node->level);
}

inline int ULodGenerator::calChildStep(QTreeNode* node)
{
	return (mRowVertexCount - 1) / UMath::pow(2, node->level + 2);
}


void ULodGenerator::generateQuadTree()
{
	int level = 0;
	QTreeNode *curQueue = nullptr;
	QTreeNode *nextQueue = nullptr;

	mRootQueue = new QTreeNode();
	generateRootNodeData(mRootQueue);

	curQueue = mRootQueue;

	while (level < mMaxLevel - 1)
	{
		QTreeNode *iterator = curQueue;
		while (iterator)
		{
			QTreeNode* parent = iterator;
			iterator = iterator->next;

			for (int i = 0; i < 4; ++i)
			{
				QTreeNode *child = new QTreeNode();
				generateChildNodeData(parent, child, level + 1, i);

				parent->child[i] = child;
				enqueue(nextQueue, child);
			}
		}

		curQueue = nextQueue;
		nextQueue = nullptr;

		++level;
	}
}

void ULodGenerator::generateRootNodeData(QTreeNode* root)
{
	root->level = 0;
	root->indexX = (mRowVertexCount - 1) / 2;
	root->indexY = (mRowVertexCount - 1) / 2;
}

void ULodGenerator::generateChildNodeData(
	QTreeNode* parent, QTreeNode* child, int level, int position)
{
	child->level = level;

	int pIndexX = parent->indexX;
	int pIndexY = parent->indexY;

	int step = calChildStep(parent);

	switch (position)
	{
	case 0:
		child->indexX = pIndexX - step;
		child->indexY = pIndexY - step;
		break;
	case 1:
		child->indexX = pIndexX + step;
		child->indexY = pIndexY - step;
		break;
	case 2:
		child->indexX = pIndexX - step;
		child->indexY = pIndexY + step;
		break;
	case 3:
		child->indexX = pIndexX + step;
		child->indexY = pIndexY + step;
		break;
	}
}


void ULodGenerator::determineRoughAndBound(QTreeNode *node)
{
	//使用stack和while循环代替递归，防止调用栈溢出。

	//用于保存递归上下文的结构体。
	struct Snapshot
	{
		QTreeNode *node;

		float d0, d1, d2, d3, d4;
		float d5, d6, d7, d8;
		float nodeSize;
		float maxD;
		XMFLOAT3 minC;
		XMFLOAT3 maxC;

		int stage;
		Snapshot *behind;
	};

	//栈顶指针。
	Snapshot *recursionStack = nullptr;

	Snapshot *current = new Snapshot();
	current->node = node;
	current->stage = 0;
	current->behind = nullptr;

	recursionStack = current;

	//返回值
	float diff;
	XMFLOAT3 mincoord;
	XMFLOAT3 maxcoord;

	while (recursionStack)
	{
		//栈pop.
		current = recursionStack;
		recursionStack = recursionStack->behind;

		switch (current->stage)
		{
		case 0:  //在第一次递归之前。
		{
			int innerStep = calInnerStep(current->node);
			TerrainVertexData *vData[9];

			vData[0] = &mVertices[
				(current->node->indexY - innerStep)*mRowVertexCount + current->node->indexX - innerStep];
			vData[1] = &mVertices[
				(current->node->indexY - innerStep)*mRowVertexCount + current->node->indexX];
			vData[2] = &mVertices[
				(current->node->indexY - innerStep)*mRowVertexCount + current->node->indexX + innerStep];
			vData[3] = &mVertices[
				current->node->indexY*mRowVertexCount + current->node->indexX - innerStep];
			vData[4] = &mVertices[
				current->node->indexY*mRowVertexCount + current->node->indexX];
			vData[5] = &mVertices[
				current->node->indexY*mRowVertexCount + current->node->indexX + innerStep];
			vData[6] = &mVertices[
				(current->node->indexY + innerStep)*mRowVertexCount + current->node->indexX - innerStep];
			vData[7] = &mVertices[
				(current->node->indexY + innerStep)*mRowVertexCount + current->node->indexX];
			vData[8] = &mVertices[
				(current->node->indexY + innerStep)*mRowVertexCount + current->node->indexX + innerStep];

			float minX, minY, minZ;
			float maxX, maxY, maxZ;
			for (int i = 0; i < 9; ++i)
			{
				float x = vData[i]->position.x;
				float y = vData[i]->position.y;
				float z = vData[i]->position.z;

				if (i == 0)
				{
					minX = maxX = x;
					minY = maxY = y;
					minZ = maxZ = z;
				}
				else
				{
					if (x < minX) minX = x;
					if (x > maxX) maxX = x;

					if (y < minY) minY = y;
					if (y > maxY) maxY = y;

					if (z < minZ) minZ = z;
					if (z > maxZ) maxZ = z;
				}
			}

			current->minC = XMFLOAT3(minX, minY, minZ);
			current->maxC = XMFLOAT3(maxX, maxY, maxZ);

			float topHor = (vData[0]->position.y + vData[2]->position.y) / 2.f;
			float rightHor = (vData[2]->position.y + vData[8]->position.y) / 2.f;
			float bottomHor = (vData[8]->position.y + vData[6]->position.y) / 2.f;
			float leftHor = (vData[6]->position.y + vData[0]->position.y) / 2.f;

			current->d0 = UMath::abs(vData[1]->position.y - topHor);
			current->d1 = UMath::abs(vData[5]->position.y - rightHor);
			current->d2 = UMath::abs(vData[7]->position.y - bottomHor);
			current->d3 = UMath::abs(vData[3]->position.y - leftHor);
			current->d4 = UMath::abs(vData[4]->position.y - (topHor + rightHor + bottomHor + leftHor) / 4.f);

			XMVECTOR length = DirectX::XMVector3Length(DirectX::XMVectorSubtract(
				DirectX::XMLoadFloat3(&vData[2]->position),
				DirectX::XMLoadFloat3(&vData[0]->position)));
			current->nodeSize = DirectX::XMVectorGetX(length);

			if (current->node->child[0])
			{
				current->stage = 1;
				current->behind = recursionStack;
				recursionStack = current;

				//递归，第一个子节点。
				Snapshot *next = new Snapshot();
				next->node = current->node->child[0];
				next->stage = 0;
				next->behind = recursionStack;
				recursionStack = next;
			}
			else
			{
				current->maxD = current->d0;
				if (current->d1 > current->maxD) current->maxD = current->d1;
				if (current->d2 > current->maxD) current->maxD = current->d2;
				if (current->d3 > current->maxD) current->maxD = current->d3;
				if (current->d4 > current->maxD) current->maxD = current->d4;

				current->stage = 4;
				current->behind = recursionStack;
				recursionStack = current;
			}

			break;
		}

		case 1:  //第一次调用返回后。
		{
			current->d5 = diff;
			if (mincoord.x < current->minC.x)
				current->minC.x = mincoord.x;
			if (mincoord.y < current->minC.y)
				current->minC.y = mincoord.y;
			if (mincoord.z < current->minC.z)
				current->minC.z = mincoord.z;

			if (maxcoord.x > current->maxC.x)
				current->maxC.x = maxcoord.x;
			if (maxcoord.y > current->maxC.y)
				current->maxC.y = maxcoord.y;
			if (maxcoord.z > current->maxC.z)
				current->maxC.z = maxcoord.z;

			current->stage = 2;
			current->behind = recursionStack;
			recursionStack = current;

			//递归，第二个子节点。
			Snapshot *next = new Snapshot();
			next->node = current->node->child[1];
			next->stage = 0;
			next->behind = recursionStack;
			recursionStack = next;

			break;
		}

		case 2:  //第二次调用返回后。
		{
			current->d6 = diff;
			if (mincoord.x < current->minC.x)
				current->minC.x = mincoord.x;
			if (mincoord.y < current->minC.y)
				current->minC.y = mincoord.y;
			if (mincoord.z < current->minC.z)
				current->minC.z = mincoord.z;

			if (maxcoord.x > current->maxC.x)
				current->maxC.x = maxcoord.x;
			if (maxcoord.y > current->maxC.y)
				current->maxC.y = maxcoord.y;
			if (maxcoord.z > current->maxC.z)
				current->maxC.z = maxcoord.z;

			current->stage = 3;
			current->behind = recursionStack;
			recursionStack = current;

			//递归，第三个子节点。
			Snapshot *next = new Snapshot();
			next->node = current->node->child[2];
			next->stage = 0;
			next->behind = recursionStack;
			recursionStack = next;

			break;
		}

		case 3:  //第三次调用返回后。
		{
			current->d7 = diff;
			if (mincoord.x < current->minC.x)
				current->minC.x = mincoord.x;
			if (mincoord.y < current->minC.y)
				current->minC.y = mincoord.y;
			if (mincoord.z < current->minC.z)
				current->minC.z = mincoord.z;

			if (maxcoord.x > current->maxC.x)
				current->maxC.x = maxcoord.x;
			if (maxcoord.y > current->maxC.y)
				current->maxC.y = maxcoord.y;
			if (maxcoord.z > current->maxC.z)
				current->maxC.z = maxcoord.z;

			current->stage = 4;
			current->behind = recursionStack;
			recursionStack = current;

			//递归，第四个子节点。
			Snapshot *next = new Snapshot();
			next->node = current->node->child[3];
			next->stage = 0;
			next->behind = recursionStack;
			recursionStack = next;

			break;
		}

		case 4:   //第四次调用返回后。
		{
			if (current->node->child[0])
			{
				current->d8 = diff;
				current->maxD = current->d0;
				if (mincoord.x < current->minC.x)
					current->minC.x = mincoord.x;
				if (mincoord.y < current->minC.y)
					current->minC.y = mincoord.y;
				if (mincoord.z < current->minC.z)
					current->minC.z = mincoord.z;

				if (maxcoord.x > current->maxC.x)
					current->maxC.x = maxcoord.x;
				if (maxcoord.y > current->maxC.y)
					current->maxC.y = maxcoord.y;
				if (maxcoord.z > current->maxC.z)
					current->maxC.z = maxcoord.z;

				if (current->d1 > current->maxD) current->maxD = current->d1;
				if (current->d2 > current->maxD) current->maxD = current->d2;
				if (current->d3 > current->maxD) current->maxD = current->d3;
				if (current->d4 > current->maxD) current->maxD = current->d4;
				if (current->d5 > current->maxD) current->maxD = current->d5;
				if (current->d6 > current->maxD) current->maxD = current->d6;
				if (current->d7 > current->maxD) current->maxD = current->d7;
				if (current->d8 > current->maxD) current->maxD = current->d8;
			}

			diff = current->maxD;
			mincoord = current->minC;
			maxcoord = current->maxC;

			current->node->rough = current->maxD / current->nodeSize;
			current->node->mincoord = current->minC;
			current->node->maxcoord = current->maxC;

			delete current;
			break;
		}
		}
	}
}


bool ULodGenerator::checkNodeCanDivide(QTreeNode* node)
{
	int step = calNeighborStep(node);

	bool leftAdjacent = false;
	bool topAdjacent = false;
	bool rightAdjacent = false;
	bool bottomAdjacent = false;

	if (node->indexX - step < 0
		|| mFlag[node->indexY*mRowVertexCount + node->indexX - step] != 0)
		leftAdjacent = true;

	if (node->indexY - step < 0
		|| mFlag[(node->indexY - step)*mRowVertexCount + node->indexX] != 0)
		topAdjacent = true;

	if (node->indexX + step > mRowVertexCount - 1
		|| mFlag[node->indexY*mRowVertexCount + node->indexX + step] != 0)
		rightAdjacent = true;

	if (node->indexY + step > mRowVertexCount - 1
		|| mFlag[(node->indexY + step)*mRowVertexCount + node->indexX] != 0)
		bottomAdjacent = true;

	return (leftAdjacent
		&& topAdjacent
		&& rightAdjacent
		&& bottomAdjacent);
}

bool ULodGenerator::assessNodeCanDivide(
	QTreeNode* node, XMFLOAT3 viewPosition)
{
	int innerStep = calInnerStep(node);

	int centerIndex = node->indexY*mRowVertexCount + node->indexX;
	int leftTopIndex = (node->indexY - innerStep)*mRowVertexCount + node->indexX - innerStep;
	int rightTopIndex = (node->indexY - innerStep)*mRowVertexCount + node->indexX + innerStep;

	XMFLOAT3 nCenter = mVertices[centerIndex].position;

	XMVECTOR length = DirectX::XMVector3Length(DirectX::XMVectorSubtract(
		DirectX::XMLoadFloat3(&nCenter),
		DirectX::XMLoadFloat3(&viewPosition)));
	float distance = DirectX::XMVectorGetX(length);

	XMVECTOR size = DirectX::XMVector3Length(DirectX::XMVectorSubtract(
		DirectX::XMLoadFloat3(&mVertices[rightTopIndex].position),
		DirectX::XMLoadFloat3(&mVertices[leftTopIndex].position)));
	float nodeSize = DirectX::XMVectorGetX(size);

	return (distance / (nodeSize * node->rough * COE_DISTANCE * COE_ROUGH)) < 1.f;
}


void ULodGenerator::drawNode(QTreeNode* node, int* indexBuffer)
{
	int step = calNeighborStep(node);

	if (!indexBuffer)
		indexBuffer = mIndices;

	bool skipLeft = false;
	bool skipTop = false;
	bool skipRight = false;
	bool skipBottom = false;

	if (node->indexX - step < 0
		|| mFlag[node->indexY*mRowVertexCount + node->indexX - step] == 0)
		skipLeft = true;

	if (node->indexY - step < 0
		|| mFlag[(node->indexY - step)*mRowVertexCount + node->indexX] == 0)
		skipTop = true;

	if (node->indexX + step > mRowVertexCount - 1
		|| mFlag[node->indexY*mRowVertexCount + node->indexX + step] == 0)
		skipRight = true;

	if (node->indexY + step > mRowVertexCount - 1
		|| mFlag[(node->indexY + step)*mRowVertexCount + node->indexX] == 0)
		skipBottom = true;

	int nodeStep = calInnerStep(node);

	int centerIndex = node->indexY*mRowVertexCount + node->indexX;
	int leftTopIndex = (node->indexY - nodeStep)*mRowVertexCount + node->indexX - nodeStep;
	int rightTopIndex = (node->indexY - nodeStep)*mRowVertexCount + node->indexX + nodeStep;
	int leftBottomIndex = (node->indexY + nodeStep)*mRowVertexCount + node->indexX - nodeStep;
	int rightBottomIndex = (node->indexY + nodeStep)*mRowVertexCount + node->indexX + nodeStep;

	if (!skipLeft)
	{
		int leftIndex = node->indexY*mRowVertexCount + node->indexX - nodeStep;

		indexBuffer[mIndexCount++] = centerIndex;
		indexBuffer[mIndexCount++] = leftBottomIndex;
		indexBuffer[mIndexCount++] = leftIndex;

		indexBuffer[mIndexCount++] = centerIndex;
		indexBuffer[mIndexCount++] = leftIndex;
		indexBuffer[mIndexCount++] = leftTopIndex;
	}
	else
	{
		indexBuffer[mIndexCount++] = centerIndex;
		indexBuffer[mIndexCount++] = leftBottomIndex;
		indexBuffer[mIndexCount++] = leftTopIndex;
	}

	if (!skipTop)
	{
		int topIndex = (node->indexY - nodeStep)*mRowVertexCount + node->indexX;

		indexBuffer[mIndexCount++] = centerIndex;
		indexBuffer[mIndexCount++] = leftTopIndex;
		indexBuffer[mIndexCount++] = topIndex;

		indexBuffer[mIndexCount++] = centerIndex;
		indexBuffer[mIndexCount++] = topIndex;
		indexBuffer[mIndexCount++] = rightTopIndex;
	}
	else
	{
		indexBuffer[mIndexCount++] = centerIndex;
		indexBuffer[mIndexCount++] = leftTopIndex;
		indexBuffer[mIndexCount++] = rightTopIndex;
	}

	if (!skipRight)
	{
		int rightIndex = node->indexY*mRowVertexCount + node->indexX + nodeStep;

		indexBuffer[mIndexCount++] = centerIndex;
		indexBuffer[mIndexCount++] = rightTopIndex;
		indexBuffer[mIndexCount++] = rightIndex;

		indexBuffer[mIndexCount++] = centerIndex;
		indexBuffer[mIndexCount++] = rightIndex;
		indexBuffer[mIndexCount++] = rightBottomIndex;
	}
	else
	{
		indexBuffer[mIndexCount++] = centerIndex;
		indexBuffer[mIndexCount++] = rightTopIndex;
		indexBuffer[mIndexCount++] = rightBottomIndex;
	}

	if (!skipBottom)
	{
		int bottomIndex = (node->indexY + nodeStep)*mRowVertexCount + node->indexX;

		indexBuffer[mIndexCount++] = centerIndex;
		indexBuffer[mIndexCount++] = rightBottomIndex;
		indexBuffer[mIndexCount++] = bottomIndex;

		indexBuffer[mIndexCount++] = centerIndex;
		indexBuffer[mIndexCount++] = bottomIndex;
		indexBuffer[mIndexCount++] = leftBottomIndex;
	}
	else
	{
		indexBuffer[mIndexCount++] = centerIndex;
		indexBuffer[mIndexCount++] = rightBottomIndex;
		indexBuffer[mIndexCount++] = leftBottomIndex;
	}
}

bool ULodGenerator::cullNodeWithBound(QTreeNode *node, XMFLOAT4X4 wvpMatrix)
{
	bool result = false;
	XMVECTOR plane[6];

	XMVECTOR col0 = DirectX::XMVectorSet(wvpMatrix._11, wvpMatrix._21, wvpMatrix._31, wvpMatrix._41);
	XMVECTOR col1 = DirectX::XMVectorSet(wvpMatrix._12, wvpMatrix._22, wvpMatrix._32, wvpMatrix._42);
	XMVECTOR col2 = DirectX::XMVectorSet(wvpMatrix._13, wvpMatrix._23, wvpMatrix._33, wvpMatrix._43);
	XMVECTOR col3 = DirectX::XMVectorSet(wvpMatrix._14, wvpMatrix._24, wvpMatrix._34, wvpMatrix._44);

	plane[0] = col2;
	plane[1] = DirectX::XMVectorSubtract(col3, col2);
	plane[2] = DirectX::XMVectorAdd(col3, col0);
	plane[3] = DirectX::XMVectorSubtract(col3, col0);
	plane[4] = DirectX::XMVectorSubtract(col3, col1);
	plane[5] = DirectX::XMVectorAdd(col3, col1);
		
	XMFLOAT3 p, q;
	for (UINT i = 0; i < 6; i++)
	{
		plane[i] = DirectX::XMVector4Normalize(plane[i]);

		XMFLOAT4 planeStore;
		DirectX::XMStoreFloat4(&planeStore, plane[i]);

		if (planeStore.x > 0)
		{
			q.x = node->maxcoord.x;
			p.x = node->mincoord.x;
		}
		else
		{
			p.x = node->maxcoord.x;
			q.x = node->mincoord.x;
		}

		if (planeStore.y > 0)
		{
			q.y = node->maxcoord.y;
			p.y = node->mincoord.y;
		}
		else
		{
			p.y = node->maxcoord.y;
			q.y = node->mincoord.y;
		}

		if (planeStore.z > 0)
		{
			q.z = node->maxcoord.z;
			p.z = node->mincoord.z;
		}
		else
		{
			p.z = node->maxcoord.z;
			q.z = node->mincoord.z;
		}

		XMVECTOR dot = DirectX::XMVector3Dot(plane[i], DirectX::XMLoadFloat3(&q));
		if (DirectX::XMVectorGetX(dot) + planeStore.w < 0)
		{
			result = true;
			break;
		}
	}

	return result;
}

void ULodGenerator::constructNodeBound(QTreeNode *node, XMFLOAT3 *bound)
{
	int innerStep = calInnerStep(node);
	TerrainVertexData *vData[9];

	vData[0] = &mVertices[
		(node->indexY - innerStep)*mRowVertexCount + node->indexX - innerStep];
	vData[1] = &mVertices[
		(node->indexY - innerStep)*mRowVertexCount + node->indexX];
	vData[2]= &mVertices[
		(node->indexY - innerStep)*mRowVertexCount + node->indexX + innerStep];
	vData[3] = &mVertices[
		node->indexY*mRowVertexCount + node->indexX - innerStep];
	vData[4] = &mVertices[
		node->indexY*mRowVertexCount + node->indexX];
	vData[5] = &mVertices[
		node->indexY*mRowVertexCount + node->indexX + innerStep];
	vData[6] = &mVertices[
		(node->indexY + innerStep)*mRowVertexCount + node->indexX - innerStep];
	vData[7] = &mVertices[
		(node->indexY + innerStep)*mRowVertexCount + node->indexX];
	vData[8] = &mVertices[
		(node->indexY + innerStep)*mRowVertexCount + node->indexX + innerStep];


	float minX = 0.f;
	float maxX = 0.f;
	float minY = 0.f;
	float maxY = 0.f;
	float minZ = 0.f;
	float maxZ = 0.f;

	for (int i = 0; i < 9; ++i)
	{
		if (i == 0)
		{
			minX = maxX = vData[i]->position.x;
			minY = maxY = vData[i]->position.y;
			minZ = maxZ = vData[i]->position.z;
		}
		else
		{
			float x = vData[i]->position.x;
			float y = vData[i]->position.y;
			float z = vData[i]->position.z;

			if (x < minX)
				minX = x;
			if (x > maxX)
				maxX = x;

			if (y < minY)
				minY = y;
			if (y > maxY)
				maxY = y;

			if (z < minZ)
				minZ = z;
			if (z > maxZ)
				maxZ = z;
		}
	}

	bound[0].x = minX;
}


void ULodGenerator::renderLodTerrain(
	XMFLOAT3 viewPosition, XMFLOAT4X4 wvpMatrix, int* indexBuffer)
{
	int level = 0;
	mIndexCount = 0;
	QTreeNode* curQueue = nullptr;
	QTreeNode* nextQueue = nullptr;

	QTreeNode* rootNode = mRootQueue;
	mFlag[rootNode->indexY*mRowVertexCount + rootNode->indexX] = 1;
	curQueue = rootNode;

	while (level <= mMaxLevel - 1)
	{
		QTreeNode* iterator = curQueue;
		while (iterator)
		{
			QTreeNode* parent = iterator;
			iterator = iterator->next;

			if (cullNodeWithBound(parent, wvpMatrix))
			{
				int innerStep = calInnerStep(parent);
				for (int i = -innerStep + 1; i < innerStep; ++i)
				{
					::memset(&mFlag[(parent->indexY + i)*mRowVertexCount
						+ parent->indexX - innerStep + 1], -1, innerStep * 2 - 2);
				}
				continue;
			}
			else if (level == mMaxLevel - 1)
			{
				drawNode(parent, indexBuffer);
			}
			else if (checkNodeCanDivide(parent)
				&& assessNodeCanDivide(parent, viewPosition))
			{
				for (int i = 0; i < 4; ++i)
				{
					enqueue(nextQueue, parent->child[i]);

					mFlag[parent->child[i]->indexY*mRowVertexCount
						+ parent->child[i]->indexX] = 1;
				}
			}
			else
			{
				int step = calChildStep(parent);

				mFlag[(parent->indexY - step)*mRowVertexCount
					+ parent->indexX - step] = 0;
				mFlag[(parent->indexY - step)*mRowVertexCount
					+ parent->indexX + step] = 0;
				mFlag[(parent->indexY + step)*mRowVertexCount
					+ parent->indexX - step] = 0;
				mFlag[(parent->indexY + step)*mRowVertexCount
					+ parent->indexX + step] = 0;

				drawNode(parent, indexBuffer);
			}
		}

		curQueue = nextQueue;
		nextQueue = nullptr;

		++level;
	}
}

void ULodGenerator::setCoefficient(float c1, float c2)
{
	COE_ROUGH = c1;
	COE_DISTANCE = c2;
}

int ULodGenerator::getLevel()
{
	return mMaxLevel;
}

float ULodGenerator::getCoef1()
{
	return COE_ROUGH;
}

float ULodGenerator::getCoef2()
{
	return COE_DISTANCE;
}


TerrainVertexData *ULodGenerator::getVertices()
{
	return mVertices;
}

int ULodGenerator::getVertexCount()
{
	return mVertexCount;
}

int ULodGenerator::getRowVertexCount()
{
	return mRowVertexCount;
}

int* ULodGenerator::getIndices()
{
	return mIndices;
}

int ULodGenerator::getIndexCount()
{
	return mIndexCount;
}

int ULodGenerator::getMaxIndexCount()
{
	return mMaxIndexCount;
}