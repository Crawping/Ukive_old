#include "UCommon.h"
#include "URenderer.h"
#include "UScene.h"
#include "UAssistConfigure.h"

UAssistConfigure::UAssistConfigure()
{
}

UAssistConfigure::~UAssistConfigure()
{
}

HRESULT UAssistConfigure::init(UScene *ownerScene)
{
	mOwnerScene = ownerScene;

	UINT numElements;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];

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

	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	std::wstring shaderFileName(::_wgetcwd(nullptr, 0));

	RH(mOwnerScene->getRenderer()->
		createVertexShader(shaderFileName + L"\\AssistVertexShader.cso",
			polygonLayout, numElements, &mVertexShader, &mInputLayout));

	RH(mOwnerScene->getRenderer()->
		createPixelShader(shaderFileName + L"\\AssistPixelShader.cso",
			&mPixelShader));

	RH(mOwnerScene->getRenderer()->
		createConstantBuffer(sizeof(AssistConstBuffer), &mAssistConstBuffer));

	return S_OK;
}

void UAssistConfigure::active()
{
	mOwnerScene->getRenderer()->setVertexShader(mVertexShader.get());
	mOwnerScene->getRenderer()->setPixelShader(mPixelShader.get());
	mOwnerScene->getRenderer()->setInputLayout(mInputLayout.get());
}

void UAssistConfigure::reset()
{
	mOwnerScene->getRenderer()->setVertexShader(mVertexShader.get());
	mOwnerScene->getRenderer()->setPixelShader(mPixelShader.get());
	mOwnerScene->getRenderer()->setInputLayout(mInputLayout.get());
}

void UAssistConfigure::close()
{
}

void UAssistConfigure::setMatrix(XMFLOAT4X4 matrix)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	resource = mOwnerScene->getRenderer()->lockResource(mAssistConstBuffer.get());
	((AssistConstBuffer*)resource.pData)->mWVP = matrix;
	mOwnerScene->getRenderer()->unlockResource(mAssistConstBuffer.get());

	mOwnerScene->getRenderer()->setConstantBuffers(0, 1, &mAssistConstBuffer);
}