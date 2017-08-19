#pragma once
#include <string>
#include <QtGui>

#if defined(_WIN32)
# define MUPDF_QT_EXPORT __declspec(dllexport)
#else
# define MUPDF_QT_EXPORT
#endif

class PoseidonDocument;
class PoseidonDocumentPrivate;
class PoseidonPage;
class PoseidonPagePrivate;

MUPDF_QT_EXPORT PoseidonDocument * loadPoseidonDocument(const QString &filePath);

class MUPDF_QT_EXPORT PoseidonDocument
{
public:
	~PoseidonDocument();
	bool needsPassword() const;
	bool authPassword(const QString &password);
	int numPages() const;
	PoseidonPage *page(int index, float scaleX = 1.0f, float scaleY = 1.0f, float rotation = 0.0f) const;
	QString pdfVersion() const;
	QString title() const;
	QString author() const;
	QString subject() const;
	QString keywords() const;
	QString creator() const;
	QString producer() const;
	QDateTime creationDate() const;
	QDateTime modDate() const;
	void setTransparentRendering(bool enable);
	void setBackgroundColor(int r, int g, int b, int a = 255);

private:
	PoseidonDocument(const QString &filePath);
	// disable copy
	PoseidonDocument(const PoseidonDocument &);
	PoseidonDocument &operator=(const PoseidonDocument &);

	PoseidonDocumentPrivate *d;

	friend class PoseidonPage;
	friend PoseidonDocument *loadPoseidonDocument(const QString &filePath);
};

class MUPDF_QT_EXPORT PoseidonPage
{
public:
	~PoseidonPage();
	PoseidonPage& operator=(const PoseidonPage&);

	bool isValid() const;
	QImage renderImage() const;
	QRect size() const;
	void setTransparentRendering(bool enable);
	void setBackgroundColor(int r, int g, int b, int a = 255);
	void setTransform(float scaleX, float scaleY, float rotation = 0.0f);
	QString text(float x0, float y0, float x1, float y1) const;

private:
	PoseidonPage(const PoseidonDocument &document, int index, float scaleX = 1.0f, float scaleY = 1.0f, float rotation = 0.0f);
	PoseidonPagePrivate *d;

	friend class PoseidonDocument;
};
