#ifndef _FRUSTUM_HXX_
#define _FRUSTUM_HXX_

#include "cs237.hxx"
#include "camera.hxx"
#include <vector>

enum {OUTSIDE = 0, INTERSECT = 1, INSIDE = 2};

class GFrustum{
	public:
		enum {
			TOP = 0, BOTTOM, LEFT,
			RIGHT, NEARP, FARP
		};

		enum {OUTSIDE, INTERSECT, INSIDE};

		std::vector<class GPlane *> planes;

		cs237::vec3f dir, up, right, pos; 	
		float near,far,Hnear,Wnear,Hfar,Wfar;

		cs237::vec3f ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr,fc,nc;
		float nw,nh,fw,fh;

		GFrustum(Camera cam);
		~GFrustum();

		//int pointInFrustum(Vec3 &p);
		int boxInFrustum(cs237::AABBd const box);
};

class GPlane{
	public:
		GPlane(cs237::vec3f p0,cs237::vec3f p1,cs237::vec3f p2);
		~GPlane();

		//std::vector<class cs237::vec3f *> points;
		cs237::vec3f normal;
		float D;

		float distance(cs237::vec3f point);
};

#endif //! _FRUSTUM_HXX_