/*! \file cs237-color.hxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_COLOR_HXX_
#define _CS237_COLOR_HXX_

#ifndef _CS237_HXX_
#error "c237-color.hxx should not be included directly"
#endif

namespace cs237 {

    struct color3ub {
	unsigned char r, g, b;

	color3ub () : r(0.0f), g(0.0f), b(0.0f) { }
	color3ub (color3ub const &c) : r(c.r), g(c.g), b(c.b) { }
	explicit color3ub (color3f const &c);
	explicit color3ub (unsigned char grey) : r(grey), g(grey), b(grey) { }
	color3ub (unsigned char rr, unsigned char gg, unsigned char bb) : r(rr), g(gg), b(bb) { }

	unsigned char & operator[] (unsigned int const &i);
	unsigned char const & operator[] (unsigned int const &i) const;

	color3ub & operator= (color3f const &c);
    };

    struct color4ub {
	unsigned char r, g, b, a;

	color4ub () : r(0), g(0), b(0), a(255) { }
	color4ub (color4ub const &c) : r(c.r), g(c.g), b(c.b), a(c.a) { }
	explicit color4ub (color3ub const &c) : r(c.r), g(c.g), b(c.b), a(255) { }
	explicit color4ub (color3ub const &c, unsigned char alpha) : r(c.r), g(c.g), b(c.b), a(alpha) { }
	explicit color4ub (color4f const &c);
	explicit color4ub (unsigned char grey) : r(grey), g(grey), b(grey), a(255) { }
	explicit color4ub (unsigned char rr, unsigned char gg, unsigned char bb) : r(rr), g(gg), b(bb), a(255) { }
	color4ub (unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa)
	    : r(rr), g(gg), b(bb), a(aa)
	{ }

	unsigned char & operator[] (unsigned int const &i);
	unsigned char const & operator[] (unsigned int const &i) const;

	color4ub & operator= (color4f const &c);
    };

    struct color3f {
	float r, g, b;

	color3f () : r(0.0f), g(0.0f), b(0.0f) { }
	color3f (color3f const &c) : r(c.r), g(c.g), b(c.b) { }
	explicit color3f (color3ub const &c);
	explicit color3f (float grey) : r(grey), g(grey), b(grey) { }
	color3f (float rr, float gg, float bb) : r(rr), g(gg), b(bb) { }

	float & operator[] (unsigned int const &i);
	float const & operator[] (unsigned int const &i) const;

	color3f & operator= (color3ub const &c);

	color3f & operator+= (float const &s);
	color3f & operator+= (color3f const &c);

	color3f & operator-= (float const &s);
	color3f & operator-= (color3f const &c);

	color3f & operator*= (float const &s);
	color3f & operator*= (color3f const &c);

	color3f & clamp ();
    };

    struct color4f {
	float r, g, b, a;

	color4f () : r(0.0f), g(0.0f), b(0.0f), a(1.0f) { }
	explicit color4f (color3f const &c) : r(c.r), g(c.g), b(c.b), a(1.0f) { }
	explicit color4f (color3f const &c, float alpha) : r(c.r), g(c.g), b(c.b), a(alpha) { }
	color4f (color4f const &c) : r(c.r), g(c.g), b(c.b), a(c.a) { }
	explicit color4f (color4ub const &c);
	explicit color4f (float grey) : r(grey), g(grey), b(grey), a(1.0f) { }
	explicit color4f (float rr, float gg, float bb) : r(rr), g(gg), b(bb), a(1.0f) { }
	color4f (float rr, float gg, float bb, float aa) : r(rr), g(gg), b(bb), a(aa) { }

	float & operator[] (unsigned int const &i);
	float const & operator[] (unsigned int const &i) const;

	color4f & operator= (color4ub const &c);

	color4f & operator+= (float const &s);
	color4f & operator+= (color4f const &c);

	color4f & operator-= (float const &s);
	color4f & operator-= (color4f const &c);

	color4f & operator*= (float const &s);
	color4f & operator*= (color4f const &c);

	color4f & clamp ();
    };

  //! \brief output the color to a stream
  //! \param s the stream to print to
  //! \param c the color to print
    std::ostream& operator<< (std::ostream& s, color3ub const &c);

  //! \brief output the color to a stream
  //! \param s the stream to print to
  //! \param c the color to print
    std::ostream& operator<< (std::ostream& s, color4ub const &c);

  //! \brief output the color to a stream
  //! \param s the stream to print to
  //! \param c the color to print
    std::ostream& operator<< (std::ostream& s, color3f const &c);

  //! \brief output the color to a stream
  //! \param s the stream to print to
  //! \param c the color to print
    std::ostream& operator<< (std::ostream& s, color4f const &c);

} /* namespace cs237 */

#endif /* !_CS237_COLOR_HXX_ */
