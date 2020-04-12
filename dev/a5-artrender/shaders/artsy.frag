#version 330

// CSci-4611 Assignment 5:  Art Render

// TODO: You need to calculate per-fragment shading here using a toon shading model

in vec3 position_in_eye_space;
in vec3 normal_in_eye_space;

out vec4 color;

uniform vec3 light_in_eye_space;
uniform vec4 Ia, Id, Is;

uniform vec4 ka, kd, ks;
uniform float s;

uniform sampler2D diffuse_ramp;
uniform sampler2D specular_ramp;


void main() {
  // vertex position in "eye space"
  vec3 v = position_in_eye_space;

  // unit vector from the vertex to the light
  vec3 l = normalize(light_in_eye_space - v);

  // unit vector from the vertex to the eye point, which is at 0,0,0 in "eye space"
  vec3 e = normalize(vec3(0,0,0) - v);

  // normal in "eye space"
  vec3 n = normalize(normal_in_eye_space);

  // halfway vector
  vec3 h = normalize(l + e);

  // calculate diffuse intensity from texture
  float tex_diff_intensity = 0.5 * dot(n, l) + 0.5;

  // calculate specular intensity from texture
  float tex_spec_intensity = pow(max(dot(h, n), 0), s);

  // calculating lighting output the color for this vertex
  vec4 ambient = ka * Ia;
  vec4 diffuse = kd * Id * texture(diffuse_ramp, vec2(tex_diff_intensity, 0));
  vec4 specular = ks * Is * texture(specular_ramp, vec2(tex_spec_intensity, 0));
  color = ambient + diffuse + specular;
}
