/* uniform sampler2D tex1;
uniform sampler2D tex2;
varying vec2  TexCoord;
uniform float pan;
void main(void)
{
   // get texture info
   vec4 v1 = texture2D(tex1, TexCoord);
   vec4 v2 = texture2D(tex2, TexCoord);
   gl_FragColor =  vec4(mix(v1, v2, pan).rgb, 1.0);
} */
#extension GL_ARB_texture_rectangle : enable

uniform sampler2D tex1;
uniform sampler2D tex2;
varying vec2  TexCoord;
uniform float pan;
uniform float tol;

void main( void )
{  

	vec4 one_third = vec4(0.333333);
	
	vec4 a = texture2D(tex1, TexCoord);
	vec4 c = texture2D(tex2, TexCoord);
	
	vec4 b = vec4(0.,0.,0.,0.0);
   
	vec4 source = a;
	vec4 target = c ;
	source.a = 0.;
	target.a = 0.;	
	vec4 vdelta = abs(source-target);
	float delta = dot(vdelta,one_third); 	
	float fade = tol / 2.;
	float scale = smoothstep(abs(tol),abs(tol)+abs(fade),delta);
	float mixamount = mix(scale,1.-scale,0.);
	vec4 result = mix(b,a,vec4(mixamount));

	gl_FragColor = result;
}