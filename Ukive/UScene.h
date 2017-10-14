#pragma once

class UInputEvent;
class UCamera;
class URenderer;
class UGraphCreator;
class UDrawingObjectManager;
class UDeviceManager;
class Direct3DView;

class UScene
{
private:
	int mPrevX;
	int mPrevY;

private:
	unsigned int mWidth;
	unsigned int mHeight;

	UCamera *mCamera;
	URenderer *mRenderer;
	UGraphCreator *mGraphCreator;
	UDrawingObjectManager *mDrawingObjectManager;
	Direct3DView *mD3DView;

public:
	UScene(Direct3DView *d3dView,
		unsigned int width, unsigned int height);
	~UScene();

	void refresh();
	void getPickLine(int sx, int sy, XMVECTOR *lineOrig, XMVECTOR *lineDir);

	virtual void onSceneCreate();
	virtual void onSceneInput(UInputEvent *e);
	virtual void onSceneResize(UINT width, UINT height);
	virtual void onSceneRender() = 0;
	virtual void onSceneDestroy();

	unsigned int getSceneWidth();
	unsigned int getSceneHeight();

	UCamera *getCamera();
	URenderer *getRenderer();
	UGraphCreator *getGraphCreator();
	UDrawingObjectManager *getDrawingObjectManager();
	Direct3DView *getWidget();
};