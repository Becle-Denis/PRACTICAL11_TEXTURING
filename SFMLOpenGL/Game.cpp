#include <Game.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

std::string readFromFile(std::string filePath)
{
	std::string r;
	std::string line;
	ifstream file(filePath);
	if (file.is_open())
	{
		while (getline(file, line))
		{
			r += line;
			r += "\n";
		}
	}
	else
	{
		DEBUG_MSG("Unable to read the file " + filePath);
	}
	return r;
}

Game::Game() : window(VideoMode(800, 600), "OpenGL Cube Texturing")
{
}

Game::~Game() {}

void Game::run()
{

	initialize();

	Event event;

	while (isRunning) {

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}
		}
		update();
		render();
	}

}

typedef struct
{
	float coordinate[3];
	float color[4];
	float texel[2];
} Vertex;

const int TRIANGLE_NUMBER = 36;
const int VERTEX_NUMBER = 24;

Vertex vertex[VERTEX_NUMBER];
GLubyte triangles[TRIANGLE_NUMBER];

/* Variable to hold the VBO identifier and shader data */
GLuint	index,		//Index to draw
		vsid,		//Vertex Shader ID
		fsid,		//Fragment Shader ID
		progID,		//Program ID
		vao = 0,	//Vertex Array ID
		vbo[1],		// Vertex Buffer ID
		positionID, //Position ID
		colorID,	// Color ID
		to,			// Texture ID 1 to 32
		textureID,	//Texture ID
		texelID;	// Texel ID

//const string filename = "texture.tga";
//const string filename = "cube.tga";

const string filename = "cube2.tga";

int width; //width of texture
int height; //height of texture
int comp_count; //Component of texture
const int number = 4; //4 = RGBA

unsigned char* img_data;

void Game::initialize()
{
	isRunning = true;
	GLint isCompiled = 0;
	GLint isLinked = 0;

	glewInit();

	DEBUG_MSG(glGetString(GL_VENDOR));
	DEBUG_MSG(glGetString(GL_RENDERER));
	DEBUG_MSG(glGetString(GL_VERSION));

	/* Vertices counter-clockwise winding */
	vertex[0].coordinate[0] = -0.25f;
	vertex[0].coordinate[1] = -0.25f;
	vertex[0].coordinate[2] = 0.0f;

	vertex[1].coordinate[0] = -0.25f;
	vertex[1].coordinate[1] = 0.25f;
	vertex[1].coordinate[2] = 0.0f;

	vertex[2].coordinate[0] = 0.25f;
	vertex[2].coordinate[1] = 0.25f;
	vertex[2].coordinate[2] = 0.0f;

	vertex[3].coordinate[0] = 0.25f;
	vertex[3].coordinate[1] = -0.25f;
	vertex[3].coordinate[2] = 0.0f;

	vertex[4].coordinate[0] = -0.25f;
	vertex[4].coordinate[1] = -0.25f;
	vertex[4].coordinate[2] = 0.5f;

	vertex[5].coordinate[0] = -0.25f;
	vertex[5].coordinate[1] = 0.25f;
	vertex[5].coordinate[2] = 0.5f;

	vertex[6].coordinate[0] = 0.25f;
	vertex[6].coordinate[1] = 0.25f;
	vertex[6].coordinate[2] = 0.5f;

	vertex[7].coordinate[0] = 0.25f;
	vertex[7].coordinate[1] = -0.25f;
	vertex[7].coordinate[2] = 0.5f;

	vertex[8].coordinate[0] = -0.25f;
	vertex[8].coordinate[1] = -0.25f;
	vertex[8].coordinate[2] = 0.0f;

	vertex[9].coordinate[0] = 0.25f;
	vertex[9].coordinate[1] = -0.25f;
	vertex[9].coordinate[2] = 0.0f;

	vertex[10].coordinate[0] = -0.25f;
	vertex[10].coordinate[1] = -0.25f;
	vertex[10].coordinate[2] = 0.5f;

	vertex[11].coordinate[0] = 0.25f;
	vertex[11].coordinate[1] = -0.25f;
	vertex[11].coordinate[2] = 0.5f;

	vertex[12].coordinate[0] = 0.25f;
	vertex[12].coordinate[1] = 0.25f;
	vertex[12].coordinate[2] = 0.0f;

	vertex[13].coordinate[0] = 0.25f;
	vertex[13].coordinate[1] = -0.25f;
	vertex[13].coordinate[2] = 0.0f;

	vertex[14].coordinate[0] = 0.25f;
	vertex[14].coordinate[1] = 0.25f;
	vertex[14].coordinate[2] = 0.5f;

	vertex[15].coordinate[0] = 0.25f;
	vertex[15].coordinate[1] = -0.25f;
	vertex[15].coordinate[2] = 0.5f;

	vertex[16].coordinate[0] = -0.25f;
	vertex[16].coordinate[1] = 0.25f;
	vertex[16].coordinate[2] = 0.0f;

	vertex[17].coordinate[0] = 0.25f;
	vertex[17].coordinate[1] = 0.25f;
	vertex[17].coordinate[2] = 0.0f;

	vertex[18].coordinate[0] = -0.25f;
	vertex[18].coordinate[1] = 0.25f;
	vertex[18].coordinate[2] = 0.5f;

	vertex[19].coordinate[0] = 0.25f;
	vertex[19].coordinate[1] = 0.25f;
	vertex[19].coordinate[2] = 0.5f;

	vertex[20].coordinate[0] = -0.25f;
	vertex[20].coordinate[1] = -0.25f;
	vertex[20].coordinate[2] = 0.0f;

	vertex[21].coordinate[0] = -0.25f;
	vertex[21].coordinate[1] = 0.25f;
	vertex[21].coordinate[2] = 0.0f;

	vertex[22].coordinate[0] = -0.25f;
	vertex[22].coordinate[1] = -0.25f;
	vertex[22].coordinate[2] = 0.5f;

	vertex[23].coordinate[0] = -0.25f;
	vertex[23].coordinate[1] = 0.25f;
	vertex[23].coordinate[2] = 0.5f;

	vertex[0].texel[0] = 0.75f;
	vertex[0].texel[1] = 0.5f;

	vertex[1].texel[0] = 0.75f;
	vertex[1].texel[1] = 0.25f;

	vertex[2].texel[0] = 1.0f;
	vertex[2].texel[1] = 0.25f;

	vertex[3].texel[0] = 1.0f;
	vertex[3].texel[1] = 0.5f;

	vertex[4].texel[0] = 0.25f;
	vertex[4].texel[1] = 0.25;

	vertex[5].texel[0] = 0.5f;
	vertex[5].texel[1] = 0.25f;
	
	vertex[6].texel[0] = 0.5f;
	vertex[6].texel[1] = 0.5f;

	vertex[7].texel[0] = 0.25f;
	vertex[7].texel[1] = 0.5f;

	vertex[8].texel[0] = 0.5f;
	vertex[8].texel[1] = 0.5f;

	vertex[9].texel[0] = 0.75f;
	vertex[9].texel[1] = 0.5f;

	vertex[10].texel[0] = 0.5f;
	vertex[10].texel[1] = 0.25f;

	vertex[11].texel[0] = 0.75f;
	vertex[11].texel[1] = 0.25f;

	vertex[12].texel[0] = 0.25f;
	vertex[12].texel[1] = 0.25f;

	vertex[13].texel[0] = 0.5f;
	vertex[13].texel[1] = 0.25f;

	vertex[14].texel[0] = 0.25f;
	vertex[14].texel[1] = 0.0f;

	vertex[15].texel[0] = 0.5f;
	vertex[15].texel[1] = 0.0f;

	vertex[16].texel[0] = 0.0f;
	vertex[16].texel[1] = 0.5f;

	vertex[17].texel[0] = 0.25f;
	vertex[17].texel[1] = 0.5f;

	vertex[18].texel[0] = 0.0f;
	vertex[18].texel[1] = 0.25f;

	vertex[19].texel[0] = 0.25f;
	vertex[19].texel[1] = 0.25f;


	vertex[20].texel[0] = 0.25f;
	vertex[20].texel[1] = 0.75f;

	vertex[21].texel[0] = 0.5f;
	vertex[21].texel[1] = 0.75f;

	vertex[22].texel[0] = 0.25f;
	vertex[22].texel[1] = 0.5f;

	vertex[23].texel[0] = 0.5f;
	vertex[23].texel[1] = 0.5f;






	




	/*Index of Poly / Triangle to Draw */
	//FACE 6
	triangles[0] = 0;   triangles[1] = 1;   triangles[2] = 2;
	triangles[3] = 0; triangles[4] = 2;  triangles[5] = 3;
	
	//Face 1
	triangles[6] = 4;   triangles[7] = 5;   triangles[8] = 6;
	triangles[9] = 4; triangles[10] = 6;  triangles[11] = 7;
	
	// Face 3 
	triangles[12] = 12;   triangles[13] = 13;   triangles[14] = 14;
	triangles[15] = 13; triangles[16] = 14;  triangles[17] = 15;
	
	// Face 2
	triangles[18] = 16;   triangles[19] = 17;   triangles[20] = 18;
	triangles[21] = 17; triangles[22] = 18;  triangles[23] = 19;
	
	// Face 4 ! 
	triangles[24] = 20;   triangles[25] = 21;   triangles[26] = 22;
	triangles[27] = 21; triangles[28] = 22;  triangles[29] = 23;
	
	//FACE 5 
	triangles[30] = 8;   triangles[31] = 9;   triangles[32] = 11;
	triangles[33] = 8; triangles[34] = 10;  triangles[35] = 11;


	/* Create a new VBO using VBO id */
	glGenBuffers(1, vbo);

	/* Bind the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	/* Upload vertex data to GPU */
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 9, vertex, GL_STATIC_DRAW); 
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * TRIANGLE_NUMBER, triangles, GL_STATIC_DRAW); // 3 -> 12!!!!!!!!!!!
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/* Vertex Shader which would normally be loaded from an external file */
	std::string vsS = readFromFile("VertexShader.txt");
	const char* vs_src = vsS.c_str();

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER); //Create Shader and set ID
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL); // Set the shaders source
	glCompileShader(vsid); //Check that the shader compiles

	//Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}

	/* Fragment Shader which would normally be loaded from an external file */
	std::string fsS = readFromFile("FragmentShader.txt");
	const char* fs_src = fsS.c_str();

	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar**)&fs_src, NULL);
	glCompileShader(fsid);
	//Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();	//Create program in GPU
	glAttachShader(progID, vsid); //Attach Vertex Shader to Program
	glAttachShader(progID, fsid); //Attach Fragment Shader to Program
	glLinkProgram(progID);

	//Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}

	// Use Progam on GPU
	// https://www.opengl.org/sdk/docs/man/html/glUseProgram.xhtml
	glUseProgram(progID);

	img_data = stbi_load(filename.c_str(), &width, &height, &comp_count, 4);

	if (img_data == NULL)
	{
		DEBUG_MSG("ERROR: Texture not loaded");
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &to);
	glBindTexture(GL_TEXTURE_2D, to);

	//Wrap around
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Filtering
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Bind to OpenGL
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml
	glTexImage2D(GL_TEXTURE_2D, //2D Texture Image
		0, //Mipmapping Level 
		GL_RGBA, //GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA 
		width, //Width
		height, //Height
		0, //Border
		GL_RGBA, //Bitmap
		GL_UNSIGNED_BYTE, img_data);

	// Find variables in the shader
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	colorID = glGetAttribLocation(progID, "sv_color");
	texelID = glGetAttribLocation(progID, "sv_texel");
	textureID = glGetUniformLocation(progID, "f_texture");
}

void Game::update()
{
	
	elapsed = clock.getElapsedTime();

	//Allowed movement
	if (movementCLock.getElapsedTime() > sf::milliseconds(30))
	{
		movementCLock.restart();

		//center calculation 
		float sumX = 0;
		float sumY = 0;
		float sumZ = 0;
		for (int i = 0; i < VERTEX_NUMBER; i++)
		{
			sumX += vertex[i].coordinate[0];
			sumY += vertex[i].coordinate[1];
			sumZ += vertex[i].coordinate[2];
		}
		db::Vector3 center(sumX / VERTEX_NUMBER, sumY / VERTEX_NUMBER, sumZ / VERTEX_NUMBER);

		//------------ROTATING--------------------------

		// rotation X 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			for (int i = 0; i < VERTEX_NUMBER; i++)
			{
				//adapting to vector
				db::Vector3 v(vertex[i].coordinate[0], vertex[i].coordinate[1], vertex[i].coordinate[2]);

				//rotating
				v = v - center;
				v = v * db::Matrix3::rotationX(1);
				v = v + center;

				//reassigning value 
				vertex[i].coordinate[0] = v.x;
				vertex[i].coordinate[1] = v.y;
				vertex[i].coordinate[2] = v.z;
			}
		}


		// rotation Y
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
		{
			for (int i = 0; i < VERTEX_NUMBER; i++)
			{
				//adapting to vector
				db::Vector3 v(vertex[i].coordinate[0], vertex[i].coordinate[1], vertex[i].coordinate[2]);

				//rotating
				v = v - center;
				v = v * db::Matrix3::rotationY(1);
				v = v + center;

				//reassigning value 
				vertex[i].coordinate[0] = v.x;
				vertex[i].coordinate[1] = v.y;
				vertex[i].coordinate[2] = v.z;
			}
		}

		// rotation Z 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			for (int i = 0; i < VERTEX_NUMBER; i++)
			{
				//adapting to vector
				db::Vector3 v(vertex[i].coordinate[0], vertex[i].coordinate[1], vertex[i].coordinate[2]);

				//rotating
				v = v - center;
				v = v * db::Matrix3::rotationZ(1);
				v = v + center;

				//reassigning value 
				vertex[i].coordinate[0] = v.x;
				vertex[i].coordinate[1] = v.y;
				vertex[i].coordinate[2] = v.z;
			}
		}

		//------------TRANSLATING------------------------

		// Translating right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			for (int i = 0; i < VERTEX_NUMBER; i++)
			{
				//adapting to vector
				db::Vector3 v(vertex[i].coordinate[0], vertex[i].coordinate[1], 1);

				//translating
				v = v * db::Matrix3::translate(1, 0);

				//reassigning value
				vertex[i].coordinate[0] = v.x;
				vertex[i].coordinate[1] = v.y;
			}
		}

		// Translating left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			for (int i = 0; i < VERTEX_NUMBER; i++)
			{
				//adapting to vector
				db::Vector3 v(vertex[i].coordinate[0], vertex[i].coordinate[1], 1);

				//translating
				v = v * db::Matrix3::translate(-1, 0);

				//reassigning value
				vertex[i].coordinate[0] = v.x;
				vertex[i].coordinate[1] = v.y;
			}
		}

		// Translating up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			for (int i = 0; i < VERTEX_NUMBER; i++)
			{
				//adapting to vector
				db::Vector3 v(vertex[i].coordinate[0], vertex[i].coordinate[1], 1);

				//translating
				v = v * db::Matrix3::translate(0, 1);

				//reassigning value
				vertex[i].coordinate[0] = v.x;
				vertex[i].coordinate[1] = v.y;
			}
		}

		// Translating down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			for (int i = 0; i < VERTEX_NUMBER; i++)
			{
				//adapting to vector
				db::Vector3 v(vertex[i].coordinate[0], vertex[i].coordinate[1], 1);

				//translating
				v = v * db::Matrix3::translate(0, -1);

				//reassigning value
				vertex[i].coordinate[0] = v.x;
				vertex[i].coordinate[1] = v.y;
			}
		}

		// Scalling up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
		{
			for (int i = 0; i < VERTEX_NUMBER; i++)
			{
				//adapting to vector
				db::Vector3 v(vertex[i].coordinate[0], vertex[i].coordinate[1], vertex[i].coordinate[2]);

				//scalling
				v = v - center;
				v = v * db::Matrix3::scale3D(101);
				v = v + center;

				//reassigning value
				vertex[i].coordinate[0] = v.x;
				vertex[i].coordinate[1] = v.y;
				vertex[i].coordinate[2] = v.z;
			}
		}

		// Scalling Down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			for (int i = 0; i < VERTEX_NUMBER; i++)
			{
				//adapting to vector
				db::Vector3 v(vertex[i].coordinate[0], vertex[i].coordinate[1], vertex[i].coordinate[2]);

				//scalling
				v = v - center;
				v = v * db::Matrix3::scale3D(99);
				v = v + center;

				//reassigning value
				vertex[i].coordinate[0] = v.x;
				vertex[i].coordinate[1] = v.y;
				vertex[i].coordinate[2] = v.z;
			}
		}

	}
	
#if (DEBUG >= 2)
	DEBUG_MSG("Update up...");
#endif

}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Drawing...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	/*	As the data positions will be updated by the this program on the
		CPU bind the updated data to the GPU for drawing	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * TRIANGLE_NUMBER, vertex, GL_STATIC_DRAW); // 3 -> 12!!!!!!!!!!!

	/*	Draw Triangle from VBO	(set where to start from as VBO can contain
		model components that 'are' and 'are not' to be drawn )	*/

	//Set Active Texture .... 32
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureID, 0);

	// Set pointers for each parameter
	// https://www.opengl.org/sdk/docs/man4/html/glVertexAttribPointer.xhtml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); 
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(texelID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));

	//Enable Arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	glEnableVertexAttribArray(texelID);

	glDrawElements(GL_TRIANGLES, TRIANGLE_NUMBER, GL_UNSIGNED_BYTE, (char*)NULL + 0); // 3 -> 12!!!!!!!!!!!

	window.display();

}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDeleteProgram(progID);
	glDeleteBuffers(1, vbo);
	stbi_image_free(img_data); //Free image
}

