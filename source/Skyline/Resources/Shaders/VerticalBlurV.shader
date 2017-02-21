#version 330

// Input vertex data, different for all executions of this shader.
in vec2 vertexPosition;

// Output data ; will be interpolated for each fragment.
out vec2 blurTextureCoords[11];

uniform float screenWidth;

void main(){
  gl_Position = vec4( vertexPosition.x, vertexPosition.y, 0.0, 1.0 );

  vec2 centerTexCoords = vertexPosition * 0.5 + 0.5;
  float pixelSize = 1.0f / screenWidth;

  for (int i = -5; i <= 5; i++) {
    blurTextureCoords[i + 5] = centerTexCoords + vec2(0.0, pixelSize * i);
  }
}
