#include "UCommon.h"
#include "UMenu.h"
#include "UMenuImpl.h"
#include "UColor.h"
#include "UWindow.h"
#include "UColorDrawable.h"
#include "UShapeDrawable.h"
#include "UInnerWindow.h"
#include "ViewGroup.h"
#include "LayoutParams.h"
#include "UContextMenuCallback.h"
#include "ViewAnimator.h"
#include "UContextMenu.h"


UContextMenu::UContextMenu(
	UWindow *window, UContextMenuCallback *callback)
{
	mIsFinished = true;
	mMenuWidth = 92;
	mMenuItemHeight = 36;

	mWindow = window;
	mCallback = callback;

	mMenu = new UMenuImpl(window);
	mMenu->setCallback(this);
	mMenu->setMenuItemHeight(mMenuItemHeight);

	mInnerWindow = std::make_shared<UInnerWindow>(window);
	mInnerWindow->setElevation(1.5f);
	mInnerWindow->setContentView(mMenu);
	mInnerWindow->setOutsideTouchable(true);
	mInnerWindow->setDismissByTouchOutside(true);
	mInnerWindow->setBackground(new UColorDrawable(window, UColor::White));
	mInnerWindow->setWidth(mMenuWidth);
}

UContextMenu::~UContextMenu()
{
}


void UContextMenu::onCreateMenu(UMenu *menu)
{
}

void UContextMenu::onPrepareMenu(UMenu *menu)
{
}

bool UContextMenu::onMenuItemClicked(UMenu *menu, UMenuItem *item)
{
	return mCallback->onContextMenuItemClicked(this, item);
}


UMenu *UContextMenu::getMenu()
{
	return mMenu;
}


void UContextMenu::invalidateMenu()
{
	if (mIsFinished) return;

	mCallback->onPrepareContextMenu(this, mMenu);
}

void UContextMenu::show(int x, int y)
{
	if (!mIsFinished) return;

	mInnerWindow->show(x, y);

	//UWidgetAnimator::createCirculeReveal(
		//mInnerWindow->getDecorView(), cCenterX, cCenterY, 0, 150)->start();

	mIsFinished = false;
}

void UContextMenu::close()
{
	if (mIsFinished)
		return;

	mIsFinished = true;
	mCallback->onDestroyContextMenu(this);

	//异步关闭TextActionMode菜单，以防止在输入事件处理流程中
	//关闭菜单时出现问题。
	class UContextMenuWorker
		: public UExecutable
	{
	private:
		std::shared_ptr<UInnerWindow> window;
	public:
		UContextMenuWorker(std::shared_ptr<UInnerWindow> w)
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
				void onAnimationStart(UAnimator *animator) {}
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
	}*worker = new UContextMenuWorker(mInnerWindow);

	mWindow->getCycler()->post(worker);
}