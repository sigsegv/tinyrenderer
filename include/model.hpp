#pragma once

#include <string>
#include <vector>
#include <array>
#include "geometry.hpp"
#include "tgaimage.h"

//struct vertex
//{
//    float x,y,z;
//};
using vertex = vector3f;
//using vertex_texture = vector2f;
using vertex_normal = vector3f;

struct vertex_texture
{
    float u,v;
};

//struct vertex_normal
//{
//    float x,y,z;
//};

struct face
{
    unsigned int v1, vt1, vn1;
    unsigned int v2, vt2, vn2;
    unsigned int v3, vt3, vn3;
};

class model
{
public:
    model();
    void load_from_disk(const std::string& filepath);
    void load_diffuse_map_from_disk(const std::string& filepath);
    void load_normal_map_from_disk(const std::string& filepath);
    void load_specular_map_from_disk(const std::string& filepath);
    
    TGAColor diffuse(const vector2f& uv);
    TGAColor specular(const vector2f& uv);
    vector3f normal(const vector2f& uv);
    const vector3f& normal(int iface, int nthvert);
    vector2f get_uv(int iface, int nthvert);
    vector3f get_vertex(int iface, int nthvert);
    
    std::vector< vertex > v;
    std::vector< vertex_texture > vt;
    std::vector< vertex_normal > vn;
    std::vector< face > f;
private:
    TGAImage m_diffuse;
    TGAImage m_normal;
    TGAImage m_specular;
};
