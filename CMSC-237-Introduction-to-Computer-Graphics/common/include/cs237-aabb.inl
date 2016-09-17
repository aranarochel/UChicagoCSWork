/*! \file cs237-aabb.inl
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2015 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_AABB_INL_
#define _CS237_AABB_INL_

#ifndef _CS237_HXX_
#  error "c237-aabb.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

	template <typename T>
	inline bool AABB<T>::isEmpty () const
	{
	    return this->_empty;
	}

	  //! clear the box (i.e., make it empty)
	template <typename T>
	inline void AABB<T>::clear ()
	{
	    this->_empty = true;
	}

	template <typename T>
	inline bool AABB<T>::includesPt (vec3<T> const &pt) const
	{
	    return (! this->_empty)
		&& (this->_min.x <= pt.x) && (pt.x <= this->_max.x)
		&& (this->_min.y <= pt.y) && (pt.y <= this->_max.y)
		&& (this->_min.z <= pt.z) && (pt.z <= this->_max.z);
	}

	template <typename T>
	inline T AABB<T>::distanceToPt (vec3<T> const &pt) const
	{
#define DELTA(A)							\
	(pt.A < this->_min.A)						\
	    ? (this->_min.A - pt.A)					\
	    : ((this->_max.A < pt.A) ? (pt.A - this->_max.A) : T(0))

	    vec3<T> dv(DELTA(x), DELTA(y), DELTA(z));
	    T lenSq = dot(dv, dv);
	    if (lenSq == 0.0)
		return lenSq;
	    else
		return sqrt(lenSq);
#undef DELTA
	}

	template <typename T>
	inline void AABB<T>::addPt (vec3<T> const &pt)
	{
	    if (this->_empty) {
		this->_empty = false;
		this->_min = pt;
		this->_max = pt;
	    }
	    else if (pt.x < this->_min.x) this->_min.x = pt.x;
	    else if (this->_max.x < pt.x) this->_max.x = pt.x;
	    if (pt.y < this->_min.y) this->_min.y = pt.y;
	    else if (this->_max.y < pt.y) this->_max.y = pt.y;
	    if (pt.z < this->_min.z) this->_min.z = pt.z;
	    else if (this->_max.z < pt.z) this->_max.z = pt.z;
	}

      //! center of the box
	template <typename T>
	inline vec3<T> AABB<T>::center () const
	{
	    assert (! this->_empty);
	    return T(0.5) * (this->_min + this->_max);
	}

	template <typename T>
	inline vec3<T> const & AABB<T>::min() const
	{
	    assert (! this->_empty);
	    return this->_min;
	}

	template <typename T>
	inline vec3<T> const & AABB<T>::max() const
	{
	    assert (! this->_empty);
	    return this->_max;
	}

	template <typename T>
	inline T AABB<T>::minX () const
	{
	    assert (! this->_empty);
	    return this->_min.x;
	}

	template <typename T>
	inline T AABB<T>::minY () const
	{
	    assert (! this->_empty);
	    return this->_min.y;
	}

	template <typename T>
	inline T AABB<T>::minZ () const
	{
	    assert (! this->_empty);
	    return this->_min.z;
	}

	template <typename T>
	inline T AABB<T>::maxX () const
	{
	    assert (! this->_empty);
	    return this->_max.x;
	}

	template <typename T>
	inline T AABB<T>::maxY () const
	{
	    assert (! this->_empty);
	    return this->_max.y;
	}

	template <typename T>
	inline T AABB<T>::maxZ () const
	{
	    assert (! this->_empty);
	    return this->_max.z;
	}

      //! return the coordinates of the i'th corner of the box, where
      //! _min is corner 0 and _max is corner 7.
	template <typename T>
	inline vec3<T> AABB<T>::corner (int i) const
	{
	    assert (! this->_empty);
	    assert ((0 <= i) && (i < 8));
	    return vec3<T>(
		(i & 4) ? this->_max.x : this->_min.x,
		(i & 2) ? this->_max.y : this->_min.y,
		(i & 1) ? this->_max.z : this->_min.z);
	}

#	define MIN(a,b)	((a <= b) ? a : b)
#	define MAX(a,b)	((a <= b) ? b : a)

      //! compute the union of two axis-aligned bounding boxes
      //! \param bb1 an axis-aligned bounding box
      //! \param bb2 an axis-aligned bounding box
      //! \return the smallest AABB that contains both \arg bb1 and \arg bb2.
	template <typename T>
	inline AABB<T> operator+ (AABB<T> const &bb1, AABB<T> &bb2)
	{
	    if ((! bb1._empty) && (! bb2.empty))
		return AABB<T>(
		    vec3<T>(
			MIN(bb1._min.x, bb2._min.x),
			MIN(bb1._min.y, bb2._min.y),
			MIN(bb1._min.z, bb2._min.z)),
		    vec3<T>(
			MAX(bb1._max.x, bb2._max.x),
			MAX(bb1._max.y, bb2._max.y),
			MAX(bb1._max.z, bb2._max.z)));
	    else if (bb1.empty)
		return bb2;
	    else
		return bb1;
	}

	template <typename T>
	inline AABB<T> & AABB<T>::operator+= (AABB const &bb)
	{
	    if ((! this->_empty) && (! bb._empty)) {
		this->_min.x = MIN(this->_min.x, bb._min.x);
		this->_min.y = MIN(this->_min.y, bb._min.y);
		this->_min.z = MIN(this->_min.z, bb._min.z);
		this->_max.x = MAX(this->_max.x, bb._max.x);
		this->_max.y = MAX(this->_max.y, bb._max.y);
		this->_max.z = MAX(this->_max.z, bb._max.z);
	    }
	    else if (this->_empty) {
		this->_empty = false;
		this->_min = bb._min;
		this->_max = bb._max;
	    }

	    return *this;
	}


	template <typename T>
	inline AABB<T> & AABB<T>::operator+= (vec3<T> const &pt)
	{
	    this->addPt (pt);
	    return *this;
	}

#	undef MIN
#	undef MAX

    } // namespace __detail

} //namespace cs237

#endif /* !_CS237_AABB_INL_ */
