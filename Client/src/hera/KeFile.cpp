#include "KeFile.h"
#include "BanFile.h"
#include "../common/Util.h"
#include "../common/FileUtil.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

KeFile::KeFile(KeWidget* doc):
_ke(doc)
{
}

KeFile::~KeFile()
{
}

PlayerWidget* KeFile::LoadKeAsPlayerWidget(QString filepath)
{
	//检测GY，如果不是返回NULL
	//
	QFile file(filepath);
	file.open(QIODevice::ReadOnly);
	QDataStream in(&file);

	// Read and check the header
	quint8 magic1;
	quint8 magic2;
	in >> magic1;
	in >> magic2;

	if(magic1 != 'G' && magic2 != 'Y')
	{
		file.close();
		return NULL;
	}
	file.close();

	QString tempFolder = FileUtil::GetTempFolder() + QUuid::createUuid().toString();
	FileUtil::UnZipFolder(tempFolder, filepath);

	PlayerWidget* player = LoadPlayerFile(tempFolder + "/this");
	LoadPlayerPages(player, tempFolder + "/pages");
	LoadPlayerActions(player, tempFolder + "/actions");

	QString audioFile = 	FileUtil::GetTempFolder() + QUuid::createUuid().toString();
	FileUtil::CopyFile(tempFolder + "/audio", audioFile);
	player->InitMedia(audioFile);

	FileUtil::RemoveDir(tempFolder);
	return player;
}

KeWidget* KeFile::LoadKeAsKeWidget(QString filepath)
{
	PlayerWidget* playerWidget = LoadKeAsPlayerWidget(filepath);
	KeTime* keTime = new KeTime(playerWidget);
	KeWidget* keWidget = new KeWidget(keTime);
	if(playerWidget != NULL)
	{
		keWidget->_allPages.clear();
		keWidget->_mainLayout->removeWidget(keWidget->_currentPage);
		delete keWidget->_currentPage;
		keWidget->_currentPage = NULL;

		int time = playerWidget->GetTotalTime();
		int position = playerWidget->_hades->GetPlayFilePosition(time);
		QString hadesFile = FileUtil::GetTempFolder() + QUuid::createUuid().toString();
		FileUtil::GetFilePart(playerWidget->_hadesFile, hadesFile, position);
		keWidget->InitHadesFromExistingFile(hadesFile, position, time);

		for(auto i = playerWidget->_allPages.begin(); i != playerWidget->_allPages.end(); ++i)
		{
			//_allPages
			//利用messagepack来生成BanBaseWidget
			PlayerPage* page = i->second;
			LokiBuffer buffer = page->_viewer->GetMsgPackData(true);
			BanBaseWidget* banPage = new BanBaseWidget(false);
			banPage->_canEditInside = false;
			banPage->SetMsgPackData(buffer);

			keWidget->AddPage(banPage, false);
		}

		keWidget->_actions.clear();
		for(vector<AnimationAction*>::iterator i = playerWidget->_actions.begin(); i != playerWidget->_actions.end(); ++i)
		{
			AnimationAction* action = (*i)->CreateFromThis();
			keWidget->_actions.push_back(action);
		}
	}
	delete playerWidget;
	return keWidget;
}

PlayerWidget* KeFile::LoadPlayerFile(QString filepath)
{
	PlayerWidget* player = NULL;

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
					//现在就一种type,应该不用检查
					player = new PlayerWidget();
					player->SetMsgPackData(buffer);
				}
			}
		}
	}
	player->UpdateDataFromMsgPack();
	return player;
}

void KeFile::LoadPlayerPages(PlayerWidget* player, QString filepath)
{
	QDir rootDir(filepath);
	QFileInfoList filesList = rootDir.entryInfoList();

	foreach(QFileInfo fileInfo, filesList)
	{
		if(fileInfo.isFile())
		{
			LoadPlayerPage(player, fileInfo.absoluteFilePath());
		}
	}
}

void KeFile::LoadPlayerPage(PlayerWidget* player, QString filepath)
{
	BanDocument* document = BanFile::LoadAsScDocument(filepath, false);
	BanBaseWidget* creator = dynamic_cast<BanBaseWidget*>(document);
	if(creator != NULL)
	{
		player->AddPage(creator);
	}
}

void KeFile::LoadPlayerActions(PlayerWidget* player, QString filepath)
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

	int type = 0;

	msgpack::unpacked result;
	while(pac.next(&result))
	{
		msgpack::object object = result.get();

		if(object.type == msgpack::type::POSITIVE_INTEGER)
		{
			object.convert(&type);
		}
		if(object.type == msgpack::type::RAW)
		{
			if(type != 0)
			{
				LokiBuffer rawLokiBuffer(const_cast<char*>(object.via.raw.ptr), object.via.raw.size);
				AnimationAction* action = AnimationAction::CreateFromType((AnimationType)type, rawLokiBuffer);
				if(action != NULL)
				{
					player->AddAction(action);
				}
			}
		}
	}
}

void KeFile::SaveFile(QString filepath)
{
	QString tempFolder = FileUtil::GetTempFolder() + QUuid::createUuid().toString();
	QDir().mkdir(tempFolder);
	QDir().mkdir(tempFolder + "/pages");

	SaveThisFile(tempFolder + "/this");
	SaveActions(tempFolder + "/actions");
	SaveAudio(tempFolder + "/audio");
	SavePages(tempFolder + "/pages");
	SaveTests(tempFolder + "/tests");

	FileUtil::ZipFolder(tempFolder, filepath);
	FileUtil::RemoveDir(tempFolder);
}

void KeFile::SaveThisFile(QString filepath)
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

	LokiBuffer thisLokiBuffer = _ke->GetMsgPackData();
	packer.pack((int)_ke->GetType());
	packer.pack_raw(thisLokiBuffer.Size());
	packer.pack_raw_body(thisLokiBuffer.Data(), thisLokiBuffer.Size());

	QFile file(filepath);
	file.open(QIODevice::WriteOnly);
	file.write(sbuffer.data(), sbuffer.size());
	file.flush();
	file.close();
}

void KeFile::SavePages(QString filepath)
{
	vector<BanBaseWidget*>& allPages = _ke->_allPages;
	for(vector<BanBaseWidget*>::iterator i = allPages.begin(); i != allPages.end(); ++i)
	{
		BanBaseWidget* widget = *i;
		QString uuid = widget->GetObjectUuid();

		//这个不太对，要最原始的
		//还有，对于scelement reset应该怎么做
		BanFile file(widget);
		file.SaveFileWithAllElements(filepath + "/" + uuid);
	}
}

void KeFile::SaveTests(QString filepath)
{
	vector<QString> tests = _ke->_attachedTests;
	for(vector<QString>::iterator i = tests.begin(); i != tests.end(); ++i)
	{
		QString testpath = *i;
		QFileInfo info(testpath);
		if(info.isFile())
		{
			FileUtil::CopyFile(testpath, FileUtil::CopyableFilename(filepath + "/" + info.fileName()));
		}
	}
}

void KeFile::SaveActions(QString filepath)
{
	vector<AnimationAction*>& actions = _ke->_actions;

	msgpack::sbuffer sbuffer;
	msgpack::packer<msgpack::sbuffer> packer(&sbuffer);

	for(vector<AnimationAction*>::iterator i = actions.begin(); i != actions.end(); i++)
	{
		AnimationAction* element = *i;
		LokiBuffer elementData = element->GetMsgPackData();

		packer.pack((int)element->GetType());
		packer.pack_raw(elementData.Size());
		packer.pack_raw_body(elementData.Data(), elementData.Size());
	}

	QFile file(filepath);
	file.open(QIODevice::WriteOnly);
	file.write(sbuffer.data(), sbuffer.size());
	file.flush();
	file.close();
}

void KeFile::SaveAudio(QString filepath)
{
	QString mediaFile = _ke->_hadesFile;
	if(QFile::exists(mediaFile))
	{
		FileUtil::CopyFile(mediaFile, filepath);
	}
}