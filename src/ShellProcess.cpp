#include "../include/stdafx.h"
#include "../include/ShellProcess.h"

#include "../include/Utility.h"
#include "../include/ShellProcess.h"

ShellProcess::ShellProcess()
{
}


ShellProcess::~ShellProcess()
{
}

string ShellProcess::exec(const string cmd)
{
    array<char, 128> buffer;
    string result;
    shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);

    if (!pipe)
    {
        throw runtime_error("popen() failed!");
    }

    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }

    return result;
}

string ShellProcess::CleanResult(string subject)
{
	string result = "";
	stringstream iss(subject);

	for (string line; std::getline(iss, line); )
	{
		std::smatch sm;

		if ((line == "\r") || (line == "\n") || (line == "\r\n"))
		{
			//Empty line
		}
		else if (regex_search(line, sm, regex("^Windows")))
		{
			// Match found
		}
		else if (regex_search(line, sm, regex("^Copyright")))
		{
			// Match found
		}
		else
		{
			// Match not found
			line = Utility::removeCharacter(line, '\"');
			result += line;
		}
	}

	return result;
}
