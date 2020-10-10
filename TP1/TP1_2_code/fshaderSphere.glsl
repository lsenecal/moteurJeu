#version 150

uniform sampler2D texture_grass;
uniform sampler2D texture_rock;
uniform sampler2D texture_snow;

in vec2 v_texcoord;
in vec4 v_position;

//! [0]
void main()
{
    float r = sqrt(v_position.x*v_position.x + v_position.y*v_position.y + v_position.z*v_position.z);

    if (r < 2.05f)
       gl_FragColor = vec4(0, 0.2, 1, 1);
    else
       gl_FragColor = vec4(0.47, 0.157, 0.121, 1);
}
//! [0]

