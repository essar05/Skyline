#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D textureSampler;

void main(){

    // Output color = color of the texture at the specified UV
    //color = vec4(0.0, 0.0, 1.0, 1.0);
    color = texture(textureSampler, UV).rgba;
}
