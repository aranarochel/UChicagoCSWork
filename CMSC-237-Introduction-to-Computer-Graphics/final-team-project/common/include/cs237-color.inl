/*! \file cs237-color.inl
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Inline operations on color values.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_COLOR_INL_
#define _CS237_COLOR_INL_

#ifndef _CS237_HXX_
#error "c237-color.inl should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      /********** Other functions **********/

	inline unsigned char const *valuePtr (color3ub const &c)
	{
	    return &(c.r);
	}

	inline unsigned char const *valuePtr (color4ub const &c)
	{
	    return &(c.r);
	}

	inline float const *valuePtr (color3f const &c)
	{
	    return &(c.r);
	}

	inline float const *valuePtr (color4f const &c)
	{
	    return &(c.r);
	}

    } /* namespace __detail */

  /********** struct color3ub member functions **********/

    inline color3ub::color3ub (color3f const &c)
	: r(__detail::toByte(c.r)), g(__detail::toByte(c.g)), b(__detail::toByte(c.b))
    { }

    inline unsigned char & color3ub::operator[] (unsigned int const &i)
    {
	assert (i < 3);
	return (&this->r)[i];
    }
    inline unsigned char const & color3ub::operator[] (unsigned int const &i) const
    {
	assert (i < 3);
	return (&this->r)[i];
    }

    inline color3ub & color3ub::operator= (color3f const &c)
    {
	this->r = __detail::toByte(c.r);
	this->g = __detail::toByte(c.g);
	this->b = __detail::toByte(c.b);
	return *this;
    }

  /********** struct color4ub member functions **********/

    inline color4ub::color4ub (color4f const &c)
	: r(__detail::toByte(c.r)), g(__detail::toByte(c.g)), b(__detail::toByte(c.b)), a(__detail::toByte(c.a))
    { }

    inline unsigned char & color4ub::operator[] (unsigned int const &i)
    {
	assert (i < 4);
	return (&this->r)[i];
    }
    inline unsigned char const & color4ub::operator[] (unsigned int const &i) const
    {
	assert (i < 4);
	return (&this->r)[i];
    }

    inline color4ub & color4ub::operator= (color4f const &c)
    {
	this->r = __detail::toByte(c.r);
	this->g = __detail::toByte(c.g);
	this->b = __detail::toByte(c.b);
	this->a = __detail::toByte(c.a);
	return *this;
    }

  /********** struct color3f member functions **********/

  //! convert a color3ub to a color3f value
    inline color3f::color3f (color3ub const &c)
	: r(__detail::toFloat(c.r)), g(__detail::toFloat(c.g)), b(__detail::toFloat(c.b))
    { }

    inline float & color3f::operator[] (unsigned int const &i)
    {
	assert (i < 3);
	return (&this->r)[i];
    }
    inline float const & color3f::operator[] (unsigned int const &i) const
    {
	assert (i < 3);
	return (&this->r)[i];
    }

  //! convert a color3ub value to a color3f l-value
    inline color3f & color3f::operator= (color3ub const &c)
    {
	this->r = __detail::toFloat(c.r);
	this->g = __detail::toFloat(c.g);
	this->b = __detail::toFloat(c.b);
	return *this;
    }

    inline color3f & color3f::operator+= (float const &s)
    {
	this->r = cs237::clamp(this->r + s);
	this->g = cs237::clamp(this->g + s);
	this->b = cs237::clamp(this->b + s);
	return *this;
    }
    inline color3f & color3f::operator+= (color3f const &c)
    {
	this->r = cs237::clamp(this->r + c.r);
	this->g = cs237::clamp(this->g + c.g);
	this->b = cs237::clamp(this->b + c.b);
	return *this;
    }

    inline color3f & color3f::operator-= (float const &s)
    {
	this->r = cs237::clamp(this->r - s);
	this->g = cs237::clamp(this->g - s);
	this->b = cs237::clamp(this->b - s);
	return *this;
    }
    inline color3f & color3f::operator-= (color3f const &c)
    {
	this->r = cs237::clamp(this->r - c.r);
	this->g = cs237::clamp(this->g - c.g);
	this->b = cs237::clamp(this->b - c.b);
	return *this;
    }

    inline color3f & color3f::operator*= (float const &s)
    {
	this->r = cs237::clamp(this->r * s);
	this->g = cs237::clamp(this->g * s);
	this->b = cs237::clamp(this->b * s);
	return *this;
    }
    inline color3f & color3f::operator*= (color3f const &c)
    {
	this->r = this->r * c.r;	// we are assuming that c is cs237::clamped to 0..1
	this->g = this->g * c.g;
	this->b = this->b * c.b;
	return *this;
    }

    inline color3f & color3f::clamp ()
    {
	this->r = cs237::clamp(this->r);
	this->g = cs237::clamp(this->g);
	this->b = cs237::clamp(this->b);
	return *this;
    }

  /********** struct color4f member functions **********/

  //! convert a color4ub to a color4f value
    inline color4f::color4f (color4ub const &c)
	: r(__detail::toFloat(c.r)), g(__detail::toFloat(c.g)), b(__detail::toFloat(c.b)), a(__detail::toFloat(c.a))
    { }

    inline float & color4f::operator[] (unsigned int const &i)
    {
	assert (i < 4);
	return (&this->r)[i];
    }
    inline float const & color4f::operator[] (unsigned int const &i) const
    {
	assert (i < 4);
	return (&this->r)[i];
    }

  //! convert a color4ub value to a color4f l-value
    inline color4f & color4f::operator= (color4ub const &c)
    {
	this->r = __detail::toFloat(c.r);
	this->g = __detail::toFloat(c.g);
	this->b = __detail::toFloat(c.b);
	this->a = __detail::toFloat(c.a);
	return *this;
    }

    inline color4f & color4f::operator+= (float const &s)
    {
	this->r = cs237::clamp(this->r + s);
	this->g = cs237::clamp(this->g + s);
	this->b = cs237::clamp(this->b + s);
	this->a = cs237::clamp(this->a + s);
	return *this;
    }
    inline color4f & color4f::operator+= (color4f const &c)
    {
	this->r = cs237::clamp(this->r + c.r);
	this->g = cs237::clamp(this->g + c.g);
	this->b = cs237::clamp(this->b + c.b);
	this->a = cs237::clamp(this->a + c.a);
	return *this;
    }

    inline color4f & color4f::operator-= (float const &s)
    {
	this->r = cs237::clamp(this->r - s);
	this->g = cs237::clamp(this->g - s);
	this->b = cs237::clamp(this->b - s);
	this->a = cs237::clamp(this->a - s);
	return *this;
    }
    inline color4f & color4f::operator-= (color4f const &c)
    {
	this->r = cs237::clamp(this->r - c.r);
	this->g = cs237::clamp(this->g - c.g);
	this->b = cs237::clamp(this->b - c.b);
	this->a = cs237::clamp(this->a - c.a);
	return *this;
    }

    inline color4f & color4f::operator*= (float const &s)
    {
	this->r = cs237::clamp(this->r * s);
	this->g = cs237::clamp(this->g * s);
	this->b = cs237::clamp(this->b * s);
	this->a = cs237::clamp(this->a * s);
	return *this;
    }
    inline color4f & color4f::operator*= (color4f const &c)
    {
	this->r = cs237::clamp(this->r * c.r);
	this->g = cs237::clamp(this->g * c.g);
	this->b = cs237::clamp(this->b * c.b);
	this->a = cs237::clamp(this->a * c.a);
	return *this;
    }

    inline color4f & color4f::clamp ()
    {
	this->r = cs237::clamp(this->r);
	this->g = cs237::clamp(this->g);
	this->b = cs237::clamp(this->b);
	this->a = cs237::clamp(this->a);
	return *this;
    }

  /********** Other functions **********/

    inline color3f operator+ (color3f const & c1, color3f const & c2)
    {
	return color3f(cs237::clamp(c1.r + c2.r), cs237::clamp(c1.g + c2.g), cs237::clamp(c1.b + c2.b));
    }

    inline color3f operator- (color3f const & c1, color3f const & c2)
    {
	return color3f(cs237::clamp(c1.r - c2.r), cs237::clamp(c1.g - c2.g), cs237::clamp(c1.b - c2.b));
    }

    inline color3f operator* (color3f const & c1, color3f const & c2)
    {
	return color3f(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b);
    }

    inline color3f mix (color3f const & c1, float t, color3f const & c2)
    {
	return color3f(
	    cs237::clamp(c1.r + t * (c2.r - c1.r)),
	    cs237::clamp(c1.g + t * (c2.g - c1.g)),
	    cs237::clamp(c1.b + t * (c2.b - c1.b)));
    }

} /* namespace cs237 */

#endif /* !_CS237_COLOR_INL_ */
