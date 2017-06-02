#include "UCommon.h"
#include "UMenu.h"
#include "UMenuImpl.h"
#include "UColor.h"
#include "UWindow.h"
#include "UShapeDrawable.h"
#include "UInnerWindow.h"
#include "UWidgetGroup.h"
#include "ULayoutParams.h"
#include "TextActionModeCallback.h"
#include "UWidgetAnimator.h"
#include "TextActionMode.h"


TextActionMode::TextActionMode(
	UWindow *window, TextActionModeCallback *callback)
{
	mIsFinished = true;
	mMenuWidth = 92;
	mMenuItemHeight = 36;

	mWindow = window;
	mCallback = callback;

	mMenu = new UMenuImpl(window);
	mMenu->setCallback(this);
	mMenu->setMenuItemHeight(mMenuItemHeight);

	UShapeDrawable *shapeDrawable
		= new UShapeDrawable(window, UShapeDrawable::SHAPE_ROUND_RECT);
	shapeDrawable->setRadius(2.f);
	shapeDrawable->setSolidEnable(true);
	shapeDrawable->setSolidColor(UColor::White);

	mInnerWindow = std::shared_ptr<UInnerWindow>(new UInnerWindow(window));
	mInnerWindow->setElevation(2.f);
	mInnerWindow->setContentView(mMenu);
	mInnerWindow->setOutsideTouchable(true);
	mInnerWindow->setBackground(shapeDrawable);
	mInnerWindow->setWidth(mMenuWidth);
}

TextActionMode::~TextActionMode()
{
}


void TextActionMode::onCreateMenu(UMenu *menu)
{
}

void TextActionMode::onPrepareMenu(UMenu *menu)
{
}

bool TextActionMode::onMenuItemClicked(UMenu *menu, UMenuItem *item)
{
	return mCallback->onActionItemClicked(this, item);
}


UMenu *TextActionMode::getMenu()
{
	return mMenu;
}


void TextActionMode::invalidateMenu()
{
	mCallback->onPrepareActionMode(this, mMenu);
}

void TextActionMode::invalidatePosition()
{
	int x, y;
	mCallback->onGetContentPosition(&x, &y);

	int windowWidth = mWindow->getClientWidth();
	int windowHeight = mWindow->getClientHeight();
	if (x + mMenuWidth > windowWidth)
		x -= mMenuWidth;

	mInnerWindow->update(x, y);
}

void TextActionMode::show()
{
	if (!mIsFinished) return;

	int x, y;
	mCallback->onGetContentPosition(&x, &y);

	int cCenterX = 0, cCenterY = 0;
	int windowWidth = mWindow->getClientWidth();
	int windowHeight = mWindow->getClientHeight();
	if (x + mMenuWidth > windowWidth)
	{
		cCenterX = mMenuWidth;
		cCenterY = 0;
		x -= mMenuWidth;
	}

	mInnerWindow->show(x, y);

	UWidgetAnimator::createCirculeReveal(
		mInnerWindow->getDecorView(), cCenterX, cCenterY, 0, 150)->start();

	mIsFinished = false;
}

void TextActionMode::close()
{
	if (mIsFinished)
		return;

	mIsFinished = true;

	mCallback->onDestroyActionMode(this);

	//异步关闭TextActionMode菜单，以防止在输入事件处理流程中
	//关闭菜单时出现问题。
	class TextActionModeWorker
		: public UExecutable
	{
	private:
		std::shared_ptr<UInnerWindow> window;
	public:
		TextActionModeWorker(std::shared_ptr<UInnerWindow> w)
		{
			window = w;
		}
		void run() override
		{
			class DismissAnimListener
				: public UAnimator::OnAnimatorListener
			{
			private:
				std::shared_ptr<UInnerWindow> window;
			public:
				DismissAnimListener(std::shared_ptr<UInnerWindow> w)
				{
					window = w;
				}
				void onAnimationStart(UAnimator *animator){}
				void onAnimationEnd(UAnimator *animator)
				{
					window->dismiss();
				}
				void onAnimationCancel(UAnimator *animator)
				{
					window->dismiss();
				}
			}*animListener = new DismissAnimListener(window);

			window->getDecorView()->animate()->
				setDuration(0.1)->alpha(0.f)->setListener(animListener)->start();
		}
	}*worker = new TextActionModeWorker(mInnerWindow);

	mWindow->getCycler()->post(worker);
}