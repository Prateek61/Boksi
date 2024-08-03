#version 430 core
#extension GL_NV_gpu_shader5:enable
layout(local_size_x=16,local_size_y=16)in;

#include "utils/camera.glsl"
#include "utils/SVO.glsl"

layout(rgba8,binding=0)uniform image2D img_output;

layout(std430, binding=1)buffer Octree
{
    Node nodes[];
};

// Uniforms
uniform Camera u_Camera;
uniform ivec3 u_Dimensions;
uniform int u_MaxDepth;
uniform float u_VoxelSize;

void main()
{
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

    //random color 
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

    // Write to output image
    imageStore(img_output, pixel_coords, color);
}