#ifndef BIOS_TWEAKS_H
#define BIOS_TWEAKS_H

#include <string>
#include "SystemOptimizer.h"

struct TweakResult;

class BiosTweaks {
private:
    SystemOptimizer& optimizer;

public:
    BiosTweaks(SystemOptimizer& optimizer);
    TweakResult optimizeCpuAndInterconnect();        // Advanced
    TweakResult optimizeMemoryPerformance();         // Advanced
    TweakResult disablePowerSavingSettings();        // Extreme
    TweakResult disableSecuritySettings();           // Extreme
    TweakResult disableLegacySupport();              // Advanced (OS-level support)
    TweakResult enableAdvancedConfig();              // Extreme
    TweakResult enableHiddenSettings();              // Extreme (Experimental)
    // New Basic Tweaks
    TweakResult enableFastBoot();                    // Basic
    TweakResult setBootPriorityToSSD();              // Basic
    // New Advanced Tweaks
    TweakResult optimizeFanCurves();                 // Advanced
    TweakResult enableAbove4GDecoding();             // Advanced
    TweakResult adjustCpuCacheRatio();               // Advanced
    // New Extreme Tweaks
    TweakResult disableHyperThreading();             // Extreme
    TweakResult enablePBO();                         // Extreme
    TweakResult increasePCIeSpeed();                 // Extreme
    TweakResult disableOnboardAudio();               // Extreme
    TweakResult adjustDRAMVoltage();                 // Extreme
    TweakResult applyBasicOptimizations();
    TweakResult applyAdvancedOptimizations();
    TweakResult applyExtremeOptimizations();
    TweakResult restoreDefaults();
};

#endif
