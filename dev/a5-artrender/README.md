CSci-4611 Assignment 5: Art Render
- Warren Weber
- weber767
- Spring Semester 2020

## Description of C level implementation
### Goal: Complete the phong.frag fragment shader to correctly calculate per-pixel shading using the standard (Blinn-)Phong lighting model.
- All the changes to implement the Phong lighting feature in the app were completed by extending the phong.frag file. The output of phong.frag is a Color that has been formulated using the illumination equation that was discussed in the lecture and used in the Gouraud shader.
```
vec4 ambient = ka * Ia;
vec4 diffuse = kd * Id * max(dot(n, l), 0);
vec4 specular = ks * Is * pow(max(dot(h, n), 0), s);

color = ambient + diffuse + specular;

```
- The key difference is that the illumination is calculated for each pixel using the normal that has been calculated by the rasterizer.  

## Description of B level implementation
### Goal: Complete the artsy.frag fragment shader to correctly calculate the per-pixel toon shading, using the “diffuseRamp.png” and “specularRamp.png” textures to control the lighting.
- To implement the artsy shading the diffuse and specular shading in artsy.frag needed to use a texture in place of the calculated intensity value.
- This changed the calculation of the diffuse intensity so that its function output a suitable value in the x-coord of the texture coordinate.
```
float tex_diff_intensity = 0.5 * dot(n, l) + 0.5;

// This is where the textures values were used to bin the possible intensity values
vec4 diffuse = kd * Id * texture(diffuse_ramp, vec2(tex_diff_intensity, 0));
vec4 specular = ks * Is * texture(specular_ramp, vec2(tex_spec_intensity, 0));

```
## Description of A level implementation
### Goal: Complete the outline.vert vertex shader to draw a black outline for the silhouette edges of the mesh, when rendering in “artsy” mode.
- the outline is implemented in outline.vert, by checking the vertex to see if it is a silhouette
  - this is carried out by using the normals of the to the left and right of the vertex.
```
if (dot(l_n, e) * dot(r_n, e) < 0.0) {
  // draw an edge
  ver = vertex + thickness * normal;
}
```
- changes to vertex were output through the gl_Position.
