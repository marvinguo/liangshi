#pragma once

#include "../common/library.h"
#include "../common/config.h"
#include "../common/Qt.h"
#include "../../../SharedCode/LokiBuffer.h"
#include "HeraDocument.h"

enum ScType
{
	ScType_Basic = 1024,
	ScType_Script = 1025,
	ScType_Test = 1026,
};

class LIANGSHIEXPORT BanDocument : public HeraDocument
{
protected:
	BanDocument();
	virtual ~BanDocument();

public:
	virtual ScType GetType() = 0;
	virtual QPixmap ToPixmap(double xZoom = 1, double yZoom = 1, bool withBackground = false, bool refresh = true) = 0;
	virtual QPixmap ToFixedPixmap(int x, int y, bool withBackground = false) = 0;
	virtual LokiBuffer GetMsgPackData(bool init = false) = 0;
	virtual void SetMsgPackData(LokiBuffer& buffer) = 0;
	virtual void UpdateDataFromMsgPack() = 0;

protected:
	QString _filepath;
};
