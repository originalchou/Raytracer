// Fall 2018

#include <glm/ext.hpp>

#include "A4.hpp"
#include "GeometryNode.hpp"
using namespace glm;

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
) {

  // Fill in raytracing code here...  

  std::cout << "Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;

    size_t h = image.height();
    size_t w = image.width();
    double d = length(view);
    double virtualH = 2.0 * d * tan(radians(fovy)/2.0);
    double virtualW = (double)w/h * virtualH;
    dmat4 Trans = glm::translate(dvec3(-(double)w/2.0, -(double)h/2.0, d));
    dmat4 S = glm::scale(dvec3(-virtualW/(double)w, -virtualH/(double)h, 1.0));
    dvec3 w_vec(normalize(view));
    dvec3 u_vec(normalize(cross(up, w_vec)));
    dvec3 v_vec(normalize(cross(w_vec, u_vec)));
    dmat4 R({
        {u_vec.x, u_vec.y, u_vec.z, 0.0},
        {v_vec.x, v_vec.y, v_vec.z, 0.0},
        {w_vec.x, w_vec.y, w_vec.z, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    });
    
    dmat4 T({
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {eye.x, eye.y, eye.z, 1.0}
    });
    
	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
            dvec4 p(x, y, 0.0, 1.0);
            dvec4 p_world = T * R * S * Trans * p;
            Ray ray(dvec4(eye, 1.0), p_world - dvec4(eye, 1.0));
            bool reached = false;
            glm::vec3 color = coloredRay(&ray, root, ambient, lights, reached);
            if (reached) {
                image(x, y, 0) = color[0];
                image(x, y, 1) = color[1];
                image(x, y, 2) = color[2];
            } else {
                image(x, y, 0) = 0.0;
                image(x, y, 1) = (double)y/(double)h;
                image(x, y, 2) = (double)y;
            }
		}
	}

}

Intersection intersect(SceneNode *root, Ray *ray) {
    Intersection its(ray);
    Material *material = nullptr;
    
    Ray transRay(root->get_inverse() * ray->origin, root->get_inverse() * ray->direction);

    if (root->m_nodeType == NodeType::GeometryNode) {
        its = static_cast<GeometryNode *>(root)->m_primitive->intersect(&transRay);
        material = static_cast<GeometryNode *>(root)->m_material;
    }
    
    if(its.hit) {
        its.phongMaterial = (PhongMaterial *)material;
    }
    
    for (SceneNode *child: root->children) {
        Intersection new_its = intersect(child, &transRay);
        
        if (new_its.hit && (!its.hit || new_its.t < its.t)) {
            its = new_its;
        }
    }
  
    if (its.hit) {
        its.p = root->get_transform() * its.p;
        dvec3 normal(its.n);
        its.n = dvec4(glm::transpose(dmat3(root->get_inverse())) * normal, 0.0);
        its.n = normalize(its.n);
    }
    return its;
}

dvec3 coloredRay(Ray *ray, SceneNode *root, const dvec3 &ambient, const std::list<Light*> &lights, bool &reach_light) {
    Intersection its = intersect(root, ray);
    dvec3 col(0.0, 0.0, 0.0);
    if (its.hit) {
        reach_light = true;
        col += (dvec3(its.phongMaterial->m_kd) * dvec3(ambient));
        dvec3 normal(its.n);
        dvec4 point = its.p;
        
        for (Light *light : lights){
            dvec3 diffuse(0.0, 0.0, 0.0);
            dvec3 specular(0.0, 0.0, 0.0);
            
            Ray shadow(point, dvec4(light->position, 1) - point);

            dvec3 shadow_dir(shadow.direction);
            dvec3 light_dir = glm::normalize(shadow_dir);
            double dis = length(dvec3(light->position) - dvec3(point));
            Intersection shadow_its = intersect(root, &shadow);
            double shadow_dis = length(shadow_its.t * shadow_dir);
            if (!shadow_its.hit || shadow_dis > dis) {
            
                //diffuse
                double l_n = glm::max(glm::dot(normal, light_dir), 0.0);
                diffuse = l_n * its.phongMaterial->m_kd;
                
                dvec3 ray_out = glm::normalize(dvec3(ray->direction));
                
                //specular
                dvec3 reflected_ray = ray_out - 2.0 * normal * dot(ray_out, normal);
                
                double r_l = glm::max(dot(reflected_ray, light_dir), 0.0);
                
                specular = its.phongMaterial->m_ks * pow(r_l, its.phongMaterial->m_shininess);
                
                col = col + dvec3(light->colour) * (diffuse + specular) /
                    (light->falloff[0] + light->falloff[1] * dis + light->falloff[2] * pow(dis, 2));
            }
        }
    }
    return col;
}
