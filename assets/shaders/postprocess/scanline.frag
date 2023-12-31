#version 330

uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

//create a scanline effect
void main()
{
    vec4 color = texture(tex, tex_coord);       // color of the pixel inside the texture(tex)
    float scanlineWidth = 0.003;                // width of the scanline
    float scanlineCount = 800.0;                // number of scanlines
    vec2 uv = tex_coord;                        // uv coordinates of the pixel inside the texture(tex)
    uv.y *= scanlineCount;                      // multiply the y coordinate of the pixel with the number of scanlines
    float res = mod(uv.y, 1.0);                 // get the remainder of the division of the y coordinate of the pixel with 1.0

    // if the remainder is less than the width of the scanline or greater than 1.0 - the width of the scanline
    if(res < scanlineWidth || res > (1.0 - scanlineWidth)){
        color = vec4(color.r* 0.2,color.g*0.2,color.b* 0.2, color.a);   // make the color of the pixel darker (draw the black line)
    } else {
        color = vec4(color.r* 0.6, color.g * 0.6 , color.b, color.a);   // acts as some sort of a tint
        // color = vec4(color.r, color.g, color.b, color.a);
    }
    frag_color = color;
}
