﻿#pragma once
#include "View.h"
#include "UEditable.h"
#include "UAnimator.h"
#include "TextActionModeCallback.h"

class TextBlink;
class TextActionMode;
class UTextKeyListener;
class UInputConnection;
class UTextDrawingEffect;


///<summary>
///文本编辑器，使用TSF。
///</summary>
class TextView : public View, 
	public UEditable::UEditWatcher, public TextActionModeCallback
{
private:
	//为支持撤销而创建的结构体（未完成）。
	struct UndoBlock
	{
		std::wstring text;
		uint32_t start;
		uint32_t oldEnd;
		uint32_t newEnd;
	};

	struct SelectionBlock
	{
		unsigned int start;
		unsigned int length;
		D2D1_RECT_F rect;
	};

	static const int MENU_ID_COPY = 1;
	static const int MENU_ID_CUT = 2;
	static const int MENU_ID_PASTE = 3;
	static const int MENU_ID_SELECTALL = 4;

	static const int MENU_ORDER_COPY = 1;
	static const int MENU_ORDER_CUT = 2;
	static const int MENU_ORDER_PASTE = 3;
	static const int MENU_ORDER_SELECTALL = 4;

private:
	UEditable *mBaseText;
	UComPtr<IDWriteTextFormat> mTextFormat;
	UComPtr<IDWriteTextLayout> mTextLayout;

	TextBlink *mTextBlink;
	TextActionMode *mTextActionMode;
	UInputConnection *mInputConnection;
	UTextKeyListener *mTextKeyListener;

	ULONG64 mProcessRef;

	float mTextSize;
	std::wstring mFontFamilyName;
	D2D1::ColorF mTextColor;
	D2D1::ColorF mSelectionBackgroundColor;
	DWRITE_TEXT_ALIGNMENT mTextAlignment;
	DWRITE_PARAGRAPH_ALIGNMENT mParagraphAlignment;
	DWRITE_FONT_WEIGHT mTextWeight;
	DWRITE_FONT_STYLE mTextStyle;

	float mLineSpacingMultiple;
	DWRITE_LINE_SPACING_METHOD mLineSpacingMethod;

	bool mIsAutoWrap;
	bool mIsEditable;
	bool mIsSelectable;

	int mPrevX, mPrevY;
	bool mIsMouseLeftKeyDown;
	bool mIsMouseRightKeyDown;
	bool mIsMouseLeftKeyDownOnText;

	int mLastSelection;
	int mFirstSelection;
	float mLastSelectionLeft;

	float mVerticalOffset;
	uint32_t mTextOffsetAtViewTop;

	std::vector<UComPtr<UTextDrawingEffect>> mTDEffectList;
	std::vector<std::shared_ptr<SelectionBlock>> mSelectionList;

private:
	void initTextView();

	int computeVerticalScrollRange();
	int computeHorizontalScrollRange();

	void computeTextOffsetAtViewTop();
	int computeVerticalScrollOffsetFromTextOffset(uint32_t tOff);

	int determineVerticalScroll(int dy);
	int determineHorizontalScroll(int dx);

	void scrollToFit(bool considerSelection);

	float getTextWidth();
	float getTextHeight();

	bool getLineInfo(
		uint32_t position,
		uint32_t *line, float *height, uint32_t *count = nullptr);
	bool getLineHeight(
		uint32_t line, float *height);

	void blinkNavigator(int keyCode);

	void locateTextBlink(int position);
	void locateTextBlink(float textX, float textY);
	void makeNewTextFormat();
	void makeNewTextLayout(float maxWidth, float maxHeight, bool autoWrap);

	bool canCut();
	bool canCopy();
	bool canPaste();
	bool canSelectAll();

	void performCut();
	void performCopy();
	void performPaste();
	void performSelectAll();

public:
	TextView(UWindow *wnd);
	TextView(UWindow *wnd, int id);
	virtual ~TextView();

	void onBeginProcess();
	void onEndProcess();

	virtual void onAttachedToWindow() override;
	virtual void onDetachedFromWindow() override;

	void autoWrap(bool enable);
	void setIsEditable(bool editable);
	void setIsSelectable(bool selectable);

	bool isAutoWrap();
	bool isEditable();
	bool isSelectable();

	void setText(std::wstring text);
	void setTextSize(float size);
	void setTextColor(D2D1::ColorF color);
	void setTextAlignment(DWRITE_TEXT_ALIGNMENT alignment);
	void setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT alignment);
	void setTextStyle(DWRITE_FONT_STYLE style);
	void setTextWeight(DWRITE_FONT_WEIGHT weight);
	void setFontFamilyName(std::wstring font);
	void setLineSpacing(bool uniform, float spacingMultiple);

	std::wstring getText();
	UEditable *getEditable();
	float getTextSize();

	void setSelection(unsigned int position);
	void setSelection(unsigned int start, unsigned int end);
	void drawSelection(unsigned int start, unsigned int end);
	std::wstring getSelection();
	int getSelectionStart();
	int getSelectionEnd();
	bool hasSelection();

	uint32_t getHitTextPosition(float textX, float textY);
	bool isHitText(float textX, float textY, uint32_t *hitPos = nullptr);
	bool isHitText(float textX, float textY, uint32_t position, uint32_t length, uint32_t *hitPos = nullptr);
	URectF getSelectionBound(unsigned int start, unsigned int end);

	void computeVisibleRegion(URectF *visibleRegon);

protected:
	virtual void onDraw(UCanvas *canvas) override;
	virtual void onMeasure(int width, int height, int widthSpec, int heightSpec) override;
	virtual bool onInputEvent(UInputEvent *e) override;

	virtual void onLayout(
		bool changed, bool sizeChanged,
		int left, int top, int right, int bottom) override;
	virtual void onSizeChanged(int width, int height, int oldWidth, int oldHeight) override;
	virtual void onFocusChanged(bool getFocus) override;
	virtual void onWindowFocusChanged(bool windowFocus) override;

	virtual void onScrollChanged(int scrollX, int scrollY, int oldScrollX, int oldScrollY) override;

	virtual bool onCheckIsTextEditor() override;
	virtual UInputConnection *onCreateInputConnection() override;

public:
	void onTextChanged(
		UEditable *editable,
		int start, int oldEnd, int newEnd) override;
	void onSelectionChanged(
		unsigned int ns, unsigned int ne,
		unsigned int os, unsigned int oe) override;
	void onSpanChanged(
		USpan *span, SpanChange action) override;

	bool onCreateActionMode(TextActionMode *mode, UMenu *menu) override;
	bool onPrepareActionMode(TextActionMode *mode, UMenu *menu) override;
	bool onActionItemClicked(TextActionMode *mode, UMenuItem *item) override;
	void onDestroyActionMode(TextActionMode *mode) override;
	void onGetContentPosition(int *x, int *y) override;
};