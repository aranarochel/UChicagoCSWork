#include "cs237.hxx"
#include "frustum.hxx"

//New Implementation
GFrustum::GFrustum (Camera cam)
{
	this->dir 	= cam.direction();
	this->up 	= cam.up();
	this->right = cs237::__detail::cross(dir,up);
	this->pos 	= cs237::toFloat(cam.position());
	this->near 	= cam.near();
	this->far 	= cam.far();
	this->Hnear = 2.0 * tan(cam.fov() / 2.0) * near;
	this->Wnear = Hnear * cam.aspect();
	this->Hfar 	= 2.0 * tan(cam.fov() / 2.0) * far;
	this->Wfar 	= Hfar * cam.aspect();

	//calculate center points
	this->fc = pos + dir * far;
	this->nc = pos + dir * near;

	float halfHfar = Hfar/2.0f;
	float halfHnear = Hnear/2.0f;
	float halfWfar = Wfar/2.0f;
	float halfWnear = Wnear/2.0f;	

	//top left's	
	this->ftl = fc + (up * halfHfar) - (right * halfWfar);
	this->ntl = nc + (up * halfHnear) - (right * halfWnear);
	//top right
	this->ftr = fc + (up * halfHfar) + (right * halfWfar);
	this->ntr = nc + (up * halfHnear) + (right * halfWnear);
	//bottom left
	this->fbl = fc - (up * halfHfar) - (right * halfWfar);
	this->nbl = nc - (up * halfHnear) - (right * halfWnear);
	//bottom right
	this->fbr = fc - (up * halfHfar) + (right * halfWfar);
	this->nbr = nc - (up * halfHnear) + (right * halfWnear);

	//set planes
	planes.push_back( new GPlane(ntr,ntl,ftl) );
	planes.push_back( new GPlane(nbl,nbr,fbr) );
	planes.push_back( new GPlane(ntl,nbl,fbl) );
	planes.push_back( new GPlane(nbr,ntr,fbr) );
	planes.push_back( new GPlane(ntl,ntr,nbr) );
	planes.push_back( new GPlane(ftr,ftl,fbl) );
}

GFrustum::~GFrustum ()
{ }

int GFrustum::boxInFrustum(cs237::AABBd const box) {

	int result = INSIDE, out,in;

	for(int i=0; i < 6; i++) {

		out=0;in=0;

		for (int k = 0; k < 8 && (in==0 || out==0); k++) {
			if (planes[i]->distance(cs237::toFloat(box.corner(k))) < 0.0)
				out++;
			else
				in++;
		}
		if (!in)
			return (OUTSIDE);
		else if (out)
			result = INTERSECT;
	}
	return(result);

 }


GPlane::GPlane(cs237::vec3f p0,cs237::vec3f p1,cs237::vec3f p2){
	this->normal = (cs237::__detail::cross(p1-p0,p2-p0)).normalize();
	this->D = cs237::__detail::dot(-normal,p0);
}

float GPlane::distance(cs237::vec3f point){
	return cs237::__detail::dot(normal,point) + D;
}



