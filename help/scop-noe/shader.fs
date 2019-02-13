// Version du GLSL

#version 330 core


// Sortie Shader

out vec4 out_Color;
in vec3 aCouleurs;

// Fonction main

void main()
{
    // Couleur finale du pixel

    out_Color = vec4(aCouleurs, 1.0);
}
