#pragma once

struct WriteThis {
	const char *readptr;
	size_t sizeleft;
};

class Utility
{
public:
	Utility();
	~Utility();

public:
	// String conversion
	static  string			wstring2string(wstring ws);
	static	string			double2string(double val);
	static	int				string2int(string val);

	// String Manipulation
	static  vector<string>	explode(string const& s, char delim);
	static	string			removeCharacter(string str, char ch);
	static  void			ReplaceStringInPlace(string& subject, const string& search, const string& replace);

	// File I/O

	// Config file
	static	string			GetConfigValue(string section, string param);

	//Json
	static	int				PostData(string url, string data);

	// Windows System
	static  string			getEXEPath();
	static  string			getCWD();
	static	string			getCurrentDateTime();

private:
	static size_t			ReadCallback(void *dest, size_t size, size_t nmemb, void *userp);
};
