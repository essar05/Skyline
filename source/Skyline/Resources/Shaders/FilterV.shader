#version 330

// Input vertex data, different for all executions of this shader.
in vec2 vertexPosition;

// Output data ; will be interpolated for each fragment.
out vec2 textureCoords;

uniform float screenWidth;

void main(){
  gl_Position = vec4( vertexPosition.x, vertexPosition.y, 0.0, 1.0 );

  textureCoords = vertexPosition * 0.5 + 0.5;
}