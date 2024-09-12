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
};

enum BlockTextures
{
    GrassTop,
    GrassSide,
    DirtTex,
    StoneTex,
    OakLogSide,
    OakLogTop,
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
        default: return BlockType::Air;
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