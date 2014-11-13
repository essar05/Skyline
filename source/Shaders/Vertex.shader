#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertexPosition_modelspace;

void main(){
    gl_Position = vec4( vertexPosition_modelspace.x, vertexPosition_modelspace.y, 0, 1 );
}

