#ifndef GRAPHICS_TWEAKS_H
#define GRAPHICS_TWEAKS_H

#include <string>
#include <windows.h>
#include "SystemOptimizer.h"

struct TweakResult;

class GraphicsTweaks {
private:
    SystemOptimizer& parent;

public:
    GraphicsTweaks(SystemOptimizer& optimizer);

    // Basic Tweaks
    TweakResult optimizeNvidiaGPU(); // Disables power-saving for better performance
    TweakResult disableMPO();        // Disables Multi-Plane Overlay

    // Advanced Tweaks
    TweakResult enableHags();        // Hardware Accelerated GPU Scheduling
    TweakResult optimizeDwm();       // Desktop Windows Manager optimization

    // Extreme Tweaks
    TweakResult enableHwFlip();        // Hardware Flip for rendering
    TweakResult disableVsyncIdleTimeout(); // Disable VSync idle timeout
    TweakResult applyDirect3DTweaks(); // Direct3D performance tweaks
    TweakResult forcePOState();        // Force P0 state (highest power state)
    TweakResult enableLowLatencyDisplay(); // Enable low latency display

    // NVIDIA GPU Tweaks
    TweakResult optimizeRecommendedSettings(); // Recommended settings for most users
    TweakResult optimizePerformance();        // Maximize GPU performance
    TweakResult optimizePower();             // Power management optimization
    TweakResult optimizeLatency();           // Low latency settings for competitive gaming
    TweakResult optimizeAdvanced();          // Advanced tuning for technical users

    // New Tweaks
    TweakResult disableHDCP();               // Disable HDCP to reduce latency
    TweakResult disablePStates();            // Disable P-States to force high-performance state
    TweakResult disableNvidiaTelemetry();    // Disable NVIDIA Telemetry for privacy

    // Restore Methods
    TweakResult restoreNvidiaGPU();           // Restore basic NVIDIA settings
    TweakResult enableMPO();                  // Re-enable MPO
    TweakResult restoreAdvancedGraphicsSettings(); // Restore advanced tweaks
    TweakResult restoreExtremeGraphicsSettings();  // Restore extreme tweaks
    TweakResult restoreNvidiaTweaks();        // Restore NVIDIA-specific tweaks
    TweakResult restoreHDCP();                // Restore HDCP settings
    TweakResult restorePStates();             // Restore P-States settings
    TweakResult restoreNvidiaTelemetry();     // Restore NVIDIA Telemetry settings

    // Optimization Levels
    TweakResult applyBasicOptimizations();    // Basic optimization level
    TweakResult applyAdvancedOptimizations(); // Advanced optimization level
    TweakResult applyExtremeOptimizations();  // Extreme optimization level
    TweakResult restoreDefaults();            // Restore all settings to default
};

#endif
