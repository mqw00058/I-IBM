//////////////////////////////////////////////////////////////////////////
// Cluster a given data set (3D point type) according to the distance 
// 1. AgglomerativeClustering algorithm [HTF09]
//
//	$Date: 2011.03.10 $
//  $Created by Min Ki Park (minkp@gist.ac.kr)
//////////////////////////////////////////////////////////////////////////


#ifndef PBM_FEATURE_CLUSTERING_CC
#define PBM_FEATURE_CLUSTERING_CC 

#include <vector>
using namespace std;
using namespace OpenMesh;
// a list of the closest normals (3d vector type)
class Cluster
{
public:
	Cluster()
	{
		num_elements = 0;
	}

	Cluster(int id, OpenMesh::Vec3f val)
	{
		elements.push_back(id);
		values.push_back(val);
		num_elements = 1;
		mean = val;
	}
	~Cluster(){};

	int num_elements;
	vector<int> elements;	//index in a clustering data set
	vector<OpenMesh::Vec3f> values;

	
	// compute cluster value statistics
	OpenMesh::Vec3f mean;
	float variance;


	
	void merge_clsuter(Cluster c){

		// update mean value
		mean = mean * num_elements;

		for(int i=0; i<c.num_elements; ++i){
			elements.push_back(c.elements[i]);
			values.push_back(c.values[i]);
			mean = mean + c.values[i];
			num_elements++;
		}
		mean = mean / num_elements;
	}

private:



};


// Refer to [Weber10] for more detail notation
class AgglomerativeClustering
{

public:
	AgglomerativeClustering(int num_clusters_) : num_clusters(num_clusters_)
	{
		initialize();
	}
	~AgglomerativeClustering(){};
	
	void initialize(){

		// construct a distance table
		d = new float*[num_clusters];

		for(int i=0; i<num_clusters; i++) {
			d[i] = new float [num_clusters];
		}
		for(int i=0; i<num_clusters; i++) {
			for(int j=0; j<num_clusters; j++) {
				d[i][j] = 0.f;
			}
		}
	}

	void set_distance_table(int x, int y, float dist_){

		d[x][y] = dist_;
		d[y][x] = dist_;
	}

	void set_initial_cluster(vector<OpenMesh::Vec3f> d){
	
		for(int i=0; i<num_clusters; ++i){
			Cluster c(i, d[i]);
			clusters.push_back(c);
		}
		datas = d;

	}

	/* compute a distance measure between two clusters */
	float compute_cluster_distance(Cluster c1, Cluster c2){
	
		int nc1 = c1.num_elements;
		int nc2 = c2.num_elements;
		float sum = 0.f;
	
		// using mean value of the distance between every pair
		for(int i=0; i< nc1; ++i)
			for(int j=0; j<nc2; ++j)
				sum += Dxy(c1.elements[i], c2.elements[j]);
		sum *= 1.f/(nc1*nc2);

		return sum;
	}

	/* main procedure of agglomerative clustering */
	void do_clustering(float T_DIST){	// T_DIST: min tolerance distance between elements in one cluster

		int iter = 0;
		const int maxIter = num_clusters;
		

		while (iter++ < maxIter) {

			// find the two clusters with minimum distance
			float closest_dist = 10000.f;
			Cluster c1,c2;
			int closest_c1, closest_c2;

			for(unsigned int i=0; i<clusters.size(); ++i){

				for(unsigned int j=i+1; j<clusters.size(); ++j){

					float dist_c12 = compute_cluster_distance(clusters[i], clusters[j]);
					//0이면 더 이상 볼 필요가 없음
					if(closest_dist > dist_c12){
						closest_dist = dist_c12;
						c1 = clusters[i];
						c2 = clusters[j];
						closest_c1 = i;
						closest_c2 = j;
					}
					if(closest_dist < 0.1f)
					{
						// loop termination!!!
						i = j = clusters.size();
					}
				}
			}
	
			// stop the clustering algorithm if...
			if(closest_dist > T_DIST)
				break;

			// now, we have the closest clusters c1, c2
			c1.merge_clsuter(c2);
			// the order is important c2 -> c1
			clusters.erase(clusters.begin() + closest_c2);
			clusters.erase(clusters.begin() + closest_c1);
			clusters.push_back(c1);
			num_clusters--;
		}
	}


	

	int num_clusters;
	vector<Cluster> clusters;	//clusters (normal set)
	vector<OpenMesh::Vec3f> datas;			//normals

private:
	float** d;

	float Dxy(int x, int y){
		return d[x][y];
	}

};
#endif