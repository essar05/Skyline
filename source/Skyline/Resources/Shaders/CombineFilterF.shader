#version 130

// Interpolated values from the vertex shaders
in vec2 textureCoords;

// Ouput data
out vec4 pixelColor;

// Values that stay constant for the whole mesh.
uniform sampler2D baseTexture;
uniform sampler2D highlightsTexture;uniform int blendMode;
uniform float highlightsIntensity;

void main() {
  vec3 baseColor = texture(baseTexture, textureCoords).rgb;
  vec3 highlightsColor = texture(highlightsTexture, textureCoords).rgb * highlightsIntensity;

  switch (blendMode) {
    //add
    case 1:
      pixelColor = vec4(baseColor + highlightsColor, 1.0f);
      break;

      //screen
    case 2:
      pixelColor = 1.0 - (1.0 - vec4(baseColor, 1.0f)) * (1.0 - vec4(highlightsColor, 1.0f));
      break;

      //lighten
    case 3:
      pixelColor = vec4(max(baseColor, highlightsColor), 1.0f);
      break;


    default:
      pixelColor = vec4(baseColor, 1.0);
  }
}