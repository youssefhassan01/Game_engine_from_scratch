#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the top-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) [Done] Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
    //idea of code 
    
    //first line gets where does the point lie by summing x and y normalised by size rounded down
    
    //second line checks if the factor is even or not if even it is the other color
    //why even? we want to start with color 0 then color 1 so if factor lies in even square we draw it in color 1 
    //otherwise draw color0
    
    //third line is simple if condition that checks our docndtion to draw color
    //i couldn't for the life of me use step() to make checkerboard to improve fragment shader as 
    //if condtions tend to be problematic, so decided to use if for the time being
    // NOTE : ask TA if anything other than if can be used 

    float factor = floor(gl_FragCoord.x/size) + floor(gl_FragCoord.y/size);
    bool isColor1 = mod(factor,2.0) == 0.0;

    frag_color = vec4(isColor1? colors[0] : colors[1],1.0);
}