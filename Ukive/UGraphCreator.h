#pragma once

struct ModelVertexData;
class UDrawingObjectManager;


class UGraphCreator
{
private:
	UDrawingObjectManager* mDrawingObjectManager;

public:
	UGraphCreator(UDrawingObjectManager* doMgr);
	~UGraphCreator();

	void calculateNormalVector(ModelVertexData *vertices, int vertexCount, int *indices, int indexCount);

	HRESULT putLine(XMFLOAT3 *point1, XMFLOAT3 *point2, int tag);

	HRESULT putCube(int tag, float edgeLength);
	HRESULT putWorldAxis(int tag, float length);
	HRESULT putMark(int tag,XMFLOAT3 *mark, float length);
	HRESULT putBlock(int tag, XMFLOAT3 *posCenter, float radius);
};

