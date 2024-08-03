#version 430 core
#extension GL_NV_gpu_shader5:enable
layout(local_size_x=16,local_size_y=16)in;

#include "utils/camera.glsl"
#include "utils/SVO.glsl"

layout(std430, binding=1)buffer Octree
{
    Node nodes[];
};

// Uniforms
uniform Camera u_Camera;
uniform ivec3 u_Dimensions;
uniform int u_MaxDepth;
uniform float u_VoxelSize;