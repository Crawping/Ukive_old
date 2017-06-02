#pragma once

class USpan
{
private:
	unsigned int mStart;
	unsigned int mEnd;

public:
	USpan(unsigned int start, unsigned int end);
	virtual ~USpan();

	static const int USPAN_BASE_TYPE_TEXT = 0x1;
	static const int USPAN_BASE_TYPE_INTERACTABLE = 0x2;
	static const int USPAN_BASE_TYPE_EFFECT = 0x4;
	static const int USPAN_BASE_TYPE_INLINEOBJECT = 0x8;

	static const int TEXT_UNDERLINE = 0x100;
	static const int EFFECT_NORMAL = 0x101;

	virtual int getType() = 0;
	virtual int getBaseType() = 0;

	void resize(unsigned int start, unsigned int end);

	unsigned int getStart();
	unsigned int getEnd();
};