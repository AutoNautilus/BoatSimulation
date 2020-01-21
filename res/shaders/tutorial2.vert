#version 150
// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1
attribute vec3 in_Position;
attribute vec4 in_Color;
 
uniform vec2 translate;
uniform float scale;
uniform mat4 transform;

// We output the ex_Color variable to the next shader in the chain
out vec4 ex_Color;

void main(void) {
    // Since we are using flat lines, our input only had two points: x and y.
	vec4 transformedPos = transform * vec4(in_Position, 1.0);
    // Set the Z coordinate to 0 and W coordinate to 1
    gl_Position = vec4(scale * (translate.x + transformedPos.x), scale * (translate.y + transformedPos.y), transformedPos.z, transformedPos.w);
 
    // Pass the color on to the fragment shader
    ex_Color = in_Color;
}