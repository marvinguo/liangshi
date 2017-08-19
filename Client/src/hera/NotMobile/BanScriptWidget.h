#pragma once

#include "BanBaseWidget.h"
#include "BanScriptEditor.h"
#include "BanScriptDeps.h"

class LIANGSHIEXPORT BanScriptRuntime;

class LIANGSHIEXPORT BanScriptWidget : public BanBaseWidget
{
	Q_OBJECT
public:
	BanScriptWidget(bool showDebugerEditor = true, QWidget *parent = 0);
	virtual ~BanScriptWidget();

	void RunScript(QString code);
	void InitDeps(QString depsFolder);

public:
	virtual ScType GetType(){return ScType_Script;}
	virtual void UpdateDataFromMsgPack();
	virtual bool SaveDocument(QString filepath = "");

	//for script related
	friend class BanScriptRuntime;
	friend class BanFile;
private:
	void reset();
	void NeedSaveScript(bool need);
	QPointF setOrigin(int x, int y);
	double rotate(double angle);
	QPointF scale(double x, double y);
	void drawLine(int x1, int y1, int x2, int y2);
	void drawRect(int x1, int y1, int width, int height);
	void drawEllipse(int x1, int y1, int width, int height);
	void drawPath(vector<QPoint>& points);
	void drawText(int x, int y, QString& text, int size);
	void clearAll();

public Q_SLOTS:
	void OnScriptEditorTriggered();
	void OnScriptDebuggerTriggered();
	void OnRunScriptTriggered();
	void OnShowDeps();

private:
	BanScriptRuntime* _scriptRuntime;
	BanScriptEditor *_codeEdit;
	BanScriptDeps* _deps;
	bool _showDebugerEditor;

	QPoint _lastOrigin;
	double _lastRotate;
	QPointF _lastScale;
};

