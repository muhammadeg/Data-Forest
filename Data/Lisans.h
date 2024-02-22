#include "curl\curl.h"
using namespace std;
#pragma comment(lib, "curl/libcurl_a.lib")
//#pragma comment(lib, "D:/lib/libcurl_a.lib")
#pragma comment(lib, "iphlpapi.lib")
#include "sha256.h"
#include <stdint.h>
typedef std::basic_string<TCHAR> tstring;
#define WIN32_LEAN_AND_MEAN        
#include <windows.h>      
#include <intrin.h>       
#include <iphlpapi.h>    
	
int ScaniaLicense = 0;
uint16_t ma1;
uint16_t ma2;
string data;
std::string GetLicenseKey()
{
	char K1[256] = "";
	EP_ProtectedStringByKey("RBsrtjj8123g", K1, sizeof(K1));

	// Code to retrieve the license key
	return K1;
}

std::string GetHWID()
{
	// Code to retrieve the hardware ID
	std::string hwid;
	HW_PROFILE_INFO hwProfileInfo;

	if (GetCurrentHwProfile(&hwProfileInfo) != NULL)
	{
		hwid = hwProfileInfo.szHwProfileGuid;
	}

	return hwid;
}

std::vector<std::string> getExplode(const std::string delimiter, const std::string ender, const std::string str) {

	std::vector<std::string> results;

	std::vector<std::string> arr = explode(delimiter, str);
	for (size_t i = 1; i < arr.size(); i++) {
		std::vector<std::string> arrend = explode(ender, arr[i]);
		results.push_back(arrend[0]);
	}
	return results;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

string getData(string url){
	CURL *curl;
	CURLcode res;
	std::string readBuffer;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}

	return readBuffer;

}
// Function to delete a registry key and its subkeys recursively
LONG DeleteRegistryKey(const HKEY hKeyRoot, const std::wstring& subkey) {
	LONG result = RegDeleteTreeW(hKeyRoot, subkey.c_str());
	return result;
}

int RemoteLisansCheck() {
	// Get license key and hardware ID
	std::string licenseKey = GetLicenseKey();
	std::string hwid = GetHWID();
	char K1[256] = "";
	EP_ProtectedStringByKey("Pv3k5R2t8nXj9sFp", K1, sizeof(K1));

	// Construct URL to check license remotely
	std::string url = K1 + licenseKey;

	// Retrieve data from the URL
	std::string response = getData(url);

	// Parse the response
	if (response.find("OK") != std::string::npos) {
		ScaniaLicense = 1;
		return 1; // License is valid
	}
	else {
		ScaniaLicense = 0;

		// License is invalid, delete the registry key
		std::wstring subkeyToDelete = L"Software\\KalOnline Core - Scania";
		LONG deleteResult = DeleteRegistryKey(HKEY_CURRENT_USER, subkeyToDelete);


		return 0; // License is invalid
	}
}