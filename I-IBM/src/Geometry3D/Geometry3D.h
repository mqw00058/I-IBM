#pragma once

#include <QString>

#include <iostream>
#include <stdio.h>
#include <set>
#include <algorithm>
#define NOMINMAX
using namespace std;


// vs2005에서 ifstream으로 파일명이 한글로된 파일을 읽지 못하는 문제가 생긴다.
// 이 때는 아래의 헤더와 코드를 추가해서 사용하자
// #include <locale>
// setlocale( LC_ALL, "Korean" );  // Console 화면에 한국어 출력

#include <locale>

/////////////////////////////////////////
// SendText()용 -------------------------
//#include "MainFrm.h"
//#include <stdarg.h>
//#include <stdafx.h>
#include "globals.h"
/////////////////////////////////////////

//=====================================================================//
//                  OpenMesh                                           //
//=====================================================================//
//--------------------- Include
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/System/omstream.hh>
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/vector_cast.hh>
#include <OpenMesh/Core/Utils/Property.hh>
#include <OpenMesh/Tools/Utils/Timer.hh>
#include <OpenMesh/Tools/Utils/StripifierT.hh>



/*
// Noise
#include <OpenMesh/Tools/Noise/AddNoiseT.hh>


//--------------------- PMM by H.S.Kim and H.K.Choi
//---------------------------------------------- Discrete Curvature
#include <OpenMesh/Tools/Property/Curvature/DiscreteCurvatureT.hh>
//-------------------- Decimater
#include <OpenMesh/Tools/Decimater/DecimaterT.hh>
#include <OpenMesh/Tools/Decimater/ModQuadricT.hh>					// Garland
#include <OpenMesh/Tools/Decimater/ModNormalFlippingT.hh>			// Normal Flipping Check

// ---------------------------------------- Smoothing
#include <OpenMesh/Tools/Smoother/JacobiLaplaceSmootherT.hh>
#include <OpenMesh/Tools/Smoother/LaplaceSIGT.hh>
// ---------------------------------------- Color Operators
#include <OpenMesh/Tools/Color/ColorOpsT.hh>
*/
//--------------------- PBM by Min Ki Park
#include <OpenMesh/Tools/Filter/BilateralFilter.hh>



//--------------------- PBM by Min Ki Park
// Consolidataion
//#include <PBM/Consolidator/ConsolidatorT.hh>

// Feature
#include <PBM/Feature/NormalClusteringT.hh>
/*
 *#include <PBM/Feature/MultiScaleT.hh>

//#include <PBM/Feature/TensorVoting.hh> //error occur

#include <PBM/Feature/ColorTensorVoting.hh>

// Filtering
#include <pbm/Filtering/UmbrellaOperator.hh>
#include <pbm/Filtering/MultiScaleJBF.hh>

//Property
#include <pbm/Property/curvature.hh>

*/
//--------------------- by In Yeop Jang
#include <PBM/ImpicitSurfaceReconstruction/ImplicitSurfaceReconstruction.hh>

class Geometry3D;
//-------------------------------------------------

struct MyTraits : public OpenMesh::DefaultTraits
{
#if 1
	typedef OpenMesh::Vec3f Point;
	typedef OpenMesh::Vec3f Normal;

#else
	typedef OpenMesh::Vec3d Point;
	typedef OpenMesh::Vec3d Normal;
#endif
};

//--------------------- Type Definitions
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits>			GeoTriMesh;

/*
typedef OpenMesh::StripifierT<MyTraits>						MyStripifier;
typedef OpenMesh::Noise::AddNoiseT<GeoTriMesh>				AddNoise;
typedef OpenMesh::DiscreteCurvature::DiscreteCurvatureT<GeoTriMesh>	MeshCurvature;
typedef OpenMesh::Decimater::DecimaterT<GeoTriMesh>			MeshDecimater;

// Decimation Module Handle type
typedef OpenMesh::Decimater::ModNormalFlippingT<MeshDecimater>::Handle			HModNormalFlipping;
typedef OpenMesh::Decimater::ModQuadricT<MeshDecimater>::Handle					HModQuadric;
//typedef OpenMesh::ColorOps::ColorOpsT<GeoTriMesh>		ColorOps;
*/

typedef OpenMesh::Filter::BilateralFilterT<GeoTriMesh>		BilateralFilter;

typedef PBM::ImplicitSurfaceReconstruction::ImplicitSurfaceReconstructionT<GeoTriMesh>		ImplicitSurface;



//typedef PBM::Consolidator::ConsolidatorT<GeoTriMesh>	Consolidator;
typedef PBM::Feature::NormalClusteringT<GeoTriMesh>		NormalClustering;
/*typedef PBM::Feature::MultiScaleT<GeoTriMesh>			MultiScaling;
typedef PBM::Feature::TensorVotingT<GeoTriMesh>			TensorVoting;
typedef PBM::Feature::CColorTensorVotingT<GeoTriMesh>	ColorTensorVoting;

typedef PBM::Property::Curvature<GeoTriMesh>			PointCurvature;

typedef PBM::Filter::UmbrellaOperatorT<GeoTriMesh>	UmbrellaOperator;
typedef PBM::Filter::MultiScaleJBFT<GeoTriMesh>		MultiScaleJBF;


*/

typedef GeoTriMesh::Scalar	Scalar;
//--------------------- Namespace
using namespace	OpenMesh;
//using namespace OpenMesh::Smoother;
//using namespace PBM;





/////////////////////////////////////////////////////////////////////////////////////

//mesh information structure
typedef struct MeshInfo
{
    unsigned int	num_vertices;
    unsigned int	num_edges;
    unsigned int	num_faces;

	void ZERO(){
		num_vertices = 0;
		num_edges	 = 0;
		num_faces	 = 0;
	}
} MeshInfo;

class Geometry3D
{
public:
	Geometry3D(void);
	virtual ~Geometry3D(void);

	GeoTriMesh			m_mesh;														// 현재 메쉬의 객체
	IO::Options			ropt, wopt;													// Mesh read/write option
	OpenMesh::Vec3f				m_modelcenter;												// 모델의 중심점(adjust center)
	OpenMesh::Vec3f				m_bbMin, m_bbMax;											// 바운딩 박스 최소, 최대값
	double				avgEdgeLength;
	OpenMesh::Vec3f				ColorVariance;

	// color properties
	float				m_colVar;		// color variation


    vector<int> m_verPickList;				// 선택된 vertex들을 저장하는 set
	vector<int> m_triPickList;				// 선택된 triangle들을 저장하는 set
	vector<int> m_edgePickList;			// 선택된 edge들을 저장하는 set

	virtual void InitializeMesh();														// 초기화
    virtual void ReadMeshFile(QString filename, QString extension);						// Read file
    virtual void WriteMeshFile(QString filename, QString extension, bool fileopt[3]);	// Write file
	virtual void UpdateMesh();															// 메쉬 업데이트
	virtual void CalculateBoundingBox();												// 메쉬 모델의 Bounding Box 계산
    virtual void CalcAvgEdgeLen();														// 메쉬 모델의 평균 edge길이

	virtual void VerPickList(int ver_ID, int pick_mode);			// 선택된 vertex를 set에 저장하거나 지움
	virtual void TriPickList(int tri_ID, int pick_mode);			// 선택된 triangle을 set에 저장하거나 지움
	virtual void EdgePickList(int edge_ID, int pick_mode);			// 선택된 edge를 set에 저장하거나 지움

	void ReportGeoInfo();
	void ReportVerInfo(int vID);

/*	// OutputWnd에 Text 출력
	// Requirement "MainFrm.h" <stdio.h> <stdarg.h>
	void SendText(const char fmt[], ...)
	{ 	
		char buffer[1024*2*2];

		va_list args;
		va_start(args, fmt);
		vsprintf_s(buffer,4096, fmt, args);
		va_end(args);

		LPCTSTR m_buffer(buffer);

		CMainFrame *frame = (CMainFrame *)AfxGetMainWnd();

		//qDebug()putWnd 클래스에 GetInfoView()
		//CMainFrame 클래스에 GetOutputWnd()
		//추가하였으므로 참고할 것
		if (frame)
			frame->GetOutputWnd()->GetInfoView()->AddString(m_buffer);
	}
*/
	//-------------------------------------------------------------------------------------
	// MESH Geometric Information
public:
	void CalcDiscreteCurvature(int mod);

	/******************************************************
	geometric properties(curvature, area 등)에 대한 access
	***/

	// easier access to voronoi area at a vertex
	GeoTriMesh::Scalar area( GeoTriMesh::VertexHandle _vh )
	{	return 	m_mesh.property(Area_, _vh); }

	// easier access to mean curvature normal at a vertex
	OpenMesh::Vec3f meancurvaturenormal( GeoTriMesh::VertexHandle _vh )
	{	return 	m_mesh.property(Kh_normal_, _vh); }

	// easier access to mean curvature at a vertex
	GeoTriMesh::Scalar meancurvature( GeoTriMesh::VertexHandle _vh )
 	{	return 	m_mesh.property(Kh_normal_, _vh).norm() * 0.5f; }
 
	// easier access to Gaussian curvature at a vertex
 	GeoTriMesh::Scalar gaussiancurvature( GeoTriMesh::VertexHandle _vh )
	{	return 	m_mesh.property(Kg_, _vh); }

	// easier access to max Principal curvature at a vertex
	GeoTriMesh::Scalar maxprincipalcurvature( GeoTriMesh::VertexHandle _vh )
	{	return m_mesh.property(Kmax_, _vh);	}

	// easier access to min Principal curvature at a vertex
	GeoTriMesh::Scalar minprincipalcurvature( GeoTriMesh::VertexHandle _vh )
	{	return m_mesh.property(Kmin_, _vh);	}

	
	// easier access to surface area 
	GeoTriMesh::Scalar surfacearea()
	{	return surfacearea_;	}


/******************************************************
	mesh geometric processing
	***/

	void mesh_decimation();
	void mesh_filtering(int mod);								// Filtering


/******************************************************
	point geometric processing
	***/
	void PointSetConsolidation();

	void Feature_Gauss_Clustering();	// [Weber10]
	void Feature_MultiScaling();		// [Pauly03]
	void Feature_MultiScaleTensorVote();

	void point_filtering(int mod);

private:
	OpenMesh::VPropHandleT<GeoTriMesh::Color>	m_currentColor;			// Storage for the current vertex color 
	//-------------------------------------------------------------
	// Mesh Properties 
	// geometric properties at current mesh
	OpenMesh::VPropHandleT<OpenMesh::Vec3f>	Kh_normal_;		//mean curvature normal
	OpenMesh::VPropHandleT<GeoTriMesh::Scalar>	Kg_;			//Gaussian curvature
	OpenMesh::VPropHandleT<GeoTriMesh::Scalar>	Kmax_;			//max Principal curvature
	OpenMesh::VPropHandleT<GeoTriMesh::Scalar>	Kmin_;			//min Principal curvature
	OpenMesh::VPropHandleT<GeoTriMesh::Scalar>	Area_;			//Voronoi area each vertex
	GeoTriMesh::Scalar							surfacearea_;	//surface area



	//-------------------------------------------------------------------------------------
	// MY PBM PROGRAM
	public:

		void ConvertToPointSet();
		// vertex color
		void Feature_ColorTensorVoting();
		void ColorEdgeDetection();

		void Point_Test();
		void SetPointColor();
		
		void FilterBilateral();
		
		//by In Yeop Jang
		void Implicit_Surface_Reconstruction();
	
	public:
		void AddNoise(int type, float ratio);



};
