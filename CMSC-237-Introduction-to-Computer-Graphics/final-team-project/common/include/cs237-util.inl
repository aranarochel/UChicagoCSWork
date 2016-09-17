/*! \file cs237-util.inl
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Inline utility functions.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_UTIL_INL_
#define _CS237_UTIL_INL_

#ifndef _CS237_HXX_
#error "c237-util.inl should not be included directly"
#endif

namespace cs237 {

  //! \brief clamp a value to the range [0..1].
  //! \param f the value to clamp
  //! \return the clamped value
    inline float clamp (float f)
    {
	return (f < 0.0f) ? 0.0f : ((1.0f < f) ? 1.0f : f);
    }

  //! \brief clamp a value to the range [0..1].
  //! \param f the value to clamp
  //! \return the clamped value
    inline double clamp (double f)
    {
	return (f < 0.0) ? 0.0 : ((1.0 < f) ? 1.0 : f);
    }

  //! \brief convert an angle from degrees to radians
  //! \param theta the angle to convert
  //! \return the angle in radians
    inline float radians (float theta)
    {
	return 0.0174532925199f * theta;	// multiply by pi/180
    }

  //! \brief convert an angle from degrees to radians
  //! \param theta the angle to convert
  //! \return the angle in radians
    inline double radians (double theta)
    {
	return 0.0174532925199 * theta;		// multiply by pi/180
    }

  //! \brief convert an angle from radians to degrees
  //! \param theta the angle to convert
  //! \return the angle in degrees
    inline float degrees (float theta)
    {
	return 57.2957795131f * theta;		// multiply by 180/pi
    }

  //! \brief convert an angle from radians to degrees
  //! \param theta the angle to convert
  //! \return the angle in degrees
    inline double degrees (double theta)
    {
	return 57.2957795131 * theta;		// multiply by 180/pi
    }

  //! \brief scalar linear interpolation
  //! \param a value for t = 0
  //! \param t interpolation factor
  //! \param b value for t = 1
    inline float mix (float a, float t, float b)
    {
	return (a + t * (b - a));
    }

  //! \brief scalar linear interpolation
  //! \param a value for t = 0
  //! \param t interpolation factor
  //! \param b value for t = 10
    inline double mix (double a, double t, double b)
    {
	return (a + t * (b - a));
    }

  //! \brief minimum at various types
    inline int min (int a, int b) { return (a < b) ? a : b; }
    inline float min (float a, float b) { return (a < b) ? a : b; }
    inline double min (double a, double b) { return (a < b) ? a : b; }

  //! \brief maximum at various types
    inline int max (int a, int b) { return (a > b) ? a : b; }
    inline float max (float a, float b) { return (a > b) ? a : b; }
    inline double max (double a, double b) { return (a > b) ? a : b; }

  //! absolute value at various types
    inline int abs (int a) { return ::abs(a); }
    inline float abs (float a) { return ::fabsf(a); }
    inline double abs (double a) { return ::fabs(a); }

    namespace __detail {

	inline float toFloat (unsigned char b)
	{
	    return static_cast<float>(b) / 255.0f;
	}

	inline unsigned char toByte (float f)
	{
	    return static_cast<unsigned char>(255.0 * clamp(f));
	}

	void ReportError (GLenum err, const char *file, int line, const char *msg);

	inline void checkError (const char *file, int line, const char *msg)
	{
	    GLenum sts = glGetError();
	    if (sts != GL_NO_ERROR)
		ReportError (sts, file, line, msg);
	}
    } // namespace __detail


  /***** conversions to float *****/

    //! convert a double-precision vec2d value to single precision
    //! \param v the vector to convert
    //! \return the converted value
    inline vec2f toFloat (vec2d const &v)
    {
	return vec2f(float(v.x), float(v.y));
    }

    //! convert a double-precision vec3d value to single precision
    //! \param v the vector to convert
    //! \return the converted value
    inline vec3f toFloat (vec3d const &v)
    {
	return vec3f(float(v.x), float(v.y), float(v.z));
    }

    //! convert a double-precision vec4d value to single precision
    //! \param v the vector to convert
    //! \return the converted value
    inline vec4f toFloat (vec4d const &v)
    {
	return vec4f(float(v.x), float(v.y), float(v.z), float(v.w));
    }

  /***** conversions to double *****/

    //! convert a single-precision vec2f value to double precision
    //! \param v the vector to convert
    //! \return the converted value
    inline vec2d toDouble (vec2f const &v)
    {
	return vec2d(double(v.x), double(v.y));
    }

    //! convert a single-precision vec3f value to double precision
    //! \param v the vector to convert
    //! \return the converted value
    inline vec3d toDouble (vec3f const &v)
    {
	return vec3d(double(v.x), double(v.y), double(v.z));
    }

    //! convert a single-precision vec4f value to double precision
    //! \param v the vector to convert
    //! \return the converted value
    inline vec4d toDouble (vec4f const &v)
    {
	return vec4d(double(v.x), double(v.y), double(v.z), double(v.w));
    }

} // namespace cs237

#define CS237_CHECK(cmd)								\
	do {										\
	    cmd;									\
	    cs237::__detail::checkError (__FILE__, __LINE__, "executing " #cmd);	\
	} while (0)

#endif /* !_CS237_UTIL_INL_ */
