// ManagdAgent_Win.cpp : Defines the entry point for the console application.
//

#include "../include/stdafx.h"

#include "../include/Utility.h"
#include "../include/InfoSystem.h"

#define APP_CODE_VERSION "0.1.0"

int worker()
{
	FILE_LOG(logINFO) << "Starting process run..." << endl;
	FILE_LOG(logINFO) << "-------------------------------------------" << endl;

	// Instantiate cxxtimer::Timer object
	cxxtimer::Timer timer;

	// Start the timer
	timer.start();

	// json preparation
	Object jsonRoot, jsonSystem, jsonAlert, jsonMetrics;

	// System
	jsonSystem << "OS" << InfoSystem::GetOSArchName(); // Linux or Windows
	jsonSystem << "OSDescription" << InfoSystem::GetOSName();
	jsonSystem << "Hostname" << InfoSystem::QueryLocalHostname();
	jsonSystem << "SystemDateTime" << InfoSystem::GetCurrentDateTime();

	// Metrics
	double val = InfoSystem::GetCpuUsage();
	string str = Utility::double2string(val);

	jsonMetrics << "CpuUsage" << str;
	jsonMetrics << "CpuUsageUnit" << "Percent";

	jsonMetrics << "RamUsage" << InfoSystem::GetRamUsage();
	jsonMetrics << "RamUsageUnit" << "Percent";

	jsonMetrics << "DriveUsageFreeBytesAvailable" << InfoSystem::GetDriveUsageFreeBytesAvailable();
	jsonMetrics << "DriveUsageFreeBytesAvailableUnit" << "GB";

	jsonMetrics << "DriveUsageTotalNumberOfBytes" << InfoSystem::GetDriveUsageTotalNumberOfBytes();
	jsonMetrics << "DriveUsageTotalNumberOfBytesUnit" << "GB";

	// Alert
	jsonAlert << "RebootRequired" << InfoSystem::RebootRequired();
	jsonAlert << "AutoUpdateEnabled" << InfoSystem::AutoUpdateEnabled();
	jsonAlert << "LastSystemUpdateDateTime" << InfoSystem::QueryLastSystemUpdateDate();
	jsonAlert << "AntiVirusStatus" << InfoSystem::QueryAntiVirusStatus();

	// Root
	jsonRoot << "CodeVersion" << APP_CODE_VERSION;
	jsonRoot << "ConfigVersion" << Utility::GetConfigValue("agent", "version");
	jsonRoot << "AccountNumber" << Utility::GetConfigValue("agent", "account");

	jsonRoot << "System" << jsonSystem;
	jsonRoot << "Metrics" << jsonMetrics;

	vector<CertExpiryInfo> vecCertExpiry = InfoSystem::ScanCertExpiry();

	Array ar;

	for (vector<CertExpiryInfo>::iterator it = vecCertExpiry.begin(); it != vecCertExpiry.end(); it++)
	{
		CertExpiryInfo ce = *it;
		Object o;

		o << "Name" << ce.certName;
		o << "ExpiryDays" << ce.certExpiryDays;

		ar << o;
	}

	jsonAlert << "CertExpiry" << ar;
	jsonRoot << "Alerts" << jsonAlert;

	string data = jsonRoot.json().c_str();
	Utility::ReplaceStringInPlace(data, "\n", "");
	Utility::ReplaceStringInPlace(data, "\t", "");

	// Print on screen
	string s = jsonRoot.json().c_str();
	Utility::ReplaceStringInPlace(s, "\n", "\r\n");
	Utility::ReplaceStringInPlace(s, "\t", "  ");
	FILE_LOG(logINFO)  << s;

	// Post json
	string url = Utility::GetConfigValue("agent", "endpoint");
	FILE_LOG(logINFO)  << "Sending json: " << data;
	Utility::PostData(url, data);

	// Stop/pause the timer
	timer.stop();

	// Get the elapsed time
	FILE_LOG(logINFO)  << "Process took " << timer.count<chrono::seconds>() << " seconds to run." << endl;

	return 0;
}

int main()
{
    FILELog::ReportingLevel() = logDEBUG3;
	FILE* log_fd = fopen("agent.log", "a+");
	Output2FILE::Stream() = log_fd;

    unsigned int waitSeconds = Utility::string2int(Utility::GetConfigValue("agent", "waitperiod"));

	for (;;)
	{
		worker();
		sleep(waitSeconds);
	}

	fclose(log_fd);
}
