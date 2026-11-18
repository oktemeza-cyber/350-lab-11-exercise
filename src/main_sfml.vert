#version 120

varying vec2 texCoordsOut;

// Given a world position (for this lab, bottom-left is (0, 0), top-right of window is (1280, 720)),
// convert it to be in [-1, 1]^2, where y = -1 is the bottom and x = -1 is the left side.
vec2 cameraPos(vec2 worldPos) {
    return ((worldPos / vec2(1280.0, 720.0) * 2.0) - 1.0);
}

void main() {
    vec2 clip = cameraPos(gl_Vertex.xy);
    gl_Position = vec4(clip, 0.0, 1.0);
    texCoordsOut = gl_MultiTexCoord0.xy;
}
