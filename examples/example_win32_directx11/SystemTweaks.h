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
    TweakResult disableHpet();           // New
    TweakResult optimizeFullscreen();    // New
    // Extreme Tweaks
    TweakResult applyMmcssTweaks();      // New
    // Restore Methods
    TweakResult restorePowerSettings();
    TweakResult restoreInterruptSteering();
    TweakResult restoreHpet();           // New
    TweakResult restoreFullscreen();     // New
    TweakResult restoreMmcssTweaks();    // New
    // Optimization Levels
    TweakResult applyBasicOptimizations();
    TweakResult applyAdvancedOptimizations();
    TweakResult applyExtremeOptimizations();
    TweakResult restoreDefaults();
    TweakResult disableBluetooth();        // New
    TweakResult disableHyperV();           // New
    TweakResult disableUselessServices();  // New
    TweakResult disableUselessFeatures();
    TweakResult disableLegacySupport();
    TweakResult adjustAudioLatency();         // New tweak #11 added here
    TweakResult disableSleepStudy();          // New tweak #14 added here
    TweakResult applyTimerRes();              // New tweak #18 added here
    TweakResult disableXhciInterruptModeration(); // New tweak #19 added here
    TweakResult optimizeAmdCpu();             // New tweak #20 added here
    TweakResult optimizeMsi();                // New tweak #22 added here
    TweakResult configureBcdEdit();           // New tweak #23 added here
    TweakResult disableHibernation();         // New tweak #24 added here
    TweakResult disableDriverUsbPowersavings(); // New tweak #27 added here
    TweakResult optimizeCpu();                // New tweak #28 added here
    TweakResult optimizeCpuBoostState();
};

#endif
