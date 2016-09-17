/*! \file cs237-color.cxx
 *
 * Support code for CMSC 23700 Autumn 2015.
 *
 * Operations on color values.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include <iomanip>

namespace cs237 {

  //! \brief output the color to a stream
  //! \param s the stream to print to
  //! \param c the color to print
    std::ostream& operator<< (std::ostream& s, color3ub const &c)
    {
	return (s << "<"
	    << std::setw(3) << int(c.r) << ","
	    << std::setw(3) << int(c.g) << ","
	    << std::setw(3) << int(c.b) << ">");
    }

  //! \brief output the color to a stream
  //! \param s the stream to print to
  //! \param c the color to print
    std::ostream& operator<< (std::ostream& s, color4ub const &c)
    {
	return (s << "<"
	    << std::setw(3) << int(c.r) << ","
	    << std::setw(3) << int(c.g) << ","
	    << std::setw(3) << int(c.b) << ","
	    << std::setw(3) << int(c.a) << ">");
    }

  //! \brief output the color to a stream
  //! \param s the stream to print to
  //! \param c the color to print
    std::ostream& operator<< (std::ostream& s, color3f const &c)
    {
	return (s << "<" << c.r << "," << c.g << "," << c.b << ">");
    }

  //! \brief output the color to a stream
  //! \param s the stream to print to
  //! \param c the color to print
    std::ostream& operator<< (std::ostream& s, color4f const &c)
    {
	return (s << "<" << c.r << "," << c.g << "," << c.b << "," << c.a << ">");
    }

} // namespace cs237
