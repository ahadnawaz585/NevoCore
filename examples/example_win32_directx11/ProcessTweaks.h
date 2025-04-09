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
    TweakResult disableCpuBloat();    // New
    TweakResult disableXboxApps();    // New
    TweakResult disableAsusBloat();   // New
    TweakResult optimizeTaskScheduler();         // New tweak #12 added here
    TweakResult adjustBaseOverTargetPriorities();
};

#endif
