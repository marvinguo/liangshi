#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

class LIANGSHIEXPORT CheckableImageButton : public QToolButton
{
	Q_OBJECT
public:
	CheckableImageButton(QString pic_name, QWidget *parent = 0);
	~CheckableImageButton();
	void setMousePress(bool mouse_press);

protected:
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void paintEvent(QPaintEvent *event);

private:
	void PaintInternal(int top_color, int middle_color, int bottom_color);

public:
	bool _isMouseovering;
	bool _isMousepressing;
};

