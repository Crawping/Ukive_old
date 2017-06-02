#pragma once

struct URect
{
	int left;
	int top;
	int right;
	int bottom;

	URect()
	{
		this->left = 0;
		this->top = 0;
		this->right = 0;
		this->bottom = 0;
	}

	URect(int left, int top, int right, int bottom)
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}

	int width()
	{
		return right - left;
	}

	int height()
	{
		return bottom - top;
	}
};

struct URectU
{
	unsigned int top;
	unsigned int left;
	unsigned int right;
	unsigned int bottom;

	unsigned int width()
	{
		return right - left;
	}

	unsigned int height()
	{
		return bottom - top;
	}
};

struct URectF
{
	float top;
	float left;
	float right;
	float bottom;

	URectF()
	{
		this->left = 0.0f;
		this->top = 0.0f;
		this->right = 0.0f;
		this->bottom = 0.0f;
	}

	URectF(float left, float top, float right, float bottom)
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}

	URectF(URect rectInt)
	{
		this->left = static_cast<float>(rectInt.left);
		this->top = static_cast<float>(rectInt.top);
		this->right = static_cast<float>(rectInt.right);
		this->bottom = static_cast<float>(rectInt.bottom);
	}

	float width()
	{
		return right - left;
	}

	float height()
	{
		return bottom - top;
	}
};

struct UPoint
{
	int x;
	int y;
};

struct UPointU
{
	unsigned int x;
	unsigned int y;
};

struct UPointF
{
	float x;
	float y;
};