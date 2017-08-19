#pragma once

#include "../common/library.h"
#include "../common/stl.h"

class Console
{
private:
	Console();
	virtual ~Console();

public:
	static void CheckStatus(int status, char* msg);
};

