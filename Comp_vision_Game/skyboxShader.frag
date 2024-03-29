#version 120
uniform samplerCube TextureMap_uniform;
varying vec3 vPosition;

void main()
{
    gl_FragColor = textureCube(TextureMap_uniform, vPosition);
}