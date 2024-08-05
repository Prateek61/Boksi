#version 430 core
#extension GL_NV_gpu_shader5:enable
layout(local_size_x=16,local_size_y=16)in;

#include "utils/Material.glsl"

struct Voxel{
    uint8_t materialID;
};

layout(std430, binding=0) buffer Voxels{
    Voxel data[];
};

layout(std430, binding=1)buffer Materials{
    Material materials[];
};

struct Camera{
    mat4 InverseProjection;
    mat4 InverseView;
    vec3 Position;
    vec3 ForwardDirection;
    vec2 ScreenSize;
};
uniform Camera u_Camera;
uniform ivec3 u_Dimensions;
uniform float u_Exposure;
uniform float u_ShadowStrength;
uniform float u_AO;
uniform float u_ShadowBias;
uniform vec3 u_LightPosition;


const float iVoxelSize=.5;
const float tMax=100.;
const float tDelta=.1;
const int maxDepth=1;

layout(rgba8,binding=0)uniform image2D img_output;

ivec2 pixel_coords=ivec2(gl_GlobalInvocationID.xy);

vec3 GetRayDirection(ivec2 pixel_coords){
    vec4 ndc=vec4(2.*vec2(pixel_coords)/u_Camera.ScreenSize-1.,1.,1.);
    vec4 clip=u_Camera.InverseProjection*ndc;
    clip=vec4(clip.xyz/clip.w,0.);
    vec4 world=u_Camera.InverseView*clip;
    vec3 dir=normalize(world.xyz);
    return dir;
}

bool IsInsideGrid(ivec3 voxel_coords){
    return all(greaterThanEqual(voxel_coords,ivec3(0)))&&all(lessThan(voxel_coords,u_Dimensions));
}

float random(vec2 p){
    const vec2 r=vec2(
        23.1406926327792690,// e^pi (Gelfond's constant)
        2.6651441426902251// 2^sqrt(2) (Gelfond–Schneider constant)
    );
    return fract(cos(mod(123456789.,1e-7+256.*dot(p,r))));
}

float rand(){
    return random(pixel_coords);
}

vec3 RandomRayDirection(vec3 rayDirection){
    return normalize(rayDirection+vec3(2.*(rand()-.5),2.*(rand()-.5),2.*(rand()-.5)));
}

uint8_t GetMaterialID(ivec3 voxel_coords){
    if(!IsInsideGrid(voxel_coords))return uint8_t(0);
    return data[voxel_coords.x+voxel_coords.y*u_Dimensions.x+voxel_coords.z*u_Dimensions.x*u_Dimensions.y].materialID;
}

vec3 VoxelNormal(ivec3 voxel_coords){
    if(!IsInsideGrid(voxel_coords))return vec3(0);
    
    float dx=float(GetMaterialID(voxel_coords+ivec3(1,0,0)))-float(GetMaterialID(voxel_coords-ivec3(1,0,0)));
    float dy=float(GetMaterialID(voxel_coords+ivec3(0,1,0)))-float(GetMaterialID(voxel_coords-ivec3(0,1,0)));
    float dz=float(GetMaterialID(voxel_coords+ivec3(0,0,1)))-float(GetMaterialID(voxel_coords-ivec3(0,0,1)));
    
    return normalize(vec3(dx,dy,dz));
}

vec3 RayMarch(vec3 start,vec3 dir,int steps){
    vec3 result=vec3(0);
    vec3 stepsize=1./abs(dir);
    vec3 toboundry=(sign(dir)*.5+.5-fract(start))/dir;
    ivec3 voxel=ivec3(floor(start));
    int maxsteps=1000;
    
    while(true){
        bvec3 mask=lessThanEqual(toboundry,min(toboundry.yzx,toboundry.zxy));
        toboundry+=vec3(mask)*stepsize;
        voxel+=ivec3(vec3(mask))*ivec3(sign(dir));
        steps++;
        
        bool hit=GetMaterialID(voxel)!=uint8_t(0);
        bool toofar=steps>maxsteps;
        bool outside=!IsInsideGrid(voxel);
        bool anything=hit||toofar||outside;
        
        if(hit){
            result=vec3(voxel);
            break;
        }
        if(toofar){
            result=vec3(0);
            break;
        }
    }
    
    return result;
}

void main() {
    vec3 rayDirection = GetRayDirection(pixel_coords); // Calculate the ray direction from the pixel coordinates
    vec3 currentPos = u_Camera.Position; // Start position of the ray is the camera position
    vec4 color = vec4(0); // Initialize the color to black
    
    int steps = 0;
    vec3 voxel = RayMarch(currentPos, rayDirection, steps); // Perform ray marching to find the first voxel hit
    
    if (voxel == vec3(0)) { // If no voxel was hit
        imageStore(img_output, pixel_coords, vec4(0.529, 0.808, 0.922, 1.0)); // Set the background color
        return;
    }
    
    vec3 normal = VoxelNormal(ivec3(voxel)); // Calculate the normal at the voxel
    vec3 albedo = materials[GetMaterialID(ivec3(voxel))].color; // Get the color of the material at the voxel
    
    
    vec3 lightPos = u_LightPosition; // Position of the light source

    vec3 lightDir = normalize(lightPos - voxel); // Direction from the voxel to the light source
        

    float shadow = 1;
    
    bool shadows = true;
    if (shadows) {
        vec3 shadowRayStart = voxel + lightDir +  normal * u_ShadowBias; // Start the shadow ray slightly off the voxel surface to avoid self-shadowing
        vec3 shadowRayDir = lightDir; // Direction of the shadow ray
        
        int shadowSteps = 0;
        vec3 shadowVoxel = RayMarch(shadowRayStart, shadowRayDir, shadowSteps); // Perform ray marching to check for shadows
        
        if (shadowVoxel != vec3(0)) { // If a voxel is hit by the shadow ray
            shadow = u_ShadowStrength;
            
        }
    }
    
    // Calculate the ambient occlusion (optional, not implemented in the provided code)
    
    // Calculate the final shaded color
    vec3 shaded = albedo * shadow;
    color = vec4(shaded, 1.0); // Set the final color
    
    imageStore(img_output, pixel_coords, color); // Store the color in the output image
}

