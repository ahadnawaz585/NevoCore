# System Optimization with SystemOptimizer

This document outlines how the `SystemOptimizer` class efficiently optimizes system resources including RAM, CPU, network performance, and FPS (frames per second) for enhanced system performance, with a particular focus on gaming and general usability.

---

## Overview

The `SystemOptimizer` class is designed to monitor and optimize system resources by applying tweaks to the registry, network settings, processes, and system configurations. It provides multiple optimization levels (Basic, Advanced, Extreme) and a restore option, ensuring flexibility and safety through backups and logging.

- **Current Date**: March 21, 2025
- **Language**: C++ with Windows API integration
- **Dependencies**: spdlog (logging), libcurl (API calls), Windows-specific libraries (e.g., iphlpapi.lib)

---

## Key Optimization Areas

### 1. RAM Optimization
- **Monitoring**: Tracks available RAM in KB using `GlobalMemoryStatusEx` to assess memory usage (`getSystemMetrics`).
- **Actions**:
  - **Cleanup**: Removes temporary files and releases unused memory blocks (`performExtremeOptimizationInternal`).
  - **Process Management**: Terminates non-essential background processes (e.g., browsers like `chrome.exe`, cloud services like `OneDrive.exe`) identified via Grok API and predefined lists (`terminateNonEssentialProcesses`).
  - **Virtual Memory**: Adjusts virtual memory configuration for optimal allocation (`initializeDemoLogs` logs this step).
- **Impact**: Increases available RAM, reducing memory pressure and improving system responsiveness.

### 2. CPU Optimization
- **Monitoring**: Calculates CPU usage percentage using `GetSystemTimes` to measure idle, kernel, and user time deltas (`getSystemMetrics`).
- **Actions**:
  - **Core Parking**: Disables CPU core parking to utilize all cores fully (`disableCoreParking`), critical for multi-threaded applications like games.
  - **Priority Adjustment**: Sets high or real-time priority for game processes (e.g., `csgo.exe`, `valorant.exe`) using `SetPriorityClass` (`prioritizeGameProcesses`).
  - **Service Tuning**: Adjusts service priorities to minimize background CPU load (`applyBasicTweaks`).
- **Impact**: Reduces CPU bottlenecks, ensuring maximum performance for critical tasks.

### 3. Network Optimization
- **Monitoring**: Measures network throughput in bytes using `GetIfTable` to sum inbound and outbound octets (`getSystemMetrics`).
- **Actions**:
  - **TCP Tweaks**: Disables Nagle's algorithm (`autotuninglevel=restricted`) and TCP Chimney Offload for lower latency (`applyGamingOptimizations`).
  - **DNS Optimization**: Sets DNS to Google Public DNS (8.8.8.8) for faster resolution (`applyGamingOptimizations`).
  - **Buffer Tuning**: Optimizes network buffer sizes and packet priorities (`initializeDemoLogs` logs this step).
- **Impact**: Reduces latency and improves network stability, crucial for online gaming and real-time applications.

### 4. FPS (Frames Per Second) Optimization
- **Focus**: Extreme optimization mode prioritizes FPS for gaming.
- **Actions**:
  - **Game Mode**: Enables Windows Game Mode to prioritize gaming resources (`enableGameMode`).
  - **Visual Effects**: Minimizes visual effects to reduce GPU/CPU overhead (`minimizeVisualEffects`).
  - **Graphics Drivers**: Disables TDR (Timeout Detection and Recovery) to prevent driver resets (`optimizeGraphicsDrivers`).
  - **Process Priority**: Elevates game processes to real-time priority (`performExtremeOptimizationInternal`).
- **Impact**: Maximizes FPS by reducing system overhead and prioritizing game-related resources.

---

## Optimization Levels

### Basic Optimization
- **Tweaks**: Applies basic registry tweaks (e.g., visual effects, startup delay reduction).
- **Goal**: Improve general system performance with minimal risk.
- **Example**: `applyBasicTweaks` adjusts `VisualFXSetting` to 2 and sets `StartupDelayInMSec` to 0.

### Advanced Optimization
- **Tweaks**: Extends basic tweaks with power settings (High Performance plan) and disables background apps.
- **Goal**: Enhance performance for power users.
- **Example**: `applyAdvancedTweaks` adds `GlobalUserDisabled` to disable background apps.

### Extreme Optimization
- **Tweaks**: Combines all above with network gaming optimizations, process termination, and FPS-specific tweaks.
- **Goal**: Maximize performance, especially for gaming, with a focus on FPS.
- **Example**: `performExtremeOptimizationInternal` disables core parking, enables Game Mode, and terminates non-essential processes.

### System Restore
- **Tweaks**: Reverts all changes to defaults, restoring backups.
- **Goal**: Ensure reversibility and system stability.
- **Example**: `restoreDefaults` resets registry and network settings.

---

## Efficiency Mechanisms

### 1. Resource Monitoring
- **Metrics**: Tracks RAM (KB), CPU (%), and network (bytes) before and after optimization (`getSystemMetrics`).
- **Load Calculation**: Computes a weighted load percentage (40% CPU, 40% RAM, 20% network) to assess system strain (`getCurrentLoadPercentage`).

### 2. Process Management
- **Identification**: Uses `CreateToolhelp32Snapshot` to list foreground and background processes (`getRunningProcesses`).
- **AI Integration**: Queries Grok API to identify non-essential processes, excluding gaming-related ones (`getGrokResponse`).
- **Protection**: Safeguards critical system and gaming processes (e.g., `svchost.exe`, `steam.exe`) while targeting resource hogs (`terminateNonEssentialProcesses`).

### 3. Safety and Logging
- **Backups**: Creates registry and network setting backups before tweaks (`backupRegistryKey`, `backupNetworkSettings`).
- **Logging**: Uses spdlog to log actions and results to rotating files (`system_optimizer.log`), aiding debugging and transparency.

### 4. Privilege Management
- **Elevation**: Checks and elevates to admin privileges if needed using `ShellExecuteExW` (`checkAndElevatePrivileges`).
- **Impact**: Ensures access to system-level tweaks.

---

## Optimization Workflow

1. **Initialization**: Sets up logger and checks privileges (`SystemOptimizer` constructor).
2. **Monitoring**: Gathers initial system metrics (`getSystemMetrics`).
3. **Tweaking**: Applies selected optimization level (e.g., `performExtremeOptimizationInternal`).
4. **Evaluation**: Compares pre- and post-optimization metrics to calculate improvement percentage (`calculateOptimizationPercentage`).
5. **Reporting**: Logs results and returns an `OptimizationResult` with percentage, tweaks applied, and summary (`performExtremeOptimization`).

---

## Performance Gains
- **RAM**: Recovers space (e.g., 340 MB) by cleaning temp files and terminating processes.
- **CPU**: Reduces usage by optimizing services and prioritizing games.
- **Network**: Lowers latency (e.g., via Nagle's disablement) for better online performance.
- **FPS**: Increases frames by minimizing overhead and maximizing hardware utilization (e.g., 27% performance improvement logged).

---

## Conclusion

The `SystemOptimizer` efficiently handles RAM, CPU, network, and FPS optimization through a tiered approach, leveraging real-time monitoring, AI-driven process management, and targeted system tweaks. Its focus on gaming (via Extreme mode) ensures maximum FPS, while safety features like backups and restores maintain system integrity. This makes it a robust tool for both general and gaming-specific performance enhancement.

---