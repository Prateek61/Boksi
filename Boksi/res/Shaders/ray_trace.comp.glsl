#version 430 core
layout(local_size_x = 16, local_size_y = 16) in;

struct Voxel {
    uint materialID;
};

layout(std430, binding = 1) buffer Voxels {
    Voxel data[];
};

layout(rgba8, binding = 0) uniform image2D img_output;


void main() {
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    
    // Random color
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
    color.r = float(pixelCoords.x) / 1280.0;
    color.g = float(pixelCoords.y) / 720.0;
    color.b = 0.5;

    imageStore(img_output, pixelCoords, color);
}