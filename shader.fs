// Version du GLSL

#version 410 core


// Sortie Shader

out vec4 FragColor;
  
uniform vec4 ourColor; // we set this variable in the OpenGL code.

void main()
{
    FragColor = ourColor;
} 

// out vec4 out_Color;
// smooth in vec3 aCouleurs;

// // Fonction main

// void main()
// {
//     // Couleur finale du pixel

//     out_Color = vec4(aCouleurs, 1.0);
// }
