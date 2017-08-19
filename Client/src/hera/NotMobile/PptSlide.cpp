#include "PptSlide.h"
#include "../common/FileUtil.h"
#include "ChineseMessageBox.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

PptSlide::PptSlide(int width, int height):
_width(width),
_height(height)
{
}

PptSlide::~PptSlide()
{
	for(auto action : _shapeActions)
	{
		delete action.action;
	}
}

void PptSlide::AnalyseShapeAnimation(QAxObject* slide)
{
	QAxObject* shapes = slide->querySubObject("Shapes");
	if(shapes != NULL)
	{
		//先分析动画
		int count;
		QAxObject* timeline = slide->querySubObject("TimeLine");
//		QAxObject* mainSequence = timeline->querySubObject("InteractiveSequences");
		QAxObject* mainSequence = timeline->querySubObject("MainSequence");
		count = mainSequence->property("Count").toInt();
		for(int i = 1; i <= count; i++)
		{
			QAxObject* effect = mainSequence->querySubObject("Item(QVariant)", i);
			int index = effect->property("Index").toInt();
			QString displayname = effect->property("DisplayName").toString();

			QAxObject* shape = effect->querySubObject("Shape");
			int id = shape->property("Id").toInt();
			QAxObject* behaviors = effect->querySubObject("Behaviors");
			int n = behaviors->property("Count").toInt();

			//只循环一次，不只为啥有那么多behavior
			for(int j = 1; j <= n; j++)
			{
				QAxObject* behavior = behaviors->querySubObject("Item(QVariant)", j);


				int left = shape->property("Left").toInt();
				int top = shape->property("Top").toInt();
				QPoint point(left, top);

				int width = shape->property("Width").toInt();
				int height = shape->property("Height").toInt();
				//20150618发现的问题，width,height有0的情况
				//取成1也不太对，难道从图片里取？
				QAxObject* lineFormat = shape->querySubObject("Line");
				width = width == 0 ? lineFormat->property("Weight").toInt() : width;
				height = height == 0 ? lineFormat->property("Weight").toInt() : height;

				int behaviorType = behavior->property("Type").toInt();
				if(behaviorType != msoAnimTypeMotion && behaviorType != msoAnimTypeSet)
				{
					continue;
				}

				if(behaviorType == msoAnimTypeMotion)
				{
					QAxObject* motion = behavior->querySubObject("MotionEffect");
					QString path = motion->property("Path").toString();
					QPoint point1 = ParseVMLPath(path);
					//move的时候，距离是增量
					point += point1;
				}

				SizeAction* action = new SizeAction();
				action->Update(point, width, height);
				_shapeActions.push_back(ShapeAction{ id, action });
				break;
			}
			_shapesWithAnimation.push_back(id);
		}

		//只保存有动画的shape
		count = shapes->property("Count").toInt();
		for(int i = 1; i <= count; i++)
		{
			QString temp = FileUtil::GetTempFolder() + FileUtil::RandomFilename() + ".png";
			QAxObject* shape = shapes->querySubObject("Item(QVariant)", i);
			shape->dynamicCall("Export(const QString&, int, int, int)", temp, 2, 0, 0);
			int id = shape->property("Id").toInt();
			
			if(find(_shapesWithAnimation.begin(), _shapesWithAnimation.end(), id) != _shapesWithAnimation.end())
			{
				_shapes.insert(pair<int, QString>(id, temp));
			}
			else
			{
				QFile::remove(temp);
			}
		}
	}

	//如果用果有动画，则不需要在背景中
	for(auto id : _shapesWithAnimation)
	{
		DeleteSlideShape(slide, id);
	}

	//保留有动画的shape
	//_shapes.earse condition??? remove_if
	vector<int> shapesWithAnimation = _shapesWithAnimation;
	map_erase_if1(_shapes, [shapesWithAnimation](const map<int, QString>::value_type& shape)
	{
		return find(shapesWithAnimation.begin(), shapesWithAnimation.end(), shape.first) == shapesWithAnimation.end();
	});
}

QPoint PptSlide::ParseVMLPath(QString path)
{
	QPoint point;

	QStringList vector = path.split(' ');
	int size = vector.size();

	if(size > 3)
	{
		double percentX = vector[size - 4].toDouble();
		double percentY = vector[size - 3].toDouble();

		int x = _width * percentX;
		int y = _height * percentY;

		point.setX(x);
		point.setY(y);
	}
	return point;
}

void PptSlide::DeleteSlideShape(QAxObject* slide, int id)
{
	QAxObject* shapes = slide->querySubObject("Shapes");
	int count = shapes->property("Count").toInt();
	for(int i = 1; i <= count; ++i)
	{
		QAxObject* shape = shapes->querySubObject("Item(QVariant)", i);
		int Id = shape->property("Id").toInt();
		if(id == Id)
		{
			shape->dynamicCall("Delete()");
			break;
		}
	}
}
