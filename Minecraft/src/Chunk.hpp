#pragma once
#include <Tellus.hpp>
#include "Block.hpp"
#define WATER_LEVEL 120

struct TextureFormat {
    unsigned int top;
    unsigned int side;
    unsigned int bottom;
};

enum class BlockFace {
    Front,
    Back,
    Right,
    Left,
    Top,
    Bottom
};

class Chunk {
   public:
    Chunk();
    ~Chunk();

    void Init(glm::ivec3 localChunkPosition = glm::ivec3(0));

    bool operator==(const Chunk& other) { return m_LocalChunkPosition == other.GetPosition(); }
    bool operator!=(const Chunk& other) { return m_LocalChunkPosition != other.GetPosition(); }

    void Generate();
    void GenMesh();
    void CreateMesh();
    void CreateMesh(std::vector<Chunk*> neighbors);
    void UploadToGPU();

    const ts::Ref<ts::VertexArray>& GetTerrainVertexArray() const { return m_TerrainVA; }
    const ts::Ref<ts::VertexArray>& GetWaterVertexArray() const { return m_WaterVA; }
    glm::mat4 GetModelMatrix() const { return glm::translate(glm::mat4(1.0f), {m_LocalChunkPosition.x * CHUNK_WIDTH, m_LocalChunkPosition.y * CHUNK_HEIGHT, m_LocalChunkPosition.z * CHUNK_DEPTH}); }
    glm::ivec3 GetPosition() const { return m_LocalChunkPosition; }
    Block GetBlock(glm::ivec3 pos) const { return m_Blocks[(pos.z * CHUNK_WIDTH * CHUNK_HEIGHT) + (pos.y * CHUNK_WIDTH) + pos.x]; }
    void SetBlock(glm::ivec3 pos, BlockType type) {
        m_Blocks[(pos.z * CHUNK_WIDTH * CHUNK_HEIGHT) + (pos.y * CHUNK_WIDTH) + pos.x].SetBlockType(type);
        m_Altered = true;
    }
    bool NeighborActive(glm::ivec3 chunkPos, glm::ivec3 blockPos);
    bool NeighborActive(glm::ivec3 chunkPos, glm::ivec3 blockPos, std::vector<Chunk*> neighbors);
    bool NeighborActive(int index, int offsetA, int offsetB = 0, int offsetC = 0);
    bool NeighborActive(int index, std::vector<Chunk*> neighbors, int offsetA, int offsetB = 0, int offsetC = 0);
    bool ProperDistance(glm::ivec3 blockPos, int maxDist);

    inline bool IsLoaded() const { return m_Loaded; }
    inline bool IsAltered() const { return m_Altered; }

   private:
    void CreateFace(unsigned int format, glm::ivec3 pos00, glm::ivec3 pos10, glm::ivec3 pos01, glm::ivec3 pos11);
    void CreateWaterFace(unsigned int format, glm::ivec3 pos00, glm::ivec3 pos10, glm::ivec3 pos01, glm::ivec3 pos11);
    void AddQuadAO(float* noise, unsigned int format, int32_t idx, int32_t facingOffset, int32_t offsetA, int32_t offsetB, glm::ivec3 pos00, glm::ivec3 pos10, glm::ivec3 pos01, glm::ivec3 pos11);
    void AddQuadAO(unsigned int format, int32_t idx, int32_t facingOffset, int32_t offsetA, int32_t offsetB, glm::ivec3 pos00, glm::ivec3 pos10, glm::ivec3 pos01, glm::ivec3 pos11);
    void AddQuadAO(unsigned int format, std::vector<Chunk*> neighbors, int32_t idx, int32_t facingOffset, int32_t offsetA, int32_t offsetB, glm::ivec3 pos00, glm::ivec3 pos10, glm::ivec3 pos01, glm::ivec3 pos11);
    glm::ivec3 To3D(int index);

   public:
    static const int CHUNK_WIDTH = 32;
    static const int CHUNK_HEIGHT = 32;
    static const int CHUNK_DEPTH = 32;
    static const int CHUNK_SIZE = CHUNK_DEPTH * CHUNK_HEIGHT * CHUNK_WIDTH;

   private:
    TextureFormat GetUVs(BlockType type);

   private:
    unsigned int* m_Vertices;
    float* m_Noise;
    std::vector<unsigned int> m_WaterVertices;
    bool m_Loaded, m_Altered = false;
    int m_VertexCount;
    ts::Ref<ts::VertexArray> m_TerrainVA, m_WaterVA;
    ts::Scope<Block[]> m_Blocks;
    FastNoise::SmartNode<FastNoise::FractalFBm> m_Fractal;
    // Chunk(0,0), Chunk(1, 0) etc.
    glm::ivec3 m_LocalChunkPosition;
};