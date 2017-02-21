#version 330

// Interpolated values from the vertex shaders
in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D textureSampler;

void main() {
  color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

  vec4 textureSample = texture(textureSampler, fragmentUV).rgba;
  color *= textureSample;
}
