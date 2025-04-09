#include "BiosTweaks.h"
#include <spdlog/spdlog.h>

BiosTweaks::BiosTweaks(SystemOptimizer& optimizer) : optimizer(optimizer) {}

TweakResult BiosTweaks::optimizeCpuAndInterconnect() {
    TweakResult result = { true, "", 0, 1 };
    // BIOS-level tweak recommendation
    result.message += "Recommendation: Enter BIOS and set CPU to Performance Mode, disable C-States, and optimize interconnect settings (e.g., PCIe lanes).\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::optimizeMemoryPerformance() {
    TweakResult result = { true, "", 0, 1 };
    // BIOS-level tweak recommendation
    result.message += "Recommendation: Enter BIOS and enable XMP (Extreme Memory Profile) or manually set RAM frequency/timings for optimal performance.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::disablePowerSavingSettings() {
    TweakResult result = { true, "", 0, 1 };
    // BIOS-level tweak recommendation
    result.message += "Recommendation: Enter BIOS and disable power-saving features like EIST, SpeedStep, and C1E. Note: May increase heat.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::disableSecuritySettings() {
    TweakResult result = { true, "", 0, 1 };
    // BIOS-level tweak recommendation
    result.message += "Recommendation: Enter BIOS and disable Secure Boot and TPM if not needed for security. Caution: Reduces system security.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::disableLegacySupport() {
    TweakResult result = { true, "", 0, 1 };
    if (system("bcdedit /set disabledynamictick yes") == 0) { // Example OS-level support
        result.tweaksApplied++;
        result.message += "Disabled dynamic ticks at OS level to complement BIOS legacy disable.\n";
    }
    result.message += "Recommendation: Enter BIOS and disable Legacy Boot/CSM support.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::enableAdvancedConfig() {
    TweakResult result = { true, "", 0, 1 };
    // BIOS-level tweak recommendation
    result.message += "Recommendation: Enter BIOS and enable advanced settings (e.g., overclocking, voltage control) for CPU, RAM, and storage customization.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::enableHiddenSettings() {
    TweakResult result = { true, "", 0, 1 };
    // Experimental BIOS-level tweak recommendation
    result.message += "Recommendation (Experimental): Access hidden BIOS settings via vendor-specific key combos (e.g., Ctrl+Alt+F1) or tools. Risk of instability.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::applyBasicOptimizations() {
    TweakResult result = { true, "", 0, 0 }; // No BIOS tweaks in Basic
    return result;
}

TweakResult BiosTweaks::applyAdvancedOptimizations() {
    TweakResult r1 = optimizeCpuAndInterconnect();
    TweakResult r2 = optimizeMemoryPerformance();
    TweakResult r3 = disableLegacySupport();

    TweakResult result = { r1.success && r2.success && r3.success,
                          r1.message + r2.message + r3.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied, 3 };
    return result;
}

TweakResult BiosTweaks::applyExtremeOptimizations() {
    TweakResult r1 = optimizeCpuAndInterconnect();
    TweakResult r2 = optimizeMemoryPerformance();
    TweakResult r3 = disablePowerSavingSettings();
    TweakResult r4 = disableSecuritySettings();
    TweakResult r5 = disableLegacySupport();
    TweakResult r6 = enableAdvancedConfig();
    TweakResult r7 = enableHiddenSettings();

    TweakResult result = { r1.success && r2.success && r3.success && r4.success && r5.success && r6.success && r7.success,
                          r1.message + r2.message + r3.message + r4.message + r5.message + r6.message + r7.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied + r5.tweaksApplied + r6.tweaksApplied + r7.tweaksApplied, 7 };
    return result;
}

TweakResult BiosTweaks::restoreDefaults() {
    TweakResult result = { true, "", 0, 7 };
    result.message += "Recommendation: Enter BIOS and reset to default settings to restore CPU, memory, power, security, and legacy options.\n";
    if (system("bcdedit /deletevalue disabledynamictick") == 0) {
        result.tweaksApplied++;
        result.message += "Restored dynamic ticks at OS level.\n";
    }
    result.tweaksApplied += 6; // For manual BIOS resets
    return result;
}
