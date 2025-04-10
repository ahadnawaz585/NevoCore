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
    // New Tweaks
    TweakResult enableDirectCacheAccess();
    TweakResult disableFlowControl();
    TweakResult optimizeMtu();
    TweakResult disableEnergyEfficientEthernet();
    TweakResult enableInterruptModeration();
    TweakResult disableLargeSendOffload();
    TweakResult optimizeTcpWindowSize();
    TweakResult disableNetAdapterPowerSaving();
    TweakResult enableEcN();
    TweakResult optimizeDscp();
    // Optimization Levels
    TweakResult applyBasicOptimizations();
    TweakResult applyAdvancedOptimizations();
    TweakResult applyExtremeOptimizations();
    TweakResult restoreDefaults();
};

#endif
