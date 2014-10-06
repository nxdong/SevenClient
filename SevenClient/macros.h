#pragma once
enum {
	// token send to server
	BASE_TOKEN_HEARTBEAT				= 1,
	BASE_TOKEN_LOGIN,
	BASE_TOKEN_NEXT,					 // 下一步(控制端已经打开对话框)

	SHELL_TOKEN_START,
	FILE_TOKEN_START,
	// command from server
	COMMAND_REPLAY_HEARTBEAT           = 100,
	COMMAND_ACTIVED,
	COMMAND_SHELLMANAGER,
	COMMAND_FILEMANAGER

};
