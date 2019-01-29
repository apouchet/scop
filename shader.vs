// Version du GLSL

#version 330 core


// Entrée Shader

layout (location  = 0) in vec2 in_Vertex;
layout (location  = 1) in vec3 couleurs;

smooth out vec3 aCouleurs;


// Fonction main

void main()
{
    // Position finale du vertex
    gl_Position = vec4(in_Vertex, 0.0, 1.0);
	aCouleurs = couleurs;
}
