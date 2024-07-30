#version 330 core

layout(location=0)out vec4 color;
in vec3 v_Position;

uniform vec4 u_Color;

struct Camera
{
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

vec4 voxels[1]=vec4[](vec4(1.,.8,.6,.4));

void main()
{
    int dim=1;
    float voxelSize=.05;

    vec3 rayOrigin=u_Camera.Position;
    vec3 currentPos=rayOrigin;
    
    vec3 rayDirection=normalize(u_Camera.LowerLeftCorner+u_Camera.Horizontal*v_Position.x+u_Camera.Vertical*v_Position.y-u_Camera.Position);
    
    float stepX=rayDirection.x>0.?1.:-1.;
    float stepY=rayDirection.y>0.?1.:-1.;
    float stepZ=rayDirection.z>0.?1.:-1.;
    
    stepX*=voxelSize;
    stepY*=voxelSize;
    stepZ*=voxelSize;
    
    float tDeltaX=voxelSize/abs(rayDirection.x);
    float tDeltaY=voxelSize/abs(rayDirection.y);
    float tDeltaZ=voxelSize/abs(rayDirection.z);
    
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
        
        ivec3 voxel_coords=ivec3(floor(currentPos/voxelSize));
        
        if(voxel_coords.x>=0&&voxel_coords.x<dim&&
            voxel_coords.y>=0&&voxel_coords.y<dim&&
        voxel_coords.z>=0&&voxel_coords.z<dim){
            
            int index=voxel_coords.x+voxel_coords.y*dim+voxel_coords.z*dim*dim;
            
            if(voxels[index].x==1.){
                color=vec4(voxels[index].yzw,1.);
                break;
            }
            
        }
        // Sky color if no hit
        color=vec4(.529,.808,.922,1.);// Sky blue color
    }
}