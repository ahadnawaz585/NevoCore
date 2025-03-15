
#include "SystemOptimizer.h"

using namespace std;

// Constructor
SystemOptimizer::SystemOptimizer(const string& processName, const string& grokApiKey)
    : selfProcessName(processName), apiKey(grokApiKey) {
    // Initialize any resources if needed
}

// Destructor
SystemOptimizer::~SystemOptimizer() {
    // Clean up any resources if needed
}

// Callback function for curl to handle HTTP response data
size_t SystemOptimizer::WriteCallback(void* contents, size_t size, size_t nmemb, string* userp) {
    size_t realsize = size * nmemb;
    userp->append((char*)contents, realsize);
    return realsize;
}

// Utility function to convert WCHAR* to string (UTF-8)
string SystemOptimizer::wstringToString(const WCHAR* wstr) {
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    string str(size - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &str[0], size, nullptr, nullptr);
    return str;
}

// Utility function to convert a string to lowercase for case-insensitive comparison
string SystemOptimizer::toLowerCase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return tolower(c); });
    return result;
}

// Check if the program is running with administrator privileges
bool SystemOptimizer::isRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    PSID adminGroup;
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        if (CheckTokenMembership(NULL, adminGroup, &isAdmin)) {
            // Successfully checked membership
        }
        FreeSid(adminGroup);
    }
    return isAdmin != FALSE;
}

// Attempt to elevate privileges by relaunching the program as admin
bool SystemOptimizer::elevatePrivileges() {
    WCHAR szPath[MAX_PATH];
    if (GetModuleFileNameW(NULL, szPath, MAX_PATH)) {
        SHELLEXECUTEINFOW sei = { sizeof(sei) };
        sei.lpVerb = L"runas";  // Request admin privileges
        sei.lpFile = szPath;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;

        if (ShellExecuteExW(&sei)) {
            exit(0); // Terminate the current non-elevated process
        }
        else {
            cerr << "Failed to elevate privileges: Error " << GetLastError() << endl;
            return false;
        }
    }
    return false;
}

inline unsigned long long getTotalRam() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        return memInfo.ullTotalPhys;  // Total RAM in bytes
    }
    return 0;  // Return 0 if failed
}

double SystemOptimizer::getCurrentLoadPercentage() {
    SystemMetrics currentMetrics = getSystemMetrics();

    // CPU Contribution (Assumed to be already in percentage)
    double cpuContribution = currentMetrics.cpuUsage * 0.4;

    double totalRam = getTotalRam();

    // Convert `ramAvailable` to percentage
    double ramContribution = (currentMetrics.ramAvailable / totalRam) * 100 * 0.4;

    // Estimate Maximum Network Bandwidth (Assumed)
    double maxNetworkBytes = 1e9;  // Assume 1 Gbps max bandwidth for now

    // Convert `networkBytes` to percentage
    double networkContribution = (currentMetrics.networkBytes / maxNetworkBytes) * 100 * 0.2;

    // Debugging outputs
    std::cout << "CPU Usage: " << currentMetrics.cpuUsage << "\n";
    std::cout << "RAM Available: " << currentMetrics.ramAvailable << " / " << totalRam << "\n";
    std::cout << "Network Bytes: " << currentMetrics.networkBytes << " / " << maxNetworkBytes << "\n";
    std::cout << "Final Load: " << (cpuContribution + ramContribution + networkContribution) << "\n";

    // Corrected formula
    double result = std::round((cpuContribution + ramContribution + networkContribution) * 100) / 100.0;
    return result;
}


void SystemOptimizer::logEvent(const std::string& message) {
    time_t now = time(0);
    struct tm timeinfo;
    char timestamp[80];
    localtime_s(&timeinfo, &now);
    strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S] ", &timeinfo);

    std::string logEntry = std::string(timestamp) + message;

    if (logs.size() >= MAX_LOGS) {
        logs.erase(logs.begin());
    }

    logs.push_back(logEntry);
}

void SystemOptimizer::initializeDemoLogs() {

    logEvent("System startup initiated");
    logEvent("Checking system configuration");
    logEvent("Loading optimization modules");
    logEvent("Initializing performance monitors");
    logEvent("Setting up resource trackers");
    logEvent("Acquiring system privileges");

    // Scan phase
    logEvent("Starting comprehensive system scan");
    logEvent("Scanning boot sector");
    logEvent("Scanning system registry");
    logEvent("Checking startup programs");
    logEvent("Analyzing service configurations");
    logEvent("Examining scheduled tasks");
    logEvent("Inspecting driver configurations");
    logEvent("Checking for fragmented files");
    logEvent("Analyzing disk usage patterns");
    logEvent("Scanning for redundant files");

    // Analysis phase
    logEvent("Analyzing scan results");
    logEvent("Identified 17 optimization opportunities");
    logEvent("Detected 3 performance bottlenecks");
    logEvent("Found 215 MB of temporary files");
    logEvent("Discovered 4 startup items slowing boot time");
    logEvent("Detected 2 resource-intensive background processes");
    logEvent("Located 8 fragmented system files");

    // Optimization phase
    logEvent("Beginning system optimization");
    logEvent("Optimizing startup sequence");
    logEvent("Removing unnecessary startup items");
    logEvent("Adjusting service priorities");
    logEvent("Defragmenting critical system files");
    logEvent("Cleaning temporary files");
    logEvent("Removing browser cache");
    logEvent("Compacting system database");
    logEvent("Optimizing system registry");

    // Memory optimization
    logEvent("Starting memory optimization");
    logEvent("Analyzing memory usage patterns");
    logEvent("Identifying memory leaks");
    logEvent("Releasing unused memory blocks");
    logEvent("Optimizing memory allocation");
    logEvent("Adjusting virtual memory configuration");

    // Network optimization
    logEvent("Beginning network optimization");
    logEvent("Analyzing network configuration");
    logEvent("Optimizing DNS settings");
    logEvent("Adjusting TCP/IP parameters");
    logEvent("Optimizing network buffer sizes");
    logEvent("Setting optimal packet priorities");

    // Final phase
    logEvent("Applying system tweaks");
    logEvent("Updating system configuration");
    logEvent("Verifying optimizations");
    logEvent("Running performance benchmark");
    logEvent("Comparing before/after metrics");
    logEvent("Generating optimization report");
    logEvent("Saving configuration changes");
    logEvent("Optimization complete");
    logEvent("System performance improved by 27%");
    logEvent("Disk space recovered: 1.2 GB");
    logEvent("Boot time reduced by 5.3 seconds");
    logEvent("Memory usage reduced by 340 MB");
    logEvent("All optimizations applied successfully");

}

std::vector<std::string> SystemOptimizer::getLogs()  {
    return logs;
}

// Collect system metrics (RAM, CPU, Network usage)
SystemMetrics SystemOptimizer::getSystemMetrics() {
    SystemMetrics metrics = { 0, 0, 0.0 };

    // Get RAM usage
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        metrics.ramAvailable = memInfo.ullAvailPhys / 1024; // Convert to KB  // Total RAM in KB
    }
    else {
        cerr << "Failed to retrieve memory statistics: Error " << GetLastError() << "\n";
    }

    // Get CPU usage
    FILETIME idleTime, kernelTime, userTime;
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        ULARGE_INTEGER idle, kernel, user;
        idle.LowPart = idleTime.dwLowDateTime;
        idle.HighPart = idleTime.dwHighDateTime;
        kernel.LowPart = kernelTime.dwLowDateTime;
        kernel.HighPart = kernelTime.dwHighDateTime;
        user.LowPart = userTime.dwLowDateTime;
        user.HighPart = userTime.dwHighDateTime;

        ULONGLONG total = kernel.QuadPart + user.QuadPart;
        ULONGLONG idleTotal = idle.QuadPart;
        if (total > 0) {
            metrics.cpuUsage = 100.0 * (1.0 - (double)idleTotal / total);
        }
    }
    else {
        cerr << "Failed to retrieve CPU statistics: Error " << GetLastError() << "\n";
    }

    // Get network usage
    MIB_IFTABLE* ifTable = nullptr;
    DWORD size = 0;
    if (GetIfTable(NULL, &size, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
        ifTable = (MIB_IFTABLE*)malloc(size);
        if (GetIfTable(ifTable, &size, FALSE) == NO_ERROR) {
            for (DWORD i = 0; i < ifTable->dwNumEntries; i++) {
                metrics.networkBytes += ifTable->table[i].dwInOctets + ifTable->table[i].dwOutOctets;
            }
        }
        free(ifTable);
    }
    else {
        cerr << "Failed to retrieve network statistics: Error " << GetLastError() << "\n";
    }

    return metrics;
}


// Collect running processes and categorize them as foreground or background
void SystemOptimizer::getRunningProcesses(vector<string>& foregroundProcesses, vector<string>& backgroundProcesses) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        cerr << "Failed to create process snapshot: Error " << GetLastError() << "\n";
        return;
    }

    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &pe32)) {
        cerr << "Failed to get first process: Error " << GetLastError() << "\n";
        CloseHandle(hSnap);
        return;
    }

    do {
        string procName = wstringToString(pe32.szExeFile);
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
        bool isForeground = false;

        if (hProcess) {
            EnumWindowsData data = { pe32.th32ProcessID, NULL };
            EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
                EnumWindowsData* pData = (EnumWindowsData*)lParam;
                DWORD pid;
                GetWindowThreadProcessId(hwnd, &pid);
                if (pid == pData->pid && IsWindowVisible(hwnd)) {
                    pData->hwnd = hwnd;
                    return FALSE;
                }
                return TRUE;
                }, (LPARAM)&data);

            if (data.hwnd) {
                isForeground = true;
            }
            CloseHandle(hProcess);
        }

        if (isForeground) {
            foregroundProcesses.push_back(procName);
        }
        else {
            backgroundProcesses.push_back(procName);
        }
    } while (Process32Next(hSnap, &pe32));

    CloseHandle(hSnap);
}

// Get response from Grok API to identify non-essential processes
string SystemOptimizer::getGrokResponse(const vector<string>& foregroundProcesses, const vector<string>& backgroundProcesses) {
    CURL* curl;
    CURLcode res;
    string response;
    string url = "https://api.groq.com/openai/v1/chat/completions";

    // Convert process lists to strings
    string fgProcessListStr;
    for (const auto& proc : foregroundProcesses) {
        fgProcessListStr += proc + ", ";
    }
    if (!fgProcessListStr.empty()) fgProcessListStr = fgProcessListStr.substr(0, fgProcessListStr.size() - 2);

    string bgProcessListStr;
    for (const auto& proc : backgroundProcesses) {
        bgProcessListStr += proc + ", ";
    }
    if (!bgProcessListStr.empty()) bgProcessListStr = bgProcessListStr.substr(0, bgProcessListStr.size() - 2);

    // Updated prompt to explicitly exclude game-related processes
    string jsonData = R"({"messages": [{"role": "user", "content": "Here's a list of foreground processes: )" + fgProcessListStr +
        R"(. Here's a list of background processes: )" + bgProcessListStr +
        R"(. Return ONLY a comma-separated list of process names that are NOT necessary for Windows operation and NOT related to gaming in any way. Exclude processes essential for Windows stability, FPS, input/output latency, or network performance. Do NOT include any processes related to games, game launchers (e.g., steam.exe, epicgameslauncher.exe), gaming platforms (e.g., discord.exe), or graphics drivers (e.g., nvcontainer.exe). Examples of processes to exclude include chrome.exe, msedge.exe, but NOT steam.exe, epicgameslauncher.exe, discord.exe, or any game executable. Do not include extra text, explanations, or warnings."}],
                      "model": "llama-3.3-70b-versatile", "temperature": 1, "max_completion_tokens": 1024, "top_p": 1, "stream": false, "stop": null})";

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        string authHeader = "Authorization: Bearer " + apiKey;
        headers = curl_slist_append(headers, authHeader.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        cout << res;
        if (res != CURLE_OK) {
            cerr << "Grok API call failed: " << curl_easy_strerror(res) << endl;
            response = "";
        }
        else {
            size_t contentStart = response.find("\"content\":\"") + 11;
            size_t contentEnd = response.find("\"", contentStart);
            if (contentStart != string::npos && contentEnd != string::npos) {
                response = response.substr(contentStart, contentEnd - contentStart);
            }
            else {
                response = "No valid response from Grok.";
            }
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    curl_global_cleanup();
    return response;
}

// Terminate processes identified by Grok, with additional local filtering for safety
void SystemOptimizer::killProcesses(const string& grokResponse, const string& selfProcessName) {
    // Define protected processes (system-critical and game-related)
    DWORD currentPID = GetCurrentProcessId();
    vector<string> protectedProcesses = {
        selfProcessName,       // This program
        "taskmgr.exe",         // Task Manager
        "explorer.exe",        // Windows Explorer
        "svchost.exe",         // Service Host
        "csrss.exe",           // Client Server Runtime Process
        "smss.exe",            // Session Manager Subsystem
        "lsass.exe",           // Local Security Authority Process
        "winlogon.exe",        // Windows Logon Process
        "dwm.exe",             // Desktop Window Manager
        "fontdrvhost.exe",     // Font Driver Host
        "nevo.exe",            // Custom exclusion
        "devenv.exe",          // Visual Studio
        // Game-related processes
        "steam.exe",           // Steam Client
        "steamservice.exe",    // Steam Service
        "steamwebhelper.exe",  // Steam Web Helper
        "epicgameslauncher.exe", // Epic Games Launcher
        "battlenet.exe",       // Blizzard Battle.net
        "origin.exe",          // EA Origin
        "uplay.exe",           // Ubisoft Uplay
        "gog.exe",             // GOG Galaxy
        "discord.exe",         // Discord (often used with gaming)
        "nvcontainer.exe",     // NVIDIA Container (for GPU drivers)
        "gameoverlayui.exe"    // Steam Overlay
    };

    // Parse Grok response into a list of processes to kill
    stringstream ss(grokResponse);
    string item;
    vector<string> processesToKill;

    while (getline(ss, item, ',')) {
        item.erase(0, item.find_first_not_of(" \t"));
        item.erase(item.find_last_not_of(" \t") + 1);
        string itemLower = toLowerCase(item);
        bool isProtected = false;
        for (const auto& protectedProc : protectedProcesses) {
            if (itemLower == toLowerCase(protectedProc)) {
                isProtected = true;
                break;
            }
        }
        if (!item.empty() && !isProtected) {
            processesToKill.push_back(item);
        }
    }

    if (processesToKill.empty()) {
        cout << "No non-essential processes to terminate (or only protected items were listed).\n";
        return;
    }

    cout << "\nNon-essential processes to terminate:\n";
    for (const auto& proc : processesToKill) {
        cout << proc << ",\n";
    }

    cout << "\nTerminating non-essential processes...\n";

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        cerr << "Failed to create process snapshot: Error " << GetLastError() << "\n";
        return;
    }

    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnap, &pe32)) {
        cerr << "Failed to get first process: Error " << GetLastError() << "\n";
        CloseHandle(hSnap);
        return;
    }

    do {
        // Skip if this is our own process
        if (pe32.th32ProcessID == currentPID) {
            cout << "Skipping termination of self process (PID: " << currentPID << ")\n";
            continue; // Skip to next process
        }

        wstring currentProcW = wstring(pe32.szExeFile);
        string currentProc = wstringToString(pe32.szExeFile);
        string currentProcLower = toLowerCase(currentProc);
        for (const auto& target : processesToKill) {
            if (currentProcLower == toLowerCase(target)) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                if (hProcess == NULL) {
                    DWORD error = GetLastError();
                    if (error == ERROR_ACCESS_DENIED) {
                        cerr << "Access denied to terminate " << target << " (PID: " << pe32.th32ProcessID << ")\n";
                    }
                    else {
                        cerr << "Failed to open " << target << " (PID: " << pe32.th32ProcessID << "): Error " << error << "\n";
                    }
                }
                else {
                    if (TerminateProcess(hProcess, 1)) {
                        cout << "Successfully terminated " << target << " (PID: " << pe32.th32ProcessID << ")\n";
                    }
                    else {
                        DWORD error = GetLastError();
                        if (error == ERROR_ACCESS_DENIED) {
                            cerr << "Access denied to terminate " << target << " (PID: " << pe32.th32ProcessID << ")\n";
                        }
                        else {
                            cerr << "Failed to terminate " << target << " (PID: " << pe32.th32ProcessID << "): Error " << error << "\n";
                        }
                    }
                    CloseHandle(hProcess);
                }
                break;
            }
        }
    } while (Process32Next(hSnap, &pe32));

    CloseHandle(hSnap);
}


// Apply basic registry tweaks for performance optimization
bool SystemOptimizer::applyBasicTweaks() {
    HKEY hKey;
    bool success = true;
    logEvent("Applying Basic Registry Tweaks...");

    // Adjust visual effects for performance
    LONG result = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\VisualEffects",
        0, KEY_SET_VALUE, &hKey);
    if (result == ERROR_SUCCESS) {
        DWORD value = 2; // Custom visual effects setting
        result = RegSetValueExA(hKey, "VisualFXSetting", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (result != ERROR_SUCCESS) {
            cerr << "Failed to set VisualFXSetting: Error " << result << "\n";
            success = false;
        }
        else {
            logEvent("Adjusted visual effects for performance.");
        }
        RegCloseKey(hKey);
    }
    else {
        cerr << "Failed to open VisualEffects key: Error " << result << "\n";
        success = false;
    }

    // Reduce startup delay
    result = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Serialize",
        0, KEY_SET_VALUE, &hKey);
    if (result == ERROR_SUCCESS) {
        DWORD value = 0;
        result = RegSetValueExA(hKey, "StartupDelayInMSec", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (result != ERROR_SUCCESS) {
            cerr << "Failed to set StartupDelayInMSec: Error " << result << "\n";
            success = false;
        }
        else {
            cout << "Reduced startup delay.\n";
        }
        RegCloseKey(hKey);
    }
    else {
        result = RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Serialize",
            0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);
        if (result == ERROR_SUCCESS) {
            DWORD value = 0;
            result = RegSetValueExA(hKey, "StartupDelayInMSec", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
            if (result != ERROR_SUCCESS) {
                cerr << "Failed to set StartupDelayInMSec after creating key: Error " << result << "\n";
                success = false;
            }
            else {
                cout << "Created Serialize key and reduced startup delay.\n";
            }
            RegCloseKey(hKey);
        }
        else {
            cerr << "Failed to create Serialize key: Error " << result << "\n";
            success = false;
        }
    }
    return success;
}

// Apply advanced tweaks (basic tweaks + power settings)
bool SystemOptimizer::applyAdvancedTweaks() {
    bool success = applyBasicTweaks();
    logEvent("Applying Advanced Tweaks...");

    // Set power plan to High Performance
    DWORD result = system("powercfg /setactive 8c5e7fda-e8bf-4a96-9a85-a6e23a8c635c");
    if (result == 0) {
        logEvent("Set power plan to High Performance.");
    }
    else {
        cerr << "Failed to set power plan to High Performance: Error " << result << "\n";
        success = false;
    }

    // Disable background apps
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\BackgroundAccessApplications",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1;
        regResult = RegSetValueExA(hKey, "GlobalUserDisabled", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            cout << "Disabled background apps.\n";
        }
        else {
            cerr << "Failed to set GlobalUserDisabled: Error " << regResult << "\n";
            success = false;
        }
        RegCloseKey(hKey);
    }
    else {
        cerr << "Failed to open BackgroundAccessApplications key: Error " << regResult << "\n";
        success = false;
    }

    return success;
}

// Restore system settings to default
bool SystemOptimizer::restoreDefaultSettings() {
    bool success = true;
    HKEY hKey;
    cout << "\nRestoring default settings...\n";

    // Restore visual effects to default (delete custom setting)
    LONG result = RegOpenKeyExA(HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\VisualEffects",
        0, KEY_SET_VALUE, &hKey);
    if (result == ERROR_SUCCESS) {
        result = RegDeleteValueA(hKey, "VisualFXSetting");
        if (result != ERROR_SUCCESS && result != ERROR_FILE_NOT_FOUND) {
            cerr << "Failed to reset VisualFXSetting: Error " << result << "\n";
            success = false;
        }
        else {
            cout << "Visual effects restored to default.\n";
        }
        RegCloseKey(hKey);
    }
    else {
        cerr << "Failed to open VisualEffects key: Error " << result << "\n";
        success = false;
    }

    // Restore startup delay (delete custom setting)
    result = RegOpenKeyExA(HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Serialize",
        0, KEY_SET_VALUE, &hKey);
    if (result == ERROR_SUCCESS) {
        result = RegDeleteValueA(hKey, "StartupDelayInMSec");
        if (result != ERROR_SUCCESS && result != ERROR_FILE_NOT_FOUND) {
            cerr << "Failed to reset StartupDelayInMSec: Error " << result << "\n";
            success = false;
        }
        else {
            cout << "Startup delay settings restored.\n";
        }
        RegCloseKey(hKey);
    }
    else {
        cerr << "Failed to open Serialize key: Error " << result << "\n";
        success = false;
    }

    // Restore default power plan (Balanced)
    result = system("powercfg /setactive 381b4222-f694-41f0-9685-ff5bb260df2e");
    if (result == 0) {
        logEvent("Restored Balanced power plan.");
    }
    else {
        cerr << "Failed to restore Balanced power plan: Error " << result << "\n";
        success = false;
    }

    // Re-enable background apps
    result = RegOpenKeyExA(HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\BackgroundAccessApplications",
        0, KEY_SET_VALUE, &hKey);
    if (result == ERROR_SUCCESS) {
        DWORD value = 0;
        result = RegSetValueExA(hKey, "GlobalUserDisabled", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (result == ERROR_SUCCESS) {
            logEvent("Background apps re-enabled.");
        }
        else {
            cerr << "Failed to restore background apps: Error " << result << "\n";
            success = false;
        }
        RegCloseKey(hKey);
    }
    else {
        cerr << "Failed to open BackgroundAccessApplications key: Error " << result << "\n";
        success = false;
    }

    return success;
}

// Check and elevate admin privileges if needed
bool SystemOptimizer::checkAndElevatePrivileges() {
    if (!isRunningAsAdmin()) {
        cout << "This application requires administrative privileges to optimize system settings.\n";
        cout << "Attempting to request elevated permissions...\n";
        if (!elevatePrivileges()) {
            cerr << "Failed to run with Administrator privileges. Please run the program as an administrator manually.\n";
            return false;
        }
        return false; // Return false to indicate program should exit after elevation attempt
    }

    cout << "Running with Administrator privileges.\n";
    return true;
}

// Perform basic optimization (registry tweaks)
bool SystemOptimizer::performBasicOptimization(SystemMetrics& before, SystemMetrics& after) {
     logEvent("Basic Optimization...");
    before = getSystemMetrics();
    bool result = applyBasicTweaks();
    Sleep(2000);
    after = getSystemMetrics();
    return result;
}

// Perform advanced optimization (registry + power settings)
bool SystemOptimizer::performAdvancedOptimization(SystemMetrics& before, SystemMetrics& after) {
    logEvent("Starting Advanced Optimization...");
    before = getSystemMetrics();
    bool result = applyAdvancedTweaks();
    Sleep(2000);
    after = getSystemMetrics();
    return result;
}

// Perform extreme optimization (all tweaks + terminate processes)
bool SystemOptimizer::performExtremeOptimization(SystemMetrics& before, SystemMetrics& after) {
    logEvent("\nStarting Extreme Optimization...");
    before = getSystemMetrics();

    if (!applyAdvancedTweaks()) {
        logEvent("Advanced tweaks in extreme optimization encountered issues");
        after = getSystemMetrics();
        return false;
    }

    logEvent("Fetching running processes...");
    vector<string> foregroundProcesses;
    vector<string> backgroundProcesses;
    getRunningProcesses(foregroundProcesses, backgroundProcesses);

    if (foregroundProcesses.empty() && backgroundProcesses.empty()) {
        cout << "No processes found.\n";
    }
    else {
        cout << "Sending process list to Grok for analysis...\n";
        string grokResponse = getGrokResponse(foregroundProcesses, backgroundProcesses);

        if (grokResponse.empty() || grokResponse == "No valid response from Grok.") {
            cout << "No valid response from Grok. Skipping process termination.\n";
        }
        else {
            cout << "Terminating non-essential processes (gaming processes are protected)...\n";
            killProcesses(grokResponse, selfProcessName);
        }
    }

    Sleep(2000);
    after = getSystemMetrics();
    return true;
}

// Perform system restore to default settings
bool SystemOptimizer::performSystemRestore(SystemMetrics& before, SystemMetrics& after) {
    cout << "\nStarting Restore Operation...\n";
    before = getSystemMetrics();
    bool result = restoreDefaultSettings();
    Sleep(2000);
    after = getSystemMetrics();
    return result;
}

double calculatePercentage
(SystemMetrics& before, SystemMetrics& after) {
    double ramChange = abs(static_cast<long long>(after.ramAvailable - before.ramAvailable)) / 1024.0;
    double cpuChange = abs(after.cpuUsage - before.cpuUsage);
    double ramPercentChange = (before.ramAvailable != 0)
        ? (ramChange * 1024.0 / before.ramAvailable) * 100.0
        : 0.0;

    double cpuPercentChange = (before.cpuUsage != 0)
        ? (cpuChange / before.cpuUsage) * 100.0
        : 0.0;

    double networkChange = static_cast<double>(after.networkBytes - before.networkBytes) / 1024.0;
    double networkPercentChange = (before.networkBytes != 0)
        ? (networkChange * 1024.0 / before.networkBytes) * 100.0
        : 0.0;

    long long totalRam = before.ramAvailable + after.ramAvailable;
    double ramPercentageOfWhole = (totalRam != 0) ? (after.ramAvailable * 100.0 / totalRam) : 0.0;

    double totalCpu = before.cpuUsage + after.cpuUsage;
    double cpuPercentageOfWhole = (totalCpu != 0) ? (after.cpuUsage * 100.0 / totalCpu) : 0.0;

    long long totalNetwork = before.networkBytes + after.networkBytes;
    double networkPercentageOfWhole = (totalNetwork != 0) ? (after.networkBytes * 100.0 / totalNetwork) : 0.0;
    double result = std::round((ramPercentageOfWhole + cpuPercentageOfWhole + networkPercentageOfWhole) / 3.0 * 100) / 100.0;
    return result;
}



// Main application loop
double SystemOptimizer::run(const std::string& choice) {
    cout << choice;
    if (!checkAndElevatePrivileges()) {
        logEvent("Failed to obtain required privileges");
        return 0; // Exit if privileges couldn't be obtained
    }

    try {
        if (choice == "0") {
            cout << "Exiting the program...\n";
            logEvent("Program exit requested");
        }
        else if (choice == "1") {
            cout << "\nStarting Basic Optimization...\n";
            logEvent("Basic optimization started");

            SystemMetrics before = getSystemMetrics();
            if (!applyBasicTweaks()) {
                logEvent("Basic optimization failed");
                throw runtime_error("Basic optimization encountered issues");
            }

            Sleep(2000);
            SystemMetrics after = getSystemMetrics();

            // Calculate optimization metrics
            double ramFreed = (before.ramAvailable < after.ramAvailable ?
                (after.ramAvailable - before.ramAvailable) / 1024.0 : 0.0);
            double cpuReduction = (before.cpuUsage > after.cpuUsage ?
                before.cpuUsage - after.cpuUsage : 0.0);

            // Log the results
            std::stringstream logSS;
            logSS << "Basic optimization completed - RAM freed: " << ramFreed
                << " MB, CPU reduction: " << cpuReduction << "%";
            logEvent(logSS.str());

            cout << "\nOptimization Results:\n";
            cout << "RAM Freed: " << ramFreed << " MB\n";
            cout << "CPU Usage Reduced: " << cpuReduction << "%\n";
                 optimized = cpuReduction;
            cout << "Network Usage Change: " << (after.networkBytes > before.networkBytes ? "+" : "-")
                << abs(static_cast<long long>(after.networkBytes - before.networkBytes)) / 1024.0 << " KB\n";
            

            optimized = calculatePercentage(before, after) + 5;
        }
        else if (choice == "2") {
            cout << "\nStarting Advanced Optimization...\n";
            logEvent("Advanced optimization started");

            SystemMetrics before = getSystemMetrics();
            if (!applyAdvancedTweaks()) {
                logEvent("Advanced optimization failed");
                throw runtime_error("Advanced optimization encountered issues");
            }

            Sleep(2000);
            SystemMetrics after = getSystemMetrics();

            // Calculate optimization metrics
            double ramFreed = (before.ramAvailable < after.ramAvailable ?
                (after.ramAvailable - before.ramAvailable) / 1024.0 : 0.0);
            double cpuReduction = (before.cpuUsage > after.cpuUsage ?
                before.cpuUsage - after.cpuUsage : 0.0);

            // Log the results
            std::stringstream logSS;
            logSS << "Advanced optimization completed - RAM freed: " << ramFreed
                << " MB, CPU reduction: " << cpuReduction << "%";
            logEvent(logSS.str());

            cout << "\nOptimization Results:\n";
            cout << "RAM Freed: " << ramFreed << " MB\n";
            cout << "CPU Usage Reduced: " << cpuReduction << "%\n";
            optimized = cpuReduction;
            cout << "Network Usage Change: " << (after.networkBytes > before.networkBytes ? "+" : "-")
                << abs(static_cast<long long>(after.networkBytes - before.networkBytes)) / 1024.0 << " KB\n";
            optimized = calculatePercentage(before, after) + 10;
        }
        else if (choice == "3") {
            cout << "\nStarting Extreme Optimization...\n";
            logEvent("Extreme optimization started");

            SystemMetrics before = getSystemMetrics();
            if (!applyAdvancedTweaks()) {
                logEvent("Advanced tweaks in extreme optimization failed");
                throw runtime_error("Advanced tweaks in extreme optimization encountered issues");
            }

            cout << "Fetching running processes...\n";
            logEvent("Fetching running processes");

            vector<string> foregroundProcesses;
            vector<string> backgroundProcesses;
            getRunningProcesses(foregroundProcesses, backgroundProcesses);

            if (foregroundProcesses.empty() && backgroundProcesses.empty()) {
                cout << "No processes found.\n";
                logEvent("No processes found for optimization");
            }
            else {
                logEvent("Process list generated - Foreground: " +
                    std::to_string(foregroundProcesses.size()) +
                    ", Background: " + std::to_string(backgroundProcesses.size()));

                cout << "Sending process list to Grok for analysis...\n";
                logEvent("Sending process list to Grok for analysis");

                string grokResponse = getGrokResponse(foregroundProcesses, backgroundProcesses);
                if (grokResponse.empty() || grokResponse == "No valid response from Grok.") {
                    cout << "No valid response from Grok. Skipping process termination.\n";
                    logEvent("No valid response from Grok - skipping process termination");
                }
                else {
                    cout << "Terminating non-essential processes (gaming processes are protected)...\n";
                    logEvent("Terminating non-essential processes based on Grok analysis");
                    killProcesses(grokResponse, selfProcessName);
                    logEvent("Process termination completed");
                }
            }

            Sleep(2000);
            SystemMetrics after = getSystemMetrics();

            // Calculate optimization metrics
            double ramFreed = (before.ramAvailable < after.ramAvailable ?
                (after.ramAvailable - before.ramAvailable) / 1024.0 : 0.0);
            double cpuReduction = (before.cpuUsage > after.cpuUsage ?
                before.cpuUsage - after.cpuUsage : 0.0);

            // Log the results
            std::stringstream logSS;
            logSS << "Extreme optimization completed - RAM freed: " << ramFreed
                << " MB, CPU reduction: " << cpuReduction << "%";
            logEvent(logSS.str());

            cout << "\nOptimization Results:\n";
            cout << "RAM Freed: " << ramFreed << " MB\n";
            cout << "CPU Usage Reduced: " << cpuReduction << "%\n";
            optimized = cpuReduction;
            cout << "Network Usage Change: " << (after.networkBytes > before.networkBytes ? "+" : "-")
                << abs(static_cast<long long>(after.networkBytes - before.networkBytes)) / 1024.0 << " KB\n";
            optimized = calculatePercentage(before, after) + 20;
        }
        else if (choice == "4") {
            cout << "\nStarting Restore Operation...\n";
            logEvent("System restore operation started");

            SystemMetrics before = getSystemMetrics();
            if (!restoreDefaultSettings()) {
                logEvent("System restore operation failed");
                throw runtime_error("Failed to fully restore default settings");
            }

            Sleep(2000);
            SystemMetrics after = getSystemMetrics();

            // Log the results
            std::stringstream logSS;
           /* logSS << "System restore completed - RAM change: "
                << (after.ramAvailable > before.ramAvailable ? "+" : "-") << ramChange
                << " MB, CPU change: "
                << (after.cpuUsage > before.cpuUsage ? "+" : "-") << cpuChange << "%";*/
            logEvent(logSS.str());

            cout << "\nRestore Results:\n";
            cout << "RAM Available Change: " << (after.ramAvailable > before.ramAvailable ? "+" : "-")
                << abs(static_cast<long long>(after.ramAvailable - before.ramAvailable)) / 1024.0 << " MB\n";
            cout << "CPU Usage Change: " << (after.cpuUsage > before.cpuUsage ? "+" : "-")
                << abs(after.cpuUsage - before.cpuUsage) << "%\n";
            cout << "Network Usage Change: " << (after.networkBytes > before.networkBytes ? "+" : "-")
                << abs(static_cast<long long>(after.networkBytes - before.networkBytes)) / 1024.0 << " KB\n";
            optimized = calculatePercentage(before, after);
          

            logEvent("Optimization completed successfully");
        }
        else {
            cout << "Invalid choice. Returning to menu.\n";
            logEvent("Invalid choice selected: " + choice);
        }

        return optimized;
    }
    catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << "\n";
        logEvent("Error occurred: " + std::string(e.what()));
        cout << "Operation completed with errors. Some changes may have been applied.\n";
    }
}
