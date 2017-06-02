#include "UCommon.h"
#include "UMath.h"
#include "RestrainLayoutParams.h"
#include "RestrainLayout.h"


RestrainLayout::RestrainLayout(UWindow *wnd)
	: UWidgetGroup(wnd)
{
}

RestrainLayout::RestrainLayout(UWindow *wnd, int id)
	: UWidgetGroup(wnd, id)
{
}

RestrainLayout::~RestrainLayout()
{
}


ULayoutParams *RestrainLayout::generateLayoutParams(ULayoutParams *lp)
{
	return new RestrainLayoutParams(lp);
}

ULayoutParams *RestrainLayout::generateDefaultLayoutParams()
{
	return new RestrainLayoutParams(
		ULayoutParams::FIT_CONTENT,
		ULayoutParams::FIT_CONTENT);
}

bool RestrainLayout::checkLayoutParams(ULayoutParams *lp)
{
	return typeid(*lp) == typeid(RestrainLayoutParams);
}


UWidget *RestrainLayout::getChildById(int id)
{
	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		UWidget *child = getChildAt(i);
		if (child->getId() == id)
			return child;
	}

	throw std::runtime_error("child view id not existed.");
}

void RestrainLayout::clearMeasureFlag()
{
	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		UWidget *child = getChildAt(i);
		RestrainLayoutParams *lp = (RestrainLayoutParams*)child->getLayoutParams();
		lp->isWidthMeasured = lp->isHeightMeasured = false;
		lp->isVertLayouted = lp->isHoriLayouted = false;
	}
}


void RestrainLayout::measureRestrainedChildren(
	int parentWidth, int parentHeight,
	int parentWidthMode, int parentHeightMode)
{
	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		UWidget *child = getChildAt(i);
		RestrainLayoutParams *lp = (RestrainLayoutParams*)child->getLayoutParams();
		if (lp->isWidthMeasured)
			continue;

		int childWidth = 0;
		int childWidthMode = 0;

		getRestrainedChildWidth(
			child, lp,
			parentWidth, parentWidthMode,
			&childWidth, &childWidthMode);

		lp->isWidthMeasured = true;
		lp->specWidth = childWidth;
		lp->specWidthMode = childWidthMode;
	}

	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		UWidget *child = getChildAt(i);
		RestrainLayoutParams *lp = (RestrainLayoutParams*)child->getLayoutParams();
		if (lp->isWidthMeasured && lp->isHeightMeasured)
			continue;

		int childHeight = 0;
		int childHeightMode = 0;

		if (!lp->isHeightMeasured)
		{
			getRestrainedChildHeight(
				child, lp,
				parentHeight, parentHeightMode,
				&childHeight, &childHeightMode);

			lp->isHeightMeasured = true;
			lp->specHeight = childHeight;
			lp->specHeightMode = childHeightMode;
		}
		else
		{
			childHeight = lp->specHeight;
			childHeightMode = lp->specHeightMode;
		}

		child->measure(lp->specWidth, childHeight, lp->specWidthMode, childHeightMode);
	}
}

void RestrainLayout::measureRestrainedChild(
	UWidget *child, RestrainLayoutParams *lp,
	int parentWidth, int parentHeight, int parentWidthMode, int parentHeightMode)
{
	int childWidth = 0;
	int childWidthSpec = 0;

	int childHeight = 0;
	int childHeightSpec = 0;

	if (!lp->isWidthMeasured)
	{
		getRestrainedChildWidth(
			child, lp,
			parentWidth, parentWidthMode,
			&childWidth, &childWidthSpec);

		lp->isWidthMeasured = true;
		lp->specWidth = childWidth;
		lp->specWidthMode = childWidthSpec;
	}
	else
	{
		childWidth = lp->specWidth;
		childWidthSpec = lp->specWidthMode;
	}

	if (!lp->isHeightMeasured)
	{
		getRestrainedChildHeight(
			child, lp,
			parentHeight, parentHeightMode,
			&childHeight, &childHeightSpec);

		lp->isHeightMeasured = true;
		lp->specHeight = childHeight;
		lp->specHeightMode = childHeightSpec;
	}
	else
	{
		childHeight = lp->specHeight;
		childHeightSpec = lp->specHeightMode;
	}

	child->measure(childWidth, childHeight, childWidthSpec, childHeightSpec);
}

void RestrainLayout::getRestrainedChildWidth(
	UWidget *child, RestrainLayoutParams *lp,
	int parentWidth, int parentWidthMode, int *width, int *widthMode)
{
	int horizontalPadding = mPaddingLeft + mPaddingRight;
	int horizontalMargins = lp->leftMargin + lp->rightMargin;

	int childWidth = 0;
	int childWidthMode = 0;

	if (lp->hasHorizontalCouple())
	{
		int size = UMath::maximum(0,
			parentWidth - horizontalPadding - horizontalMargins);

		//child�й̶��Ĵ�С��
		if (lp->width > 0)
		{
			childWidth = lp->width;
			childWidthMode = EXACTLY;
			lp->horiCoupleHandlerType = RestrainLayoutParams::CH_FIXED;
		}
		//child�����handler couple֮�������
		else
		{
			//handler couple���ڸ�View��
			if (lp->startHandledId == this->getId()
				&& lp->endHandledId == this->getId())
			{
				switch (parentWidthMode)
				{
				case FIT:
				{
					childWidth = size;
					childWidthMode = FIT;
					lp->horiCoupleHandlerType = RestrainLayoutParams::CH_WRAP;
					break;
				}
				case EXACTLY:
				{
					if (lp->width == 0
						|| lp->width == ULayoutParams::MATCH_PARENT)
					{
						childWidth = size;
						childWidthMode = EXACTLY;
						lp->horiCoupleHandlerType = RestrainLayoutParams::CH_FILL;
					}
					else if (lp->width == ULayoutParams::FIT_CONTENT)
					{
						childWidth = size;
						childWidthMode = FIT;
						lp->horiCoupleHandlerType = RestrainLayoutParams::CH_WRAP;
					}
					break;
				}
				case UNKNOWN:
				{
					//�˴������ڲ�֪����View�Ĵ�С���޷����handler couple֮�������
					//��View�Լ�������С���ɡ�
					childWidth = size;
					childWidthMode = UNKNOWN;
					lp->horiCoupleHandlerType = RestrainLayoutParams::CH_FILL;
					break;
				}
				}
			}
			//handler��û�а��ڸ�View������ζ������������View�󶨡�
			//��ʱ�����Ȳ����󶨵���View��Ȼ����ܲ�����View����ʹ��
			//�ݹ���ɴ˹��̣�Ϊ����Ҫ����ı�������¼ĳһView�Ƿ��Ѳ�����
			else
			{
				//����Handler����ͬһ��View�ϡ�
				if (lp->startHandledId == lp->endHandledId)
				{
					int measuredTargetWidth = 0;
					UWidget *target = getChildById(lp->startHandledId);
					RestrainLayoutParams *targetLp = (RestrainLayoutParams*)target->getLayoutParams();

					//������target view�Ŀ�ȡ�
					int targetWidth;
					int targetWidthMode;
					if (!targetLp->isWidthMeasured)
					{
						getRestrainedChildWidth(
							target, targetLp,
							parentWidth, parentWidthMode,
							&targetWidth, &targetWidthMode);

						targetLp->isWidthMeasured = true;
						targetLp->specWidth = targetWidth;
						targetLp->specWidthMode = targetWidthMode;
					}
					else
					{
						targetWidth = targetLp->specWidth;
						targetWidthMode = targetLp->specWidthMode;
					}

					int targetHeight = 0;
					int targetHeightMode = UNKNOWN;
					if (targetLp->isHeightMeasured)
					{
						targetHeight = targetLp->specHeight;
						targetHeightMode = targetLp->specHeightMode;
					}

					//��target view��������
					//�⽫��ʹtarget view��onMeasure()���������һ�Ρ�
					target->measure(
						targetWidth, targetHeight,
						targetWidthMode, targetHeightMode);
					measuredTargetWidth = target->getMeasuredWidth();

					childWidth = UMath::maximum(0, measuredTargetWidth - horizontalMargins);

					if (lp->width == 0
						|| lp->width == ULayoutParams::MATCH_PARENT)
					{
						childWidthMode = EXACTLY;
						lp->horiCoupleHandlerType = RestrainLayoutParams::CH_FILL;
					}
					else if (lp->width == ULayoutParams::FIT_CONTENT)
					{
						childWidthMode = FIT;
						lp->horiCoupleHandlerType = RestrainLayoutParams::CH_WRAP;
					}
				}
				else
				{
					if (parentWidthMode == EXACTLY
						|| parentWidthMode == FIT)
					{
						//ǰ�������
						int measuredStartMargin = lp->leftMargin;
						RestrainLayoutParams *childLp = lp;
						while (childLp->hasStart()
							&& childLp->startHandledId != this->getId())
						{
							UWidget *target = getChildById(childLp->startHandledId);
							RestrainLayoutParams *targetLp = (RestrainLayoutParams*)target->getLayoutParams();

							//������target view�Ŀ�ȡ�
							int targetWidth;
							int targetWidthMode;
							if (!targetLp->isWidthMeasured)
							{
								getRestrainedChildWidth(
									target, targetLp,
									parentWidth, parentWidthMode,
									&targetWidth, &targetWidthMode);

								targetLp->isWidthMeasured = true;
								targetLp->specWidth = targetWidth;
								targetLp->specWidthMode = targetWidthMode;
							}
							else
							{
								targetWidth = targetLp->specWidth;
								targetWidthMode = targetLp->specWidthMode;
							}

							int targetHeight = 0;
							int targetHeightMode = UNKNOWN;
							if (targetLp->isHeightMeasured)
							{
								targetHeight = targetLp->specHeight;
								targetHeightMode = targetLp->specHeightMode;
							}

							//��target view��������
							//�⽫��ʹtarget view��onMeasure()���������һ�Ρ�
							target->measure(
								targetWidth, targetHeight,
								targetWidthMode, targetHeightMode);

							if (childLp->startHandledEdge
								== RestrainLayoutParams::EDGE_END)
							{
								measuredStartMargin += target->getMeasuredWidth()
									+ (targetLp->hasStart() ? targetLp->leftMargin : 0);
							}
							else if (childLp->startHandledEdge
								== RestrainLayoutParams::EDGE_START)
							{
								measuredStartMargin +=
									(targetLp->hasStart() ? targetLp->leftMargin : 0);
							}

							childLp = targetLp;
						}

						//���������
						int measuredEndMargin = lp->rightMargin;
						childLp = lp;
						while (childLp->hasEnd()
							&& childLp->endHandledId != this->getId())
						{
							UWidget *target = getChildById(childLp->endHandledId);
							RestrainLayoutParams *targetLp = (RestrainLayoutParams*)target->getLayoutParams();

							//������target view�Ŀ�ȡ�
							int targetWidth;
							int targetWidthMode;
							if (!targetLp->isWidthMeasured)
							{
								getRestrainedChildWidth(
									target, targetLp,
									parentWidth, parentWidthMode,
									&targetWidth, &targetWidthMode);

								targetLp->isWidthMeasured = true;
								targetLp->specWidth = targetWidth;
								targetLp->specWidthMode = targetWidthMode;
							}
							else
							{
								targetWidth = targetLp->specWidth;
								targetWidthMode = targetLp->specWidthMode;
							}

							int targetHeight = 0;
							int targetHeightMode = UNKNOWN;
							if (targetLp->isHeightMeasured)
							{
								targetHeight = targetLp->specHeight;
								targetHeightMode = targetLp->specHeightMode;
							}

							//��target view��������
							//�⽫��ʹtarget view��onMeasure()���������һ�Ρ�
							target->measure(
								targetWidth, targetHeight,
								targetWidthMode, targetHeightMode);

							if (childLp->endHandledEdge
								== RestrainLayoutParams::EDGE_START)
							{
								measuredEndMargin += target->getMeasuredWidth()
									+ (targetLp->hasEnd() ? targetLp->rightMargin : 0);
							}
							else if (childLp->endHandledEdge
								== RestrainLayoutParams::EDGE_END)
							{
								measuredEndMargin +=
									(targetLp->hasEnd() ? targetLp->rightMargin : 0);
							}

							childLp = targetLp;
						}

						childWidth = UMath::maximum(0, parentWidth - horizontalPadding
							- measuredStartMargin - measuredEndMargin);

						if (lp->width == 0
							|| lp->width == ULayoutParams::MATCH_PARENT)
						{
							if (parentWidthMode == FIT)
							{
								childWidthMode = FIT;
								lp->horiCoupleHandlerType = RestrainLayoutParams::CH_WRAP;
							}
							else if (parentWidthMode == EXACTLY)
							{
								childWidthMode = EXACTLY;
								lp->horiCoupleHandlerType = RestrainLayoutParams::CH_FILL;
							}
						}
						else if (lp->width == ULayoutParams::FIT_CONTENT)
						{
							childWidthMode = FIT;
							lp->horiCoupleHandlerType = RestrainLayoutParams::CH_WRAP;
						}
					}
					else if (parentWidthMode == UNKNOWN)
					{
						childWidth = size;
						childWidthMode = UNKNOWN;
						lp->horiCoupleHandlerType = RestrainLayoutParams::CH_FILL;
					}
				}
			}
		}
	}
	else
	{
		this->getChildMeasure(
			parentWidth, parentWidthMode,
			horizontalMargins + horizontalPadding,
			lp->width, &childWidth, &childWidthMode);
	}

	*width = childWidth;
	*widthMode = childWidthMode;
}

void RestrainLayout::getRestrainedChildHeight(
	UWidget *child, RestrainLayoutParams *lp,
	int parentHeight, int parentHeightMode, int *height, int *heightSpec)
{
	int verticalPadding = mPaddingTop + mPaddingBottom;
	int verticalMargins = lp->topMargin + lp->bottomMargin;

	int childHeight = 0;
	int childHeightMode = 0;

	if (lp->hasVerticalCouple())
	{
		int size = UMath::maximum(0,
			parentHeight - verticalPadding - verticalMargins);

		//child�й̶��Ĵ�С��
		if (lp->height > 0)
		{
			childHeight = lp->height;
			childHeightMode = EXACTLY;
			lp->vertCoupleHandlerType = RestrainLayoutParams::CH_FIXED;
		}
		//child�����handler couple֮�������
		else
		{
			//handler couple���ڸ�View��
			if (lp->topHandledId == this->getId()
				&& lp->bottomHandledId == this->getId())
			{
				switch (parentHeightMode)
				{
				case FIT:
				{
					childHeight = size;
					childHeightMode = FIT;
					lp->vertCoupleHandlerType = RestrainLayoutParams::CH_WRAP;
					break;
				}
				case EXACTLY:
				{
					if (lp->height == 0
						|| lp->height == ULayoutParams::MATCH_PARENT)
					{
						childHeight = size;
						childHeightMode = EXACTLY;
						lp->vertCoupleHandlerType = RestrainLayoutParams::CH_FILL;
					}
					else if (lp->height == ULayoutParams::FIT_CONTENT)
					{
						childHeight = size;
						childHeightMode = FIT;
						lp->vertCoupleHandlerType = RestrainLayoutParams::CH_WRAP;
					}
					break;
				}
				case UNKNOWN:
				{
					//�˴������ڲ�֪����View�Ĵ�С���޷����handler��֮�������
					//��View�Լ�������С���ɡ�
					childHeight = size;
					childHeightMode = UNKNOWN;
					lp->vertCoupleHandlerType = RestrainLayoutParams::CH_FILL;
					break;
				}
				}
			}
			//handler��û�а��ڸ�View������ζ������������View�󶨡�
			//��ʱ�����Ȳ����󶨵���View��Ȼ����ܲ�����View����ʹ��
			//�ݹ���ɴ˹��̣�Ϊ����Ҫ����ı�������¼ĳһView�Ƿ��Ѳ�����
			else
			{
				//����Handler����ͬһ��View�ϡ�
				if (lp->topHandledId == lp->bottomHandledId)
				{
					int measuredTargetHeight = 0;
					UWidget *target = getChildById(lp->topHandledId);
					RestrainLayoutParams *targetLp = (RestrainLayoutParams*)target->getLayoutParams();

					//������target view�ĸ߶ȡ�
					int targetHeight;
					int targetHeightMode;
					if (!targetLp->isHeightMeasured)
					{
						getRestrainedChildHeight(
							target, targetLp,
							parentHeight, parentHeightMode,
							&targetHeight, &targetHeightMode);

						targetLp->isHeightMeasured = true;
						targetLp->specHeight = targetHeight;
						targetLp->specHeightMode = targetHeightMode;
					}
					else
					{
						targetHeight = targetLp->specHeight;
						targetHeightMode = targetLp->specHeightMode;
					}

					int targetWidth = 0;
					int targetWidthMode = UNKNOWN;
					if (targetLp->isWidthMeasured)
					{
						targetWidth = targetLp->specWidth;
						targetWidthMode = targetLp->specWidthMode;
					}

					//��target view��������
					//�⽫��ʹtarget view��onMeasure()���������һ�Ρ�
					target->measure(
						targetWidth, targetHeight,
						targetWidthMode, targetHeightMode);
					measuredTargetHeight = target->getMeasuredHeight();

					childHeight = UMath::maximum(0, measuredTargetHeight - verticalMargins);

					if (lp->height == 0
						|| lp->height == ULayoutParams::MATCH_PARENT)
					{
						childHeightMode = EXACTLY;
						lp->vertCoupleHandlerType = RestrainLayoutParams::CH_FILL;
					}
					else if (lp->height == ULayoutParams::FIT_CONTENT)
					{
						childHeightMode = FIT;
						lp->vertCoupleHandlerType = RestrainLayoutParams::CH_WRAP;
					}
				}
				else
				{
					if (parentHeightMode == EXACTLY
						|| parentHeightMode == FIT)
					{
						//���������
						int measuredTopMargin = lp->topMargin;
						RestrainLayoutParams *childLp = lp;
						while (childLp->hasTop()
							&& childLp->topHandledId != this->getId())
						{
							UWidget *target = getChildById(childLp->topHandledId);
							RestrainLayoutParams *targetLp = (RestrainLayoutParams*)target->getLayoutParams();

							//������target view�ĸ߶ȡ�
							int targetHeight;
							int targetHeightMode;
							if (!targetLp->isHeightMeasured)
							{
								getRestrainedChildHeight(
									target, targetLp,
									parentHeight, parentHeightMode,
									&targetHeight, &targetHeightMode);

								targetLp->isHeightMeasured = true;
								targetLp->specHeight = childHeight;
								targetLp->specHeightMode = childHeightMode;
							}
							else
							{
								targetHeight = targetLp->specHeight;
								targetHeightMode = targetLp->specHeightMode;
							}

							int targetWidth = 0;
							int targetWidthMode = UNKNOWN;
							if (targetLp->isWidthMeasured)
							{
								targetWidth = targetLp->specWidth;
								targetWidthMode = targetLp->specWidthMode;
							}

							//��target view��������
							//�⽫��ʹtarget view��onMeasure()���������һ�Ρ�
							target->measure(
								targetWidth, targetHeight,
								targetWidthMode, targetHeightMode);

							if (childLp->topHandledEdge
								== RestrainLayoutParams::EDGE_BOTTOM)
							{
								measuredTopMargin += target->getMeasuredHeight()
									+ (targetLp->hasTop() ? targetLp->topMargin : 0);
							}
							else if (childLp->topHandledEdge
								== RestrainLayoutParams::EDGE_TOP)
							{
								measuredTopMargin +=
									(targetLp->hasTop() ? targetLp->topMargin : 0);
							}

							childLp = targetLp;
						}

						//���������
						int measuredBottomMargin = lp->bottomMargin;
						childLp = lp;
						while (childLp->hasBottom()
							&& childLp->bottomHandledId != this->getId())
						{
							UWidget *target = getChildById(childLp->bottomHandledId);
							RestrainLayoutParams *targetLp = (RestrainLayoutParams*)target->getLayoutParams();

							//������target view�ĸ߶ȡ�
							int targetHeight;
							int targetHeightMode;
							if (!targetLp->isHeightMeasured)
							{
								getRestrainedChildHeight(
									target, targetLp,
									parentHeight, parentHeightMode,
									&targetHeight, &targetHeightMode);

								targetLp->isHeightMeasured = true;
								targetLp->specHeight = targetHeight;
								targetLp->specHeightMode = targetHeightMode;
							}
							else
							{
								targetHeight = targetLp->specHeight;
								targetHeightMode = targetLp->specHeightMode;
							}

							int targetWidth = 0;
							int targetWidthMode = UNKNOWN;
							if (targetLp->isWidthMeasured)
							{
								targetWidth = targetLp->specWidth;
								targetWidthMode = targetLp->specWidthMode;
							}

							//��target view��������
							//�⽫��ʹtarget view��onMeasure()���������һ�Ρ�
							target->measure(
								targetWidth, targetHeight,
								targetWidthMode, targetHeightMode);

							if (childLp->bottomHandledEdge == RestrainLayoutParams::EDGE_TOP)
							{
								measuredBottomMargin += target->getMeasuredHeight()
									+ (targetLp->hasBottom() ? targetLp->bottomMargin : 0);
							}
							else if (childLp->bottomHandledEdge == RestrainLayoutParams::EDGE_BOTTOM)
							{
								measuredBottomMargin +=
									(targetLp->hasBottom() ? targetLp->bottomMargin : 0);
							}

							childLp = targetLp;
						}

						//TODO:���ﲻӦ����parentHeight����Ӧ��ʹ������������view
						//��Ϊ�߽������㡣�˴��ٶ����հ󶨵�parent�߽硣
						childHeight = UMath::maximum(0, parentHeight - verticalPadding
							- measuredTopMargin - measuredBottomMargin);

						if (lp->height == 0
							|| lp->height == ULayoutParams::MATCH_PARENT)
						{
							if (parentHeightMode == FIT)
							{
								childHeightMode = FIT;
								lp->vertCoupleHandlerType = RestrainLayoutParams::CH_WRAP;
							}
							else if (parentHeightMode == EXACTLY)
							{
								childHeightMode = EXACTLY;
								lp->vertCoupleHandlerType = RestrainLayoutParams::CH_FILL;
							}
						}
						else if (lp->height == ULayoutParams::FIT_CONTENT)
						{
							childHeightMode = FIT;
							lp->vertCoupleHandlerType = RestrainLayoutParams::CH_WRAP;
						}
					}
					else if (parentHeightMode == UNKNOWN)
					{
						childHeight = size;
						childHeightMode = UNKNOWN;
						lp->vertCoupleHandlerType = RestrainLayoutParams::CH_FILL;
					}
				}
			}
		}
	}
	else
	{
		this->getChildMeasure(
			parentHeight, parentHeightMode,
			verticalMargins + verticalPadding,
			lp->height, &childHeight, &childHeightMode);
	}

	*height = childHeight;
	*heightSpec = childHeightMode;
}

int RestrainLayout::getLeftSpacing(UWidget *child, RestrainLayoutParams *lp)
{
	int leftSpacing = 0;
	RestrainLayoutParams *childLp = lp;
	while (childLp->hasStart()
		&& childLp->startHandledId != this->getId())
	{
		UWidget *target = getChildById(lp->startHandledId);
		RestrainLayoutParams *targetLp
			= (RestrainLayoutParams*)target->getLayoutParams();

		if (childLp->startHandledEdge
			== RestrainLayoutParams::EDGE_END)
		{
			leftSpacing += target->getMeasuredWidth()
				+ (targetLp->hasStart() ? targetLp->leftMargin : 0);
		}
		else if (childLp->startHandledEdge
			== RestrainLayoutParams::EDGE_START)
		{
			leftSpacing +=
				(targetLp->hasStart() ? targetLp->leftMargin : 0);
		}

		childLp = targetLp;
	}

	return leftSpacing;
}

int RestrainLayout::getTopSpacing(UWidget *child, RestrainLayoutParams *lp)
{
	int topSpacing = 0;
	RestrainLayoutParams *childLp = lp;
	while (childLp->hasTop()
		&& childLp->topHandledId != this->getId())
	{
		UWidget *target = getChildById(lp->topHandledId);
		RestrainLayoutParams *targetLp
			= (RestrainLayoutParams*)target->getLayoutParams();

		if (childLp->topHandledEdge
			== RestrainLayoutParams::EDGE_BOTTOM)
		{
			topSpacing += target->getMeasuredHeight()
				+ (targetLp->hasTop() ? targetLp->topMargin : 0);
		}
		else if (childLp->topHandledEdge
			== RestrainLayoutParams::EDGE_TOP)
		{
			topSpacing +=
				(targetLp->hasTop() ? targetLp->topMargin : 0);
		}

		childLp = targetLp;
	}

	return topSpacing;
}

int RestrainLayout::getRightSpacing(UWidget *child, RestrainLayoutParams *lp)
{
	int rightSpacing = 0;
	RestrainLayoutParams *childLp = lp;
	while (childLp->hasEnd()
		&& childLp->endHandledId != this->getId())
	{
		UWidget *target = getChildById(childLp->endHandledId);
		RestrainLayoutParams *targetLp = (RestrainLayoutParams*)target->getLayoutParams();

		if (childLp->endHandledEdge
			== RestrainLayoutParams::EDGE_START)
		{
			rightSpacing += target->getMeasuredWidth()
				+ (targetLp->hasEnd() ? targetLp->rightMargin : 0);
		}
		else if (childLp->endHandledEdge
			== RestrainLayoutParams::EDGE_END)
		{
			rightSpacing +=
				(targetLp->hasEnd() ? targetLp->rightMargin : 0);
		}

		childLp = targetLp;
	}

	return rightSpacing;
}

int RestrainLayout::getBottomSpacing(UWidget *child, RestrainLayoutParams *lp)
{
	int bottomSpacing = 0;
	RestrainLayoutParams *childLp = lp;
	while (childLp->hasBottom()
		&& childLp->bottomHandledId != this->getId())
	{
		UWidget *target = getChildById(childLp->bottomHandledId);
		RestrainLayoutParams *targetLp = (RestrainLayoutParams*)target->getLayoutParams();

		if (childLp->bottomHandledEdge
			== RestrainLayoutParams::EDGE_TOP)
		{
			bottomSpacing += target->getMeasuredHeight()
				+ (targetLp->hasBottom() ? targetLp->bottomMargin : 0);
		}
		else if (childLp->bottomHandledEdge
			== RestrainLayoutParams::EDGE_BOTTOM)
		{
			bottomSpacing +=
				(targetLp->hasBottom() ? targetLp->bottomMargin : 0);
		}

		childLp = targetLp;
	}

	return bottomSpacing;
}


int RestrainLayout::measureWrappedWidth()
{
	int wrappedWidth = 0;

	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		UWidget *child = getChildAt(i);
		RestrainLayoutParams *lp = (RestrainLayoutParams*)child->getLayoutParams();

		int startSpacing = getLeftSpacing(child, lp);
		int endSpacing = getRightSpacing(child, lp);

		int chainWidth =
			startSpacing + endSpacing
			+ child->getMeasuredWidth()
			+ (lp->hasStart() ? lp->leftMargin : 0) 
			+ (lp->hasEnd() ? lp->rightMargin : 0);

		wrappedWidth = UMath::maximum(wrappedWidth, chainWidth);
	}

	return wrappedWidth;
}

int RestrainLayout::measureWrappedHeight()
{
	int wrappedHeight = 0;

	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		UWidget *child = getChildAt(i);
		RestrainLayoutParams *lp = (RestrainLayoutParams*)child->getLayoutParams();

		int topSpacing = getTopSpacing(child, lp);
		int bottomSpacing = getBottomSpacing(child, lp);

		int chainHeight =
			topSpacing + bottomSpacing
			+ child->getMeasuredHeight()
			+ (lp->hasTop() ? lp->topMargin : 0)
			+ (lp->hasBottom() ? lp->bottomMargin : 0);
		wrappedHeight = UMath::maximum(wrappedHeight, chainHeight);
	}

	return wrappedHeight;
}


void RestrainLayout::layoutChild(
	UWidget *child, RestrainLayoutParams *lp,
	int left, int top, int right, int bottom)
{
	if (!lp->isVertLayouted)
		layoutChildVertical(child, lp, top, bottom);
	if (!lp->isHoriLayouted)
		layoutChildHorizontal(child, lp, left, right);

	child->layout(lp->left, lp->top, lp->right, lp->bottom);
}


void RestrainLayout::layoutChildVertical(
	UWidget *child, RestrainLayoutParams *lp,
	int top, int bottom)
{
	int childTop = top + mPaddingTop;
	if (lp->hasTop()
		&& lp->topHandledId != this->getId())
	{
		UWidget *target = getChildById(lp->topHandledId);
		RestrainLayoutParams *targetLp
			= (RestrainLayoutParams*)target->getLayoutParams();

		if (!targetLp->isVertLayouted)
			layoutChildVertical(target, targetLp, top, bottom);

		if (lp->topHandledEdge
			== RestrainLayoutParams::EDGE_BOTTOM)
		{
			childTop = targetLp->bottom;
		}
		else if (lp->topHandledEdge
			== RestrainLayoutParams::EDGE_TOP)
		{
			childTop = targetLp->top;
		}
	}

	int childBottom = bottom - mPaddingBottom;
	if (lp->hasBottom()
		&& lp->bottomHandledId != this->getId())
	{
		UWidget *target = getChildById(lp->bottomHandledId);
		RestrainLayoutParams *targetLp = (RestrainLayoutParams*)target->getLayoutParams();

		if (!targetLp->isVertLayouted)
			layoutChildVertical(target, targetLp, top, bottom);

		if (lp->bottomHandledEdge
			== RestrainLayoutParams::EDGE_TOP)
		{
			childBottom = targetLp->top;
		}
		else if (lp->bottomHandledEdge
			== RestrainLayoutParams::EDGE_BOTTOM)
		{
			childBottom = targetLp->bottom;
		}
	}

	if (lp->hasVerticalCouple())
	{
		childTop += lp->topMargin;
		childBottom -= lp->bottomMargin;

		switch (lp->vertCoupleHandlerType)
		{
		case RestrainLayoutParams::CH_FILL:
			break;
		case RestrainLayoutParams::CH_WRAP:
		case RestrainLayoutParams::CH_FIXED:
			childTop += static_cast<int>(
				(childBottom - childTop - child->getMeasuredHeight())*lp->verticalPercent);
			childBottom = childTop + child->getMeasuredHeight();
			break;
		}
	}
	else
	{
		if (lp->hasTop())
		{
			childTop += lp->topMargin;
			childBottom = childTop + child->getMeasuredHeight();
		}
		else if (lp->hasBottom())
		{
			childBottom -= lp->bottomMargin;
			childTop = childBottom - child->getMeasuredHeight();
		}
		else
			childBottom = childTop + child->getMeasuredHeight();
	}

	lp->isVertLayouted = true;
	lp->top = childTop;
	lp->bottom = childBottom;
}

void RestrainLayout::layoutChildHorizontal(
	UWidget *child, RestrainLayoutParams *lp,
	int left, int right)
{
	int childLeft = left + mPaddingLeft;
	if (lp->hasStart()
		&& lp->startHandledId != this->getId())
	{
		UWidget *target = getChildById(lp->startHandledId);
		RestrainLayoutParams *targetLp
			= (RestrainLayoutParams*)target->getLayoutParams();

		if (!targetLp->isHoriLayouted)
			layoutChildHorizontal(target, targetLp, left, right);

		if (lp->startHandledEdge
			== RestrainLayoutParams::EDGE_END)
		{
			childLeft = targetLp->right;
		}
		else if (lp->startHandledEdge
			== RestrainLayoutParams::EDGE_START)
		{
			childLeft = targetLp->left;
		}
	}

	int childRight = right - mPaddingRight;
	if (lp->hasEnd()
		&& lp->endHandledId != this->getId())
	{
		UWidget *target = getChildById(lp->endHandledId);
		RestrainLayoutParams *targetLp = (RestrainLayoutParams*)target->getLayoutParams();

		if (!targetLp->isHoriLayouted)
			layoutChildHorizontal(target, targetLp, left, right);

		if (lp->endHandledEdge
			== RestrainLayoutParams::EDGE_START)
		{
			childRight = targetLp->left;
		}
		else if (lp->endHandledEdge
			== RestrainLayoutParams::EDGE_END)
		{
			childRight = targetLp->right;
		}
	}

	if (lp->hasHorizontalCouple())
	{
		childLeft += lp->leftMargin;
		childRight -= lp->rightMargin;

		switch (lp->horiCoupleHandlerType)
		{
		case RestrainLayoutParams::CH_FILL:
			break;
		case RestrainLayoutParams::CH_WRAP:
		case RestrainLayoutParams::CH_FIXED:
			childLeft += static_cast<int>(
				(childRight - childLeft - child->getMeasuredWidth())*lp->horizontalPercent);
			childRight = childLeft + child->getMeasuredWidth();
			break;
		}
	}
	else
	{
		if (lp->hasStart())
		{
			childLeft += lp->leftMargin;
			childRight = childLeft + child->getMeasuredWidth();
		}
		else if (lp->hasEnd())
		{
			childRight -= lp->rightMargin;
			childLeft = childRight - child->getMeasuredWidth();
		}
		else
			childRight = childLeft + child->getMeasuredWidth();
	}

	lp->isHoriLayouted = true;
	lp->left = childLeft;
	lp->right = childRight;
}


void RestrainLayout::onMeasure(
	int width, int height,
	int widthMode, int heightMode)
{
	int finalWidth = 0;
	int finalHeight = 0;

	int horizontalPadding = mPaddingLeft + mPaddingRight;
	int verticalPadding = mPaddingTop + mPaddingBottom;

	clearMeasureFlag();
	this->measureRestrainedChildren(width, height, widthMode, heightMode);

	switch (widthMode)
	{
	case FIT:
		finalWidth = measureWrappedWidth() + horizontalPadding;
		finalWidth = UMath::minimum(width, finalWidth);
		finalWidth = UMath::maximum(mMinimumWidth, finalWidth);
		break;

	case EXACTLY:
		finalWidth = width;
		break;

	case UNKNOWN:
		finalWidth = measureWrappedWidth() + horizontalPadding;
		finalWidth = UMath::maximum(mMinimumWidth, finalWidth);
		break;
	}

	switch (heightMode)
	{
	case FIT:
		finalHeight = measureWrappedHeight() + verticalPadding;
		finalHeight = UMath::minimum(width, finalHeight);
		finalHeight = UMath::maximum(mMinimumHeight, finalHeight);
		break;

	case EXACTLY:
		finalHeight = height;
		break;

	case UNKNOWN:
		finalHeight = measureWrappedHeight() + verticalPadding;
		finalHeight = UMath::maximum(mMinimumHeight, finalHeight);
		break;
	}

	this->setMeasuredDimension(finalWidth, finalHeight);
}

void RestrainLayout::onLayout(
	bool changed, bool sizeChanged,
	int left, int top, int right, int bottom)
{
	int horizontalPadding = mPaddingLeft + mPaddingRight;
	int verticalPadding = mPaddingTop + mPaddingBottom;

	for (std::size_t i = 0; i < getChildCount(); ++i)
	{
		UWidget *child = getChildAt(i);
		RestrainLayoutParams *lp = (RestrainLayoutParams*)child->getLayoutParams();

		layoutChild(child, lp, 0, 0,
			right - left, bottom - top);
	}
}