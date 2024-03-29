// Includes
#include <GL/glew.h>	//OpenGL Related Functions
#include <GL/glut.h>	//OpenGL Related Functions
#include <Shader.h>		//Functions for compiling shaders
#include <Vector.h>		//Functions for Vectors
#include <iostream>		//Writing to the Console using std::cout 
#include <math.h>       // Includes Math functions
#include <Matrix.h>
#include <Mesh.h>
#include <SphericalCameraManipulator.h>
#include <Texture.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//Function Prototypes
bool initGL(int argc, char** argv);		        	//Function to init OpenGL
void initShader();					                //Function to init Shader
void initGeometry();					            //Function to init Geometry 
void drawGeometry();					            //Function to draw Geometry
void display(void);					                //OpenGL Display Function
void mouse(int button, int state, int x, int y);	//Mouse Function
void initTexture(std::string filename, GLuint & textureID);
void keyOperations();
void keyUp(unsigned char key, int x, int y);
void passiveMotion(int x, int y);
void keyPressed(unsigned char key, int x, int y);
void updateBalls(float deltaTime);
void createBall(float x, float y, float z, float vx, float vy, float vz);
void fireBall();
int findAvailableBall();
static bool LoadBMP(std::string filename, int & width, int & height, char * &data);
GLuint loadPNGTexture(const std::string &filename);
void drawtank(const Matrix4x4& ModelViewMatrix);
void drawBalls(const Matrix4x4& ModelViewMatrix);
void drawMaze(const Matrix4x4& ModelViewMatrix, const Vector3f& tankPosition);
void updateCamera(Matrix4x4& ModelViewMatrix);
void initCoins();
void checkBallCoinCollisions();
bool istankOnTile(const Vector3f& tankPosition, int row, int col, float TileScale, float mazeScale, const Vector3f& mazeOffset);
Vector3f fallingtankPosition(const Vector3f& tankPosition, float TileScale, float mazeScale, const Vector3f& mazeOffset);
void drawMenu();
void drawText(std::string text, float r, float g, float b, float x, float y);
void specialKeys(int key, int x, int y);
void specialKeysUp(int key, int x, int y);
void updateTimer(int value);
void collectCoin();
void drawHUD();

//Global Variables
GLuint shaderProgramID;			                    // Shader Program ID
GLuint textShaderID;
GLuint textColourUniformLocation;
GLuint vertexNormalAttribute;
GLuint vertexPositionAttribute;		                // Vertex Position Attribute Location
GLuint vertexTexcoordAttribute; 					// Vertex Texcoord Attribute Location
GLuint TextureMapUniformLocation; 					// Texture Map Location
GLuint ColourUniformLocation; 
GLuint TileTexture; 								// Tile Texture
GLuint coinTexture;									// coin Texture
GLuint tankTexture;								// tank Texture
GLuint ballTexture;									// ball texture         
GLint cubeTexture;                                  // cube Texture

//tank movement variables
Vector3f tankPosition(0.0f, 0.0f, 0.0f);
float wheelRotationAngle = 0.0f;
float turningDirection = 0.0f;
float turretRotationAngle = 0.0f;
float tankRotationAngle = 0.0f;
bool keyStates[256] = {false};
int prevMouseX = -1;
int currentMouseX = 0;
bool spacebarPressed = true;
Vector3f tankSize(5.0f, 2.0f, 3.0f);

Vector3f mazeOffset(0.0f, -1.5f, 0.0f);
float mazeScale = 4.0f;


//Coin variables
float coinRotationAngle = 0.0f;

//Ball variables
const float gravity = -9.81f;
float deltaTime;
const int MAX_BALLS = 10;
bool aimingMode = false;
struct BallData {
    bool active;
    float x, y, z;    
    float vx, vy, vz; 
};

BallData balls[MAX_BALLS];

//Collision variables
const int MAX_COINS = 10;
struct Coin {
    float x, y, z;
    bool active;
	int row, col;
};

Coin coins[MAX_COINS];
bool isBallCoinCollision(const BallData& ball, const Coin& coin);

//Display interractions
enum GameState {
    MENU,
    PLAYING,
	PAUSED,
    EXIT
};

GameState currentState = MENU;
int selectedMenuItem = 0;
int windowWidth = 500;
int windowHeight = 500;
float menuItemColor[3] = {1.0f, 1.0f, 1.0f};
float selectedItemColor[3] = {1.0f, 1.0f, 0.0f};


//Timer and score
int timer = 45;
int score = 0;
bool timerActive = false;

//Maze
float TileScale = 1.0f;
int mazeLayout[10][10] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

//Coins spawn
int coinPositions[10][10] = {
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 1, 1, 0, 0, 0, 0},
};

//Lighting
GLint lightPosUniform;
GLint viewPosUniform;
GLint lightDirUniform;
GLint lightColorUniform;
Vector3f updatedCameraPosition;

//Viewing
SphericalCameraManipulator cameraManip;
Matrix4x4 ModelViewMatrix;		                    // ModelView Matrix
GLuint MVMatrixUniformLocation;		                // ModelView Matrix Uniform
Matrix4x4 ProjectionMatrix;		                    // Projection Matrix
GLuint ProjectionUniformLocation;	                // Projection Matrix Uniform Location

//Mesh
Mesh TileMesh;				                            // Tile Mesh
Mesh coinMesh;											// coin Mesh
Mesh bodyMesh;										// tank body Mesh
Mesh front_wheelMesh;									// tank front wheel Mesh
Mesh back_wheelMesh;									// tank back wheel Mesh
Mesh turretMesh;										// tank turret Mesh
Mesh ballMesh;											// ball mesh
Mesh cubeMesh;											// skybox mesh


//Main Program Entry
int main(int argc, char** argv)
{	
	//init OpenGL
	if(!initGL(argc, argv))
		return -1;
		
	initCoins();
    //Init OpenGL Shader
    initShader();


    //Loading objects and textures
	cubeMesh.loadOBJ("../models/cube.obj");  
	ballMesh.loadOBJ("../models/ball.obj");
	TileMesh.loadOBJ("../models/cube.obj");
	coinMesh.loadOBJ("../models/coin.obj");
	bodyMesh.loadOBJ("../models/chassis.obj");
	front_wheelMesh.loadOBJ("../models/front_wheel.obj");
	back_wheelMesh.loadOBJ("../models/back_wheel.obj");
	turretMesh.loadOBJ("../models/turret.obj");
	initTexture("../models/ball.bmp", ballTexture);
	initTexture("../models/Tile.bmp", TileTexture);
	initTexture("../models/coin.bmp", coinTexture);
	initTexture("../models/tank.bmp", tankTexture);
	cubeTexture = loadPNGTexture("../models/skybox.png");

	//Camera view
	Vector3f bodyCentroid = bodyMesh.getMeshCentroid();
	cameraManip.setPanTiltRadius(3.15f, -2.0f, -1.0f);
	cameraManip.setFocus(bodyCentroid);

	currentState = MENU;
	//Start main loop
	glutMainLoop();

	//Clean-Up
	glDeleteProgram(shaderProgramID);

    return 0;
}  

//! Function to Initlise OpenGL
bool initGL(int argc, char** argv)
{
	// Init GLUT
    glutInit(&argc, argv);
    
	// Set Display Mode: enable double buffering, RGBA color mode, depth testing, and multi-sampling for anti-aliasing.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);

	// Enable multi-sample anti-aliasing.
    glEnable(GL_MULTISAMPLE);

	// Set Window Size
    glutInitWindowSize(500, 500);
    
    // Window Position
    glutInitWindowPosition(200, 200);

	// Create Window
    glutCreateWindow("Computer vision & graphics project");
    
    // Init GLEW
	if (glewInit() != GLEW_OK) 
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}
	
	// Set callbacks for various GLUT events
    glutDisplayFunc(display);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(specialKeys);
	glutSpecialUpFunc(specialKeysUp);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyPressed);
	glutPassiveMotionFunc(passiveMotion);

	// Enable 2D texturing
	glEnable(GL_TEXTURE_2D);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

    // Enable smooth shading for smoother color transitions
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	return true;
}

//Init Shader
void initShader()
{
	//Create shader
    shaderProgramID = Shader::LoadFromFile("shader.vert","shader.frag");
    
    // Get a handle for our vertex position buffer
	vertexPositionAttribute = glGetAttribLocation(shaderProgramID, "aVertexPosition");


    //!
	MVMatrixUniformLocation = glGetUniformLocation(shaderProgramID,     "MVMatrix_uniform"); 
	ProjectionUniformLocation = glGetUniformLocation(shaderProgramID,   "ProjMatrix_uniform");
    vertexNormalAttribute = glGetAttribLocation(shaderProgramID,    "aVertexNormal");
	vertexTexcoordAttribute = glGetAttribLocation(shaderProgramID,"aVertexTexcoord");
	TextureMapUniformLocation = glGetUniformLocation(shaderProgramID,"TextureMap_uniform");
    ColourUniformLocation = glGetUniformLocation(shaderProgramID, "Colour_uniform");
    lightDirUniform = glGetUniformLocation(shaderProgramID, "lightDir");
    viewPosUniform = glGetUniformLocation(shaderProgramID, "viewPos");
    lightColorUniform = glGetUniformLocation(shaderProgramID, "lightColor");
}


void initTexture(std::string filename, GLuint & textureID)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

    //Get texture Data
    int width, height;
    char* data;
    Texture::LoadBMP(filename, width, height, data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Generate mipmaps for the texture
    glGenerateMipmap(GL_TEXTURE_2D);

    //Cleanup data as copied to GPU
    delete[] data;
}

//Display Loop
void display(void)
{

	static int prevTime = 0;
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (currentTime - prevTime) / 1000.0f;
	prevTime = currentTime;

	// Clear the screen
	glClearColor(1.0,0.5,0.8,1.0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    keyOperations();
	updateCamera(ModelViewMatrix);

	if (currentState == MENU) {
		glUseProgram(0);
        drawMenu();
	} else if (currentState == PLAYING) {
        drawHUD();
		glUseProgram(shaderProgramID);
    
        // Setting the directional light, acting like sunlight
        Vector3f lightDirection(2.0f, 0.5f, 1.0f); 
        lightDirection = Vector3f::normalise(lightDirection);
        GLfloat lightDirectionArray[3] = {lightDirection.x, lightDirection.y, lightDirection.z};  
        glUniform3fv(lightDirUniform, 1, lightDirectionArray);

        GLfloat updatedCameraPositionArray[3] = {updatedCameraPosition.x, updatedCameraPosition.y, updatedCameraPosition.z};
        glUniform3fv(viewPosUniform, 1, updatedCameraPositionArray);

        glUniform3f(lightColorUniform, 1.0f, 1.0f, 1.0f);

		//Projection Matrix - Perspective Projection
		ProjectionMatrix.perspective(90, 1.0, 0.0001, 100.0);
		//Set Projection Matrix
		glUniformMatrix4fv(	
			ProjectionUniformLocation,  //Uniform location
			1,							//Number of Uniforms
			false,						//Transpose Matrix
			ProjectionMatrix.getPtr());	//Pointer to ModelViewMatrixValues
        
		drawMaze(ModelViewMatrix, tankPosition);
		drawtank(ModelViewMatrix);
		tankPosition = fallingtankPosition(tankPosition, TileScale, mazeScale, mazeOffset);
		drawBalls(ModelViewMatrix);
		checkBallCoinCollisions();
    } else if (currentState == PAUSED) {
        glUseProgram(0);
        drawMenu();
	}

	glUseProgram(0);
	
	glutPostRedisplay();
    glutSwapBuffers();
}

//Create a ball
void createBall(float x, float y, float z, float vx, float vy, float vz) {
    for (int i = 0; i < MAX_BALLS; ++i) {
        if (!balls[i].active) {
            balls[i].x = x;
            balls[i].y = y;
            balls[i].z = z;
            balls[i].vx = vx;
            balls[i].vy = vy;
            balls[i].vz = vz;
            balls[i].active = true;
            break;
        }
    }
}

//Search for an available ball
int findAvailableBall() {
    for (int i = 0; i < MAX_BALLS; ++i) {
        if (!balls[i].active) {
            return i;
        }
    }
    return -1;
}

// Shooting a ball
void fireBall(float deltaTime) {
    int i = findAvailableBall();
    if (i >= 0) {
        float tankRotationRadians = -tankRotationAngle * 3.14159f / 180.0f;
        float turretRotationRadians = (turretRotationAngle + 90.0f) * 3.14159f / 180.0f;
        
        float xOffset = 4.5;
        float yOffset = 3.0; 

        // Calculate the local position of the ball relative to the tank
        float localX = xOffset * cos(turretRotationRadians);
        float localY = yOffset;
        float localZ = xOffset * sin(turretRotationRadians);

        // Transform local coordinates to world coordinates
        float x = tankPosition.x + localX * cos(tankRotationRadians) - localZ * sin(tankRotationRadians);
        float y = tankPosition.y + localY;
        float z = tankPosition.z + localX * sin(tankRotationRadians) + localZ * cos(tankRotationRadians);

        float magnitude = 30.0f;
        float vx = magnitude * cos(tankRotationRadians + turretRotationRadians);
        float vy = 0.0;
        float vz = magnitude * sin(tankRotationRadians + turretRotationRadians);

        createBall(x, y, z, vx, vy, vz);
    }
}

//Update a ball position
void updateBalls(float deltaTime) {
    for (int i = 0; i < MAX_BALLS; ++i) {
        if (balls[i].active) {
            balls[i].x += balls[i].vx * deltaTime;
            balls[i].y += balls[i].vy * deltaTime;
            balls[i].z += balls[i].vz * deltaTime;
            balls[i].vy += gravity * deltaTime;

            if (balls[i].y <= 0.0f) {
                balls[i].active = false;
            }
        }
    }
}

void keyUp(unsigned char key, int x, int y)
{
    keyStates[key] = false;
}

// Handle special keys (arrow keys)
void specialKeys(int key, int x, int y) {
    keyStates[key] = true;
    glutPostRedisplay();
}

//esc key
void specialKeysUp(int key, int x, int y) {
    keyStates[key] = false;
    glutPostRedisplay();
}

//Keyboard control
void keyOperations() {
	if (currentState == MENU || currentState == PAUSED) {
        if (keyStates[GLUT_KEY_UP]) {
            keyStates[GLUT_KEY_UP] = false;
            selectedMenuItem--;
            if (selectedMenuItem < 0) {
                selectedMenuItem = 1;
            }
        }
        if (keyStates[GLUT_KEY_DOWN]) {
            keyStates[GLUT_KEY_DOWN] = false;
            selectedMenuItem++;
            if (selectedMenuItem > 1) {
                selectedMenuItem = 0;
            }
        }
        if (keyStates['\r']) {
            keyStates['\r'] = false;
            if (selectedMenuItem == 0) {
                currentState = PLAYING;
                if (!timerActive) {
                    timerActive = true;
                    glutTimerFunc(1000, updateTimer, 0);
                }
            } else if (selectedMenuItem == 1) {
                exit(0);
            }
        }   
    } else if (currentState == PLAYING) {
		float tankRotationSpeed = 0.1f;
		if (keyStates['a']) {
			turningDirection = 0.5f;
		} else if (keyStates['d']) {
			turningDirection = -0.5f;
		} else {
			turningDirection = 0.0f;
		}

		if (keyStates['w']) {
			float deltaZ = cos((tankRotationAngle + turningDirection * 5.0f) * M_PI / 180.0f) * 0.05f;
			float deltaX = sin((tankRotationAngle + turningDirection * 5.0f) * M_PI / 180.0f) * 0.05f;
			tankPosition.z += deltaZ;
			tankPosition.x += deltaX;
			tankRotationAngle += turningDirection * 1.5f;

			wheelRotationAngle -= 1.0f;
		}
		if (keyStates['s']) {
			float deltaZ = cos((tankRotationAngle + turningDirection * 5.0f) * M_PI / 180.0f) * 0.05f;
			float deltaX = sin((tankRotationAngle + turningDirection * 5.0f) * M_PI / 180.0f) * 0.05f;
			tankPosition.z -= deltaZ;
			tankPosition.x -= deltaX;
			tankRotationAngle -= turningDirection * 1.5f;
			wheelRotationAngle += 1.0f;
		}
		if (keyStates[27]) {
            currentState = PAUSED;
            timerActive = false;
            selectedMenuItem = 0; 
            keyStates[27] = false;
        }
	}
}

// Mouse Interaction
void mouse(int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        prevMouseX = -1;
    }
    cameraManip.handleMouse(button, state, x, y);
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		fireBall(deltaTime);
	}
    glutPostRedisplay();
}

//Passive mouse
void passiveMotion(int x, int y) {
    currentMouseX = x * 0.5;
    glutPostRedisplay();
}

//Toggle front view
void keyPressed(unsigned char key, int x, int y)
{
    keyStates[key] = true;

    if (key == 32)
    {
        spacebarPressed = !spacebarPressed; 
    }
}

//Loading varios extension pictures
GLuint loadPNGTexture(const std::string &filename) {
    int width, height, channels;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = channels == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    return textureID;
}

//Draw maze with coins
void drawMaze(const Matrix4x4& ModelViewMatrix, const Vector3f& tankPosition) {
    //maze with coins
	float TileScale = 2.0f;
	float mazeScale = 4.0f;
	float coinScale = 0.8f;
	float coinHeight = 3.0f;
	coinRotationAngle += 0.5f;
	Vector3f mazeOffset(0.0f, -1.5f, 0.0f);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			//Tile
			if (mazeLayout[i][j] == 1) {
				//Binding texture for Tile
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, TileTexture);
				glUniform1i(TextureMapUniformLocation, 0);

				Matrix4x4 TileModelViewMatrix = cameraManip.apply(ModelViewMatrix);
				TileModelViewMatrix.translate(mazeOffset.x + j * mazeScale, mazeOffset.y, mazeOffset.z + i * mazeScale);
				TileModelViewMatrix.scale(TileScale, TileScale, TileScale);

				glUniformMatrix4fv(
					MVMatrixUniformLocation,
					1,
					false,
					TileModelViewMatrix.getPtr());
				
				TileMesh.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
                glBindTexture(GL_TEXTURE_2D, 0);
			}
			//coins
			for (int j = 0; j < MAX_COINS; ++j) {
				if (coins[j].active && coinPositions[coins[j].row][coins[j].col] == 1) {
					//Binding texture for the coin
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, coinTexture);
					glUniform1i(TextureMapUniformLocation, 1);

					Matrix4x4 coinModelViewMatrix = cameraManip.apply(ModelViewMatrix);
					coinModelViewMatrix.translate(mazeOffset.x + coins[j].col * mazeScale, mazeOffset.y + TileScale + coinHeight, mazeOffset.z + coins[j].row * mazeScale);
					coinModelViewMatrix.rotate(coinRotationAngle, 0.0f, 1.0f, 0.0f);
					coinModelViewMatrix.scale(coinScale, coinScale, coinScale);

					glUniformMatrix4fv(
						MVMatrixUniformLocation,
						1,
						false,
						coinModelViewMatrix.getPtr());

					coinMesh.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
                    glBindTexture(GL_TEXTURE_2D, 1);
				}
			}
		}
	}
}

//Draw tank
void drawtank(const Matrix4x4& ModelViewMatrix) {
    //Binding texture for the tank
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tankTexture);
	glUniform1i(TextureMapUniformLocation, 2);

	// body
	Matrix4x4 ModelViewMatrix3 = cameraManip.apply(ModelViewMatrix);
	ModelViewMatrix3.translate(tankPosition.x, tankPosition.y, tankPosition.z);
	ModelViewMatrix3.rotate(tankRotationAngle, 0.0f, 1.0f, 0.0f);
	glUniformMatrix4fv(
		MVMatrixUniformLocation, 
		1, 
		false, 
		ModelViewMatrix3.getPtr());
	bodyMesh.Draw(vertexPositionAttribute,vertexNormalAttribute,vertexTexcoordAttribute);

	Matrix4x4 frontWheelModelViewMatrix = ModelViewMatrix3;
    Matrix4x4 backWheelModelViewMatrix = ModelViewMatrix3;
	
	Vector3f frontWheelPosition = front_wheelMesh.getMeshCentroid();
	Vector3f backWheelPosition = back_wheelMesh.getMeshCentroid();

	float frontWheelsRotation = turningDirection * 30.0f;

	frontWheelModelViewMatrix.translate(frontWheelPosition.x, frontWheelPosition.y, frontWheelPosition.z);
	frontWheelModelViewMatrix.rotate(frontWheelsRotation, 0.0f, 1.0f, 0.0f);
	frontWheelModelViewMatrix.rotate(-wheelRotationAngle, 1.0f, 0.0f, 0.0f);
	frontWheelModelViewMatrix.translate(-frontWheelPosition.x, -frontWheelPosition.y, -frontWheelPosition.z);

	backWheelModelViewMatrix.translate(backWheelPosition.x, backWheelPosition.y, backWheelPosition.z);
	backWheelModelViewMatrix.rotate(-wheelRotationAngle, 1.0f, 0.0f, 0.0f);
	backWheelModelViewMatrix.translate(-backWheelPosition.x, -backWheelPosition.y, -backWheelPosition.z);

	//Front wheel
    glUniformMatrix4fv(
		MVMatrixUniformLocation, 
		1, 
		false, 
		frontWheelModelViewMatrix.getPtr());
	front_wheelMesh.Draw(vertexPositionAttribute,vertexNormalAttribute,vertexTexcoordAttribute);

	//Backack wheel
    glUniformMatrix4fv(
		MVMatrixUniformLocation, 
		1, 
		false, 
		backWheelModelViewMatrix.getPtr());
	back_wheelMesh.Draw(vertexPositionAttribute,vertexNormalAttribute,vertexTexcoordAttribute);

	//Turret
	Matrix4x4 turretModelViewMatrix = ModelViewMatrix3;
	turretRotationAngle = (currentMouseX - glutGet(GLUT_WINDOW_WIDTH)/ 4.0f) * 0.5f;
	turretModelViewMatrix.rotate(-turretRotationAngle, 0.0f, 1.0f, 0.0f);
	glUniformMatrix4fv(
		MVMatrixUniformLocation, 
		1, 
		false, 
		turretModelViewMatrix.getPtr());
	turretMesh.Draw(vertexPositionAttribute,vertexNormalAttribute,vertexTexcoordAttribute);
}

//Drawing and updating balls
void drawBalls(const Matrix4x4& ModelViewMatrix) {
    
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, ballTexture);
	glUniform1i(TextureMapUniformLocation, 3);

    updateBalls(deltaTime);

	for (int i = 0; i < MAX_BALLS; ++i) {
		if (balls[i].active) {
			Matrix4x4 ModelViewMatrixBall = cameraManip.apply(ModelViewMatrix);
			ModelViewMatrixBall.translate(balls[i].x, balls[i].y, balls[i].z);
			ModelViewMatrixBall.scale(0.3f,0.3f,0.3f);
			glUniformMatrix4fv(
				MVMatrixUniformLocation, 
				1, 
				false, 
				ModelViewMatrixBall.getPtr());
			ballMesh.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
		}
	}
}

//Camera view
void updateCamera(Matrix4x4& ModelViewMatrix) {
   
    Vector3f bodyCentroid = bodyMesh.getMeshCentroid();
    Vector3f rearCameraOffset(0.0f, 2.0f, -5.0f);
    Vector3f frontCameraOffset(0.0f, 2.0f, 5.0f);
	Vector3f turretCameraOffset(4.5f, 4.0f, 0.0f);
    Vector3f cameraOffset;
    float cameraSpeed = 5.0f * deltaTime;

	/*if (aimingMode) {
		cameraOffset = turretCameraOffset;
	}*/
		cameraOffset = spacebarPressed ? frontCameraOffset : rearCameraOffset;

	// Calculate the new camera offset based on the tankRotationAngle
	float offsetX = -cameraOffset.z * sin(tankRotationAngle * M_PI / 180.0f);
	float offsetZ = -cameraOffset.z * cos(tankRotationAngle * M_PI / 180.0f);

	Vector3f updatedCameraOffset(
		offsetX,
		cameraOffset.y,
		offsetZ
	);

	// Update the camera focus point based on the tank's position and the updated camera offset
	updatedCameraPosition = tankPosition + updatedCameraOffset;
	Vector3f upVector(0.0f, 1.0f, 0.0f);
	ModelViewMatrix.lookAt(updatedCameraPosition, tankPosition, upVector);
}

//Collision function
bool isBallCoinCollision(const BallData& ball, const Coin& coin) {
    float distance = sqrt(pow(ball.x - coin.x, 2) + pow(ball.y - coin.y, 2) + pow(ball.z - coin.z, 2));
    float collisionDistance = 1.0f;

    return distance <= collisionDistance;
}

//Check for collision
void checkBallCoinCollisions() {
    for (int i = 0; i < MAX_BALLS; ++i) {
        if (balls[i].active) {
            for (int j = 0; j < MAX_COINS; ++j) {
                if (coins[j].active && isBallCoinCollision(balls[i], coins[j])) {
                    coins[j].active = false;
                    score++;
                    break;
                }
            }
        }
    }
}

//Initialize coins
void initCoins() {
    int coinIndex = 0;
	float TileScale = 2.0f;
    float coinHeight = 2.5f;
    float mazeScale = 4.0f;
    Vector3f mazeOffset(0.0f, -1.5f, 0.0f);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (coinPositions[i][j] == 1) {
                coins[coinIndex].x = mazeOffset.x + j * mazeScale;
                coins[coinIndex].y = mazeOffset.y + TileScale + coinHeight;
                coins[coinIndex].z = mazeOffset.z + i * mazeScale;
                coins[coinIndex].active = true;
                coins[coinIndex].row = i;
                coins[coinIndex].col = j;
                coinIndex++;
            }
            if (coinIndex >= MAX_COINS) {
                break;
            }
        }
        if (coinIndex >= MAX_COINS) {
            break;
        }
    }
}

//Boolean to determine if tank is on a tile
bool istankOnTile(const Vector3f& tankPosition, int row, int col, float TileScale, float mazeScale, const Vector3f& mazeOffset) {
    // Calculate the position of the Tile in world coordinates
    Vector3f TilePosition(mazeOffset.x + col * mazeScale, mazeOffset.y, mazeOffset.z + row * mazeScale);

    // Calculate the size of the Tile
    Vector3f TileSize(TileScale, TileScale, TileScale);

    // Check if all four bottom corners of the tank are on the Tile
    std::vector<Vector3f> tankCorners = {
        Vector3f(tankPosition.x - tankSize.x / 2, tankPosition.y - tankSize.y / 2, tankPosition.z - tankSize.z / 2),
        Vector3f(tankPosition.x + tankSize.x / 2, tankPosition.y - tankSize.y / 2, tankPosition.z - tankSize.z / 2),
        Vector3f(tankPosition.x - tankSize.x / 2, tankPosition.y - tankSize.y / 2, tankPosition.z + tankSize.z / 2),
        Vector3f(tankPosition.x + tankSize.x / 2, tankPosition.y - tankSize.y / 2, tankPosition.z + tankSize.z / 2)
    };

    for (const Vector3f& corner : tankCorners) {
        bool onTileHorizontally = (corner.x >= TilePosition.x - TileSize.x / 2) && (corner.x <= TilePosition.x + TileSize.x / 2) &&
                                    (corner.z >= TilePosition.z - TileSize.z / 2) && (corner.z <= TilePosition.z + TileSize.z / 2);
        float TileTop = TilePosition.y + TileSize.y / 2;
        
        if (!onTileHorizontally || fabs(corner.y - TileTop) > 0.1f) {
            return false;
        }
    }

    return true;
}
//Falling tank calculation
Vector3f fallingtankPosition(const Vector3f& tankPosition, float TileScale, float mazeScale, const Vector3f& mazeOffset) {
    bool tankOnTile = false;
    
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (mazeLayout[i][j] == 1) {
                Vector3f TilePosition(mazeOffset.x + j * mazeScale, mazeOffset.y, mazeOffset.z + i * mazeScale);

                if (abs(TilePosition.x - tankPosition.x) <= (TileScale / 2.0f + tankSize.x / 2.0f + 0.1f) &&
					abs(TilePosition.z - tankPosition.z) <= (TileScale / 2.0f + tankSize.z / 2.0f + 0.1f) &&
					abs((TilePosition.y + TileScale / 2.0f) - (tankPosition.y - tankSize.y / 2.0f)) <= 0.2f) {
                    tankOnTile = true;
                }
            }
        }
    }

    Vector3f newPosition = tankPosition;
    if (!tankOnTile) {
        newPosition.y -= 0.1f;
    }
    return newPosition;
}

//Function to draw menus
void drawMenu() {
     GLint currentMatrixMode;
    glGetIntegerv(GL_MATRIX_MODE, &currentMatrixMode);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    float y = windowHeight / 2.0f + 40.0f;
    float x = windowWidth / 2.0f - 50.0f; 

    if (currentState == MENU) {
        drawText("3D TANK GAME", 1, 0, 0, x - 10, y + 100);
    }

        for (int i = 0; i < 2; i++) {
        float r, g, b;
        if (i == selectedMenuItem) {
            r = selectedItemColor[0];
            g = selectedItemColor[1];
            b = selectedItemColor[2];
        } else {
            r = menuItemColor[0];
            g = menuItemColor[1];
            b = menuItemColor[2];
        }

        if (i == 0) {
            if (currentState == MENU) {
                drawText("Start Game", r, g, b, x, y);
            } else if (currentState == PAUSED) {
                drawText("Game paused", 1, 0, 0, x - 10, y + 100);
                drawText("Continue", r, g, b, x, y);
            }
        } else if (i == 1) {
            drawText("Quit", r, g, b, x, y - 30);
        }
    }
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(currentMatrixMode);
}

//Function to draw text
void drawText(std::string text, float r, float g, float b, float x, float y) {
    glColor3f(r, g, b); 
    glRasterPos2f(x, y); 
    for (unsigned int i = 0; i < text.size(); i++) 
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

//Timer function
void updateTimer(int value) {
    if (timerActive && currentState == PLAYING) {
        timer--;
        if (timer <= 0) {
            timerActive = false;
        } else {
            glutTimerFunc(1000, updateTimer, 0);
        }
    }
}

//Collecting coins
void collectCoin() {
    score++;
}

// Display score and timer
void drawHUD() {
    glUseProgram(0);           
    glDisable(GL_DEPTH_TEST); 
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    
    std::string timerText = "Time left: " + std::to_string(timer);
    std::string scoreText = "Score: " + std::to_string(score);

    drawText(timerText, 1.0f, 0.0f, 0.0f, -0.7f, 0.9f); 
    drawText(scoreText, 1.0f, 0.0f, 0.0f, -1.0f, 0.9f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}

