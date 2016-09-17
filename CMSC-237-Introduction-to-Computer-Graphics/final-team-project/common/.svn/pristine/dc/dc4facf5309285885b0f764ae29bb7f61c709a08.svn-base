/*    
      obj-reader.cxx

      This code was ported from a C program written by Nate Robins.

      Nate Robins, 1997, 2000
      nate@pobox.com, http://www.pobox.com/~nate
 
      Wavefront OBJ model file format reader/writer/manipulator.

      Includes routines for generating smooth normals with
      preservation of edges, welding redundant vertices & texture
      coordinate generation (spheremap and planar projections) + more.
  
*/

#include "cs237.hxx"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "obj-reader.hxx"

#define FREE(p)		std::free(p)

#define T(x) (model->triangles[(x)])

/* OBJAbs: returns the absolute value of a float */
static inline float OBJAbs(float f)
{
    if (f < 0) return -f; else return f;
}

/* OBJFindGroup: Find a group in the model */
OBJgroup*
OBJFindGroup(OBJmodel* model, const char* name)
{
    OBJgroup* group;
    
    assert(model);
    
    group = model->groups;
    while(group) {
        if (!strcmp(name, group->name))
            break;
        group = group->next;
    }
    
    return group;
}

/* OBJAddGroup: Add a group to the model */
OBJgroup*
OBJAddGroup(OBJmodel* model, const char* name)
{
    OBJgroup* group;
    
    group = OBJFindGroup(model, name);
    if (!group) {
        group = new OBJgroup;
        group->name = strdup(name);
        group->material = 0;
        group->numtriangles = 0;
        group->triangles = NULL;
        group->next = model->groups;
        model->groups = group;
        model->numgroups++;
    }
    
    return group;
}


/* OBJFirstPass: first pass at a Wavefront OBJ file that gets all the
 * statistics of the model (such as #vertices, #normals, etc)
 *
 * model - properly initialized OBJmodel structure
 * file  - (fopen'd) file descriptor 
 */
static void
OBJFirstPass(OBJmodel* model, FILE* file) 
{
    uint32_t numvertices;        /* number of vertices in model */
    uint32_t numnormals;         /* number of normals in model */
    uint32_t numtexcoords;       /* number of texcoords in model */
    uint32_t numtriangles;       /* number of triangles in model */
    OBJgroup* group = 0;         /* current group */
    int v, n, t;
    char buf[128];
    
    numvertices = numnormals = numtexcoords = numtriangles = 0;
    while(fscanf(file, "%s", buf) != EOF) {
        switch(buf[0]) {
        case '#':               /* comment */
            /* eat up rest of line */
            fgets(buf, sizeof(buf), file);
            break;
        case 'v':               /* v, vn, vt */
	    if (group == 0) {
	      /* make a default group */
		group = OBJAddGroup(model, "default");
	    }
            switch(buf[1]) {
            case '\0':          /* vertex */
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
                numvertices++;
                break;
            case 'n':           /* normal */
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
                numnormals++;
                break;
            case 't':           /* texcoord */
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
                numtexcoords++;
                break;
            default:
                printf("OBJFirstPass(): Unknown token \"%s\".\n", buf);
                exit(1);
                break;
            }
            break;
	case 'm':
	    fgets(buf, sizeof(buf), file);
	    sscanf(buf, "%s %s", buf, buf);
	    model->mtllibname = strdup(buf);
	    break;
	case 'u':
	    if (group == 0) {
	      /* make a default group */
		group = OBJAddGroup(model, "default");
	    }
	    /* eat up rest of line */
	    fgets(buf, sizeof(buf), file);
	    break;
	case 'g':               /* group */
	    /* eat up rest of line */
	    fgets(buf, sizeof(buf), file);
#if SINGLE_STRING_GROUP_NAMES
	    sscanf(buf, "%s", buf);
#else
	    buf[strlen(buf)-1] = '\0';  /* nuke '\n' */
#endif
	    group = OBJAddGroup(model, buf);
	    break;
	case 'f':               /* face */
	    if (group == 0) {
	      /* make a default group */
		group = OBJAddGroup(model, "default");
	    }
	    v = n = t = 0;
	    fscanf(file, "%s", buf);
	    /* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
	    if (strstr(buf, "//")) {
		/* v//n */
		sscanf(buf, "%d//%d", &v, &n);
		fscanf(file, "%d//%d", &v, &n);
		fscanf(file, "%d//%d", &v, &n);
		numtriangles++;
		group->numtriangles++;
		while(fscanf(file, "%d//%d", &v, &n) > 0) {
		    numtriangles++;
		    group->numtriangles++;
		}
	    } else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
		/* v/t/n */
		fscanf(file, "%d/%d/%d", &v, &t, &n);
		fscanf(file, "%d/%d/%d", &v, &t, &n);
		numtriangles++;
		group->numtriangles++;
		while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) {
		    numtriangles++;
		    group->numtriangles++;
		}
	    } else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
		/* v/t */
		fscanf(file, "%d/%d", &v, &t);
		fscanf(file, "%d/%d", &v, &t);
		numtriangles++;
		group->numtriangles++;
		while(fscanf(file, "%d/%d", &v, &t) > 0) {
		    numtriangles++;
		    group->numtriangles++;
		}
	    } else {
		/* v */
		fscanf(file, "%d", &v);
		fscanf(file, "%d", &v);
		numtriangles++;
		group->numtriangles++;
		while(fscanf(file, "%d", &v) > 0) {
		    numtriangles++;
		    group->numtriangles++;
		}
	    }
	    break;
	    
	default:
	    /* eat up rest of line */
	    fgets(buf, sizeof(buf), file);
	    break;
        }
  }
  
  /* set the stats in the model structure */
  model->numvertices  = numvertices;
  model->numnormals   = numnormals;
  model->numtexcoords = numtexcoords;
  model->numtriangles = numtriangles;
  
  /* allocate memory for the triangles in each group */
  group = model->groups;
  while(group) {
      group->triangles = new uint32_t[group->numtriangles];
      group->numtriangles = 0;
      group = group->next;
  }
}

/* OBJSecondPass: second pass at a Wavefront OBJ file that gets all
 * the data.
 *
 * model - properly initialized OBJmodel structure
 * file  - (fopen'd) file descriptor 
 */
static void
OBJSecondPass(OBJmodel* model, FILE* file) 
{
    uint32_t numvertices;	/* number of vertices in model */
    uint32_t numnormals;	/* number of normals in model */
    uint32_t numtexcoords;	/* number of texcoords in model */
    uint32_t numtriangles;	/* number of triangles in model */
    cs237::vec3f *vertices;	/* array of vertices  */
    cs237::vec3f *normals;	/* array of normals */
    cs237::vec2f *texcoords;	/* array of texture coordinates */
    OBJgroup* group;		/* current group pointer */
    char* material;		/* current material */
    int v, n, t;
    char buf[128];
    
    /* set the pointer shortcuts */
    vertices	= model->vertices;
    normals	= model->normals;
    texcoords	= model->texcoords;
    group	= model->groups;
    
    /* on the second pass through the file, read all the data into the
    allocated arrays */
    numvertices = numnormals = numtexcoords = 1;
    numtriangles = 0;
    material = nullptr;
    while(fscanf(file, "%s", buf) != EOF) {
        switch(buf[0]) {
        case '#':               /* comment */
            /* eat up rest of line */
            fgets(buf, sizeof(buf), file);
            break;
        case 'v':               /* v, vn, vt */
            switch(buf[1]) {
            case '\0':          /* vertex */
                fscanf(file, "%f %f %f", 
                    &(vertices[numvertices][0]), 
                    &(vertices[numvertices][1]), 
                    &(vertices[numvertices][2]));
                numvertices++;
                break;
            case 'n':           /* normal */
                fscanf(file, "%f %f %f", 
                    &(normals[numnormals][0]),
                    &(normals[numnormals][1]), 
                    &(normals[numnormals][2]));
                numnormals++;
                break;
            case 't':           /* texcoord */
                fscanf(file, "%f %f", 
                    &(texcoords[numtexcoords][0]),
                    &(texcoords[numtexcoords][1]));
                numtexcoords++;
                break;
            }
            break;
            case 'u':
                fgets(buf, sizeof(buf), file);
                sscanf(buf, "%s %s", buf, buf);
                material = strdup(buf);
	      // if there is already a material associated with this group, then we
	      // ignore this material.
/* QUESION: will this work for multiple groups with shared and different materials? */
		if (group->material == nullptr) {
		    group->material = material;
		}
                break;
            case 'g':               /* group */
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
#if SINGLE_STRING_GROUP_NAMES
                sscanf(buf, "%s", buf);
#else
                buf[strlen(buf)-1] = '\0';  /* nuke '\n' */
#endif
                group = OBJFindGroup(model, buf);
                group->material = (material != nullptr) ? strdup(material) : nullptr;
                break;
            case 'f':               /* face */
                v = n = t = 0;
                fscanf(file, "%s", buf);
                /* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
                if (strstr(buf, "//")) {
                    /* v//n */
                    sscanf(buf, "%d//%d", &v, &n);
                    T(numtriangles).vindices[0] = v < 0 ? v + numvertices : v;
                    T(numtriangles).nindices[0] = n < 0 ? n + numnormals : n;
                    fscanf(file, "%d//%d", &v, &n);
                    T(numtriangles).vindices[1] = v < 0 ? v + numvertices : v;
                    T(numtriangles).nindices[1] = n < 0 ? n + numnormals : n;
                    fscanf(file, "%d//%d", &v, &n);
                    T(numtriangles).vindices[2] = v < 0 ? v + numvertices : v;
                    T(numtriangles).nindices[2] = n < 0 ? n + numnormals : n;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d//%d", &v, &n) > 0) {
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).nindices[0] = T(numtriangles-1).nindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).nindices[1] = T(numtriangles-1).nindices[2];
                        T(numtriangles).vindices[2] = v < 0 ? v + numvertices : v;
                        T(numtriangles).nindices[2] = n < 0 ? n + numnormals : n;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                } else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
                    /* v/t/n */
                    T(numtriangles).vindices[0] = v < 0 ? v + numvertices : v;
                    T(numtriangles).tindices[0] = t < 0 ? t + numtexcoords : t;
                    T(numtriangles).nindices[0] = n < 0 ? n + numnormals : n;
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    T(numtriangles).vindices[1] = v < 0 ? v + numvertices : v;
                    T(numtriangles).tindices[1] = t < 0 ? t + numtexcoords : t;
                    T(numtriangles).nindices[1] = n < 0 ? n + numnormals : n;
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    T(numtriangles).vindices[2] = v < 0 ? v + numvertices : v;
                    T(numtriangles).tindices[2] = t < 0 ? t + numtexcoords : t;
                    T(numtriangles).nindices[2] = n < 0 ? n + numnormals : n;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) {
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).tindices[0] = T(numtriangles-1).tindices[0];
                        T(numtriangles).nindices[0] = T(numtriangles-1).nindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).tindices[1] = T(numtriangles-1).tindices[2];
                        T(numtriangles).nindices[1] = T(numtriangles-1).nindices[2];
                        T(numtriangles).vindices[2] = v < 0 ? v + numvertices : v;
                        T(numtriangles).tindices[2] = t < 0 ? t + numtexcoords : t;
                        T(numtriangles).nindices[2] = n < 0 ? n + numnormals : n;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                } else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
                    /* v/t */
                    T(numtriangles).vindices[0] = v < 0 ? v + numvertices : v;
                    T(numtriangles).tindices[0] = t < 0 ? t + numtexcoords : t;
                    fscanf(file, "%d/%d", &v, &t);
                    T(numtriangles).vindices[1] = v < 0 ? v + numvertices : v;
                    T(numtriangles).tindices[1] = t < 0 ? t + numtexcoords : t;
                    fscanf(file, "%d/%d", &v, &t);
                    T(numtriangles).vindices[2] = v < 0 ? v + numvertices : v;
                    T(numtriangles).tindices[2] = t < 0 ? t + numtexcoords : t;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d/%d", &v, &t) > 0) {
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).tindices[0] = T(numtriangles-1).tindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).tindices[1] = T(numtriangles-1).tindices[2];
                        T(numtriangles).vindices[2] = v < 0 ? v + numvertices : v;
                        T(numtriangles).tindices[2] = t < 0 ? t + numtexcoords : t;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                } else {
                    /* v */
                    sscanf(buf, "%d", &v);
                    T(numtriangles).vindices[0] = v < 0 ? v + numvertices : v;
                    fscanf(file, "%d", &v);
                    T(numtriangles).vindices[1] = v < 0 ? v + numvertices : v;
                    fscanf(file, "%d", &v);
                    T(numtriangles).vindices[2] = v < 0 ? v + numvertices : v;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d", &v) > 0) {
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).vindices[2] = v < 0 ? v + numvertices : v;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                }
                break;
                
            default:
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
                break;
    }
  }
  
}


/* public functions */

/* OBJDimensions: Calculates the dimensions (width, height, depth) of
 * a model.
 *
 * model   - initialized OBJmodel structure
 * dimensions - array of 3 GLfloats (float dimensions[3])
 */
void
OBJDimensions(OBJmodel* model, float* dimensions)
{
    uint32_t i;
    float maxx, minx, maxy, miny, maxz, minz;
    
    assert(model);
    assert(model->vertices);
    assert(dimensions);
    
    /* get the max/mins */
    maxx = minx = model->vertices[1][0];
    maxy = miny = model->vertices[1][1];
    maxz = minz = model->vertices[1][2];
    for (i = 2; i <= model->numvertices; i++) {
        if (maxx < model->vertices[i][0])
            maxx = model->vertices[i][0];
        if (minx > model->vertices[i][0])
            minx = model->vertices[i][0];
        
        if (maxy < model->vertices[i][1])
            maxy = model->vertices[i][1];
        if (miny > model->vertices[i][1])
            miny = model->vertices[i][1];
        
        if (maxz < model->vertices[i][2])
            maxz = model->vertices[i][2];
        if (minz > model->vertices[i][2])
            minz = model->vertices[i][2];
    }
    
    /* calculate model width, height, and depth */
    dimensions[0] = OBJAbs(maxx) + OBJAbs(minx);
    dimensions[1] = OBJAbs(maxy) + OBJAbs(miny);
    dimensions[2] = OBJAbs(maxz) + OBJAbs(minz);
}

/* OBJDelete: Deletes a OBJmodel structure.
 *
 * model - initialized OBJmodel structure
 */
void
OBJDelete(OBJmodel* model)
{
    OBJgroup* group;
    
    assert(model);
    
    if (model->mtllibname) FREE(model->mtllibname);
    if (model->vertices)   FREE(model->vertices);
    if (model->normals)    FREE(model->normals);
    if (model->texcoords)  FREE(model->texcoords);
    if (model->triangles)  FREE(model->triangles);
    while (model->groups) {
        group = model->groups;
        model->groups = model->groups->next;
        FREE (group->name);
	if (group->material) FREE (group->material);
        FREE (group->triangles);
        FREE (group);
    }
    
    FREE(model);
}

/* OBJReadOBJ: Reads a model description from a Wavefront .OBJ file.
 * Returns a pointer to the created object which should be free'd with
 * OBJDelete().
 *
 * filename - name of the file containing the Wavefront .OBJ format data.  
 */
OBJmodel* 
OBJReadOBJ(const char* filename)
{
    FILE* file;
    
    /* open the file */
    file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "OBJReadOBJ() failed: can't open data file \"%s\".\n",
            filename);
        exit(1);
    }
    
    /* allocate a new model */
    OBJmodel* model = new OBJmodel();
    
    /* make a first pass through the file to get a count of the number
    of vertices, normals, texcoords & triangles */
    OBJFirstPass (model, file);
    
    /* allocate memory */
    model->vertices = new cs237::vec3f[model->numvertices + 1];
    model->triangles = new OBJtriangle[model->numtriangles];
    if (model->numnormals > 0) {
        model->normals = new cs237::vec3f[model->numnormals + 1];
    }
    if (model->numtexcoords > 0) {
        model->texcoords = new cs237::vec2f[model->numtexcoords + 1];
    }
    
    /* rewind to beginning of file and read in the data this pass */
    rewind(file);
    
    OBJSecondPass(model, file);
    
    /* close the file */
    fclose(file);
    
    return model;
}

/***** OBJmodel methods *****/

OBJmodel::OBJmodel ()
    : mtllibname(0), numvertices(0), vertices(0), numnormals(0),
      normals(0), numtexcoords(0), texcoords(0), numtriangles(0),
      triangles(0), numgroups(0), groups(0)
{
}

OBJmodel::~OBJmodel ()
{
    if (this->mtllibname) FREE(this->mtllibname);
    if (this->vertices)   FREE(this->vertices);
    if (this->normals)    FREE(this->normals);
    if (this->texcoords)  FREE(this->texcoords);
    if (this->triangles)  FREE(this->triangles);
    while (this->groups) {
        OBJgroup *group = this->groups;
        this->groups = this->groups->next;
        FREE (group->name);
	if (group->material) FREE (group->material);
        FREE (group->triangles);
        FREE (group);
    }

}
