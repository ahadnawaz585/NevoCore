#ifndef SYSTEM_TWEAKS_H
#define SYSTEM_TWEAKS_H

#include <string>
#include <windows.h>
#include "SystemOptimizer.h"

struct TweakResult;

class SystemTweaks {
private:
    SystemOptimizer& parent;

public:
    SystemTweaks(SystemOptimizer& optimizer);
    // Basic Tweaks
    TweakResult applyPowerSettings();
    TweakResult manageInterruptSteering();
    // Advanced Tweaks
    TweakResult disableHpet();
    TweakResult optimizeFullscreen();
    // Extreme Tweaks
    TweakResult applyMmcssTweaks();
    // Restore Methods
    TweakResult restorePowerSettings();
    TweakResult restoreInterruptSteering();
    TweakResult restoreHpet();
    TweakResult restoreFullscreen();
    TweakResult restoreMmcssTweaks();
    // Optimization Levels
    TweakResult applyBasicOptimizations();
    TweakResult applyAdvancedOptimizations();
    TweakResult applyExtremeOptimizations();
    TweakResult restoreDefaults();
    TweakResult disableBluetooth();
    TweakResult disableHyperV();
    TweakResult disableUselessServices();
    TweakResult disableUselessFeatures();
    TweakResult disableLegacySupport();
    TweakResult adjustAudioLatency();
    TweakResult disableSleepStudy();
    TweakResult applyTimerRes();
    TweakResult disableXhciInterruptModeration();
    TweakResult optimizeAmdCpu();
    TweakResult optimizeMsi();
    TweakResult configureBcdEdit();
    TweakResult disableHibernation();
    TweakResult disableDriverUsbPowersavings();
    TweakResult optimizeCpu();
    TweakResult optimizeCpuBoostState();
    // New Tweaks
    TweakResult enableMemoryCompression();
    TweakResult optimizeNetworkThrottling();
    TweakResult disableSuperfetch();
    TweakResult optimizePageFile();
    TweakResult disableWindowsDefender();
    TweakResult optimizeThreadPriority();
    TweakResult disableBackgroundApps();
    TweakResult optimizeStorage();
    TweakResult disableMitigations();
    // New Restore Methods
    TweakResult restoreMemoryCompression();
    TweakResult restoreNetworkThrottling();
    TweakResult restoreSuperfetch();
    TweakResult restorePageFile();
    TweakResult restoreWindowsDefender();
    TweakResult restoreThreadPriority();
    TweakResult restoreBackgroundApps();
    TweakResult restoreStorage();
    TweakResult restoreMitigations();
};

#endif
