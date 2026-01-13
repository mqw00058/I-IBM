#include "../Geometry3D/Geometry3D.h"



void Geometry3D::FilterBilateral()
{
	::BilateralFilter bt(m_mesh);
	bt.mode = ::BilateralFilter::FLEISHMAN;
	bt.apply(5, 2*avgEdgeLength, 0.f);

	UpdateMesh();
}

void Geometry3D::Implicit_Surface_Reconstruction()
{
	PBM::ImplicitSurfaceReconstruction::ImplicitSurfaceReconstructionT<GeoTriMesh> isr(m_mesh);
	logviewdockwidget->addText("Loading Points\n");
	isr.LoadPoints();
	
	InitializeMesh();	
	logviewdockwidget->addText("Start Marching Cube\n");
	isr.SurfaceGeneration();
	
	m_mesh.request_face_normals();
	m_mesh.request_vertex_normals();
	m_mesh.request_face_status();
	m_mesh.request_edge_status();
	m_mesh.request_vertex_status();
	m_mesh.request_halfedge_status();


	CalcAvgEdgeLen();
	CalculateBoundingBox();
	UpdateMesh();		// Display를 위해 노말을 꼭 업데이트 할 것

	logviewdockwidget->addText("Visualize...\n");

	//isr.OutputToOpenMesh();

}