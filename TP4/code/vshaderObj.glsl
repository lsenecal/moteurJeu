#version 150

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

in vec4 a_position;

void main()
{
    // Calculate vertex position in screen space
    gl_Position = projection_matrix * view_matrix * model_matrix * a_position;

}
