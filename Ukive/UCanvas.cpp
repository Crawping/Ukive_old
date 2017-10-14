#include "UCommon.h"
#include "UTextRenderer.h"
#include "UDeviceManager.h"
#include "View.h"
#include "UCanvas.h"


UCanvas::UCanvas(UComPtr<ID2D1RenderTarget> renderTarget)
{
	mOpacity = 1.f;
	mLayerCounter = 0;
	mTextRenderer = nullptr;
	mMatrix = D2D1::Matrix3x2F::Identity();

	mRenderTarget = renderTarget;
	mSolidBrush = UDeviceManager::sCanvasSolidBrush;
	mBitmapBrush = UDeviceManager::sCanvasBitmapBrush;
}


UCanvas::~UCanvas()
{
	if (mTextRenderer)
		delete mTextRenderer;
}


void UCanvas::setOpacity(float opacity)
{
	if (opacity == mOpacity)
		return;

	mOpacity = opacity;

	mSolidBrush->SetOpacity(mOpacity);
	mBitmapBrush->SetOpacity(mOpacity);

	if (mTextRenderer)
		mTextRenderer->setOpacity(mOpacity);
}

float UCanvas::getOpacity()
{
	return mOpacity;
}


void UCanvas::beginDraw()
{
	mRenderTarget->BeginDraw();
}

void UCanvas::endDraw()
{
	mRenderTarget->EndDraw();
}


void UCanvas::popClip()
{
	mRenderTarget->PopAxisAlignedClip();
}

void UCanvas::pushClip(D2D1_RECT_F &rect)
{
	mRenderTarget->PushAxisAlignedClip(
		rect, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
}


void UCanvas::pushLayer(ID2D1Geometry *clipGeometry)
{
	if (mLayerCounter > 0)
	{
		++mLayerCounter;
		return;
	}

	UComPtr<ID2D1DeviceContext> d2dDC 
		= mRenderTarget.cast<ID2D1DeviceContext>();
	if (d2dDC == nullptr)
	{
		if (mLayer == nullptr)
		{
			HRESULT hr = mRenderTarget->CreateLayer(&mLayer);
			if (FAILED(hr))
				throw std::runtime_error("UCanvas-Constructor(): Create layer failed.");
		}

		mRenderTarget->PushLayer(
			D2D1::LayerParameters(D2D1::InfiniteRect(), clipGeometry),
			mLayer.get());
	}
	else
	{
		d2dDC->PushLayer(
			D2D1::LayerParameters1(D2D1::InfiniteRect(), clipGeometry), 
			nullptr);
	}

	++mLayerCounter;
}

void UCanvas::pushLayer(D2D1_RECT_F &contentBound, ID2D1Geometry *clipGeometry)
{
	if (mLayerCounter > 0)
	{
		++mLayerCounter;
		return;
	}

	UComPtr<ID2D1DeviceContext> d2dDC
		= mRenderTarget.cast<ID2D1DeviceContext>();
	if (d2dDC == nullptr)
	{
		if (mLayer == nullptr)
		{
			HRESULT hr = mRenderTarget->CreateLayer(&mLayer);
			if (FAILED(hr))
				throw std::runtime_error("UCanvas-Constructor(): Create layer failed.");
		}

		mRenderTarget->PushLayer(
			D2D1::LayerParameters(contentBound, clipGeometry),
			mLayer.get());
	}
	else
	{
		d2dDC->PushLayer(
			D2D1::LayerParameters1(contentBound, clipGeometry),
			nullptr);
	}

	++mLayerCounter;
}

void UCanvas::popLayer()
{
	if (mLayerCounter > 1)
	{
		--mLayerCounter;
		return;
	}

	mRenderTarget->PopLayer();

	--mLayerCounter;
}


void UCanvas::save()
{
	ID2D1Factory *factory = nullptr;
	mRenderTarget->GetFactory(&factory);

	ID2D1DrawingStateBlock *drawingStateBlock;
	factory->CreateDrawingStateBlock(&drawingStateBlock);
	mRenderTarget->SaveDrawingState(drawingStateBlock);

	mOpacityStack.push(mOpacity);
	mDrawingStateStack.push(drawingStateBlock);
}

void UCanvas::restore()
{
	if (mDrawingStateStack.empty())
		return;

	mOpacity = mOpacityStack.top();

	ID2D1DrawingStateBlock *drawingStateBlock
		= mDrawingStateStack.top().get();

	D2D1_DRAWING_STATE_DESCRIPTION desc;
	drawingStateBlock->GetDescription(&desc);

	D2D1_MATRIX_3X2_F matrix = desc.transform;

	mMatrix._11 = matrix._11;
	mMatrix._12 = matrix._12;
	mMatrix._21 = matrix._21;
	mMatrix._22 = matrix._22;
	mMatrix._31 = matrix._31;
	mMatrix._32 = matrix._32;

	mRenderTarget->RestoreDrawingState(drawingStateBlock);

	mOpacityStack.pop();
	mDrawingStateStack.pop();

	mSolidBrush->SetOpacity(mOpacity);
	mBitmapBrush->SetOpacity(mOpacity);

	if (mTextRenderer)
		mTextRenderer->setOpacity(mOpacity);
}

ID2D1RenderTarget *UCanvas::getRT()
{
	return mRenderTarget.get();
}


void UCanvas::scale(float sx, float sy)
{
	scale(sx, sy, 0.f, 0.f);
}

void UCanvas::scale(float sx, float sy, float cx, float cy)
{
	D2D1::Matrix3x2F tmp = D2D1::Matrix3x2F::Scale(D2D1::SizeF(sx, sy), D2D1::Point2F(cx, cy));
	mMatrix = mMatrix*tmp;
	mRenderTarget->SetTransform(mMatrix);
}

void UCanvas::rotate(float angle)
{
	rotate(angle, 0.f, 0.f);
}

void UCanvas::rotate(float angle, float cx, float cy)
{
	D2D1::Matrix3x2F tmp = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(cx, cy));
	mMatrix = mMatrix*tmp;
	mRenderTarget->SetTransform(mMatrix);
}

void UCanvas::translate(int dx, int dy)
{
	this->translate(
		static_cast<float>(dx),
		static_cast<float>(dy));
}

void UCanvas::translate(float dx, float dy)
{
	D2D1::Matrix3x2F tmp = D2D1::Matrix3x2F::Translation(D2D1::SizeF(dx, dy));
	mMatrix = mMatrix*tmp;
	mRenderTarget->SetTransform(mMatrix);
}


void UCanvas::setMatrix(D2D1::Matrix3x2F matrix)
{
	mMatrix = matrix;
	mRenderTarget->SetTransform(mMatrix);
}

D2D1::Matrix3x2F UCanvas::getMatrix()
{
	return mMatrix;
}


void UCanvas::fillOpacityMask(
	float width, float height,
	ID2D1Bitmap *mask, ID2D1Bitmap *content)
{
	mBitmapBrush->SetBitmap(content);

	D2D1_RECT_F rect = D2D1::RectF(0, 0, width, height);

	mRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	mRenderTarget->FillOpacityMask(
		mask, mBitmapBrush.get(), D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rect, rect);
	mRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
}


void UCanvas::drawRect(D2D1_RECT_F &rect, D2D1_COLOR_F &color)
{
	mSolidBrush->SetColor(color);
	mRenderTarget->DrawRectangle(rect, mSolidBrush.get());
}

void UCanvas::drawRect(D2D1_RECT_F &rect, float strokeWidth, D2D1_COLOR_F &color)
{
	mSolidBrush->SetColor(color);
	mRenderTarget->DrawRectangle(rect, mSolidBrush.get(), strokeWidth);
}


void UCanvas::fillRect(D2D1_RECT_F &rect, D2D1_COLOR_F &color)
{
	mSolidBrush->SetColor(color);
	mRenderTarget->FillRectangle(rect, mSolidBrush.get());
}


void UCanvas::drawRoundRect(
	D2D1_RECT_F &rect, float radius, D2D1_COLOR_F &color)
{
	mSolidBrush->SetColor(color);
	mRenderTarget->DrawRoundedRectangle(
		D2D1::RoundedRect(rect, radius, radius), mSolidBrush.get());
}

void UCanvas::drawRoundRect(
	D2D1_RECT_F &rect, float strokeWidth,
	float radius, D2D1_COLOR_F &color)
{
	mSolidBrush->SetColor(color);
	mRenderTarget->DrawRoundedRectangle(
		D2D1::RoundedRect(rect, radius, radius), mSolidBrush.get(), strokeWidth);
}

void UCanvas::fillRoundRect(
	D2D1_RECT_F &rect, float radius, D2D1_COLOR_F &color)
{
	mSolidBrush->SetColor(color);
	mRenderTarget->FillRoundedRectangle(
		D2D1::RoundedRect(rect, radius, radius), mSolidBrush.get());
}


void UCanvas::drawCircle(float cx, float cy, float radius, D2D1_COLOR_F &color)
{
	drawOval(cx, cy, radius, radius, color);
}

void UCanvas::drawCircle(float cx, float cy, float radius, float strokeWidth, D2D1_COLOR_F &color)
{
	drawOval(cx, cy, radius, radius, strokeWidth, color);
}

void UCanvas::fillCircle(float cx, float cy, float radius, D2D1_COLOR_F &color)
{
	fillOval(cx, cy, radius, radius, color);
}


void UCanvas::drawOval(float cx, float cy, float radiusX, float radiusY, D2D1_COLOR_F &color)
{
	mSolidBrush->SetColor(color);
	mRenderTarget->DrawEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(cx, cy), 
			radiusX, radiusY), 
		mSolidBrush.get());
}

void UCanvas::drawOval(float cx, float cy, float radiusX, float radiusY, float strokeWidth, D2D1_COLOR_F &color)
{
	mSolidBrush->SetColor(color);
	mRenderTarget->DrawEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(cx, cy), 
			radiusX, radiusY), 
		mSolidBrush.get(), strokeWidth);
}

void UCanvas::fillOval(float cx, float cy, float radiusX, float radiusY, D2D1_COLOR_F &color)
{
	mSolidBrush->SetColor(color);
	mRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(cx, cy), radiusX, radiusY), mSolidBrush.get());
}


void UCanvas::drawBitmap(ID2D1Bitmap *bitmap)
{
	if (bitmap == nullptr)
		return;

	D2D1_SIZE_F size = bitmap->GetSize();
	D2D1_RECT_F srcRect = D2D1::RectF(0.f, 0.f, size.width, size.height);

	drawBitmap(srcRect, srcRect, 1.f, bitmap);
}

void UCanvas::drawBitmap(float x, float y, ID2D1Bitmap *bitmap)
{
	if (bitmap == nullptr)
		return;

	D2D1_SIZE_F size = bitmap->GetSize();
	D2D1_RECT_F srcRect = D2D1::RectF(0.f, 0.f, size.width, size.height);
	D2D1_RECT_F dstRect = D2D1::RectF(x, y, size.width + x, size.height + y);

	drawBitmap(srcRect, dstRect, 1.f, bitmap);
}

void UCanvas::drawBitmap(D2D1_RECT_F &dst, float opacity, ID2D1Bitmap *bitmap)
{
	if (bitmap == nullptr)
		return;

	D2D1_SIZE_F size = bitmap->GetSize();
	D2D1_RECT_F srcRect = D2D1::RectF(0.f, 0.f, size.width, size.height);

	drawBitmap(srcRect, dst, opacity, bitmap);
}

void UCanvas::drawBitmap(D2D1_RECT_F &src, D2D1_RECT_F &dst, float opacity, ID2D1Bitmap *bitmap)
{
	if (bitmap == nullptr)
		return;

	mRenderTarget->DrawBitmap(
		bitmap, dst, opacity,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		src);
}


void UCanvas::drawText(
	std::wstring text, IDWriteTextFormat *textFormat, 
	D2D1_RECT_F &layoutRect, D2D1_COLOR_F &color)
{
	if (textFormat == nullptr)
		return;

	mSolidBrush->SetColor(color);
	mRenderTarget->DrawTextW(text.c_str(), text.length(), textFormat, layoutRect, mSolidBrush.get());
}

void UCanvas::drawTextLayout(
	float x, float y, 
	IDWriteTextLayout *textLayout, D2D1_COLOR_F &color)
{
	if (textLayout == 0)
		return;

	mSolidBrush->SetColor(color);
	mRenderTarget->DrawTextLayout(D2D1::Point2F(x, y), textLayout, mSolidBrush.get());
}

void UCanvas::drawTextLayoutWithEffect(
	View *widget, 
	float x, float y, 
	IDWriteTextLayout *textLayout, D2D1_COLOR_F &color)
{
	if (mTextRenderer == nullptr)
	{
		mTextRenderer = new UTextRenderer(mRenderTarget);
		mTextRenderer->setOpacity(mOpacity);
	}

	mTextRenderer->setTextColor(color);
	textLayout->Draw(widget, mTextRenderer, x, y);
}