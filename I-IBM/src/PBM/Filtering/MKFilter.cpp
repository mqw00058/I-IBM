#include "MKFilter.h"



template <class Mesh>
MKFilter<Mesh>::
MKFilter(Mesh& _mesh)
: mesh_(_mesh)
{
	mesh_.add_property(ev_);		// eigenvalue
	mesh_.add_property(e1_);		// eigenvector
	mesh_.add_property(e2_);		// eigenvector
	mesh_.add_property(e3_);		// eigenvector
	mesh_.add_property(feature_candidate_);	// surface saliency
	mesh_.add_property(scale_);		// surface(point) type
	mesh_.add_property(nfeature_);
	mesh_.add_property(fweight_);
	mesh_.add_property(min_fweight_);
	mesh_.add_property(density_);

	initialize();
}

//-----------------------------------------------------------------------------
template <class Mesh>
MKFilter<Mesh>::
~MKFilter()
{
	mesh_.remove_property(ev_);		// eigenvalue
	mesh_.remove_property(e1_);		// eigenvector
	mesh_.remove_property(e2_);		// eigenvector
	mesh_.remove_property(e3_);		// eigenvector
	mesh_.remove_property(feature_candidate_);	// surface saliency
	mesh_.remove_property(scale_);
	mesh_.remove_property(nfeature_);
	mesh_.remove_property(fweight_);
	mesh_.remove_property(min_fweight_);
	mesh_.remove_property(density_);
}

//-----------------------------------------------------------------------------
template <class Mesh>
void MKFilter<Mesh>::initialize()
{
	// geometry property setting
	Mesh::VertexIter	v_it, v_end(mesh_.vertices_end());

	for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it){

		// initialize point set
		point_set.push_back(mesh_.point(v_it));		
	}	
	nPts	= (int)point_set.size();
	ne		= new NE(point_set);

	// for neighborhood
	const int N = 20;
	knn		= new int [N*nPts];
	nn		= new int [nPts];

	for(int i=0; i<nPts; ++i){
		// neighborhood computation
		//nn[i] = ne->k_nearest_neighbor(i, 1, N, knn + i*N);
		ne->k_nearest_neighbor(i, N,  knn + i*N);
	}

	compute_density();
	//compute_feature_map(6);
	delete ne;
}
//-----------------------------------------------------------------------------
template <class Mesh>
void MKFilter<Mesh>::compute_density()
{
	const int	N = 8;	//# of neighbor
	const float h = 0.1;	//bandwidth
	const float h3= h*h*h;
	Point pi, pj;
	float dist;
	VertexHandle vh;

	for(int i=0; i<nPts; ++i){

		vh = mesh_.vertex_handle(i);
		pi = point_set[i];
		float d(0.f);

		for(int j=0; j<N; ++j){
			pj = point_set[ knn[i*20 +j] ];
			dist = (pi-pj).norm();
			d += exp(-dist*dist/h);
		}
		d /= (N*h3);
		mesh_.property(density_, vh) = d;
	}

	delete[] knn;

}

//-----------------------------------------------------------------------------
template <class Mesh>
void MKFilter<Mesh>::compute_feature_map(int k)
{


	double **matT,**matV; 		// matT: voting tensor (3x3), matV: ball votes
	matT = new double* [3];
	matV = new double* [3];
	// Initialize matrix
	for(int i=0; i<3; ++i) {
		matT[i] = new double [3];
		matV[i] = new double [3];
	}

	//const int	k = 8;		//k-nearest neighbor
	int			*knn = new int [k];	//k-nearest neighbor
	int			nn;					//# of neighbor found
	float		avgLen=ne->avgLen;
	float		sigma = avgLen;
	float		range = 2.5f*avgLen;
	point		pi, pj;			//point value
	float		densityMax, density;
	float		l2, vv, weight;
	Vec3f		v;
	VertexHandle vh, vhn;

	//-----------------------------
	// normal tensor voting

	for(int i=0; i<nPts; ++i){

		nn = ne->k_nearest_neighbor(i, range, k, knn);
		pi = point_set[i];
		vh = mesh_.vertex_handle(i);
		densityMax = 0.f;


		// tensor matrix initialization
		for(int ii=0; ii<3; ++ii) {
			for(int jj=0; jj<3; ++jj) {
				matT[ii][jj] = 0;
			}
		}

		// compute the maximum density among neighborhood point
		for(int j=0; j<nn; ++j){
			vhn = mesh_.vertex_handle( knn[j] );
			density = mesh_.property(density_, vhn);
			if(density > densityMax)
				densityMax = density;
		}


		for(int j=0; j<nn; ++j){
			pj = point_set[ knn[j] ];
			vhn = mesh_.vertex_handle( knn[j] );
			density = mesh_.property(density_, vhn);

			// compute the ball vote
			v = pi-pj;
			v.normalize_cond();
			l2 = v.sqrnorm();

			weight = (densityMax/density)*exp(-(l2/sigma));	// vote weight

			// I - w*(v*v_transpose)
			for(int ii=0; ii<3;++ii){
				for(int jj=0; jj<3;++jj){

					vv = v[ii]*v[jj];
					matV[ii][jj] = weight * ( (ii==jj?1:0) - vv/l2 );
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

		mesh_.property(ev_, vh)[0] = ev1;
		mesh_.property(ev_, vh)[1] = ev2;
		mesh_.property(ev_, vh)[2] = ev3;

		e1[0] = A.matV_[0][0]; e1[1] = A.matV_[1][0]; e1[2] = A.matV_[2][0];
		e2[0] = A.matV_[0][1]; e2[1] = A.matV_[1][1]; e2[2] = A.matV_[2][1];
		e3[0] = A.matV_[0][2]; e3[1] = A.matV_[1][2]; e3[2] = A.matV_[2][2];

		mesh_.property(e1_, vh) = e1;
		mesh_.property(e2_, vh) = e2;
		mesh_.property(e3_, vh) = e3;

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
void MKFilter<Mesh>::filtering(float sigma1_, float sigma2_)
{
	int nn;									// # of local neighborhood
	float sigma1_2 = sigma1_*sigma1_;		// spatial std
	float sigma2_2 = sigma2_*sigma2_;		// influence std
	float ws, wc;							// weight
	float sum_w, normalizer;				// filter weight

	Vec3f pi, pj;



	for(int i=0; i<nPts; ++i){


		pi = point_set[i];
		// find neighborhood 

		for(int j=0; j<nn; ++j){


		}


	}


}


template <class Mesh>
void MKFilter<Mesh>::color_coding()
{
	Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
	Mesh::Scalar      min_val(FLT_MAX), max_val(-FLT_MAX), val;
	Mesh::Color       col;

	// put all curvature values into one array
	std::vector<float> values;
	values.reserve(point_set.size());
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{

		values.push_back(mesh_.property(density_, v_it));	
		//values.push_back(mesh_.property(fweight_, v_it));	// surface saliency
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
		val = mesh_.property(density_, v_it);	
		//val = mesh_.property(fweight_, v_it);
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
