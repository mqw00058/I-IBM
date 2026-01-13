#include "GL3DDoc.h"

GL3DDoc::GL3DDoc(void)
{
}

GL3DDoc::~GL3DDoc(void)
{
}

// Show or Hide Face
void GL3DDoc::GLShowHideMeshTriangle()
{
    this->m_renderScene.m_bFace = !this->m_renderScene.m_bFace;
}

// Show or Hide Edge
void GL3DDoc::GLShowHideMeshEdge()
{
    this->m_renderScene.m_bEdge = !this->m_renderScene.m_bEdge;
}

// Show or Hide Vertex
void GL3DDoc::GLShowHideMeshVertex()
{
    this->m_renderScene.m_bVertex = !this->m_renderScene.m_bVertex;
}

// Show or Hide Vertex
void GL3DDoc::GLShowHideMeshTexture()
{
    this->m_renderScene.m_bTexture = !this->m_renderScene.m_bTexture;
}

// Show or Hide Boundary
void GL3DDoc::GLShowHideMeshBoundary()
{
    this->m_renderScene.m_bBoundary = !this->m_renderScene.m_bBoundary;
}

// Show or Hide Smooth
void GL3DDoc::GLShowHideMeshSmooth()
{
    this->m_renderScene.m_bSmooth = !this->m_renderScene.m_bSmooth;
}


// Show or Hide Light
void GL3DDoc::GLShowHideMeshLight()
{
    this->m_renderScene.m_bLight = !this->m_renderScene.m_bLight;
}



void GL3DDoc::GLUpdateMesh()
{
	this->m_meshInfo.num_vertices	= this->m_currentTriMesh.m_mesh.n_vertices();
	this->m_meshInfo.num_edges		= this->m_currentTriMesh.m_mesh.n_edges();
	this->m_meshInfo.num_faces		= this->m_currentTriMesh.m_mesh.n_faces();

    this->m_renderScene.GLUpdateFaceIndices(&m_currentTriMesh);
}

