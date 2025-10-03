#version 120

uniform vec3 uCameraPos;
uniform vec4 uAmbientColor;

const int kMaxDirectionalLights = 4;
const int kMaxTextureLayers = 4;
uniform int uDirectionalLightCount;
uniform vec3 uLightDirections[kMaxDirectionalLights];
uniform vec4 uLightDiffuse[kMaxDirectionalLights];
uniform vec4 uLightSpecular[kMaxDirectionalLights];
uniform bool uLightEnabled[kMaxDirectionalLights];

uniform vec4 uMaterialDiffuse;
uniform float uMaterialAmbientMix;
uniform float uMaterialSpecularStrength;
uniform float uMaterialShininess;
uniform float uMaterialExposure;
uniform float uMaterialGamma;
uniform vec4 uMaterialRimColor;
uniform float uMaterialRimStrength;
uniform float uMaterialRimExponent;
uniform int uTextureLayerCount;
uniform int uTextureBlendModes[kMaxTextureLayers];
uniform float uTextureBlendFactors[kMaxTextureLayers];
uniform sampler2D uTextureLayers[kMaxTextureLayers];
uniform sampler2D uTexture;
uniform bool uUseTexture;
uniform bool uUseVertexColor;
uniform bool uEnableLighting;
uniform vec2 uTexScrollOffset[kMaxTextureLayers];
uniform float uTexRotationRad[kMaxTextureLayers];

varying vec3 vNormal;
varying vec3 vWorldPos;
varying vec2 vTexCoord;
varying vec4 vColor;

mat2 rotationMatrix(float angle) {
  float s = sin(angle);
  float c = cos(angle);
  return mat2(c, -s, s, c);
}

vec4 applyTextureLayers(vec4 baseColor) {
  vec4 result = baseColor;
  for (int i = 0; i < uTextureLayerCount; ++i) {
    vec2 uv = vTexCoord + uTexScrollOffset[i];
    uv = rotationMatrix(uTexRotationRad[i]) * (uv - vec2(0.5)) + vec2(0.5);
    uv = fract(uv);
    vec4 texColor = texture2D(uTextureLayers[i], uv);
    int mode = uTextureBlendModes[i];
    float factor = clamp(uTextureBlendFactors[i], 0.0, 1.0);

    if (mode == 0) {
      result = mix(result, texColor, factor);
    } else if (mode == 1) {
      vec4 multiplied = result * texColor;
      result = mix(result, multiplied, factor);
    } else if (mode == 2) {
      result = clamp(result + texColor * factor, 0.0, 1.0);
    } else if (mode == 3) {
      float alphaMix = clamp(texColor.a * factor, 0.0, 1.0);
      result = mix(result, texColor, alphaMix);
    }
  }
  return clamp(result, 0.0, 1.0);
}

void main() {
  vec3 normal = normalize(vNormal);
  vec4 baseColor = uUseVertexColor ? vColor : uMaterialDiffuse;
  if (uTextureLayerCount > 0) {
    baseColor = applyTextureLayers(baseColor);
  } else if (uUseTexture) {
    baseColor *= texture2D(uTexture, vTexCoord);
  }

  if (!uEnableLighting) {
    gl_FragColor = baseColor;
    return;
  }

  vec3 viewDir = normalize(uCameraPos - vWorldPos);
  float ambientFactor = clamp(uMaterialAmbientMix, 0.0, 1.0);
  vec4 color = baseColor * (uAmbientColor * ambientFactor);

  for (int i = 0; i < uDirectionalLightCount; ++i) {
    if (!uLightEnabled[i]) {
      continue;
    }
    vec3 lightDir = normalize(-uLightDirections[i]);
    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = diff * baseColor * uLightDiffuse[i];

    vec3 reflectDir = reflect(-lightDir, normal);
    float specAngle = max(dot(viewDir, reflectDir), 0.0);
    float spec = pow(specAngle, max(uMaterialShininess, 1.0));
    vec4 specular = spec * max(uMaterialSpecularStrength, 0.0) *
                    uLightSpecular[i];

    color += diffuse + specular;
  }

  float rimBase = clamp(1.0 - max(dot(normal, viewDir), 0.0), 0.0, 1.0);
  float rimFactor = pow(rimBase, max(uMaterialRimExponent, 0.1));
  vec3 rim = uMaterialRimColor.rgb * rimFactor * max(uMaterialRimStrength, 0.0);
  color.rgb += rim;

  vec3 toneMapped = color.rgb;
  float exposure = max(uMaterialExposure, 0.0);
  if (exposure > 0.0) {
    toneMapped = vec3(1.0) - exp(-toneMapped * exposure);
  }
  toneMapped = clamp(toneMapped, 0.0, 1.0);
  float gamma = max(uMaterialGamma, 0.01);
  toneMapped = pow(toneMapped, vec3(1.0 / gamma));
  gl_FragColor = vec4(toneMapped, color.a);
}
