#pragma once
#include "UComPtr.h"
#include "UDrawingObjectManager.h"

class UWindow;
class UCanvas;
class UDeviceManager;
class ISwapChainResizeNotifier;
class IDirect3DRenderListener;

class URenderer
{
private:
	UINT mWidth, mHeight;
	UWindow *mOwnerWindow;
	UDeviceManager *mDeviceManager;
	IDirect3DRenderListener *mD3DRenderListener;
	std::list<ISwapChainResizeNotifier*> mSCResizeNotifierList;

	UComPtr<IDXGISwapChain1> mSwapChain;
	UComPtr<ID2D1Bitmap1> mBitmapRenderTarget;
	UComPtr<ID2D1DeviceContext> mD2DDeviceContext;

	UComPtr<ID2D1Effect> mShadowEffect;
	UComPtr<ID2D1Effect> mAffineTransEffect;

	HRESULT createRenderResource();
	void releaseRenderResource();

public:
	URenderer();
	~URenderer();

	HRESULT init(UWindow *window);
	HRESULT resize();
	void render(
		D2D1_COLOR_F bkColor,
		std::function<void()> renderCallback);
	void close();

	HRESULT drawWithShadow(
		float elevation, float width, float height,
		std::function<void(UComPtr<ID2D1RenderTarget> rt)> drawer);
	HRESULT drawShadow(float elevation, float alpha, ID2D1Bitmap *bitmap);
	HRESULT drawOnBitmap(
		float width, float height, ID2D1Bitmap **bitmap,
		std::function<void(UComPtr<ID2D1RenderTarget> rt)> drawer);

	void drawObjects(UDrawingObjectManager::DrawingObject *object);
	void draw(ID3D11Buffer* vertices, ID3D11Buffer* indices, int structSize, int indexCount);

	void addSwapChainResizeNotifier(ISwapChainResizeNotifier *notifier);
	void removeSwapChainResizeNotifier(ISwapChainResizeNotifier *notifier);
	void removeAllSwapChainResizeNotifier();

	void setDirect3DRenderListener(IDirect3DRenderListener *listener);

	void setVertexShader(ID3D11VertexShader *shader);
	void setPixelShader(ID3D11PixelShader *shader);
	void setInputLayout(ID3D11InputLayout *inputLayout);
	void setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
	void setConstantBuffers(UINT startSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers);

	UINT getScWidth();
	UINT getScHeight();
	UComPtr<ID2D1Effect> getShadowEffect();
	UComPtr<ID2D1Effect> getAffineTransEffect();
	UComPtr<IDXGISwapChain1> getSwapChain();
	UComPtr<ID2D1DeviceContext> getD2DDeviceContext();

	HRESULT createBitmapRenderTarget(
		IDXGISurface *dxgiSurface, ID2D1Bitmap1 **bitmap);
	HRESULT createCompatBitmapRenderTarget(
		float width, float height, ID2D1BitmapRenderTarget **bRT);
	HRESULT createDXGISurfaceRenderTarget(
		IDXGISurface *dxgiSurface, ID2D1RenderTarget **renderTarget);
	HRESULT createWindowRenderTarget(
		HWND handle, unsigned int width, unsigned int height, 
		ID2D1HwndRenderTarget **renderTarget);

	HRESULT createTextFormat(
		std::wstring fontFamilyName, 
		float fontSize, std::wstring localeName, 
		IDWriteTextFormat **textFormat);
	HRESULT createTextLayout(
		std::wstring text, IDWriteTextFormat *textFormat, 
		float maxWidth, float maxHeight, 
		IDWriteTextLayout **textLayout);

	HRESULT createVertexShader(
		std::wstring fileName, 
		D3D11_INPUT_ELEMENT_DESC *polygonLayout,
		UINT numElements,
		ID3D11VertexShader **vertexShader,
		ID3D11InputLayout **inputLayout);
	HRESULT createPixelShader(
		std::wstring fileName, 
		ID3D11PixelShader **pixelShader);

	HRESULT createVertexBuffer(
		void *vertices, UINT structSize, UINT vertexCount, ID3D11Buffer *&vertexBuffer);
	HRESULT createIndexBuffer(int *indices, UINT indexCount, ID3D11Buffer *&indexBuffer);
	HRESULT createConstantBuffer(UINT size, ID3D11Buffer **buffer);

	D3D11_MAPPED_SUBRESOURCE lockResource(ID3D11Resource *resource);
	void unlockResource(ID3D11Resource *resource);
};