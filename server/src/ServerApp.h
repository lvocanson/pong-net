#pragma once

class ServerApp
{
public:

	ServerApp();
	int Run();
	~ServerApp();

private:

	bool IsQuitKeyPressed();
};
