#pragma once
#include "UWindow.h"
#include "OnClickListener.h"
#include "OnSeekValueChangedListener.h"

class TextView;
class SeekBar;
class Direct3DView;
class RestraintLayout;


class LodWindow 
	:public UWindow, 
	public OnSeekValueChangedListener,
	public OnClickListener
{
private:
	TextView *mC1ValueTV;
	TextView *mC2ValueTV;
	TextView *mSplitValueTV;
	Direct3DView *mDirect3DView;

	SeekBar *mC1SeekBar;
	SeekBar *mC2SeekBar;
	SeekBar *mSplitSeekBar;

	void inflateCtlLayout(RestraintLayout *rightLayout);

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
	LodWindow(UApplication *app);
	LodWindow(UApplication *app, int id);
	~LodWindow();

	virtual void onCreate() override;

	void onClick(View *widget);

	void onSeekValueChanged(SeekBar *seekBar, float value);
	void onSeekIntegerValueChanged(SeekBar *seekBar, int value);
};

