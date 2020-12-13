#version 150

uniform sampler2D texture_grass;
uniform sampler2D texture_rock;
uniform sampler2D texture_snow;

in vec2 v_texcoord;
in vec4 v_position;

//! [0]
void main()
{
    // Set fragment color from texture
    if (v_position.y > 0.8f)
        gl_FragColor = texture2D(texture_snow, v_texcoord);
    else if (v_position.y > 0.2f)
        gl_FragColor = texture2D(texture_rock, v_texcoord);
    else
        gl_FragColor = texture2D(texture_grass, v_texcoord);
}
//! [0]

