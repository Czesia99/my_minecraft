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

enum class FaceOrientation
{
    Front,
    Back,
    Left,
    Right,
    Top,
    Bottom,
};

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