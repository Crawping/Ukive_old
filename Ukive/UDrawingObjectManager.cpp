#include "UCommon.h"
#include "URenderer.h"
#include "UScene.h"
#include "UModelConfigure.h"
#include "UDrawingObjectManager.h"

UDrawingObjectManager::UDrawingObjectManager(UScene *scene)
{
	mOwnerScene = scene;
}

UDrawingObjectManager::~UDrawingObjectManager()
{
}


HRESULT UDrawingObjectManager::add(void *vertices, int *indices,
	UINT structSize, UINT vertexCount, UINT indexCount, int tag)
{
	if (vertices == 0 || indices == 0 
		|| vertexCount == 0 || indexCount == 0)
		return E_FAIL;

	for (auto it = mDrawingObjectList.begin();
		it != mDrawingObjectList.end(); ++it)
	{
		if ((*it)->tag == tag)
			return E_FAIL;
	}

	DrawingObject *dObject = new DrawingObject;
	dObject->tag = tag;
	dObject->vertices = vertices;
	dObject->vertexCount = vertexCount;
	dObject->vertexStructSize = structSize;
	dObject->indexCount = indexCount;

	RH(mOwnerScene->getRenderer()->createVertexBuffer(
		vertices, dObject->vertexStructSize, vertexCount, dObject->vertexBuffer));
	RH(mOwnerScene->getRenderer()->createIndexBuffer(indices, indexCount, dObject->indexBuffer));

	mDrawingObjectList.push_back(dObject);

	return S_OK;
}

UDrawingObjectManager::DrawingObject *UDrawingObjectManager::getByTag(int tag)
{
	for (auto it = mDrawingObjectList.begin();
		it != mDrawingObjectList.end(); ++it)
	{
		if ((*it)->tag == tag)
			return *it;
	}

	return nullptr;
}

UDrawingObjectManager::DrawingObject *UDrawingObjectManager::getByPos(std::size_t pos)
{
	std::size_t index = 0;

	for (auto it = mDrawingObjectList.begin();
		it != mDrawingObjectList.end(); ++it, ++index)
	{
		if (pos == index)
			return *it;
	}

	return nullptr;
}

std::size_t UDrawingObjectManager::getCount()
{
	return mDrawingObjectList.size();
}

bool UDrawingObjectManager::contains(int tag)
{
	for (auto it = mDrawingObjectList.begin();
		it != mDrawingObjectList.end(); ++it)
	{
		if ((*it)->tag == tag)
			return true;
	}

	return false;
}

void UDrawingObjectManager::removeByTag(int tag)
{
	for (auto it = mDrawingObjectList.begin();
		it != mDrawingObjectList.end(); ++it)
	{
		if ((*it)->tag == tag)
		{
			mDrawingObjectList.erase(it);
			return;
		}
	}
}

void UDrawingObjectManager::removeByPos(std::size_t pos)
{
	std::size_t index = 0;

	for (auto it = mDrawingObjectList.begin();
		it != mDrawingObjectList.end(); ++it, ++index)
	{
		if (pos == index)
		{
			mDrawingObjectList.erase(it);
			return;
		}
	}
}


UDrawingObjectManager::DrawingObject::DrawingObject()
{
	tag = -1;
	name = L"object";
	vertices = nullptr;
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	vertexCount = 0;
	vertexStructSize = 0;
	vertexDataOffset = 0;
	indexCount = 0;
	materialIndex = 0;
}

UDrawingObjectManager::DrawingObject::~DrawingObject()
{
	if (vertices)
		delete[] vertices;
	if (vertexBuffer)
		vertexBuffer->Release();
	if (indexBuffer)
		indexBuffer->Release();
}