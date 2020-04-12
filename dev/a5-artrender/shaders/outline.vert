#version 330

// CSci-4611 Assignment 5:  Art Render

// TODO: You need to modify this vertex shader to move the edge vertex along
// the normal away from the mesh surface IF you determine that the vertex
// belongs to a silhouette edge.


uniform mat4 model_view_matrix;
uniform mat4 normal_matrix;
uniform mat4 proj_matrix;
uniform float thickness;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 left_normal;
layout(location = 3) in vec3 right_normal;

void main() {
  // vertex used in gl_Position
  vec3 ver;

  // vector in eye space
  vec3 v = (model_view_matrix * vec4(vertex, 1)).xyz;

  // unit vector from the vertex to the eye point, which is at 0,0,0 in "eye space"
  vec3 e = normalize(vec3(0,0,0) - v);

  // left_normal transformed into "eye space"
  vec3 l_n = normalize((normal_matrix * vec4(left_normal, 0)).xyz);

  // right_normal transformed into "eye space"
  vec3 r_n = normalize((normal_matrix * vec4(right_normal, 0)).xyz);

  // calculating vertex thickness for edge condition
  if (dot(l_n, e) * dot(r_n, e) < 0.0) {
    // draw an edge
    ver = vertex + thickness * normal;
  } else {
    ver = vertex;
  }
  gl_Position = proj_matrix * model_view_matrix * vec4(ver,1);
}
