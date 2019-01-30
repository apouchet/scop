// Version du GLSL

#version 410 core


// Entrée Shader

layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
}

// layout (location  = 0) in vec2 in_Vertex;
// layout (location  = 1) in vec3 couleurs;

// smooth out vec3 aCouleurs;


// // Fonction main

// void main()
// {
//     // Position finale du vertex
//     gl_Position = vec4(in_Vertex, 0.0, 1.0);
// 	aCouleurs = couleurs;
// }
