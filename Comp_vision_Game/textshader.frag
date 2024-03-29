#version 120

uniform vec4 textColor;
varying vec2 uv;
uniform sampler2D TextureMap_uniform;

void main()
{
    gl_FragColor = texture2D(TextureMap_uniform, uv) * textColor;
}