#version 150

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform sampler2D heightMap;

uniform vec3 eyePos;

in vec4 a_position;
in vec2 a_texcoord;

out vec2 v_texcoord;
out vec4 v_position;

//! [0]
void main()
{
    // Calculate vertex position in screen space

    vec4 position = a_position;
    position.y = 4.0f * texture2D(heightMap, a_texcoord).x;

    v_position = position;

    gl_Position = projection_matrix * view_matrix * model_matrix * position;//a_position;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
}
//! [0]
