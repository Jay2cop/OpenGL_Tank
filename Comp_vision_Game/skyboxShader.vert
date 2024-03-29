#version 120

attribute vec3 aVertexPosition;
uniform mat4x4 MVMatrix_uniform;
uniform mat4x4 ProjMatrix_uniform;
varying vec3 vPosition;

void main()
{
    vPosition = aVertexPosition;
    mat4x4 mvMatrix = MVMatrix_uniform;
    mvMatrix[3] = vec4(0.0, 0.0, 0.0, 1.0);
    gl_Position = ProjMatrix_uniform * mvMatrix * vec4(aVertexPosition, 1.0);
}