#version 330

uniform sampler2D tex;
in vec2 tex_coord;
out vec4 frag_color;

void main()
{
    //color
    vec4 color = texture(tex, tex_coord);      
    //thickness of scanline
    float scanlineWidth = 0.003;               
    //number of scanlines
    float scanlineCount = 800.0;               
    
    vec2 uv = tex_coord;                       
    uv.y *= scanlineCount;                     
    float res = mod(uv.y, 1.0);                
    //do the effect only if the scanline is in the range of the scanline width
    if(res < scanlineWidth || res > (1.0 - scanlineWidth)){
        color = vec4(color.r* 0.2,color.g*0.2,color.b* 0.2, color.a);   
    } else {
        color = vec4(color.r* 0.6, color.g * 0.6 , color.b, color.a);   
    }
    frag_color = color;
}
