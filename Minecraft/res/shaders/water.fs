#version 460 core

in vec2 fTexCoord;
uniform sampler2D uTexture;

out vec4 Color;

void main() {
    // Color = texture(uTexture, fTexCoord);
    Color = vec4(0.0, 0.0, 1.0, 1.0);
}