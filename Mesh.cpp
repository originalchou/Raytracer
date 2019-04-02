// Fall 2018

#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
    double min_x = INT_MAX, min_y = INT_MAX, min_z = INT_MAX;
    double max_x = INT_MIN, max_y = INT_MIN, max_z = INT_MIN;
	size_t s1, s2, s3;

	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
            min_x = glm::min(vx, min_x);
            min_y = glm::min(vy, min_y);
            min_z = glm::min(vz, min_z);
            max_x = glm::max(vx, max_x);
            max_y = glm::max(vy, max_y);
            max_z = glm::max(vz, max_z);
            m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
    bounding = new NonhierBox(glm::dvec3(min_x, min_y, min_z), max_x-min_x, max_y-min_y, max_z-min_z);
}

Intersection Mesh::intersect(Ray *ray) {
    Intersection its = bounding->intersect(ray);
    return its;
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*
  
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}
