#include "model.hpp"
#include <fstream>
#include <sstream>

model::model()
{
    
}

void model::load_from_disk(const std::string& filepath)
{
    std::ifstream fin(filepath);
    if(!fin.is_open()) throw std::runtime_error("unable to open file " + filepath);
    std::string line;
    while(!fin.eof())
    {
        std::getline(fin, line);
        std::istringstream iss(line);
        if(line.compare(0, 2, "vt") == 0)
        {
            iss.ignore(2, ' ');
            vertex_texture vt;
            iss >> vt.u >> vt.v;
            this->vt.push_back(vt);
        }
        else if(line.compare(0, 2, "vn") == 0)
        {
            iss.ignore(2, ' ');
            vertex_normal vn;
            iss >> vn.x >> vn.y >> vn.z;
            this->vn.push_back(vn);
        }
        else if(line.compare(0, 2, "vp") == 0)
        {
            
        }
        else if(line.compare(0, 1, "v") == 0)
        {
            iss.ignore(1, ' ');
            vertex v;
            iss >> v.x >> v.y >> v.z;
            this->v.push_back(v);
        }
        else if(line.compare(0, 1, "f") == 0)
        {
            //std::string trash;
            char trash;
            iss.ignore(1, ' ');
            face f;
            iss >> f.v1 >> trash >> f.vt1 >> trash >> f.vn1
                >> f.v2 >> trash >> f.vt2 >> trash >> f.vn2
                >> f.v3 >> trash >> f.vt3 >> trash >> f.vn3;
            // shift indexes down 1
            --f.v1;
            --f.vt1;
            --f.vn1;
            --f.v2;
            --f.vt2;
            --f.vn2;
            --f.v3;
            --f.vt3;
            --f.vn3;
            this->f.push_back(f);
        }
    }
}

TGAColor model::diffuse(const vector2f& uv)
{
    const float u = m_diffuse.get_width() * uv[0];
    const float v = m_diffuse.get_height() * uv[1];
    return m_diffuse.get(static_cast<int>(u), static_cast<int>(v));
}

TGAColor model::specular(const vector2f& uv)
{
    const int u = static_cast<int>(m_specular.get_width() * uv[0]);
    const int v = static_cast<int>(m_specular.get_height() * uv[1]);
    TGAColor color = m_specular.get(u, v);
    if (color.bytespp == 1)
    {
        return TGAColor(color.b, color.b, color.b, color.a);
    }
    return color;
}

vector3f model::normal(const vector2f& uv)
{
    const float u = m_normal.get_width() * uv[0];
    const float v = m_normal.get_height() * uv[1];
    TGAColor color = m_normal.get(static_cast<int>(u), static_cast<int>(v));
    return {static_cast<float>(color.r) / 255.0f, static_cast<float>(color.g) / 255.0f, static_cast<float>(color.b) / 255.0f};
}

//const vector3f& model::normal(int iface, int nthvert)
//{
//    face& fc = f[iface];
//    const unsigned int ni = (nthvert == 0) ? fc.vn1 : ((nthvert == 1) ? fc.vn2 : fc.vn3);
//    return vn[ni];
//}

vector2f model::get_uv(int iface, int nthvert)
{
    face& fc = f[iface];
    const unsigned int ti = (nthvert == 0) ? fc.vt1 : ((nthvert == 1) ? fc.vt2 : fc.vt3);
    return {vt[ti].u, vt[ti].v};
}

vector3f model::get_vertex(int iface, int nthvert)
{
    face& fc = f[iface];
    const unsigned int vi = (nthvert == 0) ? fc.v1 : ((nthvert == 1) ? fc.v2 : fc.v3);
    return v[vi];
}

void model::load_diffuse_map_from_disk(const std::string& filepath)
{
    if(!m_diffuse.read_tga_file(filepath.c_str()))
    {
        throw std::runtime_error("unable to open file " + filepath);
    }
    m_diffuse.flip_vertically();
}

void model::load_normal_map_from_disk(const std::string& filepath)
{
    if(!m_normal.read_tga_file(filepath.c_str()))
    {
        throw std::runtime_error("unable to open file " + filepath);
    }
    m_normal.flip_vertically();
}

void model::load_specular_map_from_disk(const std::string& filepath)
{
    if(!m_specular.read_tga_file(filepath.c_str()))
    {
        throw std::runtime_error("unable to open file " + filepath);
    }
    m_specular.flip_vertically();
}

