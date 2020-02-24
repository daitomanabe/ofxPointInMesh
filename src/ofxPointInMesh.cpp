
#include "ofxPointInMesh.h"

#define EPSILON 0.00000001

bool ofxPointInMesh::isInside(const glm::vec3 & point, const ofMesh & mesh){

	// check mesh mode (only OF_PRIMITIVE_TRIANGLES works correctly)
	static bool warning_given;
	if(mesh.getMode() != OF_PRIMITIVE_TRIANGLES){
		if(!warning_given){
			cout << "ERROR: ofMesh mode should be OF_PRIMITIVE_TRIANGLES" << endl;
			warning_given = true;
		}
		return false;
	}

	glm::vec3 foundIntersection; // variable to store a single found intersection
	vector <glm::vec3> results; // vector to store all found intersections
	glm::vec3 randomDirection = glm::vec3(0.1, 0.2, 0.3); // a random direction

	// go over all faces in the mesh
	const vector <ofMeshFace> & faces = mesh.getUniqueFaces();
	for(int i = 0; i < faces.size(); i++){
		// do a triangle-ray intersection on each face in the mesh
		// store the intersection (if any) in the variable foundIntersection
		if(triangleIntersection(faces[i], point, randomDirection, foundIntersection)){
			// store all found intersections
			results.push_back(foundIntersection);
		}
	}

	// handle multiple mesh intersections at the same point (by removing duplicates)
	vector <glm::vec3> unique_results;
	unique_copy(results.begin(), results.end(), back_inserter(unique_results));

	// determine if the point is inside or outside the mesh, based on the number of unique intersections
	if(unique_results.size() % 2 == 1){
		return true;
	}
	else{
		return false;
	}
}

// Möller–Trumbore: http://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool ofxPointInMesh::triangleIntersection(const ofMeshFace & face, const glm::vec3 & O, const glm::vec3 & D, glm::vec3 & R){
	const glm::vec3 & V1 = face.getVertex(0);
	const glm::vec3 & V2 = face.getVertex(1);
	const glm::vec3 & V3 = face.getVertex(2);

	glm::vec3 e1, e2; //Edge1, Edge2
	glm::vec3 P, Q, T;
	float det, inv_det, u, v;
	float t;

	//Find vectors for two edges sharing V1
	e1 = V2 - V1;
	e2 = V3 - V1;
	//Begin calculating determinant - also used to calculate u parameter
	P = glm::cross(D, e2);
	//if determinant is near zero, ray lies in plane of triangle
	det = glm::dot(e1, P);
	//NOT CULLING
	if(det > -EPSILON && det < EPSILON){
		return false;
	}
	inv_det = 1.f / det;

	//calculate distance from V1 to ray origin
	T = O - V1;

	//Calculate u parameter and test bound
	u = glm::dot(T, P) * inv_det;
	//The intersection lies outside of the triangle
	if(u < 0.f || u > 1.f){
		return false;
	}

	//Prepare to test v parameter
	Q = glm::cross(T, e1);

	//Calculate V parameter and test bound
	v = glm::dot(D, Q) * inv_det;
	//The intersection lies outside of the triangle
	if(v < 0.f || u + v  > 1.f){
		return false;
	}

	t = glm::dot(e2, Q) * inv_det;

	if(t > EPSILON){ //ray intersection
		R = O + t * D; // store intersection point
		return true;
	}

	// No hit, no win
	return false;
}
