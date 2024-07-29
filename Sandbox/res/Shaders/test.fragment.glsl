#version 330 core

layout(location = 0) out vec4 color;
in vec3 v_Position;
in vec4 v_Color;

uniform vec4 u_Color;

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
uniform Camera u_Camera;


void main()
{
	color = vec4(u_Camera.Position, 1.0);
}