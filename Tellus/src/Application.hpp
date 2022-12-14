#pragma once
#include "Window.hpp"
#include "Layers/Layer.hpp"
#include "Layers/LayerStack.hpp"
#include "Renderer/Buffer.hpp"
#include "Renderer/Camera.hpp"
#include "Renderer/Shader.hpp"
#include "Layers/ImGuiLayer.hpp"

namespace ts {

class Application {
   public:
    virtual ~Application();
    void Run();
    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    static Application& Get() { return *s_Instance; }
    inline Window& GetWindow() { return *m_Window; }

   protected:
    Application();

   private:
    static Application* s_Instance;

    Scope<Window> m_Window;
    bool m_Running = true;
    float m_LastFrame;

    LayerStack m_LayerStack;
    ImGuiLayer* m_ImGuiLayer;
};

Application* CreateApplication();

}  // namespace ts