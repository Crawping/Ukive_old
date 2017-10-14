#include "UCommon.h"
#include "UCamera.h"
#include "UWindow.h"
#include "URenderer.h"
#include "UGraphCreator.h"
#include "UDrawingObjectManager.h"
#include "UDeviceManager.h"
#include "UInputEvent.h"
#include "Direct3DView.h"
#include "UScene.h"

UScene::UScene(Direct3DView *d3dView,
	unsigned int width, unsigned int height)
{
	mWidth = width;
	mHeight = height;
	mD3DView = d3dView;
	mRenderer = mD3DView->getWindow()->getRenderer();

	mDrawingObjectManager = new UDrawingObjectManager(this);

	mGraphCreator = new UGraphCreator(mDrawingObjectManager);

	mCamera = new UCamera();
	mCamera->init(width, height);
}

UScene::~UScene()
{
	delete mDrawingObjectManager;
	mDrawingObjectManager = nullptr;

	delete mGraphCreator;
	mGraphCreator = nullptr;

	delete mCamera;
	mCamera = nullptr;
}


void UScene::refresh()
{
	mD3DView->invalidate();
}


UINT UScene::getSceneWidth()
{
	return mWidth;
}

UINT UScene::getSceneHeight()
{
	return mHeight;
}

void UScene::onSceneCreate()
{
}

void UScene::onSceneInput(UInputEvent *e)
{
}

void UScene::onSceneResize(UINT width, UINT height)
{
	mWidth = width;
	mHeight = height;

	mCamera->resize(width, height);
}

void UScene::onSceneDestroy()
{
}

void UScene::getPickLine(int sx, int sy, XMVECTOR *lineOrig, XMVECTOR *lineDir)
{
	float vx, vy;
	const XMFLOAT4X4 *worldMatrix;
	const XMFLOAT4X4 *viewMatrix;
	const XMFLOAT4X4 *projectionMatrix;

	worldMatrix = mCamera->getWorldMatrix();
	viewMatrix = mCamera->getViewMatrix();
	projectionMatrix = mCamera->getProjectionMatrix();

	vx = (2.0f * sx / mWidth - 1.0f) / projectionMatrix->_11;
	vy = (-2.0f * sy / mHeight + 1.0f) / projectionMatrix->_22;

	XMVECTOR rayDir = DirectX::XMVectorSet(vx, vy, 1.0f, 0);
	XMVECTOR rayOrigin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0);

	XMMATRIX world = DirectX::XMLoadFloat4x4(worldMatrix);
	XMMATRIX view = DirectX::XMLoadFloat4x4(viewMatrix);

	XMMATRIX inverseView = DirectX::XMMatrixInverse(0, view);
	rayDir = DirectX::XMVector3TransformNormal(rayDir, inverseView);
	rayOrigin = DirectX::XMVector3TransformCoord(rayOrigin, inverseView);

	XMMATRIX inverseWorld = DirectX::XMMatrixInverse(0, world);
	rayDir = DirectX::XMVector3TransformNormal(rayDir, inverseWorld);
	rayOrigin = DirectX::XMVector3TransformCoord(rayOrigin, inverseWorld);

	*lineOrig= rayOrigin;
	*lineDir = rayDir;
}

UCamera *UScene::getCamera()
{
	return mCamera;
}

URenderer *UScene::getRenderer()
{
	return mRenderer;
}

UGraphCreator *UScene::getGraphCreator()
{
	return mGraphCreator;
}

UDrawingObjectManager *UScene::getDrawingObjectManager()
{
	return mDrawingObjectManager;
}

Direct3DView *UScene::getWidget()
{
	return mD3DView;
}