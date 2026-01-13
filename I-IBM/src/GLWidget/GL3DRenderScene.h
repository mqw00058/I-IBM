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

	//-----------±×¸®±â ¼Ó¼º
	GLfloat vc[3];						// vertex color
	GLfloat fc[3];						// vertex color

 	std::vector<unsigned int>  indices_;		// triangle vertices index

	//---------------------------------------------------------
	void GLInitializeState();						// OpenGL State ÃÊ±âÈ­

	void GLUpdateFaceIndices(Geometry3D * _mesh);

	void GLDraw(Geometry3D *_mesh)
	{
		if (_mesh->m_mesh.n_vertices() == 0) {

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

		if (m_bMouseMove)
			GLDrawMeshElements(_mesh, 1);
		else
		{
			if (m_bFeaturepoint)
				GLDrawMeshElements(_mesh, 3);		// Draw feature points

			if (m_bFeatureedge)
				GLDrawMeshElements(_mesh, 4);		// Draw feature points


			if (m_bFace)
			{
				if (_mesh->m_mesh.n_faces() != 0)
					GLDrawMeshElements(_mesh, 0);		// Draw Mesh as triangle
			}

			if (m_bVertex)
				GLDrawMeshElements(_mesh, 1);		// Draw Mesh as point

			if (m_bEdge)
			{
				if (_mesh->m_mesh.n_edges() != 0)
					GLDrawMeshElements(_mesh, 2);		// Draw mesh as edges
			}

			if (m_bVertexNormal) {
				GLDrawMeshElements(_mesh, 8);
			}

			if (m_bBoundary)
			{
				GLDrawMeshElements(_mesh, 10);
			}
			if (m_bBoundingBox)
			{
				GLDrawMeshElements(_mesh, 7);
			}

		}
	}				// Render Scene for Mesh	

	void GLDrawMeshElements(Geometry3D * _mesh, int _mode);

	virtual void GLDrawSelectMeshVertex(Geometry3D *_mesh);		// vertex ID¿¡ µû¶ó vertex color¸¦ ±×¸²
	virtual void GLDrawSelectedMeshVertex(Geometry3D *_mesh);	// ¼±ÅÃµÈ vertex¸¸ ±×¸®±â
    virtual void GLSelectMeshVertexByColorNumber(Geometry3D *_mesh, GLint, GLint, GLint, GLint, unsigned int);	// Color ID¿¡ µû¶ó ÁÖ¾îÁø ¿µ¿ª¿¡¼­ vertex¸¦ ¼±ÅÃ


	void setcolor();

};
