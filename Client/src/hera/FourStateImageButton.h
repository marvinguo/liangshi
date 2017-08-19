#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

enum FourStateButtonStatus
{
	FourStateButtonStatus_NORMAL,
	FourStateButtonStatus_ENTER, 
	FourStateButtonStatus_PRESS, 
	FourStateButtonStatus_NOSTATUS
};

class LIANGSHIEXPORT FourStateImageButton : public QPushButton
{
	Q_OBJECT
public:
	FourStateImageButton(QWidget *parent = 0);
	~FourStateImageButton();
	void LoadPixmap(QString pic_name);

protected:
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void paintEvent(QPaintEvent *);

private:
	FourStateButtonStatus _status;
	QPixmap _pixmap; 
	int _width;
	int _height;
	bool _pressed;
};

