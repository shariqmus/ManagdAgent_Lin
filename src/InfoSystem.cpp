#include "../include/stdafx.h"
#include "../include/InfoSystem.h"

#include "../include/Utility.h"
#include "../include/ShellProcess.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #pragma warning (disable : 4996)
#endif // defined

InfoSystem::InfoSystem()
{
}


InfoSystem::~InfoSystem()
{
}

// Does this machine requires a reboot
bool InfoSystem::RebootRequired()
{
	bool bRet = false;

    return bRet;
}


bool InfoSystem::AutoUpdateEnabled()
{
	bool bRet = false;

    return bRet;
}

double InfoSystem::GetCpuUsage()
{
    double percent;
    FILE* file;
    unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

    static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

    /*FILE* file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow, &lastTotalSys, &lastTotalIdle);
    fclose(file);
*/

    file = fopen("/proc/stat", "r");
    fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow, &totalSys, &totalIdle);
    fclose(file);

    if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow ||
        totalSys < lastTotalSys || totalIdle < lastTotalIdle)
    {
        //Overflow detection. Just skip this value.
        percent = -1.0;
    }
    else
    {
        total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) +
            (totalSys - lastTotalSys);
        percent = total;
        total += (totalIdle - lastTotalIdle);
        percent /= total;
        percent *= 100;
    }

    lastTotalUser = totalUser;
    lastTotalUserLow = totalUserLow;
    lastTotalSys = totalSys;
    lastTotalIdle = totalIdle;

    return percent;
}

double InfoSystem::GetRamUsage()
{
    struct sysinfo memInfo;

    sysinfo (&memInfo);
    //long long totalVirtualMem = memInfo.totalram;

    //Add other values in next statement to avoid int overflow on right hand side...
    //totalVirtualMem += memInfo.totalswap;
    //totalVirtualMem *= memInfo.mem_unit;

    long long physMemUsed = memInfo.totalram - memInfo.freeram;

    //Multiply in next statement to avoid int overflow on right hand side...
    //physMemUsed *= memInfo.mem_unit;
    //return (int) physMemUsed;

    //USED% = (USED/TOTAL)*100
    double physMemUsedPercentage = ((double)physMemUsed / (double)memInfo.totalram);

    physMemUsedPercentage *= memInfo.mem_unit;
    physMemUsedPercentage *= 100;

    return physMemUsedPercentage;

}

double InfoSystem::GetDriveUsageFreeBytesAvailable()
{
    string diskMountPoint = "/";

    const uint GB = (1024 * 1024) * 1024;

    struct statvfs buffer;
    int ret = statvfs(diskMountPoint.c_str(), &buffer);

    if (!ret)
    {
        //const double total = ceil((double)(buffer.f_blocks * buffer.f_frsize) / GB);
        const double available = ceil((double)(buffer.f_bfree * buffer.f_frsize) / GB);
        //const double used = total - available;
        //const double usedPercentage = ceil((double)(used / total) * (double)100);

        return available;
    }

    return 0;

}

double InfoSystem::GetDriveUsageTotalNumberOfBytes()
{
    string diskMountPoint = "/";

    const uint GB = (1024 * 1024) * 1024;

    struct statvfs buffer;
    int ret = statvfs(diskMountPoint.c_str(), &buffer);

    if (!ret)
    {
        const double total = ceil((double)(buffer.f_blocks * buffer.f_frsize) / GB);
        //const double available = ceil((double)(buffer.f_bfree * buffer.f_frsize) / GB);
        //const double used = total - available;
        //const double usedPercentage = ceil((double)(used / total) * (double)100);

        return total;
    }

    return 0;
}

string InfoSystem::QueryLocalHostname()
{
    char hostname[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];

    gethostname(hostname, HOST_NAME_MAX);
    getlogin_r(username, LOGIN_NAME_MAX);

    return string(hostname);
}

string InfoSystem::QueryLastSystemUpdateDate()
{
	string lastSystemUpdateDate = "N/A";

	//Read file
    ifstream t("QuerySystemUpdates.sh");
    string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());

	string scriptOutput = "";

	scriptOutput = ShellProcess::exec(str);
	//scriptOutput = "Dec 10 04:21:00 Updated: amazon-ssm-agent-2.2.120.0-1.amzn1.x86_64"; // Debug

	stringstream iss(scriptOutput);

	for (string line; std::getline(iss, line); )
	{
		vector<string> v = Utility::explode(line, ' ');

        int i = 0;

		for (vector<string>::iterator it = v.begin(); it != v.end(); ++it)
		{
			lastSystemUpdateDate += *it;

			if (i == 2)
            {
                break;
            }
            else
            {
                lastSystemUpdateDate += " ";
                i++;
            }
		}
	}

	return lastSystemUpdateDate;
}

string InfoSystem::QueryAntiVirusStatus()
{
	string antiVirusStatus = "N/A";

	// This script should be smart enough to tell us the status of antivirus on this host

    //Read file
    ifstream t("QueryAntiVirusStatus.sh");
    string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());

	string scriptOutput = "";

	scriptOutput = ShellProcess::exec(str);

	stringstream iss(scriptOutput);

	for (string line; std::getline(iss, line); )
	{
		vector<string> v = Utility::explode(line, ';');

		for (std::vector<string>::iterator it = v.begin(); it != v.end(); ++it)
		{
			antiVirusStatus = *it;
			break; // only interested in first record
		}
	}

	return antiVirusStatus;
}

string InfoSystem::GetCurrentDateTime()
{
	return Utility::getCurrentDateTime();
}

vector<CertExpiryInfo> InfoSystem::ScanCertExpiry()
{
	vector<CertExpiryInfo> certExpiryInfo;

	return certExpiryInfo;
}

string InfoSystem::GetOSArchName()
{
    struct utsname uts;
    uname(&uts);
    return uts.sysname;
}

string InfoSystem::GetOSName()
{
	string osName = "N/A";
	string fileName = "/etc/system-release";
	string cmd = "cat " + fileName;

	// Check if file exists

    ifstream f(fileName.c_str());

    if (f.good())
    {
        osName = ShellProcess::CleanResult(ShellProcess::exec(cmd));
    }

	return osName;
}
