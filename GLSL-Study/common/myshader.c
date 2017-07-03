#include "myshader.h"

static GLchar *ebuffer; /* buffer for error messages */
static GLsizei elength; /* length of error message */


char* readShaderSource(const char* shaderFile)
{
	FILE* fp = fopen(shaderFile, "rb");

	char* buf;
	long size;

	if(fp==NULL) return NULL;
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	buf = (char*) malloc((size+1) * sizeof(char)); 
	fread(buf, 1, size, fp);
	buf[size] = '\0';
	fclose(fp);
	return buf;
}

int initShader(const GLchar* vShaderFile, const GLchar* fShaderFile)
{
	GLint status = glGetError()==GL_NO_ERROR;
	GLchar *vSource, *fSource;
	GLuint vShader, fShader;

	/* read shader files */
	vSource = readShaderSource(vShaderFile);
	if(vSource==NULL)
	{
		printf( "Failed to read vertex shaderi\n");
		return -1;
	}


	fSource = readShaderSource(fShaderFile);
	if(fSource==NULL)
	{
		printf("Failed to read fragment shader");
		return -1;
	}

	/* create g_program and shader objects */

	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	g_program = glCreateProgram();

	/* attach shaders to the g_program object */

	glAttachShader(g_program, vShader);
	glAttachShader(g_program, fShader);

	/* read shaders */

	glShaderSource(vShader, 1, (const GLchar**) &vSource, NULL);
	glShaderSource(fShader, 1, (const GLchar**) &fSource, NULL);

	/* compile vertex shader */

	glCompileShader(vShader);

	/* error check */

	glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
	if(status==GL_FALSE)
	{
		printf("Failed to compile the vertex shader.\n");
		glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &elength);
		ebuffer = (GLchar *) malloc(elength*sizeof(char));
		glGetShaderInfoLog(vShader, elength, NULL, ebuffer);
		printf("%s\n", ebuffer);
		return -1;
	}

	/* compile fragment shader */

	glCompileShader(fShader);

	glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);
	if(status==GL_FALSE)
	{
		printf("Failed to compile the fragment shader.\n");
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &elength);
		ebuffer = (GLchar *) malloc(elength*sizeof(char));
		glGetShaderInfoLog(fShader, elength, NULL, ebuffer);
		printf("%s\n", ebuffer);
		return -1;
	}


	/* link */

	glLinkProgram(g_program);
	glGetShaderiv(g_program, GL_LINK_STATUS, &status);
	if(status==GL_FALSE)
	{
		printf("Failed to link g_program object.\n");
		glGetProgramiv(g_program, GL_INFO_LOG_LENGTH, &elength);
		ebuffer = (GLchar *) malloc(elength*sizeof(char));
		glGetProgramInfoLog(g_program, elength, &elength, ebuffer);
		printf("%s\n", ebuffer);
		return -1;
	}


	/* use g_program object */

	glUseProgram(g_program);

	return 0;
}