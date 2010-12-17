#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex0;
uniform float tol;
uniform vec4 color;
void main( void )
{  

	vec4 one_third = vec4(0.333333);
	
	vec2 texcoord0 = gl_TexCoord[0].st;
	vec4 a = texture2DRect(tex0, texcoord0);
	vec4 b = vec4(0.,0.,0.,0.0); //texture2DRect(tex1, texcoord1);
	vec4 source = a;
	vec4 target = color;//vec4(color[0],color[1],color[2],1.0);
	//vec4 target = vec4(0,1,0,1.0);

	// zero our alphas
	source.a = 0.;
	target.a = 0.;	

	// measure distance from target
	vec4 vdelta = abs(source-target);
	
	// sum vector distance, scaling by a third
	float delta = dot(vdelta,one_third); 
	
	// determine scaling coefficient witin our fade range
	float fade = tol / 2.;
	float scale = smoothstep(abs(tol),abs(tol)+abs(fade),delta);

	// invert if necessary
	// -- N.A. --
	//float mixamount = mix(scale,1.-scale,invert);
	float mixamount = mix(scale,1.-scale,0.);

	// blend between sources based on mixamount	
	vec4 result = mix(b,a,vec4(mixamount));

	// set alpha value if the original value considered 
	// -- N.A. --
	//a.a = mix(mixamount, min(a.a, mixamount), alpha_history);

	// if not binary just set alpha value
	// -- N.A. --
	//result = mix(a,result,vec4(binary));
	
	// result either blend or mask based on mode
	// -- N.A. --
	//gl_FragColor = mix(result,vec4(mixamount),vec4(mode));
	
	gl_FragColor = result;
	
	
	
	/*
	vec2 st = gl_TexCoord[0].st;
	
	//horizontal blur 
	//from http://www.gamerendering.com/2008/10/11/gaussian-blur-filter-shader/
	
	vec4 color;
	
	color		+= 1.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * -4.0, 0.0));
	color		+= 2.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * -3.0, 0.0));
	color		+= 3.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * -2.0, 0.0));
	color		+= 4.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * -1.0, 0.0));	
		
	color		+= 5.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt, 0));
	
	color		+= 4.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * 1.0, 0.0));
	color		+= 3.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * 2.0, 0.0));
	color		+= 2.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * 3.0, 0.0));
	color		+= 1.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * 4.0, 0.0));
	
	color /= 25.0;
	gl_FragColor = color;
	*/
}