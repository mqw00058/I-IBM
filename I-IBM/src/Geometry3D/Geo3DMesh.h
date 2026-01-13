#pragma once

#include <QString>

#include <iostream>
#include <stdio.h>
#include <set>
#include <algorithm>

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
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits>    GeoTriMesh;


//--------------------- Namespace
using namespace	OpenMesh;

/////////////////////////////////////////////////////////////////////////////////////

//mesh information structure
typedef struct MeshInfo
{
    int	num_vertices;
    int	num_edges;
    int	num_faces;

	void ZERO(){
		num_vertices = 0;
		num_edges	 = 0;
		num_faces	 = 0;
	}
} MeshInfo;

class Geo3DMesh
{
public:
	Geo3DMesh(void);
	virtual ~Geo3DMesh(void);

	GeoTriMesh			m_mesh;														// 현재 메쉬의 객체
	IO::Options			ropt, wopt;													// Mesh read/write option
	Vec3f				m_modelcenter;												// 모델의 중심점(adjust center)
	Vec3f				m_bbMin, m_bbMax;											// 바운딩 박스 최소, 최대값

	vector<int> m_verPickList;				// 선택된 vertex들을 저장하는 set
	vector<int> m_triPickList;				// 선택된 triangle들을 저장하는 set
	vector<int> m_edgePickList;			// 선택된 edge들을 저장하는 set

	virtual void InitializeMesh();														// 초기화
    virtual void ReadMeshFile(QString filename, QString extension);						// Read file
    virtual void WriteMeshFile(QString filename, QString extension, bool fileopt[3]);	// Write file
	virtual void UpdateMesh();															// 메쉬 업데이트
	virtual void CalculateBoundingBox();												// 메쉬 모델의 Bounding Box 계산

	virtual void VerPickList(int ver_ID, int pick_mode);			// 선택된 vertex를 set에 저장하거나 지움
	virtual void TriPickList(int tri_ID, int pick_mode);			// 선택된 triangle을 set에 저장하거나 지움
	virtual void EdgePickList(int edge_ID, int pick_mode);			// 선택된 edge를 set에 저장하거나 지움

	void ReportMeshInfo();

	// OutputWnd에 Text 출력
	// Requirement "MainFrm.h" <stdio.h> <stdarg.h>
//	void SendText(const char fmt[], ...)
//	{
//		char buffer[1024*2*2];

//		va_list args;
//		va_start(args, fmt);
//		vsprintf_s(buffer,4096, fmt, args);
//		va_end(args);

//		LPCTSTR m_buffer(buffer);

//		CMainFrame *frame = (CMainFrame *)AfxGetMainWnd();

//		//COutputWnd 클래스에 GetInfoView()
//		//CMainFrame 클래스에 GetOutputWnd()
//		//추가하였으므로 참고할 것
//		if (frame)
//			frame->GetOutputWnd()->GetInfoView()->AddString(m_buffer);
//	}

};
