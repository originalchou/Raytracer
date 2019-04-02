// Fall 2018

#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "PhongMaterial.hpp"

const double epi = 1e-4;

class Ray{
public:
    glm::dvec4 origin;
    glm::dvec4 direction;
    Ray(glm::dvec4 origin, glm::dvec4 direction):origin(origin), direction(direction) {}
    Ray(glm::dvec3 origin, glm::dvec3 direction):Ray((glm::dvec4(origin, 1.0)),
        glm::dvec4(direction, 0.0)) {}
};

class Intersection{
public:
    glm::dvec4 p;
    double t;
    glm::dvec4 n;
    bool hit;
    PhongMaterial *phongMaterial;
    Ray *ray;
    Intersection(Ray *ray):ray(ray) {}
};

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::dvec3 & eye,
		const glm::dvec3 & view,
		const glm::dvec3 & up,
		double fovy,

		// Lighting parameters
		const glm::dvec3 & ambient,
		const std::list<Light *> & lights
);

Intersection intersect(SceneNode *root, Ray *ray);
glm::dvec3 coloredRay(Ray *ray, SceneNode *root, const glm::dvec3 &ambient, const std::list<Light*> &lights, bool &flag);
