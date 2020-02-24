#pragma once

#include "ofMain.h"

class ofxPointInMesh {

	public:
		static bool isInside(const glm::vec3 & point, const ofMesh & mesh);

	private:
		static bool triangleIntersection(const ofMeshFace & face, const glm::vec3 & O, const glm::vec3 & D, glm::vec3 & R);
};
