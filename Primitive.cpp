// Fall 2018

#include "Primitive.hpp"
#include "polyroots.hpp"

Intersection Primitive::intersect(Ray *ray) {
    return Intersection(ray);
}


Primitive::~Primitive()
{
}

Sphere::Sphere() {
    obj = new NonhierSphere(glm::vec3(0, 0, 0), 1.0);
}

Intersection Sphere::intersect(Ray *ray){
    return obj->intersect(ray);
}

Sphere::~Sphere()
{
    delete obj;
}

Cube::Cube() {
    obj = new NonhierBox(glm::vec3(0, 0, 0), 1.0);
}

Intersection Cube::intersect(Ray *ray) {
    return obj->intersect(ray);
}

Cube::~Cube()
{
    delete obj;
}


Intersection NonhierSphere::intersect(Ray *ray) {
    Intersection intersect(ray);
    const glm::dvec3 o(ray->origin);
    const glm::dvec3 d(ray->direction);
    const glm::dvec3 c(o-m_pos);

    double A = glm::dot(d, d);
    double B = 2 * glm::dot(d, c);
    double C = glm::dot(c, c) - m_radius * m_radius;

    double roots[2];
    size_t num = quadraticRoots(A, B, C, roots);
//    std::cout << B << std::endl;
    if (num == 0) {
        intersect.hit = false;
    } else if (num == 1) {
        intersect.t = roots[0];
        if (intersect.t <= epi) {
            intersect.hit = false;
        } else {
            intersect.hit = true;
        }
    } else if (num == 2) {
        double tempT = std::min(roots[0], roots[1]);
        if (tempT <= epi) {
            tempT = std::max(roots[0], roots[1]);
        }
        if (tempT <= epi) {
            intersect.hit = false;
        } else {
//            std::cout << "running2" << std::endl;
            intersect.t = tempT;
            intersect.hit = true;
        }
    } else {
        intersect.hit = false;
    }
    if (intersect.hit) {
        intersect.p = intersect.ray->origin + intersect.t * intersect.ray->direction;
        intersect.n = glm::normalize(intersect.p - glm::dvec4(m_pos, 1.0));
    }
    return intersect;
}

NonhierSphere::~NonhierSphere()
{
}

//algorithm bases on http://ray-tracing-conept.blogspot.com/2015/01/ray-box-intersection-and-normal.html
Intersection NonhierBox::intersect(Ray *ray) {
    Intersection its(ray);
    glm::dvec3 min_corner = m_pos;
    glm::dvec3 max_corner = m_pos + glm::dvec3(len_x, len_y, len_z);
    glm::dvec3 ray_o(ray->origin);
    glm::dvec3 ray_d(ray->direction);
    double tmin, tmax, tymin, tymax, tzmin, tzmax;
    if (ray_d.x > 0) {
        tmin = (min_corner.x - ray_o.x) / ray_d.x;
        tmax = (max_corner.x - ray_o.x) / ray_d.x;
    } else {
        tmin = (max_corner.x - ray_o.x) / ray_d.x;
        tmax = (min_corner.x - ray_o.x) / ray_d.x;
    }
    if (ray_d.y > 0) {
        tymin = (min_corner.y - ray_o.y) / ray_d.y;
        tymax = (max_corner.y - ray_o.y) / ray_d.y;
    } else {
        tymin = (max_corner.y - ray_o.y) / ray_d.y;
        tymax = (min_corner.y - ray_o.y) / ray_d.y;
    }
    if ((tmin > tymax) || (tymin > tmax)) {
        its.hit = false;
        return its;
    }
    if (tymin > tmin) {
        tmin = tymin;
    }
    if (tymax < tmax) {
        tmax = tymax;
    }
    if (ray_d.z > 0) {
        tzmin = (min_corner.z - ray_o.z) / ray_d.z;
        tzmax = (max_corner.z - ray_o.z) / ray_d.z;
    } else {
        tzmin = (max_corner.z - ray_o.z) / ray_d.z;
        tzmax = (min_corner.z - ray_o.z) / ray_d.z;
    }
    if ((tmin > tzmax) || (tzmin > tmax)) {
        its.hit = false;
        return its;
    }
    if (tzmin > tmin) {
        tmin = tzmin;
    }
    if (tzmax < tmax) {
        tmax = tzmax;
    }
    if (tmin > epi) {
        its.t = tmin;
        its.hit = true;
    } else if (tmax > epi) {
        its.t = tmax;
        its.hit = true;
    } else {
        its.hit = false;
        return its;
    }
    if (its.hit) {
        its.p = ray->origin + its.t * ray->direction;
        if(abs(its.p.x - min_corner.x) < epi) {
            its.n = normalize(glm::dvec4(-1, 0, 0, 0));
        }
        else if(abs(its.p.x - max_corner.x) < epi) {
            its.n = normalize(glm::dvec4(1, 0, 0, 0));
        }
        else if(abs(its.p.y - min_corner.y) < epi) {
            its.n = normalize(glm::dvec4(0, -1, 0, 0));
        }
        else if(abs(its.p.y - max_corner.y) < epi) {
            its.n = normalize(glm::dvec4(0, 1, 0, 0));
        }
        else if(abs(its.p.z - min_corner.z) < epi) {
            its.n = normalize(glm::dvec4(0, 0, -1, 0));
        }
        else if(abs(its.p.z - max_corner.z) < epi) {
            its.n = normalize(glm::dvec4(0, 0, 1, 0));
        } else {
            its.hit = false;
        }
    }
    
    return its;
}

NonhierBox::~NonhierBox()
{
}
