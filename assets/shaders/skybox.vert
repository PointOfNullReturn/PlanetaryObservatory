#version 120

attribute vec3 aPosition;
attribute vec3 aTexCoord;

uniform mat4 uView;
uniform mat4 uProjection;

varying vec3 vTexCoord;

void main() {
  vTexCoord = aTexCoord;
  vec4 position = uProjection * uView * vec4(aPosition, 1.0);
  gl_Position = position;
}
