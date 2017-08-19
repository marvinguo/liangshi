#include <stdio.h>
#include "../common/library.h"
#include "../common/config.h"
int main()
{
/*	uv_tcp_t server_handle;
	uv_loop_t* loop = uv_default_loop();

	uv_tcp_init(loop, &server_handle);

	const struct sockaddr_in addr = uv_ip4_addr(SERVER_ADDR, SERVER_PORT);
	if (uv_tcp_bind(&server_handle, addr))
	{
		Console::Fatal("uv_tcp_bind");
	}

	DatabaseEngine& engine = DatabaseEngine::GetInstance();
	engine.Init();

	const int backlog = 128;
	if (uv_listen((uv_stream_t*)&server_handle, backlog, SocketManager::OnConnection))
	{
		Console::Fatal("uv_listen");
	}

	printf("Listening at %s:%d\n", SERVER_ADDR, SERVER_PORT);

	uv_timer_t timer;
	uv_timer_init(loop, &timer);
	uv_timer_start(&timer, SocketManager::OnTimer, 5000, 5000);

	uv_run(loop, UV_RUN_DEFAULT);
*/
	return 0;
}