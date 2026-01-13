#pragma once

#include "gl.hh"
//#include "PRIME.h"
//#include <Geometry3D/Geometry3D.h>
#include <GLWidget/GL3DRenderScene.h>



class GL3DDoc
{
public:
	GL3DDoc(void);
	virtual ~GL3DDoc(void);

public:
	Geometry3D			m_currentTriMesh;		//OpenMesh instance
    GL3DRenderScene		m_renderScene;			//OpenGL renderScene
	MeshInfo			m_meshInfo;
    QString				m_filePath;


public:
	// Vertex, Edge, Triangle 그리기 모드 선택
	virtual void GLShowHideMeshTriangle();			// Show/Hide Face
	virtual void GLShowHideMeshEdge();				// Show/Hide Edge
	virtual void GLShowHideMeshVertex();			// Show/Hide Vertex
	virtual void GLShowHideMeshTexture();			// Show/Hide Texture
	virtual void GLShowHideMeshBoundary();			// Show/Hide Boundary
	virtual void GLShowHideMeshSmooth();			// Show/Hide Smooth
	virtual void GLShowHideMeshLight();				// Show/Hide Light
	virtual void GLUpdateMesh();

public:	
};
