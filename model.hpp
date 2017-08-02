#pragma once

#include <string>
#include <vector>
#include <array>

struct vertex
{
    float x,y,z;
};

struct vertex_texture
{
    float u,v;
};

struct vertex_normal
{
    float x,y,z;
};

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
    
    std::vector< vertex > v;
    std::vector< vertex_texture > vt;
    std::vector< vertex_normal > vn;
    std::vector< face > f;
private:
};
