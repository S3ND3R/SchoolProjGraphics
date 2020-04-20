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
- The key difference is that the illumination is calculated fr each pixel using the normal that has been calculated by the rasterizer.  

## Description of B level implementation
### Goal: Complete the artsy.frag fragment shader to correctly calculate the per-pixel toon shading, using the “diffuseRamp.png” and “specularRamp.png” textures to control the lighting.
## Description of A level implementation
### Goal: Complete the outline.vert vertex shader to draw a black outline for the silhouette edges of the mesh, when rendering in “artsy” mode.
