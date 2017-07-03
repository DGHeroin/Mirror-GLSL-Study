varying float intensity;

void main()
{
	//vec3 lightDir = vec3(6.0, 6.0, 2.0);
	vec3 lightDir = normalize(vec3(gl_LightSource[0].position));
	intensity = dot(lightDir,gl_Normal);
	gl_Position = ftransform();
}