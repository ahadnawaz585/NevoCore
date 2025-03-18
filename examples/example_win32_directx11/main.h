#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include <algorithm>  // For std::max
#include <cstdlib>    // For rand()
#include <ctime>
#include <thread>
#include <chrono>
#include <iomanip>
#include <vector>
//#include "examples/example_win32_directx9/imgui_settings.h"

struct PerfIssue {
    enum IssueType { FPS, NETWORK, INPUT_DELAY };
    IssueType type;
    bool optimized;
};

enum class IssueType { FPS, NETWORK, INPUT_DELAY };


void CustomStyleColor() 
{
    ImGuiStyle& s = ImGui::GetStyle();
    ImGuiContext& g = *GImGui;

    s.Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
    s.Colors[ImGuiCol_PopupBg] = ImColor(7, 8, 18, 127);

    s.ChildRounding = 10.f;
    s.WindowRounding = 44.f;
    s.WindowPadding = ImVec2(0, 0);

    s.Colors[ImGuiCol_ChildBg] = ImColor(18, 18, 18, 255);
    s.Colors[ImGuiCol_WindowBg] = ImColor(10, 10,10, 255);

    s.WindowBorderSize = {0.0f};
}


ID3D11ShaderResourceView* Logo = nullptr;

ID3D11ShaderResourceView* bg = nullptr;

ImFont* Days_One = nullptr;

ImFont* Alexandria_Semmi = nullptr;

ImFont* Alexandria_Semmi_1 = nullptr;

ImFont* Alexandria_Semmi_2 = nullptr;

ImFont* Alexandria_Semmi_3 = nullptr;

ImFont* Alexandria_Semmi_4 = nullptr;

ImFont* Alexandria_Regular = nullptr;

ImFont* Alexandria_Regular_1 = nullptr;

ImFont* Alexandria_Regular_2 = nullptr;

ImFont* Alexandria_Regular_3 = nullptr;

ImFont* Alexandria_Medium = nullptr;

ImFont* Icon_Arrow = nullptr;

char input[64] = { "" };

int page = 0;
static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;

int analysing_current_percentage = 0;
int optimizing_current_percentage = 0;



int fps_optimizations_found = 0;
int network_optimizations_found = 0;
int input_delay_optimizations_found = 0;

float fps_optimized_in_percentage = 0.0f;
float network_optimized_in_percentage = 0.0f;
float input_delay_optimized_in_percentage = 0.0f;

static float last_update_time = 0.0f;
static const float UPDATE_INTERVAL = 10.0f;
static struct DisplayMetrics {
    int fps_optimized = 0;
    int network_optimized = 0;
    int input_optimized = 0;
    float fps_percentage = 0.0f;
    float network_percentage = 0.0f;
    float input_percentage = 0.0f;
    float overall_percentage = 0.0f;
} display_metrics;


std::vector<PerfIssue> issues = {
      {PerfIssue::FPS, true},
      {PerfIssue::FPS, false},
      {PerfIssue::FPS, true},
      {PerfIssue::FPS, true},
      {PerfIssue::NETWORK, true},
      {PerfIssue::NETWORK, false},
      {PerfIssue::NETWORK, true},
      {PerfIssue::NETWORK, false},
      {PerfIssue::NETWORK, true},
      {PerfIssue::INPUT_DELAY, true},
      {PerfIssue::INPUT_DELAY, true},
      {PerfIssue::INPUT_DELAY, false},
      {PerfIssue::FPS, true},
      {PerfIssue::NETWORK, true},
      {PerfIssue::INPUT_DELAY, true}
};

float overall_optimized_in_percentage = 0.0f;
double optimized = 50;

const char* current_state = "";

int i = 0;
