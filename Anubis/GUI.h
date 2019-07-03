#pragma once

#include <stdbool.h>

struct IDirect3DDevice9;

#ifdef __cplusplus  
extern "C"
{
    bool GUI_handleInput(HWND, UINT, WPARAM, LPARAM);
    void GUI_init(struct IDirect3DDevice9*);
    void GUI_render(void);
}
#else
    bool GUI_handleInput(HWND, UINT, WPARAM, LPARAM);
    void GUI_init(struct IDirect3DDevice9*);
    void GUI_render(void);
#endif

struct nk_context;
extern struct nk_context* ctx;

extern bool isGuiOpen;
