#version 120

#ifdef GL_ES
precision mediump float;
#endif

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;
attribute vec4 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform bool uUseVertexColor;

varying vec3 vNormal;
varying vec3 vWorldPos;
varying vec2 vTexCoord;
varying vec4 vColor;

void main() {
  vec4 worldPos = uModel * vec4(aPosition, 1.0);
  vWorldPos = worldPos.xyz;
  vNormal = mat3(uModel) * aNormal;
  vTexCoord = aTexCoord;
  vColor = uUseVertexColor ? aColor : vec4(1.0);
  gl_Position = uProjection * uView * worldPos;
}
