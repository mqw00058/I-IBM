#include "GLWidget.h"
#include <QCursor>
#include <QApplication>

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

GLWidget::GLWidget(QWidget *parent) :
QOpenGLWidget(parent)
{
    // ???? ?????? ??????? ?????? ?????? ??????? ?????? ?????? ????
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

    m_nSelectionMode.bTriSelection = m_nSelectionMode.bVerSelection = false;
    //updateGL();
    ///*????? ?????? ?????? ?????? OpenGL ?????? ???? ??? ???????.*/
    ////makeCurrent();
}

void GLWidget::GetModel(Geometry3D* _mesh)
{
    mymesh=_mesh;
    GLUpdateFaceIndices();
    //int a= mymesh->m_mesh.n_vertices();

    m_bModelLoaded=true;


    //updateGL();

}
GLWidget::~GLWidget()
{

}

void GLWidget::initializeGL()
{
    makeCurrent();
    GLInitializeRC();
    GLInitializeState();
    doneCurrent();

}

void GLWidget::GLInitializeRC(void)
{
    glClearColor(1.f,1.f,1.f,1.0f);

    m_nManiMode = Trackball;
    //	m_nSelectionMode = NoSelection;

    m_LightTrackingAngle =0.f;
    m_LightTransform[0]=1.f; m_LightTransform[4]=0.f; m_LightTransform[8]=0.f; m_LightTransform[12]=0.f;
    m_LightTransform[1]=0.f; m_LightTransform[5]=1.f; m_LightTransform[9]=0.f; m_LightTransform[13]=0.f;
    m_LightTransform[2]=0.f; m_LightTransform[6]=0.f; m_LightTransform[10]=1.f; m_LightTransform[14]=0.f;
    m_LightTransform[3]=0.f; m_LightTransform[7]=0.f; m_LightTransform[11]=0.f; m_LightTransform[15]=1.f;

    m_TrackingAngle =  0.f;
    m_vPrevVec[0] =0.f; m_vPrevVec[1] = 0.f; m_vPrevVec[2] = 0.f;
    m_vCurrVec[0] = 0.f; m_vCurrVec[1] = 0.f; m_vCurrVec[2] = 0.f;

    m_ZoomScale = 10.f; m_PanOffsetX = m_PanOffsetY = 0.f;

    m_currentPanOffsetX = m_currentPanOffsetY = 0.f;
    m_ProjectionVolumeXOffset = m_ProjectionVolumeYOffset = 0.f;

    m_Left = -10.f; m_Right = 10.f; m_Bottom = -10.f; m_Top = 10.f;
    m_Near = 1.f; m_Far = 500000.f;


    // ISO view?? ???? ????? ????? ???
    m_mxTransform[0] = 0.544677f; m_mxTransform[1] = -0.362249f;
    m_mxTransform[2] = 0.756375f; m_mxTransform[3] = 0.f;
    m_mxTransform[4] = 0.838644f; m_mxTransform[5] = 0.233635f;
    m_mxTransform[6] = -0.492026f; m_mxTransform[7] = 0.f;
    m_mxTransform[8] = 0.001520f; m_mxTransform[9] = 0.902324f;
    m_mxTransform[10] = 0.431054f; m_mxTransform[11] = 0.f;
    m_mxTransform[12] = 0.f; m_mxTransform[13] = 0.f;
    m_mxTransform[14] = 0.f; m_mxTransform[15] = 1.f;

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

    //Light ????
    /////////////////////////////////////////////////////////////////
    glEnable(GL_LIGHTING);
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

    //////////////////////////////////////////////////////////////////////

    //glEnable(GL_COLOR_MATERIAL);
    //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    //glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
    //glMaterialf(GL_FRONT, GL_SHININESS, 103);			// 2007-3-16 ????

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

    const GLfloat m_Ambient[]	= {0.1f, 0.18725f, 0.1745f, 1.f};
    //const GLfloat m_Diffuse[]	= {0.5294f, 0.8078f, 0.9216f, 1.f};
    const GLfloat m_Diffuse[]	= {0.0f, 0.7490f, 1.0f, 1.f};
    const GLfloat m_Specular[]	= {1.297254f, 1.30829f, 1.306678f, 1.f};
    const GLfloat m_fShininess	= 150.8f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,		m_Ambient   );
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,		m_Diffuse   );
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,	m_Specular  );
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,  &m_fShininess);

    m_bCulled = TRUE;
    glEnable(GL_DEPTH_TEST);
    // Fog
    GLfloat fogColor[4] = { 0.3, 0.3, 0.4, 0.3};
    glFogi(GL_FOG_MODE,    GL_LINEAR);
    glFogfv(GL_FOG_COLOR,  fogColor);
    glFogf(GL_FOG_DENSITY, 0.35);
    glHint(GL_FOG_HINT,    GL_DONT_CARE);
    glFogf(GL_FOG_START,    5.0f);
    glFogf(GL_FOG_END,     25.0f);

    glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW);

    //glClearDepth(1.f);
    //glColor3f(0.7f, 0.7f, 0.7f);



}

// OpenGL State ????
void GLWidget::GLInitializeState()
{
    m_bModelLoaded		= false;
    m_bCenter			= false;// Center Adjust
    m_bLight			= true;
    m_bSmooth			= true;

    m_bVertex			= false;
    m_bEdge				= false;
    m_bFace				= true;


    m_bVertexNormal		= false;
    m_bBoundingBox		= false;
    m_bVertexColor		= false;
    m_bTexture			= false;
    m_bFaceColor		= false;



    m_bDrawAxis = true;
    m_bDrawWorkspace =true;



    m_bMouseMoving		= false;

    m_bLightDraw		= false;

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
    ????????? ??????? ????? ?????? ????.
    glOrtho( left, right, bottom, top, near, far );
    ???? ???? glOrth( 0, 800, 0, 600, -1, 1 ) ????? ???? ???
    ???????? ???? ????? 0, 0??? ???? ?????? 800, 600?? ??? ????? ????? ???.

    ????????? ??????? ????? ?????? ????.
    gluPerspective( fov, aspect, near, far );
    fov?? ?©­??, aspect?? ???¥ì??? ?????, near, far?? ??? ???????? ???, ??? ????? ???.
    ???? 3D??? ?????? ????? ?? ??????? ???.
    gluLookAt( eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ );

    ?? glOrtho?? gluPerspective ????? ????? ?????? ?? ????.

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
    w=width;
    h=height;
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


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ///glClearColor(.0f, .0f, 0.f, 0.f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    if(m_nManiMode == Trackball && m_TrackingAngle>0)
    {
        glPushMatrix();
        glLoadIdentity();

        glRotatef(m_TrackingAngle, m_vAxis[0], m_vAxis[1], m_vAxis[2]);
        glMultMatrixf((GLfloat *)m_mxTransform);
        glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) m_mxTransform);

        m_TrackingAngle=0.f; //???!
        glPopMatrix();
    }
    glMultMatrixf((GLfloat *)m_mxTransform);


    if(m_bDrawAxis)				// Show/Hide Axis
        GLDrawAxis();

    if(m_bDrawWorkspace)				// Show/Hide Axis
        DrawWorkspace();


    ///m_bLightDraw =true;
    if(m_bLightDraw)				// Show/Hide Light
    {
        GLfloat position[] = {0.f, 0.f, 3.f, 0.f};
        GLfloat spotDir[] = {0.f, 0.f, -1.f};
        GLfloat m_Diffuse[]	= {0.34615f, 0.3143f, 0.0903f, 1.0f};
        DrawLight(position, spotDir, m_Diffuse);
    }


    if (m_bModelLoaded)
    {
        // ????? vertex?? display
        if( m_nSelectionMode.bVerSelection || mymesh->m_verPickList.size() >0)
            GLDrawSelectedMeshVertex();

        RenderScene();

        // ????? face?? display
        if( m_nSelectionMode.bTriSelection ||  mymesh->m_triPickList.size() >0)
            GLDrawSelectedMeshFace();

    }


    // Selection rectangle ???
    if((m_nSelectionMode.bVerSelection || m_nSelectionMode.bTriSelection)  && ptReal1 != ptReal2)
    {
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);

        glLineStipple(1, 0xAAAA);  //dot line ?????
        glEnable(GL_LINE_STIPPLE);

        glBegin(GL_LINE_LOOP);
        glColor3f(0.f, 0.f, 0.f);
        glVertex3f(ptReal1[0], ptReal1[1], 0);
        glVertex3f(ptReal2[0], ptReal1[1], 0 );
        glVertex3f(ptReal2[0], ptReal2[1], 0);
        glVertex3f(ptReal1[0], ptReal2[1], 0);
        glEnd();

        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_LINE_STIPPLE);
        glPopMatrix();
    }



    glPopMatrix();
}

void GLWidget::RenderScene()
{
    if(mymesh->m_mesh.n_vertices() == 0){

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

    if(m_bMouseMoving)
        GLDrawMeshElements(1);
    else
    {
        /*	if(m_bFeaturepoint)
        GLDrawMeshElements(mymesh, 3);		// Draw feature points

        if(m_bFeatureedge)
        GLDrawMeshElements(mymesh,4);		// Draw feature points
        */

        if(m_bFace)
        {
            if(mymesh->m_mesh.n_faces() != 0)
                GLDrawMeshElements(0);		// Draw Mesh as triangle
        }

        if(m_bVertex)
            GLDrawMeshElements( 1);		// Draw Mesh as point

        if(m_bEdge)
        {
            if(mymesh->m_mesh.n_edges() != 0)
                GLDrawMeshElements(2);		// Draw mesh as edges
        }

        if(m_bVertexNormal){
            GLDrawMeshElements(8);
        }



        /*if(m_bBoundary)
        {
        GLDrawMeshElements(mymesh, 10);
        }*/
        if(m_bBoundingBox)
        {
            GLDrawMeshElements(7);
        }

    }
}

void GLWidget::GLUpdateFaceIndices()
{
    GeoTriMesh::ConstFaceIter        f_it(mymesh->m_mesh.faces_sbegin()), f_end(mymesh->m_mesh.faces_end());
    GeoTriMesh::ConstFaceVertexIter  fv_it;

    indices_.clear();
    indices_.reserve(mymesh->m_mesh.n_faces()*3);

    for (; f_it!=f_end; ++f_it)
    {
        indices_.push_back((fv_it=mymesh->m_mesh.cfv_iter(f_it)).handle().idx());
        indices_.push_back((++fv_it).handle().idx());
        indices_.push_back((++fv_it).handle().idx());
    }
}


void GLWidget::GLDrawMeshElements(int _mode)
{

    // OpenMesh Face Iterator(??? ????? ???? ????)
    GeoTriMesh::ConstFaceIter			f_it(mymesh->m_mesh.faces_begin());
    GeoTriMesh::ConstFaceIter			f_end(mymesh->m_mesh.faces_end());
    // OpenMesh FaceVertex Iterator	(?????? ???? 3???? ???????? ???? ????)
    GeoTriMesh::ConstFaceVertexIter		fv_it;

    GeoTriMesh::ConstEdgeIter			e_it(mymesh->m_mesh.edges_begin());
    GeoTriMesh::ConstEdgeIter			e_end(mymesh->m_mesh.edges_end());

    GeoTriMesh::ConstVertexIter			v_it(mymesh->m_mesh.vertices_begin());
    GeoTriMesh::ConstVertexIter			v_end(mymesh->m_mesh.vertices_end());

    GeoTriMesh::VertexHandle	vh0, vh1;
    GeoTriMesh::HalfedgeHandle	heh0;

    switch(_mode)
    {
        // -------------------- triangle --------------------
    case 0 :

        //glEnable(GL_LIGHTING);
        //glShadeModel(GL_SMOOTH);

        glEnableClientState(GL_VERTEX_ARRAY);
        GL::glVertexPointer(3, GL_FLOAT, 0, mymesh->m_mesh.points());

        if(mymesh->m_mesh.has_vertex_normals())
        {
            glEnableClientState(GL_NORMAL_ARRAY);
            GL::glNormalPointer(GL_FLOAT, 0, mymesh->m_mesh.vertex_normals());
        }

        //glPolygonMode(GL_FRONT, GL_FILL);

        if(m_bTexture == true && mymesh->m_mesh.has_vertex_texcoords2D())
        {
            glColor4f(1.0f,1.0f,1.0f,1.0f);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            GL::glTexCoordPointer(mymesh->m_mesh.texcoords2D());
            //glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_DECAL);
            glEnable(GL_TEXTURE_2D);
            glDepthFunc(GL_LEQUAL);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glAlphaFunc(GL_GREATER,0.1f);
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);	// Turn Depth Testing Off
            glEnable(GL_CULL_FACE);										// Remove Back Face
            //glDisable(GL_COLOR_MATERIAL);
        }

        else
        {
            if(m_bFaceColor == true) {

                glEnableClientState( GL_COLOR_ARRAY );
                GL::glColorPointer(3, GL_UNSIGNED_BYTE, 0, mymesh->m_mesh.vertex_colors());
            }
            else {
                glColor4f(fc[0], fc[1], fc[2],0.5f);
            }
            //	glDisable(GL_BLEND);		// Turn Blending Off
            //	glEnable(GL_DEPTH_TEST);	// Turn Depth Testing On
        }

        // ????? ????
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // ???? ?????

        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, &indices_[0]);

        glDisable(GL_POLYGON_OFFSET_FILL);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);

        if(m_bTexture == true)
        {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisable(GL_TEXTURE_2D);
        }
        else if(m_bFaceColor == true)
            glDisableClientState(GL_COLOR_ARRAY);



        break;





        // -------------------- vertex --------------------
    case 1 :
        glPointSize(2.0f);
        glEnableClientState(GL_VERTEX_ARRAY);
        GL::glVertexPointer(mymesh->m_mesh.points());

        glDisable(GL_LIGHTING);

        if(m_bVertexColor == true)
        {
            glEnableClientState( GL_COLOR_ARRAY );
            GL::glColorPointer(3, GL_UNSIGNED_BYTE, 0, mymesh->m_mesh.vertex_colors());
        }
        else
        {
            glColor3f(vc[0], vc[1], vc[2]);
        }

        glDrawArrays( GL_POINTS, 0, mymesh->m_mesh.n_vertices() );
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

        glEnable(GL_LIGHTING);

        break;

        // -------------------- edge --------------------
    case 2 :
        glEnableClientState(GL_VERTEX_ARRAY);
        GL::glVertexPointer(mymesh->m_mesh.points());

        glEnableClientState(GL_NORMAL_ARRAY);
        GL::glNormalPointer(mymesh->m_mesh.vertex_normals());

        glColor3f(0.0f, 0.0f, 0.0f);

        glDisable(GL_LIGHTING);

        glPolygonMode(GL_FRONT, GL_LINE);

        // ????? ????
        glBegin(GL_TRIANGLES);
        for (; f_it!=f_end; ++f_it)
        {
            GL::glNormal(mymesh->m_mesh.normal(f_it));
            fv_it = mymesh->m_mesh.cfv_iter(f_it.handle());
            GL::glVertex(mymesh->m_mesh.point(fv_it));
            ++fv_it;
            GL::glVertex(mymesh->m_mesh.point(fv_it));
            ++fv_it;
            GL::glVertex(mymesh->m_mesh.point(fv_it));
        }
        glEnd();

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

        glEnable(GL_LIGHTING);

        break;


        // -------------------- feature points --------------------
    case 3 :
        glPushMatrix();

        glPointSize(6.0f);
        glDisable(GL_LIGHTING);
        glColor3f(0.8f, 0.2f, 0.2f);

        glBegin(GL_POINTS);
        for(; v_it != v_end; ++v_it)
        {
            if( mymesh->m_mesh.status(v_it).feature() )
            {
                glVertex3f(mymesh->m_mesh.point(v_it)[0],mymesh->m_mesh.point(v_it)[1],mymesh->m_mesh.point(v_it)[2]);
            }
        }
        glEnd();

        glPointSize(1.0f);
        glEnable(GL_LIGHTING);
        glPopMatrix();


        glColor3fv(m_defaultcolor);

        break;

        // -------------------- feature edges --------------------
    case 4 :
        glPushMatrix();

        glDisable(GL_LIGHTING);

        glLineWidth(5.0f);
        glColor3f(0.8f, 0.2f, 0.2f);

        glBegin(GL_LINES);
        for(; e_it != e_end; ++e_it)
        {
            if(mymesh->m_mesh.status(e_it).feature() )
            {
                heh0 = mymesh->m_mesh.halfedge_handle(e_it, 0);
                vh0 = mymesh->m_mesh.from_vertex_handle(heh0);
                vh1 = mymesh->m_mesh.to_vertex_handle(heh0);
                glVertex3f(mymesh->m_mesh.point(vh0)[0], mymesh->m_mesh.point(vh0)[1], mymesh->m_mesh.point(vh0)[2]);
                glVertex3f(mymesh->m_mesh.point(vh1)[0], mymesh->m_mesh.point(vh1)[1], mymesh->m_mesh.point(vh1)[2]);
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

        // ???????? ?????
        glBegin(GL_LINES);
        glColor3f(0.f,0.f,0.f);
        glVertex3f(mymesh->m_bbMin[0], mymesh->m_bbMin[1], mymesh->m_bbMin[2]);
        glVertex3f(mymesh->m_bbMin[0], mymesh->m_bbMin[1], mymesh->m_bbMax[2]);

        glVertex3f(mymesh->m_bbMin[0], mymesh->m_bbMin[1], mymesh->m_bbMin[2]);
        glVertex3f(mymesh->m_bbMax[0], mymesh->m_bbMin[1], mymesh->m_bbMin[2]);

        glVertex3f(mymesh->m_bbMin[0], mymesh->m_bbMin[1], mymesh->m_bbMin[2]);
        glVertex3f(mymesh->m_bbMin[0], mymesh->m_bbMax[1], mymesh->m_bbMin[2]);

        glVertex3f(mymesh->m_bbMin[0], mymesh->m_bbMax[1], mymesh->m_bbMin[2]);
        glVertex3f(mymesh->m_bbMin[0], mymesh->m_bbMax[1], mymesh->m_bbMax[2]);

        glVertex3f(mymesh->m_bbMin[0], mymesh->m_bbMax[1], mymesh->m_bbMin[2]);
        glVertex3f(mymesh->m_bbMax[0], mymesh->m_bbMax[1], mymesh->m_bbMin[2]);

        glVertex3f(mymesh->m_bbMax[0], mymesh->m_bbMax[1], mymesh->m_bbMin[2]);
        glVertex3f(mymesh->m_bbMax[0], mymesh->m_bbMin[1], mymesh->m_bbMin[2]);

        glVertex3f(mymesh->m_bbMax[0], mymesh->m_bbMax[1], mymesh->m_bbMin[2]);
        glVertex3f(mymesh->m_bbMax[0], mymesh->m_bbMax[1], mymesh->m_bbMax[2]);

        glVertex3f(mymesh->m_bbMax[0], mymesh->m_bbMax[1], mymesh->m_bbMax[2]);
        glVertex3f(mymesh->m_bbMax[0], mymesh->m_bbMin[1], mymesh->m_bbMax[2]);

        glVertex3f(mymesh->m_bbMax[0], mymesh->m_bbMax[1], mymesh->m_bbMax[2]);
        glVertex3f(mymesh->m_bbMin[0], mymesh->m_bbMax[1], mymesh->m_bbMax[2]);

        glVertex3f(mymesh->m_bbMin[0], mymesh->m_bbMax[1], mymesh->m_bbMax[2]);
        glVertex3f(mymesh->m_bbMin[0], mymesh->m_bbMin[1], mymesh->m_bbMax[2]);

        glVertex3f(mymesh->m_bbMin[0], mymesh->m_bbMin[1], mymesh->m_bbMax[2]);
        glVertex3f(mymesh->m_bbMax[0], mymesh->m_bbMin[1], mymesh->m_bbMax[2]);

        glVertex3f(mymesh->m_bbMax[0], mymesh->m_bbMin[1], mymesh->m_bbMax[2]);
        glVertex3f(mymesh->m_bbMax[0], mymesh->m_bbMin[1], mymesh->m_bbMin[2]);

        glEnd();

        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);

        break;

        // -------------------- vertex normal --------------------
    case 8:
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);

        glColor3f(0.f,0.f,1.f);
        glBegin(GL_LINES);
        for(; v_it != v_end; ++v_it)
        {
            Vec3f p, n;
            p = mymesh->m_mesh.point(v_it);
            n = mymesh->m_mesh.normal(v_it);
            glVertex3f(p[0], p[1], p[2]);
            glVertex3f(p[0]+mymesh->avgEdgeLength*2*n[0], p[1]+mymesh->avgEdgeLength*2*n[1], p[2]+mymesh->avgEdgeLength*2*n[2]);	//???? ???????? 2?? ???? ????? ?? ???? ?????
        }
        glEnd();



        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);


        break;



        // -------------------- boundary edges --------------------
    case 10 :

        glDisable(GL_LIGHTING);

        glLineWidth(5.0f);
        glColor3f(0.8f, 0.2f, 0.2f);

        glBegin(GL_LINES);
        for(; e_it != e_end; ++e_it)
        {
            if(mymesh->m_mesh.is_boundary(e_it) == true)
            {
                heh0 = mymesh->m_mesh.halfedge_handle(e_it, 0);
                vh0 = mymesh->m_mesh.from_vertex_handle(heh0);
                vh1 = mymesh->m_mesh.to_vertex_handle(heh0);
                GL::glVertex(mymesh->m_mesh.point(vh0));
                GL::glVertex(mymesh->m_mesh.point(vh1));
            }
        }

        glEnd();

        glLineWidth(1.0f);

        glEnable(GL_LIGHTING);

        glColor3f(0.6f, 0.8f, 0.9f);

        break;
    }
}


// vertex color?? ???? vertex?? ???
void GLWidget::GLDrawSelectMeshVertex()
{
    GeoTriMesh::VertexIter v_it(mymesh->m_mesh.vertices_begin()), v_End(mymesh->m_mesh.vertices_end());
    int nIndex;

    glClearColor (1.0,1.0,1.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glShadeModel (GL_FLAT);
    glPointSize(1);

    glPushMatrix();

    glEnableClientState(GL_VERTEX_ARRAY);
    GL::glVertexPointer(mymesh->m_mesh.points());

    glEnableClientState(GL_NORMAL_ARRAY);
    GL::glNormalPointer(mymesh->m_mesh.vertex_normals());

    glEnable(GL_DEPTH_TEST);	// Turn Depth Testing On
    glColor3ub(255, 255, 255);

    // ????? ????
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // ???? ?????
    if(indices_.size() !=0)
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, &indices_[0]);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glBegin(GL_POINTS);
    for(; v_it != v_End; ++v_it)
    {
        nIndex = v_it.handle().idx();
        glColor3ub (nIndex % 256, (nIndex>>8) % 256, (nIndex>>16) % 256);
        GL::glVertex(mymesh->m_mesh.point(v_it));
    }

    glEnd();
    glPopMatrix();

    glDisable(GL_POLYGON_OFFSET_FILL);

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
}


void GLWidget::GLDrawSelectedMeshVertex()
{
    GeoTriMesh::VertexHandle v_handle;

    vector <int>::iterator it;


    glPointSize(5.0f);

    glPolygonOffset(-5.0f, -5.0f);
    glDisable(GL_LIGHTING);

    glColor3f(0.0f, 0.0f, 1.0f);

    glBegin(GL_POINTS);
    for(it = mymesh->m_verPickList.begin(); it != mymesh->m_verPickList.end(); ++it)
    {
        int v_index = *it;
        GeoTriMesh::VertexHandle v_handle(v_index);
        GL::glVertex(mymesh->m_mesh.point(v_handle));
    }
    glEnd();

    glEnable(GL_LIGHTING);

}

// Color ID?? ???? ????? ???????? vertex?? ????
void GLWidget::GLSelectMeshVertexByColorNumber(GLint nStartX, GLint nStartY, GLint nWidth, GLint nHeight, UINT nFlags)
{
    logviewdockwidget->addText(QString("\n\n<Vertex Selection Mode>\n\n"));

    GLDrawSelectMeshVertex();

    if (nWidth < 1) nWidth = 1;
    if (nHeight < 1) nHeight = 1;

    unsigned char *pRGB = new unsigned char [3 * (nWidth+1) * (nHeight + 1) + 3];

    glReadBuffer(GL_BACK);
    glReadPixels(nStartX, nStartY, nWidth, nHeight, GL_RGB, GL_UNSIGNED_BYTE, pRGB);

    //int nLast = 0;
    int nEnd = nWidth * nHeight;
    for (int i = 0; i < nEnd; i++)
    {
        int index = i*3;
        int ColorNumber = (pRGB[index]) + (pRGB[index+1]<<8) + (pRGB[index+2]<<16);

        if(ColorNumber >= 0 && ColorNumber < (int)mymesh->m_mesh.n_vertices())
        {
            mymesh->VerPickList(ColorNumber, nFlags);


            /*	if(nFlags == MK_LBUTTON)
            {
            mymesh->VerPickList(ColorNumber, 1);
            }
            else if(nFlags == (MK_LBUTTON | MK_CONTROL))
            {
            mymesh->VerPickList(ColorNumber, 2);
            }
            */
        }
    }

    delete[] pRGB;
}

//  ????? ???????? face?? ????
void GLWidget::GLSelectMeshFace(GLint nStartX, GLint nStartY, GLint nWidth, GLint nHeight, UINT nFlags)
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

    for(f_it=mymesh->m_mesh.faces_begin(); f_it != mymesh->m_mesh.faces_end(); ++f_it)
    {
        mymesh->m_mesh.calc_face_centroid(f_it.handle(), temp);
        posX=temp[0];	posY=temp[1];	posZ=temp[2];
        gluProject(posX,posY,posZ, modelview,projection,viewport,&winX,&winY,&winZ);
        if(((winX>= nStartX) && ((winY) >=nStartY)) && ((winX<= nStartX+nWidth) && (winY) <=nStartY+nHeight))
        {

            mymesh->TriPickList(f_it.handle().idx(),nFlags );
        }
    }
}

void GLWidget::GLDrawSelectedMeshFace()
{
    if(mymesh->m_triPickList.size() != 0 )
    {

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glPolygonOffset(-1.0f, -1.0f);

        vector<int>::iterator it;
        glPushMatrix();

        //

        //	glEnableClientState(GL_VERTEX_ARRAY);
        //	GL::glVertexPointer(3, GL_FLOAT, 0, mymesh->m_mesh.points());
        //glEnableClientState(GL_NORMAL_ARRAY);
        //GL::glNormalPointer(GL_FLOAT, 0, mymesh->m_mesh.vertex_normals());
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 0.2f, 0.2f, 0.7f);

        glDisable(GL_LIGHTING);
        glBegin(GL_TRIANGLES);
        for (it = mymesh->m_triPickList.begin(); it != mymesh->m_triPickList.end(); ++it)
        {
            GeoTriMesh::FaceHandle f_handle(*it);
            GeoTriMesh::FaceVertexIter	fv_it;
            fv_it = mymesh->m_mesh.fv_iter(f_handle);
            //glArrayElement(fv_it.handle().idx());
            glVertex3f(mymesh->m_mesh.point(fv_it.handle())[0],mymesh->m_mesh.point(fv_it.handle())[1],mymesh->m_mesh.point(fv_it.handle())[2]);
            ++fv_it;
            //glArrayElement(fv_it.handle().idx());
            glVertex3f(mymesh->m_mesh.point(fv_it.handle())[0],mymesh->m_mesh.point(fv_it.handle())[1],mymesh->m_mesh.point(fv_it.handle())[2]);
            ++fv_it;
            //glArrayElement(fv_it.handle().idx());
            glVertex3f(mymesh->m_mesh.point(fv_it.handle())[0],mymesh->m_mesh.point(fv_it.handle())[1],mymesh->m_mesh.point(fv_it.handle())[2]);
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







// ?????? ?????? ??? ????
void GLWidget::GLZoomToFit(myVec::Vec3f *m_bbMin, myVec::Vec3f *m_bbMax)
{

    double modelscale;

    modelscale = (double)((*m_bbMax-*m_bbMin).length());
    modelscale /= 2.0f;
    double m_ratio= (m_vViewport[2] > m_vViewport[3])? (double)m_vViewport[2]/m_vViewport[3]: (double)m_vViewport[3]/m_vViewport[2];



    //	GLfloat projection[16];
    //	glGetFloatv(GL_PROJECTION_MATRIX, projection);

    float x,y,z,x_,y_,z_,s_;

    //x= modelcenter.value[0];		y= modelcenter.value[1];		z= modelcenter.value[2];

    x = mymesh->m_modelcenter[0];	y = mymesh->m_modelcenter[1];	z = mymesh->m_modelcenter[2];
    //x_=projection[0]*m_mxTransform[0]*x + projection[1]*m_mxTransform[4]*y +projection[2]*m_mxTransform[8]*z +projection[3]*m_mxTransform[12];
    //y_=projection[4]*m_mxTransform[1]*x + projection[5]*m_mxTransform[5]*y +projection[6]*m_mxTransform[9]*z +projection[7]*m_mxTransform[13];
    //z_=projection[8]*m_mxTransform[2]*x + projection[9]*m_mxTransform[6]*y +projection[10]*m_mxTransform[10]*z +projection[11]*m_mxTransform[14];
    //s_=projection[12]*m_mxTransform[3]*x + projection[13]*m_mxTransform[7]*y +projection[14]*m_mxTransform[11]*z +projection[15]*m_mxTransform[15];

    x_=m_mxTransform[0]*x + m_mxTransform[4]*y +m_mxTransform[8]*z +m_mxTransform[12];
    y_=m_mxTransform[1]*x + m_mxTransform[5]*y +m_mxTransform[9]*z +m_mxTransform[13];
    z_=m_mxTransform[2]*x + m_mxTransform[6]*y +m_mxTransform[10]*z +m_mxTransform[14];
    s_=m_mxTransform[3]*x + m_mxTransform[7]*y +m_mxTransform[11]*z +m_mxTransform[15];



    //m_ZoomScale = sqrt(x_*x_ + y_*y_ + z_*z_)+modelscale;

    m_ZoomScale = fabs(x_/s_)+modelscale;



    makeCurrent();
    resizeGL(m_vViewport[2], m_vViewport[3]);
    //updateGL();
    doneCurrent();
}

// ???? ????? 3???? ???????? ???(Trackball ??)
void GLWidget::GLPtTo3DSphere(myVec::Vec2f point, GLfloat* pVec)
{
    float dis, a;

    /* project x,y onto a hemi-sphere centered within width, height */
    pVec[0] = (2.0f*point[0] - m_vViewport[2]) / m_vViewport[2];
    pVec[1] = (m_vViewport[3] - 2.0f*point[1]) / m_vViewport[3];
    dis = (float) sqrt(pVec[0]*pVec[0] + pVec[1]*pVec[1]);
    pVec[2] = (float) cos((3.14159265f/2.0f) * ((dis < 1.0f) ? dis : 1.0f));
    a = (float) sqrt(pVec[0]*pVec[0] + pVec[1]*pVec[1] + pVec[2]*pVec[2]);
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

    _position[0] = m_PanOffsetX+_x*m_ZoomScale;
    _position[1] = m_PanOffsetY+_y*m_ZoomScale*m_ratio;

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

// ????? ?????? ???
void GLWidget::GLDrawAxis(void)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    float axis_length;

    axis_length = fabs(m_Right - m_Left)*0.1;

    // ?? ?????
    glBegin(GL_LINES);
    glColor3f(1.f,0.f,0.f);
    glVertex3f(0.f,0.f,0.f);
    glVertex3f(axis_length, 0.f, 0.f);
    glColor3f(0.f,1.f,0.f);
    glVertex3f(0.f,0.f,0.f);
    glVertex3f(0.f, axis_length, 0.f);
    glColor3f(0.f,0.f,1.f);
    glVertex3f(0.f,0.f,0.f);
    glVertex3f(0.f, 0.f, axis_length);


    // X mark
    glColor3f(1.f,0.f,0.f);
    glVertex3f(0.9f * axis_length, 0.1f * axis_length, 0.0f);
    glVertex3f(1.1f * axis_length, -0.1f * axis_length, 0.0f);
    glVertex3f(1.1f * axis_length, 0.1f * axis_length, 0.0f);
    glVertex3f(0.9f * axis_length, -0.1f * axis_length, 0.0f);
    // Y mark
    glColor3f(0.f,1.f,0.f);
    glVertex3f(-0.1f * axis_length, 1.1f * axis_length, 0.0f);
    glVertex3f(0.0f, axis_length, 0.0f);
    glVertex3f(0.1f * axis_length, 1.1f * axis_length, 0.0f);
    glVertex3f(0.0f, axis_length, 0.0f);
    // Z mark
    glColor3f(0.f,0.f,1.f);
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


//?????????? ?????
void GLWidget::DrawWorkspace()
{
    int gridNum=20;
    float gridSpace=0.1f;

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
    for(int i = -halfNum; i <= halfNum; i++)
    {
        ::glVertex3f(-width, i * gridSpace, 0.0f);
        ::glVertex3f(width, i * gridSpace, 0.0f);
        ::glVertex3f(i * gridSpace, -width, 0.0f);
        ::glVertex3f(i * gridSpace, width, 0.0f);
    }
    ::glEnd();

    if(oldLight)
        ::glEnable(GL_LIGHTING);

}


void GLWidget::DrawLight(GLfloat poistion[], GLfloat direction[], GLfloat color[])
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();

    GLfloat x_vec[]={1.f, 0.f, 0.f};
    GLfloat XY_projection_direction[3];
    XY_projection_direction[0]=direction[0];
    XY_projection_direction[1]=direction[1];
    XY_projection_direction[2]=0.f;

    GLfloat dot_x=x_vec[0]*XY_projection_direction[0] + x_vec[1]*XY_projection_direction[1] + x_vec[2]*XY_projection_direction[2];
    GLfloat angle_z = acos(dot_x);

    GLfloat y_vec[]={0.f, 1.f, 0.f};
    GLfloat YZ_projection_direction[3];
    YZ_projection_direction[0]=0.f;
    YZ_projection_direction[1]=direction[1];
    YZ_projection_direction[2]=direction[2];

    GLfloat dot_y=y_vec[0]*YZ_projection_direction[0] + y_vec[1]*YZ_projection_direction[1] + y_vec[2]*YZ_projection_direction[2];
    GLfloat angle_x = acos(dot_y);

    GLfloat z_vec[]={0.f, 0.f, 1.f};
    GLfloat ZX_projection_direction[3];
    ZX_projection_direction[0]=direction[0];
    ZX_projection_direction[1]=0.f;
    ZX_projection_direction[2]=direction[2];

    GLfloat dot_z=z_vec[0]*ZX_projection_direction[0] + z_vec[1]*ZX_projection_direction[1] + z_vec[2]*ZX_projection_direction[2];
    GLfloat angle_y = acos(dot_z);

    glMultMatrixf(m_LightTransform);
    //	glTranslatef(poistion[0],poistion[1],poistion[2]);
    glRotatef(m_LightTrackingAngle,m_vAxis[0], m_vAxis[1], m_vAxis[2]);

    //glRotatef(-angle_y*180/3.14,0,1,0);
    //glRotatef(-angle_z*180/3.14,0,0,1);


    //	glRotatef(m_LightTrackingAngle, m_vAxis[0], m_vAxis[1], m_vAxis[2]);
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) m_LightTransform);
    glMultMatrixf(m_LightTransform);

    glBegin(GL_LINES);
    glColor3f(color[0],color[1],color[2]);
    glutSolidCone(2,7,20,20);
    //	glVertex3f(0,0,0);
    //glVertex3f(0, 0, 1);

    glEnd();
    glPopMatrix();
    m_LightTrackingAngle=0.f; //???!

    glLoadIdentity();
    glMultMatrixf((GLfloat *)m_mxTransform);
}
// ---------------------------------------------------------------------
// Set the manipulation mode(rotation, zoom, pan, etc)
void GLWidget::GLOnSetManiMode(int mode)
{
    switch(mode)
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
void GLWidget::GLSetTexture()
{
    //this->m_RenderScene.m_bMeshTexture = !this->m_RenderScene.m_bMeshTexture;
}

// OpenGL Texture ????
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

    m_mxTransform[0] = 0.544677f; m_mxTransform[1] = -0.362249f;
    m_mxTransform[2] = 0.756375f; m_mxTransform[3] = 0.f;
    m_mxTransform[4] = 0.838644f; m_mxTransform[5] = 0.233635f;
    m_mxTransform[6] = -0.492026f; m_mxTransform[7] = 0.f;
    m_mxTransform[8] = 0.001520f; m_mxTransform[9] = 0.902324f;
    m_mxTransform[10] = 0.431054f; m_mxTransform[11] = 0.f;
    m_mxTransform[12] = 0.f; m_mxTransform[13] = 0.f;
    m_mxTransform[14] = 0.f; m_mxTransform[15] = 1.f;


    m_TrackingAngle=0;

    resizeGL(m_vViewport[2], m_vViewport[3]);

    // autoBufferSwap();
    //setAutoBufferSwap(true);

    doneCurrent();
}

void GLWidget::GLOnViewTop()
{
    makeCurrent();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) m_mxTransform);
    m_TrackingAngle=0;
    resizeGL(m_vViewport[2], m_vViewport[3]);

    // autoBufferSwap();
    //setAutoBufferSwap(true);
    doneCurrent();
}

void GLWidget::GLOnViewBottom()
{
    makeCurrent();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(180, 0, 1, 0);
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) m_mxTransform);
    m_TrackingAngle=0;
    resizeGL(m_vViewport[2], m_vViewport[3]);
    // autoBufferSwap();
    //setAutoBufferSwap(true);
    doneCurrent();
}

void GLWidget::GLOnViewLeft()
{
    makeCurrent();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-90, 1, 0, 0);
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) m_mxTransform);
    m_TrackingAngle=0;
    resizeGL(m_vViewport[2], m_vViewport[3]);
    // autoBufferSwap();
    //setAutoBufferSwap(true);
    doneCurrent();
}

void GLWidget::GLOnViewRight()
{
    makeCurrent();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-90, 1, 0, 0);
    glRotatef(180, 0, 0, 1);
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) m_mxTransform);
    m_TrackingAngle=0;
    resizeGL(m_vViewport[2], m_vViewport[3]);

    // autoBufferSwap();
    //setAutoBufferSwap(true);
    doneCurrent();
}

void GLWidget::GLOnViewFront()
{
    makeCurrent();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-90, 0, 1, 0);
    glRotatef(-90, 1, 0, 0);
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) m_mxTransform);
    m_TrackingAngle=0;
    resizeGL(m_vViewport[2], m_vViewport[3]);

    // autoBufferSwap();
    //setAutoBufferSwap(true);
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
    glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) m_mxTransform);
    m_TrackingAngle=0;
    resizeGL(m_vViewport[2], m_vViewport[3]);

    // autoBufferSwap();
    //setAutoBufferSwap(true);
    doneCurrent();
}

void GLWidget::GLOnViewCenter()
{
    if(m_bModelLoaded)
    {
        makeCurrent();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glMultMatrixf((GLfloat *)m_mxTransform);
        if(m_bCenter)
        {

            glTranslatef(-mymesh->m_modelcenter[0],-mymesh->m_modelcenter[1],-mymesh->m_modelcenter[2]);

        }
        else
            glTranslatef(mymesh->m_modelcenter[0],mymesh->m_modelcenter[1],mymesh->m_modelcenter[2]);

        glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) m_mxTransform);

        resizeGL(m_vViewport[2], m_vViewport[3]);

        // autoBufferSwap();
        //setAutoBufferSwap(true);
        doneCurrent();

    }
}

// ???? ?????? ?????? ?¬Õ?
void GLWidget::GLStoreUserView()
{
    for(int i=0;i<4;i++)
    {
        m_currentViewport[i] = m_vViewport[i];
        for(int j=0;j<4;j++)
            m_currentTransform[i*4+j]=m_mxTransform[i*4+j];
    }

    m_currentZoomScale  = m_ZoomScale;
    m_currentPanOffsetX = m_PanOffsetX;
    m_currentPanOffsetY = m_PanOffsetY;

}

// ?????? ?????? ??????¥ä?.
void GLWidget::GLLoadUserView()
{
    for(int i=0;i<4;i++)
    {
        m_vViewport[i] = m_currentViewport[i];
        for(int j=0;j<4;j++)
            m_mxTransform[i*4+j]=m_currentTransform[i*4+j];
    }

    m_ZoomScale  = m_currentZoomScale;
    m_PanOffsetX = m_currentPanOffsetX;
    m_PanOffsetY = m_currentPanOffsetY;

    makeCurrent();
    resizeGL(m_vViewport[2], m_vViewport[3]);
    paintGL();
    // autoBufferSwap();
    //setAutoBufferSwap(true);
    doneCurrent();
}


#define SELECTION_BUFFER_LENGTH 512

// Picking ???, GLPickObjects(GLint x, GLint y)???? ?????? ????? namestack ???? ????
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
            if (index[loop*4+1] < GLuint(depth) && index[loop*4+3] != UINT_MAX)
            {
                choose = index[loop*4+3];					// Select The Closer Object
                depth = index[loop*4+1];					// Store How Far Away It Is
            }
        }
    }

    if(choose >= 0)
        return choose;

    else
        return -1;
}

void GLWidget::GLPickObjects(GLint x, GLint y)
{
    /*GLuint selectBuff[512];
    int i;
    for(i=0; i<512;i++)
    {
    selectBuff[i] = UINT_MAX;
    }
    GLint hits;

    //wglMakeCurrent(m_hDC, m_hRC);
    makeCurrent();

    m_Left = - m_ZoomScale + m_PanOffsetX;
    m_Right = m_ZoomScale + m_PanOffsetX;
    m_Bottom = - m_ZoomScale*m_ratio + m_PanOffsetY;
    m_Top = m_ZoomScale*m_ratio + m_PanOffsetY;

    glSelectBuffer(512, selectBuff);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glRenderMode(GL_SELECT);
    glLoadIdentity();
    if(m_nSelectionMode == MeshVerPick)
    gluPickMatrix((GLdouble) x, (GLdouble) (m_vViewport[3]-y), 10, 10, m_vViewport);
    else if(m_nSelectionMode == MeshTriPick)
    gluPickMatrix((GLdouble) x, (GLdouble) (m_vViewport[3]-y), 1, 1, m_vViewport);
    else if(m_nSelectionMode == MeshEdgePick)
    gluPickMatrix((GLdouble) x, (GLdouble) (m_vViewport[3]-y), 5, 5, m_vViewport);
    glOrtho(m_Left, m_Right, m_Bottom, m_Top, m_Near, m_Far);
    glTranslatef(0.f, 0.f, -50000.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    paintGL();

    hits = glRenderMode(GL_RENDER);
    if(hits>0) GLProcessSelect(hits, selectBuff);	// Picking ???
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glFlush();

    //	SwapBuffers(m_hDC);
    //wglMakeCurrent(NULL, NULL);
    autoBufferSwap();
    doneCurrent();*/
}



// ---------------- ???? ???-------------------------------------

// ?? ?????? ????????? ????? ?????? ???????. ?? ??? ????? ?????? ????? ?????????.
void GLWidget::paintEvent(QPaintEvent *event) {
    makeCurrent();
    paintGL();
    ////setAutoBufferSwap(true);
    // swapBuffers();
    //setAutoBufferSwap(true);
    doneCurrent();
}


void GLWidget::wheelEvent(QWheelEvent *event){

    if (event->angleDelta().y() < 0)
        m_ZoomScale *= 1.2f ;
    else
        m_ZoomScale *= 0.8f;

    makeCurrent();
    resizeGL(m_vViewport[2], m_vViewport[3]);
    update();
    doneCurrent();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    myVec::Vec2f pos;
    pos[0]= event->x();
    pos[1]= event->y();

    m_ptPrev = pos;
    GLPtTo3DSphere(pos, m_vPrevVec);


    // Selection ???

    if(event->button() & Qt::LeftButton)
    {
        // Selection ???
        if((m_nSelectionMode.bTriSelection) || (m_nSelectionMode.bVerSelection))
        {
            GLfloat _x=(2.*pos[0]-m_vViewport[2])/m_vViewport[2];
            GLfloat _y=(m_vViewport[3]-2.*pos[1])/m_vViewport[3];
            ptReal1[0] = m_PanOffsetX+_x*m_ZoomScale;
            ptReal1[1] = m_PanOffsetY+_y*m_ZoomScale*m_ratio;
        }

    }

}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    myVec::Vec2f pos;
    pos[0] = event->x();
    pos[1] = event->y();

    if (event->buttons() & Qt::LeftButton) {

        // vertex selection (box)
        if((m_nSelectionMode.bVerSelection) || (m_nSelectionMode.bTriSelection))
        {
            GLfloat _x=(2.*pos[0]-m_vViewport[2])/m_vViewport[2];
            GLfloat _y=(m_vViewport[3]-2.*pos[1])/m_vViewport[3];
            ptReal2[0] = m_PanOffsetX+_x*m_ZoomScale;
            ptReal2[1] = m_PanOffsetY+_y*m_ZoomScale*m_ratio;

            update();
            return;
        }

        GLfloat dx, dy, dz;

        GLPtTo3DSphere(pos, m_vCurrVec);

        dx = m_vCurrVec[0] - m_vPrevVec[0];
        dy = m_vCurrVec[1] - m_vPrevVec[1];
        dz = m_vCurrVec[2] - m_vPrevVec[2];
        m_TrackingAngle = 45.0f * (float) sqrt(dx*dx + dy*dy + dz*dz);

        if(event->modifiers() & Qt::ShiftModifier)
        {
            m_LightTrackingAngle = m_TrackingAngle;
            m_TrackingAngle=0.f;

        }

        m_vAxis[0] = m_vPrevVec[1]*m_vCurrVec[2] - m_vPrevVec[2]*m_vCurrVec[1];
        m_vAxis[1] = m_vPrevVec[2]*m_vCurrVec[0] - m_vPrevVec[0]*m_vCurrVec[2];
        m_vAxis[2] = m_vPrevVec[0]*m_vCurrVec[1] - m_vPrevVec[1]*m_vCurrVec[0];

        m_vPrevVec[0] = m_vCurrVec[0];
        m_vPrevVec[1] = m_vCurrVec[1];
        m_vPrevVec[2] = m_vCurrVec[2];




    }else if(event->buttons() & Qt::RightButton){
        m_PanOffsetX += (float)(m_ptPrev[0] - pos[0]) * m_ZoomScale / m_vViewport[2];
        m_PanOffsetY -= (float)(m_ptPrev[1] - pos[1]) * m_ZoomScale / m_vViewport[3];

        resizeGL(m_vViewport[2], m_vViewport[3]);
    }


    m_ptPrev = pos;
    update();

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

    // Selection rectangle ??? ????

    if(m_ptPrev[0] < (GLint)pos[0])
    {
        start_x	= (GLint)m_ptPrev[0];
        end_x	= (GLint)pos[0];
    }
    else
    {
        start_x	= (GLint)pos[0];
        end_x	= (GLint)m_ptPrev[0];
    }

    if(m_ptPrev[1] > (GLint)pos[1])
    {
        start_y	= (GLint)(rect.height() - m_ptPrev[1]);	 // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top
        end_y	= (GLint)(rect.height() - pos[1]);
    }
    else
    {
        start_y	= (GLint)(rect.height() - pos[1]);		 // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top
        end_y	= (GLint)(rect.height() - m_ptPrev[1]);
    }


    if(event->button() & Qt::LeftButton)
    {

        // Selection ???

        if(m_nSelectionMode.bVerSelection && m_bModelLoaded)
        {
            makeCurrent();

            glPushMatrix();

            if(m_nManiMode == Trackball)
            {
                glPushMatrix();
                glLoadIdentity();

                glRotatef(m_TrackingAngle, m_vAxis[0], m_vAxis[1], m_vAxis[2]);
                glMultMatrixf((GLfloat *)m_mxTransform);
                glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) m_mxTransform);

                m_TrackingAngle=0.f; //???!
                glPopMatrix();
            }

            glMultMatrixf((GLfloat *)m_mxTransform);


            Qt::KeyboardModifiers modifiers  = QApplication::queryKeyboardModifiers();
            if(modifiers.testFlag( Qt::ControlModifier )){
                qDebug() << "CTRL was hold when this function was called";

                GLSelectMeshVertexByColorNumber(
                    start_x, start_y,
                    end_x - start_x,
                    end_y - start_y,
                    2);
            }
            else
            {
                GLSelectMeshVertexByColorNumber(
                    start_x, start_y,
                    end_x - start_x,
                    end_y - start_y,
                    1);
            }
            glPopMatrix();

            doneCurrent();
        }

        if(m_nSelectionMode.bTriSelection &&  m_bModelLoaded)
        {
            makeCurrent();

            glPushMatrix();

            if(m_nManiMode == Trackball)
            {
                glPushMatrix();
                glLoadIdentity();

                glRotatef(m_TrackingAngle, m_vAxis[0], m_vAxis[1], m_vAxis[2]);
                glMultMatrixf((GLfloat *)m_mxTransform);
                glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) m_mxTransform);

                m_TrackingAngle=0.f; //???!
                glPopMatrix();
            }

            glMultMatrixf((GLfloat *)m_mxTransform);

            Qt::KeyboardModifiers modifiers  = QApplication::queryKeyboardModifiers();
            if(modifiers.testFlag( Qt::ControlModifier ))
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


        ptReal1.value[0] = ptReal1.value[1]=ptReal2.value[0] = ptReal2.value[1]=0;			// ????;
    }

    update();
}
