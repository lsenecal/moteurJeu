#version 150

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform sampler2D heightMap;

uniform vec2 center; // obj position on heightmap

in vec4 a_position;

void main()
{
    // Calculate vertex position in screen space

    vec4 position = a_position;
    position.y += 4.0f * texture2D(heightMap, center).x;

    gl_Position = projection_matrix * view_matrix * model_matrix * position;

}
