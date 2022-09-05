#pragma once
#define TS_MAIN
#include <Tellus.hpp>
#include "Chunk.hpp"
#include "TextureAtlas.hpp"

class Game : public ts::Layer {
   public:
    Game();
    ~Game();

    void OnUpdate() override;
    void OnEvent(ts::Event& event) override;

   private:
    ts::Scope<Chunk> m_Chunk;
    ts::Ref<ts::VertexArray> m_VertexArray;
    ts::Camera m_Camera;
    ts::Ref<ts::Texture2D> m_Texture;
    ts::Ref<TextureAtlas> m_WorldTexture;
    ts::Ref<ts::Shader> m_Shader;

    ts::Ref<ts::VertexArray> m_SkyBox;
    ts::Ref<ts::TextureCubeMap> m_SkyBoxTexture;
    ts::Ref<ts::Shader> m_SkyBoxShader;
};

class App : public ts::Application {
   public:
    App();
    ~App();
};