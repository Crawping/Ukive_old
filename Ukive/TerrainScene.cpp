#include "UCommon.h"
#include "UTags.h"
#include "UCamera.h"
#include "UWindow.h"
#include "UTextView.h"
#include "URenderer.h"
#include "UInputEvent.h"
#include "UDirect3DView.h"
#include "ULodGenerator.h"
#include "UModelConfigure.h"
#include "UAssistConfigure.h"
#include "UTerrainConfigure.h"
#include "UDrawingObjectManager.h"
#include "USystemClock.h"
#include "TerrainScene.h"

TerrainScene::TerrainScene(UDirect3DView *d3dView,
	unsigned int width, unsigned int height)
	:UScene(d3dView, width, height)
{
	mPrevTime = 0;
	mFrameCounter = 0;
	mFramePreSecond = 0;
	mLodInfoTV = nullptr;
	
	mMouseActionMode = MOUSE_ACTION_NONE;
	mIsCtrlKeyPressed = false;
	mIsShiftKeyPressed = false;
	mIsMouseLeftKeyPressed = false;

	mLodGenerator = new ULodGenerator(8192, 5);

	HRESULT hr = getRenderer()->createVertexBuffer(
		mLodGenerator->getVertices(), sizeof(TerrainVertexData),
		mLodGenerator->getVertexCount(), mVertexBuffer);
	if (FAILED(hr))
		throw std::runtime_error("");

	hr = getRenderer()->createIndexBuffer(
		mLodGenerator->getIndices(),
		mLodGenerator->getMaxIndexCount(), mIndexBuffer);
	if (FAILED(hr))
		throw std::runtime_error("");
}

TerrainScene::~TerrainScene()
{
	mIndexBuffer->Release();
	mVertexBuffer->Release();
	delete mLodGenerator;
}


void TerrainScene::updateCube()
{
	auto *object = this->getDrawingObjectManager()
		->getByTag(U3DGraphId::NORMAL_CUBE);
	if (object != nullptr)
	{
		D3D11_MAPPED_SUBRESOURCE source = getRenderer()->lockResource(object->vertexBuffer);
		if (source.pData != nullptr)
		{
			auto *modelVertexData = (ModelVertexData*)source.pData;

			((ModelVertexData*)object->vertices)[0].position.x += 0.1f;

			for (unsigned int i = 0; i < object->vertexCount; ++i)
			{
				modelVertexData[i].position = ((ModelVertexData*)object->vertices)[i].position;
				modelVertexData[i].color = ((ModelVertexData*)object->vertices)[i].color;
				modelVertexData[i].normal = ((ModelVertexData*)object->vertices)[i].normal;
				modelVertexData[i].texcoord = ((ModelVertexData*)object->vertices)[i].texcoord;
			}

			getRenderer()->unlockResource(object->vertexBuffer);
		}
	}
}

void TerrainScene::updateLodTerrain()
{
	D3D11_MAPPED_SUBRESOURCE source
		= getRenderer()->lockResource(mIndexBuffer);
	if (source.pData != nullptr)
	{
		int* indices = (int*)source.pData;

		const XMFLOAT3 *vPosition = getCamera()->getCameraPos();

		XMFLOAT4X4 wvpMatrix;
		getCamera()->getWVPMatrix(&wvpMatrix);

		mLodGenerator->renderLodTerrain(*vPosition, wvpMatrix, indices);

		getRenderer()->unlockResource(mIndexBuffer);
	}
}

void TerrainScene::elementAwareness(int ex, int ey)
{
	XMVECTOR ori;
	XMVECTOR dir;
	getPickLine(ex, ey, &ori, &dir);

	bool isHitVer = false;
	XMVECTOR vPos;
	UDrawingObjectManager::DrawingObject *object
		= getDrawingObjectManager()->getByTag(U3DGraphId::NORMAL_CUBE);
	if (object)
	{
		ModelVertexData *vData = (ModelVertexData*)object->vertices;
		for (unsigned int i = 0; i < object->vertexCount; ++i)
		{
			bool hit = false;
			XMVECTOR pos = DirectX::XMLoadFloat3(&vData[i].position);
			XMVECTOR distance = DirectX::XMVector3LinePointDistance(
				ori, DirectX::XMVectorAdd(ori, dir), pos);
			if (DirectX::XMVectorGetX(distance) < 20.f)
			{
				vPos = pos;
				isHitVer = true;
			}
		}

		if (isHitVer)
		{
			//getGraphCreator()->putBlock(155, &vPos, 10.f);
			refresh();
		}
		else
		{
			if (getDrawingObjectManager()->contains(155))
			{
				getDrawingObjectManager()->removeByTag(155);
				refresh();
			}
		}
	}
}


void TerrainScene::recreate(int level)
{
	if (level > 0 && level != mLodGenerator->getLevel())
	{
		mIndexBuffer->Release();
		mVertexBuffer->Release();
		delete mLodGenerator;

		mLodGenerator = new ULodGenerator(8192, level);

		HRESULT hr = getRenderer()->createVertexBuffer(
			mLodGenerator->getVertices(), sizeof(TerrainVertexData),
			mLodGenerator->getVertexCount(), mVertexBuffer);
		if (FAILED(hr))
			throw std::runtime_error("");

		hr = getRenderer()->createIndexBuffer(
			mLodGenerator->getIndices(),
			mLodGenerator->getMaxIndexCount(), mIndexBuffer);
		if (FAILED(hr))
			throw std::runtime_error("");
	}
}

void TerrainScene::reevaluate(float c1, float c2)
{
	mLodGenerator->setCoefficient(c1, c2);
	updateLodTerrain();
}


void TerrainScene::onSceneCreate()
{
	UScene::onSceneCreate();

	getCamera()->setCameraPosition(1024, 1024, -1024);
	getCamera()->circuleCamera2(1.f, -0.2f);

	mAssistConfigure = new UAssistConfigure();
	HRESULT hr = mAssistConfigure->init(this);
	if (FAILED(hr))
		throw std::runtime_error("");

	mModelConfigure = new UModelConfigure();
	hr = mModelConfigure->init(this);
	if (FAILED(hr))
		throw std::runtime_error("");

	mTerrainConfigure = new UTerrainConfigure();
	hr = mTerrainConfigure->init(this);
	if (FAILED(hr))
		throw std::runtime_error("");

	hr = getGraphCreator()->putWorldAxis(U3DGraphId::WORLD_AXIS, 1024);
	if (FAILED(hr))
		throw std::runtime_error("");

	updateLodTerrain();
}


void TerrainScene::onSceneResize(UINT width, UINT height)
{
	UScene::onSceneResize(width, height);

	updateLodTerrain();
}


void TerrainScene::onSceneInput(UInputEvent *e)
{
	UScene::onSceneInput(e);

	switch (e->getEvent())
	{
	case UInputEvent::EVENT_MOUSE_KEY_DOWN:
		if (e->getMouseKey() == UInputEvent::KEY_MOUSE_LEFT)
		{
			mIsMouseLeftKeyPressed = true;
			mPrevX = e->getMouseX();
			mPrevY = e->getMouseY();
		}
		break;

	case UInputEvent::EVENT_MOUSE_MOVE:
		if (mIsMouseLeftKeyPressed == true)
		{
			mMouseActionMode = MOUSE_ACTION_MOVED;

			int dx = e->getMouseX() - mPrevX;
			int dy = e->getMouseY() - mPrevY;

			if (::GetKeyState(VK_SHIFT) < 0)
			{
				getCamera()->circuleCamera2(
					(float)-dx / this->getSceneWidth(),
					(float)-dy / this->getSceneHeight());

				updateLodTerrain();
				refresh();
			}
			else if (::GetKeyState(VK_CONTROL) < 0)
			{
				getCamera()->moveCamera((float)-dx, (float)dy);

				updateLodTerrain();
				refresh();
			}

			mPrevX = e->getMouseX();
			mPrevY = e->getMouseY();
		}
		else
			elementAwareness(e->getMouseX(), e->getMouseY());
		break;

	case UInputEvent::EVENT_MOUSE_KEY_UP:
		if (e->getMouseKey() == UInputEvent::KEY_MOUSE_LEFT)
		{
			if (mMouseActionMode != MOUSE_ACTION_MOVED)
				updateCube();

			mIsMouseLeftKeyPressed = false;
		}

		mMouseActionMode = MOUSE_ACTION_NONE;
		break;

	case UInputEvent::EVENT_KEYBOARD_KEY_DOWN:
		if (e->getKeyboardKey() == VK_SHIFT)
			mIsShiftKeyPressed = true;
		if (e->getKeyboardKey() == VK_CONTROL)
			mIsCtrlKeyPressed = true;
		break;

	case UInputEvent::EVENT_KEYBOARD_KEY_UP:
		if (e->getKeyboardKey() == VK_SHIFT)
			mIsShiftKeyPressed = false;
		if (e->getKeyboardKey() == VK_CONTROL)
			mIsCtrlKeyPressed = false;
		break;

	case UInputEvent::EVENT_MOUSE_WHEEL:
		this->getCamera()->scaleCamera(e->getMouseWheel() > 0 ? 0.9f : 1.1f);
		updateLodTerrain();
		refresh();
		break;
	}
}

void TerrainScene::onSceneRender()
{
	ULONG64 currentTime = USystemClock::upTimeMillis();
	if (mPrevTime > 0)
	{
		++mFrameCounter;
		if (currentTime - mPrevTime > 500)
		{
			mFramePreSecond = (int)(((double)mFrameCounter / (currentTime - mPrevTime))*1000);
			mFrameCounter = 0;
			mPrevTime = currentTime;
		}
	}
	else
		mPrevTime = currentTime;

	std::wstringstream ss;
	ss << "FPS: " << mFramePreSecond
		<< "\nTerrain Size: " << mLodGenerator->getRowVertexCount() << "x" << mLodGenerator->getRowVertexCount()
		<< "\nTriangle Count: " << mLodGenerator->getMaxIndexCount() / 3
		<< "\nRendered Triangle Count: " << mLodGenerator->getIndexCount() / 3;

	if (mLodInfoTV)
		mLodInfoTV->setText(ss.str());
	else
		mLodInfoTV = (UTextView*)getWidget()->getWindow()->findWidgetById(0x010);


	XMFLOAT4X4 wvpMatrix;
	XMFLOAT4X4 wvpMatrixTransposed;

	getCamera()->getWVPMatrix(&wvpMatrix);
	DirectX::XMStoreFloat4x4(&wvpMatrixTransposed, DirectX::XMMatrixTranspose(
		DirectX::XMLoadFloat4x4(&wvpMatrix)));

	mAssistConfigure->active();
	mAssistConfigure->setMatrix(wvpMatrixTransposed);

	getRenderer()->setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	getRenderer()->drawObjects(getDrawingObjectManager()->getByTag(U3DGraphId::WORLD_AXIS));

	mTerrainConfigure->active();
	mTerrainConfigure->setMatrix(wvpMatrixTransposed);

	getRenderer()->setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	getRenderer()->draw(mVertexBuffer, mIndexBuffer, sizeof(TerrainVertexData), mLodGenerator->getIndexCount());

	//getRenderer()->setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//getRenderer()->drawObjects(getDrawingObjectManager()->getByTag(155));

	//mModelLightEngine->active();
	//mModelLightEngine->setMatrix(wvpMatrixTransposed);

	//getRenderer()->setPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//getRenderer()->drawObjects(getDrawingObjectManager()->getByTag(U3DGraphId::NORMAL_CUBE));
}

void TerrainScene::onSceneDestroy()
{
	UScene::onSceneDestroy();

	if (mAssistConfigure)
	{
		mAssistConfigure->close();
		delete mAssistConfigure;
		mAssistConfigure = nullptr;
	}

	if (mModelConfigure)
	{
		mModelConfigure->close();
		delete mModelConfigure;
		mModelConfigure = nullptr;
	}

	if (mTerrainConfigure)
	{
		mTerrainConfigure->close();
		delete mTerrainConfigure;
		mTerrainConfigure = nullptr;
	}
}