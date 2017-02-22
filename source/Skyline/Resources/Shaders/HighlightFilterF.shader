#version 130

// Interpolated values from the vertex shaders
in vec2 textureCoords;

// Ouput data
out vec4 pixelColor;

// Values that stay constant for the whole mesh.
uniform sampler2D textureSampler;
uniform float brightnessCutoff;

void main() {
  vec4 color = texture(textureSampler, textureCoords).rgba;
  float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
  
  /*if (brightness > 0.8) {
    pixelColor = color;
  } else {
    pixelColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
  */

  pixelColor = vec4(color.rgb * brightness, 1.0);
}