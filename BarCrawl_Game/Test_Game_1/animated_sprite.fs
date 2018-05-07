#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform int num_images;
uniform int image_index;

void main()
{    
    color = vec4(spriteColor, 1.0) * texture(image, vec2(TexCoords.x*image_index/num_images, TexCoords.y));
}  