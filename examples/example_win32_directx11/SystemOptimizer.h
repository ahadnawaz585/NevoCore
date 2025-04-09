#ifndef SYSTEM_OPTIMIZER_H
#define SYSTEM_OPTIMIZER_H

#include <string>
#include <vector>
#include <windows.h>
#include <tlhelp32.h>
#include <curl/curl.h>
#include <iphlpapi.h>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <chrono>
#include <thread>
#include <spdlog/spdlog.h> // Required for spdlog::logger
#include <memory>
#include <iomanip>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include "NetworkTweaks.h"
#include "ProcessTweaks.h"
#include "RegistryTweaks.h"
#include "SystemTweaks.h"
#include "GraphicsTweaks.h"

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "libcurl.lib")

struct SystemMetrics {
    unsigned long long ramAvailable;  // KB
    double cpuUsage;                  // Percentage
    unsigned long long networkBytes;  // Bytes
};

struct TweakResult {
    bool success;
    std::string message;
    int tweaksApplied;
    int tweaksTotal;
};

struct OptimizationResult {
    double optimizationPercentage;
    int tweaksMissingBefore;
    int tweaksAppliedAfter;
    std::string summary;
};

class SystemOptimizer {
private:
    std::string selfProcessName;
    std::string apiKey;
    double optimizedPercentage;
    static constexpr size_t MAX_LOGS = 100;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
    bool isRunningAsAdmin();
    bool elevatePrivileges();
    void getRunningProcesses(std::vector<std::string>& foregroundProcesses, std::vector<std::string>& backgroundProcesses);
    std::string getGrokResponse(const std::vector<std::string>& foregroundProcesses, const std::vector<std::string>& backgroundProcesses);
    unsigned long long getTotalRam();

    TweakResult performBasicOptimizationInternal(SystemMetrics& before, SystemMetrics& after);
    TweakResult performAdvancedOptimizationInternal(SystemMetrics& before, SystemMetrics& after);
    TweakResult performExtremeOptimizationInternal(SystemMetrics& before, SystemMetrics& after);
    TweakResult performSystemRestoreInternal(SystemMetrics& before, SystemMetrics& after);

public:
    SystemOptimizer(const std::string& processName = "nevocore_ui.exe",
        const std::string& grokApiKey = "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");
    ~SystemOptimizer();
    std::shared_ptr<spdlog::logger> logger;
    void disableCoreParking(); // Kept for SystemTweaks access
    void enableGameMode();     // Kept for SystemTweaks access
    void optimizeGraphicsDrivers(); // Kept for GraphicsTweaks access
    void minimizeVisualEffects();   // Kept for GraphicsTweaks access
    std::string wstringToString(const WCHAR* wstr);
    std::string toLowerCase(const std::string& str);
    SystemMetrics getSystemMetrics();
    double getCurrentLoadPercentage();
    bool isGameProcess(const WCHAR* processName);
    bool checkAndElevatePrivileges();
    OptimizationResult performBasicOptimization();
    OptimizationResult performAdvancedOptimization();
    OptimizationResult performExtremeOptimization();
    OptimizationResult performSystemRestore();
    double run(const std::string& choice);
    std::vector<std::string> getLogs() const;
    void initializeDemoLogs();
};

#endif
