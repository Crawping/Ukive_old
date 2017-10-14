#pragma once

class View;
class ViewGroup;


class UListAdapter
{
public:
	class UWidgetHolder
	{
	public:
		View *widget;

		UWidgetHolder(View *widget) { this->widget = widget; }
		virtual ~UWidgetHolder() {}
	};

	UListAdapter() {}
	virtual ~UListAdapter() {}

	void notifyDataChanged();

	void notifyItemChanged(int position);
	void notifyItemInserted(int position);
	void notifyItemRemoved(int position);
	void notifyItemRangeChanged(int startPosition, int count);
	void notifyItemRangeInserted(int startPosition, int count);
	void notifyItemRangeRemoved(int startPosition, int count);

	virtual UWidgetHolder *onCreateView(ViewGroup *parent, int position) = 0;

	virtual void onBindData(UWidgetHolder *holder, int position) = 0;

	virtual int getItemId(int position) = 0;

	virtual int getItemCount() = 0;
};