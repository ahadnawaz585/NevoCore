#ifndef NETWORK_TWEAKS_H
#define NETWORK_TWEAKS_H

#include <string>
#include <windows.h>
#include "SystemOptimizer.h"

struct TweakResult;

class NetworkTweaks {
private:
    SystemOptimizer& optimizer;

    bool backupNetworkSettings();
    bool restoreNetworkSettings();

public:
    NetworkTweaks(SystemOptimizer& opt);
    // Basic Tweaks
    TweakResult optimizeDnsSettings();
    // Advanced Tweaks
    TweakResult disableNaglesAlgorithm();
    TweakResult optimizeQoS();
    // Extreme Tweaks
    TweakResult disableTcpChimneyOffload();
    TweakResult enableRss();
    TweakResult disableUsbCoalescing();
    TweakResult optimizeNetwork();
    // Optimization Levels
    TweakResult applyBasicOptimizations();
    TweakResult applyAdvancedOptimizations();
    TweakResult applyExtremeOptimizations();
    TweakResult restoreDefaults();
};

#endif
