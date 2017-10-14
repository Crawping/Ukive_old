#pragma once
#include "UScene.h"
#include "UGraphCreator.h"

class TextView;
class UInputEvent;
class UAssistConfigure;
class UModelConfigure;
class UTerrainConfigure;
class USceneManager;
class ULodGenerator;


class TerrainScene : public UScene
{
private:
	TextView *mLodInfoTV;
	ID3D11Buffer* mIndexBuffer;
	ID3D11Buffer* mVertexBuffer;

private:
	int mPrevX;
	int mPrevY;
	int mFrameCounter;
	int mFramePreSecond;
	ULONG64 mPrevTime;

	int mMouseActionMode;
	bool mIsCtrlKeyPressed;
	bool mIsShiftKeyPressed;
	bool mIsMouseLeftKeyPressed;

	const static int MOUSE_ACTION_NONE = 1;
	const static int MOUSE_ACTION_MOVED = 2;
	const static int MOUSE_ACTION_MOVING = 3;

private:
	ULodGenerator *mLodGenerator;
	UAssistConfigure *mAssistConfigure;
	UModelConfigure *mModelConfigure;
	UTerrainConfigure *mTerrainConfigure;

	void updateCube();
	void updateLodTerrain();
	void elementAwareness(int ex, int ey);

public:
	TerrainScene(Direct3DView *d3dView,
		unsigned int width, unsigned int height);
	~TerrainScene();

	void recreate(int level);
	void reevaluate(float c1, float c2);

	virtual void onSceneCreate() override;
	virtual void onSceneResize(UINT width, UINT height) override;
	virtual void onSceneInput(UInputEvent *e) override;
	virtual void onSceneRender() override;
	virtual void onSceneDestroy() override;
};