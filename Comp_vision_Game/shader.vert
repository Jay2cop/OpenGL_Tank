#version 120

attribute vec3 aVertexPosition;
attribute vec2 aVertexTexcoord;
attribute vec3 aVertexNormal;

uniform mat4x4 MVMatrix_uniform;
uniform mat4x4 ProjMatrix_uniform;

varying vec2 uv;
varying vec3 vNormal;
varying vec3 fragPos; 

void main()
{
    uv = aVertexTexcoord;
    vNormal = normalize(vec3(MVMatrix_uniform * vec4(aVertexNormal, 0.0))); 
    fragPos = vec3(MVMatrix_uniform * vec4(aVertexPosition, 1.0)); 
    gl_Position = ProjMatrix_uniform * MVMatrix_uniform * vec4(aVertexPosition, 1.0);
}