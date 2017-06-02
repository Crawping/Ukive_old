#include "UCommon.h"
#include "URenderer.h"
#include "UScene.h"
#include "UModelConfigure.h"

UModelConfigure::UModelConfigure()
{
}

UModelConfigure::~UModelConfigure()
{
}

HRESULT UModelConfigure::init(UScene *ownerScene)
{
	mOwnerScene = ownerScene;

	UINT numElements;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TEXCOORD";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	std::wstring shaderFileName(::_wgetcwd(nullptr, 0));
	
	RH(mOwnerScene->getRenderer()->
		createVertexShader(shaderFileName + L"\\ModelVertexShader.cso",
			polygonLayout, numElements, &mVertexShader, &mInputLayout));

	RH(mOwnerScene->getRenderer()->
		createPixelShader(shaderFileName + L"\\ModelPixelShader.cso",
			&mPixelShader));

	RH(mOwnerScene->getRenderer()->
		createConstantBuffer(sizeof(MatrixConstBuffer), &mMatrixConstBuffer));

	RH(mOwnerScene->getRenderer()->
		createConstantBuffer(sizeof(PhongLightConstBuffer), &mModelLightConstBuffer));

	return S_OK;
}

void UModelConfigure::active()
{
	mOwnerScene->getRenderer()->setVertexShader(mVertexShader.get());
	mOwnerScene->getRenderer()->setPixelShader(mPixelShader.get());
	mOwnerScene->getRenderer()->setInputLayout(mInputLayout.get());
}

void UModelConfigure::reset()
{
	mOwnerScene->getRenderer()->setVertexShader(mVertexShader.get());
	mOwnerScene->getRenderer()->setPixelShader(mPixelShader.get());
	mOwnerScene->getRenderer()->setInputLayout(mInputLayout.get());
}

void UModelConfigure::close()
{
}

void UModelConfigure::setMatrix(XMFLOAT4X4 matrix)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	resource = mOwnerScene->getRenderer()->lockResource(mMatrixConstBuffer.get());
	((MatrixConstBuffer*)resource.pData)->mWVP = matrix;
	mOwnerScene->getRenderer()->unlockResource(mMatrixConstBuffer.get());

	mOwnerScene->getRenderer()->setConstantBuffers(0, 1, &mMatrixConstBuffer);
}