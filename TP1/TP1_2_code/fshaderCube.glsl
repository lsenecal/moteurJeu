#version 150

uniform sampler2D texture_cube;

in vec2 v_texcoord;
in vec4 v_position;

//! [0]
void main()
{
    // Set fragment color from texture
    gl_FragColor = texture2D(texture_cube, v_texcoord);
}
//! [0]

