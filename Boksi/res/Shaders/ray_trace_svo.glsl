#version 430 core
#extension GL_NV_gpu_shader5 : enable
layout(local_size_x = 16, local_size_y = 16) in;

#include "utils/camera.glsl"
#include "utils/SVO.glsl"

layout(rgba8, binding = 0) uniform image2D img_output;

layout(std430, binding = 1) buffer Octree
{
    Node nodes[];
};

// Uniforms
uniform Camera u_Camera;
uniform ivec3 u_Dimensions;
uniform int u_MaxDepth;
uniform float u_VoxelSize;

// Constants
const uint8_t EMPTY_VOXEL = uint8_t(0);
const uint8_t FILLED_VOXEL = uint8_t(1);
const uint8_t VOXEL_NOT_FOUND = uint8_t(2);
const uint8_t ERROR_VOXEL = uint8_t(3);
const uint8_t ERROR_VOXEL2 = uint8_t(4);
const vec3 epson = vec3(10e-3) * u_VoxelSize;

// Get ray direction
vec3 GetRayDirection(ivec2 pixel_coords) {
    vec4 ndc = vec4(2.0 * vec2(pixel_coords) / u_Camera.ScreenSize - 1.0, -1.0, 1.0);
    vec4 clip = u_Camera.InverseProjection * ndc;
    clip = vec4(clip.xyz / clip.w, 0.0);
    vec4 world = u_Camera.InverseView * clip;
    vec3 dir = normalize(world.xyz);
    return dir;
}

// Ray-AABB intersection test
bool RayAABBIntersect(vec3 rayOrig, vec3 rayDir, vec3 boxMin, vec3 boxMax, out float tMin, out float tMax) {
    vec3 invDir = 1.0 / rayDir;
    vec3 t0 = (boxMin - rayOrig) * invDir;
    vec3 t1 = (boxMax - rayOrig) * invDir;
    vec3 tmin = min(t0, t1);
    vec3 tmax = max(t0, t1);
    tMin = max(max(tmin.x, tmin.y), tmin.z);
    tMax = min(min(tmax.x, tmax.y), tmax.z);
    return tMin <= tMax && tMax >= 0.0;
}

struct ChildIntersection {
    int childIndex;
    float tMin;
    float tMax;
};

struct StackEntry {
    int NodeID;
    vec3 currentNodeVoxMin;
    vec3 currentNodeVoxMax;
    vec3 center;
};


// Initialize StackEntry directly
void initStackEntry(out StackEntry entry, int nodeID, vec3 nodeVoxMin, vec3 nodeVoxMax) {
    entry.NodeID = nodeID;
    entry.currentNodeVoxMin = nodeVoxMin;
    entry.currentNodeVoxMax = nodeVoxMax;
    entry.center = (nodeVoxMin + nodeVoxMax) * 0.5;
}

uint8_t TraceRay(vec3 rayOrig, vec3 rayDir)
{
    rayDir = rayDir;
    vec3 voxelMin = vec3(0.0);
    vec3 voxelMax = vec3(u_Dimensions) * u_VoxelSize;

    float tMin, tMax;
    if (!RayAABBIntersect(rayOrig, rayDir, voxelMin, voxelMax, tMin, tMax))
    {
        return EMPTY_VOXEL; // No intersection with the voxel grid
    }
    // If the ray is outside the grid
    vec3 currentPos = rayOrig;
    if (!all(greaterThanEqual(currentPos, voxelMin)) || !all(lessThanEqual(currentPos, voxelMax)))
    {
        currentPos = rayOrig + rayDir * tMin + epson * rayDir;
    }


    StackEntry functionStack[12];
    int stackSize = 0;
    initStackEntry(functionStack[stackSize++], 0, voxelMin, voxelMax);

    while (stackSize > 0)
    {
        // Pop the stackSize
        StackEntry currentEntry = functionStack[--stackSize];
        Node node = nodes[currentEntry.NodeID];

        // Do a bounds check
        // Use glsl function to check position all greater than min and all less than max
        if (!(all(greaterThanEqual(currentPos, currentEntry.currentNodeVoxMin)) && all(lessThanEqual(currentPos, currentEntry.currentNodeVoxMax))))
        {
            continue;
        }

        // Check which quadrant the ray is in
        int quadrant = 0;

        vec3 childMin = currentEntry.currentNodeVoxMin;
        vec3 childMax = currentEntry.currentNodeVoxMax;


        if (currentPos.x >= currentEntry.center.x)
        {
            quadrant |= 1;
            childMin.x = currentEntry.center.x;
        }
        else
        {
            childMax.x = currentEntry.center.x;
        }
        if (currentPos.y >= currentEntry.center.y)
        {
            quadrant |= 2;
            childMin.y = currentEntry.center.y;
        }
        else
        {
            childMax.y = currentEntry.center.y;
        }
        if (currentPos.z >= currentEntry.center.z)
        {
            quadrant |= 4;
            childMin.z = currentEntry.center.z;
        }
        else
        {
            childMax.z = currentEntry.center.z;
        }

        // Check if the quadrant goes deeper
        if (bool(node.ChildrenMask & uint8_t((1 << quadrant))))
        {
            // The quadrant goes deeper
            // Push the contents of the current stack
            functionStack[stackSize++] = currentEntry;
            // Push the new child
            initStackEntry(functionStack[stackSize++], node.Children[quadrant], childMin, childMax);
            continue;
        }
        else
        {
            uint8_t voxel = node.ChildrenVoxels[quadrant];
            if (voxel != EMPTY_VOXEL)
            {
                return voxel;
            }

            // Intersection test the child and advance the ray
            float tMinChild, tMaxChild;
            RayAABBIntersect(currentPos, rayDir, childMin, childMax, tMinChild, tMaxChild);
            currentPos = currentPos + rayDir * (epson + tMaxChild);
        }

        functionStack[stackSize++] = currentEntry;
    }

    return EMPTY_VOXEL;
}

void main() {
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

    vec4 colors[5] = vec4[5](
        vec4(0.529, 0.808, 0.922, 1.0), // Sky blue
        vec4(0.0, 0.0, 1.0, 1.0), // Blue
        vec4(1.0, 0.0, 0.0, 1.0), // Red
        vec4(0.0, 0.0, 0.0, 1.0), // Black
        vec4(1.0, 1.0, 1.0, 1.0) // Magenta
    );

    vec3 rayOrig = u_Camera.Position;
    vec3 rayDir = GetRayDirection(pixel_coords);

    uint8_t materialID = TraceRay(rayOrig, rayDir);

    // Write to output image (material ID encoded as color)
    vec4 color;

    
    color = colors[uint8_t(materialID)];

    imageStore(img_output, pixel_coords, color);
}
