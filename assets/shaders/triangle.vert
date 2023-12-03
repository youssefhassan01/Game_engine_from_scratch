#version 330
//vert is the vertex shader file where w input the location of each vertex
//and we can perform transformations on it in here
// This vertex shader should be used to render a triangle whose normalized device coordinates are:
// (-0.5, -0.5, 0.0), ( 0.5, -0.5, 0.0), ( 0.0,  0.5, 0.0)
// And it also should send the vertex color as a varying to the fragment shader where the colors are (in order):
// (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0)
out Varyings {
    vec3 color;
} vs_out;

// Currently, the triangle is always in the same position, but we don't want that.
// So two uniforms should be added: translation (vec2) and scale (vec2).
// Each vertex "v" should be transformed to be "scale * v + translation".
// The default value for "translation" is (0.0, 0.0) and for "scale" is (1.0, 1.0).

//TODO: (Req 1) [Done] Finish this shader
uniform vec2 translation = vec2(0.0,0.0);
uniform vec2 scale = vec2(1.0,1.0);

void main(){
    const vec3 initialPositions[3] = vec3[3](
        vec3(-0.5, -0.5, 0.0),
        vec3(0.5, -0.5, 0.0),
        vec3(0.0, 0.5, 0.0)
    );


    const vec3 initialColors[3] = vec3[3](
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0)
    );

    vec3 positions = initialPositions[gl_VertexID];
    positions.xy *= scale;
    positions.xy += translation;
    //this is the built in postion for the triangle being changed 
    gl_Position = vec4(positions,1.0);
   // here we output the color to the fragment shader 

    vs_out.color = initialColors[gl_VertexID];
}