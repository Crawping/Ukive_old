#pragma once
#include "UComPtr.h"

class UScene;

struct AssistVertexData
{
	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT2 texcoord;

	AssistVertexData()
	{
		position = XMFLOAT3(0, 0, 0);
		color = XMFLOAT4(0, 0, 0, 1);
		texcoord = XMFLOAT2(0, 0);
	}
};


class UAssistConfigure
{
private:
	UScene *mOwnerScene;

	struct AssistConstBuffer
	{
		XMFLOAT4X4 mWVP;
	};

private:
	UComPtr<ID3D11VertexShader> mVertexShader;
	UComPtr<ID3D11PixelShader> mPixelShader;
	UComPtr<ID3D11InputLayout> mInputLayout;
	UComPtr<ID3D11Buffer> mAssistConstBuffer;

public:
	UAssistConfigure();
	~UAssistConfigure();

	HRESULT init(UScene *ownerScene);
	void active();
	void reset();
	void close();

	void setMatrix(XMFLOAT4X4 matrix);
};