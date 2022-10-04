#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	// faces_[0,2,4]->v1, v2, v3, faces_[1,3,5]->vt1, vt2, vt3
	std::vector<std::vector<int> > faces_;
	std::vector<Vec3f> textures_;
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	Vec3f texture(int i);
	std::vector<int> face(int idx);
};

#endif //__MODEL_H__