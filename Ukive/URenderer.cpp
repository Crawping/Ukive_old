#include "UCommon.h"
#include "UWindow.h"
#include "UApplication.h"
#include "UDeviceManager.h"
#include "ISwapChainResizeNotifier.h"
#include "IDirect3DRenderListener.h"
#include "URenderer.h"


URenderer::URenderer()
{
	mD3DRenderListener = nullptr;
}

URenderer::~URenderer()
{
}


HRESULT URenderer::init(UWindow *window)
{
	mOwnerWindow = window;
	mDeviceManager = window->getApplication()->getDeviceManager();

	return createRenderResource();
}

HRESULT URenderer::createRenderResource()
{
	RH(mDeviceManager->getD2DDeviceContext()->
		CreateEffect(CLSID_D2D1Shadow, &mShadowEffect));
	RH(mDeviceManager->getD2DDeviceContext()->
		CreateEffect(CLSID_D2D12DAffineTransform, &mAffineTransEffect));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;

	RH(mDeviceManager->getDXGIFactory()->CreateSwapChainForHwnd(
		mDeviceManager->getD3DDevice().get(),
		mOwnerWindow->getWindowHandle(),
		&swapChainDesc, 0, 0, &mSwapChain));

	UComPtr<IDXGISurface> backBufferPtr;
	RH(mSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)&backBufferPtr));
	RH(createBitmapRenderTarget(backBufferPtr.get(), &mBitmapRenderTarget));

	mDeviceManager->getD2DDeviceContext()->
		SetTarget(mBitmapRenderTarget.get());

	DXGI_SWAP_CHAIN_DESC1 checkDesc;
	mSwapChain->GetDesc1(&checkDesc);

	mWidth = checkDesc.Width;
	mHeight = checkDesc.Height;

	return S_OK;
}

void URenderer::releaseRenderResource()
{
	mBitmapRenderTarget.reset();
	mSwapChain.reset();
	mShadowEffect.reset();
	mAffineTransEffect.reset();
}

HRESULT URenderer::resize()
{
	mDeviceManager->getD2DDeviceContext()->SetTarget(0);
	mBitmapRenderTarget.reset();

	for (auto it = mSCResizeNotifierList.begin();
		it != mSCResizeNotifierList.end(); ++it)
	{
		(*it)->onSwapChainResize();
	}

	RH(mSwapChain->ResizeBuffers(
		0, 0, 0, DXGI_FORMAT_UNKNOWN, 0));

	UComPtr<IDXGISurface> backBufferPtr;
	RH(mSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)&backBufferPtr));
	RH(createBitmapRenderTarget(backBufferPtr.get(), &mBitmapRenderTarget));

	mDeviceManager->getD2DDeviceContext()->
		SetTarget(mBitmapRenderTarget.get());

	DXGI_SWAP_CHAIN_DESC1 checkDesc;
	mSwapChain->GetDesc1(&checkDesc);

	mWidth = checkDesc.Width;
	mHeight = checkDesc.Height;

	for (auto it = mSCResizeNotifierList.begin();
		it != mSCResizeNotifierList.end(); ++it)
	{
		(*it)->onSwapChainResized();
	}

	return S_OK;
}

void URenderer::render(
	D2D1_COLOR_F bkColor,
	std::function<void()> renderCallback)
{
	HRESULT hr;

	if (mD3DRenderListener)
		mD3DRenderListener->onClear();

	mDeviceManager->getD2DDeviceContext()->BeginDraw();
	mDeviceManager->getD2DDeviceContext()->Clear(bkColor);

	renderCallback();

	hr = mDeviceManager->getD2DDeviceContext()->EndDraw();

	if (mD3DRenderListener)
		mD3DRenderListener->onRender();

	hr = mSwapChain->Present(UApplication::isVSyncEnabled() ? 1 : 0, 0);
}

void URenderer::close()
{
	releaseRenderResource();
}


HRESULT URenderer::drawWithShadow(
	float elevation,
	float width, float height,
	std::function<void(UComPtr<ID2D1RenderTarget> rt)> drawer)
{
	UComPtr<ID2D1BitmapRenderTarget> bmpRenderTarget;
	RH(mDeviceManager->getD2DDeviceContext()->CreateCompatibleRenderTarget(
		D2D1::SizeF(width, height), &bmpRenderTarget));

	bmpRenderTarget->BeginDraw();
	bmpRenderTarget->Clear(D2D1::ColorF(0, 0));

	drawer(bmpRenderTarget.cast<ID2D1RenderTarget>());

	RH(bmpRenderTarget->EndDraw());

	UComPtr<ID2D1Bitmap> bkBitmap;
	RH(bmpRenderTarget->GetBitmap(&bkBitmap));

	mShadowEffect->SetInput(0, bkBitmap.get());
	RH(mShadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, elevation));
	RH(mShadowEffect->SetValue(D2D1_SHADOW_PROP_COLOR, D2D1::Vector4F(0, 0, 0, .4f)));

	D2D1_MATRIX_3X2_F matrix = D2D1::Matrix3x2F::Translation(0, elevation / 2.f);
	mAffineTransEffect->SetInputEffect(0, mShadowEffect.get());
	RH(mAffineTransEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, matrix));

	mDeviceManager->getD2DDeviceContext()->DrawImage(mAffineTransEffect.get());
	mDeviceManager->getD2DDeviceContext()->DrawBitmap(bkBitmap.get());

	return S_OK;
}

HRESULT URenderer::drawShadow(float elevation, float alpha, ID2D1Bitmap *bitmap)
{
	//在 Alpha 动画时，令阴影更快消退。
	float shadowAlpha;
	if (alpha == 0.f)
		shadowAlpha = 0.f;
	else if (alpha == 1.f)
		shadowAlpha = .38f;
	else
		shadowAlpha = static_cast<float>(.38f*::pow(2, 8 * (alpha - 1)) / 1.f);

	mShadowEffect->SetInput(0, bitmap);
	RH(mShadowEffect->SetValue(D2D1_SHADOW_PROP_OPTIMIZATION, D2D1_SHADOW_OPTIMIZATION_BALANCED));
	RH(mShadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, elevation));
	RH(mShadowEffect->SetValue(D2D1_SHADOW_PROP_COLOR, D2D1::Vector4F(0, 0, 0, shadowAlpha)));

	D2D1_MATRIX_3X2_F matrix = D2D1::Matrix3x2F::Translation(0, elevation / 1.5f);
	mAffineTransEffect->SetInputEffect(0, mShadowEffect.get());
	RH(mAffineTransEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, matrix));

	mDeviceManager->getD2DDeviceContext()->DrawImage(mAffineTransEffect.get());

	return S_OK;
}

HRESULT URenderer::drawOnBitmap(
	float width, float height, ID2D1Bitmap **bitmap,
	std::function<void(UComPtr<ID2D1RenderTarget> rt)> drawer)
{
	UComPtr<ID2D1BitmapRenderTarget> bmpRenderTarget;
	RH(mDeviceManager->getD2DDeviceContext()->CreateCompatibleRenderTarget(
		D2D1::SizeF(width, height), &bmpRenderTarget));

	bmpRenderTarget->BeginDraw();
	bmpRenderTarget->Clear(D2D1::ColorF(0, 0));

	drawer(bmpRenderTarget.cast<ID2D1RenderTarget>());

	RH(bmpRenderTarget->EndDraw());
	RH(bmpRenderTarget->GetBitmap(bitmap));

	return S_OK;
}

void URenderer::drawObjects(UDrawingObjectManager::DrawingObject *object)
{
	if (object == 0)
		return;

	mDeviceManager->getD3DDeviceContext()->IASetVertexBuffers(
		0, 1, &object->vertexBuffer, &object->vertexStructSize, &object->vertexDataOffset);
	mDeviceManager->getD3DDeviceContext()->IASetIndexBuffer(object->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	mDeviceManager->getD3DDeviceContext()->DrawIndexed(object->indexCount, 0, 0);
}

void URenderer::draw(ID3D11Buffer* vertices, ID3D11Buffer* indices, int structSize, int indexCount)
{
	UINT vertexDataOffset = 0;
	UINT vertexStructSize = structSize;

	mDeviceManager->getD3DDeviceContext()->IASetVertexBuffers(
		0, 1, &vertices, &vertexStructSize, &vertexDataOffset);
	mDeviceManager->getD3DDeviceContext()->IASetIndexBuffer(indices, DXGI_FORMAT_R32_UINT, 0);
	mDeviceManager->getD3DDeviceContext()->DrawIndexed(indexCount, 0, 0);
}


void URenderer::addSwapChainResizeNotifier(ISwapChainResizeNotifier *notifier)
{
	mSCResizeNotifierList.push_back(notifier);
}

void URenderer::removeSwapChainResizeNotifier(ISwapChainResizeNotifier *notifier)
{
	for (auto it = mSCResizeNotifierList.begin();
		it != mSCResizeNotifierList.end();)
	{
		if ((*it) == notifier)
			it = mSCResizeNotifierList.erase(it);
		else
			++it;
	}
}

void URenderer::removeAllSwapChainResizeNotifier()
{
	mSCResizeNotifierList.clear();
}

void URenderer::setDirect3DRenderListener(IDirect3DRenderListener *listener)
{
	mD3DRenderListener = listener;
}


void URenderer::setVertexShader(ID3D11VertexShader *shader)
{
	mDeviceManager->getD3DDeviceContext()->VSSetShader(shader, 0, 0);
}

void URenderer::setPixelShader(ID3D11PixelShader *shader)
{
	mDeviceManager->getD3DDeviceContext()->PSSetShader(shader, 0, 0);
}

void URenderer::setInputLayout(ID3D11InputLayout *inputLayout)
{
	mDeviceManager->getD3DDeviceContext()->IASetInputLayout(inputLayout);
}

void URenderer::setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	mDeviceManager->getD3DDeviceContext()->IASetPrimitiveTopology(topology);
}

void URenderer::setConstantBuffers(
	UINT startSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers)
{
	mDeviceManager->getD3DDeviceContext()->VSSetConstantBuffers(
		startSlot, NumBuffers, ppConstantBuffers);
}


UINT URenderer::getScWidth()
{
	return mWidth;
}

UINT URenderer::getScHeight()
{
	return mHeight;
}

UComPtr<ID2D1Effect> URenderer::getShadowEffect()
{
	return mShadowEffect;
}

UComPtr<ID2D1Effect> URenderer::getAffineTransEffect()
{
	return mAffineTransEffect;
}

UComPtr<IDXGISwapChain1> URenderer::getSwapChain()
{
	return mSwapChain;
}


HRESULT URenderer::createBitmapRenderTarget(IDXGISurface *dxgiSurface, ID2D1Bitmap1 **bitmap)
{
	HRESULT hr = S_OK;

	FLOAT dpiX;
	FLOAT dpiY;
	mDeviceManager->getD2DFactory()->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
		D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
			dpiX,
			dpiY
		);

	return mDeviceManager->getD2DDeviceContext()->
		CreateBitmapFromDxgiSurface(dxgiSurface, bitmapProperties, bitmap);
}

HRESULT URenderer::createCompatBitmapRenderTarget(
	float width, float height, ID2D1BitmapRenderTarget **bRT)
{
	UComPtr<ID2D1BitmapRenderTarget> bmpRenderTarget;
	RH(mDeviceManager->getD2DDeviceContext()->CreateCompatibleRenderTarget(
		D2D1::SizeF(width, height), bRT));

	return S_OK;
}

HRESULT URenderer::createDXGISurfaceRenderTarget(
	IDXGISurface *dxgiSurface, ID2D1RenderTarget **renderTarget)
{
	HRESULT hr = S_OK;

	FLOAT dpiX;
	FLOAT dpiY;
	mDeviceManager->getD2DFactory()->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		dpiX, dpiY);

	return mDeviceManager->getD2DFactory()->CreateDxgiSurfaceRenderTarget(dxgiSurface, props, renderTarget);
}

HRESULT URenderer::createWindowRenderTarget(
	HWND handle, unsigned int width, unsigned int height, ID2D1HwndRenderTarget **renderTarget)
{
	HRESULT hr = S_OK;

	FLOAT dpiX;
	FLOAT dpiY;
	mDeviceManager->getD2DFactory()->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties();
	// Set the DPI to be the default system DPI to allow direct mapping
	// between image pixels and desktop pixels in different system DPI settings
	renderTargetProperties.dpiX = dpiX;
	renderTargetProperties.dpiY = dpiY;

	return mDeviceManager->getD2DFactory()->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(handle, D2D1::SizeU(width, height)),
		renderTarget);
}

HRESULT URenderer::createTextFormat(
	std::wstring fontFamilyName, 
	float fontSize, std::wstring localeName, 
	IDWriteTextFormat **textFormat)
{
	return mDeviceManager->getDWriteFactory()->CreateTextFormat(
		fontFamilyName.c_str(), nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		localeName.c_str(),
		textFormat);
}

HRESULT URenderer::createTextLayout(
	std::wstring text, 
	IDWriteTextFormat *textFormat, 
	float maxWidth, float maxHeight, 
	IDWriteTextLayout **textLayout)
{
	return mDeviceManager->getDWriteFactory()->CreateTextLayout(
		text.c_str(), text.length(), textFormat, maxWidth, maxHeight, textLayout);
}


HRESULT URenderer::createVertexShader(
	std::wstring fileName,
	D3D11_INPUT_ELEMENT_DESC *polygonLayout,
	UINT numElements,
	ID3D11VertexShader **vertexShader,
	ID3D11InputLayout **inputLayout)
{
	std::ifstream reader(fileName.c_str(), std::ios::binary);
	auto cpos = reader.tellg();
	reader.seekg(0, std::ios_base::end);
	size_t charSize = (size_t)reader.tellg();
	reader.seekg(cpos);

	char *shaderBuf = new char[charSize];
	reader.read(shaderBuf, charSize);

	RH(mDeviceManager->getD3DDevice()->CreateVertexShader(
		shaderBuf, charSize, 0, vertexShader));

	RH(mDeviceManager->getD3DDevice()->CreateInputLayout(
		polygonLayout, numElements,
		shaderBuf, charSize, inputLayout));

	delete[] shaderBuf;

	return S_OK;
}

HRESULT URenderer::createPixelShader(
	std::wstring fileName,
	ID3D11PixelShader **pixelShader)
{
	std::ifstream reader(fileName.c_str(), std::ios::binary);
	auto cpos = reader.tellg();
	reader.seekg(0, std::ios_base::end);
	size_t charSize = (size_t)reader.tellg();
	reader.seekg(cpos);

	char *shaderBuf = new char[charSize];
	reader.read(shaderBuf, charSize);

	RH(mDeviceManager->getD3DDevice()->CreatePixelShader(
		shaderBuf, charSize, 0, pixelShader));

	delete[] shaderBuf;

	return S_OK;
}


HRESULT URenderer::createVertexBuffer(
	void *vertices, UINT structSize, UINT vertexCount, ID3D11Buffer *&vertexBuffer)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = structSize * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	RH(mDeviceManager->getD3DDevice()
		->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer));

	return S_OK;
}

HRESULT URenderer::createIndexBuffer(int *indices, UINT indexCount, ID3D11Buffer *&indexBuffer)
{
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	// 设置索引缓冲描述.
	indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufferDesc.ByteWidth = sizeof(int)* indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 指向存临时索引缓冲.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 创建索引缓冲.
	RH(mDeviceManager->getD3DDevice()
		->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer));

	return S_OK;
}

HRESULT URenderer::createConstantBuffer(UINT size, ID3D11Buffer **buffer)
{
	HRESULT hr = E_FAIL;
	ID3D11Buffer *_buffer = 0;
	D3D11_BUFFER_DESC constBufferDesc;

	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = size;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;
	constBufferDesc.StructureByteStride = 0;

	hr = mDeviceManager->getD3DDevice()
		->CreateBuffer(&constBufferDesc, 0, &_buffer);

	*buffer = _buffer;

	return hr;
}

D3D11_MAPPED_SUBRESOURCE URenderer::lockResource(ID3D11Resource *resource)
{
	HRESULT hr = E_FAIL;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	hr = mDeviceManager->getD3DDeviceContext()
		->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (SUCCEEDED(hr))
		return mappedResource;

	mappedResource.pData = nullptr;
	return mappedResource;
}

void URenderer::unlockResource(ID3D11Resource *resource)
{
	mDeviceManager->getD3DDeviceContext()->Unmap(resource, 0);
}