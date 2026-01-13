#include "Renderer.h"

void InitializeWindow()
{
	// bind textures
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glEnable( GL_DEPTH_TEST );
	reshape(width, height);
}

void InitializeTextureInfo()
{
	// initialize texture matrices
	int index = 0;
	for(int rr = 0; rr < height; rr++)
	{
		for(int cc = 0; cc < width; cc++, index++)
		{
			dispTextureIndices.at<int>(rr, cc) = index;
			dispTextureCoordinates.at<Vec2f>(rr, cc) =
				Vec2f(static_cast<float>(cc) / width, static_cast<float>(rr) / height);
		}
	}
}

void InitializeWindow(int argc, char* argv[])
{
	// initialize glut settings
	glutInit(&argc, argv);

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH );
	glutInitWindowSize( width/2, height/2 );
	glutInitWindowPosition( 0, 0 );
    
	dispWindowIndex = glutCreateWindow("Point");

	trackball( quat, 90.0, 0.0, 0.0, 0.0 );

	glutIdleFunc( idle );
	glutDisplayFunc( display );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutSpecialFunc( special );
	glutMotionFunc( motion );
	glutMouseFunc( mouse );
	glutCloseFunc( close );

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	InitializeWindow();
	
//	glGenTextures(1, &dispBindIndex);
//	glBindTexture(GL_TEXTURE_2D, dispBindIndex);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void draw_center(void)
{
	glBegin( GL_LINES );
	glColor3f( 1.0f, 0.0f, 0.0f ); /* R */
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.2f, 0.0f, 0.0f );
	glEnd();
	glRasterPos3f( 0.2f, 0.0f, 0.0f );
	glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'x' );
  
	glBegin( GL_LINES );
	glColor3f( 0.0f, 1.0f, 0.0f ); /* G */
		glVertex3f( 0.0f,  0.2f, 0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
	glEnd();
	glRasterPos3f( 0.0f, 0.2f, 0.0f );
	glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'y' );

	glBegin(GL_LINES);
	glColor3f( 0.0f, 0.0f, 1.0f ); /* B */
		glVertex3f( 0.0f, 0.0f, -0.2f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
	glEnd();  
	glRasterPos3f( 0.0f, 0.0f, -0.2f );
	glutBitmapCharacter( GLUT_BITMAP_9_BY_15, 'z' );
}

void idle() {
  static GLuint previousClock=glutGet(GLUT_ELAPSED_TIME);
  static GLuint currentClock=glutGet(GLUT_ELAPSED_TIME);
  static GLfloat deltaT;

  currentClock = glutGet(GLUT_ELAPSED_TIME);
  deltaT=currentClock-previousClock;
  if (deltaT < 1000.0/20.0) {return;} 
  else {previousClock=currentClock;}
    
  char buff[256];
  sprintf(buff, "Frame Rate = %f", 1000.0/deltaT);
  frameRate = buff;  

  kinect.Update();

  glutPostRedisplay();
}

void display()
{
	// load data from kinect
	//WaitForSingleObject(hMutex, INFINITE);
  //EnterCriticalSection(&mCS); 
  //LeaveCriticalSection(&mCS); 
	//ReleaseMutex(hMutex);
  EnterCriticalSection(&kinect.mCriticalSection); 
	Add_Accumulated(kinect.mCameraSpacePoint, kinect.mColor, dispString);

	// clear buffers
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// set matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(t[0], t[1], t[2] - 1.0f);

	GLfloat m[4][4];
	build_rotmatrix( m, quat );
	glMultMatrixf( &m[ 0 ][ 0 ]);
	draw_center();

	// Set the projection from the XYZ to the texture image
	glMatrixMode(GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glMatrixMode( GL_MODELVIEW );
	// assign vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, kinect.mCameraSpacePoint.data); // mPoint.data);

	// assign texture coordinates
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, dispTextureCoordinates.data);

	// assign texture image
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, kinect.mColor.data);
	
	// draw elements
	glPointSize(dispPointSize);
	glDrawElements(GL_POINTS, width * height, GL_UNSIGNED_INT, dispTextureIndices.data);

	// disable what enabled earlier
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();

	// Draw 2D Text
	glColor3f(1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

  // show text info
	if(dispString.empty()) dispString = dispStringInit;
	int currHeight = height - 40;
	glRasterPos2i(10, currHeight);
	for(int cc = 0; cc < dispString.length(); cc++)
	{
		const char ch = dispString[cc];
		if(ch == 10 || ch == 13)
		{
			currHeight -= 40;
			glRasterPos2i(10, currHeight);
		}
		else glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ch);
	} 

  // show frame rate
  currHeight -= 40;
  glRasterPos2i(10, currHeight);
  for(int cc = 0; cc < frameRate.length(); cc++)
  {
    const char ch = frameRate[cc];
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ch);
  }
		
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glutSwapBuffers();
  //LeaveCriticalSection(&kinect.mCriticalSection); 
}

void close()
{
	destroyAllWindows();
	glDeleteTextures(1, &dispBindIndex);
	glutLeaveMainLoop();
}

void keyboard(unsigned char key, int x, int y)
{
	if( key == 27 ) 
	{
		glutDestroyWindow(dispWindowIndex);
	}

	else if( key == 'r' )
	{
		rot_x = rot_y = trans_x = trans_y = trans_z = 0;
		trackball( quat , 90.0, 0.0, 0.0, 0.0 );
		t[ 0 ] = t[ 1 ] = t[ 2 ] = 0;
	}

	else if( key == 'd' )
	{
    kinect.Toggle_ThresholdDepthMode();
	}

  else if( key == 'i' )
	{
    kinect.Toggle_ThresholdInfraredMode();
	}

  else if( key == 'f' )
	{
    kinect.Toggle_RemoveFlyingPixelsMode();
	}

  else if( key == 'n' )
	{
    kinect.Toggle_ApplyNLMFilterMode();
	}
  
	else if( key == 'p')
	{
		kinect.Toggle_PickBodyIndex(dispString);
	}

	else if(key >= '0' && key <= '9')
	{
		kinect.Set_PickedBodyIndex(key, dispString);
	}

	else if(key == 'c')
	{
		Toggle_SaveSelect(dispString);
	}

	else if(key == 'b')
	{
		Add_Select(kinect.mCameraSpacePoint, kinect.mColor, dispString);
	}

	else if( key == 's' )
	{
		Save(kinect.mCameraSpacePoint, kinect.mColor, kinect.mDepth, kinect.mInfrared, dispString);
	}

	else if(key == 'q')
	{
		HRESULT hr = E_FAIL;
		while(!SUCCEEDED(hr))
			kinect.InitializeDefaultSensor();
	}

	else if(key == 'a') {
		Toggle_SaveAccumulated(dispString);
	}

	// redisplay!
	//if(key != 27) glutPostRedisplay();
}

void reshape( int width, int height )
{
	glViewport( 0, 0, width, height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 58, (double)width/height, 0.1, 100 );
	glMatrixMode(GL_MODELVIEW);
}

void motion( int x, int y )
 {
	GLfloat spin_quat[4];
	float gain;
	gain = 2.0; /* trackball gain */

	if( drag_state == GLUT_DOWN )
	{
		if( button_state == GLUT_LEFT_BUTTON )
		{
			trackball( spin_quat,
				( gain * rot_x - width   ) / width,
		        ( height  - gain * rot_y ) / height,
		        (       gain * x - width   ) / width,
		        ( height  - gain * y       ) / height);
			add_quats( spin_quat, quat, quat );
		}
		else if( button_state == GLUT_RIGHT_BUTTON )
		{
			t[ 0 ] -= ((( float )trans_x - x ) / width );
			t[ 1 ] += ((( float )trans_y - y ) / height );
		}
		else if( button_state == GLUT_MIDDLE_BUTTON )
			t[ 2 ] -= ((( float )trans_z - y ) / height * 3 );
		else if( button_state == 3 || button_state == 4 ) // scroll
		{
			
		}
		//glutPostRedisplay();
	}

	rot_x = x;
	rot_y = y;

	trans_x = x;
	trans_y = y;
	trans_z = y;
}

void mouse(int button, int state, int x, int y)
{
	if( state == GLUT_DOWN )
	{
		if( button == GLUT_LEFT_BUTTON )
		{
			rot_x = x;
			rot_y = y;
		}
		else if( button == GLUT_RIGHT_BUTTON )
		{
			trans_x = x;
			trans_y = y;
		}
		else if( button == GLUT_MIDDLE_BUTTON )
		{
			trans_z = y;
		}
		else if( button == 3 || button == 4 )
		{
			const float sign = (static_cast<float>(button) - 3.5f) * 2.0f;
			t[2] -= sign * height * 0.00015f;
		}
	}

	drag_state = state;
	button_state = button;
}


void vzero(float* v)
{
	v[0] = 0.0f;
	v[1] = 0.0f;
	v[2] = 0.0f;
}

void vset(float* v, float x, float y, float z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

void vsub(const float *src1, const float *src2, float *dst)
{
    dst[0] = src1[0] - src2[0];
    dst[1] = src1[1] - src2[1];
    dst[2] = src1[2] - src2[2];
}

void vcopy(const float *v1, float *v2)
{
    register int i;
    for (i = 0 ; i < 3 ; i++)
        v2[i] = v1[i];
}

void vcross(const float *v1, const float *v2, float *cross)
{
    float temp[3];

    temp[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    temp[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    temp[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
    vcopy(temp, cross);
}

float vlength(const float *v)
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void vscale(float *v, float div)
{
    v[0] *= div;
    v[1] *= div;
    v[2] *= div;
}

void vnormal(float *v)
{
    vscale(v,1.0f/vlength(v));
}

float vdot(const float *v1, const float *v2)
{
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

void vadd(const float *src1, const float *src2, float *dst)
{
    dst[0] = src1[0] + src2[0];
    dst[1] = src1[1] + src2[1];
    dst[2] = src1[2] + src2[2];
}

void trackball(float q[4], float p1x, float p1y, float p2x, float p2y)
{
    float a[3]; /* Axis of rotation */
    float phi;  /* how much to rotate about axis */
    float p1[3], p2[3], d[3];
    float t;

    if (p1x == p2x && p1y == p2y) {
        /* Zero rotation */
        vzero(q);
        q[3] = 1.0;
        return;
    }

    /*
     * First, figure out z-coordinates for projection of P1 and P2 to
     * deformed sphere
     */
    vset(p1,p1x,p1y,tb_project_to_sphere(TRACKBALLSIZE,p1x,p1y));
    vset(p2,p2x,p2y,tb_project_to_sphere(TRACKBALLSIZE,p2x,p2y));

    /*
     *  Now, we want the cross product of P1 and P2
     */
    vcross(p2,p1,a);

    /*
     *  Figure out how much to rotate around that axis.
     */
    vsub(p1,p2,d);
    t = vlength(d) / (2.0f*TRACKBALLSIZE);

    /*
     * Avoid problems with out-of-control values...
     */
    if (t > 1.0) t = 1.0;
    if (t < -1.0) t = -1.0;
    phi = 2.0f * asin(t);

    axis_to_quat(a,phi,q);
}

void axis_to_quat(float a[3], float phi, float q[4])
{
    vnormal(a);
    vcopy(a,q);
    vscale(q,sin(phi/2.0f));
    q[3] = cos(phi/2.0f);
}

float tb_project_to_sphere(float r, float x, float y)
{
    float d, t, z;

    d = sqrt(x*x + y*y);
    if (d < r * 0.70710678118654752440f) {    /* Inside sphere */
        z = sqrt(r*r - d*d);
    } else {           /* On hyperbola */
        t = r / 1.41421356237309504880f;
        z = t*t / d;
    }
    return z;
}

void add_quats(float q1[4], float q2[4], float dest[4])
{
    static int count = 0;
    float t1[4], t2[4], t3[4];
    float tf[4];

    vcopy(q1,t1);
    vscale(t1,q2[3]);

    vcopy(q2,t2);
    vscale(t2,q1[3]);

    vcross(q2,q1,t3);
    vadd(t1,t2,tf);
    vadd(t3,tf,tf);
    tf[3] = q1[3] * q2[3] - vdot(q1,q2);

    dest[0] = tf[0];
    dest[1] = tf[1];
    dest[2] = tf[2];
    dest[3] = tf[3];

    if (++count > RENORMCOUNT) {
        count = 0;
        normalize_quat(dest);
    }
}

void normalize_quat(float q[4])
{
    int i;
    float mag;

    mag = (q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
    for (i = 0; i < 4; i++) q[i] /= mag;
}

void build_rotmatrix(float m[4][4], float q[4])
{
    m[0][0] = 1.0f - 2.0f * (q[1] * q[1] + q[2] * q[2]);
    m[0][1] = 2.0f * (q[0] * q[1] - q[2] * q[3]);
    m[0][2] = 2.0f * (q[2] * q[0] + q[1] * q[3]);
    m[0][3] = 0.0f;

    m[1][0] = 2.0f * (q[0] * q[1] + q[2] * q[3]);
    m[1][1]= 1.0f - 2.0f * (q[2] * q[2] + q[0] * q[0]);
    m[1][2] = 2.0f * (q[1] * q[2] - q[0] * q[3]);
    m[1][3] = 0.0f;

    m[2][0] = 2.0f * (q[2] * q[0] - q[1] * q[3]);
    m[2][1] = 2.0f * (q[1] * q[2] + q[0] * q[3]);
    m[2][2] = 1.0f - 2.0f * (q[1] * q[1] + q[0] * q[0]);
    m[2][3] = 0.0f;

    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
}

/*
int main(int argc, char* argv[])
{
  InitializeCriticalSection(&mCS);
	HRESULT hr = E_FAIL;
	while(!SUCCEEDED(hr))
		hr = kinect.InitializeDefaultSensor();
	int w= kinect.nDepthHeight;
	InitializeTextureInfo();
	InitializeWindow(argc, argv);
	glutMainLoop();
    return 0;
}
*/