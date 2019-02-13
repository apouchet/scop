attribute vec3 couleur = vec3(0.0f,1.0f,0.0f);

void main (void)
{
	gl_Position = ftransform();	
	gl_FrontColor = vec4(couleur,1.0);
}
