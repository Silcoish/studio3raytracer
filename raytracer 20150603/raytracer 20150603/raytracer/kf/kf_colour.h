#ifndef KF_COLOUR_HEADER
#define KF_COLOUR_HEADER

#include "kf/kf_types.h"
#include "kf/kf_math.h"
#include <ostream>

namespace kf
{
   template<typename T>
   class ColourT
   {
   public:
      typedef T type;
      T r;
      T g;
      T b;
      inline ColourT(): r(0),g(0),b(0)
      {
      }
      inline ColourT(const ColourT &c): r(c.r),g(c.g),b(c.b)
      {
      }
      explicit inline ColourT(T rgb): r(rgb),g(rgb),b(rgb)
      {
      }
      template<typename T2>
         explicit inline ColourT(const T2 *p): r(T(p[0])),g(T(p[1])),b(T(p[2]))
      {
      }
      inline ColourT(T rr, T gg, T bb): r(rr),g(gg),b(bb)
      {
      }

      inline T operator [] ( unsigned int i ) const
      {
         return *(&r+i);
      }

      inline T& operator [] ( unsigned int i )
      {
         return *(&r+i);
      }

      inline ColourT operator*=(T c)
      {
         r *= c;
         g *= c;
         b *= c;
         return *this;
      }
      inline ColourT operator/=(T c)
      {
         r /= c;
         g /= c;
         b /= c;
         return *this;
      }
      inline ColourT operator+=(const ColourT &c)
      {
         r += c.r;
         g += c.g;
         b += c.b;
         return *this;
      }
      inline ColourT operator-=(const ColourT &c)
      {
         r -= c.r;
         g -= c.g;
         b -= c.b;
         return *this;
      }
      inline ColourT operator*=(const ColourT &c)
      {
         r *= c.r;
         g *= c.g;
         b *= c.b;
         return *this;
      }
      inline ColourT operator/=(const ColourT &c)
      {
         r /= c.r;
         g /= c.g;
         b /= c.b;
         return *this;
      }
      inline ColourT operator=(const T *p)
      {
         r = p[0];
         g = p[1];
         b = p[2];
         return *this;
      }
      inline ColourT operator=(const ColourT<T> &c)
      {
         r = c.r;
         g = c.g;
         b = c.b;
         return *this;
      }
	  inline ColourT normalise()
	  {
		  T l;
		  ColourT temp;
		  l = T(sqrt(r*r + g*g + b*b));
		  if (l != 0.0)
		  {
			  temp.set(r / l, g / l, b / l);
		  }
		  return temp;
	  }
	  inline ColourT saturate()
	  {
		  ColourT temp(*this);
		  if (temp.r < 0)
			  temp.r = 0;
		  if (temp.r > 1)
			  temp.r = 1;
		  if (temp.g < 0)
			  temp.g = 0;
		  if (temp.g > 1)
			  temp.g = 1;
		  if (temp.b < 0)
			  temp.b = 0;
		  if (temp.b > 1)
			  temp.b = 1;

		  return temp;
	  }
	  inline void toSRGB()
	  {
		  if (r <= 0.0031308)
			  r = r * 12.92;
		  else
			  r = 1.055 * pow(r, 1.0 / 2.4) - 0.055;
		  if (g <= 0.0031308)
			  g = g * 12.92;
		  else
			  g = 1.055 * pow(g, 1.0 / 2.4) - 0.055;
		  if (b <= 0.0031308)
			  b = b * 12.92;
		  else
			  b = 1.055 * pow(b, 1.0 / 2.4) - 0.055;
	  }
	  inline void toLinear()
	  {
		  if (r <= 0.04045)
			  r = r / 12.92;
		  else
			  r = pow((r + 0.055) / 1.055, 2.4);
		  if (g <= 0.04045)
			  g = g / 12.92;
		  else
			  g = pow((g + 0.055) / 1.055, 2.4);
		  if (b <= 0.04045)
			  b = b / 12.92;
		  else
			  b = pow((b + 0.055) / 1.055, 2.4);
	  }

	  inline T length() const
      {
         return sqrt(r*r+g*g+b*b);
      }
      inline T length_squared() const
      {
         return r*r+g*g+b*b;
      }
      inline ColourT swiz(int comp1 = 0, int comp2 = 1, int comp3 = 2) const
      {
         return ColourT( comp1 == 0 ? r : (comp1 == 1 ? g : (comp1 == 2 ? b : 0)),
            comp2 == 0 ? r : (comp2 == 1 ? g : (comp2 == 2 ? b : 0)),
            comp3 == 0 ? r : (comp3 == 1 ? g : (comp3 == 2 ? b : 0)));
      }
      inline void set(const ColourT &value)
      {
         r = value.r;
         g = value.g;
         b = value.b;
      }
      inline void set(T rgb)
      {
         r = g = b = rgb;
      }
      inline void set(T rr, T gg, T bb)
      {
         r = rr;
         g = gg;
         b = bb;
      }
      template<typename T2>
         inline void set(const T2 *p)
      {
         r = T(p[0]);
         g = T(p[1]);
         b = T(p[2]);
      }
      inline static T dot(const ColourT &c1, const ColourT &c2)
      {
         return c1.r * c2.r + c1.g * c2.g + c1.b * c2.b;
      }
      inline static ColourT cross(const ColourT &c1, const ColourT &c2)
      {
         return ColourT(c1.g * c2.b - c1.b * c2.g,
            c1.b * c2.r - c1.r * c2.b,
            c1.r * c2.g - c1.g * c2.r);
      }

      inline T dot(const ColourT &c)
      {
         return r * c.r + g * c.g + b * c.b;
      }
      inline ColourT cross(const ColourT &c)
      {
         return ColourT(g * c.b - b * c.g,
            b * c.r - r * c.b,
            r * c.g - g * c.r);
      }

      inline friend std::ostream &operator<<(std::ostream &o, const ColourT &c)
      {
         o << "<" << c.r << ", " << c.g << ", " << c.b << ">";
         return o;
      }

      inline friend ColourT operator*(const ColourT &c,const T &cal)
      {
         return ColourT(c.r*cal,c.g*cal,c.b*cal);
      }
      inline friend ColourT operator*(const T &cal,const ColourT &c)
      {
         return ColourT(cal*c.r, cal*c.g, cal*c.b);
      }
      inline friend ColourT operator*(const ColourT &c1,const ColourT &c2)
      {
         return ColourT(c1.r*c2.r,c1.g*c2.g,c1.b*c2.b);
      }

      inline friend ColourT operator+(const ColourT &c,const T &cal)
      {
         return ColourT(c.r+cal,c.g+cal,c.b+cal);
      }
      inline friend ColourT operator+(const T &cal,const ColourT &c)
      {
         return ColourT(cal+c.r, cal+c.g, cal+c.b);
      }
      inline friend ColourT operator+(const ColourT &c1,const ColourT &c2)
      {
         return ColourT(c1.r+c2.r,c1.g+c2.g,c1.b+c2.b);
      }

      inline friend ColourT operator-(const ColourT &c,const T &cal)
      {
         return ColourT(c.r-cal,c.g-cal,c.b-cal);
      }
      inline friend ColourT operator-(const T &cal,const ColourT &c)
      {
         return ColourT(cal-c.r, cal-c.g, cal-c.b);
      }
      inline friend ColourT operator-(const ColourT &c1,const ColourT &c2)
      {
         return ColourT(c1.r-c2.r,c1.g-c2.g,c1.b-c2.b);
      }

      inline friend ColourT operator/(const ColourT &c,const T &cal)
      {
         return ColourT(c.r/cal,c.g/cal,c.b/cal);
      }
      inline friend ColourT operator/(const T &cal,const ColourT &c)
      {
         return ColourT(cal/c.r, cal/c.g, cal/c.b);
      }
      inline friend ColourT operator/(const ColourT &c1,const ColourT &c2)
      {
         return ColourT(c1.r/c2.r,c1.g/c2.g,c1.b/c2.b);
      }
      inline ColourT &operator=(T c)
      {
         r = c;
         g = c;
         b = c;
         return *this;
      }
      inline ColourT &operator,(T c)
      {
         g=b;
         b=c;
         return *this;
      }
   
      KF_API static const ColourT<T> BLACK;
	  KF_API static const ColourT<T> RED;
	  KF_API static const ColourT<T> GREEN;
	  KF_API static const ColourT<T> BLUE;
	  KF_API static const ColourT<T> WHITE;
   };

   typedef ColourT<float>  Colourf;
   typedef ColourT<double> Colourd;
   typedef ColourT<float>  Colour;

   template <typename T1, typename T2>
   T1 convertColour(const T2 &c)
   {
      T1 result;
      result.r = c.r;
      result.g = c.g;
      result.b = c.b;
      return result;
   }

   template<typename T>
   inline ColourT<T> normalise(const ColourT<T> &c)
   {
      T l;
      ColourT<T> temp;
      l = c.length();

      if(l != 0.0)
      {
         return ColourT<T>(c.r / l, c.g / l, c.b / l);
      }
      return ColourT<T>(0, 0, 0);
   }

}

#endif

