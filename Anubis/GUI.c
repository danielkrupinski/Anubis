#include <d3d9.h>
#include <Windows.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "imgui/cimgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"

#include "Config.h"
#include "GUI.h"
#include "Hooks.h"

bool isGuiOpen = false;

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
    igCreateContext(NULL);
    ImGui_ImplWin32_Init(FindWindowA("Valve001", NULL));
    ImGui_ImplDX9_Init(device);
    igStyleColorsDark(NULL);
    ImGuiStyle* style = igGetStyle();
    style->WindowRounding = 5.0f;
    style->WindowBorderSize = 0.0f;
    style->ChildBorderSize = 0.0f;
    style->GrabMinSize = 7.0f;
    style->GrabRounding = 5.0f;
    style->FrameRounding = 5.0f;
    style->PopupRounding = 5.0f;
    ImGuiIO* io = igGetIO();
    io->IniFilename = NULL;
    io->LogFilename = NULL;

    static ImWchar ranges[] = { 0x0020, 0x00FF, 0x0100, 0x017f, 0 };
    char buffer[MAX_PATH];
    if (GetWindowsDirectoryA(buffer, MAX_PATH))
        ImFontAtlas_AddFontFromFileTTF(io->Fonts, strcat(buffer, "/Fonts/Tahoma.ttf"), 16.0f, NULL, ranges);
}

static struct {
    bool triggerbot;
    bool glow;
    bool esp;
    bool misc;
    bool config;
} window;

static void renderMenuBar()
{
    if (igBeginMainMenuBar()) {
        igMenuItemBoolPtr("Triggerbot", NULL, &window.triggerbot, true);
        igMenuItemBoolPtr("Glow", NULL, &window.glow, true);
        igMenuItemBoolPtr("Esp", NULL, &window.esp, true);
        igMenuItemBoolPtr("Misc", NULL, &window.misc, true);
        igMenuItemBoolPtr("Config", NULL, &window.config, true);
        igEndMainMenuBar();
    }
}

static const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))

static void keybind(int* key)
{
    *key ? igText("[ 0x%x ]", *key) : igTextUnformatted("[ key ]", NULL);
    if (igIsItemHovered(ImGuiHoveredFlags_None)) {
        igSetTooltip("Press any key to change keybind");
        ImGuiIO* io = igGetIO();
        for (int i = 0; i < IM_ARRAYSIZE(io->KeysDown); i++)
            if (igIsKeyPressed(i, false))
                *key = i != VK_ESCAPE ? i : 0;

        for (int i = 0; i < IM_ARRAYSIZE(io->MouseDown); i++)
            if (igIsMouseDown(i))
                *key = i + (i > 1 ? 2 : 1);
    }
}

static void checkboxedColorPicker(const char* name, bool* enable, float* color)
{
    char buf[128];
    sprintf_s(buf, sizeof(buf), "##%s", name);
    igCheckbox(buf, enable);
    igSameLine(0.0f, 5.0f);
    igPushIDInt(0);
    bool openPopup = igColorButton(buf, (ImVec4) { color[0], color[1], color[2], 1.0f }, ImGuiColorEditFlags_NoTooltip, (ImVec2){ 0, 0 });
    igPopID();
    igSameLine(0.0f, 5.0f);
    igTextUnformatted(name, NULL);
    igPushIDInt(1);
    if (openPopup)
        igOpenPopup(buf, ImGuiPopupFlags_None);
    if (igBeginPopup(buf, ImGuiWindowFlags_None)) {
        igPushIDInt(2);
        igColorPicker3(buf, color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
        igPopID();
        igEndPopup();
    }
    igPopID();
}

static void renderTriggerbotWindow()
{
    if (window.triggerbot) {
        igSetNextWindowSize((ImVec2){ 0.0f, 0.0f }, ImGuiCond_None);
        igBegin("Triggerbot", &window.triggerbot, windowFlags);
        static int currentCategory = 0;
        igPushItemWidth(110.0f);
        igPushIDInt(0);
        igComboStr("", &currentCategory, "All\0Pistols\0Heavy\0SMG\0Rifles\0", -1);
        igPopID();
        igSameLine(0.0f, -1.0f);
        static int currentWeapon = 0;
        igPushIDInt(1);
        switch (currentCategory) {
        case 0:
            currentWeapon = 0;
            igNewLine();
            break;
        case 1: {
            static int currentPistol = 0;
            igComboStr("", &currentPistol, "Glock-18\0P2000\0USP-S\0Dual Berettas\0P250\0Tec-9\0Five-Seven\0CZ-75\0Desert Eagle\0Revolver\0", -1);
            currentWeapon = currentPistol + 1;
            break;
        }
        case 2: {
            static int currentHeavy = 0;
            igComboStr("", &currentHeavy, "Nova\0XM1014\0Sawed-off\0MAG-7\0M249\0Negev\0", -1);
            currentWeapon = currentHeavy + 11;
            break;
        }
        case 3: {
            static int currentSmg = 0;
            igComboStr("", &currentSmg, "Mac-10\0MP9\0MP7\0MP5-SD\0UMP-45\0P90\0PP-Bizon\0", -1);
            currentWeapon = currentSmg + 17;
            break;
        }
        case 4: {
            static int currentRifle = 0;
            igComboStr("", &currentRifle, "Galil AR\0Famas\0AK-47\0M4A4\0M4A1-S\0SSG-08\0SG-553\0AUG\0AWP\0G3SG1\0SCAR-20\0", -1);
            currentWeapon = currentRifle + 24;
            break;
        }
        }
        igPopID();
        igSameLine(0.0f, -1.0f);
        igCheckbox("Enabled", &config.triggerbot[currentWeapon].enabled);
        igSeparator();
        igCheckbox("On key", &config.triggerbot[currentWeapon].onKey);
        igSameLine(0.0f, -1.0f);
        keybind(&config.triggerbot[currentWeapon].key);
        igCheckbox("Friendly fire", &config.triggerbot[currentWeapon].friendlyFire);
        igCheckbox("Scoped only", &config.triggerbot[currentWeapon].scopedOnly);
        igCheckbox("Ignore flash", &config.triggerbot[currentWeapon].ignoreFlash);
        igCheckbox("Ignore smoke", &config.triggerbot[currentWeapon].ignoreSmoke);
        igPushItemWidth(85.0f);
        igComboStr("Hitgroup", &config.triggerbot[currentWeapon].hitgroup, "All\0Head\0Chest\0Stomach\0Left arm\0Right arm\0Left leg\0Right leg\0", -1);
        igPushItemWidth(220.0f);
        igSliderInt("Shot delay", &config.triggerbot[currentWeapon].shotDelay, 0, 250, "%d ms", ImGuiSliderFlags_None);
        igInputInt("Min damage", &config.triggerbot[currentWeapon].minDamage, 1, 100, ImGuiInputTextFlags_None);
        config.triggerbot[currentWeapon].minDamage = max(min(config.triggerbot[currentWeapon].minDamage, 250), 0);
        igCheckbox("Killshot", &config.triggerbot[currentWeapon].killshot);
        igEnd();
    }
}

static void renderGlowWindow()
{
    if (window.glow) {
        igSetNextWindowSize((ImVec2){ 450.0f, 0.0f }, ImGuiCond_None);
        igBegin("Glow", &window.glow, windowFlags);

        static int currentCategory = 0;
        igPushItemWidth(110.0f);
        igPushIDInt(0);
        igComboStr("", &currentCategory, "Allies\0Enemies\0Planting\0Defusing\0Local player\0Weapons\0C4\0Planted C4\0Chickens\0", -1);
        igPopID();
        static int currentItem = 0;
        if (currentCategory <= 3) {
            igSameLine(0.0f, -1.0f);
            static int currentType = 0;
            igPushIDInt(1);
            igComboStr("", &currentType, "All\0Visible\0Occluded\0", -1);
            igPopID();
            currentItem = currentCategory * 3 + currentType;
        } else {
            currentItem = currentCategory + 8;
        }

        igSameLine(0.0f, -1.0f);
        igCheckbox("Enabled", &config.glow[currentItem].enabled);
        igSeparator();
        igColumns(2, NULL, false);
        igSetColumnOffset(1, 150.0f);
        igCheckbox("Health based", &config.glow[currentItem].healthBased);
        igCheckbox("Rainbow", &config.glow[currentItem].rainbow);
        bool openPopup = igColorButton("Color", (ImVec4){ config.glow[currentItem].color[0], config.glow[currentItem].color[1], config.glow[currentItem].color[2], 1.0f }, ImGuiColorEditFlags_NoTooltip, (ImVec2){ 0, 0 });
        igSameLine(0.0f, 5.0f);
        igText("Color");
        igPushIDInt(2);
        if (openPopup)
            igOpenPopup("", ImGuiPopupFlags_None);
        if (igBeginPopup("", ImGuiPopupFlags_None)) {
            igPushIDInt(3);
            igColorPicker3("", config.glow[currentItem].color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
            igPopID();
            igEndPopup();
        }
        igPopID();
        igNextColumn();
        igPushItemWidth(220.0f);
        igSliderFloat("Thickness", &config.glow[currentItem].thickness, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_None);
        igSliderFloat("Alpha", &config.glow[currentItem].alpha, 0.0f, 1.0f, "%.2f", ImGuiSliderFlags_None);
        igInputInt("Style", &config.glow[currentItem].style, 1, 1, ImGuiInputTextFlags_None);
        config.glow[currentItem].style = max(min(config.glow[currentItem].style, 3), 0);
        igEnd();
    }
}

static void renderEspWindow()
{
    if (window.esp) {
        igSetNextWindowSize((ImVec2){ 0.0f, 0.0f }, ImGuiCond_None);
        igBegin("Esp", &window.esp, windowFlags);
        
        static int currentCategory = 0;
        igPushItemWidth(110.0f);
        igPushIDInt(0);
        igComboStr("", &currentCategory, "Allies\0Enemies\0Weapons", -1);
        igPopID();
        igSameLine(0.0f, -1.0f);

        if (currentCategory < 2) {
            static int currentType = 0;
            igPushIDInt(1);
            igComboStr("", &currentType, "All\0Visible\0Occluded\0", -1);
            igPopID();
            int currentItem = currentCategory * 3 + currentType;
            igSameLine(0.0f, -1.0f);
            igCheckbox("Enabled", &config.esp.players[currentItem].enabled);
            igSeparator();

            checkboxedColorPicker("Box", &config.esp.players[currentItem].box, config.esp.players[currentItem].boxColor);
        } else {
            igCheckbox("Enabled", &config.esp.weapon.enabled);
            igSeparator();

            checkboxedColorPicker("Box", &config.esp.weapon.box, config.esp.weapon.boxColor);
            igSameLine(0.0f, -1.0f);
            igComboStr("", &config.esp.weapon.boxType, "2D\0""3D\0", -1);
            checkboxedColorPicker("Snap line", &config.esp.weapon.snapLine, config.esp.weapon.snapLineColor);
            checkboxedColorPicker("Name", &config.esp.weapon.name, config.esp.weapon.nameColor);
        }
        igEnd();
    }
}

static void renderMiscWindow()
{
    if (window.misc) {
        igSetNextWindowSize((ImVec2){ 0.0f, 0.0f }, ImGuiCond_None);
        igBegin("Misc", &window.misc, windowFlags);
        igCheckbox("Auto strafe", &config.misc.autostrafe);
        igCheckbox("Bunnyhop", &config.misc.bunnyhop);
        igCheckbox("Moonwalk", &config.misc.moonwalk);
        if (igButton("Unhook", (ImVec2){ 0, 0 }))
            Hooks_restore();
        igEnd();
    }
}

static void renderConfigWindow()
{
    if (window.config) {
        igSetNextWindowSize((ImVec2){ 290.0f, 190.0f }, ImGuiCond_None);
        igBegin("Config", &window.config, windowFlags);

        igColumns(2, NULL, false);
        igSetColumnOffset(1, 170.0f);

        igPushItemWidth(160.0f);

        static int currentConfig = -1;

        if ((size_t)currentConfig >= config.count)
            currentConfig = -1;

        static char buffer[16];

        if (igListBoxStr_arr("", &currentConfig, config.names, config.count, 5) && currentConfig != -1)
            strcpy(buffer, config.names[currentConfig]);

        igPushIDInt(0);
        if (igInputText("", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue, NULL, NULL)) {
            if (currentConfig != -1)
                Config_rename(currentConfig, buffer);
        }
        igPopID();
        igNextColumn();

        igPushItemWidth(100.0f);

        if (igButton("Create config", (ImVec2){ 100.0f, 25.0f }))
            Config_add(buffer);

        if (igButton("Reset config", (ImVec2){ 100.0f, 25.0f }))
            Config_reset();

        if (currentConfig != -1) {
            if (igButton("Load selected", (ImVec2){ 100.0f, 25.0f })) {
                Config_load(currentConfig);
            }
            if (igButton("Save selected", (ImVec2){ 100.0f, 25.0f }))
                Config_save(currentConfig);
            if (igButton("Delete selected", (ImVec2){ 100.0f, 25.0f }))
                Config_remove(currentConfig);
        }
        igEnd();
    }
}

void GUI_render()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    igNewFrame();

    renderMenuBar();
    renderTriggerbotWindow();
    renderGlowWindow();
    renderEspWindow();
    renderMiscWindow();
    renderConfigWindow();

    igEndFrame();
    igRender();
    ImGui_ImplDX9_RenderDrawData(igGetDrawData());
}

void GUI_invalidateDeviceObjects(void)
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
}

void GUI_createDeviceObjects(void)
{
    ImGui_ImplDX9_CreateDeviceObjects();
}
