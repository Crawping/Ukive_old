#pragma once
#include "UWindow.h"
#include "OnClickListener.h"
#include "OnSeekValueChangedListener.h"

class UTextView;
class USeekBar;
class UDirect3DView;
class RestrainLayout;


class LodWindow 
	:public UWindow, 
	public OnSeekValueChangedListener,
	public OnClickListener
{
private:
	UTextView *mC1ValueTV;
	UTextView *mC2ValueTV;
	UTextView *mSplitValueTV;
	UDirect3DView *mDirect3DView;

	USeekBar *mC1SeekBar;
	USeekBar *mC2SeekBar;
	USeekBar *mSplitSeekBar;

	void inflateCtlLayout(RestrainLayout *rightLayout);

public:
	static const int ID_LOD_INFO = 0x010;
	static const int ID_RIGHT_RESTRAIN = 0x011;

	static const int ID_C1_LABEL = 0x012;
	static const int ID_C1_SEEKBAR = 0x014;
	static const int ID_C1_VALUE = 0x015;

	static const int ID_C2_LABEL = 0x016;
	static const int ID_C2_SEEKBAR = 0x018;
	static const int ID_C2_VALUE = 0x019;

	static const int ID_SPLIT_LABEL = 0x01A;
	static const int ID_SPLIT_SEEKBAR = 0x01B;
	static const int ID_SPLIT_VALUE = 0x01C;

	static const int ID_SUBMIT_BUTTON = 0x01D;
	static const int ID_VSYNC_BUTTON = 0x01E;

	static const int ID_MONITOR = 0x01F;
	static const int ID_HELPER = 0x020;

public:
	using UWindow::UWindow;

	virtual void onCreate() override;

	void onClick(UWidget *widget);

	void onSeekValueChanged(USeekBar *seekBar, float value);
	void onSeekIntegerValueChanged(USeekBar *seekBar, int value);
};

