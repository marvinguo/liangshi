#include "Console.h"
#include <stdio.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

void Console::CheckStatus(int status, char* msg)
{
	if(status)
	{
		fprintf(stderr, "%s: Write error %s\n", msg, uv_err_name(status));
		exit(1);
	}
}
