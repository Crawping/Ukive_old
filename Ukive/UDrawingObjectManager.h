#pragma once
#include "UGraphCreator.h"

class UScene;

class UDrawingObjectManager
{
public:
	struct DrawingObject
	{
		DrawingObject();
		~DrawingObject();

		int tag;
		std::wstring name;
		void *vertices;
		ID3D11Buffer *vertexBuffer;
		ID3D11Buffer *indexBuffer;
		UINT vertexCount;
		UINT vertexStructSize;
		UINT vertexDataOffset;
		UINT indexCount;
		UINT materialIndex;
	};

private:
	UScene *mOwnerScene;
	std::list<DrawingObject*> mDrawingObjectList;

public:
	UDrawingObjectManager(UScene *scene);
	~UDrawingObjectManager();

	HRESULT add(void *vertices, int *indices, 
		UINT structSize, UINT vertexCount, UINT indexCount, int tag);
	DrawingObject *getByTag(int tag);
	DrawingObject *getByPos(std::size_t pos);
	std::size_t getCount();
	bool contains(int tag);
	void removeByTag(int tag);
	void removeByPos(std::size_t pos);
};