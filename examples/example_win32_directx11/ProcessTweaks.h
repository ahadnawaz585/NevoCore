#ifndef PROCESS_TWEAKS_H
#define PROCESS_TWEAKS_H

#include "SystemOptimizer.h"

class ProcessTweaks {
private:
    SystemOptimizer& optimizer;

public:
    ProcessTweaks(SystemOptimizer& optimizer);
    TweakResult applyBasicOptimizations(const std::string& grokResponse, const std::string& selfProcessName,
        const std::vector<std::string>& backgroundProcesses);
    TweakResult applyAdvancedOptimizations(const std::string& grokResponse, const std::string& selfProcessName,
        const std::vector<std::string>& backgroundProcesses);
    TweakResult applyExtremeOptimizations(const std::string& grokResponse, const std::string& selfProcessName,
        const std::vector<std::string>& backgroundProcesses);
    TweakResult prioritizeGameProcesses();
    TweakResult applyIoCacheUpdateTweaks();
    TweakResult optimizeDisks();
    TweakResult disableCpuBloat();
    TweakResult disableXboxApps();
    TweakResult disableAsusBloat();
    TweakResult optimizeTaskScheduler();
    TweakResult adjustBaseOverTargetPriorities();
    // New Tweaks
    TweakResult disableTelemetry();
    TweakResult optimizeMemoryUsage();
    TweakResult disableBackgroundServices();
    TweakResult boostForegroundApps();
    TweakResult disableCortana();
    TweakResult optimizeThreadScheduling();
    TweakResult disableWindowsSearch();
    TweakResult reduceProcessOverhead();
    TweakResult disableSuperfetchService();
    TweakResult optimizePagefileUsage();
    TweakResult disableWindowsUpdate();
    TweakResult enhanceProcessAffinity();
};

#endif
