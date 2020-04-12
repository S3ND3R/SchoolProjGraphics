#version 330

// The fragment shader needs access to the texture that has been bound in c++
uniform sampler2D surface_tex;

// This color comes in from the output of the vertex shader stage.  The current
// fragment will lie somewhere within a triangle.  So, the vec4 that is passed
// in here is actually an interpolated version of the colors output by the 3
// vertex shader programs run for the 3 vertices of the triangle.
in vec4 color;

// Tex coords come in the same way as the color. They will already be
// interpolated to the correct values for this fragment inside the triangle.
in vec2 texcoords;


// All fragment shaders are required to output a vec4 color.
out vec4 final_color;


void main() {

    // outputting the color found from using the texture coords
    vec4 tex_color = texture(surface_tex, texcoords);
    final_color = tex_color * color;

    // For a Gouraud shader, there is nothing more to compute at this stage.  We
    // just output the input color.
    // final_color = color;


}
