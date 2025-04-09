#ifndef REGISTRY_TWEAKS_H
#define REGISTRY_TWEAKS_H

#include "SystemOptimizer.h"

class RegistryTweaks {
private:
    SystemOptimizer& optimizer;

    bool backupRegistryKey(const std::string& keyPath, const std::string& backupFile);
    bool restoreRegistryKey(const std::string& keyPath, const std::string& backupFile);
    TweakResult optimizePriority();
    TweakResult applyKernelTweaks();
    TweakResult setResourcePolicies();
    TweakResult disableVBS();
    TweakResult disableThreadDPC();
    TweakResult serializeTimerExpiration();
    TweakResult applyNtfsTweaks();
    TweakResult applyGeneralSettings();
    TweakResult disableWindowsUpdates();
    TweakResult disableStartupApps();  // New
    TweakResult disableTelemetry();    // New
    TweakResult disableXboxApps();     // New
    TweakResult restorePriority();
    TweakResult restoreKernelTweaks();
    TweakResult restoreResourcePolicies();
    TweakResult enableVBS();
    TweakResult enableThreadDPC();
    TweakResult restoreTimerExpiration();
    TweakResult restoreNtfsTweaks();
    TweakResult restoreGeneralSettings();
    TweakResult enableWindowsUpdates();
    TweakResult tweakSystemProfile();         // New tweak #6 added here
    TweakResult optimizeWorkerThreads();       // New tweak #7 added here
    TweakResult adjustDeviceDriverPriorities(); // New tweak #16 added here
    TweakResult optimizeMemory();

public:
    RegistryTweaks(SystemOptimizer& optimizer);
    TweakResult applyBasicOptimizations();
    TweakResult applyAdvancedOptimizations();
    TweakResult applyExtremeOptimizations();
    TweakResult restoreDefaults();
};

#endif
