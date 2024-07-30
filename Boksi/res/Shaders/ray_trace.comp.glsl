#version 430 core
layout(local_size_x=16,local_size_y=16)in;

struct Voxel{
    uint materialID;
};

layout(std430, binding=1)buffer Voxels{
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

layout(rgba8, binding=0)uniform image2D img_output;

ivec2 pixel_coords=ivec2(gl_GlobalInvocationID.xy);

vec3 rayDirection(ivec2 pixel_coords)
{
    vec4 ndc=vec4(2.*vec2(pixel_coords)/u_Camera.ScreenSize-1.,1.,1.);
    vec4 clip=u_Camera.InverseProjection*ndc;
    clip = vec4(clip.xyz / clip.w, 0.0);
    vec4 world=u_Camera.InverseView*clip;
    vec3 dir = normalize(world.xyz);
    dir *= -1;
    return dir;
}

void main()
{
    // Based on the direction of ray direct, store color
    vec3 rayDir=rayDirection(pixel_coords);

    // Random shpere

    // Place a sphere at 0, 0, 50 of radius 10
    vec3 sphereCenter=vec3(0.,0.,50.);
    float sphereRadius=10.;
    vec3 origin=u_Camera.Position;
    
    // If the ray intersects the sphere, color the pixel red, else color it blue
    vec3 oc=origin-sphereCenter;
    float a=dot(rayDir,rayDir);
    float b=2.*dot(oc,rayDir);
    float c=dot(oc,oc)-sphereRadius*sphereRadius;
    float discriminant=b*b-4.*a*c;
    if(discriminant>0.){
        vec3 p=(-b-sqrt(discriminant))/(2.*a)*rayDir;
        vec3 normal=normalize(p-sphereCenter);
        vec3 color=.5*(normal+1.);
        imageStore(img_output,pixel_coords,vec4(color,1.));
    }else{
        imageStore(img_output,pixel_coords,vec4(.529,.808,.922,1.));
    }
}

// void main(){
//     ivec2 pixel_coords=ivec2(gl_GlobalInvocationID.xy);
    
//     int dim=u_Dim;
    
//     vec3 lookFrom=u_Camera.Position;
//     vec3 lookAt=lookFrom+u_Camera.FocalLength*normalize(u_Camera.LowerLeftCorner+
//         float(pixel_coords.x)/float(u_Camera.ScreenSize.x)*u_Camera.Horizontal+
//         float(pixel_coords.y)/float(u_Camera.ScreenSize.y)*u_Camera.Vertical);
        
//         vec3 origin=lookFrom;
//         vec3 rayDirection=normalize(lookAt-lookFrom);
        
//         float maxDistance=100.;// Max march distance
        
//         vec3 currentPos=origin;
//         vec4 color=vec4(.529,.808,.922,1.);// Sky blue color
        
//         // Ray Marching Loop DDA
        
//         float stepX=rayDirection.x>0.?1.:-1.;
//         float stepY=rayDirection.y>0.?1.:-1.;
//         float stepZ=rayDirection.z>0.?1.:-1.;
        
//         stepX*=iVoxelSize;
//         stepY*=iVoxelSize;
//         stepZ*=iVoxelSize;
        
//         float tDeltaX=iVoxelSize/abs(rayDirection.x);
//         float tDeltaY=iVoxelSize/abs(rayDirection.y);
//         float tDeltaZ=iVoxelSize/abs(rayDirection.z);
        
//         float tMaxX=0;
//         float tMaxY=0;
//         float tMaxZ=0;
        
//         for(int i=0;i<1000;i++){
            
//             if(tMaxX<tMaxY){
//                 if(tMaxX<tMaxZ){
//                     currentPos.x+=stepX;
//                     tMaxX+=tDeltaX;
//                 }else{
//                     currentPos.z+=stepZ;
//                     tMaxZ+=tDeltaZ;
//                 }
//             }else{
//                 if(tMaxY<tMaxZ){
//                     currentPos.y+=stepY;
//                     tMaxY+=tDeltaY;
//                 }else{
//                     currentPos.z+=stepZ;
//                     tMaxZ+=tDeltaZ;
//                 }
//             }
            
//             ivec3 voxel_coords=ivec3(floor(currentPos/iVoxelSize));
            
//             if(voxel_coords.x>=0&&voxel_coords.x<dim&&
//                 voxel_coords.y>=0&&voxel_coords.y<dim&&
//             voxel_coords.z>=0&&voxel_coords.z<dim){
                
//                 int index=voxel_coords.x+voxel_coords.y*dim+voxel_coords.z*dim*dim;
                
//                 // assume a voxel at 0,0,0 is a solid material
//                 if(index==1){
//                     color=vec4(1.,0.,0.,1.);
//                     break;
//                 }
                
//             }
            
//         }
        
//         imageStore(img_output,pixel_coords,);
//     }