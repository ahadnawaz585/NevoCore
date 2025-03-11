#ifndef SYSTEM_OPTIMIZER_H
#define SYSTEM_OPTIMIZER_H

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <tlhelp32.h>
#include <curl/curl.h>
#include <iphlpapi.h>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <ctime>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "libcurl.lib")

struct SystemMetrics {
    unsigned long long ramAvailable;
    double cpuUsage;
    unsigned long long networkBytes;
};


struct EnumWindowsData {
    DWORD pid;
    HWND hwnd;
};

class SystemOptimizer {
private:
    double optimized = 50;
    std::string selfProcessName;
    std::string apiKey;
    std::vector<std::string> logs;
    static constexpr size_t MAX_LOGS = 100;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
    std::string wstringToString(const WCHAR* wstr);
    std::string toLowerCase(const std::string& str);
    bool isRunningAsAdmin();
    bool elevatePrivileges();
    void getRunningProcesses(std::vector<std::string>& foregroundProcesses, std::vector<std::string>& backgroundProcesses);
    std::string getGrokResponse(const std::vector<std::string>& foregroundProcesses, const std::vector<std::string>& backgroundProcesses);
    void killProcesses(const std::string& grokResponse, const std::string& selfProcessName);
    bool applyBasicTweaks();
    bool applyAdvancedTweaks();
    bool restoreDefaultSettings();
    void logEvent(const std::string& message);
 

public:

    SystemMetrics getSystemMetrics();
    std::vector<std::string> getLogs() const;
    SystemOptimizer(const std::string& processName = "SystemOptimizer.exe",
        const std::string& grokApiKey = "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");
    ~SystemOptimizer();
    double getCurrentLoadPercentage();
    void initializeDemoLogs();
    bool checkAndElevatePrivileges();
    bool performBasicOptimization(SystemMetrics& before, SystemMetrics& after);
    bool performAdvancedOptimization(SystemMetrics& before, SystemMetrics& after);
    bool performExtremeOptimization(SystemMetrics& before, SystemMetrics& after);
    bool performSystemRestore(SystemMetrics& before, SystemMetrics& after);
    double run(const std::string& choice);
    std::vector<std::string> getLogs();
};

#endif // SYSTEM_OPTIMIZER_H
