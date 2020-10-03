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

    if (r > 1.08f)
        gl_FragColor = texture2D(texture_snow, v_texcoord);
    else if (r > 1.02f)
        gl_FragColor = texture2D(texture_rock, v_texcoord);
    else
        gl_FragColor = texture2D(texture_grass, v_texcoord);
}
//! [0]

