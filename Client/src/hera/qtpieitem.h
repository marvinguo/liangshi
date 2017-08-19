#pragma once

#include "../../../src/common/Qt.h"

class QtPieItem: public QWidget
{
	friend class QtPieMenu;
	Q_OBJECT

protected:
	enum ItemType {
		Invalid,
		SubMenu,
		Action
	};

public:
	QtPieItem(const QString &text = QString::null, unsigned int weight = 1, QWidget *parent = 0);
	QtPieItem(const QIcon &icon, const QString &title = QString(), unsigned int weight = 1, QWidget *parent = 0);
	virtual ~QtPieItem();

	void SetText(const QString &text);
	QString GetText() const;

	void SetIcon(const QIcon &icon);
	QIcon GetIcon() const;

	void SetWeight(int weight);
	int GetWeight() const;

	void SetEnabled(bool enabled = true);
	bool IsEnabled() const;

	ItemType GetType(){return _type;}

protected:
	void SetType(ItemType type){_type = type;}

private:
	QString _text;
	QIcon _icon;
	bool _isEnable;
	unsigned int _weight;
	ItemType _type;
};