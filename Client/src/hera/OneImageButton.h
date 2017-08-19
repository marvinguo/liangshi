#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

class LIANGSHIEXPORT OneImageButton : public QToolButton
{
	Q_OBJECT
public:
	OneImageButton(QString path, QString tooltip, QWidget *parent = 0);
	~OneImageButton();

protected:
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void paintEvent(QPaintEvent *event);

private:
	void PaintInternal(int top_color, int middle_color, int bottom_color);

private:
	bool _isMouseovering;
};

