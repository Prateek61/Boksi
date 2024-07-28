#version 330 core

layout(location=0)in vec3 a_Position;
layout(location=1)in vec4 a_Color;

uniform mat4 u_ViewProjection;

out vec3 v_Position;

void main()
{
    v_Position=a_Position;
    
    gl_Position=vec4( 0.5 * a_Position,1.0);
}