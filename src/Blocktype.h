#pragma once

#include <glm/glm.hpp>
#include "glm/gtx/hash.hpp"
#include <unordered_map>
#include <string>
#include <vector>

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
    OakPlank,
    StoneBrick,
    Netherrack,
    GoldBlock,
    PackedIce,
    Lava,
    Barrel,
    Bookshelf,
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
    OakPlankTex,
    StoneBrickTex,
    NetherrackTex,
    GoldBlockTex,
    PackedIceTex,
    LavaTex,
    BarrelTopTex,
    BarrelSideTex,
    BarrelBottomTex,
    BookshelfTex
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

inline BlockType getBlockType(uint8_t bt)
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
        case 10: return BlockType::OakPlank;
        case 11: return BlockType::StoneBrick;
        case 12: return BlockType::Netherrack;
        case 13: return BlockType::GoldBlock;
        case 14: return BlockType::PackedIce;
        case 15: return BlockType::Lava;
        case 16: return BlockType::Barrel;
        case 17: return BlockType::Bookshelf;
        default: return BlockType::Undefined;
    }
}

inline uint8_t getBlockByte(BlockType bt)
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
        case BlockType::OakPlank: return 10;
        case BlockType::StoneBrick: return 11;
        case BlockType::Netherrack: return 12;
        case BlockType::GoldBlock: return 13;
        case BlockType::PackedIce: return 14;
        case BlockType::Lava: return 15;
        case BlockType::Barrel: return 16;
        case BlockType::Bookshelf: return 17;
        default: return BlockType::Undefined;
    }
}

inline glm::ivec3 getFaceOrientationVector(FaceOrientation orientation)
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

//Blocktype, {top, side, bottom}
inline std::unordered_map<BlockType, std::array<BlockTextures, 3>> textures_umap =
{
    {BlockType::Undefined, {BlockTextures::UndefinedTex, BlockTextures::UndefinedTex, BlockTextures::UndefinedTex}},
    {BlockType::Grass, {BlockTextures::GrassTop, BlockTextures::GrassSide, BlockTextures::DirtTex}},
    {BlockType::Dirt, {BlockTextures::DirtTex, BlockTextures::DirtTex, BlockTextures::DirtTex}},
    {BlockType::Stone, {BlockTextures::StoneTex, BlockTextures::StoneTex, BlockTextures::StoneTex}},
    {BlockType::Oak_log, {BlockTextures::OakLogTop, BlockTextures::OakLogSide, BlockTextures::OakLogTop}},
    {BlockType::Oak_leaves, {BlockTextures::LeavesOakTex, BlockTextures::LeavesOakTex, BlockTextures::LeavesOakTex}},
    {BlockType::Glass, {BlockTextures::GlassTex, BlockTextures::GlassTex, BlockTextures::GlassTex}},
    {BlockType::Water, {BlockTextures::WaterTex, BlockTextures::WaterTex, BlockTextures::WaterTex}},
    {BlockType::Sand, {BlockTextures::SandTex, BlockTextures::SandTex, BlockTextures::SandTex}},
    {BlockType::Snow, {BlockTextures::SnowTex, BlockTextures::SnowTex, BlockTextures::SnowTex}},
    {BlockType::OakPlank, {BlockTextures::OakPlankTex, BlockTextures::OakPlankTex, BlockTextures::OakPlankTex}},
    {BlockType::StoneBrick, {BlockTextures::StoneBrickTex, BlockTextures::StoneBrickTex, BlockTextures::StoneBrickTex}},
    {BlockType::Netherrack, {BlockTextures::NetherrackTex, BlockTextures::NetherrackTex, BlockTextures::NetherrackTex}},
    {BlockType::GoldBlock, {BlockTextures::GoldBlockTex, BlockTextures::GoldBlockTex, BlockTextures::GoldBlockTex}},
    {BlockType::PackedIce, {BlockTextures::PackedIceTex, BlockTextures::PackedIceTex, BlockTextures::PackedIceTex}},
    {BlockType::Lava, {BlockTextures::LavaTex, BlockTextures::LavaTex, BlockTextures::LavaTex}},
    {BlockType::Barrel, {BlockTextures::BarrelTopTex, BlockTextures::BarrelSideTex, BlockTextures::BarrelBottomTex}},
    {BlockType::Bookshelf, {BlockTextures::OakPlankTex, BlockTextures::BookshelfTex, BlockTextures::OakPlankTex}},
};

inline std::vector<std::string>block_textures_path
{
    "../assets/textures/default/undefined.png",
    "../assets/textures/default/grasstop.jpg",
    "../assets/textures/default/grass_block_side.png",
    "../assets/textures/default/dirt.png",
    "../assets/textures/default/stone.png",
    "../assets/textures/default/oak_log.png",
    "../assets/textures/default/oak_log_top.png",
    "../assets/textures/default/leaves_oak.png",
    "../assets/textures/default/glass.png",
    "../assets/textures/default/water.png",
    "../assets/textures/default/sand.png",
    "../assets/textures/default/snow.png",
    "../assets/textures/default/planks_oak.png",
    "../assets/textures/default/stonebrick.png",
    "../assets/textures/default/netherrack.png",
    "../assets/textures/default/gold_block.png",
    "../assets/textures/default/ice_packed.png",
    "../assets/textures/default/lava.png",
    "../assets/textures/default/barrel_top.png",
    "../assets/textures/default/barrel_side.png",
    "../assets/textures/default/barrel_bottom.png",
    "../assets/textures/default/bookshelf.png",
};