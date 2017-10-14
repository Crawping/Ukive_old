#include "UCommon.h"
#include "UTags.h"
#include "Button.h"
#include "TextView.h"
#include "ImageView.h"
#include "BaseLayout.h"
#include "LinearLayout.h"
#include "LayoutParams.h"
#include "UBitmapFactory.h"
#include "UColorDrawable.h"
#include "UnderlineSpan.h"
#include "UApplication.h"
#include "UDeviceManager.h"
#include "ScrollView.h"
#include "TestWindow.h"


TestWindow::TestWindow(UApplication *app)
	:UWindow(app)
{
}

TestWindow::TestWindow(UApplication *app, int id)
	: UWindow(app, id)
{
}

TestWindow::~TestWindow()
{
}


void TestWindow::onCreate()
{
	UWindow::onCreate();

	ScrollView *scrollView = new ScrollView(this);
	scrollView->setLayoutParams(
		new LayoutParams(LayoutParams::MATCH_PARENT, LayoutParams::MATCH_PARENT));

	setContentView(scrollView);

	LinearLayout *linearLayout = new LinearLayout(this);
	scrollView->addWidget(linearLayout, 
		new LayoutParams(LayoutParams::MATCH_PARENT, LayoutParams::MATCH_PARENT));


	DXGI_ADAPTER_DESC1 adapterDesc;
	DXGI_OUTPUT_DESC outputDesc;

	auto adapter = getApplication()->getDeviceManager()->getCurAdapter();
	auto output = getApplication()->getDeviceManager()->getCurOutput();
	adapter->GetDesc1(&adapterDesc);
	output->GetDesc(&outputDesc);

	std::wstring adapterName(adapterDesc.Description);
	std::wstring outputName(outputDesc.DeviceName);

	std::wstring deviceDesc = L"";
	deviceDesc.append(L"Device: ").append(adapterName)
		.append(L"\n").append(L"Monitor: ").append(outputName);


	TextView *deviceTextView = new TextView(this);
	deviceTextView->setIsSelectable(false);
	deviceTextView->setIsEditable(false);
	deviceTextView->setText(deviceDesc);
	deviceTextView->setPadding(6, 6, 6, 6);

	LayoutParams *deviceTextParams = new LayoutParams(
		LayoutParams::FIT_CONTENT,
		LayoutParams::FIT_CONTENT);
	deviceTextParams->leftMargin = deviceTextParams->rightMargin
		= deviceTextParams->topMargin = deviceTextParams->bottomMargin = 12;

	linearLayout->addWidget(deviceTextView, deviceTextParams);


	LayoutParams *textParams = new LayoutParams(
		LayoutParams::MATCH_PARENT,
		LayoutParams::FIT_CONTENT);
	textParams->leftMargin = textParams->rightMargin
		= textParams->topMargin = textParams->bottomMargin = 12;

	TextView *textView = new TextView(this, UWidgetId::TEXTVIEW);
	textView->setIsSelectable(true);
	textView->setIsEditable(true);
	textView->setText(L"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii这是一个示例程序，\n\n在这里可以显示文本。\n这是一个示例程序，\n在这里可以显示文本。\n这是一个示例程序，\n在这里可以显示文本。");
	textView->setBackground(new UColorDrawable(this, D2D1::ColorF(D2D1::ColorF::White)));
	textView->setPadding(6, 6, 6, 6);
	textView->setElevation(3.f);

	UnderlineSpan *span = new UnderlineSpan(3, 5);
	textView->getEditable()->addSpan(span);

	linearLayout->addWidget(textView, textParams);

	std::wstring imgFileName(::_wgetcwd(nullptr, 0));
	auto bitmap = this->getBitmapFactory()->decodeFile(imgFileName + L"\\freshpaint.png");
	ImageView *imageView = new ImageView(this, UWidgetId::IMAGEVIEW);
	imageView->setImageBitmap(bitmap);

	linearLayout->addWidget(imageView);


	Button *button = new Button(this);
	LayoutParams *buttonParams = new LayoutParams(
		LayoutParams::FIT_CONTENT,
		LayoutParams::FIT_CONTENT);
	buttonParams->leftMargin = buttonParams->rightMargin
		= buttonParams->topMargin = buttonParams->bottomMargin = 12;

	linearLayout->addWidget(button, buttonParams);


	/*UScene *scene = new TerrainScene(this->getSceneManager());
	HRESULT hr = scene->init(this->getD3DDrawer(), mWidth, mHeight);
	if (FAILED(hr))
		return;

	this->getSceneManager()->addScene(scene, U3DSceneId::TERRAIN_SCENE);*/
}