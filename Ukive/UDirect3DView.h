#pragma once
#include "UComPtr.h"
#include "UWidget.h"
#include "ISwapChainResizeNotifier.h"
#include "IDirect3DRenderListener.h"

class UWindow;
class TerrainScene;

class UDirect3DView :
	public UWidget,
	public ISwapChainResizeNotifier,
	public IDirect3DRenderListener
{
private:
	TerrainScene *mScene;
	D3D11_VIEWPORT mViewport;

	UComPtr<ID3D11Texture2D> mNormalDepthStencilBuffer;
	UComPtr<ID3D11RenderTargetView> mNormalRenderTargetView;
	UComPtr<ID3D11DepthStencilView> mNormalDepthStencilView;
	UComPtr<ID3D11ShaderResourceView> mShaderResourceView;

	UComPtr<ID3D11DepthStencilState> mNormalDepthStencilState;
	UComPtr<ID3D11RasterizerState> mNormalRasterizerState;
	UComPtr<ID3D11SamplerState> mNormalSamplerState;

	void initDirect3DView();

	HRESULT createState();
	void releaseState();

	void setViewports(
		float x, float y, 
		float width, float height);

	HRESULT createResourceView();
	void releaseResourceView();

public:
	UDirect3DView(UWindow *wnd);
	UDirect3DView(UWindow *wnd, int id);
	virtual ~UDirect3DView();

	virtual void onClear() override;
	virtual void onRender() override;

	virtual void onSwapChainResize() override;
	virtual void onSwapChainResized() override;

	TerrainScene *getTerrainScene();

protected:
	virtual void onMeasure(
		int width, int height,
		int widthSpec, int heightSpec) override;
	virtual void onDraw(UCanvas *canvas) override;
	virtual bool onInputEvent(UInputEvent *e) override;

	virtual void onLayout(
		bool changed, bool sizeChanged,
		int left, int top, int right, int bottom) override;
	virtual void onSizeChanged(
		int width, int height, int oldWidth, int oldHeight) override;
};