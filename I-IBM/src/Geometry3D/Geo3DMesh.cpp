#include "Geometry3D.h"
#include <QMessageBox>
#include <QLineEdit>
#include "src/EmbeddedDeform/genGraphDialog.h"

Geometry3D::Geometry3D(void) : pdmesh_(NULL), pdgraph_(NULL)
{
    setlocale( LC_ALL, "Korean" );

    InitializeMesh();
	m_texture = NULL;
	avgEdgeLength = 0.0;
}

Geometry3D::~Geometry3D(void)
{
	if (pdmesh_ != NULL)
		delete pdmesh_;
	if (pdgraph_ != NULL)
		delete pdgraph_;
	if (m_texture != NULL)
		delete[] m_texture;
}

// Mesh를 초기화
void Geometry3D::InitializeMesh(void)
{
	m_mesh.clear();
    m_mesh.release_face_normals();
    m_mesh.release_vertex_normals();
    m_mesh.release_vertex_colors();
    m_mesh.release_face_status();
    m_mesh.release_edge_status();
    m_mesh.release_halfedge_status();
    m_mesh.release_vertex_status();
    m_mesh.release_vertex_texcoords2D();
    //m_modelcenter *= 0.0f;
    //m_verPickList.clear();
    //m_triPickList.clear();
    //m_edgePickList.clear();

    m_modelcenter = GeoTriMesh::Point(0.f, 0.f, 0.f);
    m_bbMin = GeoTriMesh::Point(0, 0, 0);
    m_bbMax = GeoTriMesh::Point(0, 0, 0);

    m_verPickList.clear();
    m_triPickList.clear();
    m_edgePickList.clear();

    //dim_ev = 3;				// 초기는 3차원 geometry(vertex position)만 사용
    //has_ev = false;

	m_texture = NULL;
}

void Geometry3D::init_vertex_texcoords()
{
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
	int n;
	GeoTriMesh::TexCoord2D tex_coord;                     // texture coordinates
	GeoTriMesh::Point mass_point;

	float max_val = -std::numeric_limits<float>::max();
	float min_val = std::numeric_limits<float>::max();
	float range;

	GeoTriMesh::VertexIter vIt, vEnd = m_mesh.vertices_end();
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -//
	n = m_mesh.n_vertices();
	if (n <= 0)
		return;

	// compute mass point
	mass_point.vectorize(0);
	for (vIt = m_mesh.vertices_begin(); vIt != vEnd; ++vIt)
	{
		const GeoTriMesh::Point& pt = m_mesh.point(vIt);

		if (pt[1] > max_val)
			max_val = pt[1];
		if (pt[1] < min_val)
			min_val = pt[1];

		mass_point += pt;
	}
	mass_point *= (1.0 / n);
	range = max_val - min_val;

	if (range == 0.)
		return;

	for (vIt = m_mesh.vertices_begin(); vIt != vEnd; ++vIt)
	{
		const GeoTriMesh::VertexHandle& vh = vIt.handle();
		GeoTriMesh::Point pt = m_mesh.point(vh);
		pt = pt - mass_point;

		/*tex_coord[0] = atan2(pt[0], pt[2]);
		tex_coord[0] = tex_coord[0] + M_PI;         // map into interval [0..2PI]
		tex_coord[0] = tex_coord[0] / (2.*M_PI);    // normalize*/

		tex_coord[0] = atan2(pt[0], fabs(pt[2]));
		tex_coord[0] = tex_coord[0] + M_PI_2;       // map into interval [0..PI]
		tex_coord[0] = tex_coord[0] / M_PI;         // normalize

		// adjust extremal cases to prevent from texturing artifacts
		if (tex_coord[0] >= 1.0f)
			tex_coord[0] = .999f;
		if (tex_coord[0] <= 0.0f)
			tex_coord[0] = 0.001f;

		//tex_coord[1] = atan2(pt[1], sqrt((pt[0]*pt[0])+(pt[2]*pt[2])));d
		//tex_coord[1] = tex_coord[1] + M_PI_2;       // map into interval [0..PI]
		//tex_coord[1] = tex_coord[1] / M_PI;         // normalize

		tex_coord[1] = (pt[1] + mass_point[1] - min_val) / range;

		// adjust extremal cases to prevent from texturing artifacts
		if (tex_coord[1] >= 1.0f)
			tex_coord[1] = .999f;
		if (tex_coord[1] <= 0.0f)
			tex_coord[1] = 0.001f;

		m_mesh.set_texcoord2D(vh, tex_coord);
	}
}

// Read a mesh file
void Geometry3D::ReadMeshFile(QString filename, QString extension)
{
    InitializeMesh();

    //char *r_file = new char[filename.GetLength()+2];
    //size_t Converted = 0;
    //wcstombs_s(&Converted, r_file, filename.GetLength()+1, filename, _TRUNCATE);

//int     sizeOfString = (filename.size());
//	LPTSTR  _file = new TCHAR[ sizeOfString ];
//	const char* filename="D:\\10.CurrentResearch\\I-IBM-OpenGL+OpenMesh\\fandisk.obj";
//	_tcscpy_s(_file, sizeOfString, filename);

    //// Convert a TCHAR string to a LPCSTR
    //CT2CA pszConvertedAnsiString(str);
    //// Construct a std::string using the LPCSTR input
    //std::string _file(pszConvertedAnsiString);

    //std::string _file = (LPSTR)(LPCTSTR)filename;

	// request and update normals
	m_mesh.request_face_normals();
	m_mesh.request_vertex_normals();
	m_mesh.request_face_status();
	m_mesh.request_edge_status();
	m_mesh.request_vertex_status();
	m_mesh.request_halfedge_status();

	m_mesh.request_face_colors();
	m_mesh.request_vertex_colors();
	m_mesh.request_vertex_texcoords2D();

std::string _filename = filename.toLocal8Bit().constData();
  //  std::string stdFileName = filename.utf8();
   // QString qsecond = QString::fromUtf8(stlstring.c_str());
   // QString qs;
    //std::string current_locale_text = qPrintable(filename);
	//std::string _filename("D:\\10.CurrentResearch\\I-IBM-OpenGL+OpenMesh\\fandisk.obj");
	//std::string _filename(filename.toStdString());
	//std::string _filename("C:/fandisk.obj");
if (extension == "obj" || extension == "ply" || extension == "stl")
{
	ropt += IO::Options::VertexColor;
	ropt += IO::Options::VertexTexCoord;
	m_mesh.request_vertex_colors();
	if (!OpenMesh::IO::read_mesh(m_mesh, _filename, ropt))
	{
		std::cerr << "read error\n";
		exit(1);
	}

	else if (extension == "obj") //acquire texture info from obj file
	{
		//	SendText("Open file :\t%s\n\n", _file);
		//	SendText(" ");

		LoadTextureFromObjFile(_filename);
	}
}
    else if (extension == "off" ){
        //ropt += IO::Options::VertexColor;
        //m_mesh.request_vertex_colors();

        if (!OpenMesh::IO::read_mesh(m_mesh, (std::string)filename.toStdString(), ropt))
        {
            std::cerr << "read error\n";
            exit(1);
        }

        else
        {
            //	SendText("Open file :\t%s\n\n", _file);
            //	SendText(" ");
        }
    }

    if(!ropt.check(IO::Options::VertexTexCoord) )
    {
        m_mesh.release_vertex_texcoords2D();
    }
   


	CalcAvgEdgeLen();
    CalculateBoundingBox();
    UpdateMesh();		// Display를 위해 노말을 꼭 업데이트 할 것
    //ReportMeshInfo();
}

void Geometry3D::LoadTextureFromObjFile(std::string objfilepath)
{
	std::string path_;
	//material 정보읽기위해
	std::fstream _in(objfilepath.c_str(), std::ios_base::in); // obj file open

#if defined(WIN32)
	std::string::size_type dot = objfilepath.find_last_of("\\/");
#else
	std::string::size_type dot = objfilepath.rfind("/");
#endif
	path_ = (dot == std::string::npos)
		? "./"
		: std::string(objfilepath.substr(0, dot + 1));



	std::string line;
	std::string keyWrd;
	while (_in && !_in.eof())
	{
		std::getline(_in, line);
		if (_in.bad()){
			omerr() << "  Warning! Could not read file properly!\n";
			return;
		}
		////////// Trim Both leading and trailing spaces/////////////////
		size_t start = line.find_first_not_of(" \t\r\n");
		size_t end = line.find_last_not_of(" \t\r\n");

		if ((std::string::npos == start) || (std::string::npos == end))
			line = "";
		else
			line = line.substr(start, end - start + 1);
		/////////////////////////////////////////////////////////////////

		// comment
		if (line.size() == 0 || line[0] == '#' || isspace(line[0])) {
			continue;
		}

		std::stringstream stream(line);
		stream >> keyWrd;

		// material file
		if (keyWrd == "mtllib")
		{
			std::string matFile;

			// Get the rest of the line, removing leading or trailing spaces
			// This will define the filename of the texture
			std::getline(stream, matFile);

			////////// Trim Both leading and trailing spaces/////////////////
			size_t start = matFile.find_first_not_of(" \t\r\n");
			size_t end = matFile.find_last_not_of(" \t\r\n");

			if ((std::string::npos == start) || (std::string::npos == end))
				matFile = "";
			else
				matFile = matFile.substr(start, end - start + 1);
			/////////////////////////////////////////////////////////////////

			matFile = path_ + matFile; // mtl file path

			_in.close();
			std::fstream matStream(matFile.c_str(), std::ios_base::in); //mtl file open

			if (matStream){
				while (matStream && !matStream.eof())
				{
					std::getline(matStream, line);
					if (matStream.bad()){
						omerr() << "  Warning! Could not read file properly!\n";
						return;
					}

					if (line.empty())
						continue;

					std::stringstream stream(line);

					stream >> keyWrd;

					if (keyWrd == "map_Kd") {
						// Get the rest of the line, removing leading or trailing spaces
						// This will define the filename of the texture
						std::getline(stream, m_texture_path);
						////////// Trim Both leading and trailing spaces/////////////////
						size_t start = m_texture_path.find_first_not_of(" \t\r\n");
						size_t end = m_texture_path.find_last_not_of(" \t\r\n");

						if ((std::string::npos == start) || (std::string::npos == end))
							m_texture_path = "";
						else
							m_texture_path = m_texture_path.substr(start, end - start + 1);
						/////////////////////////////////////////////////////////////////
						m_texture_path = path_ + m_texture_path;

					}
				}
			}

		}
	}
}
// 파일 출력
void Geometry3D::WriteMeshFile(QString filename, QString extension, bool fileopt[3])
{
    //char *w_file = new char[filename.GetLength()+2];
    //size_t Converted = 0;
    //wcstombs_s(&Converted, w_file, filename.GetLength()+1, filename, _TRUNCATE);

    //	int     sizeOfString = (filename.GetLength() + 1);
    //	LPTSTR  lpsz = new TCHAR[ sizeOfString ];
    //	_tcscpy_s(lpsz, sizeOfString, filename);

    wopt.clear();



    if(fileopt[0] == true)
    	wopt += IO::Options::VertexColor;
    else
    	wopt -= IO::Options::VertexColor;

    if(fileopt[1] == true)
    	wopt += IO::Options::VertexNormal;
    else
    	wopt -= IO::Options::VertexNormal;

    if(fileopt[2] == true && m_mesh.has_vertex_texcoords2D())
    	wopt += IO::Options::VertexTexCoord;
    else
    	wopt -= IO::Options::VertexTexCoord;

	std::string lpz = filename.toLocal8Bit().constData();//filename.toStdString();
    if (extension == "obj" ){
		
        if (!OpenMesh::IO::write_mesh(m_mesh, lpz, wopt))
        {
           // std::cerr << "write error\n";
            exit(1);
        }

        else
        {
            logviewdockwidget->addText(QString("Save file :\t%1\n\n").arg(filename));
            //			SendText(" ");
        }
		//lpz.clear();
    }

    else if (extension == "off" ){
        if (!OpenMesh::IO::write_mesh(m_mesh,lpz/*(std::string)filename.toStdString()*/, wopt))
        {
            std::cerr << "write error\n";
            exit(1);
        }

        else
        {
            logviewdockwidget->addText(QString("Save file :\t%1\n\n").arg(filename));
            //			SendText(" ");
        }
    }
}

// mesh update
void Geometry3D::UpdateMesh()
{
    m_mesh.update_normals();			// 노말을 꼭 업데이트 할 것
}


void Geometry3D::CalculateBoundingBox()
{
    // set center and radius
    GeoTriMesh::ConstVertexIter  v_it(m_mesh.vertices_begin()),
            v_end(m_mesh.vertices_end());

    m_bbMin = m_bbMax = (Vec3f)m_mesh.point(v_it);
    for (; v_it!=v_end; ++v_it)
    {
        m_bbMin.minimize((Vec3f)m_mesh.point(v_it));
        m_bbMax.maximize((Vec3f)m_mesh.point(v_it));
    }

    //calculate model center
    m_modelcenter = (m_bbMin + m_bbMax)/2.f;
}

void Geometry3D::CalcAvgEdgeLen()
{
	// Calculate the average length of edges

	GeoTriMesh::VertexIter	v_it, v_end(m_mesh.vertices_end());
	GeoTriMesh::EdgeIter  e_it, e_end(m_mesh.edges_end());

	VertexHandle	vh0, vh1;	// two vertices of an edge
	HalfedgeHandle	heh;
	Vec3f			v0, v1;
	

	for (e_it=m_mesh.edges_begin(); e_it!=e_end; ++e_it)
	{
		heh = m_mesh.halfedge_handle(e_it, 0);
		vh0 = m_mesh.from_vertex_handle(heh);
		vh1 = m_mesh.to_vertex_handle(heh);
		v0 = vector_cast<Vec3f>(m_mesh.point(vh0));
		v1 = vector_cast<Vec3f>(m_mesh.point(vh1));

		avgEdgeLength +=(double) (v0-v1).length();

	}
	avgEdgeLength /= m_mesh.n_edges();
		
	//*** color의 variance 제대로 계산할 것!!
	//Vec3f			c;
	//int n=0;
	//Vec3f mean(0.f, 0.f, 0.f);
	//Vec3f M2(0.f, 0.f, 0.f);
	//for(v_it=m_mesh.vertices_begin(); v_it!=v_end; ++v_it){

	//	c = vector_cast<Vec3f>(m_mesh.color(v_it));
	//	n++;
	//	Vec3f delta;
	//	delta = c-mean;
	//	mean = mean + delta/n;
	//	M2 = M2 + delta*(c-mean);
	//}
	//ColorVariance = M2/n;
	//ColorVariance[0] = sqrt(ColorVariance[0]);
	//ColorVariance[1] = sqrt(ColorVariance[1]);
	//ColorVariance[2] = sqrt(ColorVariance[2]);



}


// 선택된 vertex를 set에 저장
// pick_mode : 0(none) 1(select), 2(deselect all)
// 정렬된 컨테이너를 사용하는 경우 binary_search를 사용하면 log(N)으로 존재여부를 검색할 수 있다.
void Geometry3D::VerPickList(int ver_ID, int pick_mode)
{
	vector<int>::iterator it;

	switch(pick_mode)
    {
    case 0:					// none
        break;

    case 1:					// select
		it = find(m_verPickList.begin(), m_verPickList.end(), ver_ID);
		if( it == m_verPickList.end() )		// list에 없으면
		{ 
            GeoTriMesh::VertexHandle v_handle(ver_ID);
            m_mesh.status(v_handle).set_selected(true);				// Vertex status를 selected로
            m_verPickList.push_back(ver_ID);
			GeoTriMesh::Point p = m_mesh.point(v_handle);
			logviewdockwidget->addText(QString("Model ID: %1 -> Vertex %2  (%3,%4,%5)  is selected \n").arg(this->ModelID).arg(ver_ID).arg(p[0]).arg(p[1]).arg(p[2]));
		
            //ReportVertex(ver_ID);

        }

        break;

    case 2:					// deselect
		it = find(m_verPickList.begin(), m_verPickList.end(), ver_ID);
		if( it != m_verPickList.end() )		// list에 없으면
		{ 
            GeoTriMesh::VertexHandle v_handle(ver_ID);
            m_verPickList.erase(it);
            m_mesh.status(v_handle).set_selected(false);

            logviewdockwidget->addText(QString("Vertex %1 is deselected\n").arg(ver_ID));
        }
        break;

    case 3:					// deselect all mode

        for(it = m_verPickList.begin(); it != m_verPickList.end(); ++it)
        {
            int v_index = *it;
            GeoTriMesh::VertexHandle v_handle(v_index);
            m_mesh.status(v_handle).set_selected(false);
        }

        m_verPickList.clear();
        break;
    }
}

// 선택된 triangle을 set에 저장
// pick_mode : 0(none) 1(select), 2(deselect all)
// 정렬된 컨테이너를 사용하는 경우 binary_search를 사용하면 log(N)으로 존재여부를 검색할 수 있다.
void Geometry3D::TriPickList(int tri_ID, int pick_mode)
{
    switch(pick_mode)
    {
    case 0:					// none
        break;

    case 1:					// select
        if( !binary_search(m_triPickList.begin(), m_triPickList.end(), tri_ID) )
        {
            GeoTriMesh::FaceHandle f_handle(tri_ID);
            m_mesh.status(f_handle).set_selected(true);				// Face status를 selected로
            m_triPickList.push_back(tri_ID);
            logviewdockwidget->addText(QString("Vertex %1 is selected\n").arg(tri_ID));
            //ReportTriangle(tri_ID);
        }
        break;

    case 2:					// deselect
        if( binary_search(m_triPickList.begin(), m_triPickList.end(), tri_ID) )
        {
            GeoTriMesh::FaceHandle f_handle(tri_ID);
            m_triPickList.erase(remove(m_triPickList.begin(), m_triPickList.end(), tri_ID) , m_triPickList.end());
            m_mesh.status(f_handle).set_selected(false);
            logviewdockwidget->addText(QString("Vertex %1 is deselected\n").arg(tri_ID));
        }
        break;


    case 3:					// deselect all mode
        vector<int>::iterator it;

        for(it = m_triPickList.begin(); it != m_triPickList.end(); ++it)
        {
            int f_index = *it;
            GeoTriMesh::FaceHandle f_handle(f_index);
            m_mesh.status(f_handle).set_selected(false);
        }

        m_triPickList.clear();
        break;
    }
}

// 선택된 edge를 set에 저장
// pick_mode : 0(none) 1(select), 2(deselect all), 3 (set feature edge)
// 정렬된 컨테이너를 사용하는 경우 binary_search를 사용하면 log(N)으로 존재여부를 검색할 수 있다.
void Geometry3D::EdgePickList(int edge_ID, int pick_mode)
{
    vector<int>::iterator it;

    switch(pick_mode)
    {
    case 0:					// none
        break;

    case 1:					// select
        if( !binary_search(m_edgePickList.begin(), m_edgePickList.end(), edge_ID) )
        {
            GeoTriMesh::EdgeHandle e_handle(edge_ID);
            m_mesh.status(e_handle).set_selected(true);				// Face status를 selected로
            m_edgePickList.push_back(edge_ID);
            //SendText("Edge %d is selected\n", edge_ID);
            //ReportEdge(edge_ID);
        }
        break;

    case 2:				// deselect
        if( binary_search(m_edgePickList.begin(), m_edgePickList.end(), edge_ID) )
        {
            GeoTriMesh::EdgeHandle e_handle(edge_ID);
            m_edgePickList.erase(remove(m_edgePickList.begin(), m_edgePickList.end(), edge_ID) , m_edgePickList.end());
            //m_mesh.status(e_handle).set_selected(false);
            //SendText("Edge %d is deselected\n\n", edge_ID);
        }
        break;

    case 3:					// deselect all mode
        for(it = m_edgePickList.begin(); it != m_edgePickList.end(); ++it)
        {
            int e_index = *it;
            GeoTriMesh::EdgeHandle e_handle(e_index);
            m_mesh.status(e_handle).set_selected(false);
        }
        m_edgePickList.clear();
        break;

    case 4:					// set feature
        for(it = m_edgePickList.begin(); it != m_edgePickList.end(); ++it)
        {
            int e_index = *it;
            GeoTriMesh::EdgeHandle e_handle(e_index);
            if(m_mesh.status(e_handle).feature())
            {
                m_mesh.status(e_handle).set_feature(false);
            }
            else
            {
                m_mesh.status(e_handle).set_feature(true);
            }
            m_mesh.status(e_handle).set_selected(false);
        }
        m_edgePickList.clear();
        break;
    }

}


void Geometry3D::DeleteVertices(std::vector<int> vlist)
{
	std::vector<int>::iterator it, it1;
	for (it = vlist.begin(); it != vlist.end(); ++it)
	{
		OpenMesh::VertexHandle vh(*it);
		if (m_mesh.is_valid_handle(vh))
		{
			m_mesh.delete_vertex(vh);
			it1 = std::find(m_verPickList.begin(), m_verPickList.end(), *it);
			if (it1 != m_verPickList.end())
				m_verPickList.erase(it1);

		}
	}
	m_mesh.garbage_collection();
	m_mesh.update_normals();
}



void Geometry3D::convert_Geo3DMesh_DeformableMesh3d()
{

	OpenMesh::Utils::MeshCleanT<GeoTriMesh> clean(m_mesh);
	clean.RemoveSmallConnectedComponentsSize(200); //face가 100개 이하로 뭉쳐있는 outliers 제거
	//int aa = m_mesh.n_vertices();
	OpenMesh::TriMesh_ArrayKernelT<MyTraits> &a = m_mesh;
	// aa = m_mesh.n_vertices();
	meshtalent::DeformableMesh3d::InterMesh &b = OpenMesh::mesh_cast<OpenMesh::TriMesh_ArrayKernelT<InterTraits> &>(a);
	//aa = m_mesh.n_vertices();
//	meshtalent::DeformableMesh3d::InterMesh mesh = m_mesh;
	pdmesh_ = new meshtalent::DeformableMesh3d(&b);
	//aa = m_mesh.n_vertices();
}

void Geometry3D::gen_graph_query()
{
	//int a = m_mesh.n_vertices();
	if (pdgraph_ || !m_mesh.n_vertices()) {
		QMessageBox(QMessageBox::NoIcon, "Alert", "Condition not matched");
		return;
	}
	
	GenGraphDialog dialog;
	if (dialog.exec()) {
		pdgraph_ = new meshtalent::DeformationGraph(*pdmesh_, dialog.nodeNumLineEdit->text().toDouble(),
			dialog.relateNumLineEdit->text().toDouble(),
			dialog.sampleScaleLineEdit->text().toDouble(),
			dialog.deleteRadiusLineEdit->text().toDouble(),
			dialog.ErotLineEdit->text().toDouble(),
			dialog.EregLineEdit->text().toDouble(),
			dialog.EconLineEdit->text().toDouble(),
			dialog.ElenLineEdit->text().toDouble());

			//	pdgraph_ = new meshtalent::DeformationGraph(*pdmesh_, 300, 3, 20, 150, 1, 10, 100, 20); // E_rot, E_reg, E_con, E_len
            
	}

	//pdgraph_ = new meshtalent::DeformationGraph(*pdmesh_, 300, 3, 3, 150, 0.001, 0.001, 100, 0.0001); // E_rot, E_reg, E_con, E_len

}



/*void Geometry3D::ReportMeshInfo()
{
//	SendText("# of vertices: %d,\t", m_mesh.n_vertices());
//	SendText("# of edges : %d,\t", m_mesh.n_edges());
//	SendText("# of faces : %d\n\n", m_mesh.n_faces());
//	SendText("Center of the model : (%10.6f, %10.6f, %10.6f)\n\n", m_modelcenter[0], m_modelcenter[1], m_modelcenter[2]);
//	SendText(" ");
}
*/
