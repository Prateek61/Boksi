// SVO path tracing as described in the paper "Ray Tracing with Selective Refinement for Efficient and Detailed Voxel Rendering" by Philipp Slusallek and Ingo Wald.

#version 430 core
#extension GL_NV_gpu_shader5 : enable
layout(local_size_x = 16, local_size_y = 16) in;

#include "utils/camera.glsl"
#include "utils/SVO.glsl"
#include "utils/Material.glsl"

layout(rgba8, binding = 0) uniform image2D img_output; // Output texture
layout(std430, binding = 1) buffer Octree { Node nodes[]; }; // Octree buffer
layout(std430, binding = 2) buffer Materials { Material materials[]; }; // Materials buffer

// Uniforms
uniform Camera u_Camera;
uniform ivec3 u_Dimensions;
uniform int u_MaxDepth;
uniform float u_VoxelSize;
uniform float u_Time = 0.25;

// Function definitions
// Get ray direction
vec3 GetRayDirection(ivec2 pixel_coords);
bool RayAABBIntersect(vec3 rayOrig, vec3 rayDir, vec3 boxMin, vec3 boxMax, out float tMin, out float tMax);

#define EMPTY_VOXEL uint16_t(0) // Sky blue
#define MATERIAL_ID_TYPE uint16_t

// Define epson as the base 2 exponential of 23 (mantissa bits)
const int s_max = 23;
const float epson = exp2(-s_max);
const uint8_t uint8_zero = uint8_t(0);
const uint16_t uint16_zero = uint16_t(0);

const vec3 NightColor = vec3(.15, 0.3, 0.6);
const vec3 HorizonColor = vec3(0.6, 0.3, 0.4);
const vec3 DayColor = vec3(0.7, 0.8, 1);

const vec3 SunColor = vec3(1.0, 0.8, 0.6);
const vec3 SunRimColor = vec3(1.0, 0.66, 0.33);

struct Stack
{
    int parentIdx;
    float t_max;
};
MATERIAL_ID_TYPE TraceRay(vec3 rayOrig, vec3 rayDir, int root, out vec3 hitPos, out float hit_t, out int parent_idx, out int hit_idx, out int hit_scale)
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
    // t_max = min(t_max, 1.0);

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
        uint8_t child_getter_mask = uint8_t(1 << (idx ^ octant_mask ^ 7));
        if ((parent.ValidMask & child_getter_mask) != uint8_zero && t_min < tc_max)
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
                if ((parent.ChildrenMask & child_getter_mask) == uint8_zero)
                {
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
                parentIdx = parent.Children[idx ^ octant_mask ^ 7];
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

    float outside = float(scale >= s_max);
    t_min = 2.0 * outside + t_min * (1 - outside);

    // Undo mirroring of the coordinate system
    if ((octant_mask & 1) == 0) { pos.x = 3.0f - scale_exp2 - pos.x; }
    if ((octant_mask & 2) == 0) { pos.y = 3.0f - scale_exp2 - pos.y; }
    if ((octant_mask & 4) == 0) { pos.z = 3.0f - scale_exp2 - pos.z; }

    hit_t = t_min;
    hitPos.x = min(max(rayOrig.x + t_min * rayDir.x, pos.x + epson), pos.x + scale_exp2 - epson);
    hitPos.y = min(max(rayOrig.y + t_min * rayDir.y, pos.y + epson), pos.y + scale_exp2 - epson);
    hitPos.z = min(max(rayOrig.z + t_min * rayDir.z, pos.z + epson), pos.z + scale_exp2 - epson);
    parent_idx = parentIdx;

    idx = idx ^ octant_mask ^ 7;
    hit_idx = idx;
    hit_scale = scale;

    return EMPTY_VOXEL * MATERIAL_ID_TYPE(outside) + parent.ChildrenVoxels[idx] * MATERIAL_ID_TYPE(1. - outside);
}

vec3 TransformVoxelSpaceToOctreeSpace(vec3 voxelPos);
vec3 TransformWorldSpaceToOctreeSpace(vec3 worldPos);
vec3 TransformOctreeSpaceToWorldSpace(vec3 octreePos);
vec3 TransformOCtreeSpaceToVoxelSpace(vec3 octreePos);

vec3 GetSunDirection();
vec4 GetSkyColor(vec3 dir);

void main()
{
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

    vec3 rayOrig = u_Camera.Position;
    vec3 rayDir = GetRayDirection(pixel_coords);

    // Perform intersection test and check if ray hits the octree
    float tMin, tMax;
    if (!RayAABBIntersect(rayOrig, rayDir, vec3(0), vec3(u_Dimensions) * u_VoxelSize, tMin, tMax))
    {
        imageStore(img_output, pixel_coords, GetSkyColor(rayDir));
        return;
    }
    if (!(tMin <= 0))
    {
        rayOrig += rayDir * (tMin + 0.01 * u_VoxelSize);
    }

    vec3 transformedOrig = TransformWorldSpaceToOctreeSpace(rayOrig);
    // Trace the ray
    vec3 hitPos;
    int parent_idx;
    int hit_idx;
    int hit_scale;
    float hit_t;
    MATERIAL_ID_TYPE materialID = TraceRay(transformedOrig, rayDir, 0, hitPos, hit_t, parent_idx, hit_idx, hit_scale);
    vec4 color;
    if (materialID == EMPTY_VOXEL)
    {
        color = GetSkyColor(rayDir);
    }
    else
    {
        color = vec4(materials[materialID].color, 1.0);

        // Compute shadows
        vec3 lightDir = GetSunDirection();

        vec3 shadowOrig = hitPos + lightDir * 0.001 * (u_VoxelSize / u_Dimensions);
        vec3 shadowHitPos;
        int shadowParentIdx;
        int shadowHitIdx;
        int shadowHitScale;
        float shadowHitT;
        MATERIAL_ID_TYPE shadowMaterialID = TraceRay(shadowOrig, lightDir, 0, shadowHitPos, shadowHitT, shadowParentIdx, shadowHitIdx, shadowHitScale);
        if (shadowMaterialID != EMPTY_VOXEL)
        {
            color *= 0.5;
        }
    }

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
    // Make sure rayDir is not zero
    if (abs(rayDir.x) < epson) rayDir.x = sign(rayDir.x) * epson;
    if (abs(rayDir.y) < epson) rayDir.y = sign(rayDir.y) * epson;
    if (abs(rayDir.z) < epson) rayDir.z = sign(rayDir.z) * epson;

    vec3 invDir = 1.0 / rayDir;
    vec3 t0 = (boxMin - rayOrig) * invDir;
    vec3 t1 = (boxMax - rayOrig) * invDir;
    vec3 tmin = min(t0, t1);
    vec3 tmax = max(t0, t1);
    tMin = max(max(tmin.x, tmin.y), tmin.z);
    tMax = min(min(tmax.x, tmax.y), tmax.z);
    return tMin <= tMax && tMax >= 0.0;
}

vec3 TransformVoxelSpaceToOctreeSpace(vec3 voxelPos)
{
    // Voxel space to [0, 1] space
    voxelPos /= float(u_Dimensions.x);
    // [0, 1] space to [1, 2] space
    voxelPos += vec3(1.0);
    return voxelPos;
}

vec3 TransformWorldSpaceToOctreeSpace(vec3 worldPos)
{
    // World Space to Voxel Space
    worldPos /= u_VoxelSize;
    // Voxel Space to [0, 1] Space
    worldPos /= float(u_Dimensions.x);
    // [0, 1] Space to [1, 2] Space
    worldPos += vec3(1.0);
    return worldPos;
}

vec3 TransformOctreeSpaceToWorldSpace(vec3 octreePos)
{
    // [1, 2] Space to [0, 1] Space
    octreePos -= vec3(1.0);
    // [0, 1] Space to Voxel Space
    octreePos *= float(u_Dimensions.x);
    // Voxel Space to World Space
    octreePos *= u_VoxelSize;
    return octreePos;
}

vec3 TransformOCtreeSpaceToVoxelSpace(vec3 octreePos)
{
    // [1, 2] Space to [0, 1] Space
    octreePos -= vec3(1.0);
    // [0, 1] Space to Voxel Space
    octreePos *= float(u_Dimensions.x);
    return octreePos;
}

vec4 GetSkyColor(vec3 dir)
{
    vec3 sun_dir = GetSunDirection();
    float sun = clamp( dot(sun_dir, dir), 0.0, 1.0 );
    float sun_height = sun_dir.y;

    float neight_height = -0.8;
    float day_height = 0.3;

    float horizon_length = day_height - neight_height;
    float inverse_hl = 1.0 / horizon_length;
    float half_horizon_length = horizon_length * 0.5;
    float inverse_hhl = 1.0 / half_horizon_length;
    float mid_point = neight_height + half_horizon_length;

    float night_contrib = clamp((sun_height - mid_point) * (-inverse_hhl), 0.0, 1.0);
    float horizon_contrib = -clamp(abs((sun_height - mid_point) * (-inverse_hhl)), 0.0, 1.0) + 1.0;
    float day_contrib = clamp((sun_height - mid_point) * inverse_hhl, 0.0, 1.0);

    // sky color
    vec3 sky_color = NightColor * night_contrib + HorizonColor * horizon_contrib + DayColor * day_contrib;

    vec3 color = sky_color;

    // atmosphere brighter near horizon
    color -= clamp(dir.y, 0.0, 0.5);

    // draw sun
    color += 0.4 * SunRimColor * pow(sun, 4.0);
    color += 1.0 * SunColor * pow(sun, 2000.0);

    return vec4(color, 1.0);
}

vec3 GetSunDirection()
{
    float sun_speed  = 0.35 * u_Time;
    vec3 sun_dir = normalize(vec3(cos(sun_speed), sin(sun_speed), 0.0));
    sun_dir = normalize(vec3(1.));
    return sun_dir;
}