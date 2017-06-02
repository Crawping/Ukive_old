#pragma once
#include "ULayoutParams.h"

class RestrainLayoutParams : public ULayoutParams
{
private:
	void initLayoutParams();

public:
	static const int EDGE_START = 0;
	static const int EDGE_TOP = 1;
	static const int EDGE_END = 2;
	static const int EDGE_BOTTOM = 3;
	static const int EDGE_NONE = -1;

	static const int CH_FILL = 0;
	static const int CH_WRAP = 1;
	static const int CH_FIXED = 2;

public:
	RestrainLayoutParams(int width, int height);
	RestrainLayoutParams(ULayoutParams *lp);
	~RestrainLayoutParams();

	//������һ�β������������LayoutParams�󶨵�View
	//�Ĳ�����Ϣ��ע��ǵ��ڲ������̿�ʼǰ����ñ�ǡ�
	int specWidth;
	int specWidthMode;
	int specHeight;
	int specHeightMode;
	bool isWidthMeasured;
	bool isHeightMeasured;

	//������һ�β��ֹ��������LayoutParams�󶨵�View
	//�Ĳ�����Ϣ��ע��ǵ��ڲ��ֹ��̿�ʼǰ����ñ�ǡ�
	int left, right;
	int top, bottom;
	bool isVertLayouted;
	bool isHoriLayouted;
	
	float verticalPercent;
	float horizontalPercent;

	int horiCoupleHandlerType;
	int vertCoupleHandlerType;

	int startHandledId;
	int startHandledEdge;
	int topHandledId;
	int topHandledEdge;
	int endHandledId;
	int endHandledEdge;
	int bottomHandledId;
	int bottomHandledEdge;

public:
	bool hasStart();
	bool hasTop();
	bool hasEnd();
	bool hasBottom();

	bool hasVerticalCouple();
	bool hasHorizontalCouple();
};