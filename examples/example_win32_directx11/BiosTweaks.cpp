#include "BiosTweaks.h"
#include <spdlog/spdlog.h>

BiosTweaks::BiosTweaks(SystemOptimizer& optimizer) : optimizer(optimizer) {}

TweakResult BiosTweaks::optimizeCpuAndInterconnect() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and set CPU to Performance Mode, disable C-States, and optimize interconnect settings (e.g., PCIe lanes).\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::optimizeMemoryPerformance() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and enable XMP (Extreme Memory Profile) or manually set RAM frequency/timings for optimal performance.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::disablePowerSavingSettings() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and disable power-saving features like EIST, SpeedStep, and C1E. Note: May increase heat.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::disableSecuritySettings() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and disable Secure Boot and TPM if not needed for security. Caution: Reduces system security.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::disableLegacySupport() {
    TweakResult result = { true, "", 0, 1 };
    if (system("bcdedit /set disabledynamictick yes") == 0) {
        result.tweaksApplied++;
        result.message += "Disabled dynamic ticks at OS level to complement BIOS legacy disable.\n";
    }
    result.message += "Recommendation: Enter BIOS and disable Legacy Boot/CSM support.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::enableAdvancedConfig() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and enable advanced settings (e.g., overclocking, voltage control) for CPU, RAM, and storage customization.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::enableHiddenSettings() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation (Experimental): Access hidden BIOS settings via vendor-specific key combos (e.g., Ctrl+Alt+F1) or tools. Risk of instability.\n";
    result.tweaksApplied++;
    return result;
}

// New Basic Tweaks
TweakResult BiosTweaks::enableFastBoot() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and enable Fast Boot to reduce startup time.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::setBootPriorityToSSD() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and set boot priority to SSD for faster OS loading.\n";
    result.tweaksApplied++;
    return result;
}

// New Advanced Tweaks
TweakResult BiosTweaks::optimizeFanCurves() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and optimize fan curves for better cooling/performance balance.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::enableAbove4GDecoding() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and enable Above 4G Decoding for GPU-intensive tasks.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::adjustCpuCacheRatio() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and increase CPU cache ratio for better performance.\n";
    result.tweaksApplied++;
    return result;
}

// New Extreme Tweaks
TweakResult BiosTweaks::disableHyperThreading() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and disable Hyper-Threading for lower latency (single-thread focus).\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::enablePBO() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and enable Precision Boost Overdrive (AMD CPUs) for higher boost clocks.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::increasePCIeSpeed() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and force PCIe to Gen 4/5 if hardware supports it.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::disableOnboardAudio() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and disable onboard audio to free resources if unused.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::adjustDRAMVoltage() {
    TweakResult result = { true, "", 0, 1 };
    result.message += "Recommendation: Enter BIOS and increase DRAM voltage slightly for RAM overclock stability.\n";
    result.tweaksApplied++;
    return result;
}

TweakResult BiosTweaks::applyBasicOptimizations() {
    TweakResult r1 = enableFastBoot();
    TweakResult r2 = setBootPriorityToSSD();

    TweakResult result = { r1.success && r2.success,
                          r1.message + r2.message,
                          r1.tweaksApplied + r2.tweaksApplied, 2 };
    return result;
}

TweakResult BiosTweaks::applyAdvancedOptimizations() {
    TweakResult r1 = optimizeCpuAndInterconnect();
    TweakResult r2 = optimizeMemoryPerformance();
    TweakResult r3 = disableLegacySupport();
    TweakResult r4 = optimizeFanCurves();
    TweakResult r5 = enableAbove4GDecoding();
    TweakResult r6 = adjustCpuCacheRatio();

    TweakResult result = { r1.success && r2.success && r3.success && r4.success && r5.success && r6.success,
                          r1.message + r2.message + r3.message + r4.message + r5.message + r6.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied + r5.tweaksApplied + r6.tweaksApplied, 6 };
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
    TweakResult r8 = disableHyperThreading();
    TweakResult r9 = enablePBO();
    TweakResult r10 = increasePCIeSpeed();
    TweakResult r11 = disableOnboardAudio();
    TweakResult r12 = adjustDRAMVoltage();

    TweakResult result = { r1.success && r2.success && r3.success && r4.success && r5.success && r6.success && r7.success && r8.success && r9.success && r10.success && r11.success && r12.success,
                          r1.message + r2.message + r3.message + r4.message + r5.message + r6.message + r7.message + r8.message + r9.message + r10.message + r11.message + r12.message,
                          r1.tweaksApplied + r2.tweaksApplied + r3.tweaksApplied + r4.tweaksApplied + r5.tweaksApplied + r6.tweaksApplied + r7.tweaksApplied + r8.tweaksApplied + r9.tweaksApplied + r10.tweaksApplied + r11.tweaksApplied + r12.tweaksApplied, 12 };
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
