#version 430 core
layout(local_size_x = 16, local_size_y = 16) in;

struct Voxel {
    uint materialID;
};

layout(std430, binding = 1) buffer Voxels {
    Voxel data[];
};

struct Camera {
    vec3 Position;
    vec3 Up;
    float FOV;
    float FocalLength;
    float AspectRatio;
    
    vec3 LowerLeftCorner;
    vec3 Horizontal;
    vec3 Vertical;
    float PixelWidth;
    vec2 ScreenSize;
};

uniform Camera u_Camera;
uniform int u_Dim;

layout(rgba8, binding = 0) uniform image2D img_output;

void main() {
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = vec4(0.529, 0.808, 0.922, 1.0); // Default color (sky blue)

    float voxelSize = 0.05;
    float iVoxelSize = 1.0 / voxelSize;
    float epsilon = 1e-6;

    vec3 rayOrigin = u_Camera.Position;
    vec3 pixelPos = u_Camera.LowerLeftCorner + u_Camera.Horizontal * (u_Camera.PixelWidth * pixelCoords.x) + u_Camera.Vertical * (u_Camera.PixelWidth * pixelCoords.y);
    vec3 rayDirection = normalize(pixelPos - rayOrigin);

    vec3 currentPos = rayOrigin;

    float stepX = (rayDirection.x > 0) ? 1.0 : -1.0;
    float stepY = (rayDirection.y > 0) ? 1.0 : -1.0;
    float stepZ = (rayDirection.z > 0) ? 1.0 : -1.0;

    float tDeltaX = (abs(rayDirection.x) > epsilon) ? iVoxelSize / abs(rayDirection.x) : 1e10;
    float tDeltaY = (abs(rayDirection.y) > epsilon) ? iVoxelSize / abs(rayDirection.y) : 1e10;
    float tDeltaZ = (abs(rayDirection.z) > epsilon) ? iVoxelSize / abs(rayDirection.z) : 1e10;

    float tMaxX = 0.0;
    float tMaxY = 0.0;
    float tMaxZ = 0.0;

    for (int i = 0; i < 1000; i++) {
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                currentPos.x += stepX * voxelSize;
                tMaxX += tDeltaX;
            } else {
                currentPos.z += stepZ * voxelSize;
                tMaxZ += tDeltaZ;
            }
        } else {
            if (tMaxY < tMaxZ) {
                currentPos.y += stepY * voxelSize;
                tMaxY += tDeltaY;
            } else {
                currentPos.z += stepZ * voxelSize;
                tMaxZ += tDeltaZ;
            }
        }

        ivec3 voxel_coords = ivec3(floor(currentPos / voxelSize));
        voxel_coords = clamp(voxel_coords, ivec3(0), ivec3(u_Dim - 1));

        int index = voxel_coords.x + voxel_coords.y * u_Dim + voxel_coords.z * u_Dim * u_Dim;

        if (data[index].materialID == 1) {
            color = vec4(1.0, 0.0, 0.0, 1.0); // Red
            break;
        } else if (data[index].materialID == 2) {
            color = vec4(0.0, 1.0, 0.0, 1.0); // Green
            break;
        } else if (data[index].materialID == 3) {
            color = vec4(0.0, 0.0, 1.0, 1.0); // Blue
            break;
        }
    }

    imageStore(img_output, pixelCoords, color);
}
