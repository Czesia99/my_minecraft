#pragma once

#include <glm/glm.hpp>

enum BlockType
{
    Air,
    Grass,
    Dirt,
    Stone,
    Oak_log,
    Oak_leaves,
    Glass,
    Water,
    Sand,
    Snow,
    Undefined,
};

enum BlockTextures
{
    UndefinedTex,
    GrassTop,
    GrassSide,
    DirtTex,
    StoneTex,
    OakLogSide,
    OakLogTop,
    LeavesOakTex,
    GlassTex,
    WaterTex,
    SandTex,
    SnowTex,
};

enum FaceOrientation
{
    Front,
    Back,
    Left,
    Right,
    Top,
    Bottom,
};

constexpr BlockType getBlockType(uint8_t bt)
{
    switch (bt)
    {
        case 0: return BlockType::Air;
        case 1: return BlockType::Grass;
        case 2: return BlockType::Dirt;
        case 3: return BlockType::Stone;
        case 4: return BlockType::Oak_log;
        case 5: return BlockType::Oak_leaves;
        case 6: return BlockType::Glass;
        case 7: return BlockType::Water;
        case 8: return BlockType::Sand;
        case 9: return BlockType::Snow;
        default: return BlockType::Undefined;
    }
}

constexpr uint8_t getBlockByte(BlockType bt)
{
    switch (bt)
    {
        case BlockType::Air: return 0;
        case BlockType::Grass: return 1;
        case BlockType::Dirt: return 2;
        case BlockType::Stone: return 3;
        case BlockType::Oak_log: return 4;
        case BlockType::Oak_leaves: return 5;
        case BlockType::Glass: return 6;
        case BlockType::Water: return 7;
        case BlockType::Sand: return 8;
        case BlockType::Snow: return 9;
        default: return BlockType::Undefined;
    }
}

constexpr glm::ivec3 getFaceOrientationVector(FaceOrientation orientation)
{
    switch (orientation)
    {
        case FaceOrientation::Front: return {0, 0, -1};
        case FaceOrientation::Back: return {0, 0, 1};
        case FaceOrientation::Left: return {-1, 0, 0};
        case FaceOrientation::Right: return {1, 0, 0};
        case FaceOrientation::Top: return {0, 1, 0};
        case FaceOrientation::Bottom: return {0, -1, 0};
        default: return {0, 0, 0};
    }
}