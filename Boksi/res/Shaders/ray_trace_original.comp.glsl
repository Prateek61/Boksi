#version 430 core
#extension GL_NV_gpu_shader5:enable
layout(local_size_x=16, local_size_y=16) in;

#include "utils/camera.glsl"

struct Voxel {
    uint8_t materialID;
};

layout(std430, binding=1) buffer Voxels {
    Voxel data[];
};

struct Material {
    vec3 color;
};

layout(std430, binding=2) buffer Materials {
    Material materials[];
};

struct Camera {
    mat4 InverseProjection;
    mat4 InverseView;
    vec3 Position;
    vec3 ForwardDirection;
    vec2 ScreenSize;
};

uniform Camera u_Camera;
uniform ivec3 u_Dimensions;

const float iVoxelSize = 0.1;
const float maxDepth = 1000.0;

layout(rgba8, binding=0) uniform image2D img_output;

ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

vec3 GetRayDirection(ivec2 pixel_coords) {
    vec4 ndc = vec4(2.0 * vec2(pixel_coords) / u_Camera.ScreenSize - 1.0, -1.0, 1.0);
    vec4 clip = u_Camera.InverseProjection * ndc;
    clip = vec4(clip.xyz / clip.w, 0.0);
    vec4 world = u_Camera.InverseView * clip;
    vec3 dir = normalize(world.xyz);
    return dir;
}

bool IsInsideGrid(ivec3 voxel_coords) {
    return all(greaterThanEqual(voxel_coords, ivec3(0))) && all(lessThan(voxel_coords, u_Dimensions));
}

float random(vec2 p) {
    const vec2 r = vec2(23.1406926327792690, 2.6651441426902251);
    return fract(cos(mod(123456789.0, 1e-7 + 256.0 * dot(p, r))));
}

float rand() {
    return random(pixel_coords);
}

vec3 RandomRayDirection(vec3 rayDirection) {
    return normalize(rayDirection + vec3(2.0 * (rand() - 0.5), 2.0 * (rand() - 0.5), 2.0 * (rand() - 0.5)));
}

int GetMaterialID(ivec3 voxel_coords) {
    if (!IsInsideGrid(voxel_coords)) return 0;
    int index = voxel_coords.x + voxel_coords.y * u_Dimensions.x + voxel_coords.z * u_Dimensions.x * u_Dimensions.y;
    return int(data[index].materialID);
}

ivec3 RayMarch(vec3 start, vec3 dir, int maxDepth) {
    vec3 step = sign(dir) * iVoxelSize;
    vec3 tMax = (step + sign(step) * iVoxelSize - fract(start / iVoxelSize) * iVoxelSize) / abs(dir);
    vec3 tDelta = iVoxelSize / abs(dir);

    for (int i = 0; i < maxDepth; i++) {
        if (tMax.x < tMax.y) {
            if (tMax.x < tMax.z) {
                start += step.x * vec3(1, 0, 0);
                tMax.x += tDelta.x;
            } else {
                start += step.z * vec3(0, 0, 1);
                tMax.z += tDelta.z;
            }
        } else {
            if (tMax.y < tMax.z) {
                start += step.y * vec3(0, 1, 0);
                tMax.y += tDelta.y;
            } else {
                start += step.z * vec3(0, 0, 1);
                tMax.z += tDelta.z;
            }
        }

        ivec3 voxel_coords = ivec3(start / iVoxelSize);

        if (!IsInsideGrid(voxel_coords)) return ivec3(0);
        if (GetMaterialID(voxel_coords) != 0) return voxel_coords;
    }

    return ivec3(0);
}

void main() {
    vec3 rayDirection = GetRayDirection(pixel_coords);
    vec3 currentPos = u_Camera.Position;
    vec4 BG_COLOR = vec4(0.529, 0.808, 0.922, 1.0);

    ivec3 voxel_coords = RayMarch(currentPos, rayDirection, int(maxDepth));

    if (voxel_coords == ivec3(0)) {
        imageStore(img_output, pixel_coords, BG_COLOR);
        return;
    }

    int materialID = GetMaterialID(voxel_coords);
    vec4 color = vec4(materials[materialID].color, 1.0);

    imageStore(img_output, pixel_coords, color);
}
