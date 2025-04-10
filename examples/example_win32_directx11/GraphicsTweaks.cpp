#include "GraphicsTweaks.h"
#include <spdlog/spdlog.h>

GraphicsTweaks::GraphicsTweaks(SystemOptimizer& optimizer) : parent(optimizer) {}

// Basic Tweaks
TweakResult GraphicsTweaks::optimizeNvidiaGPU() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0;
        regResult = RegSetValueExA(hKey, "PowerMizerEnable", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled Nvidia GPU power-saving for better performance.\n";
            parent.logger->info("Disabled Nvidia GPU power-saving for better performance");
        }
        else {
            result.success = false;
            result.message += "Failed to disable Nvidia power-saving: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to disable Nvidia power-saving: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open Nvidia GPU key (may not be an Nvidia GPU): Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open Nvidia GPU key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::disableMPO() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\Dwm",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0;
        regResult = RegSetValueExA(hKey, "OverlayTestMode", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled Multi-Plane Overlay (MPO).\n";
            parent.logger->info("Disabled Multi-Plane Overlay (MPO)");
        }
        else {
            result.success = false;
            result.message += "Failed to disable MPO: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to disable MPO: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open Dwm key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open Dwm key: Error {}", regResult);
    }
    return result;
}

// Advanced Tweaks
TweakResult GraphicsTweaks::enableHags() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 2; // 2 = Force On
        regResult = RegSetValueExA(hKey, "HwSchMode", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Enabled Hardware Accelerated GPU Scheduling (HAGS).\n";
            parent.logger->info("Enabled Hardware Accelerated GPU Scheduling (HAGS)");
        }
        else {
            result.success = false;
            result.message += "Failed to enable HAGS: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to enable HAGS: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::optimizeDwm() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\DWM",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0;
        regResult = RegSetValueExA(hKey, "Composition", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Optimized Desktop Windows Manager by disabling composition.\n";
            parent.logger->info("Optimized Desktop Windows Manager by disabling composition");
        }
        else {
            result.success = false;
            result.message += "Failed to optimize DWM: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to optimize DWM: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open DWM key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open DWM key: Error {}", regResult);
    }
    return result;
}

// Extreme Tweaks
TweakResult GraphicsTweaks::enableHwFlip() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1;
        regResult = RegSetValueExA(hKey, "HWFlipQueue", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Enabled Hardware Flip for improved rendering.\n";
            parent.logger->info("Enabled Hardware Flip for improved rendering");
        }
        else {
            result.success = false;
            result.message += "Failed to enable HW Flip: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to enable HW Flip: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::disableVsyncIdleTimeout() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0;
        regResult = RegSetValueExA(hKey, "VSyncIdleTimeout", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled VSync Idle Timeout to reduce latency.\n";
            parent.logger->info("Disabled VSync Idle Timeout to reduce latency");
        }
        else {
            result.success = false;
            result.message += "Failed to disable VSync Idle Timeout: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to disable VSync Idle Timeout: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::applyDirect3DTweaks() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Direct3D",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1; // Single buffer for lower latency
        regResult = RegSetValueExA(hKey, "FlipExBufferCount", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Applied Direct3D tweaks for optimized performance.\n";
            parent.logger->info("Applied Direct3D tweaks for optimized performance");
        }
        else {
            result.success = false;
            result.message += "Failed to apply Direct3D tweaks: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to apply Direct3D tweaks: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open Direct3D key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open Direct3D key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::forcePOState() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers\\Power",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // Force P0 state (highest power state)
        regResult = RegSetValueExA(hKey, "DefaultD3TransitionLatency", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Forced GPU to P0 state.\n";
            parent.logger->info("Forced GPU to P0 state");
        }
        else {
            result.success = false;
            result.message += "Failed to force P0 state: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to force P0 state: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers\\Power key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers\\Power key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::enableLowLatencyDisplay() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1; // Enable low latency mode
        regResult = RegSetValueExA(hKey, "LowLatency", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Enabled low latency display.\n";
            parent.logger->info("Enabled low latency display");
        }
        else {
            result.success = false;
            result.message += "Failed to enable low latency display: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to enable low latency display: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers key: Error {}", regResult);
    }
    return result;
}

// NVIDIA GPU Tweaks
TweakResult GraphicsTweaks::optimizeRecommendedSettings() {
    TweakResult result = { true, "Applying recommended NVIDIA settings...\n", 0, 5 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        struct Setting { DWORD value; const char* key; const char* desc; };
        Setting settings[] = {
            {1, "TextureFilteringQuality", "Set texture filtering to High Performance"},
            {0, "VSync", "Disabled Vertical Sync"},
            {1, "VRPreRenderLimit", "Set pre-rendered frames to 1"},
            {1, "ShaderCache", "Enabled shader cache"},
            {0, "AnisotropicFiltering", "Set anisotropic filtering to application-controlled"}
        };
        for (const auto& setting : settings) {
            regResult = RegSetValueExA(hKey, setting.key, 0, REG_DWORD, (BYTE*)&setting.value, sizeof(DWORD));
            if (regResult == ERROR_SUCCESS) {
                result.tweaksApplied++;
                result.message += std::string(setting.desc) + ".\n";
                parent.logger->info(setting.desc);
            }
            else {
                result.success = false;
                result.message += std::string("Failed to set ") + setting.key + ": Error " + std::to_string(regResult) + "\n";
                parent.logger->error("Failed to set {}: Error {}", setting.key, regResult);
            }
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NVIDIA registry key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NVIDIA registry key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::optimizePerformance() {
    TweakResult result = { true, "Applying performance optimizations...\n", 0, 4 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        struct Setting { DWORD value; const char* key; const char* desc; };
        Setting settings[] = {
            {1, "PowerMgtMode", "Set power management to max performance"},
            {1, "TrilinearOptimization", "Enabled trilinear optimization"},
            {1, "ThreadedOptimization", "Enabled threaded optimization"},
            {2, "LowLatencyMode", "Set low latency mode to Ultra"}
        };
        for (const auto& setting : settings) {
            regResult = RegSetValueExA(hKey, setting.key, 0, REG_DWORD, (BYTE*)&setting.value, sizeof(DWORD));
            if (regResult == ERROR_SUCCESS) {
                result.tweaksApplied++;
                result.message += std::string(setting.desc) + ".\n";
                parent.logger->info(setting.desc);
            }
            else {
                result.success = false;
                result.message += std::string("Failed to set ") + setting.key + ": Error " + std::to_string(regResult) + "\n";
                parent.logger->error("Failed to set {}: Error {}", setting.key, regResult);
            }
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NVIDIA registry key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NVIDIA registry key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::optimizePower() {
    TweakResult result = { true, "Applying power management tweaks...\n", 0, 2 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        struct Setting { DWORD value; const char* key; const char* desc; };
        Setting settings[] = {
            {100, "PowerLimit", "Unlocked power limit to maximum"},
            {1, "PowerMgtMode", "Set power management to max performance"}
        };
        for (const auto& setting : settings) {
            regResult = RegSetValueExA(hKey, setting.key, 0, REG_DWORD, (BYTE*)&setting.value, sizeof(DWORD));
            if (regResult == ERROR_SUCCESS) {
                result.tweaksApplied++;
                result.message += std::string(setting.desc) + ".\n";
                parent.logger->info(setting.desc);
            }
            else {
                result.success = false;
                result.message += std::string("Failed to set ") + setting.key + ": Error " + std::to_string(regResult) + "\n";
                parent.logger->error("Failed to set {}: Error {}", setting.key, regResult);
            }
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NVIDIA registry key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NVIDIA registry key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::optimizeLatency() {
    TweakResult result = { true, "Applying latency optimizations...\n", 0, 3 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        struct Setting { DWORD value; const char* key; const char* desc; };
        Setting settings[] = {
            {2, "LowLatencyMode", "Set low latency mode to Ultra"},
            {1, "VRPreRenderLimit", "Set pre-rendered frames to 1"},
            {0, "VSync", "Disabled Vertical Sync"}
        };
        for (const auto& setting : settings) {
            regResult = RegSetValueExA(hKey, setting.key, 0, REG_DWORD, (BYTE*)&setting.value, sizeof(DWORD));
            if (regResult == ERROR_SUCCESS) {
                result.tweaksApplied++;
                result.message += std::string(setting.desc) + ".\n";
                parent.logger->info(setting.desc);
            }
            else {
                result.success = false;
                result.message += std::string("Failed to set ") + setting.key + ": Error " + std::to_string(regResult) + "\n";
                parent.logger->error("Failed to set {}: Error {}", setting.key, regResult);
            }
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NVIDIA registry key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NVIDIA registry key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::optimizeAdvanced() {
    TweakResult result = { true, "Applying advanced tuning...\n", 0, 4 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        struct Setting { DWORD value; const char* key; const char* desc; };
        Setting settings[] = {
            {0, "MFAA", "Disabled Multi-Frame Anti-Aliasing"},
            {1, "TextureFilteringNegativeLODBias", "Allowed negative LOD bias"},
            {0, "TripleBuffering", "Disabled triple buffering"},
            {1, "AnisotropicSampleOptimization", "Enabled anisotropic sample optimization"}
        };
        for (const auto& setting : settings) {
            regResult = RegSetValueExA(hKey, setting.key, 0, REG_DWORD, (BYTE*)&setting.value, sizeof(DWORD));
            if (regResult == ERROR_SUCCESS) {
                result.tweaksApplied++;
                result.message += std::string(setting.desc) + ".\n";
                parent.logger->info(setting.desc);
            }
            else {
                result.success = false;
                result.message += std::string("Failed to set ") + setting.key + ": Error " + std::to_string(regResult) + "\n";
                parent.logger->error("Failed to set {}: Error {}", setting.key, regResult);
            }
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NVIDIA registry key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NVIDIA registry key: Error {}", regResult);
    }
    return result;
}

// New Tweaks
TweakResult GraphicsTweaks::disableHDCP() {
    TweakResult result = { true, "Disabling HDCP...\n", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // 0 = Disable HDCP
        regResult = RegSetValueExA(hKey, "HDCPSupport", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled HDCP to reduce latency.\n";
            parent.logger->info("Disabled HDCP to reduce latency");
        }
        else {
            result.success = false;
            result.message += "Failed to disable HDCP: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to disable HDCP: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NVIDIA registry key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NVIDIA registry key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::disablePStates() {
    TweakResult result = { true, "Disabling P-States...\n", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // 0 = Disable P-States, force P0
        regResult = RegSetValueExA(hKey, "DisableDynamicPstate", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled P-States to force high-performance state.\n";
            parent.logger->info("Disabled P-States to force high-performance state");
        }
        else {
            result.success = false;
            result.message += "Failed to disable P-States: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to disable P-States: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NVIDIA registry key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NVIDIA registry key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::disableNvidiaTelemetry() {
    TweakResult result = { true, "Disabling NVIDIA Telemetry...\n", 0, 3 };
    HKEY hKey;
    LONG regResult;

    regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\NVIDIA Corporation\\NvControlPanel2\\Client",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // 0 = Disable telemetry
        regResult = RegSetValueExA(hKey, "OptIn", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled NVIDIA Telemetry opt-in.\n";
            parent.logger->info("Disabled NVIDIA Telemetry opt-in");
        }
        else {
            result.success = false;
            result.message += "Failed to disable telemetry opt-in: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to disable telemetry opt-in: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NvControlPanel2 key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NvControlPanel2 key: Error {}", regResult);
    }

    regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Schedule\\TaskCache\\Tree\\NVIDIA",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteKeyA(hKey, "NvTmMon");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Disabled NVIDIA Telemetry monitoring task.\n";
            parent.logger->info("Disabled NVIDIA Telemetry monitoring task");
        }
        regResult = RegDeleteKeyA(hKey, "NvTmRep");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Disabled NVIDIA Telemetry reporting task.\n";
            parent.logger->info("Disabled NVIDIA Telemetry reporting task");
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open telemetry task key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open telemetry task key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::enableGsyncOptimizations() {
    TweakResult result = { true, "Optimizing G-Sync settings...\n", 0, 2 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        struct Setting { DWORD value; const char* key; const char* desc; };
        Setting settings[] = {
            {1, "GsyncMode", "Enabled G-Sync full optimization"},
            {0, "GsyncLatencyOptimize", "Optimized G-Sync latency"}
        };
        for (const auto& setting : settings) {
            regResult = RegSetValueExA(hKey, setting.key, 0, REG_DWORD, (BYTE*)&setting.value, sizeof(DWORD));
            if (regResult == ERROR_SUCCESS) {
                result.tweaksApplied++;
                result.message += std::string(setting.desc) + ".\n";
                parent.logger->info(setting.desc);
            }
            else {
                result.success = false;
                result.message += std::string("Failed to set ") + setting.key + ": Error " + std::to_string(regResult) + "\n";
                parent.logger->error("Failed to set {}: Error {}", setting.key, regResult);
            }
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NVIDIA registry key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NVIDIA registry key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::disableGpuIdleStates() {
    TweakResult result = { true, "Disabling GPU idle states...\n", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // 0 = Disable idle states
        regResult = RegSetValueExA(hKey, "IdleStateTimeout", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Disabled GPU idle states for consistent performance.\n";
            parent.logger->info("Disabled GPU idle states for consistent performance");
        }
        else {
            result.success = false;
            result.message += "Failed to disable GPU idle states: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to disable GPU idle states: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::optimizeShaderPrecache() {
    TweakResult result = { true, "Optimizing shader precache...\n", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 2; // 2 = Optimize precache size
        regResult = RegSetValueExA(hKey, "ShaderCacheSize", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Optimized shader precache size.\n";
            parent.logger->info("Optimized shader precache size");
        }
        else {
            result.success = false;
            result.message += "Failed to optimize shader precache: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to optimize shader precache: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NVIDIA registry key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NVIDIA registry key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::enableFastVRAM() {
    TweakResult result = { true, "Enabling fast VRAM access...\n", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1; // 1 = Enable fast VRAM access
        regResult = RegSetValueExA(hKey, "VRAMFastAccess", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Enabled fast VRAM access for improved performance.\n";
            parent.logger->info("Enabled fast VRAM access for improved performance");
        }
        else {
            result.success = false;
            result.message += "Failed to enable fast VRAM: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to enable fast VRAM: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::disableDriverOverhead() {
    TweakResult result = { true, "Reducing driver overhead...\n", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 0; // 0 = Minimize driver overhead
        regResult = RegSetValueExA(hKey, "DriverOverheadReduction", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Reduced driver overhead for better efficiency.\n";
            parent.logger->info("Reduced driver overhead for better efficiency");
        }
        else {
            result.success = false;
            result.message += "Failed to reduce driver overhead: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to reduce driver overhead: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::optimizeDisplayPipeline() {
    TweakResult result = { true, "Optimizing display pipeline...\n", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        DWORD value = 1; // 1 = Optimize pipeline
        regResult = RegSetValueExA(hKey, "DisplayPipelineOptimization", 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        if (regResult == ERROR_SUCCESS) {
            result.tweaksApplied++;
            result.message += "Optimized display pipeline for better performance.\n";
            parent.logger->info("Optimized display pipeline for better performance");
        }
        else {
            result.success = false;
            result.message += "Failed to optimize display pipeline: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to optimize display pipeline: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers key: Error {}", regResult);
    }
    return result;
}

// Restore Methods
TweakResult GraphicsTweaks::restoreNvidiaGPU() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "PowerMizerEnable");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored Nvidia GPU power-saving settings.\n";
            parent.logger->info("Restored Nvidia GPU power-saving settings");
        }
        else {
            result.success = false;
            result.message += "Failed to restore Nvidia power-saving: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to restore Nvidia power-saving: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open Nvidia GPU key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open Nvidia GPU key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::enableMPO() {
    TweakResult result = { true, "", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\Dwm",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "OverlayTestMode");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Re-enabled Multi-Plane Overlay (MPO).\n";
            parent.logger->info("Re-enabled Multi-Plane Overlay (MPO)");
        }
        else {
            result.success = false;
            result.message += "Failed to re-enable MPO: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to re-enable MPO: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open Dwm key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open Dwm key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::restoreAdvancedGraphicsSettings() {
    TweakResult result = { true, "", 0, 2 };
    HKEY hKey;
    LONG regResult;

    regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "HwSchMode");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored Hardware Accelerated GPU Scheduling to default.\n";
            parent.logger->info("Restored Hardware Accelerated GPU Scheduling to default");
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers Delegate key: Error {}", regResult);
    }

    regResult = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\DWM",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "Composition");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored Desktop Windows Manager settings.\n";
            parent.logger->info("Restored Desktop Windows Manager settings");
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open DWM key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open DWM key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::restoreExtremeGraphicsSettings() {
    TweakResult result = { true, "", 0, 5 };
    HKEY hKey;
    LONG regResult;

    regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        const char* values[] = { "HWFlipQueue", "VSyncIdleTimeout", "LowLatency" };
        for (const char* value : values) {
            regResult = RegDeleteValueA(hKey, value);
            if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
                result.tweaksApplied++;
                result.message += std::string("Restored ") + value + " settings.\n";
                parent.logger->info("Restored {} settings", value);
            }
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers key: Error {}", regResult);
    }

    regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Direct3D",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "FlipExBufferCount");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored Direct3D settings.\n";
            parent.logger->info("Restored Direct3D settings");
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open Direct3D key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open Direct3D key: Error {}", regResult);
    }

    regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers\\Power",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "DefaultD3TransitionLatency");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored P0 state settings.\n";
            parent.logger->info("Restored P0 state settings");
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers\\Power key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers\\Power key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::restoreNvidiaTweaks() {
    TweakResult result = { true, "Restoring NVIDIA-specific tweaks...\n", 0, 14 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        const char* values[] = {
            "TextureFilteringQuality", "VSync", "VRPreRenderLimit", "ShaderCache", "AnisotropicFiltering",
            "PowerMgtMode", "TrilinearOptimization", "ThreadedOptimization", "LowLatencyMode",
            "PowerLimit", "MFAA", "TextureFilteringNegativeLODBias", "TripleBuffering", "AnisotropicSampleOptimization"
        };
        for (const char* value : values) {
            regResult = RegDeleteValueA(hKey, value);
            if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
                result.tweaksApplied++;
                result.message += std::string("Restored ") + value + " to default.\n";
                parent.logger->info("Restored {} to default", value);
            }
            else {
                result.success = false;
                result.message += std::string("Failed to restore ") + value + ": Error " + std::to_string(regResult) + "\n";
                parent.logger->error("Failed to restore {}: Error {}", value, regResult);
            }
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NVIDIA registry key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NVIDIA registry key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::restoreHDCP() {
    TweakResult result = { true, "Restoring HDCP...\n", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "HDCPSupport");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored HDCP settings to default.\n";
            parent.logger->info("Restored HDCP settings to default");
        }
        else {
            result.success = false;
            result.message += "Failed to restore HDCP: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to restore HDCP: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NVIDIA registry key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NVIDIA registry key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::restorePStates() {
    TweakResult result = { true, "Restoring P-States...\n", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "DisableDynamicPstate");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored P-States to default dynamic behavior.\n";
            parent.logger->info("Restored P-States to default dynamic behavior");
        }
        else {
            result.success = false;
            result.message += "Failed to restore P-States: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to restore P-States: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NVIDIA registry key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NVIDIA registry key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::restoreNvidiaTelemetry() {
    TweakResult result = { true, "Restoring NVIDIA Telemetry...\n", 0, 1 };
    HKEY hKey;
    LONG regResult;

    regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\NVIDIA Corporation\\NvControlPanel2\\Client",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "OptIn");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored NVIDIA Telemetry settings to default.\n";
            parent.logger->info("Restored NVIDIA Telemetry settings to default");
        }
        else {
            result.success = false;
            result.message += "Failed to restore telemetry opt-in: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to restore telemetry opt-in: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NvControlPanel2 key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NvControlPanel2 key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::restoreGsyncOptimizations() {
    TweakResult result = { true, "Restoring G-Sync settings...\n", 0, 2 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        const char* values[] = { "GsyncMode", "GsyncLatencyOptimize" };
        for (const char* value : values) {
            regResult = RegDeleteValueA(hKey, value);
            if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
                result.tweaksApplied++;
                result.message += std::string("Restored ") + value + " to default.\n";
                parent.logger->info("Restored {} to default", value);
            }
            else {
                result.success = false;
                result.message += std::string("Failed to restore ") + value + ": Error " + std::to_string(regResult) + "\n";
                parent.logger->error("Failed to restore {}: Error {}", value, regResult);
            }
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NVIDIA registry key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NVIDIA registry key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::restoreGpuIdleStates() {
    TweakResult result = { true, "Restoring GPU idle states...\n", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "IdleStateTimeout");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored GPU idle state settings.\n";
            parent.logger->info("Restored GPU idle state settings");
        }
        else {
            result.success = false;
            result.message += "Failed to restore GPU idle states: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to restore GPU idle states: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::restoreShaderPrecache() {
    TweakResult result = { true, "Restoring shader precache...\n", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "ShaderCacheSize");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored shader precache settings.\n";
            parent.logger->info("Restored shader precache settings");
        }
        else {
            result.success = false;
            result.message += "Failed to restore shader precache: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to restore shader precache: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open NVIDIA registry key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open NVIDIA registry key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::restoreFastVRAM() {
    TweakResult result = { true, "Restoring VRAM access settings...\n", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "VRAMFastAccess");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored VRAM access settings.\n";
            parent.logger->info("Restored VRAM access settings");
        }
        else {
            result.success = false;
            result.message += "Failed to restore VRAM access: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to restore VRAM access: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::restoreDriverOverhead() {
    TweakResult result = { true, "Restoring driver overhead settings...\n", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "DriverOverheadReduction");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored driver overhead settings.\n";
            parent.logger->info("Restored driver overhead settings");
        }
        else {
            result.success = false;
            result.message += "Failed to restore driver overhead: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to restore driver overhead: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers key: Error {}", regResult);
    }
    return result;
}

TweakResult GraphicsTweaks::restoreDisplayPipeline() {
    TweakResult result = { true, "Restoring display pipeline settings...\n", 0, 1 };
    HKEY hKey;
    LONG regResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\GraphicsDrivers",
        0, KEY_SET_VALUE, &hKey);
    if (regResult == ERROR_SUCCESS) {
        regResult = RegDeleteValueA(hKey, "DisplayPipelineOptimization");
        if (regResult == ERROR_SUCCESS || regResult == ERROR_FILE_NOT_FOUND) {
            result.tweaksApplied++;
            result.message += "Restored display pipeline settings.\n";
            parent.logger->info("Restored display pipeline settings");
        }
        else {
            result.success = false;
            result.message += "Failed to restore display pipeline: Error " + std::to_string(regResult) + "\n";
            parent.logger->error("Failed to restore display pipeline: Error {}", regResult);
        }
        RegCloseKey(hKey);
    }
    else {
        result.success = false;
        result.message += "Failed to open GraphicsDrivers key: Error " + std::to_string(regResult) + "\n";
        parent.logger->error("Failed to open GraphicsDrivers key: Error {}", regResult);
    }
    return result;
}

// Optimization Levels
TweakResult GraphicsTweaks::applyBasicOptimizations() {
    TweakResult r1 = optimizeNvidiaGPU();
    TweakResult r2 = disableMPO();
    TweakResult result = { r1.success && r2.success, r1.message + r2.message,
                          r1.tweaksApplied + r2.tweaksApplied, 2 };
    return result;
}

TweakResult GraphicsTweaks::applyAdvancedOptimizations() {
    TweakResult r1 = optimizeNvidiaGPU();
    TweakResult r2 = disableMPO();
    TweakResult r3 = enableHags();
    TweakResult r4 = optimizeDwm();
    TweakResult r5 = optimizeRecommendedSettings();
    TweakResult result = { r1.success && r2.success && r3.success && r4.success && r5.success,
                          r1.message + r2.message + r3.message + r4.message + r5.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied + r5.tweaksApplied, 9 };
    return result;
}

TweakResult GraphicsTweaks::applyExtremeOptimizations() {
    TweakResult r1 = optimizeNvidiaGPU();
    TweakResult r2 = disableMPO();
    TweakResult r3 = enableHags();
    TweakResult r4 = optimizeDwm();
    TweakResult r5 = enableHwFlip();
    TweakResult r6 = disableVsyncIdleTimeout();
    TweakResult r7 = applyDirect3DTweaks();
    TweakResult r8 = { true, "Optimized graphics drivers\n", 1, 1 };
    parent.optimizeGraphicsDrivers();
    TweakResult r9 = forcePOState();
    TweakResult r10 = enableLowLatencyDisplay();
    TweakResult r11 = optimizePerformance();
    TweakResult r12 = optimizePower();
    TweakResult r13 = optimizeLatency();
    TweakResult r14 = optimizeAdvanced();
    TweakResult r15 = disableHDCP();
    TweakResult r16 = disablePStates();
    TweakResult r17 = disableNvidiaTelemetry();
    TweakResult r18 = enableGsyncOptimizations();
    TweakResult r19 = disableGpuIdleStates();
    TweakResult r20 = optimizeShaderPrecache();
    TweakResult r21 = enableFastVRAM();
    TweakResult r22 = disableDriverOverhead();
    TweakResult r23 = optimizeDisplayPipeline();
    TweakResult result = { r1.success && r2.success && r3.success && r4.success && r5.success && r6.success &&
                          r7.success && r8.success && r9.success && r10.success && r11.success && r12.success &&
                          r13.success && r14.success && r15.success && r16.success && r17.success && r18.success &&
                          r19.success && r20.success && r21.success && r22.success && r23.success,
                          r1.message + r2.message + r3.message + r4.message + r5.message + r6.message +
                          r7.message + r8.message + r9.message + r10.message + r11.message + r12.message +
                          r13.message + r14.message + r15.message + r16.message + r17.message + r18.message +
                          r19.message + r20.message + r21.message + r22.message + r23.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied +
                          r5.tweaksApplied + r6.tweaksApplied + r7.tweaksApplied + r8.tweaksApplied +
                          r9.tweaksApplied + r10.tweaksApplied + r11.tweaksApplied + r12.tweaksApplied +
                          r13.tweaksApplied + r14.tweaksApplied + r15.tweaksApplied + r16.tweaksApplied +
                          r17.tweaksApplied + r18.tweaksApplied + r19.tweaksApplied + r20.tweaksApplied +
                          r21.tweaksApplied + r22.tweaksApplied + r23.tweaksApplied, 27 };
    return result;
}

TweakResult GraphicsTweaks::restoreDefaults() {
    TweakResult r1 = restoreNvidiaGPU();
    TweakResult r2 = enableMPO();
    TweakResult r3 = restoreAdvancedGraphicsSettings();
    TweakResult r4 = restoreExtremeGraphicsSettings();
    TweakResult r5 = restoreNvidiaTweaks();
    TweakResult r6 = restoreHDCP();
    TweakResult r7 = restorePStates();
    TweakResult r8 = restoreNvidiaTelemetry();
    TweakResult r9 = restoreGsyncOptimizations();
    TweakResult r10 = restoreGpuIdleStates();
    TweakResult r11 = restoreShaderPrecache();
    TweakResult r12 = restoreFastVRAM();
    TweakResult r13 = restoreDriverOverhead();
    TweakResult r14 = restoreDisplayPipeline();
    TweakResult result = { r1.success && r2.success && r3.success && r4.success && r5.success && r6.success &&
                          r7.success && r8.success && r9.success && r10.success && r11.success && r12.success &&
                          r13.success && r14.success,
                          r1.message + r2.message + r3.message + r4.message + r5.message + r6.message +
                          r7.message + r8.message + r9.message + r10.message + r11.message + r12.message +
                          r13.message + r14.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied +
                          r5.tweaksApplied + r6.tweaksApplied + r7.tweaksApplied + r8.tweaksApplied +
                          r9.tweaksApplied + r10.tweaksApplied + r11.tweaksApplied + r12.tweaksApplied +
                          r13.tweaksApplied + r14.tweaksApplied, 31 };
    return result;
}
