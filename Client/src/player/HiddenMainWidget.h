#pragma once

#include "../common/Qt.h"
#include "../hera/HeraMainWidget.h"

class HiddenMainWidget : public QWidget
{
	Q_OBJECT
public:
	HiddenMainWidget(QWidget *parent = 0);
	~HiddenMainWidget();

Q_SIGNALS:

private Q_SLOTS:
	void OpenFileTriggered(QString filepath);
	void OnHeraCountChangeTriggered(int count);

private:
	void OpenPreview(QString filepath);
	void ResizeHera();

private:
	HeraMainWidget* _hera;
	bool _preview;

	//因为加入了新的机制，这个必须所有的hera关闭才能关闭hidden,这里必须想法
	vector<HeraMainWidget*> _heras;
};

