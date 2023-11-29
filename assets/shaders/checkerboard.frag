#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the top-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
    float factor = floor(gl_FragCoord.x/size) + floor(gl_FragCoord.y/size);
    bool isColor1 = mod(factor,2.0) == 0.0;

    frag_color = vec4(isColor1? colors[0] : colors[1],1.0);
}