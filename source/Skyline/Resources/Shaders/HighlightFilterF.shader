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
  
  brightness = max(0.0, brightness - brightnessCutoff);
  color.rgb *= sign(brightness);
  color.a = 1.0;
  pixelColor = color;

  //pixelColor = vec4(color.rgb * brightness, 1.0);
}