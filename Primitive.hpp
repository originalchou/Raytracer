// Fall 2018

#pragma once

#include <glm/glm.hpp>
#include "A4.hpp"

class Primitive {
public:
  virtual Intersection intersect(Ray *ray);
  virtual ~Primitive();
};

class Sphere : public Primitive {
public:
  Primitive *obj;
  Sphere();
  virtual Intersection intersect(Ray *ray);
  virtual ~Sphere();
};

class Cube : public Primitive {
public:
  Primitive *obj;
  Cube();
  virtual Intersection intersect(Ray *ray);
  virtual ~Cube();
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::dvec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual Intersection intersect(Ray *ray);
  virtual ~NonhierSphere();

private:
  glm::dvec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::dvec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
      len_x = size;
      len_y = size;
      len_z = size;
  }
  NonhierBox(const glm::dvec3& pos, double len_x, double len_y, double len_z)
    : m_pos(pos), m_size(len_x)
  {
     this->len_x = len_x;
     this->len_y = len_y;
     this->len_z = len_z;
  }
  virtual Intersection intersect(Ray *ray);
  virtual ~NonhierBox();

private:
  glm::dvec3 m_pos;
  double m_size;
  double len_x;
  double len_y;
  double len_z;
};
