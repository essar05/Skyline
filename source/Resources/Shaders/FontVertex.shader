#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vertexPosition;
// Output data ; will be interpolated for each fragment.
out vec2 UV;

uniform mat4 MVP;

void main(){
    gl_Position = MVP * vec4(vertexPosition.x, vertexPosition.y, 0, 1 );

    // UV of the vertex. No special space for this one.
    UV = vertexPosition.zw;
}
