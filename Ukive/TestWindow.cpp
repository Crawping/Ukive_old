#include "UCommon.h"
#include "UTags.h"
#include "UButton.h"
#include "UTextView.h"
#include "UImageView.h"
#include "UBaseLayout.h"
#include "ULinearLayout.h"
#include "ULayoutParams.h"
#include "UBitmapFactory.h"
#include "UColorDrawable.h"
#include "UnderlineSpan.h"
#include "UApplication.h"
#include "UDeviceManager.h"
#include "UScrollView.h"
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

	UScrollView *scrollView = new UScrollView(this);
	scrollView->setLayoutParams(
		new ULayoutParams(ULayoutParams::MATCH_PARENT, ULayoutParams::MATCH_PARENT));

	setContentView(scrollView);

	ULinearLayout *linearLayout = new ULinearLayout(this);
	scrollView->addWidget(linearLayout, 
		new ULayoutParams(ULayoutParams::MATCH_PARENT, ULayoutParams::MATCH_PARENT));


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


	UTextView *deviceTextView = new UTextView(this);
	deviceTextView->setIsSelectable(false);
	deviceTextView->setIsEditable(false);
	deviceTextView->setText(deviceDesc);
	deviceTextView->setPadding(6, 6, 6, 6);

	ULayoutParams *deviceTextParams = new ULayoutParams(
		ULayoutParams::FIT_CONTENT,
		ULayoutParams::FIT_CONTENT);
	deviceTextParams->leftMargin = deviceTextParams->rightMargin
		= deviceTextParams->topMargin = deviceTextParams->bottomMargin = 12;

	linearLayout->addWidget(deviceTextView, deviceTextParams);


	ULayoutParams *textParams = new ULayoutParams(
		ULayoutParams::MATCH_PARENT,
		ULayoutParams::FIT_CONTENT);
	textParams->leftMargin = textParams->rightMargin
		= textParams->topMargin = textParams->bottomMargin = 12;

	UTextView *textView = new UTextView(this, UWidgetId::TEXTVIEW);
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
	UImageView *imageView = new UImageView(this, UWidgetId::IMAGEVIEW);
	imageView->setImageBitmap(bitmap);

	linearLayout->addWidget(imageView);


	UButton *button = new UButton(this);
	ULayoutParams *buttonParams = new ULayoutParams(
		ULayoutParams::FIT_CONTENT,
		ULayoutParams::FIT_CONTENT);
	buttonParams->leftMargin = buttonParams->rightMargin
		= buttonParams->topMargin = buttonParams->bottomMargin = 12;

	linearLayout->addWidget(button, buttonParams);


	/*UScene *scene = new TerrainScene(this->getSceneManager());
	HRESULT hr = scene->init(this->getD3DDrawer(), mWidth, mHeight);
	if (FAILED(hr))
		return;

	this->getSceneManager()->addScene(scene, U3DSceneId::TERRAIN_SCENE);*/
}