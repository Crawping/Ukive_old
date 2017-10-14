#include "UCommon.h"
#include "UTags.h"
#include "UColor.h"
#include "UFloat.h"
#include "Button.h"
#include "SeekBar.h"
#include "TextView.h"
#include "Direct3DView.h"
#include "LayoutParams.h"
#include "FrameLayout.h"
#include "LinearLayout.h"
#include "ViewAnimator.h"
#include "LinearLayoutParams.h"
#include "UEditTextDrawable.h"
#include "RestraintLayout.h"
#include "RestraintLayoutParams.h"
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
	LinearLayout *rootLayout = new LinearLayout(this);
	rootLayout->setOrientation(LinearLayout::HORIZONTAL);
	rootLayout->setLayoutParams(new LayoutParams(
		LayoutParams::MATCH_PARENT,
		LayoutParams::MATCH_PARENT));

	//3d view.
	Direct3DView *d3dView = new Direct3DView(this);
	LinearLayoutParams *d3dViewLp = new LinearLayoutParams(
		0, LayoutParams::MATCH_PARENT);
	d3dViewLp->leftMargin = d3dViewLp->topMargin 
		= d3dViewLp->rightMargin = d3dViewLp->bottomMargin = 8;
	d3dViewLp->weight = 2;
	d3dView->setLayoutParams(d3dViewLp);
	d3dView->setBackground(new UColorDrawable(this, UColor::White));
	d3dView->setElevation(2);

	mDirect3DView = d3dView;

	//right view.
	RestraintLayout *rightLayout = new RestraintLayout(this, ID_RIGHT_RESTRAIN);
	LinearLayoutParams *rightViewLp = new LinearLayoutParams(
		0, LayoutParams::MATCH_PARENT);
	rightViewLp->weight = 1;
	rightLayout->setLayoutParams(rightViewLp);

	////////////////////Right View Panel.

	inflateCtlLayout(rightLayout);

	////////////////////End Panel////////////////////

	rootLayout->addWidget(d3dView);
	rootLayout->addWidget(rightLayout);

	setContentView(rootLayout);
}


void LodWindow::inflateCtlLayout(RestraintLayout *rightLayout)
{
	/////////////////////////////第一行//////////////////////////////////
	//c1 label.
	TextView *c1Label = new TextView(this, ID_C1_LABEL);
	c1Label->setIsEditable(false);
	c1Label->setIsSelectable(false);
	c1Label->setText(L"C1值:");
	c1Label->setTextSize(13.f);

	RestraintLayoutParams *c1LabelLp = new RestraintLayoutParams(
		LayoutParams::FIT_CONTENT, LayoutParams::FIT_CONTENT);
	c1LabelLp
		->startHandle(ID_RIGHT_RESTRAIN, RestraintLayoutParams::START, 8)
		->topHandle(ID_RIGHT_RESTRAIN, RestraintLayoutParams::TOP, 12);

	rightLayout->addWidget(c1Label, c1LabelLp);

	//c1 seekbar.
	SeekBar *c1SeekBar = new SeekBar(this, ID_C1_SEEKBAR);
	c1SeekBar->setMaximum(60.f);
	c1SeekBar->setProgress(2.f-1.f);
	c1SeekBar->setOnSeekValueChangedListener(this);

	RestraintLayoutParams *c1SeekBarLp = new RestraintLayoutParams(
		LayoutParams::MATCH_PARENT, LayoutParams::FIT_CONTENT);
	c1SeekBarLp
		->startHandle(ID_C1_LABEL, RestraintLayoutParams::END, 4)
		->topHandle(ID_C1_LABEL, RestraintLayoutParams::TOP)
		->endHandle(ID_C1_VALUE, RestraintLayoutParams::START, 4)
		->bottomHandle(ID_C1_LABEL, RestraintLayoutParams::BOTTOM);

	mC1SeekBar = c1SeekBar;
	rightLayout->addWidget(c1SeekBar, c1SeekBarLp);

	//c1 value.
	TextView *c1Value = new TextView(this, ID_C1_VALUE);
	c1Value->setIsEditable(false);
	c1Value->setIsSelectable(false);
	c1Value->setText(L"2.00");
	c1Value->setTextSize(13.f);

	RestraintLayoutParams *c1ValueLp = new RestraintLayoutParams(
		36, LayoutParams::FIT_CONTENT);
	c1ValueLp
		->topHandle(ID_C1_LABEL, RestraintLayoutParams::TOP)
		->endHandle(ID_RIGHT_RESTRAIN, RestraintLayoutParams::END, 8)
		->bottomHandle(ID_C1_LABEL, RestraintLayoutParams::BOTTOM);

	mC1ValueTV = c1Value;
	rightLayout->addWidget(c1Value, c1ValueLp);

	/////////////////////////////第二行//////////////////////////////////
	//c2 label.
	TextView *c2Label = new TextView(this, ID_C2_LABEL);
	c2Label->setIsEditable(false);
	c2Label->setIsSelectable(false);
	c2Label->setText(L"C2值:");
	c2Label->setTextSize(13.f);

	RestraintLayoutParams *c2LabelLp = new RestraintLayoutParams(
		LayoutParams::FIT_CONTENT, LayoutParams::FIT_CONTENT);
	c2LabelLp
		->startHandle(ID_RIGHT_RESTRAIN, RestraintLayoutParams::START, 8)
		->topHandle(ID_C1_LABEL, RestraintLayoutParams::BOTTOM, 8);

	rightLayout->addWidget(c2Label, c2LabelLp);

	//c2 seekbar.
	SeekBar *c2SeekBar = new SeekBar(this, ID_C2_SEEKBAR);
	c2SeekBar->setMaximum(60.f);
	c2SeekBar->setProgress(30.f-1.f);
	c2SeekBar->setOnSeekValueChangedListener(this);

	RestraintLayoutParams *c2SeekBarLp = new RestraintLayoutParams(
		LayoutParams::MATCH_PARENT, LayoutParams::FIT_CONTENT);
	c2SeekBarLp
		->startHandle(ID_C2_LABEL, RestraintLayoutParams::END, 4)
		->topHandle(ID_C2_LABEL, RestraintLayoutParams::TOP)
		->endHandle(ID_C2_VALUE, RestraintLayoutParams::START, 4)
		->bottomHandle(ID_C2_LABEL, RestraintLayoutParams::BOTTOM);

	mC2SeekBar = c2SeekBar;
	rightLayout->addWidget(c2SeekBar, c2SeekBarLp);

	//c2 value.
	TextView *c2Value = new TextView(this, ID_C2_VALUE);
	c2Value->setIsEditable(false);
	c2Value->setIsSelectable(false);
	c2Value->setText(L"30.00");
	c2Value->setTextSize(13.f);

	RestraintLayoutParams *c2ValueLp = new RestraintLayoutParams(
		36, LayoutParams::FIT_CONTENT);
	c2ValueLp
		->topHandle(ID_C2_LABEL, RestraintLayoutParams::TOP)
		->endHandle(ID_RIGHT_RESTRAIN, RestraintLayoutParams::END, 8)
		->bottomHandle(ID_C2_LABEL, RestraintLayoutParams::BOTTOM);

	mC2ValueTV = c2Value;
	rightLayout->addWidget(c2Value, c2ValueLp);

	/////////////////////////////第三行//////////////////////////////////
	//split label.
	TextView *splitLabel = new TextView(this, ID_SPLIT_LABEL);
	splitLabel->setIsEditable(false);
	splitLabel->setIsSelectable(false);
	splitLabel->setText(L"分割:");
	splitLabel->setTextSize(13.f);

	RestraintLayoutParams *splitLabelLp = new RestraintLayoutParams(
		LayoutParams::FIT_CONTENT, LayoutParams::FIT_CONTENT);
	splitLabelLp->startHandledId = ID_RIGHT_RESTRAIN;
	splitLabelLp->startHandledEdge = RestraintLayoutParams::START;
	splitLabelLp->leftMargin = 8;
	splitLabelLp->topHandledId = ID_C2_LABEL;
	splitLabelLp->topHandledEdge = RestraintLayoutParams::BOTTOM;
	splitLabelLp->topMargin = 16;

	rightLayout->addWidget(splitLabel, splitLabelLp);

	//split seekbar.
	SeekBar *splitSeekBar = new SeekBar(this, ID_SPLIT_SEEKBAR);
	splitSeekBar->setMaximum(10.f);
	splitSeekBar->setProgress(5.f-1.f);
	splitSeekBar->setOnSeekValueChangedListener(this);

	RestraintLayoutParams *splitSeekBarLp = new RestraintLayoutParams(
		LayoutParams::MATCH_PARENT, LayoutParams::FIT_CONTENT);
	splitSeekBarLp
		->startHandle(ID_C2_SEEKBAR, RestraintLayoutParams::START)
		->topHandle(ID_SPLIT_LABEL, RestraintLayoutParams::TOP)
		->endHandle(ID_SPLIT_VALUE, RestraintLayoutParams::START, 4)
		->bottomHandle(ID_SPLIT_LABEL, RestraintLayoutParams::BOTTOM);

	mSplitSeekBar = splitSeekBar;
	rightLayout->addWidget(splitSeekBar, splitSeekBarLp);

	//split value.
	TextView *splitValue = new TextView(this, ID_SPLIT_VALUE);
	splitValue->setIsEditable(false);
	splitValue->setIsSelectable(false);
	splitValue->setText(L"5");
	splitValue->setTextSize(13.f);

	RestraintLayoutParams *splitValueLp = new RestraintLayoutParams(
		36, LayoutParams::FIT_CONTENT);
	splitValueLp->topHandledId = ID_SPLIT_LABEL;
	splitValueLp->topHandledEdge = RestraintLayoutParams::TOP;
	splitValueLp->bottomHandledId = ID_SPLIT_LABEL;
	splitValueLp->bottomHandledEdge = RestraintLayoutParams::BOTTOM;
	splitValueLp->endHandledId = ID_RIGHT_RESTRAIN;
	splitValueLp->endHandledEdge = RestraintLayoutParams::END;
	splitValueLp->rightMargin = 8;

	mSplitValueTV = splitValue;
	rightLayout->addWidget(splitValue, splitValueLp);

	
	//submit button.
	Button *submitBT = new Button(this, ID_SUBMIT_BUTTON);
	submitBT->setText(L"提交");
	submitBT->setTextSize(12.f);
	submitBT->setOnClickListener(this);

	RestraintLayoutParams *submitBTLp = new RestraintLayoutParams(
		LayoutParams::FIT_CONTENT, LayoutParams::FIT_CONTENT);
	submitBTLp->topMargin = 16;
	submitBTLp->topHandledId = ID_SPLIT_LABEL;
	submitBTLp->topHandledEdge = RestraintLayoutParams::BOTTOM;
	submitBTLp->endHandledId = ID_RIGHT_RESTRAIN;
	submitBTLp->endHandledEdge = RestraintLayoutParams::END;
	submitBTLp->rightMargin = 8;

	rightLayout->addWidget(submitBT, submitBTLp);

	//vsync button.
	Button *vsyncBT = new Button(this, ID_VSYNC_BUTTON);
	vsyncBT->setText(L"VSYNC ON");
	vsyncBT->setTextSize(12.f);
	vsyncBT->setTextColor(UColor::White);
	vsyncBT->setTextWeight(DWRITE_FONT_WEIGHT_BOLD);
	vsyncBT->setButtonColor(UColor::Blue500);
	vsyncBT->setOnClickListener(this);

	RestraintLayoutParams *vsyncBTLp = new RestraintLayoutParams(
		LayoutParams::FIT_CONTENT, LayoutParams::FIT_CONTENT);
	vsyncBTLp->topMargin = 8;
	vsyncBTLp->topHandledId = ID_SUBMIT_BUTTON;
	vsyncBTLp->topHandledEdge = RestraintLayoutParams::BOTTOM;
	vsyncBTLp->endHandledId = ID_RIGHT_RESTRAIN;
	vsyncBTLp->endHandledEdge = RestraintLayoutParams::END;
	vsyncBTLp->rightMargin = 8;

	rightLayout->addWidget(vsyncBT, vsyncBTLp);


	//渲染参数显示器
	TextView *renderInfoTV = new TextView(this, ID_LOD_INFO);
	renderInfoTV->setIsEditable(false);
	renderInfoTV->setIsSelectable(false);
	renderInfoTV->setText(L"Render info.");
	renderInfoTV->setTextSize(12.f);

	RestraintLayoutParams *renderInfoTVLp = new RestraintLayoutParams(
		LayoutParams::MATCH_PARENT, LayoutParams::FIT_CONTENT);
	renderInfoTVLp
		->startHandle(ID_RIGHT_RESTRAIN, RestraintLayoutParams::START, 8)
		->topHandle(ID_VSYNC_BUTTON, RestraintLayoutParams::BOTTOM, 16)
		->endHandle(ID_RIGHT_RESTRAIN, RestraintLayoutParams::END, 8);

	rightLayout->addWidget(renderInfoTV, renderInfoTVLp);


	//帮助说明
	TextView *helperTV = new TextView(this);
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

	RestraintLayoutParams *helperTVLp = new RestraintLayoutParams(
		LayoutParams::MATCH_PARENT, LayoutParams::FIT_CONTENT);
	helperTVLp
		->startHandle(ID_RIGHT_RESTRAIN, RestraintLayoutParams::START, 8)
		->topHandle(ID_LOD_INFO, RestraintLayoutParams::BOTTOM, 24)
		->endHandle(ID_RIGHT_RESTRAIN, RestraintLayoutParams::END, 8);

	rightLayout->addWidget(helperTV, helperTVLp);
}


void LodWindow::onClick(View *widget)
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
		Button *vsyncButton = (Button*)widget;
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

void LodWindow::onSeekValueChanged(SeekBar *seekBar, float value)
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

void LodWindow::onSeekIntegerValueChanged(SeekBar *seekBar, int value)
{
	switch (seekBar->getId())
	{
	case ID_SPLIT_SEEKBAR:
		mSplitValueTV->setText(std::to_wstring(1 + value));
		break;
	}
}