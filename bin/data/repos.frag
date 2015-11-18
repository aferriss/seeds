//setup for 2 texture
varying vec2 texcoord0;
varying vec2 texcoord1;
//varying vec2 texdim0;
//varying vec2 texdim1;
uniform vec2 res;
//uniform vec4 scale;
//uniform vec4 bias;
uniform vec2 boundmode;
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform vec2 params;

vec3 brightnessContrast(vec3 value, float brightness, float contrast)
{
    return (value - 0.5) * contrast + 0.5 + brightness;
}

void main()
{
    
    /*
    vec4 look = texture2DRect(tex1,texcoord1);//sample repos texture
    vec2 offs = vec2(look.y-look.x,look.w-look.z)*amt;
    vec2 coord = offs+texcoord0;//relative coordinates
    //coord = mod(coord,texcoord0);
    vec4 repos = texture2DRect(tex0, coord);
    */
    vec4 t = texture2DRect(tex0, vec2(texcoord0.x - 2.0, texcoord0.y + 2.0) );
    vec4 u = texture2DRect(tex0, texcoord0);
    vec4 v = texture2DRect(tex0, vec2(texcoord0.x + 2.0, texcoord0.y - 2.0) );
    
    t.rgb = brightnessContrast(t.rgb, params.x, params.y);
    u.rgb = brightnessContrast(u.rgb, params.x, params.y);
    v.rgb = brightnessContrast(v.rgb, params.x, params.y);
    
    // output texture
    //uuugl_FragColor = vec4(1.0-repos.r,1.0-repos.g,1.0-repos.b,repos.a);//*scale+bias;
    gl_FragColor = vec4(t.r, u.g, v.b, 1.0);
    //gl_FragColor = vec4(1.0,0.0,0.0,1.0);
   
}