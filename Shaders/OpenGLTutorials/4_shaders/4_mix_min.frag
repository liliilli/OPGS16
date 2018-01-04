#version 330 core
out vec4 FragColor;

in vec3 ourColor; 
in vec2 TexCoord; 

uniform float min_density;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    // vec2 texture2_coord = vec2(-TexCoord.y, TexCoord.x);

    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), min_density);
}