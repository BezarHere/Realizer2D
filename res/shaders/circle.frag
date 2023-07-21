uniform float blur_radius = 1.5;

void main()
{
    vec2 offx = vec2(blur_radius, 0.0);
    vec2 offy = vec2(0.0, blur_radius);

    vec4 pixel = vec4(1.0, 0.7, 0.2, 1.0);

    gl_FragColor =  gl_Color * (pixel / 16.0);
}