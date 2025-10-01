#version 120

uniform vec3 uCameraPos;
uniform vec4 uAmbientColor;

const int kMaxDirectionalLights = 4;
uniform int uDirectionalLightCount;
uniform vec3 uLightDirections[kMaxDirectionalLights];
uniform vec4 uLightDiffuse[kMaxDirectionalLights];
uniform vec4 uLightSpecular[kMaxDirectionalLights];
uniform bool uLightEnabled[kMaxDirectionalLights];

uniform vec4 uMaterialDiffuse;
uniform sampler2D uTexture;
uniform bool uUseTexture;
uniform bool uUseVertexColor;
uniform bool uEnableLighting;

varying vec3 vNormal;
varying vec3 vWorldPos;
varying vec2 vTexCoord;
varying vec4 vColor;

void main() {
  vec3 normal = normalize(vNormal);
  vec4 baseColor = uUseVertexColor ? vColor : uMaterialDiffuse;
  if (uUseTexture) {
    baseColor *= texture2D(uTexture, vTexCoord);
  }

  if (!uEnableLighting) {
    gl_FragColor = baseColor;
    return;
  }

  vec3 viewDir = normalize(uCameraPos - vWorldPos);
  vec4 color = baseColor * uAmbientColor;

  for (int i = 0; i < uDirectionalLightCount; ++i) {
    if (!uLightEnabled[i]) {
      continue;
    }
    vec3 lightDir = normalize(-uLightDirections[i]);
    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = diff * baseColor * uLightDiffuse[i];

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec4 specular = spec * uLightSpecular[i];

    color += diffuse + specular;
  }

  gl_FragColor = color;
}
