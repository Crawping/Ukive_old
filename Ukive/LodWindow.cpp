#include "UCommon.h"
#include "UTags.h"
#include "UColor.h"
#include "UFloat.h"
#include "UButton.h"
#include "USeekBar.h"
#include "UTextView.h"
#include "UDirect3DView.h"
#include "ULayoutParams.h"
#include "UFrameLayout.h"
#include "ULinearLayout.h"
#include "UWidgetAnimator.h"
#include "ULinearLayoutParams.h"
#include "UEditTextDrawable.h"
#include "RestrainLayout.h"
#include "RestrainLayoutParams.h"
#include "UColorDrawable.h"
#include "TerrainScene.h"
#include "UApplication.h"
#include "LodWindow.h"


LodWindow::LodWindow(UApplication *app)
	:UWindow(app)
{
}

LodWindow::LodWindow(UApplication *app, int id)
	: UWindow(app, id)
{
}

LodWindow::~LodWindow()
{
}


void LodWindow::onCreate()
{
	UWindow::onCreate();

	//root layout.
	ULinearLayout *rootLayout = new ULinearLayout(this);
	rootLayout->setOrientation(ULinearLayout::HORIZONTAL);
	rootLayout->setLayoutParams(new ULayoutParams(
		ULayoutParams::MATCH_PARENT,
		ULayoutParams::MATCH_PARENT));

	//3d view.
	UDirect3DView *d3dView = new UDirect3DView(this);
	ULinearLayoutParams *d3dViewLp = new ULinearLayoutParams(
		0, ULayoutParams::MATCH_PARENT);
	d3dViewLp->leftMargin = d3dViewLp->topMargin 
		= d3dViewLp->rightMargin = d3dViewLp->bottomMargin = 8;
	d3dViewLp->weight = 2;
	d3dView->setLayoutParams(d3dViewLp);
	d3dView->setBackground(new UColorDrawable(this, UColor::White));
	d3dView->setElevation(2);

	mDirect3DView = d3dView;

	//right view.
	RestrainLayout *rightLayout = new RestrainLayout(this, ID_RIGHT_RESTRAIN);
	ULinearLayoutParams *rightViewLp = new ULinearLayoutParams(
		0, ULayoutParams::MATCH_PARENT);
	rightViewLp->weight = 1;
	rightLayout->setLayoutParams(rightViewLp);

	////////////////////Right View Panel.

	inflateCtlLayout(rightLayout);

	////////////////////End Panel////////////////////

	rootLayout->addWidget(d3dView);
	rootLayout->addWidget(rightLayout);

	setContentView(rootLayout);
}


void LodWindow::inflateCtlLayout(RestrainLayout *rightLayout)
{
	/////////////////////////////第一行//////////////////////////////////
	//c1 label.
	UTextView *c1Label = new UTextView(this, ID_C1_LABEL);
	c1Label->setIsEditable(false);
	c1Label->setIsSelectable(false);
	c1Label->setText(L"C1值:");
	c1Label->setTextSize(13.f);

	RestrainLayoutParams *c1LabelLp = new RestrainLayoutParams(
		ULayoutParams::FIT_CONTENT, ULayoutParams::FIT_CONTENT);
	c1LabelLp
		->startHandle(ID_RIGHT_RESTRAIN, RestrainLayoutParams::EDGE_START, 8)
		->topHandle(ID_RIGHT_RESTRAIN, RestrainLayoutParams::EDGE_TOP, 12);

	rightLayout->addWidget(c1Label, c1LabelLp);

	//c1 seekbar.
	USeekBar *c1SeekBar = new USeekBar(this, ID_C1_SEEKBAR);
	c1SeekBar->setMaximum(60.f);
	c1SeekBar->setProgress(2.f-1.f);
	c1SeekBar->setOnSeekValueChangedListener(this);

	RestrainLayoutParams *c1SeekBarLp = new RestrainLayoutParams(
		ULayoutParams::MATCH_PARENT, ULayoutParams::FIT_CONTENT);
	c1SeekBarLp
		->startHandle(ID_C1_LABEL, RestrainLayoutParams::EDGE_END, 4)
		->topHandle(ID_C1_LABEL, RestrainLayoutParams::EDGE_TOP)
		->endHandle(ID_C1_VALUE, RestrainLayoutParams::EDGE_START, 4)
		->bottomHandle(ID_C1_LABEL, RestrainLayoutParams::EDGE_BOTTOM);

	mC1SeekBar = c1SeekBar;
	rightLayout->addWidget(c1SeekBar, c1SeekBarLp);

	//c1 value.
	UTextView *c1Value = new UTextView(this, ID_C1_VALUE);
	c1Value->setIsEditable(false);
	c1Value->setIsSelectable(false);
	c1Value->setText(L"2.00");
	c1Value->setTextSize(13.f);

	RestrainLayoutParams *c1ValueLp = new RestrainLayoutParams(
		36, ULayoutParams::FIT_CONTENT);
	c1ValueLp
		->topHandle(ID_C1_LABEL, RestrainLayoutParams::EDGE_TOP)
		->endHandle(ID_RIGHT_RESTRAIN, RestrainLayoutParams::EDGE_END, 8)
		->bottomHandle(ID_C1_LABEL, RestrainLayoutParams::EDGE_BOTTOM);

	mC1ValueTV = c1Value;
	rightLayout->addWidget(c1Value, c1ValueLp);

	/////////////////////////////第二行//////////////////////////////////
	//c2 label.
	UTextView *c2Label = new UTextView(this, ID_C2_LABEL);
	c2Label->setIsEditable(false);
	c2Label->setIsSelectable(false);
	c2Label->setText(L"C2值:");
	c2Label->setTextSize(13.f);

	RestrainLayoutParams *c2LabelLp = new RestrainLayoutParams(
		ULayoutParams::FIT_CONTENT, ULayoutParams::FIT_CONTENT);
	c2LabelLp
		->startHandle(ID_RIGHT_RESTRAIN, RestrainLayoutParams::EDGE_START, 8)
		->topHandle(ID_C1_LABEL, RestrainLayoutParams::EDGE_BOTTOM, 8);

	rightLayout->addWidget(c2Label, c2LabelLp);

	//c2 seekbar.
	USeekBar *c2SeekBar = new USeekBar(this, ID_C2_SEEKBAR);
	c2SeekBar->setMaximum(60.f);
	c2SeekBar->setProgress(30.f-1.f);
	c2SeekBar->setOnSeekValueChangedListener(this);

	RestrainLayoutParams *c2SeekBarLp = new RestrainLayoutParams(
		ULayoutParams::MATCH_PARENT, ULayoutParams::FIT_CONTENT);
	c2SeekBarLp
		->startHandle(ID_C2_LABEL, RestrainLayoutParams::EDGE_END, 4)
		->topHandle(ID_C2_LABEL, RestrainLayoutParams::EDGE_TOP)
		->endHandle(ID_C2_VALUE, RestrainLayoutParams::EDGE_START, 4)
		->bottomHandle(ID_C2_LABEL, RestrainLayoutParams::EDGE_BOTTOM);

	mC2SeekBar = c2SeekBar;
	rightLayout->addWidget(c2SeekBar, c2SeekBarLp);

	//c2 value.
	UTextView *c2Value = new UTextView(this, ID_C2_VALUE);
	c2Value->setIsEditable(false);
	c2Value->setIsSelectable(false);
	c2Value->setText(L"30.00");
	c2Value->setTextSize(13.f);

	RestrainLayoutParams *c2ValueLp = new RestrainLayoutParams(
		36, ULayoutParams::FIT_CONTENT);
	c2ValueLp
		->topHandle(ID_C2_LABEL, RestrainLayoutParams::EDGE_TOP)
		->endHandle(ID_RIGHT_RESTRAIN, RestrainLayoutParams::EDGE_END, 8)
		->bottomHandle(ID_C2_LABEL, RestrainLayoutParams::EDGE_BOTTOM);

	mC2ValueTV = c2Value;
	rightLayout->addWidget(c2Value, c2ValueLp);

	/////////////////////////////第三行//////////////////////////////////
	//split label.
	UTextView *splitLabel = new UTextView(this, ID_SPLIT_LABEL);
	splitLabel->setIsEditable(false);
	splitLabel->setIsSelectable(false);
	splitLabel->setText(L"分割:");
	splitLabel->setTextSize(13.f);

	RestrainLayoutParams *splitLabelLp = new RestrainLayoutParams(
		ULayoutParams::FIT_CONTENT, ULayoutParams::FIT_CONTENT);
	splitLabelLp->startHandledId = ID_RIGHT_RESTRAIN;
	splitLabelLp->startHandledEdge = RestrainLayoutParams::EDGE_START;
	splitLabelLp->leftMargin = 8;
	splitLabelLp->topHandledId = ID_C2_LABEL;
	splitLabelLp->topHandledEdge = RestrainLayoutParams::EDGE_BOTTOM;
	splitLabelLp->topMargin = 16;

	rightLayout->addWidget(splitLabel, splitLabelLp);

	//split seekbar.
	USeekBar *splitSeekBar = new USeekBar(this, ID_SPLIT_SEEKBAR);
	splitSeekBar->setMaximum(10.f);
	splitSeekBar->setProgress(5.f-1.f);
	splitSeekBar->setOnSeekValueChangedListener(this);

	RestrainLayoutParams *splitSeekBarLp = new RestrainLayoutParams(
		ULayoutParams::MATCH_PARENT, ULayoutParams::FIT_CONTENT);
	splitSeekBarLp
		->startHandle(ID_C2_SEEKBAR, RestrainLayoutParams::EDGE_START)
		->topHandle(ID_SPLIT_LABEL, RestrainLayoutParams::EDGE_TOP)
		->endHandle(ID_SPLIT_VALUE, RestrainLayoutParams::EDGE_START, 4)
		->bottomHandle(ID_SPLIT_LABEL, RestrainLayoutParams::EDGE_BOTTOM);

	mSplitSeekBar = splitSeekBar;
	rightLayout->addWidget(splitSeekBar, splitSeekBarLp);

	//split value.
	UTextView *splitValue = new UTextView(this, ID_SPLIT_VALUE);
	splitValue->setIsEditable(false);
	splitValue->setIsSelectable(false);
	splitValue->setText(L"5");
	splitValue->setTextSize(13.f);

	RestrainLayoutParams *splitValueLp = new RestrainLayoutParams(
		36, ULayoutParams::FIT_CONTENT);
	splitValueLp->topHandledId = ID_SPLIT_LABEL;
	splitValueLp->topHandledEdge = RestrainLayoutParams::EDGE_TOP;
	splitValueLp->bottomHandledId = ID_SPLIT_LABEL;
	splitValueLp->bottomHandledEdge = RestrainLayoutParams::EDGE_BOTTOM;
	splitValueLp->endHandledId = ID_RIGHT_RESTRAIN;
	splitValueLp->endHandledEdge = RestrainLayoutParams::EDGE_END;
	splitValueLp->rightMargin = 8;

	mSplitValueTV = splitValue;
	rightLayout->addWidget(splitValue, splitValueLp);

	
	//submit button.
	UButton *submitBT = new UButton(this, ID_SUBMIT_BUTTON);
	submitBT->setText(L"提交");
	submitBT->setTextSize(12.f);
	submitBT->setOnClickListener(this);

	RestrainLayoutParams *submitBTLp = new RestrainLayoutParams(
		ULayoutParams::FIT_CONTENT, ULayoutParams::FIT_CONTENT);
	submitBTLp->topMargin = 16;
	submitBTLp->topHandledId = ID_SPLIT_LABEL;
	submitBTLp->topHandledEdge = RestrainLayoutParams::EDGE_BOTTOM;
	submitBTLp->endHandledId = ID_RIGHT_RESTRAIN;
	submitBTLp->endHandledEdge = RestrainLayoutParams::EDGE_END;
	submitBTLp->rightMargin = 8;

	rightLayout->addWidget(submitBT, submitBTLp);

	//vsync button.
	UButton *vsyncBT = new UButton(this, ID_VSYNC_BUTTON);
	vsyncBT->setText(L"VSYNC ON");
	vsyncBT->setTextSize(12.f);
	vsyncBT->setTextColor(UColor::White);
	vsyncBT->setTextWeight(DWRITE_FONT_WEIGHT_BOLD);
	vsyncBT->setButtonColor(UColor::Blue500);
	vsyncBT->setOnClickListener(this);

	RestrainLayoutParams *vsyncBTLp = new RestrainLayoutParams(
		ULayoutParams::FIT_CONTENT, ULayoutParams::FIT_CONTENT);
	vsyncBTLp->topMargin = 8;
	vsyncBTLp->topHandledId = ID_SUBMIT_BUTTON;
	vsyncBTLp->topHandledEdge = RestrainLayoutParams::EDGE_BOTTOM;
	vsyncBTLp->endHandledId = ID_RIGHT_RESTRAIN;
	vsyncBTLp->endHandledEdge = RestrainLayoutParams::EDGE_END;
	vsyncBTLp->rightMargin = 8;

	rightLayout->addWidget(vsyncBT, vsyncBTLp);


	//渲染参数显示器
	UTextView *renderInfoTV = new UTextView(this, ID_LOD_INFO);
	renderInfoTV->setIsEditable(false);
	renderInfoTV->setIsSelectable(false);
	renderInfoTV->setText(L"Render info.");
	renderInfoTV->setTextSize(12.f);

	RestrainLayoutParams *renderInfoTVLp = new RestrainLayoutParams(
		ULayoutParams::MATCH_PARENT, ULayoutParams::FIT_CONTENT);
	renderInfoTVLp
		->startHandle(ID_RIGHT_RESTRAIN, RestrainLayoutParams::EDGE_START, 8)
		->topHandle(ID_VSYNC_BUTTON, RestrainLayoutParams::EDGE_BOTTOM, 16)
		->endHandle(ID_RIGHT_RESTRAIN, RestrainLayoutParams::EDGE_END, 8);

	rightLayout->addWidget(renderInfoTV, renderInfoTVLp);


	//帮助说明
	UTextView *helperTV = new UTextView(this);
	helperTV->setIsEditable(true);
	helperTV->setIsSelectable(true);
	helperTV->setText(
		L"►拖动滑条，点击提交按钮来更改参数。\
		\n►鼠标滚轮可进行缩放。\
		\n►按住Shift键和鼠标左键拖动可旋转摄像机。\
		\n►按住Ctrl键和鼠标左键拖动可移动摄像机");
	helperTV->setTextSize(14.f);
	helperTV->setBackground(new UEditTextDrawable(this));
	helperTV->setPadding(4, 4, 4, 4);

	RestrainLayoutParams *helperTVLp = new RestrainLayoutParams(
		ULayoutParams::MATCH_PARENT, ULayoutParams::FIT_CONTENT);
	helperTVLp
		->startHandle(ID_RIGHT_RESTRAIN, RestrainLayoutParams::EDGE_START, 8)
		->topHandle(ID_LOD_INFO, RestrainLayoutParams::EDGE_BOTTOM, 24)
		->endHandle(ID_RIGHT_RESTRAIN, RestrainLayoutParams::EDGE_END, 8);

	rightLayout->addWidget(helperTV, helperTVLp);
}


void LodWindow::onClick(UWidget *widget)
{
	switch (widget->getId())
	{
	case ID_SUBMIT_BUTTON:
	{
		TerrainScene *scene = mDirect3DView->getTerrainScene();

		float c1 = mC1SeekBar->getProgress() + 1.f;
		float c2 = mC2SeekBar->getProgress() + 1.f;
		int splitCount = static_cast<int>(mSplitSeekBar->getProgress()) + 1;

		scene->recreate(splitCount);
		scene->reevaluate(c1, c2);
		scene->refresh();

		break;
	}

	case ID_VSYNC_BUTTON:
	{
		UButton *vsyncButton = (UButton*)widget;
		if (vsyncButton->getText() == L"VSYNC ON")
		{
			UApplication::setVSync(false);
			vsyncButton->setText(L"VSYNC OFF");
			vsyncButton->setButtonColor(UColor::Yellow800);
		}
		else if (vsyncButton->getText() == L"VSYNC OFF")
		{
			UApplication::setVSync(true);
			vsyncButton->setText(L"VSYNC ON");
			vsyncButton->setButtonColor(UColor::Blue500);
		}
		break;
	}
	}
}

void LodWindow::onSeekValueChanged(USeekBar *seekBar, float value)
{
	switch (seekBar->getId())
	{
	case ID_C1_SEEKBAR:
		mC1ValueTV->setText(UFloat::toString(1.f + value, 2));
		break;

	case ID_C2_SEEKBAR:
		mC2ValueTV->setText(UFloat::toString(1.f + value, 2));
		break;
	}
}

void LodWindow::onSeekIntegerValueChanged(USeekBar *seekBar, int value)
{
	switch (seekBar->getId())
	{
	case ID_SPLIT_SEEKBAR:
		mSplitValueTV->setText(std::to_wstring(1 + value));
		break;
	}
}