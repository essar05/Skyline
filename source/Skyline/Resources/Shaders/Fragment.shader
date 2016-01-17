#version 130

// Interpolated values from the vertex shaders
in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D textureSampler;

void main(){

	vec2 pos = fragmentPosition;
	vec4 cl = fragmentColor;

    // Output color = color of the texture at the specified UV
    //color = vec4(0.0, 1.0, 0.0, 1.0);
    color = fragmentColor.rgba * texture(textureSampler, fragmentUV).rgba;
}
