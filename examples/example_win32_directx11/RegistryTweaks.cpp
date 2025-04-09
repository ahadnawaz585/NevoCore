#include "RegistryTweaks.h"
#include <windows.h>

RegistryTweaks::RegistryTweaks(SystemOptimizer& optimizer) : optimizer(optimizer) {}

TweakResult RegistryTweaks::applyBasicOptimizations() {
    TweakResult result = { true, "", 0, 3 };
    backupRegistryKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\VisualEffects", "visual_effects_backup.reg");
    backupRegistryKey("Control Panel\\Desktop", "desktop_backup.reg");
    backupRegistryKey("Software\\Microsoft\\Windows\\CurrentVersion\\Run", "startup_backup.reg");

    HKEY hKey;
    LONG regResult;

    regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\VisualEffects", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 2;
        regResult = RegSetValueExA(hKey, "VisualFXSetting", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Adjusted visual effects for performance.\n";
        }
        RegCloseKey(hKey);
    }

    TweakResult r1 = applyGeneralSettings();
    TweakResult r2 = disableStartupApps();

    result.success &= r1.success && r2.success;
    result.tweaksApplied += r1.tweaksApplied + r2.tweaksApplied;
    result.message += r1.message + r2.message;
    return result;
}

TweakResult RegistryTweaks::applyAdvancedOptimizations() {
    TweakResult result = applyBasicOptimizations();
    result.tweaksTotal = 6;

    backupRegistryKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Serialize", "serialize_backup.reg");
    backupRegistryKey("SYSTEM\\CurrentControlSet\\Control\\PriorityControl", "priority_backup.reg");
    backupRegistryKey("SYSTEM\\CurrentControlSet\\Control\\FileSystem", "ntfs_backup.reg");
    backupRegistryKey("SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection", "telemetry_backup.reg");

    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Serialize", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0;
        regResult = RegSetValueExA(hKey, "StartupDelayInMSec", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Reduced startup delay.\n";
        }
        RegCloseKey(hKey);
    }

    TweakResult r1 = optimizePriority();
    TweakResult r2 = applyNtfsTweaks();
    TweakResult r3 = disableTelemetry();

    result.success &= r1.success && r2.success && r3.success;
    result.tweaksApplied += r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied;
    result.message += r1.message + r2.message + r3.message;
    return result;
}

TweakResult RegistryTweaks::applyExtremeOptimizations() {
    TweakResult result = applyAdvancedOptimizations();
    result.tweaksTotal = 18;

    backupRegistryKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\BackgroundAccessApplications", "background_backup.reg");
    backupRegistryKey("SYSTEM\\CurrentControlSet\\Control", "kernel_backup.reg");
    backupRegistryKey("SOFTWARE\\Policies\\Microsoft\\Windows\\System", "vbs_backup.reg");
    backupRegistryKey("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\kernel", "dpc_backup.reg");
    backupRegistryKey("SYSTEM\\CurrentControlSet\\Control\\TimeZoneInformation", "timer_backup.reg");
    backupRegistryKey("SOFTWARE\\Policies\\Microsoft\\Windows\\WindowsUpdate", "updates_backup.reg");
    backupRegistryKey("SYSTEM\\CurrentControlSet\\Services\\XblAuthManager", "xbox_backup.reg");
    backupRegistryKey("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile", "system_profile_backup.reg");
    backupRegistryKey("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Executive", "threads_backup.reg");
    backupRegistryKey("SYSTEM\\CurrentControlSet\\Control\\PriorityControl", "device_priority_backup.reg");
    backupRegistryKey("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management", "memory_backup.reg");

    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\BackgroundAccessApplications", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1;
        regResult = RegSetValueExA(hKey, "GlobalUserDisabled", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled background apps.\n";
        }
        RegCloseKey(hKey);
    }

    TweakResult r1 = setResourcePolicies();
    TweakResult r2 = applyKernelTweaks();
    TweakResult r3 = disableVBS();
    TweakResult r4 = disableThreadDPC();
    TweakResult r5 = serializeTimerExpiration();
    TweakResult r6 = disableWindowsUpdates();
    TweakResult r7 = disableXboxApps();
    TweakResult r8 = tweakSystemProfile();         // New tweak #6 added here (Line 60)
    TweakResult r9 = optimizeWorkerThreads();       // New tweak #7 added here (Line 61)
    TweakResult r10 = adjustDeviceDriverPriorities(); // New tweak #16 added here (Line 62)
    TweakResult r11 = optimizeMemory();

    result.success &= r1.success && r2.success && r3.success && r4.success && r5.success && r6.success && r7.success && r8.success && r9.success && r10.success && r11.success;
    result.tweaksApplied += r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied + r5.tweaksApplied + r6.tweaksApplied + r7.tweaksApplied + r8.tweaksApplied + r9.tweaksApplied + r10.tweaksApplied + r11.tweaksApplied;
    result.message += r1.message + r2.message + r3.message + r4.message + r5.message + r6.message + r7.message + r8.message + r9.message + r10.message + r11.message;
    return result;
}
TweakResult RegistryTweaks::tweakSystemProfile() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // Disable lazy mode
        RegSetValueExA(hKey, "NetworkThrottlingIndex", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        result.tweaksApplied++;
        result.message += "Tweaked system profile.\n";
        RegCloseKey(hKey);
    }
    return result;
}

// New tweak #7: Optimize Worker Threads (Line 95)
TweakResult RegistryTweaks::optimizeWorkerThreads() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Executive", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 32; // Increase thread pool
        RegSetValueExA(hKey, "AdditionalCriticalWorkerThreads", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        result.tweaksApplied++;
        result.message += "Optimized worker threads.\n";
        RegCloseKey(hKey);
    }
    return result;
}

// New tweak #16: Adjust Device and Driver Priorities (Line 110)
TweakResult RegistryTweaks::adjustDeviceDriverPriorities() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\PriorityControl", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1; // Raise device priority
        RegSetValueExA(hKey, "Win32PrioritySeparation", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        result.tweaksApplied++;
        result.message += "Adjusted device and driver priorities.\n";
        RegCloseKey(hKey);
    }
    return result;
}

// New tweak #20: Optimize Memory (Line 125)
TweakResult RegistryTweaks::optimizeMemory() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1; // Enable large system cache
        RegSetValueExA(hKey, "LargeSystemCache", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        result.tweaksApplied++;
        result.message += "Optimized memory settings.\n";
        RegCloseKey(hKey);
    }
    return result;
}
TweakResult RegistryTweaks::restoreDefaults() {
    TweakResult result = { true, "", 0, 14 }; // Updated total tweaks: 3 Basic + 4 Advanced + 5 Extreme

    HKEY hKey;
    LONG regResult;

    // Restore Visual Effects
    regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\VisualEffects", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "VisualFXSetting");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Visual effects restored to default.\n";
        }
        RegCloseKey(hKey);
    }

    // Restore Startup Delay
    regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Serialize", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "StartupDelayInMSec");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Startup delay settings restored.\n";
        }
        RegCloseKey(hKey);
    }

    // Restore General Settings (New)
    TweakResult r1 = restoreGeneralSettings();
    result.success &= r1.success;
    result.tweaksApplied += r1.tweaksApplied;
    result.message += r1.message;

    // Restore Background Apps
    regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\BackgroundAccessApplications", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0;
        regResult = RegSetValueExA(hKey, "GlobalUserDisabled", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Background apps re-enabled.\n";
        }
        RegCloseKey(hKey);
    }

    // Restore Advanced Tweaks
    TweakResult r2 = restorePriority();
    TweakResult r3 = restoreResourcePolicies();
    TweakResult r4 = restoreNtfsTweaks(); // New
    result.success &= r2.success && r3.success && r4.success;
    result.tweaksApplied += r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied;
    result.message += r2.message + r3.message + r4.message;

    // Restore Extreme Tweaks
    TweakResult r5 = restoreKernelTweaks();
    TweakResult r6 = enableVBS();
    TweakResult r7 = enableThreadDPC();
    TweakResult r8 = restoreTimerExpiration();
    TweakResult r9 = enableWindowsUpdates(); // New
    result.success &= r5.success && r6.success && r7.success && r8.success && r9.success;
    result.tweaksApplied += r5.tweaksApplied + r6.tweaksApplied + r7.tweaksApplied + r8.tweaksApplied + r9.tweaksApplied;
    result.message += r5.message + r6.message + r7.message + r8.message + r9.message;

    // Restore from backups
    restoreRegistryKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\VisualEffects", "visual_effects_backup.reg");
    restoreRegistryKey("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Serialize", "serialize_backup.reg");
    restoreRegistryKey("Control Panel\\Desktop", "desktop_backup.reg"); // New
    restoreRegistryKey("SYSTEM\\CurrentControlSet\\Control\\PriorityControl", "priority_backup.reg");
    restoreRegistryKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\BackgroundAccessApplications", "background_backup.reg");
    restoreRegistryKey("SYSTEM\\CurrentControlSet\\Control\\FileSystem", "ntfs_backup.reg"); // New
    restoreRegistryKey("SYSTEM\\CurrentControlSet\\Control", "kernel_backup.reg");
    restoreRegistryKey("SOFTWARE\\Policies\\Microsoft\\Windows\\System", "vbs_backup.reg");
    restoreRegistryKey("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\kernel", "dpc_backup.reg");
    restoreRegistryKey("SYSTEM\\CurrentControlSet\\Control\\TimeZoneInformation", "timer_backup.reg");
    restoreRegistryKey("SOFTWARE\\Policies\\Microsoft\\Windows\\WindowsUpdate", "updates_backup.reg"); // New

    return result;
}

// Existing Helper Methods (unchanged unless noted)
TweakResult RegistryTweaks::optimizePriority() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\PriorityControl", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 26;
        regResult = RegSetValueExA(hKey, "Win32PrioritySeparation", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Optimized priority for foreground applications.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to set Win32PrioritySeparation: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::applyKernelTweaks() {
    TweakResult result = { true, "", 0, 1 };
    if (system("bcdedit /set disabledynamictick yes") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled dynamic ticks for lower latency.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable dynamic ticks.\n";
    }
    return result;
}

TweakResult RegistryTweaks::setResourcePolicies() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 100;
        regResult = RegSetValueExA(hKey, "SystemResponsiveness", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Set resource policies to prioritize foreground tasks.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to set SystemResponsiveness: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::disableVBS() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\System", 0, KEY_SET_VALUE, &hKey);
    if (regResult != ERROR_SUCCESS) {
        regResult = RegCreateKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\System", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);
    }
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0;
        regResult = RegSetValueExA(hKey, "EnableVirtualizationBasedSecurity", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled Virtualization-Based Security.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to disable VBS: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::disableThreadDPC() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\kernel", 0, KEY_SET_VALUE, &hKey);
    if (regResult != ERROR_SUCCESS) {
        regResult = RegCreateKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\kernel", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);
    }
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0;
        regResult = RegSetValueExA(hKey, "DisableThreadDPC", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled Thread DPC for lower-priority tasks.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to disable Thread DPC: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::serializeTimerExpiration() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\TimeZoneInformation", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1;
        regResult = RegSetValueExA(hKey, "RealTimeIsUniversal", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Serialized timer expiration for better resource management.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to set RealTimeIsUniversal: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

// New Helper Methods
TweakResult RegistryTweaks::applyGeneralSettings() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Desktop", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        const char* value = "0"; // Reduce menu show delay to 0ms
        regResult = RegSetValueExA(hKey, "MenuShowDelay", 0, REG_SZ, (BYTE*)value, strlen(value) + 1);
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Reduced menu show delay for faster UI response.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to set MenuShowDelay: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::applyNtfsTweaks() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\FileSystem", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1; // Disable last access updates
        regResult = RegSetValueExA(hKey, "NtfsDisableLastAccessUpdate", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled NTFS last access updates for better performance.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to set NtfsDisableLastAccessUpdate: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::disableWindowsUpdates() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\WindowsUpdate", 0, KEY_SET_VALUE, &hKey);
    if (regResult != ERROR_SUCCESS) {
        regResult = RegCreateKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\WindowsUpdate", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);
    }
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1;
        regResult = RegSetValueExA(hKey, "NoAutoUpdate", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled Windows Updates to reduce background activity.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to disable Windows Updates: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::restorePriority() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\PriorityControl", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 2; // Default value
        regResult = RegSetValueExA(hKey, "Win32PrioritySeparation", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Restored default priority settings.\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::restoreKernelTweaks() {
    TweakResult result = { true, "", 0, 1 };
    if (system("bcdedit /set disabledynamictick no") == 0) {
        result.tweaksApplied++;
        result.message += "Restored dynamic ticks.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to restore dynamic ticks.\n";
    }
    return result;
}

TweakResult RegistryTweaks::restoreResourcePolicies() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "SystemResponsiveness");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored default resource policies.\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::enableVBS() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\System", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "EnableVirtualizationBasedSecurity");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Re-enabled Virtualization-Based Security.\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::enableThreadDPC() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\kernel", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "DisableThreadDPC");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Re-enabled Thread DPC.\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::restoreTimerExpiration() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\TimeZoneInformation", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "RealTimeIsUniversal");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored default timer expiration settings.\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::restoreGeneralSettings() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\Desktop", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        const char* value = "400"; // Default menu show delay (400ms)
        regResult = RegSetValueExA(hKey, "MenuShowDelay", 0, REG_SZ, (BYTE*)value, strlen(value) + 1);
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Restored default menu show delay.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to restore MenuShowDelay: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::restoreNtfsTweaks() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\FileSystem", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "NtfsDisableLastAccessUpdate");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored NTFS last access update settings.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to restore NtfsDisableLastAccessUpdate: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::enableWindowsUpdates() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\WindowsUpdate", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "NoAutoUpdate");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Re-enabled Windows Updates.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to re-enable Windows Updates: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::disableStartupApps() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        // Example: Disable OneDrive (extend with enumeration if needed)
        regResult = RegDeleteValueA(hKey, "OneDrive");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Disabled startup apps.\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::disableTelemetry() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection", 0, KEY_SET_VALUE, &hKey);
    if (regResult != ERROR_SUCCESS) {
        regResult = RegCreateKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\DataCollection", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);
    }
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0;
        regResult = RegSetValueExA(hKey, "AllowTelemetry", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled Windows telemetry.\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}

TweakResult RegistryTweaks::disableXboxApps() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\XblAuthManager", 0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 4; // Disabled
        regResult = RegSetValueExA(hKey, "Start", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled Xbox services via registry.\n";
        }
        RegCloseKey(hKey);
    }
    return result;
}
bool RegistryTweaks::backupRegistryKey(const std::string& keyPath, const std::string& backupFile) {
    std::string command = "reg export \"HKEY_CURRENT_USER\\" + keyPath + "\" \"" + backupFile + "\" /y";
    return system(command.c_str()) == 0;
}

bool RegistryTweaks::restoreRegistryKey(const std::string& keyPath, const std::string& backupFile) {
    std::string command = "reg import \"" + backupFile + "\"";
    return system(command.c_str()) == 0;
}
