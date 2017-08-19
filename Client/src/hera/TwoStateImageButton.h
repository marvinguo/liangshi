#pragma once

#include "../common/Qt.h"
#include "../common/library.h"

class LIANGSHIEXPORT TwoStateImageButton : public QToolButton
{
	Q_OBJECT
public:
	TwoStateImageButton(QString normalImage, QString checkedImage, QWidget *parent = 0);
	~TwoStateImageButton();

public:
	void SetChecked(bool checked);

protected:
	virtual void enterEvent(QEvent *);
	virtual void leaveEvent(QEvent *);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void paintEvent(QPaintEvent *);

private Q_SLOTS:
	void OnClickTriggered();

private:
	void PaintInternal(int top_color, int middle_color, int bottom_color);
	void ChangeButtonImage();

private:
	QPixmap _normal;
	QPixmap _checked;
	bool _isChecked;
	bool _isMouseovering;
};

