#pragma once
enum {
	// token send to server
	BASE_TOKEN_HEARTBEAT				= 1,
	BASE_TOKEN_LOGIN,
	BASE_TOKEN_NEXT,					 // ��һ��(���ƶ��Ѿ��򿪶Ի���)

	SHELL_TOKEN_START,
	FILE_TOKEN_START,
	// command from server
	COMMAND_REPLAY_HEARTBEAT           = 100,
	COMMAND_ACTIVED,
	COMMAND_SHELLMANAGER,
	COMMAND_FILEMANAGER

};
