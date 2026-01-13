#include "GL3DRenderScene.h"
//#include "SetPointDlg.h"

GL3DRenderScene::GL3DRenderScene(void)
{
	//GLInitializeState();
}


GL3DRenderScene::~GL3DRenderScene(void)
{
}

// OpenGL State ÃÊ±âÈ­
void GL3DRenderScene::GLInitializeState()
{
	m_bLight			= true;
	m_bSmooth			= true;

	m_bVertex			= false;
	m_bEdge				= false;
	m_bFace				= true;
	m_bAddWireFrame		= false;

	m_bFaceColor		= true;
	m_bVertexColor		= false;
	m_bTexture			= false;
	

	m_bBoundary			= false;
	m_bVertexNormal		= false;
	m_bBoundingBox		= false;

	m_bFeaturepoint			= true;
	m_bFeatureedge			= false;

	m_bMouseMove			= false;

	m_defaultcolor[0] = 0.6f;
	m_defaultcolor[1] = 0.8f;
	m_defaultcolor[2] = 0.9f;
	// default color
	vc[0] = 0.3f;
	vc[1] = 1.f;
	vc[2] = 1.f;
	fc[0] = 0.6f;
	fc[1] = 0.8f;
	fc[2] = 0.9f;


}

void GL3DRenderScene::GLUpdateFaceIndices(Geometry3D* _mesh)
{
	GeoTriMesh::ConstFaceIter        f_it(_mesh->m_mesh.faces_sbegin()), f_end(_mesh->m_mesh.faces_end());
	GeoTriMesh::ConstFaceVertexIter  fv_it;

	indices_.clear();
	indices_.reserve(_mesh->m_mesh.n_faces()*3);

	for (; f_it!=f_end; ++f_it)
	{
		indices_.push_back((fv_it=_mesh->m_mesh.cfv_iter(f_it)).handle().idx());
		indices_.push_back((++fv_it).handle().idx());
		indices_.push_back((++fv_it).handle().idx());
	}
}

// ±×·ÁÁÙ ¸Þ½¬ ¿ä¼Ò¸¦ ¼±ÅÃ
void GL3DRenderScene::GLDrawMeshElements(Geometry3D* _mesh, int _mode)
{

	// OpenMesh Face Iterator(¸Þ½¬ ÀüÃ¼¸¦ µ¹±â À§ÇØ)
	GeoTriMesh::ConstFaceIter			f_it(_mesh->m_mesh.faces_begin());
	GeoTriMesh::ConstFaceIter			f_end(_mesh->m_mesh.faces_end());
	// OpenMesh FaceVertex Iterator	(»ï°¢Çü¿¡ ¼ÓÇÑ 3°³ÀÇ ²ÀÁöÁ¡À» µ¹±â À§ÇØ)
	GeoTriMesh::ConstFaceVertexIter		fv_it;

	GeoTriMesh::ConstEdgeIter			e_it(_mesh->m_mesh.edges_begin());
	GeoTriMesh::ConstEdgeIter			e_end(_mesh->m_mesh.edges_end());

	GeoTriMesh::ConstVertexIter			v_it(_mesh->m_mesh.vertices_begin());
	GeoTriMesh::ConstVertexIter			v_end(_mesh->m_mesh.vertices_end());

	GeoTriMesh::VertexHandle	vh0, vh1;
	GeoTriMesh::HalfedgeHandle	heh0;

	switch(_mode)
	{
		// -------------------- triangle -------------------- 
	case 0 :
		//
		

		//glEnable(GL_LIGHTING);
		//glShadeModel(GL_SMOOTH);

		glEnableClientState(GL_VERTEX_ARRAY);
		GL::glVertexPointer(_mesh->m_mesh.points());

		glEnableClientState(GL_NORMAL_ARRAY);
		GL::glNormalPointer(_mesh->m_mesh.vertex_normals());

		if(m_bTexture == true && _mesh->m_mesh.has_vertex_texcoords2D())
		{
			glColor4f(1.0f,1.0f,1.0f,1.0f);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			GL::glTexCoordPointer(_mesh->m_mesh.texcoords2D());
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
				GL::glColorPointer(3, GL_UNSIGNED_BYTE, 0, _mesh->m_mesh.vertex_colors());
			}
			else {
				glColor3f(fc[0], fc[1], fc[2]);
			}
			glDisable(GL_BLEND);		// Turn Blending Off
			glEnable(GL_DEPTH_TEST);	// Turn Depth Testing On
		}

		// ±×¸®±â ½ÃÀÛ		
		glEnable(GL_POLYGON_OFFSET_FILL);	
		glPolygonOffset(1.0f, 1.0f);		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	

		// »ï°¢Çü ±×¸®±â
		glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, &indices_[0]);

		glDisable(GL_POLYGON_OFFSET_FILL);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		if(m_bTexture == true)
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		else
			glDisableClientState(GL_COLOR_ARRAY);

		glDisable(GL_TEXTURE_2D);

		break;



	

		// -------------------- vertex -------------------- 
	case 1 :
		glPointSize(2.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		GL::glVertexPointer(_mesh->m_mesh.points());

		glDisable(GL_LIGHTING);

		if(m_bVertexColor == true)
		{
			glEnableClientState( GL_COLOR_ARRAY );
			GL::glColorPointer(3, GL_UNSIGNED_BYTE, 0, _mesh->m_mesh.vertex_colors());
		}

		else
		{
			glColor3f(vc[0], vc[1], vc[2]);
		}

		glDrawArrays( GL_POINTS, 0, _mesh->m_mesh.n_vertices() );
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		glEnable(GL_LIGHTING);

		break;

		// -------------------- edge -------------------- 
	case 2 :
		glEnableClientState(GL_VERTEX_ARRAY);
		GL::glVertexPointer(_mesh->m_mesh.points());

		glEnableClientState(GL_NORMAL_ARRAY);
		GL::glNormalPointer(_mesh->m_mesh.vertex_normals());

		glColor3f(0.0f, 0.0f, 0.0f);

		glDisable(GL_LIGHTING);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// ±×¸®±â ½ÃÀÛ
		glBegin(GL_TRIANGLES);
		for (; f_it!=f_end; ++f_it)
		{
			GL::glNormal(_mesh->m_mesh.normal(f_it));
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
			if( _mesh->m_mesh.status(v_it).feature() )
			{
				glVertex3f(_mesh->m_mesh.point(v_it)[0],_mesh->m_mesh.point(v_it)[1],_mesh->m_mesh.point(v_it)[2]);
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
			if(_mesh->m_mesh.status(e_it).feature() )
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

		// ¹Ù¿îµù¹Ú½º ±×¸®±â
		glBegin(GL_LINES);
		glColor3f(0.f,0.f,0.f);
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
		for(; v_it != v_end; ++v_it)
		{	
			Vec3f p, n;
			p = _mesh->m_mesh.point(v_it);
			n = _mesh->m_mesh.normal(v_it);
			glVertex3f(p[0], p[1], p[2]);
			glVertex3f(p[0]+0.1f*n[0], p[1]+0.1f*n[1], p[2]+0.1f*n[2]);
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
			if(_mesh->m_mesh.is_boundary(e_it) == true)
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
}

// vertex color¿¡ µû¶ó vertex¸¦ ±×¸²
void GL3DRenderScene::GLDrawSelectMeshVertex(Geometry3D* _mesh)
{

	GeoTriMesh::VertexIter v_it(_mesh->m_mesh.vertices_begin()), v_End(_mesh->m_mesh.vertices_end());
	int nIndex;

	glClearColor (1.0,1.0,1.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glShadeModel (GL_FLAT);
	glPointSize(1);

	glPushMatrix();

	glEnableClientState(GL_VERTEX_ARRAY);
	GL::glVertexPointer(_mesh->m_mesh.points());

	glEnableClientState(GL_NORMAL_ARRAY);
	GL::glNormalPointer(_mesh->m_mesh.vertex_normals());

	glEnable(GL_DEPTH_TEST);	// Turn Depth Testing On
	glColor3ub(255, 255, 255);

	// ±×¸®±â ½ÃÀÛ		
	glEnable(GL_POLYGON_OFFSET_FILL);	
	glPolygonOffset(1.0f, 1.0f);		
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	

	// »ï°¢Çü ±×¸®±â
	if(indices_.size() !=0)
		glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, &indices_[0]);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glBegin(GL_POINTS);
	for(; v_it != v_End; ++v_it)
	{
		nIndex = v_it.handle().idx();
		glColor3ub (nIndex % 256, (nIndex>>8) % 256, (nIndex>>16) % 256);
		GL::glVertex(_mesh->m_mesh.point(v_it));
	}

	glEnd();
	glPopMatrix();

	glDisable(GL_POLYGON_OFFSET_FILL);

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
}

void GL3DRenderScene::GLDrawSelectedMeshVertex(Geometry3D* _mesh)
{
	GeoTriMesh::VertexHandle v_handle;

    vector<int>::iterator it;

	

	glPointSize(5.0f);
	
	glDisable(GL_LIGHTING);
	
	glColor3f(1.0f, 0.0f, 0.0f);
	
	glBegin(GL_POINTS);
    for(it = _mesh->m_verPickList.begin(); it != _mesh->m_verPickList.end(); ++it)
	{
		int v_index = *it;
		GeoTriMesh::VertexHandle v_handle(v_index);
		GL::glVertex(_mesh->m_mesh.point(v_handle));
	}
	glEnd();

	
	glEnable(GL_LIGHTING);
	
}

// Color ID¿¡ µû¶ó ÁÖ¾îÁø ¿µ¿ª¿¡¼­ vertex¸¦ ¼±ÅÃ
void GL3DRenderScene::GLSelectMeshVertexByColorNumber(Geometry3D* _mesh, GLint nStartX, GLint nStartY, GLint nWidth, GLint nHeight, unsigned int nFlags)
{
/*	GLDrawSelectMeshVertex(_mesh);
	
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
		
		if(ColorNumber >= 0 && ColorNumber < (int)_mesh->m_mesh.n_vertices())
		{
			
			if(nFlags == MK_LBUTTON) 
			{
				_mesh->VerPickList(ColorNumber, 1);
			}
			else if(nFlags == (MK_LBUTTON | MK_CONTROL))
			{
				_mesh->VerPickList(ColorNumber, 2);
			}

		}
	}

	delete[] pRGB;
    */
}


void GL3DRenderScene::setcolor()
{
/*	CSetPointDlg dlg(vc[0], vc[1], vc[2], fc[0], fc[1], fc[2]);

	if(dlg.DoModal() == IDOK){

		this->vc[0] = dlg.m_pointcolor_R;
		this->vc[1] = dlg.m_pointcolor_G;
		this->vc[2] = dlg.m_pointcolor_B;

		this->fc[0] = dlg.m_facecolor_R;
		this->fc[1] = dlg.m_facecolor_G;
		this->fc[2] = dlg.m_facecolor_B;


	}
    */
}



