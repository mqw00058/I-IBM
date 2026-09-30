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
//   $Revision: 2006.1.18 $
//   $Date: 2005/07/27 10:53:00 $
//   $Created by Hyun Soo Kim and Han Kyun Choi ({hskim, korwairs}@gist.ac.kr)
//   $This class comes from QuadricT.hh 
//                                                                            
//=============================================================================


//=============================================================================
//
//  CLASS ColorQuadricT
//
//=============================================================================

#ifndef OPENMESH_GEOMETRY_COLORQUADRIC_HH
#define OPENMESH_GEOMETRY_COLORQUADRIC_HH


//== INCLUDES =================================================================

#include "Config.hh"
#include <OpenMesh/Core/Geometry/VectorT.hh>
#include <OpenMesh/Core/Utils/GenProg.hh>

//== NAMESPACE ================================================================

namespace OpenMesh { //BEGIN_NS_OPENMESH
namespace Geometry { //BEGIN_NS_GEOMETRY


//== CLASS DEFINITION =========================================================


/** /class ColorQuadricT ColorQuadricT.hh <OSG/Geometry/Types/ColorQuadricT.hh>

    Stores a quadric as a 7x7 symmetrix matrix. Used by the
    error quadric based mesh decimation algorithms.
**/

template <class Scalar,
		  class Vector3Elem = VectorT<Scalar, 3>,
		  class Vector6Elem = VectorT<Scalar, 6>,
          class Vector7Elem = VectorT<Scalar, 7> >
class ColorQuadricT
{
public:
   typedef Scalar           value_type;
   typedef ColorQuadricT<Scalar> type;
   typedef ColorQuadricT<Scalar> Self;
   //   typedef VectorInterface<Scalar, VecStorage3<Scalar> > Vec3;
   //   typedef VectorInterface<Scalar, VecStorage4<Scalar> > Vec4;
   typedef Vector3Elem      Vec3;
   typedef Vector6Elem      Vec6;
   typedef Vector7Elem      Vec7;
   
   /// construct with upper triangle of symmetrix 7x7 matrix
   ColorQuadricT(Scalar _e00, Scalar _e01, Scalar _e02, Scalar _e03, Scalar _e04, Scalar _e05, Scalar _e06, 
					          Scalar _e11, Scalar _e12, Scalar _e13, Scalar _e14, Scalar _e15, Scalar _e16,
							  			   Scalar _e22, Scalar _e23, Scalar _e24, Scalar _e25, Scalar _e26,
													    Scalar _e33, Scalar _e34, Scalar _e35, Scalar _e36,
																     Scalar _e44, Scalar _e45, Scalar _e46,
																			      Scalar _e55, Scalar _e56, 
																						       Scalar _e66)
				: e00(_e00), e01(_e01), e02(_e02), e03(_e03), e04(_e04), e05(_e05), e06(_e06), 
				             e11(_e11), e12(_e12), e13(_e13), e14(_e14), e15(_e15), e16(_e16),
				                        e22(_e22), e23(_e23), e24(_e24), e25(_e25), e26(_e26),
												   e33(_e33), e34(_e34), e35(_e35), e36(_e36),
														      e44(_e44), e45(_e45), e46(_e46),
																		 e55(_e55), e56(_e56), 
																					e66(_e66)
   {}


  /// constructor from given plane equation: ax+by+cz+dr+eg+fb+g=0
   ColorQuadricT( Scalar _a=0.0, Scalar _b=0.0, Scalar _c=0.0, Scalar _d=0.0, Scalar _e=0.0, Scalar _f=0.0, Scalar _g=0.0  )
	   :  e00(_a*_a), e01(_a*_b), e02(_a*_c), e03(_a*_d), e04(_a*_e), e05(_a*_f), e06(_a*_g), 
	                  e11(_b*_b), e12(_b*_c), e13(_b*_d), e14(_b*_e), e15(_b*_f), e16(_b*_g),
	                              e22(_c*_c), e23(_c*_d), e24(_c*_e), e25(_c*_f), e26(_c*_g),
	                                          e33(_d*_d), e34(_d*_e), e35(_d*_f), e36(_d*_g),
	                                                      e44(_e*_e), e45(_e*_f), e46(_e*_g),
	                                                                  e55(_f*_f), e56(_f*_g), 
	                                                                              e66(_g*_g)
   {}


   /// set all entries to zero
   void clear()  { e00 = e01 = e02 = e03 = e04 = e05 = e06 = 
	                     e11 = e12 = e13 = e14 = e15 = e16 =
	                           e22 = e23 = e24 = e25 = e26 =
	                                 e33 = e34 = e35 = e36 =
	                                       e44 = e45 = e46 =
	                                             e55 = e56 = 
	                                                   e66 = 0.0; }

  
   /// add quadrics
   ColorQuadricT<Scalar>& operator+=( const ColorQuadricT<Scalar>& _q )
   {
	   e00 += _q.e00; e01 += _q.e01; e02 += _q.e02; e03 += _q.e03; e04 += _q.e04; e05 += _q.e05; e06 += _q.e06; 
		              e11 += _q.e11; e12 += _q.e12; e13 += _q.e13; e14 += _q.e14; e15 += _q.e15; e16 += _q.e16;
		                             e22 += _q.e22; e23 += _q.e23; e24 += _q.e24; e25 += _q.e25; e26 += _q.e26;
		                                            e33 += _q.e33; e34 += _q.e34; e35 += _q.e35; e36 += _q.e36;
		                                                           e44 += _q.e44; e45 += _q.e45; e46 += _q.e46;
		                                                                          e55 += _q.e55; e56 += _q.e56; 
		                                                                                         e66 += _q.e66;

      return *this;
   }


   /// multiply by scalar
   ColorQuadricT<Scalar>& operator*=( Scalar _s)
   {
	   e00 *= _s; e01 *= _s; e02 *= _s; e03 *= _s; e04 *= _s; e05 *= _s; e06 *= _s; 
				  e11 *= _s; e12 *= _s; e13 *= _s; e14 *= _s; e15 *= _s; e16 *= _s;
					         e22 *= _s; e23 *= _s; e24 *= _s; e25 *= _s; e26 *= _s;
					                    e33 *= _s; e34 *= _s; e35 *= _s; e36 *= _s;
					                               e44 *= _s; e45 *= _s; e46 *= _s;
					                                          e55 *= _s; e56 *= _s; 
					                                                     e66 *= _s;
      return *this;
   }


   /// multiply 7D vector from right: Q*v
   Vec7 operator*(const Vec7& _v) const
   {
	   return Vec7(_v[0]*e00 + _v[1]*e01 + _v[2]*e02 + _v[3]*e03 + _v[4]*e04 + _v[5]*e05 + _v[6]*e06, 
		           _v[0]*e01 + _v[1]*e11 + _v[2]*e12 + _v[3]*e13 + _v[4]*e14 + _v[5]*e15 + _v[6]*e16,
		           _v[0]*e02 + _v[1]*e12 + _v[2]*e22 + _v[3]*e23 + _v[4]*e24 + _v[5]*e25 + _v[6]*e26,
		           _v[0]*e03 + _v[1]*e13 + _v[2]*e23 + _v[3]*e33 + _v[4]*e34 + _v[5]*e35 + _v[6]*e36,
		           _v[0]*e04 + _v[1]*e14 + _v[2]*e24 + _v[3]*e34 + _v[4]*e44 + _v[5]*e45 + _v[6]*e46,
		           _v[0]*e05 + _v[1]*e15 + _v[2]*e25 + _v[3]*e35 + _v[4]*e45 + _v[5]*e55 + _v[6]*e56, 
		           _v[0]*e06 + _v[1]*e16 + _v[2]*e26 + _v[3]*e36 + _v[4]*e46 + _v[5]*e56 + _v[6]*e66;



		  );	          
   }
  

   /// evaluate quadric Q at vector v: v*Q*v
   Scalar operator()(const Vec6 _v) const
   {
      Scalar x(_v[0]), y(_v[1]), z(_v[2]), r(_v[3]), g(_v[4]), b(_v[5]);
	  return e00*x*x + 2.0*e01*x*y + 2.0*e02*x*z + 2.0*e03*x*r + 2.0*e04*x*g + 2.0*e05*x*b + 2.0*e06*x + 
		                   e11*y*y + 2.0*e12*y*z + 2.0*e13*y*r + 2.0*e14*y*g + 2.0*e15*y*b + 2.0*e16*y +
		                                 e22*z*z + 2.0*e23*z*r + 2.0*e24*z*g + 2.0*e25*z*b + 2.0*e26*z +
		                                               e33*r*r + 2.0*e34*r*g + 2.0*e35*r*b + 2.0*e36*r +
		                                                             e44*g*g + 2.0*e45*g*b + 2.0*e46*g +
		                                                                           e55*b*b + 2.0*e56*b + 
		                                                                                         e66;
   }


   /// evaluate quadric Q at vector v: v*Q*v
   Scalar operator()(const Vec7 _v) const
   {
      Scalar x(_v[0]), y(_v[1]), z(_v[2]), w(_v[3]), r(_v[3]), g(_v[4]), b(_v[5]), w(_v[6]);
	  return e00*x*x + 2.0*e01*x*y + 2.0*e02*x*z + 2.0*e03*x*r + 2.0*e04*x*g + 2.0*e05*x*b + 2.0*e06*x*w + 
		                   e11*y*y + 2.0*e12*y*z + 2.0*e13*y*r + 2.0*e14*y*g + 2.0*e15*y*b + 2.0*e16*y*w +
		                                 e22*z*z + 2.0*e23*z*r + 2.0*e24*z*g + 2.0*e25*z*b + 2.0*e26*z*w +
		                                               e33*r*r + 2.0*e34*r*g + 2.0*e35*r*b + 2.0*e36*r*w +
		                                                             e44*g*g + 2.0*e45*g*b + 2.0*e46*g*w +
		                                                                           e55*b*b + 2.0*e56*b*w + 
		                                                                                         e66*w*w;
   }
  

private:

   Scalar e00, e01, e02, e03, e04, e05, e06, 
               e11, e12, e13, e14, e15, e16,
			        e22, e23, e24, e25, e26,
				         e33, e34, e35, e36,
				              e44, e45, e46,
					               e55, e56, 
										e66;
};


/// ColorQuadric using floats
typedef ColorQuadricT<float> ColorQuadricf;

/// ColorQuadric using double
typedef ColorQuadricT<double> ColorQuadricd;


//=============================================================================
} // END_NS_GEOMETRY
} // END_NS_OPENMESH
//============================================================================
#endif // OPENMESH_GEOMETRY_HH defined
//=============================================================================
