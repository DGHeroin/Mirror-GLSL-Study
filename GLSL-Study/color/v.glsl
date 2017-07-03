attribute vec4 gl_Color;

varying vec4 gl_FrontColor; // writable on the vertex shader
varying vec4 gl_BackColor; // writable on the vertex shader


uniform float time;
void main()
{
	gl_FrontColor = gl_Color;

	
	vec4 v = vec4(gl_Vertex);
	v.z = sin(5.0*v.x + time*0.01)*0.25;
	//v.z = 0.0;
	//v.z = sin(5.0*v.x )*0.25;

	gl_Position = gl_ModelViewProjectionMatrix * v;
}