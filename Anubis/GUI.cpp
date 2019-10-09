#include <d3d9.h>
#include <string>
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
    bool triggerbot{ false };
    bool glow{ false };
    bool esp{ false };
    bool misc{ false };
    bool config{ false };
} window;

static void renderMenuBar() noexcept
{
    if (ImGui::BeginMainMenuBar()) {
        ImGui::MenuItem("Triggerbot", nullptr, &window.triggerbot);
        ImGui::MenuItem("Glow", nullptr, &window.glow);
        ImGui::MenuItem("Esp", nullptr, &window.esp);
        ImGui::MenuItem("Misc", nullptr, &window.misc);
        ImGui::MenuItem("Config", nullptr, &window.config);
        ImGui::EndMainMenuBar();
    }
}

static constexpr auto windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

static void keybind(int& key) noexcept
{
    key ? ImGui::Text("[ 0x%x ]", key) : ImGui::TextUnformatted("[ key ]");
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Press any key to change keybind");
        ImGuiIO& io = ImGui::GetIO();
        for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
            if (ImGui::IsKeyPressed(i))
                key = i != VK_ESCAPE ? i : 0;

        for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
            if (ImGui::IsMouseDown(i))
                key = i + (i > 1 ? 2 : 1);
    }
}

static void checkboxedColorPicker(const std::string& name, bool* enable, float* color) noexcept
{
    ImGui::Checkbox(("##" + name).c_str(), enable);
    ImGui::SameLine(0.0f, 5.0f);
    ImGui::PushID(0);
    bool openPopup = ImGui::ColorButton(("##" + name).c_str(), ImColor{ color[0], color[1], color[2] }, ImGuiColorEditFlags_NoTooltip);
    ImGui::PopID();
    ImGui::SameLine(0.0f, 5.0f);
    ImGui::TextUnformatted(name.c_str());
    ImGui::PushID(1);
    if (openPopup)
        ImGui::OpenPopup(("##" + name).c_str());
    if (ImGui::BeginPopup(("##" + name).c_str())) {
        ImGui::PushID(2);
        ImGui::ColorPicker3(("##" + name).c_str(), color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
        ImGui::PopID();
        ImGui::EndPopup();
    }
    ImGui::PopID();
}

static void renderTriggerbotWindow() noexcept
{
    if (window.triggerbot) {
        ImGui::SetNextWindowSize({ 0.0f, 0.0f });
        ImGui::Begin("Triggerbot", &window.triggerbot, windowFlags);
        static int currentCategory{ 0 };
        ImGui::PushItemWidth(110.0f);
        ImGui::PushID(0);
        ImGui::Combo("", &currentCategory, "All\0Pistols\0Heavy\0SMG\0Rifles\0");
        ImGui::PopID();
        ImGui::SameLine();
        static int currentWeapon{ 0 };
        ImGui::PushID(1);
        switch (currentCategory) {
        case 0:
            currentWeapon = 0;
            ImGui::NewLine();
            break;
        case 1: {
            static int currentPistol{ 0 };
            ImGui::Combo("", &currentPistol, "Glock-18\0P2000\0USP-S\0Dual Berettas\0P250\0Tec-9\0Five-Seven\0CZ-75\0Desert Eagle\0Revolver\0");
            currentWeapon = currentPistol + 1;
            break;
        }
        case 2: {
            static int currentHeavy{ 0 };
            ImGui::Combo("", &currentHeavy, "Nova\0XM1014\0Sawed-off\0MAG-7\0M249\0Negev\0");
            currentWeapon = currentHeavy + 11;
            break;
        }
        case 3: {
            static int currentSmg{ 0 };
            ImGui::Combo("", &currentSmg, "Mac-10\0MP9\0MP7\0MP5-SD\0UMP-45\0P90\0PP-Bizon\0");
            currentWeapon = currentSmg + 17;
            break;
        }
        case 4: {
            static int currentRifle{ 0 };
            ImGui::Combo("", &currentRifle, "Galil AR\0Famas\0AK-47\0M4A4\0M4A1-S\0SSG-08\0SG-553\0AUG\0AWP\0G3SG1\0SCAR-20\0");
            currentWeapon = currentRifle + 24;
            break;
        }
        }
        ImGui::PopID();
        ImGui::SameLine();
        ImGui::Checkbox("Enabled", &config.triggerbot[currentWeapon].enabled);
        ImGui::Separator();
        ImGui::Checkbox("On key", &config.triggerbot[currentWeapon].onKey);
        ImGui::SameLine();
        keybind(config.triggerbot[currentWeapon].key);
        ImGui::Checkbox("Friendly fire", &config.triggerbot[currentWeapon].friendlyFire);
        ImGui::Checkbox("Scoped only", &config.triggerbot[currentWeapon].scopedOnly);
        ImGui::Checkbox("Ignore flash", &config.triggerbot[currentWeapon].ignoreFlash);
        ImGui::Checkbox("Ignore smoke", &config.triggerbot[currentWeapon].ignoreSmoke);
        ImGui::PushItemWidth(85.0f);
        ImGui::Combo("Hitgroup", &config.triggerbot[currentWeapon].hitgroup, "All\0Head\0Chest\0Stomach\0Left arm\0Right arm\0Left leg\0Right leg\0");
        ImGui::PushItemWidth(220.0f);
        ImGui::SliderInt("Shot delay", &config.triggerbot[currentWeapon].shotDelay, 0, 250, "%d ms");
        ImGui::InputInt("Min damage", &config.triggerbot[currentWeapon].minDamage);
        config.triggerbot[currentWeapon].minDamage = max(min(config.triggerbot[currentWeapon].minDamage, 250), 0);
        ImGui::Checkbox("Killshot", &config.triggerbot[currentWeapon].killshot);
        ImGui::End();
    }
}

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

static void renderEspWindow() noexcept
{
    if (window.esp) {
        ImGui::SetNextWindowSize({ 0.0f, 0.0f });
        ImGui::Begin("Esp", &window.esp, windowFlags);
        
        static int currentCategory{ 0 };
        ImGui::PushItemWidth(110.0f);
        ImGui::PushID(0);
        ImGui::Combo("", &currentCategory, "Allies\0Enemies\0Weapons");
        ImGui::PopID();
        ImGui::SameLine();

        if (currentCategory < 2) {
            static int currentType{ 0 };
            ImGui::PushID(1);
            ImGui::Combo("", &currentType, "All\0Visible\0Occluded\0");
            ImGui::PopID();
            int currentItem = currentCategory * 3 + currentType;
            ImGui::SameLine();
            ImGui::Checkbox("Enabled", &config.esp.players[currentItem].enabled);
            ImGui::Separator();

            checkboxedColorPicker("Box", &config.esp.players[currentItem].box, config.esp.players[currentItem].boxColor);
        } else {
            ImGui::Checkbox("Enabled", &config.esp.weapon.enabled);
            ImGui::Separator();

            checkboxedColorPicker("Box", &config.esp.weapon.box, config.esp.weapon.boxColor);
            ImGui::SameLine();
            ImGui::Combo("", &config.esp.weapon.boxType, "2D\0""3D\0");
        }
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
        ImGui::Checkbox("Moonwalk", &config.misc.moonwalk);
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
    renderTriggerbotWindow();
    renderGlowWindow();
    renderEspWindow();
    renderMiscWindow();
    renderConfigWindow();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void GUI_invalidateDeviceObjects(void)
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
}

void GUI_createDeviceObjects(void)
{
    ImGui_ImplDX9_CreateDeviceObjects();
}
