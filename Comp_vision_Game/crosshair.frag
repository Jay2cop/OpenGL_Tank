#version 120

varying vec2 TexCoord;

uniform sampler2D textureSampler;

void main()
{
    gl_FragColor = texture2D(textureSampler, TexCoord);
}