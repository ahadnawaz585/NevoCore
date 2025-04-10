

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

// New Tweaks
TweakResult NetworkTweaks::enableDirectCacheAccess() {
    TweakResult result = { true, "", 0, 1 };
    if (system("netsh int tcp set global dca=enabled") == 0) {
        result.tweaksApplied++;
        result.message += "Enabled Direct Cache Access for network performance.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to enable Direct Cache Access.\n";
    }
    return result;
}

TweakResult NetworkTweaks::disableFlowControl() {
    TweakResult result = { true, "", 0, 1 };
    if (system("netsh int tcp set global flowcontrol=disabled") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled Ethernet flow control.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable flow control.\n";
    }
    return result;
}

TweakResult NetworkTweaks::optimizeMtu() {
    TweakResult result = { true, "", 0, 1 };
    if (system("netsh interface ipv4 set subinterface \"Ethernet\" mtu=1500 store=persistent") == 0) {
        result.tweaksApplied++;
        result.message += "Set optimal MTU size to 1500.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to optimize MTU size.\n";
    }
    return result;
}

TweakResult NetworkTweaks::disableEnergyEfficientEthernet() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}\\0001",
        0, KEY_SET_VALUE, &hKey);

    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // Disable EEE
        regResult = RegSetValueExA(hKey, "EEE", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled Energy Efficient Ethernet.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to disable EEE: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open network adapter registry key: Error " + std::to_string(regResult) + "\n";
    }
    return result;
}

TweakResult NetworkTweaks::enableInterruptModeration() {
    TweakResult result = { true, "", 0, 1 };
    if (system("netsh int tcp set global interruptmoderation=enabled") == 0) {
        result.tweaksApplied++;
        result.message += "Enabled interrupt moderation for network efficiency.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to enable interrupt moderation.\n";
    }
    return result;
}

TweakResult NetworkTweaks::disableLargeSendOffload() {
    TweakResult result = { true, "", 0, 1 };
    if (system("netsh int tcp set global lso=disabled") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled Large Send Offload for lower latency.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to disable Large Send Offload.\n";
    }
    return result;
}

TweakResult NetworkTweaks::optimizeTcpWindowSize() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters",
        0, KEY_SET_VALUE, &hKey);

    if (regResult == ERROR_SUCCESS) {
        DWORD value = 64240; // Optimal TCP window size
        regResult = RegSetValueExA(hKey, "TcpWindowSize", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Optimized TCP window size.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to optimize TCP window size: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open TCP parameters key: Error " + std::to_string(regResult) + "\n";
    }
    return result;
}

TweakResult NetworkTweaks::disableNetAdapterPowerSaving() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}\\0001",
        0, KEY_SET_VALUE, &hKey);

    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // Disable power saving
        regResult = RegSetValueExA(hKey, "PnPCapabilities", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled network adapter power saving.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to disable power saving: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open network adapter key: Error " + std::to_string(regResult) + "\n";
    }
    return result;
}

TweakResult NetworkTweaks::enableEcN() {
    TweakResult result = { true, "", 0, 1 };
    if (system("netsh int tcp set global ecncapability=enabled") == 0) {
        result.tweaksApplied++;
        result.message += "Enabled Explicit Congestion Notification.\n";
    }
    else {
        result.success = false;
        result.message += "Failed to enable ECN.\n";
    }
    return result;
}

TweakResult NetworkTweaks::optimizeDscp() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\QoS",
        0, KEY_SET_VALUE, &hKey);

    if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
        if (regResult == ERROR_FILE_NOT_FOUND) {
            RegCreateKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\QoS", &hKey);
        }
        DWORD value = 46; // DSCP value for high priority
        regResult = RegSetValueExA(hKey, "DefaultDSCP", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Optimized DSCP for better packet priority.\n";
        }
        else {
            result.success = false;
            result.message += "Failed to optimize DSCP: Error " + std::to_string(regResult) + "\n";
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open QoS key: Error " + std::to_string(regResult) + "\n";
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
    TweakResult r4 = optimizeNetwork();
    TweakResult r5 = enableDirectCacheAccess();
    TweakResult r6 = optimizeMtu();
    TweakResult result = { r1.success && r2.success && r3.success && r4.success && r5.success && r6.success,
                          r1.message + r2.message + r3.message + r4.message + r5.message + r6.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied +
                          r5.tweaksApplied + r6.tweaksApplied, 6 };
    return result;
}

TweakResult NetworkTweaks::applyExtremeOptimizations() {
    TweakResult r1 = optimizeDnsSettings();
    TweakResult r2 = disableNaglesAlgorithm();
    TweakResult r3 = optimizeQoS();
    TweakResult r4 = disableTcpChimneyOffload();
    TweakResult r5 = enableRss();
    TweakResult r6 = disableUsbCoalescing();
    TweakResult r7 = optimizeNetwork();
    TweakResult r8 = enableDirectCacheAccess();
    TweakResult r9 = disableFlowControl();
    TweakResult r10 = optimizeMtu();
    TweakResult r11 = disableEnergyEfficientEthernet();
    TweakResult r12 = enableInterruptModeration();
    TweakResult r13 = disableLargeSendOffload();
    TweakResult r14 = optimizeTcpWindowSize();
    TweakResult r15 = disableNetAdapterPowerSaving();
    TweakResult r16 = enableEcN();
    TweakResult r17 = optimizeDscp();
    TweakResult result = { r1.success && r2.success && r3.success && r4.success && r5.success &&
                          r6.success && r7.success && r8.success && r9.success && r10.success &&
                          r11.success && r12.success && r13.success && r14.success && r15.success &&
                          r16.success && r17.success,
                          r1.message + r2.message + r3.message + r4.message + r5.message +
                          r6.message + r7.message + r8.message + r9.message + r10.message +
                          r11.message + r12.message + r13.message + r14.message + r15.message +
                          r16.message + r17.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied +
                          r5.tweaksApplied + r6.tweaksApplied + r7.tweaksApplied + r8.tweaksApplied +
                          r9.tweaksApplied + r10.tweaksApplied + r11.tweaksApplied + r12.tweaksApplied +
                          r13.tweaksApplied + r14.tweaksApplied + r15.tweaksApplied + r16.tweaksApplied +
                          r17.tweaksApplied, 17 };
    return result;
}

TweakResult NetworkTweaks::restoreDefaults() {
    TweakResult result = { true, "", 0, 16 }; // Updated total tweaks to restore

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

    // Restore Network Settings
    if (system("netsh int tcp set global congestionprovider=default") == 0) {
        result.tweaksApplied++;
        result.message += "Restored network congestion provider to default.\n";
    }

    // Restore DCA
    if (system("netsh int tcp set global dca=disabled") == 0) {
        result.tweaksApplied++;
        result.message += "Restored Direct Cache Access to default.\n";
    }

    // Restore Flow Control
    if (system("netsh int tcp set global flowcontrol=enabled") == 0) {
        result.tweaksApplied++;
        result.message += "Restored flow control to default.\n";
    }

    // Restore MTU
    if (system("netsh interface ipv4 set subinterface \"Ethernet\" mtu=0 store=persistent") == 0) {
        result.tweaksApplied++;
        result.message += "Restored MTU to default.\n";
    }

    // Restore EEE
    regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}\\0001",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "EEE");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored Energy Efficient Ethernet to default.\n";
        }
        RegCloseKey(hKey);
    }

    // Restore Interrupt Moderation
    if (system("netsh int tcp set global interruptmoderation=disabled") == 0) {
        result.tweaksApplied++;
        result.message += "Restored interrupt moderation to default.\n";
    }

    // Restore LSO
    if (system("netsh int tcp set global lso=enabled") == 0) {
        result.tweaksApplied++;
        result.message += "Restored Large Send Offload to default.\n";
    }

    // Restore TCP Window Size
    regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "TcpWindowSize");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored TCP window size to default.\n";
        }
        RegCloseKey(hKey);
    }

    // Restore Power Saving
    regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}\\0001",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "PnPCapabilities");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored network adapter power saving to default.\n";
        }
        RegCloseKey(hKey);
    }

    // Restore ECN
    if (system("netsh int tcp set global ecncapability=disabled") == 0) {
        result.tweaksApplied++;
        result.message += "Restored ECN to default.\n";
    }

    // Restore DSCP
    regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\QoS",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "DefaultDSCP");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored DSCP to default.\n";
        }
        RegCloseKey(hKey);
    }

    if (!restoreNetworkSettings()) {
        result.success = false;
        result.message += "Failed to restore network settings from backup.\n";
    }

    return result;
}

// Helper Functions
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
