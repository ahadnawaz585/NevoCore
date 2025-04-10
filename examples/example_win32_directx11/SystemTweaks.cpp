#include "SystemTweaks.h"
#include <spdlog/spdlog.h>

SystemTweaks::SystemTweaks(SystemOptimizer& optimizer) : parent(optimizer) {}

// Basic Tweaks
TweakResult SystemTweaks::applyPowerSettings() {
    TweakResult result = { true, "", 0, 1 };
    if (system("powercfg /setactive 8c5e7fda-e8bf-4a96-9a85-a6e23a8c635c") == 0) {
        result.tweaksApplied++;
        result.message += "Set power plan to High Performance (Stix Powerplan).\n";
    }
    else {
        result.success = false;
        result.message += "Failed to set power plan to High Performance.\n";
    }
    return result;
}

TweakResult SystemTweaks::manageInterruptSteering() {
    TweakResult result = { true, "", 0, 1 };
    if (system("powershell -Command \"Set-NetAdapterRss -Name * -Enabled $true\"") == 0) {
        result.tweaksApplied++;
        result.message += "Enabled RSS for interrupt steering across CPU cores.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to enable RSS for interrupt steering.\n";
    }
    return result;
}

// Advanced Tweaks
TweakResult SystemTweaks::disableHpet() {
    TweakResult result = { true, "", 0, 1 };
    if (system("bcdedit /set useplatformclock false") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled High Precision Event Timer (HPET) for lower latency.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable HPET.\n";
    }
    return result;
}

TweakResult SystemTweaks::optimizeFullscreen() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "System\\GameConfigStore", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
        if (regResult == ERROR_FILE_NOT_FOUND) {
            RegCreateKeyA(HKEY_CURRENT_USER, "System\\GameConfigStore", &hKey);
        }
        DWORD value = 0; // Enable fullscreen optimizations
        regResult = RegSetValueExA(hKey, "GameDVR_FSEBehavior", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Enabled fullscreen optimizations for games.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to enable fullscreen optimizations: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GameConfigStore key: Error " + std::to_string(regResult) + "\n";
    }
    return result;
}

// Extreme Tweaks
TweakResult SystemTweaks::applyMmcssTweaks() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile\\Tasks\\Games", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0x2; // High priority for games
        regResult = RegSetValueExA(hKey, "Priority", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Applied MMCSS tweaks to prioritize game scheduling.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to set MMCSS priority: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open MMCSS Games key: Error " + std::to_string(regResult) + "\n";
    }
    return result;
}

// Existing Tweaks
TweakResult SystemTweaks::disableBluetooth() {
    TweakResult result = { true, "", 0, 1 };
    if (system("sc config bthserv start= disabled") == 0 && system("sc config bthenum start= disabled") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled Bluetooth services.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable Bluetooth.\n";
    }
    return result;
}

TweakResult SystemTweaks::disableHyperV() {
    TweakResult result = { true, "", 0, 1 };
    if (system("dism /online /disable-feature /featurename:Microsoft-Hyper-V-All /quiet /norestart") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled Hyper-V.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable Hyper-V.\n";
    }
    return result;
}

TweakResult SystemTweaks::disableUselessServices() {
    TweakResult result = { true, "", 0, 1 };
    std::vector<std::string> services = { "dmwappushservice", "MapsBroker" };
    for (const auto& svc : services) {
        if (system(("sc config " + svc + " start= disabled").c_str()) != 0) {
            result.success = false;
            result.message += "Failed to disable service: " + svc + "\n";
        }
    }
    if (result.success) {
        result.tweaksApplied++;
        result.message += "Disabled useless services.\n";
    }
    return result;
}

TweakResult SystemTweaks::disableUselessFeatures() {
    TweakResult result = { true, "", 0, 1 };
    if (system("dism /online /disable-feature /featurename:WindowsMediaPlayer /quiet /norestart") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled useless Windows features.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable features.\n";
    }
    return result;
}

TweakResult SystemTweaks::disableLegacySupport() {
    TweakResult result = { true, "", 0, 1 };
    if (system("bcdedit /set disabledynamictick yes") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled dynamic ticks to support BIOS legacy disable.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable dynamic ticks.\n";
    }
    return result;
}

TweakResult SystemTweaks::adjustAudioLatency() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Audio", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1; // Disable enhancements
        RegSetValueExA(hKey, "DisableAudioEnhancements", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        result.tweaksApplied++;
        result.message += "Adjusted audio latency.\n";
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::disableSleepStudy() {
    TweakResult result = { true, "", 0, 1 };
    if (system("powercfg /sleepstudy /disable") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled sleep study.\n";
    }
    return result;
}

TweakResult SystemTweaks::applyTimerRes() {
    TweakResult result = { true, "", 0, 1 };
    if (system("bcdedit /set timerresolution 5000") == 0) { // 0.5ms resolution
        result.tweaksApplied++;
        result.message += "Applied timer resolution tweak.\n";
    }
    return result;
}

TweakResult SystemTweaks::disableXhciInterruptModeration() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Enum\\USB", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // Disable moderation
        RegSetValueExA(hKey, "InterruptModeration", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        result.tweaksApplied++;
        result.message += "Disabled xHCI interrupt moderation (Intel only).\n";
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::optimizeAmdCpu() {
    TweakResult result = { true, "", 0, 1 };
    if (system("bcdedit /set tscsyncpolicy Enhanced") == 0) { // AMD-specific tweak
        result.tweaksApplied++;
        result.message += "Optimized AMD CPU settings.\n";
    }
    return result;
}

TweakResult SystemTweaks::optimizeMsi() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1; // Enable MSI mode
        RegSetValueExA(hKey, "EnableMSI", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        result.tweaksApplied++;
        result.message += "Optimized MSI interrupts.\n";
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::configureBcdEdit() {
    TweakResult result = { true, "", 0, 1 };
    if (system("bcdedit /set disabledynamictick yes") == 0 &&
        system("bcdedit /set useplatformtick yes") == 0) {
        result.tweaksApplied++;
        result.message += "Configured BCDEdit for performance.\n";
    }
    return result;
}

TweakResult SystemTweaks::disableHibernation() {
    TweakResult result = { true, "", 0, 1 };
    if (system("powercfg /hibernate off") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled hibernation.\n";
    }
    return result;
}

TweakResult SystemTweaks::disableDriverUsbPowersavings() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Power", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // Disable power savings
        RegSetValueExA(hKey, "CsEnabled", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        result.tweaksApplied++;
        result.message += "Disabled driver and USB power savings.\n";
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::optimizeCpu() {
    TweakResult result = { true, "", 0, 1 };
    if (system("powercfg -setacvalueindex SCHEME_MIN SUB_PROCESSOR PROCTHROTTLEMIN 100") == 0) {
        result.tweaksApplied++;
        result.message += "Optimized CPU by disabling throttling.\n";
    }
    return result;
}

TweakResult SystemTweaks::optimizeCpuBoostState() {
    TweakResult result = { true, "", 0, 1 };
    if (system("powercfg -setacvalueindex SCHEME_MIN SUB_PROCESSOR PERFBOOSTMODE 2") == 0) { // Aggressive boost
        result.tweaksApplied++;
        result.message += "Optimized CPU boost state.\n";
    }
    return result;
}

// New Tweaks
TweakResult SystemTweaks::enableMemoryCompression() {
    TweakResult result = { true, "", 0, 1 };
    if (system("powershell -Command \"Enable-MMAgent -MemoryCompression\"") == 0) {
        result.tweaksApplied++;
        result.message += "Enabled memory compression for better memory usage.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to enable memory compression.\n";
    }
    return result;
}

TweakResult SystemTweaks::optimizeNetworkThrottling() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0xFFFFFFFF; // Disable throttling
        regResult = RegSetValueExA(hKey, "NetworkThrottlingIndex", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Optimized network throttling.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to optimize network throttling: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::disableSuperfetch() {
    TweakResult result = { true, "", 0, 1 };
    if (system("sc config SysMain start= disabled") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled Superfetch/Prefetch service.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable Superfetch.\n";
    }
    return result;
}

TweakResult SystemTweaks::optimizePageFile() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // System-managed page file
        regResult = RegSetValueExA(hKey, "PagingFiles", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Optimized page file settings.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to optimize page file: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::disableWindowsDefender() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows Defender", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
        if (regResult == ERROR_FILE_NOT_FOUND) {
            RegCreateKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows Defender", &hKey);
        }
        DWORD value = 1; // Disable Defender
        regResult = RegSetValueExA(hKey, "DisableAntiSpyware", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled Windows Defender.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to disable Windows Defender: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::optimizeThreadPriority() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\PriorityControl", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0x18; // Boost foreground app priority
        regResult = RegSetValueExA(hKey, "Win32PrioritySeparation", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Optimized thread priority for foreground apps.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to optimize thread priority: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::disableBackgroundApps() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\BackgroundAccessApplications", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // Disable background apps
        regResult = RegSetValueExA(hKey, "GlobalUserDisabled", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled background applications.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to disable background apps: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::optimizeStorage() {
    TweakResult result = { true, "", 0, 1 };
    if (system("fsutil behavior set disabledeletenotify 0") == 0) { // Enable TRIM for SSD
        result.tweaksApplied++;
        result.message += "Optimized storage settings for SSD.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to optimize storage.\n";
    }
    return result;
}

TweakResult SystemTweaks::disableMitigations() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 2; // Disable mitigations
        regResult = RegSetValueExA(hKey, "FeatureSettingsOverride", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled CPU vulnerability mitigations.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to disable mitigations: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

// Restore Methods
TweakResult SystemTweaks::restorePowerSettings() {
    TweakResult result = { true, "", 0, 1 };
    if (system("powercfg /setactive 381b4222-f694-41f0-9685-ff5bb260df2e") == 0) {
        result.tweaksApplied++;
        result.message += "Restored Balanced power plan.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to restore Balanced power plan.\n";
    }
    return result;
}

TweakResult SystemTweaks::restoreInterruptSteering() {
    TweakResult result = { true, "", 0, 1 };
    if (system("powershell -Command \"Set-NetAdapterRss -Name * -Enabled $false\"") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled RSS to restore default interrupt steering.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable RSS.\n";
    }
    return result;
}

TweakResult SystemTweaks::restoreHpet() {
    TweakResult result = { true, "", 0, 1 };
    if (system("bcdedit /deletevalue useplatformclock") == 0) {
        result.tweaksApplied++;
        result.message += "Restored default HPET settings.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to restore HPET.\n";
    }
    return result;
}

TweakResult SystemTweaks::restoreFullscreen() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "System\\GameConfigStore", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "GameDVR_FSEBehavior");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored default fullscreen optimization settings.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to restore fullscreen optimizations: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::restoreMmcssTweaks() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile\\Tasks\\Games", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "Priority");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored default MMCSS settings for games.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to restore MMCSS priority: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::restoreMemoryCompression() {
    TweakResult result = { true, "", 0, 1 };
    if (system("powershell -Command \"Disable-MMAgent -MemoryCompression\"") == 0) {
        result.tweaksApplied++;
        result.message += "Restored default memory compression settings.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to restore memory compression.\n";
    }
    return result;
}

TweakResult SystemTweaks::restoreNetworkThrottling() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "NetworkThrottlingIndex");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored default network throttling settings.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to restore network throttling: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::restoreSuperfetch() {
    TweakResult result = { true, "", 0, 1 };
    if (system("sc config SysMain start= auto") == 0) {
        result.tweaksApplied++;
        result.message += "Restored Superfetch/Prefetch service.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to restore Superfetch.\n";
    }
    return result;
}

TweakResult SystemTweaks::restorePageFile() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "PagingFiles");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored default page file settings.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to restore page file: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::restoreWindowsDefender() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows Defender", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "DisableAntiSpyware");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored Windows Defender settings.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to restore Windows Defender: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::restoreThreadPriority() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\PriorityControl", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "Win32PrioritySeparation");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored default thread priority settings.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to restore thread priority: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::restoreBackgroundApps() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\BackgroundAccessApplications", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "GlobalUserDisabled");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored background applications settings.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to restore background apps: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult SystemTweaks::restoreStorage() {
    TweakResult result = { true, "", 0, 1 };
    if (system("fsutil behavior set disabledeletenotify 1") == 0) {
        result.tweaksApplied++;
        result.message += "Restored default storage settings.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to restore storage settings.\n";
    }
    return result;
}

TweakResult SystemTweaks::restoreMitigations() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "FeatureSettingsOverride");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored CPU vulnerability mitigations.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to restore mitigations: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

// Optimization Levels
TweakResult SystemTweaks::applyBasicOptimizations() {
    TweakResult r1 = applyPowerSettings();
    TweakResult result = { r1.success, r1.message, r1.tweaksApplied, 1 };
    return result;
}

TweakResult SystemTweaks::applyAdvancedOptimizations() {
    TweakResult r1 = applyPowerSettings();
    TweakResult r2 = disableHpet();
    TweakResult r3 = optimizeFullscreen();
    TweakResult r4 = disableBluetooth();
    TweakResult r5 = disableLegacySupport();
    TweakResult r6 = adjustAudioLatency();
    TweakResult r7 = configureBcdEdit();
    TweakResult r8 = disableHibernation();
    TweakResult r9 = enableMemoryCompression();
    TweakResult r10 = optimizeNetworkThrottling();
    TweakResult result = { r1.success && r2.success && r3.success && r4.success && r5.success &&
                          r6.success && r7.success && r8.success && r9.success && r10.success,
                          r1.message + r2.message + r3.message + r4.message + r5.message +
                          r6.message + r7.message + r8.message + r9.message + r10.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied +
                          r5.tweaksApplied + r6.tweaksApplied + r7.tweaksApplied + r8.tweaksApplied +
                          r9.tweaksApplied + r10.tweaksApplied, 10 };
    return result;
}

TweakResult SystemTweaks::applyExtremeOptimizations() {
    TweakResult r1 = applyPowerSettings();
    TweakResult r2 = manageInterruptSteering();
    TweakResult r3 = disableHpet();
    TweakResult r4 = optimizeFullscreen();
    TweakResult r5 = applyMmcssTweaks();
    TweakResult r6 = { true, "", 0, 2 };
    parent.disableCoreParking(); r6.tweaksApplied++; r6.message += "Disabled CPU core parking\n";
    parent.enableGameMode(); r6.tweaksApplied++; r6.message += "Enabled Windows Game Mode\n";
    TweakResult r7 = disableBluetooth();
    TweakResult r8 = disableHyperV();
    TweakResult r9 = disableUselessServices();
    TweakResult r10 = disableUselessFeatures();
    TweakResult r11 = disableLegacySupport();
    TweakResult r12 = adjustAudioLatency();
    TweakResult r13 = disableSleepStudy();
    TweakResult r14 = applyTimerRes();
    TweakResult r15 = disableXhciInterruptModeration();
    TweakResult r16 = optimizeAmdCpu();
    TweakResult r17 = optimizeMsi();
    TweakResult r18 = configureBcdEdit();
    TweakResult r19 = disableHibernation();
    TweakResult r20 = disableDriverUsbPowersavings();
    TweakResult r21 = optimizeCpu();
    TweakResult r22 = optimizeCpuBoostState();
    TweakResult r23 = enableMemoryCompression();
    TweakResult r24 = optimizeNetworkThrottling();
    TweakResult r25 = disableSuperfetch();
    TweakResult r26 = optimizePageFile();
    TweakResult r27 = disableWindowsDefender();
    TweakResult r28 = optimizeThreadPriority();
    TweakResult r29 = disableBackgroundApps();
    TweakResult r30 = optimizeStorage();
    TweakResult r31 = disableMitigations();
    TweakResult result = { r1.success && r2.success && r3.success && r4.success && r5.success &&
                          r6.success && r7.success && r8.success && r9.success && r10.success &&
                          r11.success && r12.success && r13.success && r14.success && r15.success &&
                          r16.success && r17.success && r18.success && r19.success && r20.success &&
                          r21.success && r22.success && r23.success && r24.success && r25.success &&
                          r26.success && r27.success && r28.success && r29.success && r30.success &&
                          r31.success,
                          r1.message + r2.message + r3.message + r4.message + r5.message +
                          r6.message + r7.message + r8.message + r9.message + r10.message +
                          r11.message + r12.message + r13.message + r14.message + r15.message +
                          r16.message + r17.message + r18.message + r19.message + r20.message +
                          r21.message + r22.message + r23.message + r24.message + r25.message +
                          r26.message + r27.message + r28.message + r29.message + r30.message +
                          r31.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied +
                          r5.tweaksApplied + r6.tweaksApplied + r7.tweaksApplied + r8.tweaksApplied +
                          r9.tweaksApplied + r10.tweaksApplied + r11.tweaksApplied + r12.tweaksApplied +
                          r13.tweaksApplied + r14.tweaksApplied + r15.tweaksApplied + r16.tweaksApplied +
                          r17.tweaksApplied + r18.tweaksApplied + r19.tweaksApplied + r20.tweaksApplied +
                          r21.tweaksApplied + r22.tweaksApplied + r23.tweaksApplied + r24.tweaksApplied +
                          r25.tweaksApplied + r26.tweaksApplied + r27.tweaksApplied + r28.tweaksApplied +
                          r29.tweaksApplied + r30.tweaksApplied + r31.tweaksApplied, 31 };
    return result;
}

TweakResult SystemTweaks::restoreDefaults() {
    TweakResult r1 = restorePowerSettings();
    TweakResult r2 = restoreInterruptSteering();
    TweakResult r3 = restoreHpet();
    TweakResult r4 = restoreFullscreen();
    TweakResult r5 = restoreMmcssTweaks();
    TweakResult r6 = restoreMemoryCompression();
    TweakResult r7 = restoreNetworkThrottling();
    TweakResult r8 = restoreSuperfetch();
    TweakResult r9 = restorePageFile();
    TweakResult r10 = restoreWindowsDefender();
    TweakResult r11 = restoreThreadPriority();
    TweakResult r12 = restoreBackgroundApps();
    TweakResult r13 = restoreStorage();
    TweakResult r14 = restoreMitigations();
    TweakResult result = { r1.success && r2.success && r3.success && r4.success && r5.success &&
                          r6.success && r7.success && r8.success && r9.success && r10.success &&
                          r11.success && r12.success && r13.success && r14.success,
                          r1.message + r2.message + r3.message + r4.message + r5.message +
                          r6.message + r7.message + r8.message + r9.message + r10.message +
                          r11.message + r12.message + r13.message + r14.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied +
                          r5.tweaksApplied + r6.tweaksApplied + r7.tweaksApplied + r8.tweaksApplied +
                          r9.tweaksApplied + r10.tweaksApplied + r11.tweaksApplied + r12.tweaksApplied +
                          r13.tweaksApplied + r14.tweaksApplied, 14 };
    return result;
}
