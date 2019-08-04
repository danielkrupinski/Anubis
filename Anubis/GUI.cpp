#include <d3d9.h>
#include <Windows.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"

#include "Config.h"
#include "GUI.h"
#include "Hooks.h"

bool isGuiOpen = false;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool GUI_handleInput(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        isGuiOpen = !isGuiOpen;
    }
    if (isGuiOpen && !ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam))
        return true;
    return false;
}

void GUI_init(IDirect3DDevice9* device)
{
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(FindWindowA("Valve001", NULL));
    ImGui_ImplDX9_Init(device);
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 0.0f;
    style.GrabMinSize = 7.0f;
    style.GrabRounding = 5.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 5.0f;
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    static ImWchar ranges[] = { 0x0020, 0x00FF, 0x0100, 0x017f, 0 };
    char buffer[MAX_PATH];
    if (GetWindowsDirectoryA(buffer, MAX_PATH))
        io.Fonts->AddFontFromFileTTF(strcat(buffer, "/Fonts/Tahoma.ttf"), 16.0f, nullptr, ranges);
}

static struct {
    bool glow{ false };
    bool misc{ false };
    bool config{ false };
} window;

static void renderMenuBar() noexcept
{
    if (ImGui::BeginMainMenuBar()) {
        ImGui::MenuItem("Glow", nullptr, &window.glow);
        ImGui::MenuItem("Misc", nullptr, &window.misc);
        ImGui::MenuItem("Config", nullptr, &window.config);
        ImGui::EndMainMenuBar();
    }
}

static constexpr auto windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

static void renderGlowWindow() noexcept
{
    if (window.glow) {
        ImGui::SetNextWindowSize({ 450.0f, 0.0f });
        ImGui::Begin("Glow", &window.glow, windowFlags);

        static int currentCategory{ 0 };
        ImGui::PushItemWidth(110.0f);
        ImGui::PushID(0);
        ImGui::Combo("", &currentCategory, "Allies\0Enemies\0Planting\0Defusing\0Local player\0Weapons\0C4\0Planted C4\0Chickens\0");
        ImGui::PopID();
        static int currentItem{ 0 };
        if (currentCategory <= 3) {
            ImGui::SameLine();
            static int currentType{ 0 };
            ImGui::PushID(1);
            ImGui::Combo("", &currentType, "All\0Visible\0Occluded\0");
            ImGui::PopID();
            currentItem = currentCategory * 3 + currentType;
        } else {
            currentItem = currentCategory + 8;
        }

        ImGui::SameLine();
        ImGui::Checkbox("Enabled", &config.glow[currentItem].enabled);
        ImGui::Separator();
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnOffset(1, 150.0f);
        ImGui::Checkbox("Health based", &config.glow[currentItem].healthBased);
        ImGui::Checkbox("Rainbow", &config.glow[currentItem].rainbow);
        bool openPopup = ImGui::ColorButton("Color", ImVec4{ config.glow[currentItem].color }, ImGuiColorEditFlags_NoTooltip);
        ImGui::SameLine(0.0f, 5.0f);
        ImGui::Text("Color");
        ImGui::PushID(2);
        if (openPopup)
            ImGui::OpenPopup("");
        if (ImGui::BeginPopup("")) {
            ImGui::PushID(3);
            ImGui::ColorPicker3("", config.glow[currentItem].color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
            ImGui::PopID();
            ImGui::EndPopup();
        }
        ImGui::PopID();
        ImGui::NextColumn();
        ImGui::PushItemWidth(220.0f);
        ImGui::SliderFloat("Thickness", &config.glow[currentItem].thickness, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Alpha", &config.glow[currentItem].alpha, 0.0f, 1.0f, "%.2f");
        ImGui::InputInt("Style", &config.glow[currentItem].style, 1, 1);
        config.glow[currentItem].style = max(min(config.glow[currentItem].style, 3), 0);
        ImGui::End();
    }
}

static void renderMiscWindow() noexcept
{
    if (window.misc) {
        ImGui::SetNextWindowSize({ 0.0f, 0.0f });
        ImGui::Begin("Misc", &window.misc, windowFlags);
        ImGui::Checkbox("Auto strafe", &config.misc.autostrafe);
        ImGui::Checkbox("Bunnyhop", &config.misc.bunnyhop);
        if (ImGui::Button("Unhook"))
            Hooks_restore();
        ImGui::End();
    }
}

static void renderConfigWindow() noexcept
{
    if (window.config) {
        ImGui::SetNextWindowSize({ 290.0f, 190.0f });
        ImGui::Begin("Config", &window.config, windowFlags);

        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnOffset(1, 170.0f);

        ImGui::PushItemWidth(160.0f);

        static int currentConfig = -1;

        if (static_cast<size_t>(currentConfig) >= config.count)
            currentConfig = -1;

        static char buffer[16];

        if (ImGui::ListBox("", &currentConfig, [](void* data, int idx, const char** out_text) {
            *out_text = static_cast<const char**>(data)[idx];
            return true;
        }, config.names, config.count, 5) && currentConfig != -1)
            strcpy(buffer, config.names[currentConfig]);

        ImGui::PushID(0);
        if (ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
            if (currentConfig != -1)
                Config_rename(currentConfig, buffer);
        }
        ImGui::PopID();
        ImGui::NextColumn();

        ImGui::PushItemWidth(100.0f);

        if (ImGui::Button("Create config", { 100.0f, 25.0f }))
            Config_add(buffer);

        if (ImGui::Button("Reset config", { 100.0f, 25.0f }))
            Config_reset();

        if (currentConfig != -1) {
            if (ImGui::Button("Load selected", { 100.0f, 25.0f })) {
                Config_load(currentConfig);
            }
            if (ImGui::Button("Save selected", { 100.0f, 25.0f }))
                Config_save(currentConfig);
            if (ImGui::Button("Delete selected", { 100.0f, 25.0f }))
                Config_remove(currentConfig);
        }
        ImGui::End();
    }
}

void GUI_render()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    renderMenuBar();
    renderGlowWindow();
    renderMiscWindow();
    renderConfigWindow();

    ImGui::ShowDemoWindow();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}
