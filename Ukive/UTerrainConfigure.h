#pragma once
#include "UComPtr.h"

class UScene;


struct TerrainVertexData
{
	XMFLOAT3 position;
	XMFLOAT2 texcoord;

	TerrainVertexData()
	{
		position = XMFLOAT3(0, 0, 0);
		texcoord = XMFLOAT2(0, 0);
	}
};


class UTerrainConfigure
{
private:
	UScene *mOwnerScene;

	struct MatrixConstBuffer
	{
		XMFLOAT4X4 mWVP;
	};

private:
	UComPtr<ID3D11VertexShader> mVertexShader;
	UComPtr<ID3D11PixelShader> mPixelShader;
	UComPtr<ID3D11InputLayout> mInputLayout;

	UComPtr<ID3D11Buffer> mMatrixConstBuffer;

public:
	UTerrainConfigure();
	~UTerrainConfigure();

	HRESULT init(UScene *ownerScene);
	void active();
	void reset();
	void close();

	void setMatrix(XMFLOAT4X4 matrix);
};