#pragma once

#define NEAR_PLANE 10.0f
#define FAR_PLANE 100000.0f

class UCamera
{
private:
	XMFLOAT3 mPos;
	XMFLOAT3 mLookAt;
	XMFLOAT3 mUp;
	XMFLOAT3 mRight;
	XMFLOAT3 mLook;
	XMFLOAT3 mZVector;
	XMFLOAT3 mYVector;

	XMFLOAT4X4 mWorldMatrix;
	XMFLOAT4X4 mViewMatrix;
	XMFLOAT4X4 mProjectionMatrix;
	XMFLOAT4X4 mOrthoMatrix;

	float mRadius;
	UINT mWidth;
	UINT mHeight;

public:
	UCamera();
	~UCamera();

	void init(unsigned int wWidth, unsigned int wHeight);
	void close();
	void resize(unsigned int wWidth, unsigned int wHeight);

	void moveCamera(float dx, float dy);
	void scaleCamera(float factor);
	void circuleCamera(float dxAngle, float dyAngle);
	void circuleCamera2(float dxAngle, float dyAngle);
	
	void moveWorld(float dx, float dy);
	void scaleWorld(int direction);
	void rotateWorld(float dxAngle, float dyAngle);

	//该方法有缺陷，请看方法内注释。
	void setCameraPosition(float x,float y,float z);
	//该方法有缺陷，请看方法内注释。
	void setCameraLookAt(float x, float y, float z);

	const XMFLOAT3 *getCameraPos();
	const XMFLOAT3 *getCameraLookAt();
	const XMFLOAT3 *getCameraUp();

	const XMFLOAT4X4 *getWorldMatrix();
	const XMFLOAT4X4 *getViewMatrix();
	const XMFLOAT4X4 *getProjectionMatrix();
	const XMFLOAT4X4 *getOrthoMatrix();

	void getWVPMatrix(XMFLOAT4X4 *wvp);
	void getWVOMatrix(XMFLOAT4X4 *wvo);
};

