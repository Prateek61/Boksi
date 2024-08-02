#version 430 core
#extension GL_NV_gpu_shader5:enable
layout(local_size_x=16,local_size_y=16)in;

#include "utils/camera.glsl"

struct Voxel{
    uint8_t materialID;
};

layout(std430,binding=1)buffer Voxels{
    Voxel data[];
};

uniform Camera u_Camera;
uniform ivec3 u_Dimensions;

const float iVoxelSize=.5;
const float tMax=100.;
const float tDelta=.1;
const int maxDepth=1;

layout(rgba8,binding=0)uniform image2D img_output;

ivec2 pixel_coords=ivec2(gl_GlobalInvocationID.xy);

vec3 GetRayDirection(ivec2 pixel_coords)
{
    vec4 ndc=vec4(2.*vec2(pixel_coords)/u_Camera.ScreenSize-1.,1.,1.);
    vec4 clip=u_Camera.InverseProjection*ndc;
    clip=vec4(clip.xyz/clip.w,0.);
    vec4 world=u_Camera.InverseView*clip;
    vec3 dir=normalize(world.xyz);
    return dir;
}

bool IsInsideGrid(ivec3 voxel_coords)
{
    return all(greaterThanEqual(voxel_coords, ivec3(0))) && all(lessThan(voxel_coords, u_Dimensions));
}


float random(vec2 p)
{
    // We need irrationals for pseudo randomness.
    // Most (all?) known transcendental numbers will (generally) work.
    const vec2 r=vec2(
        23.1406926327792690,// e^pi (Gelfond's constant)
    2.6651441426902251);// 2^sqrt(2) (Gelfond–Schneider constant)
    return fract(cos(mod(123456789.,1e-7+256.*dot(p,r))));
}

float rand()
{
    return random(pixel_coords);
}

vec3 RandomRayDirection(vec3 rayDirection )
{
    
    return normalize(rayDirection+vec3(2.*(rand()-.5),2.*(rand()-.5),2.*(rand()-.5)));
    
}

vec4 RayMarch(vec3 start, vec3 dir)
{
    const int maxSteps = 1000;
    const int maxDepth = 5; // Define a fixed maximum depth
    vec4 resultColor = vec4(0.529, 0.808, 0.922, 1.0); // Default sky blue color
    float attenuation = 1.0; // Initial attenuation factor

    struct RayState {
        vec3 start;
        vec3 dir;
        int depth;
        float attenuation;
    };

    // Initialize the stack with the initial state
    RayState stack[maxDepth];
    int stackSize = 0;

    stack[stackSize++] = RayState(start, dir, maxDepth, attenuation);

    while (stackSize > 0)
    {
        // Pop the state from the stack
        RayState current = stack[--stackSize];

        if (current.depth <= 0) {
            continue;
        }

        float stepX = current.dir.x > 0.0 ? 1.0 : -1.0;
        float stepY = current.dir.y > 0.0 ? 1.0 : -1.0;
        float stepZ = current.dir.z > 0.0 ? 1.0 : -1.0;

        stepX *= iVoxelSize;
        stepY *= iVoxelSize;
        stepZ *= iVoxelSize;

        float tDeltaX = iVoxelSize / abs(current.dir.x);
        float tDeltaY = iVoxelSize / abs(current.dir.y);
        float tDeltaZ = iVoxelSize / abs(current.dir.z);

        float tMaxX = 0.0;
        float tMaxY = 0.0;
        float tMaxZ = 0.0;

        for (int i = 0; i < maxSteps; i++) {
            // if (tMaxX < tMaxY) {
            //     if (tMaxX < tMaxZ) {
            //         current.start.x += stepX;
            //         tMaxX += tDeltaX;
            //     } else {
            //         current.start.z += stepZ;
            //         tMaxZ += tDeltaZ;
            //     }
            // } else {
            //     if (tMaxY < tMaxZ) {
            //         current.start.y += stepY;
            //         tMaxY += tDeltaY;
            //     } else {
            //         current.start.z += stepZ;
            //         tMaxZ += tDeltaZ;
            //     }
            // }

            // Optimize
            float tMaxXLessThanTMaxY = float(tMaxX < tMaxY);
            float tMaxXLessThanTMaxZ = float(tMaxX < tMaxZ);
            float tMaxYLessThanTMaxZ = float(tMaxY < tMaxZ);

            current.start += vec3(tMaxXLessThanTMaxY * tMaxXLessThanTMaxZ * stepX, (1.0 - tMaxXLessThanTMaxY) * tMaxYLessThanTMaxZ * stepY, ((1.0 - tMaxXLessThanTMaxY) * (1.0 - tMaxYLessThanTMaxZ) + tMaxXLessThanTMaxY * (1 - tMaxXLessThanTMaxZ)) * stepZ);
            tMaxX += tMaxXLessThanTMaxY * tMaxXLessThanTMaxZ * tDeltaX;
            tMaxY += (1.0 - tMaxXLessThanTMaxY) * tMaxYLessThanTMaxZ * tDeltaY;
            tMaxZ += ((tMaxXLessThanTMaxY) * (1.0 - tMaxXLessThanTMaxZ) + (1 - tMaxXLessThanTMaxY) * (1 - tMaxYLessThanTMaxZ)) * tDeltaZ;

            ivec3 voxel_coords = ivec3(floor(current.start / iVoxelSize));

            if (IsInsideGrid(voxel_coords)) {

                int index = voxel_coords.x + voxel_coords.y * u_Dimensions.x + voxel_coords.z * u_Dimensions.x * u_Dimensions.y;

                if (data[index].materialID != uint8_t(0)) {
                    vec3 normal = vec3(0.0);

                    // Calculate normal using central difference
                    if (voxel_coords.x > 0 && voxel_coords.x < u_Dimensions.x - 1 &&
                        voxel_coords.y > 0 && voxel_coords.y < u_Dimensions.y - 1 &&
                        voxel_coords.z > 0 && voxel_coords.z < u_Dimensions.z - 1) {
                        
                        int indexX1 = (voxel_coords.x - 1) + voxel_coords.y * u_Dimensions.x + voxel_coords.z * u_Dimensions.x * u_Dimensions.y;
                        int indexX2 = (voxel_coords.x + 1) + voxel_coords.y * u_Dimensions.x + voxel_coords.z * u_Dimensions.x * u_Dimensions.y;
                        int indexY1 = voxel_coords.x + (voxel_coords.y - 1) * u_Dimensions.x + voxel_coords.z * u_Dimensions.x * u_Dimensions.y;
                        int indexY2 = voxel_coords.x + (voxel_coords.y + 1) * u_Dimensions.x + voxel_coords.z * u_Dimensions.x * u_Dimensions.y;
                        int indexZ1 = voxel_coords.x + voxel_coords.y * u_Dimensions.x + (voxel_coords.z - 1) * u_Dimensions.x * u_Dimensions.y;
                        int indexZ2 = voxel_coords.x + voxel_coords.y * u_Dimensions.x + (voxel_coords.z + 1) * u_Dimensions.x * u_Dimensions.y;

                        normal.x = data[indexX2].materialID - data[indexX1].materialID;
                        normal.y = data[indexY2].materialID - data[indexY1].materialID;
                        normal.z = data[indexZ2].materialID - data[indexZ1].materialID;

                        normal = normalize(normal);
                    }

                    vec3 color;
                    if (data[index].materialID == uint8_t(1)) {
                        color = vec3(1.0, 0.0, 0.0);
                    } else if (data[index].materialID == uint8_t(2)) {
                        color = vec3(0.0, 1.0, 0.0);
                    } else if (data[index].materialID == uint8_t(3)) {
                        color = vec3(0.0, 0.0, 1.0);
                    }

                    resultColor = vec4(color, 1.0) * current.attenuation;
                    vec3 newDir = RandomRayDirection(current.dir) + normal;

                    stack[stackSize++] = RayState(current.start, newDir , current.depth - 1, current.attenuation * 0.8);
                    break;
                }
            }
        }
    }

    return resultColor;
}


void main()
{
    // Based on the direction of ray direct, store color
    vec3 rayDirection=GetRayDirection(pixel_coords);
    vec3 currentPos=u_Camera.Position;
    
    // Ray Marching Loop DDA
    vec4 color=RayMarch(currentPos,rayDirection);
    
    imageStore(img_output,pixel_coords,color);
    
}
