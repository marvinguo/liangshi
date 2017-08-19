#include "BanFile.h"
#include "BanBaseWidget.h"
#ifdef _LiangshiDesktop
	#include "NotMobile/BanScriptWidget.h"
#endif
#include "BanElement.h"
#include "BanElementWithLabel.h"
#include "TestBanElement.h"
#include "TestBanElementWithLabel.h"
#include "TestBaseWidget.h"
#include "../common/Util.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

BanFile::BanFile(BanDocument* doc):
_doc(doc)
{
}


BanFile::~BanFile()
{
}

bool BanFile::LoadScImageOnly(QString filepath, QPixmap& pixmap)
{
	bool result = true;
	QString tempFolder = FileUtil::GetTempFolder() + QUuid::createUuid().toString();
	FileUtil::UnZipFolder(tempFolder, filepath);
	if(QFile::exists(tempFolder + "/image"))
		pixmap.load(tempFolder + "/image");
	else
		result = false;
	FileUtil::RemoveDir(tempFolder);
	return result;
}

bool BanFile::LoadScThumbnailOnly(QString filepath, QPixmap& pixmap)
{
	bool result = true;
	QString tempFolder = FileUtil::GetTempFolder() + QUuid::createUuid().toString();
	FileUtil::UnZipFolder(tempFolder, filepath);
	if(QFile::exists(tempFolder + "/thumbnail"))
		pixmap.load(tempFolder + "/thumbnail");
	else
		result = false;
	FileUtil::RemoveDir(tempFolder);
	return result;
}

BanDocument* BanFile::LoadAsScDocument(QString filepath, bool direct)
{
	QString tempFolder = FileUtil::GetTempFolder() + QUuid::createUuid().toString();
	FileUtil::UnZipFolder(tempFolder, filepath);

	BanDocument* document = LoadScDocument(tempFolder, direct);

	FileUtil::RemoveDir(tempFolder);
	return document;
}

BanDocument* BanFile::LoadScDocument(QString tempFolder, bool direct)
{
	BanDocument* document = NULL;

	QString filepath = tempFolder + "/this";
	QFile readFile(filepath);
	readFile.open(QIODevice::ReadOnly);
	QByteArray array = readFile.readAll();
	LokiBuffer buffer(array.data(), array.size());
	readFile.close();

	msgpack::unpacker pac;
	pac.reserve_buffer(buffer.Size());
	memcpy(pac.buffer(), buffer.Data(), buffer.Size());
	pac.buffer_consumed(buffer.Size());

	bool getHeader = false;
	bool getVersion = false;
	bool getRandom = false;
	int type = 0;

	msgpack::unpacked result;
	while(pac.next(&result))
	{
		msgpack::object object = result.get();

		if(object.type == msgpack::type::POSITIVE_INTEGER)
		{
			if(!getVersion)
			{
				int version = 0;
				object.convert(&version);
				getVersion = true;

				if(version > KE_FILE_MIN_VERSION)
				{
					return false;
				}
			}
			else
			{
				object.convert(&type);
			}
		}
		if(object.type == msgpack::type::RAW)
		{
			if(!getHeader)
			{
				string header;
				object.convert(&header);
				getHeader = true;
			}
			else if(!getRandom && type == 0)
			{
				getRandom = true;
			}
			else
			{
				if(type != 0)
				{
					LokiBuffer buffer(const_cast<char*>(object.via.raw.ptr), object.via.raw.size);
					if(type == ScType_Basic)
					{
						document = new BanBaseWidget();
					}
#ifdef _LiangshiDesktop
					else if (type == ScType_Script)
					{
						BanScriptWidget* scriptWidget = new BanScriptWidget();
						//这里加载script deps,直接拷过来就行
						if(direct)
						{
							scriptWidget->InitDeps(tempFolder + "/scriptDeps");
						}
						document = scriptWidget;
					}
#endif
					else if (type == ScType_Test)
					{
						document = new TestBaseWidget();
					}

					if(document != NULL)
					{
						document->SetMsgPackData(buffer);
					}
				}
			}
		}
	}
	if(document != NULL)
		document->UpdateDataFromMsgPack();
	return document;
}


vector<BanElement*> BanFile::LoadAsScElements(QString filepath, bool needTitle)
{
	vector<BanElement*> elements;

	QString tempFolder = FileUtil::GetTempFolder() + QUuid::createUuid().toString();
	FileUtil::UnZipFolder(tempFolder, filepath);

	LoadScElements(tempFolder + "/this", elements, needTitle);

	FileUtil::RemoveDir(tempFolder);
	return elements;
}

bool BanFile::LoadScElements(QString filepath, vector<BanElement*>& elements, bool needTitle)
{
	QFile readFile(filepath);
	readFile.open(QIODevice::ReadOnly);
	QByteArray array = readFile.readAll();
	LokiBuffer buffer(array.data(), array.size());
	readFile.close();

	msgpack::unpacker pac;
	pac.reserve_buffer(buffer.Size());
	memcpy(pac.buffer(), buffer.Data(), buffer.Size());
	pac.buffer_consumed(buffer.Size());

	bool getHeader = false;
	bool getVersion = false;
	bool getRandom = false;
	int type = 0;

	msgpack::unpacked result;
	while(pac.next(&result))
	{
		msgpack::object object = result.get();

		if(object.type == msgpack::type::POSITIVE_INTEGER)
		{
			if(!getVersion)
			{
				int version = 0;
				object.convert(&version);
				getVersion = true;

				if(version > KE_FILE_MIN_VERSION)
				{
					return false;
				}
			}
			else
			{
				object.convert(&type);
			}
		}
		if(object.type == msgpack::type::RAW)
		{
			if(!getHeader)
			{
				string header;
				object.convert(&header);
				getHeader = true;
			}
			else if(!getRandom && type == 0)
			{
				getRandom = true;
			}
			else
			{
				if(type != 0)
				{
					LokiBuffer buffer(const_cast<char*>(object.via.raw.ptr), object.via.raw.size);
					if(type == ScType_Basic || type == ScType_Script)
					{
						BanBaseWidget* document = new BanBaseWidget(true);
						document->SetMsgPackData(buffer);
						//document->UpdateDataFromMsgPack();
						if(!needTitle)
							elements.push_back(new BanElement(document));
						else
							elements.push_back(new BanElementWithLabel(document));
					}
					else if (type = ScType_Test)
					{
						TestBaseWidget* document = new TestBaseWidget(true);
						document->SetMsgPackData(buffer);
						//document->UpdateDataFromMsgPack();
						if(!needTitle)
							elements.push_back(new TestBanElement(document));
						else
							elements.push_back(new TestBanElementWithLabel(document));
					}
				}
			}
		}
	}
	return true;
}

void BanFile::SaveFile(QString filepath)
{
	QString tempFolder = FileUtil::GetTempFolder() + QUuid::createUuid().toString();
	QDir().mkdir(tempFolder);

	SaveImageFile(tempFolder + "/image");
	//SaveThumbnailFile(tempFolder + "/thumbnail");
	SaveThisFile(tempFolder + "/this");
#ifdef _LiangshiDesktop
	if(_doc->GetType() == ScType_Script)
		SaveScriptDepsFiles(tempFolder);
#endif

	FileUtil::ZipFolder(tempFolder, filepath);
	FileUtil::RemoveDir(tempFolder);
}

void BanFile::SaveFileWithAllElements(QString filepath)
{
	QString tempFolder = FileUtil::GetTempFolder() + QUuid::createUuid().toString();
	QDir().mkdir(tempFolder);

	SaveThisFile(tempFolder + "/this", true);

	FileUtil::ZipFolder(tempFolder, filepath);
	FileUtil::RemoveDir(tempFolder);
}


void BanFile::SaveImageFile(QString filepath)
{
	QPixmap image = _doc->ToPixmap();
	image.save(filepath, "PNG");
}

void BanFile::SaveThumbnailFile(QString filepath)
{
	QPixmap image = _doc->ToFixedPixmap(32, 32, true);
	image.save(filepath, "PNG");
}

void BanFile::SaveThisFile(QString filepath, bool allElement)
{
	string header = "GY";
	int version = MATERIAL_FILE_CURREN_VERSION;

	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);
	packer.pack(header);
	packer.pack(version);

	LokiBuffer randomLokiBuffer = Util::GenerateRandomLokiBuffer();
	packer.pack_raw(randomLokiBuffer.Size());
	packer.pack_raw_body(randomLokiBuffer.Data(), randomLokiBuffer.Size());

	LokiBuffer thisLokiBuffer = _doc->GetMsgPackData(allElement);

	//这个地方的type，以后怎么用？课题也是一种板书，想办法
	packer.pack((int)_doc->GetType());
	packer.pack_raw(thisLokiBuffer.Size());
	packer.pack_raw_body(thisLokiBuffer.Data(), thisLokiBuffer.Size());

	QFile file(filepath);
	file.open(QIODevice::WriteOnly);
	file.write(sbuffer.data(), sbuffer.size());
	file.flush();
	file.close();
}

void BanFile::SaveScriptDepsFiles(QString folder)
{
#ifdef _LiangshiDesktop
	QString scriptDeps = folder+"/scriptDeps";
	QDir().mkdir(scriptDeps);

	BanScriptWidget* doc = dynamic_cast<BanScriptWidget*>(_doc);
	if(doc != NULL)
	{
		doc->_deps->CopyTo(scriptDeps);
	}
#endif
}

