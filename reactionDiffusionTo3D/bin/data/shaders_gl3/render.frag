#version 150

// This fill the billboard made on the Geometry Shader with a texture

uniform sampler2DRect tex0;
uniform float discardRed;
uniform vec3 lightPos;

in vec2 vTexCoord;
in vec4 vPosition;
in vec3 vNormal;

out vec4 vFragColor;

void main() {
    vec4 color = texture(tex0, vTexCoord);
    if(color.r > discardRed) {
        discard;
    }else{
        //insert here light calculation, using light position
        // and normals obtained by with the normalMap + normals of the sphere.
        vFragColor = texture(tex0, vTexCoord);
    }
}