#include "AnimationAction.h"
#include "ShowAction.h"
#include "SizeAction.h"
#include "HideAction.h"
#include "SwitchPageAction.h"
#include "LineWidthAction.h"
#include "LineStyleAction.h"
#include "LineColorAction.h"
#include "TopMostAction.h"
#include "BottomMostAction.h"
#include "TextChangedAction.h"
#include "NetworkShowAction.h"
#include "NetworkSwitchPageAction.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

AnimationAction* AnimationAction::CreateFromType(AnimationType type, LokiBuffer& buffer)
{
	msgpack::unpacked msg;
	msgpack::unpack(&msg, buffer.Data(), buffer.Size());

	AnimationAction *action = NULL;

	try
	{
		if(type == AnimationType_Show)
		{
			ShowAction* a = new ShowAction();
			msg.get().convert(a);
			action = a;
		}
		else if(type == AnimationType_Size)
		{
			SizeAction* a = new SizeAction();
			msg.get().convert(a);
			action = a;
		}
		else if(type == AnimationType_Hide)
		{
			HideAction* a = new HideAction();
			msg.get().convert(a);
			action = a;
		}
		else if(type == AnimationType_SwitchPage)
		{
			SwitchPageAction* a = new SwitchPageAction();
			msg.get().convert(a);
			action = a;
		}
		else if(type == AnimationType_LineWidth)
		{
			LineWidthAction* a = new LineWidthAction();
			msg.get().convert(a);
			action = a;
		}
		else if(type == AnimationType_LineStyle)
		{
			LineStyleAction* a = new LineStyleAction();
			msg.get().convert(a);
			action = a;
		}
		else if(type == AnimationType_LineColor)
		{
			LineColorAction* a = new LineColorAction();
			msg.get().convert(a);
			action = a;
		}
		else if(type == AnimationType_TopMost)
		{
			TopMostAction* a = new TopMostAction();
			msg.get().convert(a);
			action = a;
		}
		else if(type == AnimationType_BottomMost)
		{
			BottomMostAction* a = new BottomMostAction();
			msg.get().convert(a);
			action = a;
		}
		else if(type == AnimationType_TextChanged)
		{
			TextChangedAction* a = new TextChangedAction();
			msg.get().convert(a);
			action = a;
		}
		else if(type == AnimationType_NetworkShow)
		{
			NetworkShowAction* a = new NetworkShowAction();
			msg.get().convert(a);
			action = a;
		}
		else if(type == AnimationType_NetworkSwitchPage)
		{
			NetworkSwitchPageAction* a = new NetworkSwitchPageAction();
			msg.get().convert(a);
			action = a;
		}
	}
	catch(...)
	{
		action = NULL;
	}
	return action;
}

AnimationAction::AnimationAction()
{
	_played = false;
}

AnimationAction::~AnimationAction()
{
}
