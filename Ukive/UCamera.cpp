#include "UCommon.h"
#include "UCamera.h"

UCamera::UCamera()
{
	mWidth = 1;
	mHeight = 1;
}

UCamera::~UCamera()
{
}

void UCamera::init(unsigned int wWidth, unsigned int wHeight)
{
	mWidth = wWidth;
	mHeight = wHeight;

	//摄像机位置。
	mPos = XMFLOAT3(0.0f, 0.0f, -10.0f);
	//摄像机看向的位置。
	mLookAt = XMFLOAT3(0.0f, 0.0f, 0.0f);

	mUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	mRight = XMFLOAT3(1.0f, 0.0f, 0.0f);
	mLook = XMFLOAT3(0.0f, 0.0f, 1.0f);

	mZVector = XMFLOAT3(0.0f, 0.0f, 1.0f);
	mYVector = XMFLOAT3(0.0f, 1.0f, 0.0f);

	XMVECTOR upVec = DirectX::XMLoadFloat3(&mUp);
	XMVECTOR posVec = DirectX::XMLoadFloat3(&mPos);
	XMVECTOR lookAtVec = DirectX::XMLoadFloat3(&mLookAt);

	DirectX::XMStoreFloat(&mRadius, DirectX::XMVector3Length(DirectX::XMVectorSubtract(lookAtVec, posVec)));

	DirectX::XMStoreFloat4x4(&mWorldMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mViewMatrix, DirectX::XMMatrixLookAtLH(posVec, lookAtVec, upVec));

	DirectX::XMStoreFloat4x4(&mProjectionMatrix,
		DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, (float)mWidth / mHeight, NEAR_PLANE, FAR_PLANE));
	DirectX::XMStoreFloat4x4(&mOrthoMatrix,
		DirectX::XMMatrixOrthographicLH((float)mWidth, (float)mHeight, NEAR_PLANE, FAR_PLANE));
}

void UCamera::close()
{
}

void UCamera::resize(unsigned int wWidth, unsigned int wHeight)
{
	mWidth = wWidth;
	mHeight = wHeight;

	DirectX::XMStoreFloat4x4(&mProjectionMatrix,
		DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, (float)mWidth / mHeight, NEAR_PLANE, FAR_PLANE));

	DirectX::XMStoreFloat4x4(&mOrthoMatrix,
		DirectX::XMMatrixOrthographicLH((float)mWidth, (float)mHeight, NEAR_PLANE, FAR_PLANE));
}


void UCamera::moveCamera(float dx, float dy)
{
	XMVECTOR frontVec = DirectX::XMVectorSet(mLook.x, 0, mLook.z, 0);
	frontVec = DirectX::XMVector3Normalize(frontVec);

	XMVECTOR upVec = DirectX::XMLoadFloat3(&mUp);
	XMVECTOR rightVec = DirectX::XMLoadFloat3(&mRight);
	XMVECTOR posVec = DirectX::XMLoadFloat3(&mPos);
	XMVECTOR lookAtVec = DirectX::XMLoadFloat3(&mLookAt);

	XMVECTOR deltaVec = DirectX::XMVectorAdd(
		DirectX::XMVectorScale(frontVec, dy),
		DirectX::XMVectorScale(rightVec, dx));

	posVec = DirectX::XMVectorAdd(posVec, deltaVec);
	lookAtVec = DirectX::XMVectorAdd(lookAtVec, deltaVec);

	DirectX::XMStoreFloat4x4(&mViewMatrix,
		DirectX::XMMatrixLookAtLH(posVec, lookAtVec, upVec));

	DirectX::XMStoreFloat3(&mPos, posVec);
	DirectX::XMStoreFloat3(&mLookAt, lookAtVec);
}

void UCamera::scaleCamera(float factor)
{
	XMVECTOR upVec = DirectX::XMLoadFloat3(&mUp);
	XMVECTOR lookVec = DirectX::XMLoadFloat3(&mLook);
	XMVECTOR lookAtVec = DirectX::XMLoadFloat3(&mLookAt);

	mRadius *= factor;

	XMVECTOR posVec = DirectX::XMVectorAdd(lookAtVec,
		DirectX::XMVectorScale(lookVec, -mRadius));

	DirectX::XMStoreFloat4x4(&mViewMatrix,
		DirectX::XMMatrixLookAtLH(posVec, lookAtVec, upVec));

	DirectX::XMStoreFloat3(&mPos, posVec);
}

void UCamera::circuleCamera(float dxAngle, float dyAngle)
{
	XMVECTOR yVec = DirectX::XMLoadFloat3(&mYVector);
	XMVECTOR upVec = DirectX::XMLoadFloat3(&mUp);
	XMVECTOR rightVec = DirectX::XMLoadFloat3(&mRight);
	XMVECTOR posVec = DirectX::XMLoadFloat3(&mPos);
	XMVECTOR lookAtVec = DirectX::XMLoadFloat3(&mLookAt);

	XMMATRIX rotateMatrix
		= DirectX::XMMatrixRotationAxis(yVec, dxAngle);
	posVec = DirectX::XMVector3TransformCoord(posVec, rotateMatrix);
	rightVec = DirectX::XMVector3TransformNormal(rightVec, rotateMatrix);

	rotateMatrix = DirectX::XMMatrixRotationAxis(rightVec, dyAngle);
	posVec = DirectX::XMVector3TransformCoord(posVec, rotateMatrix);

	XMVECTOR lookVec 
		= DirectX::XMVectorSubtract(lookAtVec, posVec);

	upVec = DirectX::XMVector3Cross(lookVec, rightVec);
	upVec = DirectX::XMVector3Normalize(upVec);
	lookVec = DirectX::XMVector3Normalize(lookVec);

	DirectX::XMStoreFloat4x4(&mViewMatrix,
		DirectX::XMMatrixLookAtLH(posVec, lookAtVec, upVec));

	DirectX::XMStoreFloat3(&mUp, upVec);
	DirectX::XMStoreFloat3(&mRight, rightVec);
	DirectX::XMStoreFloat3(&mPos, posVec);
	DirectX::XMStoreFloat3(&mLook, lookVec);
}

void UCamera::circuleCamera2(float dxAngle, float dyAngle)
{
	XMVECTOR yVec = DirectX::XMLoadFloat3(&mYVector);
	XMVECTOR rightVec = DirectX::XMLoadFloat3(&mRight);
	XMVECTOR posVec = DirectX::XMLoadFloat3(&mPos);
	XMVECTOR lookVec = DirectX::XMLoadFloat3(&mLook);

	XMMATRIX rotateMatrix
		= DirectX::XMMatrixRotationAxis(yVec, dxAngle);
	lookVec = DirectX::XMVector3TransformCoord(lookVec, rotateMatrix);
	rightVec = DirectX::XMVector3TransformNormal(rightVec, rotateMatrix);

	XMVECTOR lookAtVec
		= DirectX::XMVectorAdd(posVec,
			DirectX::XMVectorScale(lookVec, mRadius));

	rotateMatrix
		= DirectX::XMMatrixRotationAxis(rightVec, dyAngle);
	lookVec = DirectX::XMVector3TransformCoord(lookVec, rotateMatrix);

	lookAtVec = DirectX::XMVectorAdd(posVec,
		DirectX::XMVectorScale(lookVec, mRadius));

	XMVECTOR upVec = DirectX::XMVector3Cross(lookVec, rightVec);
	upVec = DirectX::XMVector3Normalize(upVec);
	lookVec = DirectX::XMVector3Normalize(lookVec);

	DirectX::XMStoreFloat4x4(&mViewMatrix,
		DirectX::XMMatrixLookAtLH(posVec, lookAtVec, upVec));

	DirectX::XMStoreFloat3(&mUp, upVec);
	DirectX::XMStoreFloat3(&mRight, rightVec);
	DirectX::XMStoreFloat3(&mLook, lookVec);
	DirectX::XMStoreFloat3(&mLookAt, lookAtVec);
}


void UCamera::moveWorld(float dx, float dy)
{
	XMVECTOR frontVec
		= DirectX::XMVectorSet(mLook.x, 0, mLook.z, 0);
	frontVec = DirectX::XMVector3Normalize(frontVec);
	frontVec = DirectX::XMVectorScale(frontVec, dy);

	XMVECTOR rightVec 
		= DirectX::XMLoadFloat3(&mRight);
	rightVec = DirectX::XMVectorScale(rightVec, dx);

	XMMATRIX transMatrix
		= DirectX::XMMatrixTranslation(
			DirectX::XMVectorGetX(frontVec) + DirectX::XMVectorGetX(rightVec),
			0, DirectX::XMVectorGetZ(frontVec) + DirectX::XMVectorGetZ(rightVec));

	XMMATRIX worldMatrix
		= DirectX::XMLoadFloat4x4(&mWorldMatrix);
	DirectX::XMMatrixMultiply(worldMatrix, transMatrix);
	DirectX::XMStoreFloat4x4(&mWorldMatrix, worldMatrix);
}

void UCamera::scaleWorld(int direction)
{
	float scaleFactor;

	if (direction > 0)
		scaleFactor = 1.1f;
	else
		scaleFactor = 0.9f;

	XMMATRIX scaleMatrix 
		= DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor);

	XMMATRIX worldMatrix
		= DirectX::XMLoadFloat4x4(&mWorldMatrix);
	DirectX::XMMatrixMultiply(worldMatrix, scaleMatrix);
	DirectX::XMStoreFloat4x4(&mWorldMatrix, worldMatrix);
}

void UCamera::rotateWorld(float dxAngle, float dyAngle)
{
	XMMATRIX rotateMatrix
		= DirectX::XMMatrixRotationY(dxAngle);

	XMMATRIX worldMatrix
		= DirectX::XMLoadFloat4x4(&mWorldMatrix);
	DirectX::XMMatrixMultiply(worldMatrix, rotateMatrix);
	DirectX::XMStoreFloat4x4(&mWorldMatrix, worldMatrix);
}

void UCamera::setCameraPosition(float x, float y, float z)
{
	XMVECTOR upVec = DirectX::XMLoadFloat3(&mUp);
	XMVECTOR lookAtVec = DirectX::XMLoadFloat3(&mLookAt);
	XMVECTOR posVec = DirectX::XMVectorSet(x, y, z, 0);
	XMVECTOR lookVec = DirectX::XMVectorSubtract(lookAtVec, posVec);

	DirectX::XMStoreFloat(&mRadius, DirectX::XMVector3Length(lookVec));

	//TODO:此处假设在变换前后up不变。当up在变换后改变，需要额外变量确定right。
	XMVECTOR rightVec = DirectX::XMVector3Cross(upVec, lookVec);
	upVec = DirectX::XMVector3Cross(lookVec, rightVec);

	upVec = DirectX::XMVector3Normalize(upVec);
	lookVec = DirectX::XMVector3Normalize(lookVec);
	rightVec = DirectX::XMVector3Normalize(rightVec);

	DirectX::XMStoreFloat4x4(&mViewMatrix,
		DirectX::XMMatrixLookAtLH(posVec, lookAtVec, upVec));

	DirectX::XMStoreFloat3(&mUp, upVec);
	DirectX::XMStoreFloat3(&mRight, rightVec);
	DirectX::XMStoreFloat3(&mPos, posVec);
	DirectX::XMStoreFloat3(&mLook, lookVec);
}

void UCamera::setCameraLookAt(float x, float y, float z)
{
	XMVECTOR upVec = DirectX::XMLoadFloat3(&mUp);
	XMVECTOR posVec = DirectX::XMLoadFloat3(&mPos);
	XMVECTOR lookAtVec = DirectX::XMVectorSet(x, y, z, 0);
	XMVECTOR lookVec = DirectX::XMVectorSubtract(lookAtVec, posVec);

	DirectX::XMStoreFloat(&mRadius, DirectX::XMVector3Length(lookVec));

	//TODO:此处假设在变换前后up不变。当up在变换后改变，需要额外变量确定right。
	XMVECTOR rightVec = DirectX::XMVector3Cross(upVec, lookVec);
	upVec = DirectX::XMVector3Cross(lookVec, rightVec);

	upVec = DirectX::XMVector3Normalize(upVec);
	lookVec = DirectX::XMVector3Normalize(lookVec);
	rightVec = DirectX::XMVector3Normalize(rightVec);

	DirectX::XMStoreFloat4x4(&mViewMatrix,
		DirectX::XMMatrixLookAtLH(posVec, lookAtVec, upVec));

	DirectX::XMStoreFloat3(&mUp, upVec);
	DirectX::XMStoreFloat3(&mRight, rightVec);
	DirectX::XMStoreFloat3(&mLook, lookVec);
	DirectX::XMStoreFloat3(&mLookAt, lookVec);
}

const XMFLOAT3 *UCamera::getCameraPos()
{
	return &mPos;
}

const XMFLOAT3 *UCamera::getCameraLookAt()
{
	return &mLookAt;
}

const XMFLOAT3 *UCamera::getCameraUp()
{
	return &mUp;
}

const XMFLOAT4X4 *UCamera::getWorldMatrix()
{
	return &mWorldMatrix;
}

const XMFLOAT4X4 *UCamera::getViewMatrix()
{
	return &mViewMatrix;
}

const XMFLOAT4X4 *UCamera::getProjectionMatrix()
{
	return &mProjectionMatrix;
}

const XMFLOAT4X4 *UCamera::getOrthoMatrix()
{
	return &mOrthoMatrix;
}

void UCamera::getWVPMatrix(XMFLOAT4X4 *wvp)
{
	XMMATRIX world = DirectX::XMLoadFloat4x4(&mWorldMatrix);
	XMMATRIX view = DirectX::XMLoadFloat4x4(&mViewMatrix);
	XMMATRIX projection = DirectX::XMLoadFloat4x4(&mProjectionMatrix);

	DirectX::XMStoreFloat4x4(wvp,
		DirectX::XMMatrixMultiply(
			DirectX::XMMatrixMultiply(world, view), projection));
}

void UCamera::getWVOMatrix(XMFLOAT4X4 *wvo)
{
	XMMATRIX world = DirectX::XMLoadFloat4x4(&mWorldMatrix);
	XMMATRIX view = DirectX::XMLoadFloat4x4(&mViewMatrix);
	XMMATRIX ortho = DirectX::XMLoadFloat4x4(&mOrthoMatrix);

	DirectX::XMStoreFloat4x4(wvo,
		DirectX::XMMatrixMultiply(
			DirectX::XMMatrixMultiply(world, view), ortho));
}