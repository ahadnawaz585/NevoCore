

#include "ProcessTweaks.h"
#include <tlhelp32.h>
#include <sstream>

ProcessTweaks::ProcessTweaks(SystemOptimizer& optimizer) : optimizer(optimizer) {}

TweakResult ProcessTweaks::applyBasicOptimizations(const std::string& grokResponse,
    const std::string& selfProcessName, const std::vector<std::string>& backgroundProcesses) {
    TweakResult result = { true, "", 0, 1 };
    DWORD currentPID = GetCurrentProcessId();

    std::vector<std::string> protectedProcesses = {
        selfProcessName, "System", "Secure System", "Registry", "smss.exe", "csrss.exe", "wininit.exe",
        "services.exe", "lsass.exe", "winlogon.exe", "svchost.exe", "dwm.exe", "steam.exe", "discord.exe"
    };
    std::vector<std::string> basicKillProcesses = { "msedge.exe", "firefox.exe", "explorer.exe" };

    std::vector<std::string> processesToKill;
    std::stringstream ss(grokResponse);
    std::string process;
    while (std::getline(ss, process, ',')) {
        process.erase(0, process.find_first_not_of(" \t"));
        process.erase(process.find_last_not_of(" \t") + 1);
        if (!process.empty()) processesToKill.push_back(optimizer.toLowerCase(process));
    }
    processesToKill.insert(processesToKill.end(), basicKillProcesses.begin(), basicKillProcesses.end());
    result.tweaksTotal = static_cast<int>(processesToKill.size());

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        result.success = false;
        result.message += "Failed to create process snapshot: Error " + std::to_string(GetLastError()) + "\n";
        return result;
    }

    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &pe32)) {
        result.success = false;
        result.message += "Failed to get first process: Error " + std::to_string(GetLastError()) + "\n";
        CloseHandle(hSnap);
        return result;
    }

    do {
        std::string procName = optimizer.wstringToString(pe32.szExeFile);
        std::string procLower = optimizer.toLowerCase(procName);
        bool isProtected = std::find(protectedProcesses.begin(), protectedProcesses.end(), procLower) != protectedProcesses.end();
        bool shouldKill = std::find(processesToKill.begin(), processesToKill.end(), procLower) != processesToKill.end() &&
            pe32.th32ProcessID != currentPID;

        if (!isProtected && shouldKill) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
            if (hProcess) {
                if (TerminateProcess(hProcess, 1)) {
                    result.tweaksApplied++;
                    result.message += "Terminated basic non-essential process: " + procName + "\n";
                }
                else {
                    result.success = false;
                    result.message += "Failed to terminate " + procName + ": Error " + std::to_string(GetLastError()) + "\n";
                }
                CloseHandle(hProcess);
            }
        }
    } while (Process32Next(hSnap, &pe32));

    CloseHandle(hSnap);
    return result;
}

TweakResult ProcessTweaks::applyAdvancedOptimizations(const std::string& grokResponse,
    const std::string& selfProcessName, const std::vector<std::string>& backgroundProcesses) {
    TweakResult r1 = applyBasicOptimizations(grokResponse, selfProcessName, backgroundProcesses);
    std::vector<std::string> advancedKillProcesses = { "OneDrive.exe", "Dropbox.exe" };
    r1.tweaksTotal += static_cast<int>(advancedKillProcesses.size());

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        r1.success = false;
        r1.message += "Failed to create process snapshot: Error " + std::to_string(GetLastError()) + "\n";
        return r1;
    }

    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &pe32)) {
        r1.success = false;
        r1.message += "Failed to get first process: Error " + std::to_string(GetLastError()) + "\n";
        CloseHandle(hSnap);
        return r1;
    }

    do {
        std::string procName = optimizer.wstringToString(pe32.szExeFile);
        std::string procLower = optimizer.toLowerCase(procName);
        if (std::find(advancedKillProcesses.begin(), advancedKillProcesses.end(), procLower) != advancedKillProcesses.end() &&
            pe32.th32ProcessID != GetCurrentProcessId()) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
            if (hProcess) {
                if (TerminateProcess(hProcess, 1)) {
                    r1.tweaksApplied++;
                    r1.message += "Terminated advanced non-essential process: " + procName + "\n";
                }
                CloseHandle(hProcess);
            }
        }
    } while (Process32Next(hSnap, &pe32));
    CloseHandle(hSnap);

    TweakResult r2 = applyIoCacheUpdateTweaks();
    TweakResult r3 = disableCpuBloat();
    TweakResult r4 = optimizeTaskScheduler();
    TweakResult r5 = disableTelemetry();
    TweakResult r6 = optimizeMemoryUsage();

    TweakResult result = { r1.success && r2.success && r3.success && r4.success && r5.success && r6.success,
                          r1.message + r2.message + r3.message + r4.message + r5.message + r6.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied +
                          r5.tweaksApplied + r6.tweaksApplied, 6 };
    return result;
}

TweakResult ProcessTweaks::applyExtremeOptimizations(const std::string& grokResponse,
    const std::string& selfProcessName, const std::vector<std::string>& backgroundProcesses) {
    TweakResult r1 = applyAdvancedOptimizations(grokResponse, selfProcessName, backgroundProcesses);
    std::vector<std::string> extremeKillProcesses = { "teams.exe", "slack.exe" };
    r1.tweaksTotal += static_cast<int>(extremeKillProcesses.size());

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        r1.success = false;
        r1.message += "Failed to create process snapshot: Error " + std::to_string(GetLastError()) + "\n";
        return r1;
    }

    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &pe32)) {
        r1.success = false;
        r1.message += "Failed to get first process: Error " + std::to_string(GetLastError()) + "\n";
        CloseHandle(hSnap);
        return r1;
    }

    do {
        std::string procName = optimizer.wstringToString(pe32.szExeFile);
        std::string procLower = optimizer.toLowerCase(procName);
        if (std::find(extremeKillProcesses.begin(), extremeKillProcesses.end(), procLower) != extremeKillProcesses.end() &&
            pe32.th32ProcessID != GetCurrentProcessId()) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
            if (hProcess) {
                if (TerminateProcess(hProcess, 1)) {
                    r1.tweaksApplied++;
                    r1.message += "Terminated extreme non-essential process: " + procName + "\n";
                }
                CloseHandle(hProcess);
            }
        }
    } while (Process32Next(hSnap, &pe32));
    CloseHandle(hSnap);

    TweakResult r2 = prioritizeGameProcesses();
    TweakResult r3 = optimizeDisks();
    TweakResult r4 = disableXboxApps();
    TweakResult r5 = disableAsusBloat();
    TweakResult r6 = adjustBaseOverTargetPriorities();
    TweakResult r7 = disableBackgroundServices();
    TweakResult r8 = boostForegroundApps();
    TweakResult r9 = disableCortana();
    TweakResult r10 = optimizeThreadScheduling();
    TweakResult r11 = disableWindowsSearch();
    TweakResult r12 = reduceProcessOverhead();
    TweakResult r13 = disableSuperfetchService();
    TweakResult r14 = optimizePagefileUsage();
    TweakResult r15 = disableWindowsUpdate();
    TweakResult r16 = enhanceProcessAffinity();

    TweakResult result = { r1.success && r2.success && r3.success && r4.success && r5.success &&
                          r6.success && r7.success && r8.success && r9.success && r10.success &&
                          r11.success && r12.success && r13.success && r14.success && r15.success &&
                          r16.success,
                          r1.message + r2.message + r3.message + r4.message + r5.message +
                          r6.message + r7.message + r8.message + r9.message + r10.message +
                          r11.message + r12.message + r13.message + r14.message + r15.message +
                          r16.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied +
                          r5.tweaksApplied + r6.tweaksApplied + r7.tweaksApplied + r8.tweaksApplied +
                          r9.tweaksApplied + r10.tweaksApplied + r11.tweaksApplied + r12.tweaksApplied +
                          r13.tweaksApplied + r14.tweaksApplied + r15.tweaksApplied + r16.tweaksApplied,
                          16 };
    return result;
}

TweakResult ProcessTweaks::prioritizeGameProcesses() {
    TweakResult result = { true, "", 0, 5 }; // Assuming up to 5 game processes
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        result.success = false;
        result.message += "Failed to create snapshot: Error " + std::to_string(GetLastError()) + "\n";
        return result;
    }

    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &pe32)) {
        result.success = false;
        result.message += "Failed to get first process: Error " + std::to_string(GetLastError()) + "\n";
        CloseHandle(hSnap);
        return result;
    }

    do {
        if (optimizer.isGameProcess(pe32.szExeFile)) {
            HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pe32.th32ProcessID);
            if (hProcess) {
                if (SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS)) {
                    result.tweaksApplied++;
                    result.message += "Prioritized game process: " + optimizer.wstringToString(pe32.szExeFile) + "\n";
                }
                CloseHandle(hProcess);
            }
        }
    } while (Process32Next(hSnap, &pe32));

    CloseHandle(hSnap);
    return result;
}

TweakResult ProcessTweaks::applyIoCacheUpdateTweaks() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // Disable LargeSystemCache for gaming
        regResult = RegSetValueExA(hKey, "LargeSystemCache", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Applied I/O cache tweaks.\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult ProcessTweaks::optimizeDisks() {
    TweakResult result = { true, "", 0, 1 };
    if (system("fsutil behavior set disabledeletenotify 0") == 0) {
        result.tweaksApplied++;
        result.message += "Optimized disk settings.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to optimize disks.\n";
    }
    return result;
}

TweakResult ProcessTweaks::disableCpuBloat() {
    TweakResult result = { true, "", 0, 1 };
    std::vector<std::string> cpuBloatProcesses = { "backgroundTaskHost.exe", "ctfmon.exe" };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        result.success = false;
        result.message += "Failed to create snapshot: Error " + std::to_string(GetLastError()) + "\n";
        return result;
    }
    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &pe32)) {
        CloseHandle(hSnap);
        result.success = false;
        result.message += "Failed to get first process.\n";
        return result;
    }
    do {
        std::string procName = optimizer.toLowerCase(optimizer.wstringToString(pe32.szExeFile));
        if (std::find(cpuBloatProcesses.begin(), cpuBloatProcesses.end(), procName) != cpuBloatProcesses.end() &&
            pe32.th32ProcessID != GetCurrentProcessId()) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
            if (hProcess && TerminateProcess(hProcess, 1)) {
                result.tweaksApplied++;
                result.message += "Terminated CPU bloat process: " + procName + "\n";
            }
            CloseHandle(hProcess);
        }
    } while (Process32Next(hSnap, &pe32));
    CloseHandle(hSnap);
    return result;
}

TweakResult ProcessTweaks::disableXboxApps() {
    TweakResult result = { true, "", 0, 1 };
    std::vector<std::string> xboxProcesses = { "xboxapp.exe", "xboxgipsvc.exe" };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        result.success = false;
        result.message += "Failed to create snapshot: Error " + std::to_string(GetLastError()) + "\n";
        return result;
    }
    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &pe32)) {
        CloseHandle(hSnap);
        result.success = false;
        result.message += "Failed to get first process.\n";
        return result;
    }
    do {
        std::string procName = optimizer.toLowerCase(optimizer.wstringToString(pe32.szExeFile));
        if (std::find(xboxProcesses.begin(), xboxProcesses.end(), procName) != xboxProcesses.end() &&
            pe32.th32ProcessID != GetCurrentProcessId()) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
            if (hProcess && TerminateProcess(hProcess, 1)) {
                result.tweaksApplied++;
                result.message += "Terminated Xbox process: " + procName + "\n";
            }
            CloseHandle(hProcess);
        }
    } while (Process32Next(hSnap, &pe32));
    CloseHandle(hSnap);
    return result;
}

TweakResult ProcessTweaks::disableAsusBloat() {
    TweakResult result = { true, "", 0, 1 };
    std::vector<std::string> asusProcesses = { "asuscomsvc.exe", "asusmainsvc.exe" };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        result.success = false;
        result.message += "Failed to create snapshot: Error " + std::to_string(GetLastError()) + "\n";
        return result;
    }
    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &pe32)) {
        CloseHandle(hSnap);
        result.success = false;
        result.message += "Failed to get first process.\n";
        return result;
    }
    do {
        std::string procName = optimizer.toLowerCase(optimizer.wstringToString(pe32.szExeFile));
        if (std::find(asusProcesses.begin(), asusProcesses.end(), procName) != asusProcesses.end() &&
            pe32.th32ProcessID != GetCurrentProcessId()) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
            if (hProcess && TerminateProcess(hProcess, 1)) {
                result.tweaksApplied++;
                result.message += "Terminated Asus bloat process: " + procName + "\n";
            }
            CloseHandle(hProcess);
        }
    } while (Process32Next(hSnap, &pe32));
    CloseHandle(hSnap);
    return result;
}

TweakResult ProcessTweaks::optimizeTaskScheduler() {
    TweakResult result = { true, "", 0, 1 };
    if (system("schtasks /change /tn \"\\Microsoft\\Windows\\Maintenance\\WinSAT\" /disable") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled unnecessary scheduled tasks.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable scheduled tasks.\n";
    }
    return result;
}

TweakResult ProcessTweaks::adjustBaseOverTargetPriorities() {
    TweakResult result = { true, "", 0, 1 };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        result.success = false;
        result.message += "Failed to create snapshot: Error " + std::to_string(GetLastError()) + "\n";
        return result;
    }
    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &pe32)) {
        result.success = false;
        result.message += "Failed to get first process: Error " + std::to_string(GetLastError()) + "\n";
        CloseHandle(hSnap);
        return result;
    }
    do {
        if (optimizer.isGameProcess(pe32.szExeFile)) {
            HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pe32.th32ProcessID);
            if (hProcess) {
                if (SetPriorityClass(hProcess, ABOVE_NORMAL_PRIORITY_CLASS)) {
                    result.tweaksApplied++;
                    result.message += "Adjusted priority for: " + optimizer.wstringToString(pe32.szExeFile) + "\n";
                }
                CloseHandle(hProcess);
            }
        }
    } while (Process32Next(hSnap, &pe32));
    CloseHandle(hSnap);
    return result;
}

// New Tweaks
TweakResult ProcessTweaks::disableTelemetry() {
    TweakResult result = { true, "", 0, 1 };
    std::vector<std::string> telemetryProcesses = { "diagtrack.exe", "compatelrunner.exe" };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        result.success = false;
        result.message += "Failed to create snapshot: Error " + std::to_string(GetLastError()) + "\n";
        return result;
    }
    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &pe32)) {
        CloseHandle(hSnap);
        return result;
    }
    do {
        std::string procName = optimizer.toLowerCase(optimizer.wstringToString(pe32.szExeFile));
        if (std::find(telemetryProcesses.begin(), telemetryProcesses.end(), procName) != telemetryProcesses.end()) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
            if (hProcess && TerminateProcess(hProcess, 1)) {
                result.tweaksApplied++;
                result.message += "Terminated telemetry process: " + procName + "\n";
            }
            CloseHandle(hProcess);
        }
    } while (Process32Next(hSnap, &pe32));
    CloseHandle(hSnap);
    return result;
}

TweakResult ProcessTweaks::optimizeMemoryUsage() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1; // Enable memory compression
        regResult = RegSetValueExA(hKey, "EnableCompression", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Optimized memory usage settings.\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult ProcessTweaks::disableBackgroundServices() {
    TweakResult result = { true, "", 0, 1 };
    std::vector<std::string> services = { "wuauserv", "bits" };
    for (const auto& service : services) {
        if (system(("sc stop " + service).c_str()) == 0) {
            result.tweaksApplied++;
            result.message += "Stopped background service: " + service + "\n";
        }
    }
    return result;
}

TweakResult ProcessTweaks::boostForegroundApps() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\PriorityControl",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0x18; // Boost foreground priority
        regResult = RegSetValueExA(hKey, "Win32PrioritySeparation", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Boosted foreground application priority.\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult ProcessTweaks::disableCortana() {
    TweakResult result = { true, "", 0, 1 };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        result.success = false;
        result.message += "Failed to create snapshot: Error " + std::to_string(GetLastError()) + "\n";
        return result;
    }
    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &pe32)) {
        CloseHandle(hSnap);
        return result;
    }
    do {
        std::string procName = optimizer.toLowerCase(optimizer.wstringToString(pe32.szExeFile));
        if (procName.find("cortana") != std::string::npos) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
            if (hProcess && TerminateProcess(hProcess, 1)) {
                result.tweaksApplied++;
                result.message += "Terminated Cortana process: " + procName + "\n";
            }
            CloseHandle(hProcess);
        }
    } while (Process32Next(hSnap, &pe32));
    CloseHandle(hSnap);
    return result;
}

TweakResult ProcessTweaks::optimizeThreadScheduling() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\kernel",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // Optimize for short variable threads
        regResult = RegSetValueExA(hKey, "ThreadScheduling", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Optimized thread scheduling.\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult ProcessTweaks::disableWindowsSearch() {
    TweakResult result = { true, "", 0, 1 };
    if (system("sc config WSearch start= disabled") == 0 && system("sc stop WSearch") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled Windows Search indexing.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable Windows Search.\n";
    }
    return result;
}

TweakResult ProcessTweaks::reduceProcessOverhead() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\Session Manager",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1; // Reduce process overhead
        regResult = RegSetValueExA(hKey, "OptimizeProcessOverhead", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Reduced process overhead.\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult ProcessTweaks::disableSuperfetchService() {
    TweakResult result = { true, "", 0, 1 };
    if (system("sc config SysMain start= disabled") == 0 && system("sc stop SysMain") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled Superfetch service.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable Superfetch.\n";
    }
    return result;
}

TweakResult ProcessTweaks::optimizePagefileUsage() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // System-managed pagefile
        regResult = RegSetValueExA(hKey, "PagingFiles", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Optimized pagefile usage.\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult ProcessTweaks::disableWindowsUpdate() {
    TweakResult result = { true, "", 0, 1 };
    if (system("sc config wuauserv start= disabled") == 0 && system("sc stop wuauserv") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled Windows Update service temporarily.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable Windows Update.\n";
    }
    return result;
}

TweakResult ProcessTweaks::enhanceProcessAffinity() {
    TweakResult result = { true, "", 0, 5 }; // Assuming up to 5 processes
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        result.success = false;
        result.message += "Failed to create snapshot: Error " + std::to_string(GetLastError()) + "\n";
        return result;
    }
    PROCESSENTRY32 pe32{};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hSnap, &pe32)) {
        CloseHandle(hSnap);
        return result;
    }
    do {
        if (optimizer.isGameProcess(pe32.szExeFile)) {
            HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pe32.th32ProcessID);
            if (hProcess) {
                DWORD_PTR affinityMask = 0xF; // Use first 4 cores
                if (SetProcessAffinityMask(hProcess, affinityMask)) {
                    result.tweaksApplied++;
                    result.message += "Enhanced affinity for: " + optimizer.wstringToString(pe32.szExeFile) + "\n";
                }
                CloseHandle(hProcess);
            }
        }
    } while (Process32Next(hSnap, &pe32));
    CloseHandle(hSnap);
    return result;
}
