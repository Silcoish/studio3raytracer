#ifndef KF_ALGORITHMS_3D_HEADER
#define KF_ALGORITHMS_3D_HEADER

#include "kf/kf_types.h"
#include "kf/kf_math.h"
#include <ostream>

namespace kf
{
   template<typename T>
   void constrain_3d_fixed_dist(T &pos1, T &pos2, float desired_dist, float compensate1, float compensate2)
   {
      T delta = pos2 - pos1;
      float deltalength = (float)sqrt(delta.x*delta.x+delta.y*delta.y+delta.z*delta.z);
      if(deltalength > 0)
      {
         float diff = (deltalength - desired_dist) / deltalength;
         pos1 += delta * compensate1 * diff;
         pos2 -= delta * compensate2 * diff;
      }
   }

   template<typename T>
   void constrain_3d_min_dist(T &pos1, T &pos2, float min_dist, float compensate1, float compensate2)
   {
      T delta = pos2 - pos1;
      float deltalength = (float)sqrt(delta.x*delta.x+delta.y*delta.y+delta.z*delta.z);
      if(deltalength > 0 && deltalength < min_dist)
      {
         float diff = (deltalength - min_dist) / deltalength;
         pos1 += delta * compensate1 * diff;
         pos2 -= delta * compensate2 * diff;
      }
   }

}
#endif
