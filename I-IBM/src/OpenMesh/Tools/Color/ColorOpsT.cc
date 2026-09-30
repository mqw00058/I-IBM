//=============================================================================
//                                                                            
//                               OpenMesh                                     
//        Copyright (C) 2003 by Computer Graphics Group, RWTH Aachen          
//                           www.openmesh.org                                 
//                                                                            
//-----------------------------------------------------------------------------
//                                                                            
//                                License                                     
//                                                                            
//   This library is free software; you can redistribute it and/or modify it 
//   under the terms of the GNU Library General Public License as published  
//   by the Free Software Foundation, version 2.                             
//                                                                             
//   This library is distributed in the hope that it will be useful, but       
//   WITHOUT ANY WARRANTY; without even the implied warranty of                
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
//   Library General Public License for more details.                          
//                                                                            
//   You should have received a copy of the GNU Library General Public         
//   License along with this library; if not, write to the Free Software       
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                 
//                                                                            
//-----------------------------------------------------------------------------
//                                                                            
//   $Revision: 2006.7.5 $
//   $Date: 2006/01/18 10:53:00 $
//   $Created by Hyun Soo Kim (hskim@gist.ac.kr)
//                                                                            
//=============================================================================

/** \file ColorOpsT.cc
    
 */

//=============================================================================
//
//  CLASS ColorOpsT - IMPLEMENTATION
//
//=============================================================================

#define OPENMESH_COLOROPST_C

//== INCLUDES =================================================================

#include <OpenMesh/Tools/Color/ColorOpsT.hh>

//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace ColorOps {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
ColorOpsT<Mesh>::
ColorOpsT(Mesh& _mesh)
  : mesh_(_mesh)
{
	mesh_.add_property(new_color_);


}

//-----------------------------------------------------------------------------


template <class Mesh>
ColorOpsT<Mesh>::
~ColorOpsT()
{
	mesh_.remove_property(new_color_);

}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorOpsT<Mesh>::initialize()
{

}

//-----------------------------------------------------------------------------


template <class Mesh>
void ColorOpsT<Mesh>::SplitChannelR()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.property(new_color_, v_it)[0] = mesh_.color(v_it)[0];	
		mesh_.property(new_color_, v_it)[1] = 0;
		mesh_.property(new_color_, v_it)[2] = 0;
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorOpsT<Mesh>::SplitChannelG()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.property(new_color_, v_it)[0] = 0;
		mesh_.property(new_color_, v_it)[1] = mesh_.color(v_it)[1];	
		mesh_.property(new_color_, v_it)[2] = 0;
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorOpsT<Mesh>::SplitChannelB()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.property(new_color_, v_it)[0] = 0;
		mesh_.property(new_color_, v_it)[1] = 0;	
		mesh_.property(new_color_, v_it)[2] = mesh_.color(v_it)[2];	
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorOpsT<Mesh>::RGB_To_Gray()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.property(new_color_, v_it)[0] = 0.3f*mesh_.color(v_it)[0] + 0.59f*mesh_.color(v_it)[1] + 0.11f*mesh_.color(v_it)[2];
		mesh_.property(new_color_, v_it)[1] = 0.3f*mesh_.color(v_it)[0] + 0.59f*mesh_.color(v_it)[1] + 0.11f*mesh_.color(v_it)[2];
		mesh_.property(new_color_, v_it)[2] = 0.3f*mesh_.color(v_it)[0] + 0.59f*mesh_.color(v_it)[1] + 0.11f*mesh_.color(v_it)[2];
	}	
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorOpsT<Mesh>::R_To_Gray()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.property(new_color_, v_it)[0] = mesh_.color(v_it)[0];	
		mesh_.property(new_color_, v_it)[1] = mesh_.color(v_it)[0];
		mesh_.property(new_color_, v_it)[2] = mesh_.color(v_it)[0];
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorOpsT<Mesh>::G_To_Gray()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.property(new_color_, v_it)[0] = mesh_.color(v_it)[1];
		mesh_.property(new_color_, v_it)[1] = mesh_.color(v_it)[1];	
		mesh_.property(new_color_, v_it)[2] = mesh_.color(v_it)[1];
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorOpsT<Mesh>::B_To_Gray()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.property(new_color_, v_it)[0] = mesh_.color(v_it)[2];
		mesh_.property(new_color_, v_it)[1] = mesh_.color(v_it)[2];	
		mesh_.property(new_color_, v_it)[2] = mesh_.color(v_it)[2];	
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorOpsT<Mesh>::UpdateColor()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.set_color(v_it, mesh_.property(new_color_, v_it));
	}
}

//-----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
// Begin to implement from 2005.8.20.
// Image-processing : Soften
// valence(n) : valence of vertex
//
// Soften Mask
//       1      1
//        \    /
//	       \  /
//          \/
//	1 ----- 1------ 1       * 1/(n+1)
//          /\
//         /  \
//        /    \
//       1      1
///////////////////////////////////////////////////////////////////////////////
template <class Mesh>
void ColorOpsT<Mesh>::Soften()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
	typename Mesh::VertexVertexIter  vv_it;

	int i, dest_color[3];
	unsigned int valence;

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		valence = 0;

		for(i=0;i<3;i++)
			dest_color[i] = 0;
		
		for(vv_it = mesh_.vv_iter( v_it );vv_it; ++vv_it)
		{
			for(i=0;i<3;i++)
				dest_color[i] += (unsigned char)mesh_.color(vv_it)[i];
			
			valence++;
		}
		
		for(i=0;i<3;i++)
		{
			dest_color[i] += (unsigned char)mesh_.color(v_it)[i];
			dest_color[i] = __max(0, __min(255, dest_color[i]/(valence+1)));
			mesh_.property(new_color_, v_it)[i] = dest_color[i];
		}
	}
}

//-----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
// Begin to implement from 2005.8.20.
// Image-processing : Sharpen
// valence(n) : valence of vertex
//
// Sharpen Mask
//      -1      -1
//        \    /
//	       \  /
//          \/
//	-1 -----n+1------ -1       * 1/(n+1)
//          /\
//         /  \
//        /    \
//      -1      -1
///////////////////////////////////////////////////////////////////////////////
template <class Mesh>
void ColorOpsT<Mesh>::Sharpen()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
	typename Mesh::VertexVertexIter  vv_it;

	int i, dest_color[3];
	unsigned int valence;

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		valence = 0;

		for(i=0;i<3;i++)
			dest_color[i] = 0;
		
		for(vv_it = mesh_.vv_iter( v_it );vv_it; ++vv_it)
		{
			for(i=0;i<3;i++)
				dest_color[i] -= (unsigned char)mesh_.color(vv_it)[i];
			
			valence++;
		}
		
		for(i=0;i<3;i++)
		{
			dest_color[i] += (valence+1) * (unsigned char)mesh_.color(v_it)[i];
			dest_color[i] = __max(0, __min(255, dest_color[i]));
			mesh_.property(new_color_, v_it)[i] = dest_color[i];
		}
	}
}

//-----------------------------------------------------------------------------

// 현재 필터로 적용할 마스크가 적당하지 않음
template <class Mesh>
void ColorOpsT<Mesh>::Emboss()
{
	
}

//-----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
// Begin to implement from 2005.8.20.
// Image-processing : Edge Detect
// valence(n) : valence of vertex
//
// Edge Detect Mask
//      -1      -1
//        \    /
//	       \  /
//          \/
//	-1 ----- n------ -1       * 1/(n+1)
//          /\
//         /  \
//        /    \
//      -1      -1
///////////////////////////////////////////////////////////////////////////////

template <class Mesh>
void ColorOpsT<Mesh>::EdgeDetect()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
	typename Mesh::VertexVertexIter  vv_it;

	int i, dest_color[3];
	unsigned int valence;

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		valence = 0;

		for(i=0;i<3;i++)
			dest_color[i] = 0;
		
		for(vv_it = mesh_.vv_iter( v_it );vv_it; ++vv_it)
		{
			for(i=0;i<3;i++)
				dest_color[i] -= (unsigned char)mesh_.color(vv_it)[i];

			valence++;
		}
		
		for(i=0;i<3;i++)
		{
			dest_color[i] += (valence) * ( (unsigned char)mesh_.color(v_it)[i] );
			dest_color[i] = __max(0, __min(255, dest_color[i]));
			mesh_.property(new_color_, v_it)[i] = dest_color[i];
		}
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorOpsT<Mesh>::Inverse()
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	int i;
	
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		for(i=0;i<3;i++)
			mesh_.property(new_color_, v_it)[i] = (unsigned char)255 - (unsigned char) mesh_.color(v_it)[i];
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorOpsT<Mesh>::Brightness(int _brightness)
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	int i;
	
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		for(i=0;i<3;i++)
			mesh_.property(new_color_, v_it)[i] = (unsigned char)__max(0, __min(255, (unsigned char) mesh_.color(v_it)[i] + _brightness));
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorOpsT<Mesh>::Contrast(int _contrast)
{
	typename Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());

	int i;
	
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		for(i=0;i<3;i++)
			mesh_.property(new_color_, v_it)[i] = (unsigned char)__max(0, __min(255, (unsigned char) mesh_.color(v_it)[i] + _contrast));
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorOpsT<Mesh>::Bilateral(double _closeness, double _similarity)
{
	typename Mesh::VertexIter			v_it, v_end(mesh_.vertices_end());
	typename Mesh::VertexVertexIter		vv_it;
	typename Mesh::EdgeIter				e_it, e_end(mesh_.edges_end());

	typename Mesh::VertexHandle			vh0, vh1;	// two vertices of an edge
	typename Mesh::HalfedgeHandle		heh;

	Vec3								v0, v1;
	Color								c0, c1;

	OpenMesh::VPropHandleT<Vec3>		_result_Lab;
	
	mesh_.add_property(_result_Lab);
	
	// CIE Lab Color space
	ColorConversion		CC(mesh_);
	CC.initialize();
	CC.RGB2XYZ();
	CC.XYZ2Lab();

	// Calculate average of the edage length and color difference
	double _aveEdgeLength(0.0f), _aveColorDiff(0.0f);
	Vec3   _sum;
	Vec3	_zero(0, 0, 0);
	double _normalizer;			
	double _geo_dist, _color_dist;		// geometric and color distance
	double _wc, _ws;					// weights of closeness and similarity

	for (e_it=mesh_.edges_begin(); e_it!=e_end; ++e_it)
	{
		heh = mesh_.halfedge_handle(e_it, 0);
		vh0 = mesh_.from_vertex_handle(heh);
		vh1 = mesh_.to_vertex_handle(heh);
		
		v0[0] = mesh_.point(vh0)[0]; v0[1] = mesh_.point(vh0)[1]; v0[2] = mesh_.point(vh0)[2];
		c0[0] = mesh_.color(vh0)[0]; c0[1] = mesh_.color(vh0)[1]; c0[2] = mesh_.color(vh0)[2];
			
		v1[0] = mesh_.point(vh1)[0]; v1[1] = mesh_.point(vh1)[1]; v1[2] = mesh_.point(vh1)[2];
		c1[0] = mesh_.color(vh1)[0]; c1[1] = mesh_.color(vh1)[1]; c1[2] = mesh_.color(vh1)[2];

		_aveEdgeLength	+= (v0-v1).length();
		_aveColorDiff	+= CC.ColorDiff(vh0, vh1);
	}

	_aveEdgeLength	/= mesh_.n_edges();
	_aveColorDiff	/= mesh_.n_edges();

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		_sum = _zero;
		_normalizer = 0.0f;

		vh0 = v_it.handle();

		v0[0] = mesh_.point(vh0)[0]; v0[1] = mesh_.point(vh0)[1]; v0[2] = mesh_.point(vh0)[2];
		c0[0] = mesh_.color(vh0)[0]; c0[1] = mesh_.color(vh0)[1]; c0[2] = mesh_.color(vh0)[2];

		for(vv_it=mesh_.vv_iter(v_it); vv_it; ++vv_it)
		{
			vh1 = vv_it.handle();			
				
			v1[0] = mesh_.point(vh1)[0]; v1[1] = mesh_.point(vh1)[1]; v1[2] = mesh_.point(vh1)[2];
			c1[0] = mesh_.color(vh1)[0]; c1[1] = mesh_.color(vh1)[1]; c1[2] = mesh_.color(vh1)[2];

			_geo_dist	= (v0-v1).length();
			_color_dist = CC.ColorDiff(vh0, vh1);

			_wc = exp(-(_geo_dist*_geo_dist)     / (2*_aveEdgeLength*_aveEdgeLength*_closeness*_closeness));
			_ws = exp(-(_color_dist*_color_dist) / (2*_aveColorDiff*_aveColorDiff*_similarity*_similarity));
			
			_sum += _wc*_ws*CC.Lab(vh1);
			_normalizer += _wc*_ws;
		}

		mesh_.property(_result_Lab, v_it) = _sum/_normalizer;		
	}

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		CC.Set_Lab(v_it, mesh_.property(_result_Lab, v_it));
	}

	CC.Lab2XYZ();
	CC.XYZ2RGB();

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.property(new_color_, v_it) = CC.NewRGB(v_it);
	}

	mesh_.remove_property(_result_Lab);
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorOpsT<Mesh>::BilateralRGB(double _closeness, double _similarity)
{
	typename Mesh::VertexIter			v_it, v_end(mesh_.vertices_end());
	typename Mesh::VertexVertexIter		vv_it;
	typename Mesh::EdgeIter				e_it, e_end(mesh_.edges_end());

	typename Mesh::VertexHandle			vh0, vh1;	// two vertices of an edge
	typename Mesh::HalfedgeHandle		heh;

	Vec3								v0, v1;		// position of a vertex
	Vec3								c0, c1;		// color of a vertex
	
	// Calculate average of the edage length and color difference
	double _aveEdgeLength(0.0f), _aveColorDiff(0.0f);
	Vec3   _sum;
	Vec3	_zero(0, 0, 0);
	double _normalizer;			
	double _geo_dist, _color_dist;		// geometric and color distance
	double _wc, _ws;					// weights of closeness and similarity
	Color  _resultRGB;

	for (e_it=mesh_.edges_begin(); e_it!=e_end; ++e_it)
	{
		heh = mesh_.halfedge_handle(e_it, 0);
		vh0 = mesh_.from_vertex_handle(heh);
		vh1 = mesh_.to_vertex_handle(heh);
		
		v0 = vector_cast<Vec3>(mesh_.point(vh0));
		c0 = vector_cast<Vec3>(mesh_.color(vh0));

		v1 = vector_cast<Vec3>(mesh_.point(vh1));
		c1 = vector_cast<Vec3>(mesh_.color(vh1));

		_aveEdgeLength	+= (v0-v1).length();
		_aveColorDiff	+= (c0-c1).length();
	}

	_aveEdgeLength	/= mesh_.n_edges();
	_aveColorDiff	/= mesh_.n_edges();

	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		_sum = _zero;
		_normalizer = 0.0f;

		vh0 = v_it.handle();

		v0 = vector_cast<Vec3>(mesh_.point(vh0));
		c0 = vector_cast<Vec3>(mesh_.color(vh0));

		for(vv_it=mesh_.vv_iter(v_it); vv_it; ++vv_it)
		{
			vh1 = vv_it.handle();			
				
			v1 = vector_cast<Vec3>(mesh_.point(vh1));
			c1 = vector_cast<Vec3>(mesh_.color(vh1));

			_geo_dist	= (v0-v1).length();
			_color_dist = (c0-c1).length();
			_wc = exp(-(_geo_dist*_geo_dist)     / (2*_aveEdgeLength*_aveEdgeLength*_closeness*_closeness));
			_ws = exp(-(_color_dist*_color_dist) / (2*_aveColorDiff*_aveColorDiff*_similarity*_similarity));
			
			_sum += _wc*_ws*c1;
			_normalizer += _wc*_ws;
		}

		_sum = _sum/_normalizer;

		_resultRGB[0] = (unsigned char)(__max(0, __min(255.0f, _sum[0])));
		_resultRGB[1] = (unsigned char)(__max(0, __min(255.0f, _sum[1])));
		_resultRGB[2] = (unsigned char)(__max(0, __min(255.0f, _sum[2])));

		mesh_.property(new_color_, v_it) = _resultRGB;
	}

}

template <class Mesh>
void ColorOpsT<Mesh>::color_coding()
{
	Mesh::VertexIter  v_it, v_end(mesh_.vertices_end());
	Mesh::Scalar      min_val(FLT_MAX), max_val(-FLT_MAX), val;
	Mesh::Color       col;

	// put all curvature values into one array
	std::vector<float> values;
	for (v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		col = mesh_.property(new_color_, v_it);
		values.push_back(col[0]+col[1]+col[2]);	// surface saliency
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
		val = mesh_.property(new_color_, v_it)[0]+mesh_.property(new_color_, v_it)[1]+mesh_.property(new_color_, v_it)[2];
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

//-----------------------------------------------------------------------------

//=============================================================================
} // namespace ColorOps
} // namespace OpenMesh
//=============================================================================
