#include "ImGuiLayout.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

ImGuiLayout::ImGuiLayout(MirControl* parent, SDL_Window* window, SDL_Renderer* renderer)
        : ImGuiControl(parent, 0, 0, nullptr, Point(0, 0)), window(window), renderer(renderer) {

    // 初始化 ImGui 上下文
    imgui_context = ImGui::CreateContext();
    ImGui::SetCurrentContext(imgui_context);

    // 初始化 ImGui SDL2 和 SDLRenderer 后端
    if (!ImGui_ImplSDL2_InitForSDLRenderer(window, renderer) ||
        !ImGui_ImplSDLRenderer2_Init(renderer)) {
        throw std::runtime_error("Failed to initialize ImGui SDL backend.");
    }

    // 默认大小为父控件的尺寸
    if (parent != nullptr) {
        setSize(parent->getSize());
    }
}

bool ImGuiLayout::beginDraw() {
    ImGui::SetCurrentContext(imgui_context);
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    return true;
}

bool ImGuiLayout::endDraw() {
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    return true;
}

void* ImGuiLayout::getImGuiContext() const {
    return imgui_context;
}
