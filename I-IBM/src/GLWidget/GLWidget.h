#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <stdio.h>
#include <QtOpenGL>
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <math.h>
#include "../globals.h"
#include "src/Geometry3D/Geometry3D.h"
#include <QOpenGLWidget>

//#include "MemoryLeakDetector/DebugNew.h"

namespace myVec {

	struct Vec2f{
		float value[2];

		inline float& operator [](int _i){
			return value[_i];
		}

		inline bool operator!=(const Vec2f& _rhs) const {
			return !(this->value[0] == _rhs.value[0] && this->value[1] == _rhs.value[1]);
		}
		inline void operator =(const Vec2f& _rhs) {
			this->value[0] = _rhs.value[0];
			this->value[1] = _rhs.value[1];
		}

		inline float length(){
			return (float)sqrt(pow(value[0],2)+pow(value[1],2));
		}

	};

	struct Vec3f{
		float value[3];

		inline Vec3f& operator - (Vec3f& _rhs){
			value[0]-=_rhs.value[0];    value[1]-=_rhs.value[1];    value[2]-=_rhs.value[2];
			return *this;
		}

		inline float length(){
			return (float)sqrt(pow(value[0],2)+pow(value[1],2)+pow(value[2],2));
		}

		inline Vec3f& operator =(const Vec3f& _rhs) {
			value[0] = _rhs.value[0];
			value[1] = _rhs.value[1];
			value[2] = _rhs.value[2];
			return *this;
		}

		inline Vec3f& operator /(const float _rhs) {
			value[0] /= _rhs;
			value[1] /= _rhs;
			value[2] /= _rhs;
			return *this;
		}
	};

}


class GLWidget : public QOpenGLWidget
{
	Q_OBJECT


public:
    GLWidget(QWidget *parent = 0);
	~GLWidget();

	//다른 클래스에서 읽은 모델데이터의 포인터 받아오기
	void GetModel(Geometry3D* _mesh);
	bool m_bModelLoaded;
	Geometry3D* mymesh;


	// OpenGL 셋팅 관련 변수
	GLfloat m_ratio;									//윈도우의 가로 세로 비율
	GLfloat m_ZoomScale, m_PanOffsetX, m_PanOffsetY;	// 확대,축소, 이동 관련
	GLfloat m_Left, m_Right, m_Bottom, m_Top, m_Near, m_Far;	// ViewPort
	GLfloat m_vPrevVec[3], m_vCurrVec[3], m_vAxis[3], m_TrackingAngle, m_LightTrackingAngle;
	GLfloat m_LightTransform[16];
	GLfloat m_mxTransform[16];						// Transform matrix
	GLint m_vcurrentViewport[4];						// 현재 뷰 포트
	GLint m_vViewport[4];								// 현재 뷰 포트
	GLfloat m_currentViewport[4];						// 저장용 viewport
	GLfloat m_currentTransform[16];					// 저장용 transform matrix :현재 시점을 저장

	GLint w,h;
	void GLInitializeState();									//세팅변수들 초기화
	void GLInitializeRC(void);										// 초기화
	virtual void initializeGL();
	virtual void resizeGL(int cx, int cy);			// 윈도우 크기 재조정용
	virtual void paintGL();


	//Manupulation 변수
	myVec::Vec3f  m_bbMin, m_bbMax;							// 바운딩 박스 최소,최대값(ZoomToFit을 위한)
	GLfloat m_currentZoomScale, m_currentPanOffsetX, m_currentPanOffsetY,\
		m_ProjectionVolumeXOffset,m_ProjectionVolumeYOffset; // 현재 시점 저장용
	myVec::Vec2f ptReal1, ptReal2;							// Box Zooming을 위한 변수
	myVec::Vec2f m_ptPrev;									// 마우스의 이전 좌표 저장
	enum ManipulMode {None=0, Trackball, Zooming, Box_Zooming, Panning} m_nManiMode; // 화면조작 모드
	struct SelectionMode {
		bool bVerSelection;
		bool bTriSelection;
} m_nSelectionMode;			// 메쉬 선택 모드
	// Manupulation 함수
	void GLPtTo3DSphere(myVec::Vec2f point, GLfloat* pVec);		// 마우스 좌표를 3차원 구 좌표로 변환(trackball용)
	myVec::Vec2f GLScreenToPoint(int, int);					// 마우스 좌표를 OpenGL 2D 좌표로 변환
	void GLOnSetManiMode(int mode);						// Set the manipulation mode(rotation, zoom, pan, etc)
	void GLOnSetSelectionMode(int mode);		// Set the selection mode(Vertex, Edge, Triangle)

	

	// Texture 이미지 관련 변수
	//CImage m_image;



	// 좌표축 그리는 함수

	// Window Message Handler
	/*   BOOL		GLPreCreateWindow(CREATESTRUCT& cs);
	int			GLOnCreate( HWND hWnd);
	virtual int	GLOnDestory(HWND hWnd);
	BOOL		GLOnEraseBkgnd(){return FALSE;}
	void		GLOnSize(UINT nType, int cx, int cy);
	void		GLOnMouseWheel(UINT nFlags, short zDelta);
	void		GLOnLButtonDown(UINT nFlags, Vec2f point);
	void		GLOnLButtonUp(UINT nFlags, Vec2f point);
	void		GLOnRButtonDown(UINT nFlags, Vec2f point);
	void		GLOnRButtonUp(UINT nFlags, Vec2f point);
	void		GLOnMouseMove(UINT nFlags, Vec2f point);
	*/

	// OpenGL state 셋팅 함수들

	void		GLSetTexture();				// Texture On/Off
	//void		GLInitTexture();			// OpenGL Texture 초기화

	//모델을 시점 변환 함수들
	void GLOnViewIso();												// Isometric View
	void GLOnViewTop();												// Top View
	void GLOnViewBottom();											// Bottom View
	void GLOnViewLeft();											// Left View
	void GLOnViewRight();											// Right View
	void GLOnViewFront();											// Front View
	void GLOnViewBack();											// Back View
	void GLOnViewCenter();

	//현재시점을 저장하는 함수들
	void GLStoreUserView();						// 현재 시점을 저장해 둔다
	void GLLoadUserView();						// 저장된 시점을 불러들인다.

	// Selection Mode
	void GLVertexSelect();											// Select or Deselect vertex
	void GLVertexDeselectAll();										// Deselect All
	void GLVertexRemove();											// Delete all vertex in the picking set
	void GLEdgeSelect();											// Select or Deselect edge
	void GLEdgeDeselectAll();										// Deselect All
	void GLEdgeRemove();											// Delete all edges in the picking set
	void GLFaceSelect();											// Select or Deselect triangle
	void GLFaceDeselectAll();										// Deselect All
	void GLFaceRemove();											// Delete all triangles in the picking set

	// OpenGL Selection
	int GLProcessSelect(GLint hits, GLuint index[512]);				// Picking 처리, GLPickObjects(GLint x, GLint y)에서 호출
	void GLPickObjects(GLint x, GLint y);
	void GLDrawSelectMeshVertex();
	void GLDrawSelectedMeshVertex();
	void GLSelectMeshVertexByColorNumber(GLint nStartX, GLint nStartY, GLint nWidth, GLint nHeight, UINT nFlags);
	void GLUpdateFaceIndices();
	void GLSelectMeshFace(GLint nStartX, GLint nStartY, GLint nWidth, GLint nHeight, UINT nFlags);
	void GLDrawSelectedMeshFace();

	//Draw 세팅 변수
	bool m_bLight;
	bool m_bSmooth;
	bool m_bVertex;
	bool m_bEdge;
	bool m_bFace;
	bool m_bMouseMoving;
	bool m_bVertexNormal;
	bool m_bVertexColor;
	bool m_bBoundingBox;
	bool m_bTexture;
	bool m_bFaceColor;
	GLfloat m_defaultcolor[3]; 

	//-----------그리기 속성
	GLfloat vc[3];						// vertex color
	GLfloat fc[3];						// vertex color

	std::vector<unsigned int>  indices_;		// triangle vertices index
	void RenderScene();
	void GLDrawMeshElements(int mode);		// OpenGL에서 뿌려줄 메쉬 요소 선택

	// 모델 렌더링 관련
	bool m_bCulled;										// Back face culling
	bool m_bDrawAxis;									// Draw XYZ Axis
	bool m_bCenter;									// Centering
	bool m_bDrawWorkspace;									//Draw Workspace
	bool m_bLightDraw;									//Draw Light

	void GLZoomToFit(myVec::Vec3f *m_bbMin, myVec::Vec3f *m_bbMax);	// 모델을 윈도우에 맞춤
	void GLDrawAxis(void);
	void DrawWorkspace() ;	
	void DrawLight(GLfloat poistion[], GLfloat direction[], GLfloat color[]);
	//signals:
	virtual void wheelEvent(QWheelEvent *event);
	virtual void paintEvent(QPaintEvent *event);
	virtual void mousePressEvent( QMouseEvent *event);
	virtual void mouseReleaseEvent( QMouseEvent *event);
	virtual void mouseMoveEvent( QMouseEvent *event);
	//public slots:

};

#endif // GLWIDGET_H
