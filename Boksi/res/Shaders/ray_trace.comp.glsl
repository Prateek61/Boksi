#version 430 core
layout(local_size_x=16,local_size_y=16)in;

struct Voxel{
    uint materialID;
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

const int u_Dim=64;// Ensure this matches your voxel grid size
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
    dir*=-1;
    return dir;
}

void main()
{
    // Based on the direction of ray direct, store color
    vec3 rayDirection=GetRayDirection(pixel_coords);
    vec3 currentPos=u_Camera.Position;
    
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
        
        if(tMaxX<tMaxY){
            if(tMaxX<tMaxZ){
                currentPos.x+=stepX;
                tMaxX+=tDeltaX;
            }else{
                currentPos.z+=stepZ;
                tMaxZ+=tDeltaZ;
            }
        }else{
            if(tMaxY<tMaxZ){
                currentPos.y+=stepY;
                tMaxY+=tDeltaY;
            }else{
                currentPos.z+=stepZ;
                tMaxZ+=tDeltaZ;
            }
        }
        
        ivec3 voxel_coords=ivec3(floor(currentPos/iVoxelSize));
        
        if(voxel_coords.x>=0&&voxel_coords.x<u_Dim&&
            voxel_coords.y>=0&&voxel_coords.y<u_Dim&&
        voxel_coords.z>=0&&voxel_coords.z<u_Dim){
            
            int index=voxel_coords.x+voxel_coords.y*u_Dim+voxel_coords.z*u_Dim*u_Dim;
            
            // assume a voxel at 0,0,0 is a solid material
            if(data[index].materialID==1){
                color=vec4(1.,0.,0.,1.);// Red color
                break;
            }
            if(data[index].materialID==2){
                color=vec4(0.,1.,0.,1.);// Green color
                break;
            }
            if(data[index].materialID==3){
                color=vec4(0.,0.,1.,1.);// Blue color
                break;
            }
            
        }
        
    }
    
    imageStore(img_output,pixel_coords,color);
    
}
