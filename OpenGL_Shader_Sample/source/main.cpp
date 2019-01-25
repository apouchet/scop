#include <GL/glew.h>

#include <GL/freeglut.h>
/*
#include <GL/gl.h>
#include <GL/glu.h>
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Déclaration des points de notre cube
GLfloat cubeVertices[] =
{
    // Face de devant
    1,1,1,
    -1,1,1,
    -1,-1,1,
    1,-1,1,

    1, 1, -1,
    -1, 1, -1,

    -1, -1, -1,
    1, -1, -1,
};

GLfloat cubeColours[] =
{
    1, 0, 0,
    0, 1, 0,
    0, 1, 0,
    1, 0, 0,

    0, 0, 1,
    1, 1, 0,

    1, 1, 0,
    0, 0, 1,
};

GLubyte cubeIndices[] =
{
    0, 1, 2, 3,
    0, 4, 7, 3,
    4, 5, 6, 7,
    1, 2, 6, 5,
    2, 3, 7, 6,
    0, 1, 5, 4
};

GLuint vertexID = 0;
GLuint fragmentID = 0;
GLuint programID = 0;

GLint attributeId = 0;

unsigned int getFileSize(FILE* const pFile)
{
    long length = 0;

    fseek(pFile,0,SEEK_END);

    length = ftell(pFile);

    // Ne pas oublier de mettre le fichier à son début, sinon on ne peut pas le lire
    fseek(pFile,0,SEEK_SET);

    return length;
}

/**
    Le fonction retourne un pointeur, qui a été alloué dans la fonction même.
    ! Ceci est extrèmement dangereux car cela peut entrainer des fuites de mémoire
    du à l'oubli de libéré le pointeur.
*/
char* readFile(const char* fileName)
{
    FILE* pFile = NULL;
    char* fileContent = NULL;
    unsigned int fileSize = 0;

    pFile = fopen(fileName,"r");
    if ( pFile == NULL )
    {
        fprintf(stderr,"Erreur d'ouverture du fichier: '%s'\n",fileName);
        return NULL;
    }

    fileSize = getFileSize(pFile);

    fileContent = (char*)malloc(fileSize+1);
    if ( fileContent == NULL )
    {
        fprintf(stderr,"Erreur d'allocation de la mémoire pour y placer le contenu du fichier\n");
        return NULL;
    }

    fread(fileContent,fileSize,1,pFile);
    // Termine le tableau qui contient le shader
    fileContent[fileSize] = '\0';

    fclose(pFile);

    return fileContent;
}

void deleteShader()
{
    // On arrête d'utiliser le programme shader
    glUseProgram(0);

    // Deliage des shaders au programme
    glDetachShader(programID, fragmentID);
    glDetachShader(programID, vertexID);

    // Destruction du programme
    glDeleteProgram(programID);

    // Destruction des IDs des shaders
    glDeleteShader(fragmentID);
    glDeleteShader(vertexID);
}

// Pour plus de simplicité, j'ajoute une fonction qui vérifie la compilation des shaders
char checkShaderCompilation(GLuint shaderID)
{
    GLint compilationStatus = 0;

    // Verification de la compialtion pour le vertex shader
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &compilationStatus);
    if ( compilationStatus != GL_TRUE )
    {
        // Nous savons que la compilation a échoué, donc nous devons savoir pourquoi
        // Nous devons récupéré la taille du log
        GLint logLength = 0;
        GLchar* log = NULL;

        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

        // Maintenant que nous avons la taille, nous pouvons alloué la mémoire suffisante pour ce log
        log = (GLchar*)malloc(logLength);
        if ( log == NULL )
        {
            fprintf(stderr,"Erreur d'allocation de mémoire pour le log de la compilation du shader\n");
            return 0;
        }

        glGetShaderInfoLog(shaderID, logLength, &logLength, log);

        // On peut afficher le message
        fprintf(stderr,"Erreur de compilation:\n%s",log);

        // Et on n'oublie pas de libéré la mémoire
        free(log);

        return 0;
    }

    return 1; // Pas d'erreur
}

void loadShader()
{
    GLchar* vertexSource = NULL;
    GLchar* fragmentSource = NULL;
    GLint programState = 0;
    GLint vertexSize = 0;
    GLint fragmentSize = 0;
    GLenum errorState = GL_NO_ERROR;

    // Création des IDs
    vertexID = glCreateShader(GL_VERTEX_SHADER);
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

    // Lecture des fichiers
    // Certaines personnes aiment avoir le fichier du vertex shader avec l'extion .vert
    // et le fichier du fragement shader avec l'extension .frag
    vertexSource = (GLchar*)readFile("data/simple.vert");
    fragmentSource = (GLchar*)readFile("data/simple.frag");

    if ( !vertexSource || !fragmentSource )
    {
        // Ici, il faudrait faire en sorte que le programme s'arrête
        deleteShader();    // Nettoyage
        return;
    }

    // Chargement des sources dans OpenGL
    vertexSize = strlen(vertexSource);
    fragmentSize = strlen(fragmentSource);
    glShaderSource(vertexID, 1, (const GLchar**)(&vertexSource), &vertexSize);
    glShaderSource(fragmentID, 1, (const GLchar**)(&fragmentSource), &fragmentSize);

    // Compilation du vertex shader
    glCompileShader(vertexID);
    glCompileShader(fragmentID);

    // Vérification des erreurs
    if ( !checkShaderCompilation(vertexID) || !checkShaderCompilation(fragmentID))
    {
        deleteShader();
        return;
    }


    // Creation de l'ID pour le programme
    programID = glCreateProgram();

    // On attache les shader ensemble
    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);

    // On peut enfin passer aux liage.
    glLinkProgram(programID);

    // Et encore une fois on vérifie si tout se passe bien
    glGetProgramiv(programID , GL_LINK_STATUS  , &programState);
    if ( programState != GL_TRUE)
    {
        // On récupère la taille du log
        GLint logSize = 0;
        GLchar* log = NULL;

        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logSize);

        // On peut allouer la mémoire, une fois que l'on a la taille du log
        log = (GLchar*)malloc(logSize);
        if ( log == NULL )
        {
            fprintf(stderr,"Erreur d'allocation de mémoire pour le log de la compilation du programme\n");
            deleteShader();
            return;
        }

        // Et on récupère le log
        glGetProgramInfoLog(programID, logSize, &logSize, log);

        // On affiche
        fprintf(stderr,"Erreur lors du liage du shader:\n%s",log);

        free(log);
        deleteShader();
        return;
    }

    attributeId = glGetAttribLocation(programID, "couleur");
    errorState = glGetError();
    if ( attributeId == -1 || errorState != GL_NO_ERROR )
    {
        fprintf(stderr,"Erreur (%d) lors de la récupération de l'id de la variable attribute 'couleur'\n",errorState);
    }

    // Voilà, nous sommes prêt
    glUseProgram(programID);

    glEnableVertexAttribArray(attributeId);
    errorState = glGetError();

    glVertexAttribPointer(attributeId,
                          3,
                          GL_FLOAT,
                          GL_TRUE,
                          0,
                          cubeColours);

    if ( errorState != GL_NO_ERROR )
    {
        fprintf(stderr,"Erreur (%d) lors du passage du tableau de valeur à la variable attribute 'couleur'\n",errorState);
    }
}

void initScene()
{
	glClearColor(0.0,0.5,0.0,1.0);

	// Depth
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

    // Perspective mode
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)640/(GLfloat)480,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);

    loadShader();

	// Init the cube
        // Tell to OpenGL that we are using :
        //  - Vertices
        //  - Colours
	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

    // Assign our data to OpenGL
    glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
    //glColorPointer(3, GL_FLOAT, 0, cubeColours);
}

void renderScene()
{
    // Rotation stuff
    static unsigned int rotationAngle;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(-2,5,-4,
              0,0,0,
              0,1,0);

    glRotated(rotationAngle, 1, 1, 0);

    // draw the cube
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);

	if ( glGetError() != GL_NO_ERROR )
	{
	    fprintf(stderr,"Error occur during drawing");
		return;
	}

	glutSwapBuffers();

	rotationAngle++;
}

void quitScene()
{
    glDisableVertexAttribArray(attributeId);
    //glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    deleteShader();
}

void keyboardFeedback(unsigned char key, int x, int y)
{
	if ( key == 27 ) // Escape
	{
	    quitScene();
		glutLeaveMainLoop();
	}
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(640,480);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE |GLUT_DEPTH);

	glutCreateWindow("OpenGL Shaders");

	// Demarrage de GLEW
	if ( glewInit() != GLEW_OK )
	{
	    fprintf(stderr,"Echec du demarrage de GLEW\n");
	    return -1;
	}

	// On vérifie que l'on peut géré les shaders
	// Check if the machine is capable of rendering our scene
	if ( !glewIsSupported("GL_ARB_shading_language_100") )
	{
        fprintf(stderr,"GL_ARB_shading_language_100 introuvables\n");
		return -2;
	}
	if ( !glewIsSupported("GL_ARB_shader_objects") )
	{
		fprintf(stderr,"GL_ARB_shader_objects introuvables\n");
		return -3;
	}
	if ( !glewIsSupported("GL_ARB_vertex_shader") )
	{
		fprintf(stderr,"GL_ARB_vertex_shader introuvables\n");
		return -4;
	}
	if ( !glewIsSupported("GL_ARB_fragment_shader") )
	{
		fprintf(stderr,"GL_ARB_fragment_shader introuvables\n");
		return -5;
	}

	initScene();
	// glutDisplayFunc(renderScene);
	glutKeyboardFunc(keyboardFeedback);
	glutIdleFunc(renderScene);

	glutMainLoop();

    quitScene();

	return 0;
}
