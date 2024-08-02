#version 430 core
#extension GL_NV_gpu_shader5 : enable
layout(local_size_x=16,local_size_y=16)in;

#include "utils/camera.glsl"

struct Voxel{
    uint8_t materialID;
};

layout(std430,binding=1)buffer Voxels{
    Voxel data[];
};

struct Material{
    vec3 color;
};
layout(std430,binding=2)buffer Materials{
    Material materials[];
};

uniform Camera u_Camera;
uniform ivec3 u_Dimensions;

const float iVoxelSize=.5;
const float tMax=100.;
const float tDelta=.1;

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

void main()
{
    // Based on the direction of ray direct, store color
    vec3 rayDirection=GetRayDirection(pixel_coords);
    vec3 currentPos=u_Camera.Position;

    vec4 colors[3] = vec4[3](
        vec4(1.,0.,0.,1.),// Red color
        vec4(0.,1.,0.,1.),// Green color
        vec4(0.,0.,1.,1.)// Blue color
    );
    
    vec4 color=vec4(.529,.808,.922,1.);// Sky blue color
    
    // Ray Marching Loop DDA
    
    float stepX=rayDirection.x>0.?1.:-1.;
    float stepY=rayDirection.y>0.?1.:-1.;
    float stepZ=rayDirection.z>0.?1.:-1.;
    
    stepX*=iVoxelSize;
    stepY*=iVoxelSize;
    stepZ*=iVoxelSize;
    
    float tDeltaX=iVoxelSize/abs(rayDirection.x);
    float tDeltaY=iVoxelSize/abs(rayDirection.y);
    float tDeltaZ=iVoxelSize/abs(rayDirection.z);
    
    float tMaxX=0;
    float tMaxY=0;
    float tMaxZ=0;
    
    for(int i=0;i<1000;i++){
        
        // if(tMaxX<tMaxY){
        //     if(tMaxX<tMaxZ){
        //         currentPos.x+=stepX;
        //         tMaxX+=tDeltaX;
        //     }else{
        //         currentPos.z+=stepZ;
        //         tMaxZ+=tDeltaZ;
        //     }
        // }else{
        //     if(tMaxY<tMaxZ){
        //         currentPos.y+=stepY;
        //         tMaxY+=tDeltaY;
        //     }else{
        //         currentPos.z+=stepZ;
        //         tMaxZ+=tDeltaZ;
        //     }
        // }

        // Optimize
        float tMaxXLessThanTMaxY = float(tMaxX < tMaxY);
        float tMaxXLessThanTMaxZ = float(tMaxX < tMaxZ);
        float tMaxYLessThanTMaxZ = float(tMaxY < tMaxZ);

        currentPos += vec3(tMaxXLessThanTMaxY * tMaxXLessThanTMaxZ * stepX, (1.0 - tMaxXLessThanTMaxY) * tMaxYLessThanTMaxZ * stepY, ((1.0 - tMaxXLessThanTMaxY) * (1.0 - tMaxYLessThanTMaxZ) + tMaxXLessThanTMaxY * (1 - tMaxXLessThanTMaxZ)) * stepZ);
        tMaxX += tMaxXLessThanTMaxY * tMaxXLessThanTMaxZ * tDeltaX;
        tMaxY += (1.0 - tMaxXLessThanTMaxY) * tMaxYLessThanTMaxZ * tDeltaY;
        tMaxZ += ((tMaxXLessThanTMaxY) * (1.0 - tMaxXLessThanTMaxZ) + (1 - tMaxXLessThanTMaxY) * (1 - tMaxYLessThanTMaxZ)) * tDeltaZ;
        
        ivec3 voxel_coords=ivec3(floor(currentPos/iVoxelSize));
        
        // TODO: Can clip the ray to the grid initially, so that we don't have to check if it's inside the grid
        if(IsInsideGrid(voxel_coords)){            
            int index = voxel_coords.x + voxel_coords.y*u_Dimensions.x + voxel_coords.z * u_Dimensions.x * u_Dimensions.y;
            
            // assume a voxel at 0,0,0 is a solid material
            if(data[index].materialID!=uint8_t(0)){
                color = vec4(materials[int(data[index].materialID) - 1].color, 1.);
                break;
            }
        }   
    }
    
    imageStore(img_output,pixel_coords,color);
    
}