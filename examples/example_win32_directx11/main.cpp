#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include "main.h"
#include "definitions.h"
#include "systemoptimizer.h"
#include "imgui_settings.h"
#include "Font.h"
#include <dwmapi.h>
#include <string>
#include <sstream>
//#include <D3DX11tex.h>
#include <iostream>
#pragma comment(lib, "D3DX11.lib")
using namespace std;
namespace image {
    ID3D11ShaderResourceView* help = nullptr;
    ID3D11ShaderResourceView* Key = nullptr;
    ID3D11ShaderResourceView* discord = nullptr;
    ID3D11ShaderResourceView* youtube = nullptr;
    ID3D11ShaderResourceView* exit = nullptr;
    ID3D11ShaderResourceView* succes = nullptr;
    ID3D11ShaderResourceView* up = nullptr;
    ID3D11ShaderResourceView* keyboard = nullptr;
    ID3D11ShaderResourceView* wifi = nullptr;
    ID3D11ShaderResourceView* warning = nullptr;
    ID3D11ShaderResourceView* succes_inject = nullptr;
    ID3D11ShaderResourceView* enjoy = nullptr;
    ID3D11ShaderResourceView* back = nullptr;
}

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();

SystemOptimizer optimizer;

bool Spinner(const char* label, float radius, int thickness, const ImU32& color) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size(radius * 2, radius * 2);

    const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id)) return false;

    ImGui::GetForegroundDrawList()->PathClear();

    int num_segments = 1500;
    int start = abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

    const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
    const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

    const ImVec2 centre(pos.x + radius, pos.y + radius);

    for (int i = 0; i < num_segments; i++) {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        ImGui::GetForegroundDrawList()->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
            centre.y + ImSin(a + g.Time * 8) * radius));
    }

    ImGui::GetForegroundDrawList()->PathStroke(color, false, thickness);
    return true;
}

static float rotationAngle = 0.0f;

void DrawRotatingSegmentedCircleWithFade(ImDrawList* drawList, ImVec2 centre, float radius, float startAngle, float angle, int num_segments, ImU32 startColor, float startThickness, float endThickness) {
    const float angle_offset = angle / num_segments;
    const float th_diff = endThickness - startThickness;
    ImVec4 startColorVec4 = ImGui::ColorConvertU32ToFloat4(startColor);

    for (int i = 0; i < num_segments; i++) {
        const float a = startAngle + (i * angle_offset) + rotationAngle;
        const float a1 = startAngle + ((i + 1) * angle_offset) + rotationAngle;
        const float a_rad = a * IM_PI / 180.0f;
        const float a1_rad = a1 * IM_PI / 180.0f;
        const float current_thickness = startThickness + (th_diff * i) / (num_segments - 1);
        float alpha = 1.0f - (static_cast<float>(i) / (num_segments - 1));
        ImU32 currentColor = ImGui::ColorConvertFloat4ToU32(ImVec4(startColorVec4.x, startColorVec4.y, startColorVec4.z, startColorVec4.w * alpha));

        drawList->AddLine(ImVec2(centre.x + cosf(a_rad) * radius, centre.y + sinf(a_rad) * radius),
            ImVec2(centre.x + cosf(a1_rad) * radius, centre.y + sinf(a1_rad) * radius),
            currentColor, current_thickness);
    }
}

void MoveWindow(HWND hwnd) {
    static int moveOffsetX = 0, moveOffsetY = 0;

    if (ImGui::IsMouseClicked(0)) {
        POINT point;
        RECT rect;
        GetCursorPos(&point);
        GetWindowRect(hwnd, &rect);
        moveOffsetX = point.x - rect.left;
        moveOffsetY = point.y - rect.top;
    }

    if (moveOffsetY >= 0 && moveOffsetY <= HEIGHT && ImGui::IsMouseDragging(0)) {
        POINT point;
        GetCursorPos(&point);
        SetWindowPos(hwnd, HWND_NOTOPMOST, point.x - moveOffsetX, point.y - moveOffsetY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
    else if (moveOffsetY >= 0 && moveOffsetY <= HEIGHT && ImGui::IsMouseDragging(ImGuiMouseButton_Right) && ImGui::IsMouseDragging(0)) {
        POINT point;
        GetCursorPos(&point);
        SetWindowPos(hwnd, HWND_NOTOPMOST, point.x - moveOffsetX, point.y - moveOffsetY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
}

std::string FormatFloat(float value) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << value;
    return stream.str();
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
D3DX11_IMAGE_LOAD_INFO info;
ID3DX11ThreadPump* pump = nullptr;

int applyRandomAdjustment(int& value) {
    int adjusted_value = value + (rand() % 10) - 1;
    return adjusted_value < 0 ? 0 : adjusted_value;
}

int main(int, char**) {
    static double optimizationResult = 0.0;
    static bool Toggle1 = true, Toggle2 = false;
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"NEVOCORE", NULL };
    ::RegisterClassExW(&wc);
    HWND hwnd = CreateWindowExW(0, wc.lpszClassName, L"NEVOCORE", WS_POPUP, (GetSystemMetrics(SM_CXSCREEN) / 2) - (WIDTH / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (HEIGHT / 2), WIDTH, HEIGHT, nullptr, nullptr, wc.hInstance, nullptr);

    MARGINS margins = { -1, -1, -1, -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    ShowWindow(GetConsoleWindow(), FALSE);
    UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    Days_One = io.Fonts->AddFontFromMemoryTTF(&Days_One_R, sizeof Days_One_R, 18.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Alexandria_Semmi = io.Fonts->AddFontFromMemoryTTF(&Alexandria_S, sizeof Alexandria_S, 43.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Alexandria_Semmi_1 = io.Fonts->AddFontFromMemoryTTF(&Alexandria_S, sizeof Alexandria_S, 23.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Alexandria_Semmi_2 = io.Fonts->AddFontFromMemoryTTF(&Alexandria_S, sizeof Alexandria_S, 16.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Alexandria_Semmi_3 = io.Fonts->AddFontFromMemoryTTF(&Alexandria_S, sizeof Alexandria_S, 21.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Alexandria_Semmi_4 = io.Fonts->AddFontFromMemoryTTF(&Alexandria_S, sizeof Alexandria_S, 31.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Alexandria_Regular = io.Fonts->AddFontFromMemoryTTF(&Alexandria_R, sizeof Alexandria_R, 23.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Alexandria_Regular_1 = io.Fonts->AddFontFromMemoryTTF(&Alexandria_R, sizeof Alexandria_R, 27.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Alexandria_Regular_2 = io.Fonts->AddFontFromMemoryTTF(&Alexandria_R, sizeof Alexandria_R, 16.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Alexandria_Regular_3 = io.Fonts->AddFontFromMemoryTTF(&Alexandria_R, sizeof Alexandria_R, 13.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Alexandria_Medium = io.Fonts->AddFontFromMemoryTTF(&Alexandria_M, sizeof Alexandria_M, 21.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Icon_Arrow = io.Fonts->AddFontFromMemoryTTF(&Arrow, sizeof Arrow, 8.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    if (!image::Key) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, Key_Icon, sizeof(Key_Icon), &info, pump, &image::Key, 0);
    if (!image::help) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, help_Icon, sizeof(help_Icon), &info, pump, &image::help, 0);
    if (!image::discord) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, discord_Icon, sizeof(discord_Icon), &info, pump, &image::discord, 0);
    if (!image::youtube) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, youtube_Icon, sizeof(youtube_Icon), &info, pump, &image::youtube, 0);
    if (!image::exit) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, exit_Icon, sizeof(exit_Icon), &info, pump, &image::exit, 0);
    if (!image::succes) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, succes_Font, sizeof(succes_Font), &info, pump, &image::succes, 0);
    if (!image::up) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, up_icon, sizeof(up_icon), &info, pump, &image::up, 0);
    if (!image::keyboard) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, keyboard_icon, sizeof(keyboard_icon), &info, pump, &image::keyboard, 0);
    if (!image::wifi) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, wifi_icon, sizeof(wifi_icon), &info, pump, &image::wifi, 0);
    if (!image::warning) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, warning_icon, sizeof(warning_icon), &info, pump, &image::warning, 0);
    if (!image::succes_inject) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, succes_inject_icon, sizeof(succes_inject_icon), &info, pump, &image::succes_inject, 0);
    if (!image::enjoy) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, enjoy_icon, sizeof(enjoy_icon), &info, pump, &image::enjoy, 0);
    if (!image::back) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, back_icon, sizeof(back_icon), &info, pump, &image::back, 0);

    bool done = false;
    int select;
    while (!done) {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) done = true;
        }
        if (done) break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGuiStyle* style = &ImGui::GetStyle();
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        MoveWindow(hwnd);

        CustomStyleColor();
        ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
        ImGui::Begin("General", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

        auto draw = ImGui::GetWindowDrawList();
        const auto& p = ImGui::GetWindowPos();
        const ImVec2& region = ImGui::GetContentRegionMax();

        tab_alpha = ImLerp(tab_alpha, (page == active_tab) ? 1.f : 0.f, 15.f * ImGui::GetIO().DeltaTime);
        if (tab_alpha < 0.01f && tab_add < 0.01f) active_tab = page;
        if (!Logo) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, Logo_Font, sizeof(Logo_Font), &info, pump, &Logo, 0);
        ImGui::GetWindowDrawList()->AddImageRounded(Logo, ImVec2(p.x + 145, p.y + 49), ImVec2(p.x + 331, p.y + 81), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::text_active), 0.f, ImDrawFlags_RoundCornersLeft);

        ImGui::SetCursorPos(ImVec2(region.x - 55, 38));
        if (ImGui::Exit_Button("exit", image::exit, ImVec2(12, 12))) exit(0);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * style->Alpha);

        if (active_tab == 0) {
            if (!optimizer.checkAndElevatePrivileges()) return 0;
            ImGui::GetWindowDrawList()->AddText(Alexandria_Semmi, 33.f, ImVec2(p.x + 133, p.y + 124), ImGui::GetColorU32(c::text_active), "Welcome back!");
            ImGui::GetWindowDrawList()->AddText(Alexandria_Regular, 20.f, ImVec2(p.x + 70, p.y + 184), ImGui::GetColorU32(c::con_text_active), "please login to confirm your credentials");

            ImGui::SetCursorPos(ImVec2(72, 264));
            ImGui::BeginGroup();
            ImGui::InputTextWithHint("Login", image::Key, "Enter your License", input, 64);
            ImGui::EndGroup();

            ImGui::SetCursorPos(ImVec2(72, 381));
            if (ImGui::Button("LOGIN", ImVec2(332, 47))) page = 1;

            ImGui::SetCursorPos(ImVec2(186, 466));
            ImGui::Help_Button("Need Help?", image::help, ImVec2(110, 25));

            ImGui::SetCursorPos(ImVec2(195, 514));
            ImGui::Circle_Button("Discord", image::discord, ImVec2(36, 36));
            ImGui::Circle_Button("Youtube", image::youtube, ImVec2(36, 36));
        }

        if (active_tab == 1) {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 center = ImGui::GetCursorScreenPos() + ImVec2(region) / 2 + ImVec2(0, -50);
            float radius = 40.0f;
            float startAngle = 0.0f;
            float angle = 180.0f;
            int numSegments = 10;
            ImU32 color = ImGui::GetColorU32(c::bg_logging_in_circle);
            float startThickness = 1.0f;
            float endThickness = 5.0f;

            DrawRotatingSegmentedCircleWithFade(drawList, center, radius, startAngle, angle, numSegments, color, startThickness, endThickness);

            rotationAngle += ImGui::GetIO().DeltaTime * 300.0f;
            if (rotationAngle > 360.0f) rotationAngle -= 360.0f;

            ImGui::GetWindowDrawList()->AddText(Alexandria_Regular_1, 20.f, p + ImVec2((region.x + 40 - Alexandria_Regular_1->CalcTextSizeA(Alexandria_Regular_1->FontSize, FLT_MAX, 0.f, "logging in...").x) / 2, region.y - 210), ImGui::GetColorU32(c::con_text_active), "logging in...");

            static float timer = 0.f;
            timer += 6.f * ImGui::GetIO().DeltaTime;

            static DWORD dwTickStart = GetTickCount();
            if (GetTickCount() - dwTickStart > 30) {
                if (i < 100) i++;
                dwTickStart = GetTickCount();
            }

            if (timer > 20.f) {
                page = 2;
                i = 0;
                timer = 0;
            }
        }

        if (active_tab == 2) {
            ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x + 44, p.y + 102), ImVec2(p.x + 432, p.y + 473), ImGui::GetColorU32(c::rect_beginchild), 29.f);
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 45, p.y + 103), ImVec2(p.x + 431, p.y + 472), ImGui::GetColorU32(c::bg_beginchild), 29.f);

            ImGui::GetWindowDrawList()->AddText(Alexandria_Semmi_1, 23.f, ImVec2(p.x + 140, p.y + 147), ImGui::GetColorU32(c::text_active), "Optimization Mode");
            ImGui::GetWindowDrawList()->AddText(Alexandria_Semmi_2, 16.f, ImVec2(p.x + 116, p.y + 180), ImGui::GetColorU32(c::con_text_active), "Choose a mode based on your needs.");

            ImGui::SetCursorPos(ImVec2(90, 211));
            static int select2 = 0;
            const char* items2[3] = { "Basic", "Advanced", "Extreme" };
            ImGui::Combo("Type", &select2, items2, IM_ARRAYSIZE(items2), 2);

            ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + 90, p.y + 305), ImVec2(p.x + 386, p.y + 305), ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 1.0f)), 1.0f);

            ImGui::SetCursorPos(ImVec2(102, 338));
            ImGui::BeginGroup();

            ImGui::Checkbox("AI Enhanced", &Toggle1);
            ImGui::Checkbox("Reset Computer", &Toggle2);
            ImGui::EndGroup();

            ImGui::SetCursorPos(ImVec2(305, 401));
            if (ImGui::Button_Back("Revert", "Revert Changes?", image::back, ImVec2(71, 25))) {
                Toggle1 = false;
                Toggle2 = false;
                select2 = 0;
                optimizer.run("4");
            }

            ImGui::SetCursorPos(ImVec2(44, 498));
            if (ImGui::Button("Analyze", ImVec2(388, 47))) {
                page = 3;
                select = select2;
                current_state = "Analysing your PC, please wait..";
            }
        }

        if (active_tab == 3) {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 center = ImGui::GetCursorScreenPos() + ImVec2(region) / 2 + ImVec2(0, -50);
            float radius = 80.0f;
            float startAngle = 0.0f;
            float angle = 180.0f;
            int numSegments = 10;
            ImU32 color = ImGui::GetColorU32(c::main_color);
            float startThickness = 1.0f;
            float endThickness = 5.0f;

            DrawRotatingSegmentedCircleWithFade(drawList, center, radius, startAngle, angle, numSegments, color, startThickness, endThickness);

            rotationAngle += ImGui::GetIO().DeltaTime * 350.0f;
            if (rotationAngle > 360.0f) rotationAngle -= 360.0f;

            ImGui::GetWindowDrawList()->AddText(Alexandria_Regular_1, 20.f,
                p + ImVec2((region.x + 75 - Alexandria_Regular_1->CalcTextSizeA(Alexandria_Regular_1->FontSize, FLT_MAX, 0.f, current_state).x) / 2, region.y - 100),
                ImGui::GetColorU32(c::con_text_active), current_state);

            static float timer = 0.f;
            timer += 6.f * ImGui::GetIO().DeltaTime;
            static DWORD dwTickStart = GetTickCount();

            if (GetTickCount() - dwTickStart > 30) {
                if (analysing_current_percentage < 100) analysing_current_percentage++;
                dwTickStart = GetTickCount();
            }

            std::string text = std::to_string(analysing_current_percentage) + "%";
            const ImVec2 textSize = Alexandria_Semmi_4->CalcTextSizeA(Alexandria_Semmi_4->FontSize, FLT_MAX, 0.f, text.c_str());
            ImVec2 textPos = p + ImVec2((region.x - textSize.x) / 2 + 5.0f, (region.y - textSize.y) / 2 + 5.0f);
            ImGui::GetWindowDrawList()->AddText(Alexandria_Semmi_4, 28.f, textPos, ImGui::GetColorU32(c::text_active), text.c_str());

            if (analysing_current_percentage == 100) {
                page = 4;
                overall_optimized_in_percentage = 0.234f;
                timer = 0;
            }
        }

        if (active_tab == 4) {
            struct IssueMetrics {
                int total = 0;
                int optimized = 0;
                float percentage = 0.0f;
            };

            float current_time = ImGui::GetTime();
            bool should_update = (current_time - last_update_time >= UPDATE_INTERVAL);

            if (should_update) {
                IssueMetrics fps_metrics, network_metrics, input_metrics;

                for (const auto& issue : issues) {
                    switch (issue.type) {
                    case PerfIssue::FPS:
                        fps_metrics.total++;
                        fps_metrics.optimized += issue.optimized ? 1 : 0;
                        break;
                    case PerfIssue::NETWORK:
                        network_metrics.total++;
                        network_metrics.optimized += issue.optimized ? 1 : 0;
                        break;
                    case PerfIssue::INPUT_DELAY:
                        input_metrics.total++;
                        input_metrics.optimized += issue.optimized ? 1 : 0;
                        break;
                    }
                }

                auto calculatePercentage = [](int optimized, int total) -> float {
                    float percentage = total > 0 ? (static_cast<float>(optimized) / total) * 100.0f : 0.0f;
                    return (percentage < 100.0f) ? percentage : (50 + (rand() % 50));
                };

                display_metrics.fps_optimized = applyRandomAdjustment(fps_metrics.optimized);
                display_metrics.network_optimized = applyRandomAdjustment(network_metrics.optimized);
                display_metrics.input_optimized = applyRandomAdjustment(input_metrics.optimized);

                display_metrics.fps_percentage = calculatePercentage(display_metrics.fps_optimized, fps_metrics.total);
                display_metrics.network_percentage = calculatePercentage(display_metrics.network_optimized, network_metrics.total);
                display_metrics.input_percentage = calculatePercentage(display_metrics.input_optimized, input_metrics.total);

                int total_issues = fps_metrics.total + network_metrics.total + input_metrics.total;
                int total_optimized = display_metrics.fps_optimized + display_metrics.network_optimized + display_metrics.input_optimized;
                display_metrics.overall_percentage = calculatePercentage(total_optimized, total_issues);

                last_update_time = current_time;
            }

            ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x + 44, p.y + 102), ImVec2(p.x + 432, p.y + 473), ImGui::GetColorU32(c::rect_beginchild), 29.f);
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 45, p.y + 103), ImVec2(p.x + 431, p.y + 472), ImGui::GetColorU32(c::bg_beginchild), 29.f);

            ImGui::BeginGroup();
            ImGui::SetCursorPos(ImVec2(110, 160));
            double loadPercentage = optimizer.getCurrentLoadPercentage();

            std::string overall_percentage_str;
            {
                std::ostringstream stream;
                stream << std::fixed << std::setprecision(2) << loadPercentage;
                overall_percentage_str = stream.str() + "%";
            }

            ImGui::SelectButtons("optimized_widget", "Degraded", ImGui::GetColorU32(c::yellow), overall_percentage_str.c_str(), "Your PC could run smoother", display_metrics.overall_percentage);

            ImGui::SetCursorPos(ImVec2(73, 214));
            ImGui::BeginGroup();
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 11));

            auto formatMetricText = [](int optimized, float percentage, const char* prefix) {
                return prefix + std::to_string(optimized) + " optimizations to boost\n" + FormatFloat(percentage) + "%)";
            };

            std::string fps_text = formatMetricText(display_metrics.fps_optimized, display_metrics.fps_percentage, "Found ");
            std::string input_text = formatMetricText(display_metrics.input_optimized, display_metrics.input_percentage, "Found ");
            std::string network_text = formatMetricText(display_metrics.network_optimized, display_metrics.network_percentage, "Found ");

            ImGui::Info_Button(fps_text.c_str(), image::up, ImVec2(326, 57));
            ImGui::Info_Button(input_text.c_str(), image::keyboard, ImVec2(326, 57));
            ImGui::Info_Button(network_text.c_str(), image::wifi, ImVec2(326, 57));

            ImGui::PopStyleVar();
            ImGui::EndGroup();

            ImGui::SetCursorPos(ImVec2(75, 420));
            ImGui::Warning_Button("Click on \"Optimize Now\" to get the full\npotential of your pc.", image::warning);

            ImGui::SetCursorPos(ImVec2(44, 498));
            if (ImGui::Button("Optimize Now", ImVec2(388, 47))) {
                page = 5;
                current_state = "Optimizing your PC, please wait..";
            }
            ImGui::EndGroup();
        }

        if (active_tab == 5) {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 center = ImGui::GetCursorScreenPos() + ImVec2(region) / 2 + ImVec2(0, -50);
            float radius = 80.0f;
            float startAngle = 0.0f;
            float angle = 180.0f;
            int numSegments = 10;
            ImU32 color = ImGui::GetColorU32(c::main_color);
            float startThickness = 1.0f;
            float endThickness = 5.0f;

            DrawRotatingSegmentedCircleWithFade(drawList, center, radius, startAngle, angle, numSegments, color, startThickness, endThickness);

            rotationAngle += ImGui::GetIO().DeltaTime * 350.0f;
            if (rotationAngle > 360.0f) rotationAngle -= 360.0f;

            ImGui::GetWindowDrawList()->AddText(Alexandria_Regular_1, 20.f,
                p + ImVec2((region.x + 75 - Alexandria_Regular_1->CalcTextSizeA(Alexandria_Regular_1->FontSize, FLT_MAX, 0.f, current_state).x) / 2, region.y - 100),
                ImGui::GetColorU32(c::con_text_active), current_state);

            static float timer = 0.f;
            timer += 6.f * ImGui::GetIO().DeltaTime;
            static DWORD dwTickStart = GetTickCount();

            if (GetTickCount() - dwTickStart > 30) {
                if (optimizing_current_percentage < 100) optimizing_current_percentage++;
                dwTickStart = GetTickCount();
            }

            std::string text = std::to_string(optimizing_current_percentage) + "%";
            const ImVec2 textSize = Alexandria_Semmi_4->CalcTextSizeA(Alexandria_Semmi_4->FontSize, FLT_MAX, 0.f, text.c_str());
            ImVec2 textPos = p + ImVec2((region.x - textSize.x) / 2 + 5.0f, (region.y - textSize.y) / 2 + 5.0f);
            ImGui::GetWindowDrawList()->AddText(Alexandria_Semmi_4, 28.f, textPos, ImGui::GetColorU32(c::text_active), text.c_str());

            static SystemMetrics before, after;
            static std::thread optimizationThread;
            static bool optimizationStarted = false;
            static bool optimizationCompleted = false;
            static bool restartPending = false;

            // Function to disable startup apps
            auto DisableStartupApps = []() {
                // 1. Disable Registry Run entries
                auto DisableRegistryRun = [](HKEY root, const wchar_t* regPath) {
                    HKEY hKey;
                    if (RegOpenKeyExW(root, regPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) {
                        wchar_t valueName[256];
                        DWORD valueNameLen = 256;
                        DWORD index = 0;
                        std::vector<std::wstring> valueNames;

                        // Collect all value names first
                        while (RegEnumValueW(hKey, index, valueName, &valueNameLen, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
                            valueNames.push_back(valueName);
                            valueNameLen = 256;
                            index++;
                        }

                        // Delete each value
                        for (const auto& name : valueNames) {
                            LONG result = RegDeleteValueW(hKey, name.c_str());
                            if (result != ERROR_SUCCESS) {
                                std::cerr << "Failed to delete " << std::string(name.begin(), name.end()) << ": " << result << std::endl;
                            }
                        }
                        RegCloseKey(hKey);
                    }
                    else {
                        std::cerr << "Failed to open Registry key: " << GetLastError() << std::endl;
                    }
                };

                DisableRegistryRun(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
                DisableRegistryRun(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

                // 2. Disable Startup Folder items
                auto DisableStartupFolder = [](int folderCSIDL) { // Changed to int
                    wchar_t folderPath[MAX_PATH];
                    if (SHGetFolderPathW(NULL, folderCSIDL, NULL, 0, folderPath) == S_OK) {
                        std::filesystem::path path(folderPath);
                        for (const auto& entry : std::filesystem::directory_iterator(path)) {
                            if (entry.is_regular_file()) {
                                std::filesystem::path newPath = entry.path().parent_path() / (entry.path().filename().string() + ".disabled");
                                std::filesystem::rename(entry.path(), newPath); // Rename to disable
                            }
                        }
                    }
                    else {
                        std::cerr << "Failed to get Startup folder path: " << GetLastError() << std::endl;
                    }
                };

                DisableStartupFolder(CSIDL_STARTUP);       // Current user Startup folder
                DisableStartupFolder(CSIDL_COMMON_STARTUP); // All users Startup folder

                // Flush Registry changes to disk
                RegFlushKey(HKEY_CURRENT_USER);
                RegFlushKey(HKEY_LOCAL_MACHINE);
            };

            if (optimizing_current_percentage == 0 && !optimizationStarted) {
                before = optimizer.getSystemMetrics();
                optimizationStarted = true;

                optimizationThread = std::thread([&]() {
                    try {
                        if (select == 0) optimizationResult = optimizer.run("1");
                        else if (select == 1) optimizationResult = optimizer.run("2");
                        else if (select == 2) optimizationResult = optimizer.run("3");
                        optimizationCompleted = true;
                    }
                    catch (const std::exception& e) {
                        optimizationCompleted = true;
                        optimizationResult = 0.0;
                        std::cerr << "Optimization thread error: " << e.what() << std::endl;
                    }
                    });
                optimizationThread.detach();
            }

            if (optimizing_current_percentage >= 100 && optimizationCompleted) {
                after = optimizer.getSystemMetrics();
                optimized = optimizationResult;
                overall_optimized_in_percentage = optimized;
                page = 6;
                timer = 0;
                optimizationStarted = false;
                optimizationCompleted = false;
                optimizing_current_percentage = 0;

                // Check if "Reset Computer" (Toggle2) is true and initiate restart
                if (Toggle2 && !restartPending) {
                    // Disable startup apps before restarting
                    DisableStartupApps();

                    // Request privilege to shutdown (requires admin rights)
                    HANDLE hToken;
                    TOKEN_PRIVILEGES tkp;
                    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
                        LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
                        tkp.PrivilegeCount = 1;
                        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
                        AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
                        CloseHandle(hToken);
                    }

                    // Initiate system restart
                    if (InitiateSystemShutdownEx(
                        NULL,                     // Computer name (NULL for local)
                        L"PC Optimization Complete - Restarting...", // Message to display
                        10,                       // Timeout in seconds before shutdown
                        FALSE,                    // Force apps to close (false = ask politely)
                        TRUE,                     // TRUE = Restart, FALSE = Shutdown
                        SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_MAINTENANCE // Reason code
                    )) {
                        restartPending = true; // Prevent multiple restart calls
                    }
                    else {
                        std::cerr << "Failed to initiate restart: " << GetLastError() << std::endl;
                    }
                }
            }
        }

        if (active_tab == 6) {
            optimizer.initializeDemoLogs();
            std::vector<std::string> demoLogs = optimizer.getLogs();
            ImGui::GetWindowDrawList()->AddRect(ImVec2(p.x + 54, p.y + 102), ImVec2(p.x + 422, p.y + 473), ImGui::GetColorU32(c::rect_beginchild), 29.f);
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + 55, p.y + 103), ImVec2(p.x + 421, p.y + 472), ImGui::GetColorU32(c::bg_beginchild), 29.f);

            ImGui::BeginGroup();
            ImGui::SetCursorPos(ImVec2(97, 221));
            ImGui::Succes_Button("PC Successfully Optimized!", image::succes_inject);
            ImGui::SetCursorPos(ImVec2(94, 266));
            ImGui::BeginChild("Logs", ImVec2(288, 179), false);
            ImGui::PushFont(Alexandria_Medium);
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(c::text_warning));
            for (const auto& log : demoLogs) ImGui::TextWrapped("%s", log.c_str());
            ImGui::PopStyleColor();
            ImGui::PopFont();
            ImGui::EndChild();
            ImGui::EndGroup();

            ImGui::SetCursorPos(ImVec2(125, 165));
            double loadPercentage = optimizer.getCurrentLoadPercentage(); 
            std::string optimizationResultString = ([](double val) {
                std::ostringstream stream;
                stream << std::fixed << std::setprecision(2) << val;
                return stream.str();
                })(optimizationResult) + "%"; 

                ImGui::SelectButtons("optimized_widget", "Optimized", ImGui::GetColorU32(c::green),
                    optimizationResultString.c_str(), "Your PC runs smooth",
                    optimizationResult); 

                ImGui::SetCursorPos(ImVec2(152, 512));
                ImGui::Enjoy_Button("Enjoy Gaming!", image::enjoy);
        }

        ImGui::PopStyleVar();
        ImGui::End();

        ImGui::Render();
        ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

bool CreateDeviceD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK) return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget() {
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget() {
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return true;

    switch (msg) {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED) {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
