#pragma once
#include "Info.h"

struct CertExpiryInfo
{
	string certName;
	int certExpiryDays;
};

class InfoSystem : public Info
{
public:
	InfoSystem();
	~InfoSystem();

public:
	static	bool			RebootRequired();		// Does this machine requires a reboot
	static	bool			AutoUpdateEnabled();	// Auto update enabled
	static	double			GetCpuUsage();
	static	double			GetRamUsage();
	static	double		    GetDriveUsageFreeBytesAvailable();
	static	double		    GetDriveUsageTotalNumberOfBytes();
	static	string			QueryLocalHostname();
	static  string			QueryLastSystemUpdateDate();
	static	string			QueryAntiVirusStatus();
	static  string			GetCurrentDateTime();
	static  string          GetOSArchName();
	static  string          GetOSName();

	static  vector<CertExpiryInfo> ScanCertExpiry();

};
