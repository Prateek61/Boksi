#version 330 core

// Define the camera struct
struct Camera
{
    vec3 Position;
    vec3 Direction;
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