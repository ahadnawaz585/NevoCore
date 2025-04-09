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
    TweakResult applyBasicOptimizations();
    TweakResult applyAdvancedOptimizations();
    TweakResult applyExtremeOptimizations();
    TweakResult restoreDefaults();
};

#endif
