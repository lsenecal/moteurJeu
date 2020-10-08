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

    /*if (r > 1.08f)
        gl_FragColor = texture2D(texture_snow, v_texcoord);
    else if (r > 1.02f)
        gl_FragColor = texture2D(texture_rock, v_texcoord);
    else
        gl_FragColor = texture2D(texture_grass, v_texcoord);*/

    /*if (r > 2.19f)
        gl_FragColor = vec4(1, 1, 1, 1);
    else if (r > 2.00f)
        gl_FragColor = vec4(0.47, 0.157, 0.121, 1);
    else
        gl_FragColor = vec4(0.0f, 0, 1, 255);*/

    if (r < 2.1f)
       gl_FragColor = vec4(0, 0.2, 1, 1);
    else
       gl_FragColor = vec4(0.47, 0.157, 0.121, 1);
}
//! [0]

