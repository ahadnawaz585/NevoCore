#include "NetworkTweaks.h"
#include <spdlog/spdlog.h>

NetworkTweaks::NetworkTweaks(SystemOptimizer& opt) : optimizer(opt) {}

// Basic Tweaks
TweakResult NetworkTweaks::optimizeDnsSettings() {
    TweakResult result = { true, "", 0, 1 };

    if (!backupNetworkSettings()) {
        result.success = false;
        result.message += "Failed to backup network settings.\n";
        return result;
    }

    if (system("netsh interface ip set dns name=\"Ethernet\" source=static addr=8.8.8.8") == 0) {
        result.tweaksApplied++;
        result.message += "Set DNS to Google Public DNS (8.8.8.8).\n";
    }
    else {
        result.success = false;
        result.message += "Failed to set DNS to Google Public DNS.\n";
    }

    return result;
}

// Advanced Tweaks
TweakResult NetworkTweaks::disableNaglesAlgorithm() {
    TweakResult result = { true, "", 0, 1 };

    if (system("netsh int tcp set global autotuninglevel=restricted") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled Nagle's algorithm for lower latency.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable Nagle's algorithm.\n";
    }

    return result;
}

TweakResult NetworkTweaks::optimizeQoS() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Policies\\Microsoft\\Windows\\QoS",
        0, KEY_SET_VALUE, &hKey);

    if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
        if (regResult == ERROR_FILE_NOT_FOUND) {
            RegCreateKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Policies\\Microsoft\\Windows\\QoS", &hKey);
        }
        DWORD value = 80; // Prioritize game traffic
        regResult = RegSetValueExA(hKey, "GamePriority", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Optimized QoS to prioritize game traffic.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to optimize QoS: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open QoS registry key: Error " + std::to_string(regResult) + "\n";
    }
    return result;
}

// Extreme Tweaks
TweakResult NetworkTweaks::disableTcpChimneyOffload() {
    TweakResult result = { true, "", 0, 1 };

    if (system("netsh int tcp set global chimney=disabled") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled TCP Chimney Offload.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable TCP Chimney Offload.\n";
    }

    return result;
}

TweakResult NetworkTweaks::enableRss() {
    TweakResult result = { true, "", 0, 1 };

    if (system("netsh int tcp set global rss=enabled") == 0) {
        result.tweaksApplied++;
        result.message += "Enabled Receive Side Scaling (RSS) for better network performance.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to enable RSS.\n";
    }

    return result;
}

TweakResult NetworkTweaks::disableUsbCoalescing() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\usbflags",
        0, KEY_SET_VALUE, &hKey);

    if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
        if (regResult == ERROR_FILE_NOT_FOUND) {
            RegCreateKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\usbflags", &hKey);
        }
        DWORD value = 0;
        regResult = RegSetValueExA(hKey, "Coalescing", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled USB coalescing to reduce interrupt overhead.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to disable USB coalescing: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open usbflags registry key: Error " + std::to_string(regResult) + "\n";
    }
    return result;
}

// Optimization Levels
TweakResult NetworkTweaks::applyBasicOptimizations() {
    TweakResult r1 = optimizeDnsSettings();

    TweakResult result = { r1.success, r1.message, r1.tweaksApplied, 1 };
    return result;
}

TweakResult NetworkTweaks::applyAdvancedOptimizations() {
    TweakResult r1 = optimizeDnsSettings();
    TweakResult r2 = disableNaglesAlgorithm();
    TweakResult r3 = optimizeQoS();
    TweakResult r4 = optimizeNetwork(); // New tweak #15 added here (Line 50)
    TweakResult result = { r1.success && r2.success && r3.success && r4.success,
                          r1.message + r2.message + r3.message + r4.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied, 4 };
    return result;
}

TweakResult NetworkTweaks::applyExtremeOptimizations() {
    TweakResult r1 = optimizeDnsSettings();
    TweakResult r2 = disableNaglesAlgorithm();
    TweakResult r3 = optimizeQoS();
    TweakResult r4 = disableTcpChimneyOffload();
    TweakResult r5 = enableRss();
    TweakResult r6 = disableUsbCoalescing();
    TweakResult r7 = optimizeNetwork(); // New tweak #15 added here (Line 65)
    TweakResult result = { r1.success && r2.success && r3.success && r4.success && r5.success && r6.success && r7.success,
                          r1.message + r2.message + r3.message + r4.message + r5.message + r6.message + r7.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied + r5.tweaksApplied + r6.tweaksApplied + r7.tweaksApplied, 7 };
    return result;
}

TweakResult NetworkTweaks::restoreDefaults() {
    TweakResult result = { true, "", 0, 6 }; // Total tweaks to restore

    // Restore DNS
    if (system("netsh interface ip set dns name=\"Ethernet\" source=dhcp") == 0) {
        result.tweaksApplied++;
        result.message += "Restored DNS to DHCP.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to restore DNS to DHCP.\n";
    }

    // Restore TCP autotuning (Nagle's algorithm)
    if (system("netsh int tcp set global autotuninglevel=normal") == 0) {
        result.tweaksApplied++;
        result.message += "Restored TCP autotuning to normal.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to restore TCP autotuning.\n";
    }

    // Restore QoS
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Policies\\Microsoft\\Windows\\QoS",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "GamePriority");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored QoS settings to default.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to restore QoS: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }

    // Restore TCP Chimney Offload
    if (system("netsh int tcp set global chimney=enabled") == 0) {
        result.tweaksApplied++;
        result.message += "Re-enabled TCP Chimney Offload.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to re-enable TCP Chimney Offload.\n";
    }

    // Restore RSS
    if (system("netsh int tcp set global rss=disabled") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled RSS to restore defaults.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable RSS.\n";
    }

    // Restore USB Coalescing
    regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\usbflags",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "Coalescing");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored USB coalescing to default.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to restore USB coalescing: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }

    if (!restoreNetworkSettings()) {
        result.success = false;
        result.message += "Failed to restore network settings from backup.\n";
    }

    return result;
}
TweakResult NetworkTweaks::optimizeNetwork() {
    TweakResult result = { true, "", 0, 1 };
    if (system("netsh int tcp set global autotuninglevel=disabled") == 0 &&
        system("netsh int tcp set global congestionprovider=ctcp") == 0) {
        result.tweaksApplied++;
        result.message += "Optimized network adapter settings.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to optimize network settings.\n";
    }
    return result;
}
bool NetworkTweaks::backupNetworkSettings() {
    bool success = system("netsh dump > network_backup.txt") == 0;
    if (!success) {
        if (optimizer.logger) optimizer.logger->error("Failed to backup network settings");
    }
    else {
        if (optimizer.logger) optimizer.logger->info("Network settings backed up to network_backup.txt");
    }
    return success;
}

bool NetworkTweaks::restoreNetworkSettings() {
    bool success = system("netsh exec network_backup.txt") == 0;
    if (!success) {
        optimizer.logger->error("Failed to restore network settings from backup");
    }
    return success;
}
