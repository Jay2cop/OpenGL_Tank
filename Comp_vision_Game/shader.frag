#version 120

uniform sampler2D TextureMap_uniform;

uniform vec3 lightDir;
uniform vec3 viewPos;
uniform vec3 lightColor;

varying vec2 uv;
varying vec3 vNormal;
varying vec3 fragPos;

void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  
    // Diffuse 
    vec3 norm = normalize(vNormal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
  
    // Specular
    float specularStrength = 1.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    vec3 result = (ambient + diffuse + specular) * texture2D(TextureMap_uniform, uv).rgb;
    gl_FragColor = vec4(result, 1.0);
}


