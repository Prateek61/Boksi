// SVO path tracing as described in the paper "Ray Tracing with Selective Refinement for Efficient and Detailed Voxel Rendering" by Philipp Slusallek and Ingo Wald.

#version 430 core
#extension GL_NV_gpu_shader5 : enable
layout(local_size_x = 16, local_size_y = 16) in;

#include "utils/camera.glsl"
#include "utils/SVO.glsl"
#include "utils/Material.glsl"

layout(rgba8, binding = 0) uniform image2D img_output; // Output texture
layout(std430, binding = 0) buffer Octree { Node nodes[]; }; // Octree buffer
layout(std430, binding = 1) buffer Materials { Material materials[]; }; // Materials buffer

// Uniforms
uniform Camera u_Camera;
uniform ivec3 u_Dimensions = ivec3(256);
uniform int u_MaxDepth;
uniform float u_VoxelSize = 1.0f;

// Function definitions
// Get ray direction
vec3 GetRayDirection(ivec2 pixel_coords);
bool RayAABBIntersect(vec3 rayOrig, vec3 rayDir, vec3 boxMin, vec3 boxMax, out float tMin, out float tMax);

#define EMPTY_VOXEL uint8_t(0) // Sky blue
#define FILLED_VOXEL uint8_t(1) // Pink
#define VOXEL_NOT_FOUND uint8_t(2) // White
#define ERROR_VOXEL uint8_t(3) // Black
#define ERROR_VOXEL2 uint8_t(4) // White
#define ERROR_VOXEL3 uint8_t(5) // Green

// Define epson as the base 2 exponential of 23 (mantissa bits)
const int s_max = 23;
const float epson = exp2(-s_max);
const uint8_t uint_zero = uint8_t(0);

struct Stack
{
    int parentIdx;
    float t_max;
};
uint8_t TraceRay(vec3 rayOrig, vec3 rayDir, int root, out vec3 hitPos, out float hit_t, out int parent_idx)
{
    // Initialize the stack
    Stack stack[s_max + 1]; // Stack of parent Voxels

    // Get rid of small ray direction components to avoid division by zero
    if (abs(rayDir.x) < epson) rayDir.x = sign(rayDir.x) * epson;
    if (abs(rayDir.y) < epson) rayDir.y = sign(rayDir.y) * epson;
    if (abs(rayDir.z) < epson) rayDir.z = sign(rayDir.z) * epson;

    // Precompute the coefficients of tx(x), ty(y), and tz(z)
    // The octree is assumed to reside at coordinates [1, 2]
    float tx_coef = 1.0 / -abs(rayDir.x);
    float ty_coef = 1.0 / -abs(rayDir.y);
    float tz_coef = 1.0 / -abs(rayDir.z);

    float tx_bias = tx_coef * rayOrig.x;
    float ty_bias = ty_coef * rayOrig.y;
    float tz_bias = tz_coef * rayOrig.z;

    // Select octant mask to mirrorthe coordinate system so
    // that the ray direction is negative in all dimensions
    int octant_mask = 7;
    if (rayDir.x > 0.0) { octant_mask ^= 1; tx_bias = 3.0f * tx_coef - tx_bias; }
    if (rayDir.y > 0.0) { octant_mask ^= 2; ty_bias = 3.0f * ty_coef - ty_bias; }
    if (rayDir.z > 0.0) { octant_mask ^= 4; tz_bias = 3.0f * tz_coef - tz_bias; }

    // Initialize the active span of t-values
    float t_min = max(max(2.0 * tx_coef - tx_bias, 2.0 * ty_coef - ty_bias), 2.0 * tz_coef - tz_bias);
    float t_max = min(min(tx_coef - tx_bias, ty_coef - ty_bias), tz_coef - tz_bias);
    float h = t_max;
    t_min = max(t_min, 0.0);
    t_max = min(t_max, h);

    // Initialize the current voxel to the first child of the root
    Node parent = nodes[root];
    int parentIdx = root;
    int idx = 0;
    vec3 pos = vec3(1.0);
    int scale = s_max - 1;
    float scale_exp2 = 0.5;

    if (1.5 * tx_coef - tx_bias > t_min) { idx ^= 1; pos.x = 1.5; }
    if (1.5 * ty_coef - ty_bias > t_min) { idx ^= 2; pos.y = 1.5; }
    if (1.5 * tz_coef - tz_bias > t_min) { idx ^= 4; pos.z = 1.5; }

    // Traverse voxels along the ray as long as the current voxel
    // stays within the octree

    while (scale < s_max)
    {
        // Fetch child descriptor
        // Already there in the parent node

        // Determine maximmum t-value of the cube by evaluating
        // tx(), ty(), and tz() at its corners
        float tx_corner = pos.x * tx_coef - tx_bias;
        float ty_corner = pos.y * ty_coef - ty_bias;
        float tz_corner = pos.z * tz_coef - tz_bias;
        float tc_max = min(min(tx_corner, ty_corner), tz_corner);

        // Process voxel if the corresponding bit in valid mask set
        // and the active t-span is non-empty
        uint8_t child_getter_mask = uint8_t(1 << idx);
        if ((parent.ValidMask & child_getter_mask) != uint_zero && t_min < tc_max)
        {
            // Terminate if the voxel is small enough
            // TODO: Implement LOD

            // Intersect active t-span with the cube and evaluate
            // tx(), ty(), and tz() at the center of the voxel

            float tv_max = min(t_max, tc_max);
            float half_vox = scale_exp2 * 0.5;
            float tx_center = half_vox * tx_coef + tx_corner;
            float ty_center = half_vox * ty_coef + ty_corner;
            float tz_center = half_vox * tz_coef + tz_corner;

            // Intersect with contour if the corresponding bit in the countour mask is set
            // I am not using countours for now

            // Descend to the first child if the resulting t-span is non-empty
            if (t_min <= tv_max)
            {
                // Terminate if the corresponding bit in the children mask is not set
                if ((parent.ValidMask & child_getter_mask) == uint_zero)
                {
                    return FILLED_VOXEL;
                    break;
                }

                // PUSH
                // Write current parent to the stack.
                if (tc_max < h)
                {
                    stack[scale].parentIdx = parentIdx;
                    stack[scale].t_max = t_max;
                }
                h = tc_max;

                // Update the parent
                parentIdx = parent.Children[idx];
                parent = nodes[parentIdx];

                // Select child voxel that the ray enters next
                idx = 0;
                scale--;
                scale_exp2 = half_vox;

                if (tx_center > t_min) { idx ^= 1; pos.x += scale_exp2; }
                if (ty_center > t_min) { idx ^= 2; pos.y += scale_exp2; }
                if (tz_center > t_min) { idx ^= 4; pos.z += scale_exp2; }

                // Update active t-span
                t_max = tv_max;
                continue;
            }
        }

        // Advance
        // Step along the ray
        int step_mask = 0;
        if (tx_corner <= tc_max) { step_mask ^= 1; pos.x -= scale_exp2; }
        if (ty_corner <= tc_max) { step_mask ^= 2; pos.y -= scale_exp2; }
        if (tz_corner <= tc_max) { step_mask ^= 4; pos.z -= scale_exp2; }

        // Update active t-span and flip bits of the child slot index
        t_min = tc_max;
        idx ^= step_mask;

        // Proceed with POP if the bit flips disagree with the ray direction
        if ((idx & step_mask) != 0)
        {
            // POP 
            // Find the highest differing bit between two positions.

            unsigned int differing_bits = 0;
            if ((step_mask & 1) != 0)
            {
                differing_bits |= floatBitsToInt(pos.x) ^ floatBitsToInt(pos.x + scale_exp2);
            }
            if ((step_mask & 2) != 0)
            {
                differing_bits |= floatBitsToInt(pos.y) ^ floatBitsToInt(pos.y + scale_exp2);
            }
            if ((step_mask & 4) != 0)
            {
                differing_bits |= floatBitsToInt(pos.z) ^ floatBitsToInt(pos.z + scale_exp2);
            }
            scale = (floatBitsToInt((float)differing_bits) >> 23) - 127; // Position of the highest differing bit
            scale_exp2 = intBitsToFloat((scale - s_max + 127) << 23); // exp2d(scale - s_max)

            // Restore parent voxel from the stack

            Stack StackEntry = stack[scale];
            parentIdx = StackEntry.parentIdx;
            parent = nodes[parentIdx];
            t_max = StackEntry.t_max;

            // Round cube position and extract child slot index
            int shx = floatBitsToInt(pos.x) >> scale;
            int shy = floatBitsToInt(pos.y) >> scale;
            int shz = floatBitsToInt(pos.z) >> scale;
            pos.x = intBitsToFloat(shx << scale);
            pos.y = intBitsToFloat(shy << scale);
            pos.z = intBitsToFloat(shz << scale);
            idx = (shx & 1) | ((shy & 1) << 1) | ((shz & 1) << 2);

            // Prevent same parent from being stored again
            h = 0.0;
        }
    }

    // Indicate miss if we are outside the octree
    if (scale >= s_max)
    {
        t_min = 2.0f;
    }

    // Undo mirroring of the coordinate system
    if ((octant_mask & 1) == 0) { pos.x = 3.0f - scale_exp2 - pos.x; }
    if ((octant_mask & 2) == 0) { pos.y = 3.0f - scale_exp2 - pos.y; }
    if ((octant_mask & 4) == 0) { pos.z = 3.0f - scale_exp2 - pos.z; }

    hit_t = t_min;
    hitPos.x = min(max(rayOrig.x + t_min * rayDir.x, pos.x + epson), pos.x + scale_exp2 - epson);
    hitPos.y = min(max(rayOrig.y + t_min * rayDir.y, pos.y + epson), pos.y + scale_exp2 - epson);
    hitPos.z = min(max(rayOrig.z + t_min * rayDir.z, pos.z + epson), pos.z + scale_exp2 - epson);
    parent_idx = parentIdx;
    return parent.ChildrenVoxels[idx];
}

void main()
{
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

    vec3 rayOrig = u_Camera.Position;
    vec3 rayDir = GetRayDirection(pixel_coords);

    vec3 transformedOrig = u_Camera.Position;
    // World Space to Voxel Space
    transformedOrig /= u_VoxelSize;
    // Voxel Space to [0, 1] Space
    transformedOrig /= float(u_Dimensions.x);
    // [0, 1] Space to [1, 2] Space
    transformedOrig += vec3(1.0);

    // Perform intersection test and check if ray hits the octree
    float tMin, tMax;
    if (!RayAABBIntersect(transformedOrig, rayDir, vec3(1.), vec3(2.), tMin, tMax))
    {
        imageStore(img_output, pixel_coords, vec4(materials[EMPTY_VOXEL].color, 1));
        return;
    }
    // if (!(tMin <= 0.0))
    // {
    //     transformedOrig += rayDir * (tMin + epson);
    // }

    vec3 hitPos;
    int parent_idx;
    float hit_t;
    uint8_t materialID = TraceRay(transformedOrig, rayDir, 0, hitPos, hit_t, parent_idx);

    if (hit_t >= 2.0)
    {
        materialID = ERROR_VOXEL;
    }

    vec4 color = vec4(materials[materialID].color, 1.0);

    imageStore(img_output, pixel_coords, color);
}

// Get ray direction
vec3 GetRayDirection(ivec2 pixel_coords) {
    vec4 ndc = vec4(2.0 * vec2(pixel_coords) / u_Camera.ScreenSize - 1.0, -1.0, 1.0);
    vec4 clip = u_Camera.InverseProjection * ndc;
    clip = vec4(clip.xyz / clip.w, 0.0);
    vec4 world = u_Camera.InverseView * clip;
    vec3 dir = normalize(world.xyz);
    return dir;
}

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
