#version 330 core

uniform vec3 bgColor;
uniform vec3 l1Color;
uniform vec3 l2Color;
uniform vec3 combColor;

uniform sampler2D layer1;
uniform sampler2D layer2;

in vec2 UV;
out vec4 color;

void main()
{
    vec4 colL1;
    vec4 colL2;

    colL1 = texture(layer1, UV);
    colL2 = texture(layer2, UV);

    if(colL1.r != 0 && colL2.r != 0) {
        color = vec4(combColor, 1.0);
    } else if(colL1.r != 0) {
        color = vec4(l1Color, 1.0);
    } else if(colL2.r != 0) {
        color = vec4(l2Color, 1.0);
    } else {
        color = vec4(bgColor, 1.0);
    }
}