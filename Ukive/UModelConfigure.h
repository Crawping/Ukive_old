#pragma once
#include "UComPtr.h"

class UScene;


struct ModelVertexData
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT3 normal;
	XMFLOAT2 texcoord;

	ModelVertexData()
	{
		position = XMFLOAT3(0, 0, 0);
		color = XMFLOAT4(0, 0, 0, 1);
		normal = XMFLOAT3(0, 0, 0);
		texcoord = XMFLOAT2(0, 0);
	}
};


class UModelConfigure
{
private:
	UScene *mOwnerScene;

	struct MatrixConstBuffer
	{
		XMFLOAT4X4 mWVP;
	};

	struct PhongLightConstBuffer
	{
		XMFLOAT4X4 mWVP;
	};

private:
	UComPtr<ID3D11VertexShader> mVertexShader;
	UComPtr<ID3D11PixelShader> mPixelShader;
	UComPtr<ID3D11InputLayout> mInputLayout;
	
	UComPtr<ID3D11Buffer> mMatrixConstBuffer;
	UComPtr<ID3D11Buffer> mModelLightConstBuffer;

public:
	UModelConfigure();
	~UModelConfigure();

	HRESULT init(UScene *ownerScene);
	void active();
	void reset();
	void close();

	void setMatrix(XMFLOAT4X4 matrix);
};