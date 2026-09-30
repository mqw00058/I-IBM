#include "GLWidget.h"
//#include <EmbeddedDeform/math/vector3d.h>
#include <qapplication.h>
/*GLfloat m_currentViewport[4] = {0.0f, 0.0f, 0.0f, 0.0f};
GLfloat m_currentTransform[16] =
{
0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f,
0.0f,0.0f,0.0f,0.0f
};

GLfloat m_currentZoomScale = 0.0f;
GLfloat m_currentPanOffsetX = 0.0f;
GLfloat m_currentPanOffsetY = 0.0f;
*/

class InterTriMesh;
GLWidget::GLWidget(QWidget* parent)
: QGLWidget(parent)
{
	// 현재 시점을 저장하고 저장된 시점을 사용하여 사용자 시점을 지정
	m_currentTransform[0] = 0.544677f;	m_currentTransform[1] = -0.362249f;
	m_currentTransform[2] = 0.756375f;	m_currentTransform[3] = 0.f;
	m_currentTransform[4] = 0.838644f;	m_currentTransform[5] = 0.233635f;
	m_currentTransform[6] = -0.492026f;	m_currentTransform[7] = 0.f;
	m_currentTransform[8] = 0.001520f;	m_currentTransform[9] = 0.902324f;
	m_currentTransform[10] = 0.431054f;	m_currentTransform[11] = 0.f;
	m_currentTransform[12] = 0.f;			m_currentTransform[13] = 0.f;
	m_currentTransform[14] = 0.f;			m_currentTransform[15] = 1.f;

	m_currentZoomScale = 10.f;
	m_currentPanOffsetX = m_currentPanOffsetY = 0.f;
	m_ProjectionVolumeXOffset = m_ProjectionVolumeYOffset = 0.f;

	m_nSelectionMode.bDeformMove = m_nSelectionMode.bDeformSelection = m_nSelectionMode.bTriSelection = m_nSelectionMode.bVerSelection = false;

	ext_current_model_index = 0;
	//updateGL();
	///*아래의 메소드는 현재의 위젯을 OpenGL 용으로 바꾸어 주는 메소드이다.*/
	////makeCurrent();


	index_current_deform = -1;
	bShowGraph = false;
	mymesh = NULL;
	m_bDrawingOptions.clear();
	m_bSmooth = false;
	m_bTexture = false;



}

void GLWidget::GetModel(std::vector<Geometry3D*>* _mesh)
{
	mymesh = _mesh;
	GLUpdateFaceIndices();
	//int a= (*mymesh)[i].m_mesh.n_vertices();


	if (mymesh != NULL)
	{
		drawingoptions drwops;
		drwops.m_bVertex = false;
		drwops.m_bEdge = false;
		drwops.m_bFace = true;
		drwops.m_bVertexNormal = false;
		drwops.m_bBoundingBox = false;
		drwops.m_bVertexColor = false;
		drwops.m_bFaceColor = false;
		m_bDrawingOptions.push_back(drwops);
	}

	m_bModelLoaded = true;

	//updateGL();

}
GLWidget::~GLWidget()
{

}

void GLWidget::initializeGL()
{
	//You should not call makeCurrent() or doneCurrent() inside paintGL(), resizeGL() or initializeGL() since OpenGL context is managed by Qt and already made / done current there.
	//makeCurrent();

	GLInitializeRC();
	GLInitializeState();
	//doneCurrent();

}

void GLWidget::GLInitializeRC(void)
{
	glClearColor(1.f, 1.f, 1.f, 1.0f);

	glDisable(GL_DITHER);

	glEnable(GL_DEPTH_TEST);

	m_nManiMode = Trackball;
	//	m_nSelectionMode = NoSelection;

	m_LightTrackingAngle = 0.f;
	m_LightTransform[0] = 1.f; m_LightTransform[4] = 0.f; m_LightTransform[8] = 0.f; m_LightTransform[12] = 0.f;
	m_LightTransform[1] = 0.f; m_LightTransform[5] = 1.f; m_LightTransform[9] = 0.f; m_LightTransform[13] = 0.f;
	m_LightTransform[2] = 0.f; m_LightTransform[6] = 0.f; m_LightTransform[10] = 1.f; m_LightTransform[14] = 0.f;
	m_LightTransform[3] = 0.f; m_LightTransform[7] = 0.f; m_LightTransform[11] = 0.f; m_LightTransform[15] = 1.f;

	m_TrackingAngle = 0.f;
	m_vPrevVec[0] = 0.f; m_vPrevVec[1] = 0.f; m_vPrevVec[2] = 0.f;
	m_vCurrVec[0] = 0.f; m_vCurrVec[1] = 0.f; m_vCurrVec[2] = 0.f;

	m_ZoomScale = 10.f; m_PanOffsetX = m_PanOffsetY = 0.f;

	m_currentPanOffsetX = m_currentPanOffsetY = 0.f;
	m_ProjectionVolumeXOffset = m_ProjectionVolumeYOffset = 0.f;

	m_Left = -10.f; m_Right = 10.f; m_Bottom = -10.f; m_Top = 10.f;
	m_Near = 1.f; m_Far = 500000.f;


	// ISO view와 같이 보이기 위해서 필요
	m_mxTransform[0] = 0.544677f; m_mxTransform[1] = -0.362249f;
	m_mxTransform[2] = 0.756375f; m_mxTransform[3] = 0.f;
	m_mxTransform[4] = 0.838644f; m_mxTransform[5] = 0.233635f;
	m_mxTransform[6] = -0.492026f; m_mxTransform[7] = 0.f;
	m_mxTransform[8] = 0.001520f; m_mxTransform[9] = 0.902324f;
	m_mxTransform[10] = 0.431054f; m_mxTransform[11] = 0.f;
	m_mxTransform[12] = 0.f; m_mxTransform[13] = 0.f;
	m_mxTransform[14] = 0.f; m_mxTransform[15] = 1.f;


	glLoadIdentity();

	/*
	// Lighting Set
	GLfloat ambient[] = {0.0f, 0.0f, 0.0f, 1.f};
	GLfloat diffuse[] = {1.f, 1.f, 1.f, 1.f};
	GLfloat position[] = {0.f, 0.f, 3.f, 0.f};
	GLfloat spotDir[] = {0.f, 0.f, -1.f};
	GLfloat specref[] = { 0.9f, 0.9f, 0.9f, 1.f };

	//	GLfloat lmodel_ambient[] = {0.2f, 0.2f, 0.2f, 1.f};
	//GLfloat local_view[] = {0.f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);


	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	//glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
	*/

	//Light 설정
	/////////////////////////////////////////////////////////////////
	/*glEnable(GL_LIGHTING);
	GLfloat pos1[] = { 0.1,  0.1, -0.02, 0.0};
	GLfloat pos2[] = {-0.1,  0.1, -0.02, 0.0};
	GLfloat pos3[] = { 0.0,  0.0,  0.1,  0.0};

	GLfloat col1[] = { 0.7,  0.7,  0.8,  1.0};
	GLfloat col2[] = { 0.8,  0.7,  0.7,  1.0};
	GLfloat col3[] = { 1.0,  1.0,  1.0,  1.0};

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_POSITION, pos1);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,  col1);
	glLightfv(GL_LIGHT0,GL_SPECULAR, col1);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1,GL_POSITION, pos2);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,  col2);
	glLightfv(GL_LIGHT1,GL_SPECULAR, col2);

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2,GL_POSITION, pos3);
	glLightfv(GL_LIGHT2,GL_DIFFUSE,  col3);
	glLightfv(GL_LIGHT2,GL_SPECULAR, col3);
	*/

	GLfloat pos1[] = { 0.1, 0.1, -0.02, 0.0 };
	GLfloat pos2[] = { -0.1, 0.1, -0.02, 0.0 };
	GLfloat pos3[] = { 0.0, 0.0, 0.1, 0.0 };
	GLfloat col1[] = { 0.7, 0.7, 0.8, 1.0 };
	GLfloat col2[] = { 0.8, 0.7, 0.7, 1.0 };
	GLfloat col3[] = { 1.0, 1.0, 1.0, 1.0 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, col1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, col1);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, pos2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, col2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, col2);

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_POSITION, pos3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, col3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, col3);

	//////////////////////////////////////////////////////////////////////

	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	//glMaterialf(GL_FRONT, GL_SHININESS, 103);			// 2007-3-16 이전

	//// Silver
	//const GLfloat m_Ambient[]	= {0.19225f, 0.19225f, 0.19225f, 1.0f};
	//const GLfloat m_Diffuse[]	= {0.50754f, 0.50754f, 0.50754f, 1.0f};
	//const GLfloat m_Specular[]	= {0.508273f, 0.508273f, 0.508273f, 1.0f};
	//const GLfloat m_fShininess	= 51.2f;

	//// Polished Silver
	//const GLfloat m_Ambient[]	= {0.23125f, 0.23125f, 0.23125f, 1.0f};
	//const GLfloat m_Diffuse[]	= {0.2775f, 0.2775f, 0.2775f, 1.0f};
	//const GLfloat m_Specular[]	= {0.773911f, 0.773911f, 0.773911f};
	//const GLfloat m_fShininess	= 89.6f;

	//// Emerald
	//const GLfloat m_Ambient[]	= {0.0215f, 0.1745f, 0.0215f, 0.55f};
	//const GLfloat m_Diffuse[]	= {0.07568f, 0.61424f, 0.07568f, 0.55f};
	//const GLfloat m_Specular[]	= {0.633f, 0.727811f, 0.633f, 0.55f};
	//const GLfloat m_fShininess	= 76.8f;

	//// Jude
	//const GLfloat m_Ambient[]	= {0.135f, 0.2225f, 0.1575f, 0.95f};
	//const GLfloat m_Diffuse[]	= {0.54f, 0.89f, 0.63f, 0.95f};
	//const GLfloat m_Specular[]	= {0.316228f, 0.316228f, 0.316228f, 0.55f};
	//const GLfloat m_fShininess	= 12.8f;

	//// Obsidian
	//const GLfloat m_Ambient[]	= {0.05375f, 0.05f, 0.06625f, 0.82f};
	//const GLfloat m_Diffuse[]	= {0.18275f, 0.17f, 0.22525f, 0.82f};
	//const GLfloat m_Specular[]	= {0.332741f, 0.328634f, 0.346435f, 0.82f};
	//const GLfloat m_fShininess	= 12.8f;

	// Pearl
	//	const GLfloat m_Ambient[]	= {0.25f, 0.20725f, 0.20725f, 0.922f};
	//	const GLfloat m_Diffuse[]	= {1.0f, 0.829f, 0.829f, 0.922f};
	//	const GLfloat m_Specular[]	= {0.296648f, 0.296648f, 0.296648f, 0.922f};
	//	const GLfloat m_fShininess	= 11.264f;

	//// Ruby
	//const GLfloat m_Ambient[]	= {0.1745f, 0.01175f, 0.01175f, 0.55f};
	//const GLfloat m_Diffuse[]	= {0.61424f, 0.04136f, 0.04136f, 0.55f};
	//const GLfloat m_Specular[]	= {0.727811f, 0.626959f, 0.626959f, 0.55f};
	//const GLfloat m_fShininess	= 76.8f;

	//// Turquoise
	//const GLfloat m_Ambient[]	= {0.1f, 0.18725f, 0.1745f, 0.8f};
	//const GLfloat m_Diffuse[]	= {0.396f, 0.74151f, 0.69012f, 0.8f};
	//const GLfloat m_Specular[]	= {0.297254f, 0.30829f, 0.306678f, 0.8f};
	//const GLfloat m_fShininess	= 12.8f;

	//// Black Plastic
	//const GLfloat m_Ambient[]	= {0.0f, 0.0f, 0.0f, 1.0f};
	//const GLfloat m_Diffuse[]	= {0.01f, 0.01f, 0.01f, 1.0f};
	//const GLfloat m_Specular[]	= {0.5f, 0.5f, 0.5f, 1.0f};
	//const GLfloat m_fShininess	= 32.0f;

	//// Black Rubber
	//const GLfloat m_Ambient[]	= {0.02f, 0.02f, 0.02f, 1.0f};
	//const GLfloat m_Diffuse[]	= {0.01f, 0.01f, 0.01f, 1.0f};
	//const GLfloat m_Specular[]	= {0.4f, 0.4f, 0.4f, 1.0f};
	//const GLfloat m_fShininess	= 10.0f;

	//// Brass
	//const GLfloat m_Ambient[]	= {0.329412f, 0.223529f, 0.027451f, 1.0f};
	//const GLfloat m_Diffuse[]	= {0.780392f, 0.568627f, 0.113725f, 1.0f};
	//const GLfloat m_Specular[]	= {0.922157f, 0.922157f, 0.922157f, 1.0f};
	//const GLfloat m_fShininess	= 27.8974f;

	//// Bronze
	//const GLfloat m_Ambient[]	= {0.2125f, 0.1275f, 0.054f, 1.0f};
	//const GLfloat m_Diffuse[]	= {0.714f, 0.4284f, 0.18144f, 1.0f};
	//const GLfloat m_Specular[]	= {0.393548f, 0.271906f, 0.166721f, 1.0f};
	//const GLfloat m_fShininess	= 25.6f;

	//// Polished Bronze
	//const GLfloat m_Ambient[]	= {0.25f, 0.148f, 0.06475f, 1.0f};
	//const GLfloat m_Diffuse[]	= {0.4f, 0.2368f, 0.1036f, 1.0f};
	//const GLfloat m_Specular[]	= {0.774597f, 0.458561f, 0.200621f, 1.0f};
	//const GLfloat m_fShininess	= 76.8f;

	//// Chrome
	//const GLfloat m_Ambient[]	= {0.25f, 0.25f, 0.25f, 1.0f};
	//const GLfloat m_Diffuse[]	= {0.4f, 0.4f, 0.4f, 1.0f};
	//const GLfloat m_Specular[]	= {0.774597f, 0.774597f, 0.774597f, 1.0f};
	//const GLfloat m_fShininess	= 76.8f;

	//// Copper
	//const GLfloat m_Ambient[]	= {0.19125f, 0.0735f, 0.0225f, 1.0f};
	//const GLfloat m_Diffuse[]	= {0.7038f, 0.27048f, 0.0828f, 1.0f};
	//const GLfloat m_Specular[]	= {0.25677f, 0.137622f, 0.086014f, 1.0f};
	//const GLfloat m_fShininess	= 12.8f;

	//// Polished Copper
	//const GLfloat m_Ambient[]	= {0.2295f, 0.08825f, 0.0275f, 1.0f};
	//const GLfloat m_Diffuse[]	= {0.5508f, 0.2118f, 0.066f, 1.0f};
	//const GLfloat m_Specular[]	= {0.580594f, 0.223257f, 0.0695701f, 1.0f};
	//const GLfloat m_fShininess	= 51.2f;

	//// Gold
	//const GLfloat m_Ambient[]	= {0.24725f, 0.1995f, 0.0745f, 1.0f};
	//const GLfloat m_Diffuse[]	= {0.75164f, 0.60648f, 0.22648f, 1.0f};
	//const GLfloat m_Specular[]	= {0.628281f, 0.555802f, 0.366065f, 1.0f};
	//const GLfloat m_fShininess	= 51.2f;

	//// Polished Gold
	//const GLfloat m_Ambient[]	= {0.24725f, 0.2245f, 0.0645f, 1.0f};
	//const GLfloat m_Diffuse[]	= {0.34615f, 0.3143f, 0.0903f, 1.0f};
	//const GLfloat m_Specular[]	= {0.797357f, 0.7233991f, 0.208006f, 1.0f};
	//const GLfloat m_Specular[]	= {1.f, 1.f, 1.f, 1.0f};
	//const GLfloat m_fShininess	= 120.0f;

	////
	//const GLfloat m_Ambient[]	= {0.105882f, 0.058824f, 0.113725f, 1.0f};
	//const GLfloat m_Diffuse[]	= {0.427451f, 0.470588f, 0.541176f, 1.0f};
	//const GLfloat m_Specular[]	= {0.333333f, 0.333333f, 0.521569f, 1.0f};
	//const GLfloat m_fShininess	= {9.84615f);

	////
	//const GLfloat m_Ambient[]	= {0.19225f, 0.19225f, 0.19225f, 1.0f};
	//const GLfloat m_Diffuse[]	= {0.50754f, 0.50754f, 0.50754f, 1.0f};
	//const GLfloat m_Specular[]	= {0.508273f, 0.508273f, 0.508273f, 1.0f};
	//const GLfloat m_fShininess	= 51.2f;

	const GLfloat m_Ambient[] = { 0.2f, 0.28725f, 0.2745f, 1.f };
	//const GLfloat m_Diffuse[]	= {0.5294f, 0.8078f, 0.9216f, 1.f};
	const GLfloat m_Diffuse[] = { 0.1f, 0.7490f, 1.0f, 1.f };
	const GLfloat m_Specular[] = { 1.f, 1.f, 1.f, 1.f };
	const GLfloat m_fShininess = 150.8f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_Ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_Diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_Specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &m_fShininess);

	/*

	// Fog
	GLfloat fogColor[4] = { 0.3, 0.3, 0.4, 0.3 };
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.35);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 5.0f);
	glFogf(GL_FOG_END, 25.0f);
	*/

	m_bCulled = TRUE;
	// ----- OpenGL settings -----

	glDepthFunc(GL_LEQUAL);		// Specify depth function to use

	glEnable(GL_DEPTH_TEST);    // Enable the depth buffer


	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);

	//glClearDepth(1.f);
	//glColor3f(0.7f, 0.7f, 0.7f);


}


// OpenGL State 초기화
void GLWidget::GLInitializeState()
{
	m_bModelLoaded = false;
	m_bCenter = false;// Center Adjust
	m_bLight = true;

	/*
	if (mymesh != NULL)
	{
	for (int i = 0; i<mymesh->size(); i++)
	{
	m_bDrawingOptions[i].m_bSmooth = false;
	m_bDrawingOptions[i].m_bVertex = false;
	m_bDrawingOptions[i].m_bEdge = false;
	m_bDrawingOptions[i].m_bFace = true;
	m_bDrawingOptions[i].m_bVertexNormal = false;
	m_bDrawingOptions[i].m_bBoundingBox = false;
	m_bDrawingOptions[i].m_bVertexColor = false;
	m_bDrawingOptions[i].m_bTexture = false;
	m_bDrawingOptions[i].m_bFaceColor = false;
	}
	}*/

	m_bDrawAxis = true;
	m_bDrawWorkspace = true;



	m_bMouseMoving = false;

	m_bLightDraw = false;

	m_defaultcolor[0] = 0.6f;
	m_defaultcolor[1] = 0.8f;
	m_defaultcolor[2] = 0.9f;
	// default color
	vc[0] = 1.f;
	vc[1] = 0.f;
	vc[2] = 0.f;

	fc[0] = 0.6f;
	fc[1] = 0.8f;
	fc[2] = 0.9f;

}

void GLWidget::resizeGL(int width, int height)
{
	/*	//Set Viewport to window system
	glViewport(0,0, width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	직교좌표계를 정의하는 함수는 다음과 같다.
	glOrtho( left, right, bottom, top, near, far );
	예를 들어 glOrth( 0, 800, 0, 600, -1, 1 ) 이라고 한다면 이는
	윈도우의 좌측 하단이 0, 0이고 우측 상단이 800, 600이 되는 스크린 좌표계가 된다.

	원근좌표계를 정의하는 함수는 다음과 같다.
	gluPerspective( fov, aspect, near, far );
	fov는 시야각, aspect는 가로세로 종횡비, near, far는 화면에 보여지는 최소, 최대 거리가 된다.
	여기에 3D이기 때문에 한가지 더 선언해야 한다.
	gluLookAt( eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ );

	위 glOrtho와 gluPerspective 함수는 동시에 사용할 수 없다.

	glOrtho( left, right, bottom, top, near, far );
	gluPerspective( fov, aspect, near, far );	gluLookAt( eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ );


	glOrtho(-width/2, width/2, -height/2, height/2, 10000.0, -10000.0);
	//glFrustum(-width/2, width/2, -height/2, height/2, 1, -1);
	//gluPerspective(45,(float)width/height, 0.01, 100.0);
	w=width;
	h=height;


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0,0,5, 0,0,0, 0,0,0);
	*/
	w = width;
	h = height;
	m_ratio = (float)height / width;
	m_Left = (-m_ZoomScale) + m_PanOffsetX /*- m_ProjectionVolumeXOffset*/;
	m_Right = m_ZoomScale + m_PanOffsetX /*+ m_ProjectionVolumeXOffset*/;
	m_Bottom = (-m_ZoomScale)*m_ratio + m_PanOffsetY /*- m_ProjectionVolumeYOffset*m_ratio*/;
	m_Top = m_ZoomScale*m_ratio + m_PanOffsetY /*+ m_ProjectionVolumeYOffset*m_ratio*/;

	//QString str =QString("%1 %2\n").arg(m_Top ).arg(m_Right);
	//logviewdockwidget->addText(str);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(m_Left, m_Right, m_Bottom, m_Top, m_Near, m_Far);
	glTranslatef(0.f, 0.f, -50000.f);

	glViewport(0, 0, width, height);
	glGetIntegerv(GL_VIEWPORT, m_vViewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void GLWidget::makeCheckImage()
{
	int i, j, c;
	for (i = 0; i < checkImageHeight; i++)
	{
		for (j = 0; j < checkImageWidth; j++)
		{
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
}
void GLWidget::GLSetTexture(GLint width, GLint height, BYTE* texture)
{
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	//glShadeModel(GL_FLAT);
	//	glEnable(GL_DEPTH_TEST);
	//makeCheckImage();
	/*

	glGenTextures(1, &texName);
	listTexName.push_back(texName);
	glBindTexture(GL_TEXTURE_2D, listTexName[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	*/
	//	listTexName.reserve(mymesh->size());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//if (mymesh != NULL)
	{
		//	for (int i = 0; i < mymesh->size(); i++)
		{
			/*	if (texName > 0)
			{
			glDeleteTextures(1, &texName);
			}
			*/
			glGenTextures(1, &texName);
			listTexName.push_back(texName);
			glBindTexture(GL_TEXTURE_2D, listTexName.back());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPLACE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPLACE);
			//cv::Mat im = cv::imread(mymesh->at(i)->m_texture_path);

			//mymesh->at(i)->m_texture_width = im.cols;
			//mymesh->at(i)->m_texture_height = im.rows;

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, mymesh->back()->m_texture);
		}
	}
}


void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	///glClearColor(.0f, .0f, 0.f, 0.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();

	if (m_nManiMode == Trackball && m_TrackingAngle > 0)
	{
		glPushMatrix();
		glLoadIdentity();

		glRotatef(m_TrackingAngle, m_vAxis[0], m_vAxis[1], m_vAxis[2]);
		glMultMatrixf((GLfloat *)m_mxTransform);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)m_mxTransform);

		m_TrackingAngle = 0.f; //중요!
		glPopMatrix();
	}
	glMultMatrixf((GLfloat *)m_mxTransform);


	if (m_bDrawWorkspace)				// Show/Hide Axis
		DrawWorkspace();

	if (m_bDrawAxis)				// Show/Hide Axis
		GLDrawAxis();


	/*

	///m_bLightDraw =true;
	if(m_bLightDraw)				// Show/Hide Light
	{
	GLfloat position[] = {0.f, 0.f, 3.f, 0.f};
	GLfloat spotDir[] = {0.f, 0.f, -1.f};
	GLfloat m_Diffuse[]	= {0.34615f, 0.3143f, 0.0903f, 1.0f};
	DrawLight(position, spotDir, m_Diffuse);
	}

	*/
	/*


	for (int i = 0; i< listTexName.size(); i++)
	{
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, listTexName[i]);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
	glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);
	}
	*/

	if (m_bModelLoaded)
	{

		for (int i = 0; i < mymesh->size(); i++)
		{

			/*
			//GLPickObjects(GLint x, GLint y) 함수
			if (m_nSelectionMode.bDeformSelection || m_nSelectionMode.bDeformMove)
			GLShowVerPickMode(&(*mymesh)[i]);
			*/

			if (m_nSelectionMode.bDeformSelection || m_nSelectionMode.bDeformMove)
			{
				if ((*mymesh)[i]->pdmesh_ != NULL && (*mymesh)[i]->pdmesh_->getSVSet().size() > 0)
				{
					draw_select_boxes();
				}
			}

			if (bShowGraph)
			{

				draw_graph();

			}

			// 선택된 vertex를 display
			if (m_nSelectionMode.bVerSelection || (*mymesh)[i]->m_verPickList.size() > 0)
				GLDrawSelectedMeshVertex((*mymesh)[i]);

			RenderScene((*mymesh)[i], &m_bDrawingOptions[i]);

			// 선택된 face를 display
			if (m_nSelectionMode.bTriSelection || (*mymesh)[i]->m_triPickList.size() > 0)
				GLDrawSelectedMeshFace((*mymesh)[i]);

		}
	}



	// Selection rectangle 표현
	if ((m_nSelectionMode.bVerSelection || m_nSelectionMode.bTriSelection || m_nSelectionMode.bDeformSelection || m_nSelectionMode.bDeformMove) && ptReal1 != ptReal2)
	{
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		glLineStipple(1, 0xAAAA);  //dot line 그리기
		glEnable(GL_LINE_STIPPLE);

		glBegin(GL_LINE_LOOP);
		glColor3f(0.f, 0.f, 0.f);
		glVertex3f(ptReal1[0], ptReal1[1], 0);
		glVertex3f(ptReal2[0], ptReal1[1], 0);
		glVertex3f(ptReal2[0], ptReal2[1], 0);
		glVertex3f(ptReal1[0], ptReal2[1], 0);
		glEnd();

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_LINE_STIPPLE);
		glPopMatrix();
	}


#ifdef _WIN32 // Kinect v2 live view (Windows only)
	if (bConnectionKinect)
	{
		//EnterCriticalSection(&(kinect.mCriticalSection)); 
		//kinect.Update();

		// Set the projection from the XYZ to the texture image
		glMatrixMode(GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glMatrixMode(GL_MODELVIEW);
		// assign vertex array
		glEnableClientState(GL_VERTEX_ARRAY);
		//WaitForSingleObject(kinect.hDepthMutex,INFINITE);

		// recompute x, y using z and focal length
		//focal length [ 1063.018  1065.133 ] ± [ 1.880  1.889 ]
		//principal point [ 962.373  526.689 ] ± [ 1.085  0.885 ]
		//distortion [ 0.042369  -0.037696  -0.002894  0.000978 ] ± [ 0.002178  0.009347  0.000238  0.000308 ]
		//WaitForSingleObject(kinect.hDepthMutex,INFINITE);		
		float fl_x = 1063.018;
		float fl_y = 1065.133;
		float pp_x = 962.373;
		float pp_y = 526.689;

		for (int rr = 0; rr < kinect.cColorHeight; rr++)
		{
			for (int cc = 0; cc < kinect.cColorWidth; cc++)
			{
				float Z = kinect.mCameraSpacePoint.at<float>(rr, cc * 3 + 2);
				if (Z > 0)
				{
					//x_p = f * x / z - cx
					//x = (x_p - cx ) * z / f
					kinect.mCameraSpacePoint.at<float>(rr, cc * 3 + 0) = (cc - pp_x) * Z / fl_x;
					kinect.mCameraSpacePoint.at<float>(rr, cc * 3 + 1) = -(rr - pp_y) * Z / fl_y;
				}
			}
		}
		//	EnterCriticalSection(&kinect.mCriticalSection); 

		glVertexPointer(3, GL_FLOAT, 0, kinect.mCameraSpacePoint.data); // mPoint.data);
		//ReleaseMutex(kinect.hDepthMutex);
		//ReleaseMutex(kinect.hDepthMutex);
		// assign texture coordinates
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glTexCoordPointer(2, GL_FLOAT, 0, dispTextureCoordinates.data);

		// assign texture image
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnable(GL_TEXTURE_2D);

		WaitForSingleObject(kinect.hColorMutex, INFINITE);

		//cv::Mat mColor;
		//		kinect.m_colorImage.convertTo(mColor,CV_8UC3);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, kinect.cColorWidth, kinect.cColorHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, kinect.m_colorImage.data);
		//	mColor.release();
		// draw elements
		glPointSize(2.0f);
		glDrawElements(GL_POINTS, kinect.cColorWidth * kinect.cColorHeight, GL_UNSIGNED_INT, dispTextureIndices.data);
		ReleaseMutex(kinect.hColorMutex);

		//	LeaveCriticalSection(&kinect.mCriticalSection);

		// disable what enabled earlier
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);

		//LeaveCriticalSection(&(kinect.mCriticalSection));
		//updateGL();
	}
#endif

	glPopMatrix();
	glFlush();
}

// Draw vertices of mesh again for picking
void GLWidget::GLShowVerPickMode(Geometry3D *_mesh)
{

	GeoTriMesh::VertexIter v_it(_mesh->m_mesh.vertices_begin()), v_End(_mesh->m_mesh.vertices_end());

	glPushMatrix();

	glDisable(GL_LIGHTING);

	glInitNames();
	glPointSize(5);

	int iVertex;				// vertex index

	for (; v_it != v_End; ++v_it)
	{
		iVertex = v_it.handle().idx();
		glPushName(iVertex);
		if (!_mesh->m_mesh.status(v_it).selected())
			glColor3f(0.3f, 0.0f, 1.0f);
		else
			glColor3f(0.8f, 0.2f, 0.2f);
		glBegin(GL_POINTS);
		glVertex3f(_mesh->m_mesh.point(v_it)[0], _mesh->m_mesh.point(v_it)[1], _mesh->m_mesh.point(v_it)[2]);
		glEnd();
		glPopName();
	}

	glPointSize(1);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

}

void GLWidget::GLShowTriPickMode(Geometry3D *_mesh)
{
	//--------------------------기본적인 Setting----------------------------------------//
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //투명 효과를 위한 함수
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);  //깊이 버퍼를 읽기 전용으로
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);    //원래 모델보다 조금더 앞으로 나와라.. 	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// FACE

	glPushMatrix();

	glInitNames();

	GeoTriMesh::ConstFaceIter		f_it(_mesh->m_mesh.faces_begin()), f_End(_mesh->m_mesh.faces_end());
	GeoTriMesh::ConstFaceVertexIter	fv_it;

	glEnableClientState(GL_VERTEX_ARRAY);
	GL::glVertexPointer(3, GL_FLOAT, 0, _mesh->m_mesh.points());

	int iTriIndex;				// triangle index
	glColor3f(1.0f, 1.0f, 1.0f);

	//
	for (; f_it != f_End; ++f_it)
	{
		iTriIndex = f_it.handle().idx();

		//	
		glPushName(iTriIndex);
		glBegin(GL_TRIANGLES);
		fv_it = _mesh->m_mesh.cfv_iter(f_it.handle());
		glArrayElement(fv_it.handle().idx());
		//	glPushName(fv_it.handle().idx());
		++fv_it;
		glArrayElement(fv_it.handle().idx());
		//	glPushName(fv_it.handle().idx());
		++fv_it;
		glArrayElement(fv_it.handle().idx());
		//glPushName(fv_it.handle().idx());
		glEnd();

		glPopName();

	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glPopMatrix();
}

void GLWidget::RenderScene(Geometry3D *_mesh, drawingoptions* drwopt)
{


	if (_mesh->m_mesh.n_vertices() == 0){

		return;
	}

	// Lighting
	if (m_bLight)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	// Smooth
	if (m_bSmooth)
		glShadeModel(GL_SMOOTH);
	else
		glShadeModel(GL_FLAT);

	if (m_bMouseMoving)
		GLDrawMeshElements(_mesh, 1);
	else
	{
		/*	if(m_bFeaturepoint)
		GLDrawMeshElements(mymesh, 3);		// Draw feature points

		if(m_bFeatureedge)
		GLDrawMeshElements(mymesh,4);		// Draw feature points
		*/

		if (drwopt->m_bFace)
		{
			//	if (_mesh->m_mesh.n_faces() != 0)
			GLDrawMeshElements(_mesh, 0);		// Draw Mesh as triangle
		}

		if (drwopt->m_bVertex)
			GLDrawMeshElements(_mesh, 1);		// Draw Mesh as point

		if (drwopt->m_bEdge)
		{
			//	if (_mesh->m_mesh.n_edges() != 0)
			GLDrawMeshElements(_mesh, 2);		// Draw mesh as edges
		}

		if (drwopt->m_bVertexNormal){
			GLDrawMeshElements(_mesh, 8);
		}



		/*if(m_bBoundary)
		{
		GLDrawMeshElements(mymesh, 10);
		}*/
		if (drwopt->m_bBoundingBox)
		{
			GLDrawMeshElements(_mesh, 7);
		}

	}
}

void GLWidget::GLUpdateFaceIndices()
{
	GeoTriMesh::ConstFaceIter        f_it((*mymesh).back()->m_mesh.faces_sbegin()), f_end((*mymesh).back()->m_mesh.faces_end());
	GeoTriMesh::ConstFaceVertexIter  fv_it;
	std::vector<unsigned int> temp;
	temp.clear();
	temp.reserve((*mymesh).back()->m_mesh.n_faces() * 3);

	for (; f_it != f_end; ++f_it)
	{
		temp.push_back((fv_it = (*mymesh).back()->m_mesh.cfv_iter(f_it)).handle().idx());
		temp.push_back((++fv_it).handle().idx());
		temp.push_back((++fv_it).handle().idx());
	}

	indices_.push_back(temp);

}

// 그려줄 메쉬 요소를 선택
void GLWidget::GLDrawMeshElements(Geometry3D *_mesh, int _mode)
{
	glPushMatrix();
	// OpenMesh Face Iterator(메쉬 전체를 돌기 위해)
	GeoTriMesh::ConstFaceIter			f_it(_mesh->m_mesh.faces_begin());
	GeoTriMesh::ConstFaceIter			f_end(_mesh->m_mesh.faces_end());
	// OpenMesh FaceVertex Iterator	(삼각형에 속한 3개의 꼭지점을 돌기 위해)
	GeoTriMesh::ConstFaceVertexIter		fv_it;

	GeoTriMesh::ConstEdgeIter			e_it(_mesh->m_mesh.edges_begin());
	GeoTriMesh::ConstEdgeIter			e_end(_mesh->m_mesh.edges_end());

	GeoTriMesh::ConstVertexIter			v_it(_mesh->m_mesh.vertices_begin());
	GeoTriMesh::ConstVertexIter			v_end(_mesh->m_mesh.vertices_end());

	GeoTriMesh::VertexHandle	vh0, vh1;
	GeoTriMesh::HalfedgeHandle	heh0;
	//std::vector<unsigned int> temp;
	switch (_mode)
	{
		// -------------------- triangle -------------------- 
	case 0:


		//glPolygonMode(GL_BLEND, GL_FILL);
		/*
		if (m_bDrawingOptions[_mesh->ModelID].m_bTexture == true && _mesh->m_mesh.has_vertex_texcoords2D())
		{
		glEnableClientState(GL_VERTEX_ARRAY);
		GL::glVertexPointer(3, GL_FLOAT, 0, _mesh->m_mesh.points());


		if (_mesh->m_mesh.has_vertex_normals())
		{
		glEnableClientState(GL_NORMAL_ARRAY);
		GL::glNormalPointer(GL_FLOAT, 0, _mesh->m_mesh.vertex_normals());
		}
		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_COLOR);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		GL::glTexCoordPointer(_mesh->m_mesh.texcoords2D());
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, listTexName[_mesh->ModelID]);
		//glDepthFunc(GL_LEQUAL);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glAlphaFunc(GL_GREATER, 0.1f);
		//glEnable(GL_BLEND);
		//glEnable(GL_DEPTH_TEST);	// Turn Depth Testing Off
		//glEnable(GL_CULL_FACE);										// Remove Back Face
		//glDisable(GL_COLOR_MATERIAL);
		}
		else
		{
		glEnableClientState(GL_VERTEX_ARRAY);
		GL::glVertexPointer(3, GL_FLOAT, 0, _mesh->m_mesh.points());


		if (_mesh->m_mesh.has_vertex_normals())
		{
		glEnableClientState(GL_NORMAL_ARRAY);
		GL::glNormalPointer(GL_FLOAT, 0, _mesh->m_mesh.vertex_normals());
		}

		if (m_bDrawingOptions[_mesh->ModelID].m_bFaceColor == true) {

		glEnableClientState(GL_COLOR_ARRAY);
		GL::glColorPointer(3, GL_UNSIGNED_BYTE, 0, _mesh->m_mesh.vertex_colors());
		}
		else {
		glColor4f(fc[0], fc[1], fc[2], 0.5f);
		}
		//	glDisable(GL_BLEND);		// Turn Blending Off
		//	glEnable(GL_DEPTH_TEST);	// Turn Depth Testing On

		}
		*/

		//	// 그리기 시작		
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(2.0f, 2.0f);

		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_LINE);      // Draw Backfacing Polygons As Wireframes ( NEW )

		// 삼각형 그리기
		/*	if (m_bDrawingOptions[_mesh->ModelID].m_bTexture == true || m_bDrawingOptions[_mesh->ModelID].m_bFaceColor == true)
		{
		glDrawElements(GL_TRIANGLES, indices_[_mesh->ModelID].size(), GL_UNSIGNED_INT, &indices_[_mesh->ModelID][0]);
		}
		else{*/

		if (m_bTexture == true || m_bDrawingOptions[_mesh->ModelID].m_bFaceColor == true)
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, listTexName[_mesh->ModelID]);
		}
		else if (m_bTexture == false)
		{
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_COLOR);
			glEnableClientState(GL_COLOR_ARRAY);
		}

		if (m_bSmooth)
		{
			//temp = indices_[i];
			//glDrawElements(GL_TRIANGLES, temp.size(), GL_UNSIGNED_INT, &temp[0]);
			//	temp.clear();


			//위 glDrawElements와 동일 역할

			glBegin(GL_TRIANGLES);
			for (f_it; f_it != f_end; ++f_it) {
				fv_it = _mesh->m_mesh.cfv_iter(f_it.handle());
				if (m_bTexture == true && _mesh->m_mesh.has_vertex_texcoords2D())
					glTexCoord2f(_mesh->m_mesh.texcoord2D(fv_it.handle())[0], _mesh->m_mesh.texcoord2D(fv_it.handle())[1]);
				glNormal3fv(&(_mesh->m_mesh.normal(fv_it)[0]));
				glVertex3fv(&_mesh->m_mesh.point(fv_it)[0]);
				//glArrayElement(fv_it.handle().idx());
				++fv_it;
				if (m_bTexture == true && _mesh->m_mesh.has_vertex_texcoords2D())
					glTexCoord2f(_mesh->m_mesh.texcoord2D(fv_it.handle())[0], _mesh->m_mesh.texcoord2D(fv_it.handle())[1]);
				glNormal3fv(&(_mesh->m_mesh.normal(fv_it)[0]));
				glVertex3fv(&_mesh->m_mesh.point(fv_it)[0]);
				//glArrayElement(fv_it.handle().idx());
				++fv_it;
				if (m_bTexture == true && _mesh->m_mesh.has_vertex_texcoords2D())
					glTexCoord2f(_mesh->m_mesh.texcoord2D(fv_it.handle())[0], _mesh->m_mesh.texcoord2D(fv_it.handle())[1]);
				glNormal3fv(&(_mesh->m_mesh.normal(fv_it)[0]));
				glVertex3fv(&_mesh->m_mesh.point(fv_it)[0]);
				//glArrayElement(fv_it.handle().idx());
			}
			glEnd();


		}
		else
		{
			glBegin(GL_TRIANGLES);
			for (f_it; f_it != f_end; ++f_it) {

				glNormal3fv(&_mesh->m_mesh.normal(f_it)[0]);

				fv_it = _mesh->m_mesh.cfv_iter(f_it.handle());
				//	glNormal3fv(&(_mesh->m_mesh.normal(fv_it)[0]));
				if (m_bTexture == true && _mesh->m_mesh.has_vertex_texcoords2D())
					glTexCoord2f(_mesh->m_mesh.texcoord2D(fv_it.handle())[0], _mesh->m_mesh.texcoord2D(fv_it.handle())[1]);
				glVertex3fv(&_mesh->m_mesh.point(fv_it)[0]);
				//glArrayElement(fv_it.handle().idx());
				++fv_it;
				//	glNormal3fv(&(_mesh->m_mesh.normal(fv_it)[0]));
				if (m_bTexture == true && _mesh->m_mesh.has_vertex_texcoords2D())
					glTexCoord2f(_mesh->m_mesh.texcoord2D(fv_it.handle())[0], _mesh->m_mesh.texcoord2D(fv_it.handle())[1]);
				glVertex3fv(&_mesh->m_mesh.point(fv_it)[0]);
				//glArrayElement(fv_it.handle().idx());
				++fv_it;
				//	glNormal3fv(&(_mesh->m_mesh.normal(fv_it)[0]));
				if (m_bTexture == true && _mesh->m_mesh.has_vertex_texcoords2D())
					glTexCoord2f(_mesh->m_mesh.texcoord2D(fv_it.handle())[0], _mesh->m_mesh.texcoord2D(fv_it.handle())[1]);
				glVertex3fv(&_mesh->m_mesh.point(fv_it)[0]);
				//glArrayElement(fv_it.handle().idx());	

			}
			glEnd();
		}
		//}

		glDisable(GL_POLYGON_OFFSET_LINE);


		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		if (m_bTexture == true)
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_COLOR);
		}
		else if (m_bDrawingOptions[_mesh->ModelID].m_bFaceColor == true)
			glDisableClientState(GL_COLOR_ARRAY);

		break;





		// -------------------- vertex -------------------- 
	case 1:
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		glPointSize(4.0f);
		/*	glEnableClientState(GL_VERTEX_ARRAY);
		GL::glVertexPointer(_mesh->m_mesh.points());

		glDisable(GL_LIGHTING);

		if (m_bVertexColor == true)
		{
		glEnableClientState(GL_COLOR_ARRAY);
		GL::glColorPointer(3, GL_UNSIGNED_BYTE, 0, _mesh->m_mesh.vertex_colors());
		}
		else
		{
		glColor3f(vc[0], vc[1], vc[2]);
		}

		glDrawArrays(GL_POINTS, 0, _mesh->m_mesh.n_vertices());
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		*/


		//	GeoTriMesh::ConstVertexIter vIt(_mesh->m_mesh.vertices_begin()), vEnd(_mesh->m_mesh.vertices_end());
		if (_mesh->m_mesh.has_vertex_normals())
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			GL::glNormalPointer(GL_FLOAT, 0, _mesh->m_mesh.vertex_normals());
		}

	glBegin(GL_POINTS);

		for (int j = 0; v_it != v_end; ++j, ++v_it) {
		
			if (m_bDrawingOptions[_mesh->ModelID].m_bVertexColor == true)
				glColor3bv((const GLbyte*)&_mesh->m_mesh.color(v_it)[0]);
			else
				glColor3f(vc[0], vc[1], vc[2]);

			glNormal3fv(&_mesh->m_mesh.normal(v_it)[0]);
			glVertex3fv(&_mesh->m_mesh.point(v_it)[0]);
			
		}
glEnd();

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glEnable(GL_CULL_FACE);
		break;

		// -------------------- edge -------------------- 
	case 2:
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		glEnableClientState(GL_VERTEX_ARRAY);
		GL::glVertexPointer(_mesh->m_mesh.points());

		glEnableClientState(GL_NORMAL_ARRAY);
		GL::glNormalPointer(_mesh->m_mesh.vertex_normals());

		glColor3f(0.0f, 0.0f, 0.0f);


		glPolygonMode(GL_FRONT, GL_LINE);

		// 그리기 시작
		glBegin(GL_TRIANGLES);
		for (; f_it != f_end; ++f_it)
		{
			//	GL::glNormal(_mesh->m_mesh.normal(f_it));
			fv_it = _mesh->m_mesh.cfv_iter(f_it.handle());
			GL::glVertex(_mesh->m_mesh.point(fv_it));
			++fv_it;
			GL::glVertex(_mesh->m_mesh.point(fv_it));
			++fv_it;
			GL::glVertex(_mesh->m_mesh.point(fv_it));
		}
		glEnd();

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);

		break;


		// -------------------- feature points -------------------- 
	case 3:
		glPushMatrix();

		glPointSize(6.0f);
		glDisable(GL_LIGHTING);
		glColor3f(0.8f, 0.2f, 0.2f);

		glBegin(GL_POINTS);
		for (; v_it != v_end; ++v_it)
		{
			if (_mesh->m_mesh.status(v_it).feature())
			{
				glVertex3f(_mesh->m_mesh.point(v_it)[0], _mesh->m_mesh.point(v_it)[1], _mesh->m_mesh.point(v_it)[2]);
			}
		}
		glEnd();

		glPointSize(1.0f);
		glEnable(GL_LIGHTING);
		glPopMatrix();


		glColor3fv(m_defaultcolor);

		break;

		// -------------------- feature edges -------------------- 
	case 4:
		glPushMatrix();

		glDisable(GL_LIGHTING);

		glLineWidth(5.0f);
		glColor3f(0.8f, 0.2f, 0.2f);

		glBegin(GL_LINES);
		for (; e_it != e_end; ++e_it)
		{
			if (_mesh->m_mesh.status(e_it).feature())
			{
				heh0 = _mesh->m_mesh.halfedge_handle(e_it, 0);
				vh0 = _mesh->m_mesh.from_vertex_handle(heh0);
				vh1 = _mesh->m_mesh.to_vertex_handle(heh0);
				glVertex3f(_mesh->m_mesh.point(vh0)[0], _mesh->m_mesh.point(vh0)[1], _mesh->m_mesh.point(vh0)[2]);
				glVertex3f(_mesh->m_mesh.point(vh1)[0], _mesh->m_mesh.point(vh1)[1], _mesh->m_mesh.point(vh1)[2]);
			}
		}

		glEnd();

		glLineWidth(1.0f);

		glEnable(GL_LIGHTING);
		glPopMatrix();

		glColor3fv(m_defaultcolor);

		break;


		// -------------------- bounding box -------------------- 
	case 7:
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

		// 바운딩박스 그리기
		glBegin(GL_LINES);
		glColor3f(0.f, 0.f, 0.f);
		glVertex3f(_mesh->m_bbMin[0], _mesh->m_bbMin[1], _mesh->m_bbMin[2]);
		glVertex3f(_mesh->m_bbMin[0], _mesh->m_bbMin[1], _mesh->m_bbMax[2]);

		glVertex3f(_mesh->m_bbMin[0], _mesh->m_bbMin[1], _mesh->m_bbMin[2]);
		glVertex3f(_mesh->m_bbMax[0], _mesh->m_bbMin[1], _mesh->m_bbMin[2]);

		glVertex3f(_mesh->m_bbMin[0], _mesh->m_bbMin[1], _mesh->m_bbMin[2]);
		glVertex3f(_mesh->m_bbMin[0], _mesh->m_bbMax[1], _mesh->m_bbMin[2]);

		glVertex3f(_mesh->m_bbMin[0], _mesh->m_bbMax[1], _mesh->m_bbMin[2]);
		glVertex3f(_mesh->m_bbMin[0], _mesh->m_bbMax[1], _mesh->m_bbMax[2]);

		glVertex3f(_mesh->m_bbMin[0], _mesh->m_bbMax[1], _mesh->m_bbMin[2]);
		glVertex3f(_mesh->m_bbMax[0], _mesh->m_bbMax[1], _mesh->m_bbMin[2]);

		glVertex3f(_mesh->m_bbMax[0], _mesh->m_bbMax[1], _mesh->m_bbMin[2]);
		glVertex3f(_mesh->m_bbMax[0], _mesh->m_bbMin[1], _mesh->m_bbMin[2]);

		glVertex3f(_mesh->m_bbMax[0], _mesh->m_bbMax[1], _mesh->m_bbMin[2]);
		glVertex3f(_mesh->m_bbMax[0], _mesh->m_bbMax[1], _mesh->m_bbMax[2]);

		glVertex3f(_mesh->m_bbMax[0], _mesh->m_bbMax[1], _mesh->m_bbMax[2]);
		glVertex3f(_mesh->m_bbMax[0], _mesh->m_bbMin[1], _mesh->m_bbMax[2]);

		glVertex3f(_mesh->m_bbMax[0], _mesh->m_bbMax[1], _mesh->m_bbMax[2]);
		glVertex3f(_mesh->m_bbMin[0], _mesh->m_bbMax[1], _mesh->m_bbMax[2]);

		glVertex3f(_mesh->m_bbMin[0], _mesh->m_bbMax[1], _mesh->m_bbMax[2]);
		glVertex3f(_mesh->m_bbMin[0], _mesh->m_bbMin[1], _mesh->m_bbMax[2]);

		glVertex3f(_mesh->m_bbMin[0], _mesh->m_bbMin[1], _mesh->m_bbMax[2]);
		glVertex3f(_mesh->m_bbMax[0], _mesh->m_bbMin[1], _mesh->m_bbMax[2]);

		glVertex3f(_mesh->m_bbMax[0], _mesh->m_bbMin[1], _mesh->m_bbMax[2]);
		glVertex3f(_mesh->m_bbMax[0], _mesh->m_bbMin[1], _mesh->m_bbMin[2]);

		glEnd();

		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

		break;

		// -------------------- vertex normal -------------------- 
	case 8:
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);


		glBegin(GL_LINES);
		glColor3f(0.f, 0.f, 1.f);
		for (; v_it != v_end; ++v_it)
		{
			float p[3], n[3];
			p[0] = _mesh->m_mesh.point(v_it)[0];	 p[1] = _mesh->m_mesh.point(v_it)[1]; p[2] = _mesh->m_mesh.point(v_it)[2];
			n[0] = _mesh->m_mesh.normal(v_it)[0]; n[1] = _mesh->m_mesh.normal(v_it)[1]; n[2] = _mesh->m_mesh.normal(v_it)[2];
			glVertex3f(p[0], p[1], p[2]);
			glVertex3f(p[0] + _mesh->avgEdgeLength * 2 * n[0], p[1] + _mesh->avgEdgeLength * 2 * n[1], p[2] + _mesh->avgEdgeLength * 2 * n[2]);	//평균 에지길의 2배 정도 길이로 노말 벡터 그리기
		}
		glEnd();

	
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);

		break;

		// -------------------- boundary edges -------------------- 
	case 10:

		glDisable(GL_LIGHTING);

		glLineWidth(5.0f);
		glColor3f(0.8f, 0.2f, 0.2f);

		glBegin(GL_LINES);
		for (; e_it != e_end; ++e_it)
		{
			if (_mesh->m_mesh.is_boundary(e_it) == true)
			{
				heh0 = _mesh->m_mesh.halfedge_handle(e_it, 0);
				vh0 = _mesh->m_mesh.from_vertex_handle(heh0);
				vh1 = _mesh->m_mesh.to_vertex_handle(heh0);
				GL::glVertex(_mesh->m_mesh.point(vh0));
				GL::glVertex(_mesh->m_mesh.point(vh1));
			}
		}

		glEnd();

		glLineWidth(1.0f);

		glEnable(GL_LIGHTING);

		glColor3f(0.6f, 0.8f, 0.9f);

		break;
	}
	glPopMatrix();

}





// vertex index에 따라 vertex color를 세팅하여 그림
void GLWidget::GLDrawSelectMeshVertex(Geometry3D* _mesh)
{

	GeoTriMesh::VertexIter v_it(_mesh->m_mesh.vertices_begin()), v_End(_mesh->m_mesh.vertices_end());
	int nIndex;

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
	glPointSize(1);

	glPushMatrix();
	/*

	glEnableClientState(GL_VERTEX_ARRAY);
	GL::glVertexPointer(_mesh->m_mesh.points());

	glEnableClientState(GL_NORMAL_ARRAY);
	GL::glNormalPointer(_mesh->m_mesh.vertex_normals());
	*/

	//glEnable(GL_DEPTH_TEST);	// Turn Depth Testing On
	//glColor3ub(255, 255, 255);

/*

	// 그리기 시작		
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// 삼각형 그리기
	std::vector<unsigned int> temp = indices_[ext_current_model_index];
	int a = temp.size();
	//qDebug() << a;
	if (a != 0)
	{
		glDrawElements(GL_TRIANGLES, a, GL_UNSIGNED_INT, &temp[0]);
	}
	glDisable(GL_POLYGON_OFFSET_FILL);
*/


	/*
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);*/

	glBegin(GL_POINTS);
	for (; v_it != v_End; ++v_it)
	{
		nIndex = v_it.handle().idx();
		glColor3ub(nIndex % 256, (nIndex >> 8) % 256, (nIndex >> 16) % 256);
		GL::glVertex(_mesh->m_mesh.point(v_it));
	}

	glEnd();
	glPopMatrix();



	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
}

// Color ID에 따라 주어진 영역에서 vertex를 선택
void GLWidget::GLSelectMeshVertexByColorNumber(Geometry3D* _mesh, GLint nStartX, GLint nStartY, GLint nWidth, GLint nHeight, UINT nFlags)
{

	GLDrawSelectMeshVertex(_mesh);


	if (nWidth < 1) nWidth = 1;
	if (nHeight < 1) nHeight = 1;

	unsigned char *pRGB = new unsigned char[3 * (nWidth + 1) * (nHeight + 1) + 3];

	glReadBuffer(GL_BACK);
	glReadPixels(nStartX, nStartY, nWidth, nHeight, GL_RGB, GL_UNSIGNED_BYTE, pRGB);

	//int nLast = 0;
	int nEnd = nWidth * nHeight;

	for (int i = 0; i < nEnd; i++)
	{
		int index = i * 3;
		int ColorNumber = (pRGB[index]) + (pRGB[index + 1] << 8) + (pRGB[index + 2] << 16);

		if (ColorNumber >= 0 && ColorNumber < (int)_mesh->m_mesh.n_vertices())
		{
			if (!m_nSelectionMode.bDeformSelection && !m_nSelectionMode.bDeformMove)
				_mesh->VerPickList(ColorNumber, nFlags);

			if (m_nSelectionMode.bDeformSelection)
			{
				if (nFlags != 2)
					_mesh->pdmesh_->getSVSet().insert(GeoTriMesh::VertexHandle(ColorNumber));
				else
				{
					std::set<VertexHandle>::iterator it = std::find(_mesh->pdmesh_->getSVSet().begin(), _mesh->pdmesh_->getSVSet().end(), VertexHandle(ColorNumber));
					if (it != _mesh->pdmesh_->getSVSet().end())
						_mesh->pdmesh_->getSVSet().erase(it);
				}


				//	(*mymesh)[ext_current_model_index].selectedHandleIDs.push_back(VertexHandle(ColorNumber));

			}
			else if (m_nSelectionMode.bDeformMove)
			{
				if (std::find(_mesh->pdmesh_->getSVSet().begin(), _mesh->pdmesh_->getSVSet().end(), VertexHandle(ColorNumber)) != _mesh->pdmesh_->getSVSet().end())
				{
					(*mymesh)[ext_current_model_index]->selectedHandleIDs.clear();
					(*mymesh)[ext_current_model_index]->selectedHandleIDs.push_back(VertexHandle(ColorNumber));
				}
			}

			/*	if(nFlags == MK_LBUTTON)
			{
			_mesh->VerPickList(ColorNumber, 1);
			}
			else if(nFlags == (MK_LBUTTON | MK_CONTROL))
			{
			_mesh->VerPickList(ColorNumber, 2);
			}
			*/
		}
	}
	delete[] pRGB;

}

//  주어진 영역에서 face를 선택
void GLWidget::GLSelectMeshFace(GLint nStartX, GLint nStartY, GLint nWidth, GLint nHeight, UINT nFlags)
{
	for (int i = 0; i < mymesh->size(); i++)
	{
		logviewdockwidget->addText(QString("\n\n<Face Selection Mode>\n\n"));

		GeoTriMesh::FaceIter f_it;
		GeoTriMesh::Point temp;

		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewport);

		GLdouble winX, winY, winZ;//2D point
		GLdouble posX, posY, posZ;//3D point

		for (f_it = (*mymesh)[i]->m_mesh.faces_begin(); f_it != (*mymesh)[i]->m_mesh.faces_end(); ++f_it)
		{
			(*mymesh)[i]->m_mesh.calc_face_centroid(f_it.handle(), temp);
			posX = temp[0];	posY = temp[1];	posZ = temp[2];
			gluProject(posX, posY, posZ, modelview, projection, viewport, &winX, &winY, &winZ);
			if (((winX >= nStartX) && ((winY) >= nStartY)) && ((winX <= nStartX + nWidth) && (winY) <= nStartY + nHeight))
			{

				(*mymesh)[i]->TriPickList(f_it.handle().idx(), nFlags);
			}
		}
	}
}

void GLWidget::GLDrawSelectedMeshVertex(Geometry3D *_mesh)
{

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	GeoTriMesh::VertexHandle v_handle;

	vector <int>::iterator it;

	glPointSize(5.0f);

	//glPolygonOffset(5.0f, 5.0f);


	glColor3f(0.0f, 0.0f, 1.0f);


	glBegin(GL_POINTS);
	for (it = _mesh->m_verPickList.begin(); it != _mesh->m_verPickList.end(); ++it)
	{
		int v_index = *it;
		GeoTriMesh::VertexHandle v_handle(v_index);
		GL::glVertex(_mesh->m_mesh.point(v_handle));
	}
	glEnd();




	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
}

void GLWidget::GLDrawSelectedMeshFace(Geometry3D *_mesh)
{
	if (_mesh->m_triPickList.size() != 0)
	{

		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonOffset(-1.0f, -1.0f);

		vector<int>::iterator it;
		glPushMatrix();

		//		

		//	glEnableClientState(GL_VERTEX_ARRAY);
		//	GL::glVertexPointer(3, GL_FLOAT, 0, (*mymesh)[i].m_mesh.points());
		//glEnableClientState(GL_NORMAL_ARRAY);
		//GL::glNormalPointer(GL_FLOAT, 0, (*mymesh)[i].m_mesh.vertex_normals());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 0.2f, 0.2f, 0.7f);

		glDisable(GL_LIGHTING);
		glBegin(GL_TRIANGLES);
		for (it = _mesh->m_triPickList.begin(); it != _mesh->m_triPickList.end(); ++it)
		{
			GeoTriMesh::FaceHandle f_handle(*it);
			GeoTriMesh::FaceVertexIter	fv_it;
			fv_it = _mesh->m_mesh.fv_iter(f_handle);
			//glArrayElement(fv_it.handle().idx());
			glVertex3f(_mesh->m_mesh.point(fv_it.handle())[0], _mesh->m_mesh.point(fv_it.handle())[1], _mesh->m_mesh.point(fv_it.handle())[2]);
			++fv_it;
			//glArrayElement(fv_it.handle().idx());
			glVertex3f(_mesh->m_mesh.point(fv_it.handle())[0], _mesh->m_mesh.point(fv_it.handle())[1], _mesh->m_mesh.point(fv_it.handle())[2]);
			++fv_it;
			//glArrayElement(fv_it.handle().idx());
			glVertex3f(_mesh->m_mesh.point(fv_it.handle())[0], _mesh->m_mesh.point(fv_it.handle())[1], _mesh->m_mesh.point(fv_it.handle())[2]);
		}
		glEnd();
		//--------------------------------------------------------------------------------------//
		//	glDisableClientState(GL_VERTEX_ARRAY);
		//glDisableClientState(GL_NORMAL_ARRAY);
		glDisable(GL_BLEND);

		glDisable(GL_POLYGON_OFFSET_FILL);
		glPopMatrix();

	}
}





// 모델을 윈도우 크기에 맞춤
void GLWidget::GLZoomToFit(myVec::Vec3f *m_bbMin, myVec::Vec3f *m_bbMax)
{
	makeCurrent();

	double modelscale;

	modelscale = (double)((*m_bbMax - *m_bbMin).length());
	modelscale /= 2.0f;
	double m_ratio = (m_vViewport[2] > m_vViewport[3]) ? (double)m_vViewport[2] / m_vViewport[3] : (double)m_vViewport[3] / m_vViewport[2];



	//	GLfloat projection[16];
	//	glGetFloatv(GL_PROJECTION_MATRIX, projection);

	float x, y, z, x_, y_, z_, s_;

	//x= modelcenter.value[0];		y= modelcenter.value[1];		z= modelcenter.value[2];

	x = (*mymesh)[ext_current_model_index]->m_modelcenter[0];	y = (*mymesh)[ext_current_model_index]->m_modelcenter[1];	z = (*mymesh)[ext_current_model_index]->m_modelcenter[2];
	//x_=projection[0]*m_mxTransform[0]*x + projection[1]*m_mxTransform[4]*y +projection[2]*m_mxTransform[8]*z +projection[3]*m_mxTransform[12];	
	//y_=projection[4]*m_mxTransform[1]*x + projection[5]*m_mxTransform[5]*y +projection[6]*m_mxTransform[9]*z +projection[7]*m_mxTransform[13];
	//z_=projection[8]*m_mxTransform[2]*x + projection[9]*m_mxTransform[6]*y +projection[10]*m_mxTransform[10]*z +projection[11]*m_mxTransform[14];
	//s_=projection[12]*m_mxTransform[3]*x + projection[13]*m_mxTransform[7]*y +projection[14]*m_mxTransform[11]*z +projection[15]*m_mxTransform[15];

	x_ = m_mxTransform[0] * x + m_mxTransform[4] * y + m_mxTransform[8] * z + m_mxTransform[12];
	y_ = m_mxTransform[1] * x + m_mxTransform[5] * y + m_mxTransform[9] * z + m_mxTransform[13];
	z_ = m_mxTransform[2] * x + m_mxTransform[6] * y + m_mxTransform[10] * z + m_mxTransform[14];
	s_ = m_mxTransform[3] * x + m_mxTransform[7] * y + m_mxTransform[11] * z + m_mxTransform[15];



	//m_ZoomScale = sqrt(x_*x_ + y_*y_ + z_*z_)+modelscale;

	m_ZoomScale = fabs(x_ / s_) + modelscale;




	resizeGL(m_vViewport[2], m_vViewport[3]);
	//updateGL();
	doneCurrent();
}

// 마우스 좌표를 3차원 공간으로 변환(Trackball 용)
void GLWidget::GLPtTo3DSphere(myVec::Vec2f point, GLfloat* pVec)
{
	float dis, a;

	/* project x,y onto a hemi-sphere centered within width, height */
	pVec[0] = (2.0f*point[0] - m_vViewport[2]) / m_vViewport[2];
	pVec[1] = (m_vViewport[3] - 2.0f*point[1]) / m_vViewport[3];
	dis = (float)sqrt(pVec[0] * pVec[0] + pVec[1] * pVec[1]);
	pVec[2] = (float)cos((3.14159265f / 2.0f) * ((dis < 1.0f) ? dis : 1.0f));
	a = (float)sqrt(pVec[0] * pVec[0] + pVec[1] * pVec[1] + pVec[2] * pVec[2]);
	pVec[0] /= a;
	pVec[1] /= a;
	pVec[2] /= a;
}

myVec::Vec2f GLWidget::GLScreenToPoint(int x, int y)
{
	myVec::Vec2f _position;

	//GLfloat _x = (2.*x - m_vViewport[2]) / m_vViewport[2];
	//GLfloat _y = (m_vViewport[3] - 2.*y) / m_vViewport[3];

	GLfloat _x = (2.*x - m_vViewport[2]) / m_vViewport[2];
	GLfloat _y = (m_vViewport[3] - 2.*y) / m_vViewport[3];

	_position[0] = m_PanOffsetX + _x*m_ZoomScale;
	_position[1] = m_PanOffsetY + _y*m_ZoomScale*m_ratio;

	//GLint viewport[4];
	//GLdouble mvmatrix[16],projmatrix[16];
	//GLint realy;
	//GLdouble wx, wy, wz;
	//::wglmakeCurrent(m_hDC,m_hRC);
	//glPushMatrix();

	//switch(mode)
	//{
	//case 1:			// XY View
	//	break;

	//case 2:			// YZ View
	//	glRotatef(180, 0, 1, 0);
	//	break;

	//case 3:			// ZX View
	//	break;
	//}

	//glGetIntegerv(GL_VIEWPORT,viewport);
	//glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	//glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
	//realy=viewport[3]-(GLint)y-1;
	//gluUnProject((GLdouble)x, (GLdouble)realy, 0.5, mvmatrix, projmatrix, viewport,
	//			&wx, &wy, &wz);
	//_position[0] = wx;
	//_position[1] = wy;
	//glPopMatrix();
	//::wglmakeCurrent(NULL,NULL);

	return _position;
}

// 좌표축 그려주는 함수
void GLWidget::GLDrawAxis(void)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	float axis_length;

	axis_length = fabs(m_Right - m_Left)*0.1;

	// 축 그리기
	glBegin(GL_LINES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(axis_length, 0.f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, axis_length, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, axis_length);


	// X mark
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(0.9f * axis_length, 0.1f * axis_length, 0.0f);
	glVertex3f(1.1f * axis_length, -0.1f * axis_length, 0.0f);
	glVertex3f(1.1f * axis_length, 0.1f * axis_length, 0.0f);
	glVertex3f(0.9f * axis_length, -0.1f * axis_length, 0.0f);
	// Y mark
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(-0.1f * axis_length, 1.1f * axis_length, 0.0f);
	glVertex3f(0.0f, axis_length, 0.0f);
	glVertex3f(0.1f * axis_length, 1.1f * axis_length, 0.0f);
	glVertex3f(0.0f, axis_length, 0.0f);
	// Z mark
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(-0.1f * axis_length, 0.0f, 1.1f * axis_length);
	glVertex3f(0.1f * axis_length, 0.0f, 1.1f * axis_length);
	glVertex3f(0.1f * axis_length, 0.0f, 1.1f * axis_length);
	glVertex3f(-0.1f * axis_length, 0.0f, axis_length);
	glVertex3f(-0.1f * axis_length, 0.0f, axis_length);
	glVertex3f(0.1f * axis_length, 0.0f, axis_length);




	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}


//워크스페이스 그리기
void GLWidget::DrawWorkspace()
{
	glDisable(GL_TEXTURE_2D);

	int gridNum = 20;
	float gridSpace = 0.1f;

	float gridColor[4];
	gridColor[0] = 0.6039f;
	gridColor[1] = 0.6039f;
	gridColor[2] = 0.6039f;
	gridColor[3] = 1.0f;

	int halfNum = gridNum / 2;
	float width = gridSpace * halfNum;

	GLboolean oldLight = ::glIsEnabled(GL_LIGHTING);
	::glDisable(GL_LIGHTING);

	::glColor3fv(gridColor);
	::glBegin(GL_LINES);
	for (int i = -halfNum; i <= halfNum; i++)
	{
		::glVertex3f(-width, i * gridSpace, 0.0f);
		::glVertex3f(width, i * gridSpace, 0.0f);
		::glVertex3f(i * gridSpace, -width, 0.0f);
		::glVertex3f(i * gridSpace, width, 0.0f);
	}
	::glEnd();

	if (oldLight)
		::glEnable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
}


void GLWidget::DrawLight(GLfloat poistion[], GLfloat direction[], GLfloat color[])
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();

	GLfloat x_vec[] = { 1.f, 0.f, 0.f };
	GLfloat XY_projection_direction[3];
	XY_projection_direction[0] = direction[0];
	XY_projection_direction[1] = direction[1];
	XY_projection_direction[2] = 0.f;

	GLfloat dot_x = x_vec[0] * XY_projection_direction[0] + x_vec[1] * XY_projection_direction[1] + x_vec[2] * XY_projection_direction[2];
	GLfloat angle_z = acos(dot_x);

	GLfloat y_vec[] = { 0.f, 1.f, 0.f };
	GLfloat YZ_projection_direction[3];
	YZ_projection_direction[0] = 0.f;
	YZ_projection_direction[1] = direction[1];
	YZ_projection_direction[2] = direction[2];

	GLfloat dot_y = y_vec[0] * YZ_projection_direction[0] + y_vec[1] * YZ_projection_direction[1] + y_vec[2] * YZ_projection_direction[2];
	GLfloat angle_x = acos(dot_y);

	GLfloat z_vec[] = { 0.f, 0.f, 1.f };
	GLfloat ZX_projection_direction[3];
	ZX_projection_direction[0] = direction[0];
	ZX_projection_direction[1] = 0.f;
	ZX_projection_direction[2] = direction[2];

	GLfloat dot_z = z_vec[0] * ZX_projection_direction[0] + z_vec[1] * ZX_projection_direction[1] + z_vec[2] * ZX_projection_direction[2];
	GLfloat angle_y = acos(dot_z);

	glMultMatrixf(m_LightTransform);
	//	glTranslatef(poistion[0],poistion[1],poistion[2]);
	glRotatef(m_LightTrackingAngle, m_vAxis[0], m_vAxis[1], m_vAxis[2]);

	//glRotatef(-angle_y*180/3.14,0,1,0);
	//glRotatef(-angle_z*180/3.14,0,0,1);


	//	glRotatef(m_LightTrackingAngle, m_vAxis[0], m_vAxis[1], m_vAxis[2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)m_LightTransform);
	glMultMatrixf(m_LightTransform);

	glBegin(GL_LINES);
	glColor3f(color[0], color[1], color[2]);
	glutSolidCone(2, 7, 20, 20);
	//	glVertex3f(0,0,0);
	//glVertex3f(0, 0, 1);

	glEnd();
	glPopMatrix();
	m_LightTrackingAngle = 0.f; //중요!

	glLoadIdentity();
	glMultMatrixf((GLfloat *)m_mxTransform);
}
// ---------------------------------------------------------------------
// Set the manipulation mode(rotation, zoom, pan, etc)
void GLWidget::GLOnSetManiMode(int mode)
{
	switch (mode)
	{
	case 1:			// track ball mode
		m_nManiMode = Trackball;
		break;
	case 2:			// Zooming mode
		m_nManiMode = Zooming;
		break;
	case 3:			// Box Zoom mode
		m_nManiMode = Box_Zooming;
		break;
	case 4:			// Panning mode
		m_nManiMode = Panning;
		break;
	}
}


// Texture On/Off
/*
void GLWidget::GLSetTexture()
{
//this->m_RenderScene.m_bMeshTexture = !this->m_RenderScene.m_bMeshTexture;
}
*/
// OpenGL Texture 초기화
//void GLWidget::GLInitTexture()
//{
// 	this->makeCurrent();
// 	this->m_RenderScene.GLInitTexture(this->m_image);
// 	this->MakeNULL();

//}


// ---------------- Viewing Direction-------------------------------------
// Isometric View
void GLWidget::GLOnViewIso()
{
	makeCurrent();

	m_mxTransform[0] = 0.544677f; m_mxTransform[1] = -0.362249f; 	m_mxTransform[2] = 0.756375f; m_mxTransform[3] = 0.f;
	m_mxTransform[4] = 0.838644f; m_mxTransform[5] = 0.233635f; 	m_mxTransform[6] = -0.492026f; m_mxTransform[7] = 0.f;
	m_mxTransform[8] = 0.001520f; m_mxTransform[9] = 0.902324f; 	m_mxTransform[10] = 0.431054f; m_mxTransform[11] = 0.f;
	m_mxTransform[12] = 0.f; m_mxTransform[13] = 0.f;	m_mxTransform[14] = 0.f; m_mxTransform[15] = 1.f;


	m_TrackingAngle = 0;

	resizeGL(m_vViewport[2], m_vViewport[3]);

	autoBufferSwap();
	doneCurrent();
}

void GLWidget::GLOnViewTop()
{
	makeCurrent();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)m_mxTransform);
	m_TrackingAngle = 0;
	resizeGL(m_vViewport[2], m_vViewport[3]);

	autoBufferSwap();
	doneCurrent();
}

void GLWidget::GLOnViewBottom()
{
	makeCurrent();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(180, 0, 1, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)m_mxTransform);
	m_TrackingAngle = 0;
	resizeGL(m_vViewport[2], m_vViewport[3]);
	autoBufferSwap();
	doneCurrent();
}

void GLWidget::GLOnViewLeft()
{
	makeCurrent();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(-90, 1, 0, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)m_mxTransform);
	m_TrackingAngle = 0;
	resizeGL(m_vViewport[2], m_vViewport[3]);
	autoBufferSwap();
	doneCurrent();
}

void GLWidget::GLOnViewRight()
{
	makeCurrent();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(-90, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)m_mxTransform);
	m_TrackingAngle = 0;
	resizeGL(m_vViewport[2], m_vViewport[3]);

	autoBufferSwap();
	doneCurrent();
}

void GLWidget::GLOnViewFront()
{
	makeCurrent();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(-90, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)m_mxTransform);
	m_TrackingAngle = 0;
	resizeGL(m_vViewport[2], m_vViewport[3]);

	autoBufferSwap();
	doneCurrent();
}

void GLWidget::GLOnViewBack()
{
	makeCurrent();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(-90, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)m_mxTransform);
	m_TrackingAngle = 0;
	resizeGL(m_vViewport[2], m_vViewport[3]);

	autoBufferSwap();
	doneCurrent();
}

void GLWidget::GLOnViewCenter()
{
	if (m_bModelLoaded)
	{
		makeCurrent();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf((GLfloat *)m_mxTransform);
		if (m_bCenter)
		{

			glTranslatef(-(*mymesh)[ext_current_model_index]->m_modelcenter[0], -(*mymesh)[ext_current_model_index]->m_modelcenter[1], -(*mymesh)[ext_current_model_index]->m_modelcenter[2]);

		}
		else
			glTranslatef((*mymesh)[ext_current_model_index]->m_modelcenter[0], (*mymesh)[ext_current_model_index]->m_modelcenter[1], (*mymesh)[ext_current_model_index]->m_modelcenter[2]);

		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)m_mxTransform);

		resizeGL(m_vViewport[2], m_vViewport[3]);

		autoBufferSwap();
		doneCurrent();

	}
}

// 현재 시점을 저장해 둔다
void GLWidget::GLStoreUserView()
{
	for (int i = 0; i < 4; i++)
	{
		m_currentViewport[i] = m_vViewport[i];
		for (int j = 0; j < 4; j++)
			m_currentTransform[i * 4 + j] = m_mxTransform[i * 4 + j];
	}

	m_currentZoomScale = m_ZoomScale;
	m_currentPanOffsetX = m_PanOffsetX;
	m_currentPanOffsetY = m_PanOffsetY;

}

// 저장된 시점을 불러들인다.
void GLWidget::GLLoadUserView()
{
	for (int i = 0; i < 4; i++)
	{
		m_vViewport[i] = m_currentViewport[i];
		for (int j = 0; j < 4; j++)
			m_mxTransform[i * 4 + j] = m_currentTransform[i * 4 + j];
	}

	m_ZoomScale = m_currentZoomScale;
	m_PanOffsetX = m_currentPanOffsetX;
	m_PanOffsetY = m_currentPanOffsetY;

	makeCurrent();
	resizeGL(m_vViewport[2], m_vViewport[3]);
	paintGL();
	autoBufferSwap();
	doneCurrent();
}

unsigned int ProcessHits(const unsigned int hitCount, const GLuint *buffer)
{
	unsigned int i;
	GLuint names, *ptr, minZ, *ptrNames, numberOfNames;

	ptr = (GLuint *)buffer;
	minZ = 0xffffffff;
	for (i = 0; i < hitCount; i++) {
		names = *ptr;
		ptr++;
		if (*ptr < minZ) {
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr + 2;
		}

		ptr += names + 2;
	}

	return (unsigned int)*ptrNames;
}

#define SELECTION_BUFFER_LENGTH 512

// Picking 처리, GLPickObjects(GLint x, GLint y)에서 호출하고 선택된 namestack 값을 리턴
int GLWidget::GLProcessSelect(GLint hits, GLuint index[SELECTION_BUFFER_LENGTH])
{
	int choose, depth;

	if (hits > 0)											// If There Were More Than 0 Hits
	{
		choose = index[3];									// Make Our Selection The First Object
		depth = index[1];									// Store How Far Away It Is

		for (int loop = 1; loop < hits; loop++)				// Loop Through All The Detected Hits
		{
			// If This Object Is Closer To Us Than The One We Have Selected
			if (index[loop * 4 + 1] < GLuint(depth) && index[loop * 4 + 3] != UINT_MAX)
			{
				choose = index[loop * 4 + 3];					// Select The Closer Object
				depth = index[loop * 4 + 1];					// Store How Far Away It Is
			}
		}
	}

	if (choose >= 0)
	{
		//(*mymesh)[0].VerPickList(0,choose,1);
		return choose;
	}
	else
		return -1;
}

int pickprocess(GLint hits, GLuint *buffer)
{
	if (hits > 0) {
		int k = 0;
		unsigned int depth = UINT_MAX;
		int cellId = -1;
		for (int i = 0; i < hits; i++) {
			int stackSize = buffer[k++];

			//To correct the depthMin value;
			int depthMin = (buffer[k++] >> 8) & 0x00FFFFFF;
			if (depthMin < depth) {
				depth = depthMin;
				k++;
				cellId = buffer[k++];
				k += stackSize - 1;
			}
			else {
				k += 1 + stackSize;
			}
		}
		return cellId;
	}

}
void GLWidget::GLPickObjects(GLint x, GLint y)
{
	GLuint selectBuff[512];
	int i;
	for (i = 0; i < 512; i++)
	{
		selectBuff[i] = UINT_MAX;
	}
	GLint hits;

	//wglMakeCurrent(m_hDC, m_hRC);
	makeCurrent();

	//GLdouble projection[16];
	//glGetDoublev(GL_PROJECTION_MATRIX, projection);

	m_Left = -m_ZoomScale + m_PanOffsetX;
	m_Right = m_ZoomScale + m_PanOffsetX;
	m_Bottom = -m_ZoomScale*m_ratio + m_PanOffsetY;
	m_Top = m_ZoomScale*m_ratio + m_PanOffsetY;

	glSelectBuffer(512, selectBuff);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glRenderMode(GL_SELECT);
	glLoadIdentity();
	//if(m_nSelectionMode == MeshVerPick)
	gluPickMatrix((GLdouble)x, (GLdouble)(m_vViewport[3] - y), 10, 10, m_vViewport);
	//else if(m_nSelectionM     ode == MeshTriPick)
	//gluPickMatrix((GLdouble) x, (GLdouble) (m_vViewport[3]-y), 1, 1, m_vViewport); 
	//else if(m_nSelectionMode == MeshEdgePick)
	//gluPickMatrix((GLdouble) x, (GLdouble) (m_vViewport[3]-y), 5, 5, m_vViewport);


	glOrtho(m_Left, m_Right, m_Bottom, m_Top, m_Near, m_Far);
	glTranslatef(0.f, 0.f, -50000.f);



	//glMultMatrixd(projection);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	paintGL();
	//GLDrawMeshElements(1);

	hits = glRenderMode(GL_RENDER);
	int selectedVerIdx = -1;
	int sel = -1;
	if (hits > 0)
	{
		selectedVerIdx = GLProcessSelect(hits, selectBuff);	// Picking 처리
		//selectedVerIdx = pickprocess(hits, selectBuff);
		//	selectedVerIdx = ProcessHits(hits, selectBuff);
		/*	if (selectedVerIdx != -1 && selectedVerIdx < (*mymesh)[ext_current_model_index].m_mesh.n_vertices() && m_nSelectionMode.bDeformSelection)
		{
		//(*mymesh)[0].pdmesh_->getSVSet().insert(GeoTriMesh::VertexHandle(selectedVerIdx));
		//(*mymesh)[0].m_defromCtrList.push_back(selectedVerIdx);

		//for (int i = 0; i < mymesh->size(); i++)
		(*mymesh)[ext_current_model_index].pdmesh_->getSVSet().insert(GeoTriMesh::VertexHandle(selectedVerIdx));
		pairs[ext_current_model_index].push_back(GeoTriMesh::VertexHandle(selectedVerIdx));
		}
		else*/ if (selectedVerIdx != -1 && selectedVerIdx < (*mymesh)[ext_current_model_index]->m_mesh.n_vertices() && m_nSelectionMode.bDeformMove)
		{
			/*
			(*mymesh)[ext_current_model_index].selectedHandles.clear();
			std::vector<GeoTriMesh::VertexHandle> handleIDs;
			handleIDs = (*mymesh)[ext_current_model_index].pdmesh_->getHandleIDs();

			for (int a = 0; a < handleIDs.size(); a++)
			{
			if (handleIDs[a].idx() == selectedVerIdx)
			{
			index_current_deform = selectedVerIdx;
			(*mymesh)[ext_current_model_index].selectedHandles.push_back(a);
			//break;
			}
			}
			*/
			index_current_deform = selectedVerIdx;
			/*
			std::vector<OpenMesh::VertexHandle>::iterator iter = std::find((*mymesh)[ext_current_model_index].selectedHandleIDs.begin(), (*mymesh)[ext_current_model_index].selectedHandleIDs.end(), OpenMesh::VertexHandle(selectedVerIdx));
			if (iter == (*mymesh)[ext_current_model_index].selectedHandleIDs.end())
			{
			(*mymesh)[ext_current_model_index].selectedHandleIDs.push_back(VertexHandle(selectedVerIdx));
			}*/
			(*mymesh)[ext_current_model_index]->selectedHandleIDs.clear();
			(*mymesh)[ext_current_model_index]->selectedHandleIDs.push_back(VertexHandle(selectedVerIdx));


		}
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();

	//	SwapBuffers(m_hDC);
	//wglMakeCurrent(NULL, NULL);
	//autoBufferSwap();
	doneCurrent();
}



// ---------------- 이벤트 핸들-------------------------------------

// 이 메소드는 무엇인가를 그리때 발생하는 메소드이다. 이 안에 그리는 이벤트나 함수를 추가하면된다.
void GLWidget::paintEvent(QPaintEvent *event) {
	makeCurrent();
	paintGL();
	//setAutoBufferSwap(true);
	swapBuffers();
	doneCurrent();
}


void GLWidget::wheelEvent(QWheelEvent *event){

	if (event->delta() < 0)
		m_ZoomScale *= 1.2f;
	else
		m_ZoomScale *= 0.8f;

	makeCurrent();
	resizeGL(m_vViewport[2], m_vViewport[3]);
	updateGL();
	doneCurrent();
}



void GLWidget::mousePressEvent(QMouseEvent *event)
{
	myVec::Vec2f pos;
	pos[0] = event->x();
	pos[1] = event->y();

	m_ptPrev = pos;
	GLPtTo3DSphere(pos, m_vPrevVec);



	if (event->button() & Qt::LeftButton)
	{
		// Selection 처리
		if ((m_nSelectionMode.bTriSelection) || (m_nSelectionMode.bVerSelection) || m_nSelectionMode.bDeformSelection || m_nSelectionMode.bDeformMove)
		{
			GLfloat _x = (2.*pos[0] - m_vViewport[2]) / m_vViewport[2];
			GLfloat _y = (m_vViewport[3] - 2.*pos[1]) / m_vViewport[3];
			ptReal1[0] = m_PanOffsetX + _x*m_ZoomScale;
			ptReal1[1] = m_PanOffsetY + _y*m_ZoomScale*m_ratio;
		}
		/*

		//Deform Picking처리
		if (m_nSelectionMode.bDeformMove && m_bModelLoaded)
		{
		GLPickObjects(event->x(), event->y());
		}*/
	}
}



void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	myVec::Vec2f pos;
	pos[0] = event->x();
	pos[1] = event->y();


	if (event->buttons() & Qt::LeftButton) {

		if ((m_nSelectionMode.bVerSelection) || (m_nSelectionMode.bTriSelection) || m_nSelectionMode.bDeformSelection || m_nSelectionMode.bDeformMove)
		{
			GLfloat _x = (2.*pos[0] - m_vViewport[2]) / m_vViewport[2];
			GLfloat _y = (m_vViewport[3] - 2.*pos[1]) / m_vViewport[3];
			ptReal2[0] = m_PanOffsetX + _x*m_ZoomScale;
			ptReal2[1] = m_PanOffsetY + _y*m_ZoomScale*m_ratio;

			updateGL();
			return;
		}

		if (m_nManiMode == Trackball)

		{
			GLfloat dx, dy, dz;

			GLPtTo3DSphere(pos, m_vCurrVec);

			dx = m_vCurrVec[0] - m_vPrevVec[0];
			dy = m_vCurrVec[1] - m_vPrevVec[1];
			dz = m_vCurrVec[2] - m_vPrevVec[2];

			m_TrackingAngle = 45.0f * (float)sqrt(dx*dx + dy*dy + dz*dz);

			m_vAxis[0] = m_vPrevVec[1] * m_vCurrVec[2] - m_vPrevVec[2] * m_vCurrVec[1];
			m_vAxis[1] = m_vPrevVec[2] * m_vCurrVec[0] - m_vPrevVec[0] * m_vCurrVec[2];
			m_vAxis[2] = m_vPrevVec[0] * m_vCurrVec[1] - m_vPrevVec[1] * m_vCurrVec[0];

			m_vPrevVec[0] = m_vCurrVec[0];
			m_vPrevVec[1] = m_vCurrVec[1];
			m_vPrevVec[2] = m_vCurrVec[2];
			updateGL();
			return;
		}


		/*
		GLdouble winx, winy, winz;

		if (m_nSelectionMode.bDeformMove)
		{
		m_TrackingAngle = 0.f;


		//typedef meshtalent::math::Vector3d<double> V3d;
		//V3d v(-0.001699, -0.001298, 0.000000);

		if ((*mymesh)[ext_current_model_index].selectedHandleIDs.size() > 0)
		{

		//int handleIndex = (*mymesh)[ext_current_model_index].selectedHandles[0];
		//std::vector<GeoTriMesh::VertexHandle>& handleIDs = (*mymesh)[ext_current_model_index].pdmesh_->getHandleIDs();
		//assert(handleIndex < handleIDs.size());
		//GeoTriMesh::VertexHandle vh = handleIDs[handleIndex];

		GeoTriMesh::VertexHandle vh = (*mymesh)[ext_current_model_index].selectedHandleIDs[0];
		GeoTriMesh::Point p = (*mymesh)[ext_current_model_index].m_mesh.point(vh);

		// get z-buffer of this handle.


		gluProject(p[0], p[1], p[2], modelview_matrix, projection_matrix, m_vViewport, &winx, &winy, &winz);

		//logviewdockwidget->addText(QString("p: %1 %2 %3\n").arg(p[0]).arg(p[1]).arg(p[2]));

		// get the translation vector.
		GLdouble objx, objy, objz;
		gluUnProject((GLdouble)(event->x()), (GLdouble)(m_vViewport[3] - event->y()), winz, modelview_matrix, projection_matrix, m_vViewport, &objx, &objy, &objz);

		//	logviewdockwidget->addText(QString("obj: %1 %2 %3\n").arg(objx).arg(objy).arg(objz));

		typedef meshtalent::math::Vector3d<double> V3d;
		//V3d v(dx, dy, dz);

		(*mymesh)[ext_current_model_index].pdmesh_->getHandleIDs();

		V3d v((objx - p[0]) / 100, (objy - p[1]) / 100, (objz - p[2]) / 100);


		//dx = m_mxTransform[0] * v[0] + m_mxTransform[4] * v[1] + m_mxTransform[8] * v[2] + m_mxTransform[12];
		//dy = m_mxTransform[1] * v[0] + m_mxTransform[5] * v[1] + m_mxTransform[9] * v[2] + m_mxTransform[13];
		//dz = m_mxTransform[2] * v[0] + m_mxTransform[6] * v[1] + m_mxTransform[10] * v[2] + m_mxTransform[14];
		//GLfloat s = m_mxTransform[3] * v[0] + m_mxTransform[7] * v[1] + m_mxTransform[11] * v[2] + m_mxTransform[15];
		//dx /= s;
		//dy /= s;
		//dz /= s;


		//	v=V3d(dx, dy,dz);
		std::vector<V3d> ts;		for (int i = 0; i < (*mymesh)[ext_current_model_index].selectedHandleIDs.size(); i++)		ts.push_back(v);
		logviewdockwidget->addText(QString("v : %1 %2 %3\n").arg(v[0]).arg(v[1]).arg(v[2]));
		(*mymesh)[ext_current_model_index].pdmesh_->translate((*mymesh)[ext_current_model_index].selectedHandleIDs, ts);
		}
		//updateGL();
		//return;
		}*/
		/*

		if (event->modifiers() & Qt::ShiftModifier)
		{
		m_LightTrackingAngle = m_TrackingAngle;
		m_TrackingAngle = 0.f;

		}
		*/


	}
	else if (event->buttons() & Qt::RightButton){
		m_PanOffsetX += (float)(m_ptPrev[0] - pos[0]) * m_ZoomScale / m_vViewport[2];
		m_PanOffsetY -= (float)(m_ptPrev[1] - pos[1]) * m_ZoomScale / m_vViewport[3];

		resizeGL(m_vViewport[2], m_vViewport[3]);
	}


	m_ptPrev = pos;
	updateGL();

	//	if(m_nSelectionMode != NoSelection)
	//		updateGL();
}



void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	Vec2f pos;
	pos[0] = event->x();
	pos[1] = event->y();

	QRect rect;
	rect = this->rect();

	GLint start_x, start_y, end_x, end_y;

	// Selection rectangle 좌표 설정

	if (m_ptPrev[0] < (GLint)pos[0])
	{
		start_x = (GLint)m_ptPrev[0];
		end_x = (GLint)pos[0];
	}
	else
	{
		start_x = (GLint)pos[0];
		end_x = (GLint)m_ptPrev[0];
	}

	if (m_ptPrev[1] > (GLint)pos[1])
	{
		start_y = (GLint)(rect.height() - m_ptPrev[1]);	 // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top
		end_y = (GLint)(rect.height() - pos[1]);
	}
	else
	{
		start_y = (GLint)(rect.height() - pos[1]);		 // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top
		end_y = (GLint)(rect.height() - m_ptPrev[1]);
	}


	if (event->button() & Qt::LeftButton)
	{

		// Selection 처리

		if ((m_nSelectionMode.bVerSelection || m_nSelectionMode.bDeformSelection || m_nSelectionMode.bDeformMove) && m_bModelLoaded)
		{
			makeCurrent();

			glPushMatrix();

			if (m_nManiMode == Trackball)
			{
				glPushMatrix();
				glLoadIdentity();

				glRotatef(m_TrackingAngle, m_vAxis[0], m_vAxis[1], m_vAxis[2]);
				glMultMatrixf((GLfloat *)m_mxTransform);
				glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)m_mxTransform);

				m_TrackingAngle = 0.f; //중요!
				glPopMatrix();
			}

			glMultMatrixf((GLfloat *)m_mxTransform);


			Qt::KeyboardModifiers modifiers = QApplication::queryKeyboardModifiers();
			if (modifiers.testFlag(Qt::ControlModifier)){
				qDebug() << "CTRL was hold when this function was called";

				GLSelectMeshVertexByColorNumber((*mymesh)[ext_current_model_index],
					start_x, start_y,
					end_x - start_x,
					end_y - start_y,
					2);
			}
			else
			{
				GLSelectMeshVertexByColorNumber((*mymesh)[ext_current_model_index],
					start_x, start_y,
					end_x - start_x,
					end_y - start_y,
					1);
			}

			glPopMatrix();

			doneCurrent();
		}

		if (m_nSelectionMode.bTriSelection &&  m_bModelLoaded)
		{
			makeCurrent();

			glPushMatrix();

			if (m_nManiMode == Trackball)
			{
				glPushMatrix();
				glLoadIdentity();

				glRotatef(m_TrackingAngle, m_vAxis[0], m_vAxis[1], m_vAxis[2]);
				glMultMatrixf((GLfloat *)m_mxTransform);
				glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)m_mxTransform);

				m_TrackingAngle = 0.f; //중요!
				glPopMatrix();
			}

			glMultMatrixf((GLfloat *)m_mxTransform);

			Qt::KeyboardModifiers modifiers = QApplication::queryKeyboardModifiers();
			if (modifiers.testFlag(Qt::ControlModifier))
			{
				qDebug() << "CTRL was hold when this function was called";

				GLSelectMeshFace(
					start_x, start_y,
					end_x - start_x,
					end_y - start_y,
					2);
			}
			else
			{
				GLSelectMeshFace(
					start_x, start_y,
					end_x - start_x,
					end_y - start_y,
					1);
			}
			glPopMatrix();

			doneCurrent();
		}


		ptReal1.value[0] = ptReal1.value[1] = ptReal2.value[0] = ptReal2.value[1] = 0;			// 초기화;


		/*

		if (m_nSelectionMode.bDeformSelection)
		{


		makeCurrent();

		glPushMatrix();

		if (m_nManiMode == Trackball)
		{
		glPushMatrix();
		glLoadIdentity();

		glRotatef(m_TrackingAngle, m_vAxis[0], m_vAxis[1], m_vAxis[2]);
		glMultMatrixf((GLfloat *)m_mxTransform);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)m_mxTransform);

		m_TrackingAngle = 0.f; //중요!
		glPopMatrix();
		}

		glMultMatrixf((GLfloat *)m_mxTransform);



		//	processMousePickRelease(event);

		glPopMatrix();

		doneCurrent();
		}*/



	}

	updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent * event)
{

	if ((*mymesh)[ext_current_model_index]->selectedHandleIDs.size() > 0)
	{
		typedef meshtalent::math::Vector3d<double> V3d;
		std::vector<V3d> ts;

		switch (event->key())
		{
			//10픽셀씩
		case Qt::Key_Left:
		{
							 Vec3f u = Vec3f(-10 * m_ZoomScale / m_vViewport[2],
								 0.0 * m_ZoomScale / m_vViewport[3],
								 0.0);

							 V3d v(0.0, 0.0, 0.0);

							 // vector calculation 
							 for (int i = 0; i < 3; ++i){
								 for (int j = 0; j < 3; ++j){
									 v[i] += m_mxTransform[i * 4 + j] * u[j];

								 }
							 }

							 ts.push_back(v);
							 (*mymesh)[ext_current_model_index]->pdmesh_->translate((*mymesh)[ext_current_model_index]->selectedHandleIDs, ts);
							 break;
		}
		case Qt::Key_Right:
		{
							  Vec3f u = Vec3f(10 * m_ZoomScale / m_vViewport[2],
								  0.0 * m_ZoomScale / m_vViewport[3],
								  0.0);

							  V3d v(0.0, 0.0, 0.0);

							  // vector calculation 
							  for (int i = 0; i < 3; ++i){
								  for (int j = 0; j < 3; ++j){
									  v[i] += m_mxTransform[i * 4 + j] * u[j];

								  }
							  }
							  ts.push_back(v);
							  (*mymesh)[ext_current_model_index]->pdmesh_->translate((*mymesh)[ext_current_model_index]->selectedHandleIDs, ts);
							  break;
		}
		case Qt::Key_Down:
		{
							 Vec3f u = Vec3f(0.0 * m_ZoomScale / m_vViewport[2],
								 -10.0 * m_ZoomScale / m_vViewport[3],
								 0.0);

							 V3d v(0.0, 0.0, 0.0);

							 // vector calculation 
							 for (int i = 0; i < 3; ++i){
								 for (int j = 0; j < 3; ++j){
									 v[i] += m_mxTransform[i * 4 + j] * u[j];

								 }
							 }
							 ts.push_back(v);
							 (*mymesh)[ext_current_model_index]->pdmesh_->translate((*mymesh)[ext_current_model_index]->selectedHandleIDs, ts);
							 break;
		}
		case Qt::Key_Up:
		{
						   Vec3f u = Vec3f(0.0* m_ZoomScale / m_vViewport[2],
							   10.0 * m_ZoomScale / m_vViewport[3],
							   0.0);

						   V3d v(0.0, 0.0, 0.0);

						   // vector calculation 
						   for (int i = 0; i < 3; ++i){
							   for (int j = 0; j < 3; ++j){
								   v[i] += m_mxTransform[i * 4 + j] * u[j];

							   }
						   }
						   ts.push_back(v);
						   (*mymesh)[ext_current_model_index]->pdmesh_->translate((*mymesh)[ext_current_model_index]->selectedHandleIDs, ts);
						   break;
		}
		case Qt::Key_PageDown:
		{
								 Vec3f u = Vec3f(0.0 * m_ZoomScale / m_vViewport[2],
									 0.0 * m_ZoomScale / m_vViewport[3],
									 -10.0* m_ZoomScale / m_vViewport[3]);

								 V3d v(0.0, 0.0, 0.0);

								 // vector calculation 
								 for (int i = 0; i < 3; ++i){
									 for (int j = 0; j < 3; ++j){
										 v[i] += m_mxTransform[i * 4 + j] * u[j];

									 }
								 }
								 ts.push_back(v);
								 (*mymesh)[ext_current_model_index]->pdmesh_->translate((*mymesh)[ext_current_model_index]->selectedHandleIDs, ts);
								 break;
		}
		case Qt::Key_PageUp:
		{
							   Vec3f u = Vec3f(0.0 * m_ZoomScale / m_vViewport[2],
								   0.0 * m_ZoomScale / m_vViewport[3],
								   10.0* m_ZoomScale / m_vViewport[3]);

							   V3d v(0.0, 0.0, 0.0);

							   // vector calculation 
							   for (int i = 0; i < 3; ++i){
								   for (int j = 0; j < 3; ++j){
									   v[i] += m_mxTransform[i * 4 + j] * u[j];

								   }
							   }
							   ts.push_back(v);
							   (*mymesh)[ext_current_model_index]->pdmesh_->translate((*mymesh)[ext_current_model_index]->selectedHandleIDs, ts);
							   break;
		}
		}

		updateGL();
	}
}




void GLWidget::drawSelectBox(const OpenMesh::Vec3f& center, double radius, float *color)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);        // Enable Blending ( NEW )
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Set The Blend Mode ( NEW )

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // Draw Backfacing Polygons As Wireframes ( NEW )
	//	glLineWidth(outlineWidth);        // Set The Line Width ( NEW )

	// glCullFace(GL_FRONT_);        // Don't Draw Any Front-Facing Polygons ( NEW )

	glDepthFunc(GL_LEQUAL);        // Change The Depth Mode ( NEW )

	double xmin, ymin, zmin, xmax, ymax, zmax;
	xmin = center[0] - radius;
	ymin = center[1] - radius;
	zmin = center[2] - radius;
	xmax = center[0] + radius;
	ymax = center[1] + radius;
	zmax = center[2] + radius;
	glBegin(GL_QUADS);
	glColor3f(color[0], color[1], color[2]);
	glVertex3d(xmin, ymin, zmin);
	glVertex3d(xmin, ymin, zmax);
	glVertex3d(xmax, ymin, zmax);
	glVertex3d(xmax, ymin, zmin);

	glVertex3d(xmin, ymax, zmin);
	glVertex3d(xmin, ymax, zmax);
	glVertex3d(xmax, ymax, zmax);
	glVertex3d(xmax, ymax, zmin);

	glVertex3d(xmin, ymin, zmin);
	glVertex3d(xmin, ymax, zmin);
	glVertex3d(xmin, ymax, zmax);
	glVertex3d(xmin, ymin, zmax);

	glVertex3d(xmax, ymin, zmin);
	glVertex3d(xmax, ymax, zmin);
	glVertex3d(xmax, ymax, zmax);
	glVertex3d(xmax, ymin, zmax);

	glVertex3d(xmin, ymin, zmin);
	glVertex3d(xmin, ymax, zmin);
	glVertex3d(xmax, ymax, zmin);
	glVertex3d(xmax, ymin, zmin);

	glVertex3d(xmin, ymin, zmax);
	glVertex3d(xmin, ymax, zmax);
	glVertex3d(xmax, ymax, zmax);
	glVertex3d(xmax, ymin, zmax);

	glEnd();

	glDepthFunc(GL_LESS);        // Reset The Depth-Testing Mode ( NEW )

	//glCullFace(GL_BACK);        // Reset The Face To Be Culled ( NEW )

	//glPolygonMode(GL_BACK, GL_FILL);      // Reset Back-Facing Polygon Drawing Mode ( NEW )

	glDisable(GL_BLEND);        // Disable Blending ( NEW )

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

void GLWidget::draw_graph()
{
	int j = ext_current_model_index;

	assert(((*mymesh)[j]->pdgraph_) != NULL);

	const std::vector<meshtalent::DeformationGraph::Link>& edges = (*mymesh)[j]->pdgraph_->getEdges();
	const int size = edges.size();

	glDisable(GL_LIGHTING);

	glColor3f(1.0, 0.0, 0.0);
	// graph nodes.
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < size; ++i) {
		glVertex3dv(&edges[i].first.g[0]);
	}
	glEnd();
	glColor3f(0.0, 0.0, 0.0);
	// graph edges.
	glBegin(GL_LINES);
	for (int i = 0; i < size; ++i) {
		for (size_t j = 0; j < edges[i].second.size(); ++j) {
			glVertex3dv(&edges[i].first.g[0]);
			glVertex3dv(&edges[edges[i].second[j]].first.g[0]);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);

}
void GLWidget::draw_select_boxes()
{
	for (int j = 0; j < mymesh->size(); j++)
	{
		float color[3];
		color[0] = 1.0;
		color[1] = 0.0;
		color[2] = 0.0;

		const std::set<GeoTriMesh::VertexHandle>& selectedVertices =
			(*mymesh)[j]->pdmesh_->getSVSet();
		if (selectedVertices.size()>0)
		{
			//glColor3f(1.0, 0.0, 1.0);

			typedef std::set<GeoTriMesh::VertexHandle>::const_iterator CITER;

			for (CITER cit = selectedVertices.begin(); cit != selectedVertices.end(); ++cit) {
				OpenMesh::Vec3f center = OpenMesh::vector_cast<Vec3f>((*mymesh)[j]->m_mesh.point(*cit));
				//if (m_nSelectionMode.bDeformMove) {
				//		glLoadName(i);
				//	}
				drawSelectBox(center, 0.005*(((*mymesh)[j]->m_bbMax - (*mymesh)[j]->m_bbMin).norm()), color);
			}
		}

		if ((*mymesh)[j]->selectedHandleIDs.size() > 0 && m_nSelectionMode.bDeformMove)
		{
			color[0] = 0.0; color[1] = 0.0; color[2] = 1.0;
			//GeoTriMesh::VertexHandle vh(index_current_deform);
			for (int i = 0; i < (*mymesh)[j]->selectedHandleIDs.size(); i++)
			{
				GeoTriMesh::VertexHandle vh = (*mymesh)[j]->selectedHandleIDs[i];
				OpenMesh::Vec3f center = OpenMesh::vector_cast<Vec3f>((*mymesh)[j]->m_mesh.point(vh));
				drawSelectBox(center, 0.005*(((*mymesh)[j]->m_bbMax - (*mymesh)[j]->m_bbMin).norm()), color);
			}
		}
	}
}

