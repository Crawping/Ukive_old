﻿#include "UCommon.h"
#include "UDrawingObjectManager.h"
#include "UModelConfigure.h"
#include "UAssistConfigure.h"
#include "UGraphCreator.h"

UGraphCreator::UGraphCreator(UDrawingObjectManager* doMgr)
{
	mDrawingObjectManager = doMgr;
}

UGraphCreator::~UGraphCreator()
{
}

void UGraphCreator::calculateNormalVector(
	ModelVertexData *vertices, int vertexCount, int *indices, int indexCount)
{
	int triangleCount = indexCount / 3;

	XMVECTOR e0;
	XMVECTOR e1;
	XMVECTOR faceNormal;

	for (int i = 0; i < triangleCount; ++i)
	{
		int i0 = indices[i * 3 + 0];
		int i1 = indices[i * 3 + 1];
		int i2 = indices[i * 3 + 2];

		XMVECTOR i0Vec 
			= DirectX::XMLoadFloat3(&vertices[i0].position);

		e0 = DirectX::XMVectorSubtract(
			DirectX::XMLoadFloat3(&vertices[i1].position),
			i0Vec);

		e1 = DirectX::XMVectorSubtract(
			DirectX::XMLoadFloat3(&vertices[i2].position),
			i0Vec);

		faceNormal = DirectX::XMVector3Cross(e0, e1);

		DirectX::XMStoreFloat3(
			&vertices[i0].normal,
			DirectX::XMVectorAdd(
				DirectX::XMLoadFloat3(&vertices[i0].normal),
				faceNormal));
		DirectX::XMStoreFloat3(
			&vertices[i1].normal,
			DirectX::XMVectorAdd(
				DirectX::XMLoadFloat3(&vertices[i1].normal),
				faceNormal));
		DirectX::XMStoreFloat3(
			&vertices[i2].normal,
			DirectX::XMVectorAdd(
				DirectX::XMLoadFloat3(&vertices[i2].normal),
				faceNormal));
	}

	for (int i = 0; i < vertexCount; ++i)
	{
		XMVECTOR normalVec = DirectX::XMLoadFloat3(&vertices[i].normal);
		DirectX::XMStoreFloat3(
			&vertices[i].normal, 
			DirectX::XMVector3Normalize(normalVec));
	}
}


HRESULT UGraphCreator::putWorldAxis(int tag, float length)
{
	int *indices;
	UINT vertexCount = 6;
	UINT indexCount = 6;
	AssistVertexData *vertexData;

	if (length < 10.0f)
	{
		length = 10.0f;
	}

	vertexData = new AssistVertexData[vertexCount];
	vertexData[0].position = XMFLOAT3(-length, 0, 0);
	vertexData[0].color = XMFLOAT4(1, 0, 0, 1);
	vertexData[1].position = XMFLOAT3(length, 0, 0);
	vertexData[1].color = XMFLOAT4(1, 0, 0, 1);

	vertexData[2].position = XMFLOAT3(0, -length, 0);
	vertexData[2].color = XMFLOAT4(0, 1, 0, 1);
	vertexData[3].position = XMFLOAT3(0, length, 0);
	vertexData[3].color = XMFLOAT4(0, 1, 0, 1);

	vertexData[4].position = XMFLOAT3(0, 0, -length);
	vertexData[4].color = XMFLOAT4(0, 0, 1, 1);
	vertexData[5].position = XMFLOAT3(0, 0, length);
	vertexData[5].color = XMFLOAT4(0, 0, 1, 1);

	indices = new int[indexCount]
	{
		0, 1, 2, 3, 4, 5
	};

	RH(mDrawingObjectManager->add(
		vertexData, indices, sizeof(AssistVertexData), vertexCount, indexCount, tag));

	return S_OK;
}

HRESULT UGraphCreator::putLine(XMFLOAT3 *point1, XMFLOAT3 *point2, int tag)
{
	int *indices;
	UINT vertexCount = 2;
	UINT indexCount = 2;
	AssistVertexData *vertexData;

	vertexData = new AssistVertexData[vertexCount];
	vertexData[0].position = *point1;
	vertexData[0].color = XMFLOAT4(0.5f, 0, 0, 1);
	vertexData[1].position = *point2;
	vertexData[1].color = XMFLOAT4(0.5f, 0, 0, 1);

	indices = new int[indexCount]
	{
		0, 1
	};

	RH(mDrawingObjectManager->add(
		vertexData, indices, sizeof(AssistVertexData), vertexCount, indexCount, tag));

	return S_OK;
}

HRESULT UGraphCreator::putMark(int tag, DirectX::XMFLOAT3 *mark, float length)
{
	int *indices;
	UINT vertexCount = 4;
	UINT indexCount = 4;
	AssistVertexData *vertexData;

	vertexData = new AssistVertexData[vertexCount];
	vertexData[0].position = XMFLOAT3(mark->x - length, 0, mark->z);
	vertexData[0].color = XMFLOAT4(0.5f, 0, 0, 1);
	vertexData[1].position = XMFLOAT3(mark->x + length, 0, mark->z);
	vertexData[1].color = XMFLOAT4(0.5f, 0, 0, 1);
	vertexData[2].position = XMFLOAT3(mark->x, 0, mark->z - length);
	vertexData[2].color = XMFLOAT4(0.5f, 0, 0, 1);
	vertexData[3].position = XMFLOAT3(mark->x, 0, mark->z + length);
	vertexData[3].color = XMFLOAT4(0.5f, 0, 0, 1);

	indices = new int[indexCount]
	{
		0, 1, 2, 3
	};

	RH(mDrawingObjectManager->add(
		vertexData, indices, sizeof(AssistVertexData), vertexCount, indexCount, tag));

	return S_OK;
}

HRESULT UGraphCreator::putBlock(int tag, XMFLOAT3 *posCenter, float radius)
{
	int *indices;
	UINT vertexCount = 8;
	UINT indexCount = 36;
	AssistVertexData *vertexData;

	vertexData = new AssistVertexData[vertexCount];
	vertexData[0].position = XMFLOAT3(
		posCenter->x - radius, posCenter->y - radius, posCenter->z - radius);
	vertexData[0].color = XMFLOAT4(1, 1, 0, 1);

	vertexData[1].position = XMFLOAT3(
		posCenter->x + radius, posCenter->y - radius, posCenter->z - radius);
	vertexData[1].color = XMFLOAT4(1, 1, 0, 1);

	vertexData[2].position = XMFLOAT3(
		posCenter->x + radius, posCenter->y - radius, posCenter->z + radius);
	vertexData[2].color = XMFLOAT4(1, 1, 0, 1);

	vertexData[3].position = XMFLOAT3(
		posCenter->x - radius, posCenter->y - radius, posCenter->z + radius);
	vertexData[3].color = XMFLOAT4(1, 1, 0, 1);

	vertexData[4].position = XMFLOAT3(
		posCenter->x - radius, posCenter->y + radius, posCenter->z + radius);
	vertexData[4].color = XMFLOAT4(1, 1, 0, 1);

	vertexData[5].position = XMFLOAT3(
		posCenter->x - radius, posCenter->y + radius, posCenter->z - radius);
	vertexData[5].color = XMFLOAT4(1, 1, 0, 1);

	vertexData[6].position = XMFLOAT3(
		posCenter->x + radius, posCenter->y + radius, posCenter->z - radius);
	vertexData[6].color = XMFLOAT4(1, 1, 0, 1);

	vertexData[7].position = XMFLOAT3(
		posCenter->x + radius, posCenter->y + radius, posCenter->z + radius);
	vertexData[7].color = XMFLOAT4(1, 1, 0, 1);

	indices = new int[indexCount]
	{
		0, 5, 1, 1, 5, 6, 1, 6, 2, 2, 6, 7, 2, 7, 3, 3, 7, 4, 3, 4, 0, 0, 4, 5, 3, 0, 2, 2, 0, 1, 5, 4, 6, 6, 4, 7
	};

	RH(mDrawingObjectManager->add(
		vertexData, indices, sizeof(AssistVertexData), vertexCount, indexCount, tag));

	return S_OK;
}

HRESULT UGraphCreator::putCube(int tag, float edgeLength)
{
	int *indices;
	UINT vertexCount = 8;
	UINT indexCount = 36;
	ModelVertexData *modelVertexData;

	float half = edgeLength / 2.f;

	modelVertexData = new ModelVertexData[vertexCount];
	modelVertexData[0].position = XMFLOAT3(-half, -half, -half);
	modelVertexData[0].color = XMFLOAT4(1, 0, 0, 1);
	modelVertexData[0].texcoord = XMFLOAT2(0, 1);

	modelVertexData[1].position = XMFLOAT3(half, -half, -half);
	modelVertexData[1].color = XMFLOAT4(1, 0, 0, 1);
	modelVertexData[1].texcoord = XMFLOAT2(1, 1);

	modelVertexData[2].position = XMFLOAT3(half, -half, half);
	modelVertexData[2].color = XMFLOAT4(0, 1, 0, 1);
	modelVertexData[2].texcoord = XMFLOAT2(1, 0);

	modelVertexData[3].position = XMFLOAT3(-half, -half, half);
	modelVertexData[3].color = XMFLOAT4(0, 1, 0, 1);
	modelVertexData[3].texcoord = XMFLOAT2(0, 0);

	modelVertexData[4].position = XMFLOAT3(-half, half, half);
	modelVertexData[4].color = XMFLOAT4(0, 0, 1, 1);
	modelVertexData[4].texcoord = XMFLOAT2(0, 0);

	modelVertexData[5].position = XMFLOAT3(-half, half, -half);
	modelVertexData[5].color = XMFLOAT4(0, 0, 1, 1);
	modelVertexData[5].texcoord = XMFLOAT2(0, 1);

	modelVertexData[6].position = XMFLOAT3(half, half, -half);
	modelVertexData[6].color = XMFLOAT4(0, 0, 1, 1);
	modelVertexData[6].texcoord = XMFLOAT2(1, 1);

	modelVertexData[7].position = XMFLOAT3(half, half, half);
	modelVertexData[7].color = XMFLOAT4(0, 0, 1, 1);
	modelVertexData[7].texcoord = XMFLOAT2(1, 0);

	indices = new int[indexCount]
	{
		0, 5, 1,
			1, 5, 6,
			1, 6, 2,
			2, 6, 7,
			2, 7, 3,
			3, 7, 4,
			3, 4, 0,
			0, 4, 5,
			3, 0, 2,
			2, 0, 1,
			5, 4, 6,
			6, 4, 7
	};

	calculateNormalVector(modelVertexData, vertexCount, indices, indexCount);

	RH(mDrawingObjectManager->add(
		modelVertexData, indices, sizeof(ModelVertexData), vertexCount, indexCount, tag));

	return S_OK;
}