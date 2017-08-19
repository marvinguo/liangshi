#include <QtCore>
#include <map>
using namespace std;

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif


QString GetFileMd5(QString filepath)
{
	QFile outputFile(filepath);
	outputFile.open(QIODevice::ReadOnly);
	QByteArray array = outputFile.readAll();
	outputFile.close();
	QByteArray hashData = QCryptographicHash::hash(array, QCryptographicHash::Md5);
	return hashData.toHex();
}

void GetFolderFileJson(QString dir, QString prefix, QJsonArray& all)
{
	QDir dirInput(dir);

	if (dirInput.exists(dir))
	{
		Q_FOREACH(QFileInfo info, dirInput.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
		{
			if (info.isDir())
			{
				GetFolderFileJson(info.absoluteFilePath(), prefix + "/" + info.fileName(), all);
			}
			else
			{
				QJsonObject json;
				json["name"] = prefix + "/" + info.fileName();
				json["md5"] = GetFileMd5(info.absoluteFilePath());
				all.append(json);
			}
		}
	}
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QStringList arguments = QCoreApplication::arguments();
	if(arguments.size() < 3)
	{
		return -1;
	}


	QJsonArray all;
	GetFolderFileJson(arguments[1], "", all);
	QJsonDocument doc(all);
	QByteArray byteArray = doc.toJson(QJsonDocument::Compact);

	QFile outputFile(arguments[2]);
	outputFile.open(QIODevice::WriteOnly);
	outputFile.write(byteArray.data(), byteArray.size());
	outputFile.flush();
	outputFile.close();
	//return a.exec();
}

/*
QJsonParseError jerror;
QJsonDocument jdoc= QJsonDocument::fromJson(file.readAll(),&jerror);
if(jerror.error() != QJsonParserError::NoError)
return false;
QJsonObject obj = jdoc.object();
*/