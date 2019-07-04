#include <d3d9.h>
#include <Windows.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"

#include "Config.h"
#include "GUI.h"

bool isGuiOpen = false;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool GUI_handleInput(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        isGuiOpen = !isGuiOpen;
    }
    if (isGuiOpen && !ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam))
        return true;
    return false;
}

void GUI_init(IDirect3DDevice9* device) noexcept
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
    bool misc{ false };
} window;

static void renderMenuBar() noexcept
{
    if (ImGui::BeginMainMenuBar()) {
        ImGui::MenuItem("Misc", nullptr, &window.misc);
        ImGui::EndMainMenuBar();
    }
}

static constexpr auto windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

static void renderMiscWindow() noexcept
{
    if (window.misc) {
        ImGui::SetNextWindowSize({ 0.0f, 0.0f });
        ImGui::Begin("Misc", &window.misc, windowFlags);
        ImGui::Checkbox("Bunnyhop", &config.misc.bunnyhop);
        ImGui::Checkbox("Autostrafe", &config.misc.autostrafe);
        ImGui::End();
    }
}

void GUI_render() noexcept
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    renderMenuBar();
    renderMiscWindow();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}
