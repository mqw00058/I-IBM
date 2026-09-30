#pragma once

//#include "stdafx.h"
#include "gl.hh"

//      Mesh
#include <Geometry3D/Geometry3D.h>

#include <vector>
//#include <set>
#include <algorithm>

using namespace OpenMesh;
using namespace GL;

class GL3DRenderScene
{
public:
	GL3DRenderScene(void);
	virtual ~GL3DRenderScene(void);

public:
	// OpenGL state
	bool m_bLight;
	bool m_bSmooth;
	bool m_bVertex;
	bool m_bEdge;
	bool m_bFace;
	bool m_bAddWireFrame;
	bool m_bVertexColor;	
	bool m_bFaceColor;
	bool m_bBoundingBox;
	
	bool m_bColor;
	bool m_bVertexNormal;
	bool m_bBoundary;

	bool m_bFeaturepoint;
	bool m_bFeatureedge;

	bool m_bMouseMove;

	bool m_bTexture;	

	GLfloat m_defaultcolor[3]; 

	//-----------그리기 속성
	GLfloat vc[3];						// vertex color
	GLfloat fc[3];						// vertex color

 	std::vector<unsigned int>  indices_;		// triangle vertices index

	//---------------------------------------------------------
	void GLInitializeState();						// OpenGL State 초기화

	void GLDraw(Geometry3D *mymesh);				// Render Scene for Mesh	

	virtual void GLDrawSelectMeshVertex(Geometry3D *_mesh);		// vertex ID에 따라 vertex color를 그림
	virtual void GLDrawSelectedMeshVertex(Geometry3D *_mesh);	// 선택된 vertex만 그리기
    virtual void GLSelectMeshVertexByColorNumber(Geometry3D *_mesh, GLint, GLint, GLint, GLint, unsigned int);	// Color ID에 따라 주어진 영역에서 vertex를 선택


	void setcolor();

};
