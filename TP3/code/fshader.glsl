#version 150

uniform sampler2D texture_surface;

in vec2 v_texcoord;
in vec3 v_position;

//! [0]
void main()
{
    gl_FragColor = texture2D(texture_surface, v_texcoord);
    //gl_FragColor = vec4(v_position.xyz, 1);
}
//! [0]

