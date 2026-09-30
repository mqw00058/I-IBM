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
//   $Date: 2006.12.05 $
//   $Created by Hyun Soo Kim (hskim@gist.ac.kr)
//                                                                            
//=============================================================================

/** \file ColorConversionT.cc
    
 */

//=============================================================================
//
//  CLASS ColorConversionT - IMPLEMENTATION
//
//=============================================================================

#define OPENMESH_ColorConversion_CC

//== INCLUDES =================================================================

#include <OpenMesh/Tools/Color/ColorConversionT.hh>

//== NAMESPACES ===============================================================


namespace OpenMesh {
namespace ColorConversion {


//== IMPLEMENTATION ==========================================================


template <class Mesh>
ColorConversionT<Mesh>::
ColorConversionT(Mesh& _mesh)
  : mesh_(_mesh)
{
	mesh_.add_property(xyz_);
	mesh_.add_property(lab_);	
	mesh_.add_property(RGB_);
}

//-----------------------------------------------------------------------------


template <class Mesh>
ColorConversionT<Mesh>::
~ColorConversionT()
{
	mesh_.remove_property(xyz_);
	mesh_.remove_property(lab_);
	mesh_.remove_property(RGB_);
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorConversionT<Mesh>::initialize()
{
	typename Mesh::VertexIter			v_it, v_end(mesh_.vertices_end());
	for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		mesh_.property(RGB_, v_it) = mesh_.color(v_it);
	}
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorConversionT<Mesh>::RGB2XYZ()
{
	typename Mesh::VertexIter			v_it, v_end(mesh_.vertices_end());
	for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		RGB2XYZ(v_it);	
	}
	
}

//-----------------------------------------------------------------------------					

template <class Mesh>
void ColorConversionT<Mesh>::RGB2XYZ(VertexHandle _vh)
{
	Vec3 _RGB;
	Vec3 _XYZ;
	double _R, _G, _B;
	double _X, _Y, _Z;

	_RGB = vector_cast<Vec3>(mesh_.color(_vh));

	_R = _RGB[0]/255.0f;
	_G = _RGB[1]/255.0f;
	_B = _RGB[2]/255.0f;

	double _r, _g, _b;

	// sRGB is used
	if(_R <= 0.04045)
		_r = _R/12.92;
	else
		_r = (double)pow((_R+0.055)/1.055, (double)2.4f);

	if(_G <= 0.04045)
		_g = _G/12.92;
	else
		_g = (double)pow((_G+0.055)/1.055, (double)2.4f);

	if(_B <= 0.04045)
		_b = _B/12.92;
	else
		_b = (double)pow((_B+0.055)/1.055, (double)2.4f);

	_X =  0.412424*_r +  0.357579*_g +  0.180464*_b;
	_Y =  0.212656*_r +  0.715158*_g + 0.0721856*_b;
	_Z = 0.0193324*_r +  0.119193*_g +  0.950444*_b;

	_XYZ[0] = _X; _XYZ[1] = _Y; _XYZ[2] = _Z;

	mesh_.property(xyz_, _vh) = _XYZ;
}

//-----------------------------------------------------------------------------

template <class Mesh>
void ColorConversionT<Mesh>::XYZ2Lab()
{
	typename Mesh::VertexIter			v_it, v_end(mesh_.vertices_end());
	for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		XYZ2Lab(v_it);	
	}
}

//-----------------------------------------------------------------------------					

template <class Mesh>
void ColorConversionT<Mesh>::XYZ2Lab(VertexHandle _vh)
{
	Vec3 _XYZ;
	Vec3 _Lab;

	double _X, _Y, _Z;
	double _xr, _yr, _zr;
	double _Xr, _Yr, _Zr;
	double _L, _a, _b;
	double _fx, _fy, _fz;

	double _eps, _kappa;
	_eps = 0.008856; _kappa = 903.3;

	_XYZ = mesh_.property(xyz_, _vh);

	_X = _XYZ[0];
	_Y = _XYZ[1];
	_Z = _XYZ[2];

	// Using D65 Light 
	_Xr = 0.950468;
	_Yr = 0.999999;
	_Zr = 1.088970;

	_xr = _X / _Xr;
	_yr = _Y / _Yr;
	_zr = _Z / _Zr;

	if(_xr > _eps)
		_fx = (double)pow((double)_xr, (double)(1.0f/3.0f));
	else
		_fx = (_kappa*_xr+16.0f)/116.0f;

	if(_yr > _eps)
		_fy = (double)pow((double)_yr, (double)(1.0f/3.0f));
	else
		_fy = (_kappa*_yr+16.0f)/116.0f;

	if(_zr > _eps)
		_fz = (double)pow((double)_zr, (double)(1.0f/3.0f));
	else
		_fz = (_kappa*_zr+16.0f)/116.0f;

	_L = 116.0f*_fy - 16.0f;
	_a = 500.0f*(_fx-_fy);
	_b = 200.0f*(_fy-_fz);

	_Lab[0] = _L; _Lab[1] = _a; _Lab[2] = _b;

	mesh_.property(lab_, _vh) = _Lab;
}

//-----------------------------------------------------------------------------
// Convert Lab to XYZ for all vertices

template <class Mesh>
void ColorConversionT<Mesh>::Lab2XYZ()
{
	typename Mesh::VertexIter			v_it, v_end(mesh_.vertices_end());
	for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		Lab2XYZ(v_it);	
	}
}
//-----------------------------------------------------------------------------

// Convert Lab to XYZ at a vertex
template <class Mesh>
void ColorConversionT<Mesh>::Lab2XYZ(VertexHandle _vh)
{
	Vec3 _XYZ;
	Vec3 _Lab;
	
	double Xr, Yr, Zr;
	double _xr, _yr, _zr;
	double _L, _a, _b;

	double _eps, _kappa;
	_eps = 0.008856; _kappa = 903.3;

	double _fx, _fy, _fz;

	// Using D65 Light 
	Xr = 0.950468;
	Yr = 0.999999;
	Zr = 1.088970;

	_Lab = mesh_.property(lab_, _vh);

	_L = _Lab[0]; _a = _Lab[1]; _b = _Lab[2];

	if( _L > _eps*_kappa )
		_yr = (double)pow( (double)((_L+16.0f)/116.0f), (double)(3.0f));
	else
		_yr = _L/_kappa;

	if(_yr > _eps)
		_fy = (_L+16)/116;
	else
		_fy = (_kappa*_yr+16)/116;

	_fx = _a/500.0f + _fy;
	_fz = _fy - _b/200.0f;

	if( (double)pow((double)_fx, (double)(3.0f)) > _eps )
		_xr = (double)pow((double)_fx, (double)(3.0f));
	else
		_xr = (116.0f*_fx-16.0f)/_kappa;

	if( (double)pow((double)_fz, (double)(3.0f)) > _eps )
		_zr = (double)pow((double)_fz, (double)(3.0f));
	else
		_zr = (116.0f*_fz-16.0f)/_kappa;

	_XYZ[0] = _xr*Xr;
	_XYZ[1] = _yr*Yr;
	_XYZ[2] = _zr*Zr;

	mesh_.property(xyz_, _vh) = _XYZ;
}

//-----------------------------------------------------------------------------

// Convert XYZ to RGB for all vertices
template <class Mesh>
void ColorConversionT<Mesh>::XYZ2RGB()
{
	typename Mesh::VertexIter			v_it, v_end(mesh_.vertices_end());
	for(v_it=mesh_.vertices_begin(); v_it!=v_end; ++v_it)
	{
		XYZ2RGB(v_it);	
	}

}

//-----------------------------------------------------------------------------

// Convert XYZ to RGB at a vertex
template <class Mesh>
void ColorConversionT<Mesh>::XYZ2RGB(VertexHandle _vh)
{
	Color _RGB;
	Vec3 _XYZ;
	double _R, _G, _B;
	double _X, _Y, _Z;
		
	double _r, _g, _b;

	_XYZ = mesh_.property(xyz_, _vh);

	_X = _XYZ[0]; _Y = _XYZ[1]; _Z = _XYZ[2];

	_r =   3.24071*_X -  1.53726*_Y -  0.498571*_Z;
	_g = -0.969258*_X +  1.87599*_Y + 0.0415557*_Z;
	_b = 0.0556352*_X - 0.203996*_Y +   1.05707*_Z;

	if(_r <= 0.0031308)
		_R = 12.92*_r;
	else
		_R = 1.055*(double)pow((double)_r, (double)(1.0f/2.4f) ) - 0.055;

	if(_g <= 0.0031308)
		_G = 12.92*_g;
	else
		_G = 1.055*(double)pow((double)_g, (double)(1.0f/2.4f) ) - 0.055;

	if(_b <= 0.0031308)
		_B = 12.92*_b;
	else
		_B = 1.055*(double)pow((double)_b, (double)(1.0f/2.4f) ) - 0.055;

	_R *= 255.0f; _G *= 255.0f; _B *= 255.0f;
	_R = __max(0, __min(255.0f, _R));
	_G = __max(0, __min(255.0f, _G));
	_B = __max(0, __min(255.0f, _B));

	_RGB[0] = (unsigned char)_R; _RGB[1] = (unsigned char)_G; _RGB[2] = (unsigned char)_B;

	mesh_.property(RGB_, _vh) = _RGB;
}		

//-----------------------------------------------------------------------------

template <class Mesh>
double ColorConversionT<Mesh>::ColorDiff(VertexHandle _vh1, VertexHandle _vh2)
{
	Vec3 _Lab1, _Lab2;
	_Lab1 = mesh_.property(lab_, _vh1);
	_Lab2 = mesh_.property(lab_, _vh2);

	return (double)(_Lab1-_Lab2).norm();	
}

//-----------------------------------------------------------------------------

// Set the lab value of a vertex
template <class Mesh>
void ColorConversionT<Mesh>::Set_Lab(VertexHandle _vh, Vec3 _lab)		
{
	mesh_.property(lab_, _vh) = _lab;
}
//-----------------------------------------------------------------------------

//=============================================================================
} // namespace ColorConversion
} // namespace OpenMesh
//=============================================================================
