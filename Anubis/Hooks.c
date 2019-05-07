#include <d3d9.h>
#include <stdbool.h>
#include <Windows.h>

#include "Hacks/Misc.h"
#include "Hooks.h"
#include "Memory.h"
#include "SDK/Engine.h"
#include "SDK/EntityList.h"
#include "SDK/UserCmd.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_D3D9_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_d3d9.h"

Hooks hooks;

struct nk_context* ctx;

static LRESULT __stdcall hookedWndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (nk_d3d9_handle_event(window, msg, wParam, lParam))
        return 0;
    return CallWindowProc(hooks.originalWndProc, window, msg, wParam, lParam);
}

static HRESULT __stdcall hookedPresent(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND windowOverride, const RGNDATA* dirtyRegion)
{
    static bool init = false;
    if (!init) {
        ctx = nk_d3d9_init(device, 1280, 720);
        struct nk_font_atlas* atlas;
        nk_d3d9_font_stash_begin(&atlas);
        nk_d3d9_font_stash_end();
        init = true;
    }

    nk_input_end(ctx);

    if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
        NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
    {
        enum { EASY, HARD };
        static int op = EASY;
        static int property = 20;

        nk_layout_row_dynamic(ctx, 30, 2);
        if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
        if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
        nk_layout_row_dynamic(ctx, 22, 1);
        nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx, "background:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 25, 1);
        static struct nk_colorf bg = { 0.1f, 0.18f, 0.24f, 1.0f };

        if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400))) {
            nk_layout_row_dynamic(ctx, 120, 1);
            bg = nk_color_picker(ctx, bg, NK_RGBA);
            nk_layout_row_dynamic(ctx, 25, 1);
            bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
            bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
            bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
            bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
            nk_combo_end(ctx);
        }
    }
    nk_end(ctx);
    nk_d3d9_render(NK_ANTI_ALIASING_ON);
    nk_input_begin(ctx);
    return hooks.originalPresent(device, src, dest, windowOverride, dirtyRegion);
}

static size_t calculateLength(uintptr_t* vmt)
{
    size_t length = 0;
    MEMORY_BASIC_INFORMATION memoryInfo;
    while (VirtualQuery((LPVOID)vmt[length], &memoryInfo, sizeof(memoryInfo)) && memoryInfo.Protect == PAGE_EXECUTE_READ)
        length++;
    return length;
}

static void hookVmt(void* const base, VmtHook* vmtHook)
{
    vmtHook->base = base;
    vmtHook->oldVmt = *((uintptr_t**)base);
    vmtHook->length = calculateLength(vmtHook->oldVmt) + 1;
    vmtHook->newVmt = malloc(vmtHook->length * sizeof(uintptr_t));
    memcpy(vmtHook->newVmt, vmtHook->oldVmt - 1, vmtHook->length * sizeof(uintptr_t));
    *((uintptr_t**)base) = vmtHook->newVmt + 1;
}

static void hookMethod(VmtHook* vmtHook, size_t index, void* function)
{
    if (index < vmtHook->length)
        vmtHook->newVmt[index + 1] = (uintptr_t)function;
}

static bool __stdcall hookedCreateMove(float inputSampleTime, UserCmd* cmd)
{
    bool result = ((bool(__stdcall*)(float, UserCmd*))hooks.clientMode.oldVmt[24])(inputSampleTime, cmd);
    
    if (!cmd->command_number)
        return result;

    Misc_autostrafe(cmd);
    Misc_bunnyhop(cmd);

    return false;
}

void initializeHooks(void)
{
    hookVmt(memory.clientMode, &hooks.clientMode);
    hookMethod(&hooks.clientMode, 24, hookedCreateMove);

    HWND window = FindWindowA("Valve001", NULL);
    hooks.originalWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)hookedWndProc);

    hooks.originalPresent = **memory.present;
    **memory.present = hookedPresent;
}
