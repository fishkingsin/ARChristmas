#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex0; //
uniform sampler2DRect tex1;
uniform sampler2DRect tex2;

uniform float tol;
uniform vec4 keycolor;

void main( void )
{  

	vec4 one_third = vec4(0.333333);
	
	vec2 texcoord0 = gl_TexCoord[0].st;
	//vec2 texcoord1 = vec2(gl_TexCoord[0].s, 480.0 - gl_TexCoord[0].t); //flip on vertical side for loaded image
	vec4 a = texture2DRect(tex0, texcoord0);
	vec4 b = texture2DRect(tex1, texcoord0);
	vec4 source = a;
	vec4 target = keycolor;

	// zero our alphas
	source.a = 0.;
	target.a = 0.;	



	// measure distance from target
	vec4 vdelta_chroma = abs(source-target);
	
	// sum vector distance, scaling by a third
	float delta_chroma = dot(vdelta_chroma,one_third); 


	target = texture2DRect(tex2, texcoord0);
	target.a = 0;


	// measure distance from target
	vec4 vdelta_bg = abs(source-target);
	
	// sum vector distance, scaling by a third
	float delta_bg = dot(vdelta_bg,one_third); 


	// Pick the closer value of keying value
	float delta = min(delta_chroma, delta_bg);
	
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
	//gl_FragColor = b;
	
	
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