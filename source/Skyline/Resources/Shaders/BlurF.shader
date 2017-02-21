#version 330

out vec4 color;

in vec2 blurTextureCoords[11];

uniform sampler2D textureSampler;

void main(void) {
  color = vec4(0.0);
  color += texture(textureSampler, blurTextureCoords[0]) * 0.0093;
  color += texture(textureSampler, blurTextureCoords[1]) * 0.028002;
  color += texture(textureSampler, blurTextureCoords[2]) * 0.065984;
  color += texture(textureSampler, blurTextureCoords[3]) * 0.121703;
  color += texture(textureSampler, blurTextureCoords[4]) * 0.175713;
  color += texture(textureSampler, blurTextureCoords[5]) * 0.198596;
  color += texture(textureSampler, blurTextureCoords[6]) * 0.175713;
  color += texture(textureSampler, blurTextureCoords[7]) * 0.121703;
  color += texture(textureSampler, blurTextureCoords[8]) * 0.065984;
  color += texture(textureSampler, blurTextureCoords[9]) * 0.028002;
  color += texture(textureSampler, blurTextureCoords[10]) * 0.0093;
}