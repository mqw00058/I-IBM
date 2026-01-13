#ifndef PBM_FEATURE_COLORTENSORVOTING_CC
#define PBM_FEATURE_COLORTENSORVOTING_CC


#include "ColorTensorVoting.hh"

//== NAMESPACES ===============================================================
namespace PBM {
namespace Feature {



template <class Mesh>
CColorTensorVotingT<Mesh>::CColorTensorVotingT(Mesh& _mesh)
: mesh_(_mesh)
{
	mesh_.add_property(fweight_);
	initialize();
}

template <class Mesh>
CColorTensorVotingT<Mesh>::~CColorTensorVotingT(void)
{
	mesh_.remove_property(fweight_);
}

//-------------------------------------------------------------------------------
template <class Mesh>
void CColorTensorVotingT<Mesh>::initialize()
{
	// color statistics (variance, average distance)
	int		n=0;
	point	p;
	//point	mean(0.f, 0.f, 0.f);
	Vec3uc	c;
	Vec3f	mean(0, 0, 0);
	Vec3f	M2(0.f, 0.f, 0.f);

	nPts = mesh_.n_vertices();
	for(int i=0; i<nPts; ++i){
		VertexHandle vh = mesh_.vertex_handle(i);
		p = vector_cast<point>(mesh_.point(vh));
		c = vector_cast<color>(mesh_.color(vh));
		point_set.push_back( p );
		color_set.push_back( c );

// 		n++;
// 		color delta;
// 		delta = c-mean;
// 		mean = mean + delta/n;
// 		M2 = M2 + delta*(c-mean);

	}
// 	var = M2/n;
// 	var[0] = sqrt(var[0]);
// 	var[1] = sqrt(var[1]);
// 	var[2] = sqrt(var[2]);


	// construct kd-tree
	ne = new PBM::NormalEstimation(point_set);

}

//-------------------------------------------------------------------------------
template <class Mesh>
void CColorTensorVotingT<Mesh>::tensorvoting()
{
	double **matT,**matV; 		// matT: voting tensor (3x3), matV: ball votes
	matT = new double* [3];
	matV = new double* [3];
	// Initialize matrix
	for(int i=0; i<3; ++i) {
		matT[i] = new double [3];
		matV[i] = new double [3];
	}
	//double matI[3][3]={1,0,0,
	//				   0,1,0,
	//	               0,0,1};		//identity matrix

	const int k = 6;		//k-nearest neighbor
	int* knn = new int [k];	//k-nearest neighbor
	int nn;					//# of neighbor found
	float range = 5.f*ne->avgLen;
	point pi, pj;			//point value
	color ci, cj;			//color value

	//-----------------------------
	// normal tensor voting
	
	for(int i=0; i<nPts; ++i){

		nn = ne->k_nearest_neighbor(i, range, k, knn);
		pi = point_set[i];
		ci = color_set[i];

		// tensor matrix initialization
		for(int ii=0; ii<3; ++ii) {
			for(int jj=0; jj<3; ++jj) {
				matT[ii][jj] = 0;
			}
		}

		for(int j=0; j<nn; ++j){
			pj = point_set[ knn[j] ];
			cj = color_set[ knn[j] ];

			// spatial distance
			point d = pi-pj;
			float l = d.norm();

			// weight가 중요함, 현재는 spatial weight만 고려해주고 있음
			float mu = exp(-(l*l/range));
	
			// range distance
			color v = ci-cj;
			float vv;
			float vvnorm = v.sqrnorm();

			//vvnorm = cj.sqrnorm();


			// I - len*(v*v_transpose)
			for(int ii=0; ii<3;++ii){
				for(int jj=0; jj<3;++jj){

					vv = v[ii]*v[jj];
					//matV[ii][jj] = mu * ( ii==jj?1:0 - vv/vvnorm );
					matV[ii][jj] = mu * vv/vvnorm ;
				}
			}
			// integrating the ball votes
			matT[0][0] += matV[0][0];matT[0][1] += matV[0][1];matT[0][2] += matV[0][2];
			matT[1][0] += matV[1][0];matT[1][1] += matV[1][1];matT[1][2] += matV[1][2];
			matT[2][0] += matV[2][0];matT[2][1] += matV[2][1];matT[2][2] += matV[2][2];
		}
		// Eigen-decomposition for analysis
		EigenSolver A;
		A.SolveEigenSystem(3, matT);
		A.SortEigenvectors();
		float ev1, ev2, ev3;	// eigenvalues
		Vec3f e1, e2, e3;		// eigenvectors

		ev1 = A.eigenValue_[0];
		ev2 = A.eigenValue_[1];
		ev3 = A.eigenValue_[2];

		float ss = ev1-ev2;
		float sc = ev2-ev3;
		float sp = ev3;
		float fw = (ev2+ev3)/ev1;

		//feature weight
		mesh_.property(fweight_, mesh_.vertex_handle(i)) = fw;


	}

	delete[] knn;
	delete[] matT;
	delete[] matV;
}

template <class Mesh>
void CColorTensorVotingT<Mesh>::color_coding()
{
	Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
	Mesh::Scalar      min_val(FLT_MAX), max_val(-FLT_MAX), val;
	Mesh::Color       col;

	// put all curvature values into one array
	std::vector<float> values;
	values.reserve(point_set.size());
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{

		values.push_back(mesh_.property(fweight_, v_it));	// surface saliency
	}


	// discard upper and lower 5%
	unsigned int n = values.size()-1;
	unsigned int i = n / 20;
	std::sort(values.begin(), values.end());
	min_val = values[i];
	max_val = values[n-1-i];


	// define uniform color intervalls [v0,v1,v2,v3,v4]
	Mesh::Scalar v0, v1, v2, v3, v4;
	v0 = min_val + 0.0/4.0 * (max_val - min_val);
	v1 = min_val + 1.0/4.0 * (max_val - min_val);
	v2 = min_val + 2.0/4.0 * (max_val - min_val);
	v3 = min_val + 3.0/4.0 * (max_val - min_val);
	v4 = min_val + 4.0/4.0 * (max_val - min_val);



	// map curvatures to colors
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		val = mesh_.property(fweight_, v_it);
		col = Mesh::Color(255,255,255);
		unsigned char u;

		if (val < v0) {	// v0
			col = Mesh::Color(0, 0, 255);
		}
		else if (val > v4) {
			col = Mesh::Color(255, 0, 0);
		}

		else if (val <= v2) {
			if (val <= v1) // [v0, v1]
			{
				u = (unsigned char) (255.0 * (val - v0) / (v1 - v0));
				col = Mesh::Color(0, u, 255);
			}      
			else // ]v1, v2]
			{
				u = (unsigned char) (255.0 * (val - v1) / (v2 - v1));
				col = Mesh::Color(0, 255, 255-u);
			}
		}
		else {
			if (val <= v3) // ]v2, v3]
			{
				u = (unsigned char) (255.0 * (val - v2) / (v3 - v2));
				col = Mesh::Color(u, 255, 0);
			}
			else // ]v3, v4]
			{
				u = (unsigned char) (255.0 * (val - v3) / (v4 - v3));
				col = Mesh::Color(255, 255-u, 0);
			}
		}

		mesh_.set_color(v_it, col);
	}
}
//=============================================================================
} // namespace Feature
} // namespace PBM
//=============================================================================
#endif