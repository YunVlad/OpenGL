#version 330 core

out vec4 color;

in vec3 currPos;

uniform sampler2D rectMap;

vec2 SphericalMap(vec3 v)
{
    vec2 UV = vec2(atan(v.z, v.x), asin(v.y));
    UV *= vec2(0.1591, 0.3183);
    UV += 0.5;
    return UV;
}

void main()
{		
    vec2 UV = SphericalMap(normalize(currPos));
    vec3 rectColor = texture(rectMap, UV).rgb;
    
    color = vec4(rectColor, 1.0);
}