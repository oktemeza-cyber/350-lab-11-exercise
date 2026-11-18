#version 120

uniform sampler1D transforms;
uniform int numTransforms;

attribute vec2 position;
attribute vec2 texCoords;
attribute float spriteNumber;

varying vec2 texCoordsOut;

// Given a world position (for this lab, bottom-left is (0, 0), top-right of window is (1280, 720)),
// convert it to be in [-1, 1]^2, where y = -1 is the bottom and x = -1 is the left side.
vec2 cameraPos(vec2 worldPos) {
    return ((worldPos / vec2(1280.0, 720.0) * 2.0) - 1.0);
}

void main() {
   float coord = (spriteNumber + 0.5) / float(numTransforms);
   vec4 transform = texture1D(transforms, coord);

   vec2 scale = transform.xy;
   vec2 translation = transform.zw;

   vec2 world = position * scale + translation;

   gl_Position = vec4(cameraPos(world), 0.0, 1.0);
   texCoordsOut = texCoords;
}
