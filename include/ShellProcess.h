#pragma once

#define BUFSIZE 4096

class ShellProcess
{
public:
	ShellProcess();
	~ShellProcess();

private:

public:
    static string       exec(const string cmd);
	static string       CleanResult(string subject);
};
