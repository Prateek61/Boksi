#version 430 core
#extension GL_NV_gpu_shader5:enable
layout(local_size_x=16,local_size_y=16)in;

struct Voxel{
    uint8_t materialID;
};

layout(std430,binding=1)buffer Voxels{
    Voxel data[];
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

const float iVoxelSize=.5;
const float tMax=100.;
const float tDelta=.1;
const int maxDepth=1;

layout(rgba8,binding=0)uniform image2D img_output;

ivec2 pixel_coords=ivec2(gl_GlobalInvocationID.xy);

// Material struct to hold color information
struct Material{
    vec3 color;
};

layout(std430,binding=2)buffer Materials{
    Material materials[];
};

Material tempMaterial[5]={
    Material(vec3(1,0,0)),// Red
    Material(vec3(1.,1.,1.)),
    Material(vec3(0,0,1)),// Blue
    Material(vec3(0,1,0)),// Green
    Material(vec3(1,1,0)),// Yellow
    
};

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

void main(){
    vec3 rayDirection = GetRayDirection(pixel_coords);
    vec3 currentPos = u_Camera.Position;
    vec4 color = vec4(0);
    
    int steps = 0;
    vec3 voxel = RayMarch(currentPos, rayDirection, steps);
    
    if(voxel == vec3(0)){
        imageStore(img_output, pixel_coords, vec4(.529, .808, .922, 1.)); // Background color
        return;
    }
    
    vec3 normal = VoxelNormal(ivec3(voxel));
    color = vec4(tempMaterial[GetMaterialID(ivec3(voxel))].color, 1.);
    
    // Simple lighting
    vec3 lightDir = normalize(u_Camera.Position);
    float diffuse = max(dot(normal, lightDir), 0.);
    
    bool shadows = true;
    if(shadows){
        vec3 lightPos = vec3(0, 385, 0);
        vec3 lightDir = normalize(lightPos - voxel);
        
        vec3 shadowRayStart = voxel + lightDir * iVoxelSize;
        vec3 shadowRayDir = lightDir;
        
        int shadowSteps = 0;
        vec3 shadowVoxel = RayMarch(shadowRayStart, shadowRayDir, shadowSteps);
        
        if(shadowVoxel != vec3(0)){
            color = vec4(0);
        }
    }
    
    // Reflection calculation
    bool reflection = true;
    int reflectionCount = 5;
    
    if(reflection){
        vec3 reflDir = reflect(rayDirection, normal);
        vec3 bounce = voxel;
        vec3 reflNormal = normal;
        
        for(int x = 0; x < reflectionCount; x++){
            int steps = 0;
            reflDir = reflect(reflDir, reflNormal);
            bounce = RayMarch(bounce + reflDir * .1, reflDir, steps);
            reflNormal = VoxelNormal(ivec3(bounce));
            
            vec3 reflColor = tempMaterial[GetMaterialID(ivec3(bounce))].color;
            color.rgb += reflColor * .1; 
        }
    }
    
    imageStore(img_output, pixel_coords, color); // Foreground color
}

