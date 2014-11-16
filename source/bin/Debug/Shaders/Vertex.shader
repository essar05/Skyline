#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){
    gl_Position = MVP * vec4( vertexPosition_modelspace.x, vertexPosition_modelspace.y, 0, 1 );

    // UV of the vertex. No special space for this one.
    UV = vertexUV;
}
