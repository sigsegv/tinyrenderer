#include <cassert>
#include <cstring>
#include <fstream>
#include <random>
#include <limits>
#include <iostream>
#include "tgaimage.h"
#include "model.hpp"
#include "geometry.hpp"
#include "matrix.hpp"
#include "our_gl.hpp"


const TGAColor white(255, 255, 255, 255);
const TGAColor red(255, 0, 0, 255);
const TGAColor green(0, 255, 0, 255);
const TGAColor blue(0, 0, 255, 255);

model model1;
TGAImage texture;
vector3f gl_light_dir = {1.f, 1.f, 1.f};

vector3f normalize_color(const TGAColor& color)
{
    return{ color[0] / 255.f, color[1] / 255.f, color[2] / 255.f };
}

class phong_shader : public ishader
{
public:
    matrix<float,2,3> varying_uv; // same as above
    
    matrix44f uniform_m;   // projection*modelview
    matrix44f uniform_mit; // (projection*modelview).invert_transpose()
    
    virtual vector3f vertex(unsigned iface, unsigned nthvert) override
    {
        varying_uv.set_col(nthvert, vec2f_to_mat12f(model1.get_uv(iface, nthvert)));
        vector3f vertex = model1.get_vertex(iface, nthvert);
        matrix41f v = vec3f_to_mat41f(vertex);
        v = uniform_m * v;
        return mat41f_to_vec3f(v);
    }

    virtual bool fragment(const vector3f& bar, TGAColor& color) override
    {
        const matrix31f mat_bar = vec3f_to_mat31f(bar);
        const matrix21f uv = varying_uv*mat_bar;
        const vector3f n = mat41f_to_vec3f(uniform_mit * vec3f_to_mat41f(model1.normal(mat21f_to_vec2f(uv)))).unit();
        const vector3f l = mat41f_to_vec3f(uniform_m * vec3f_to_mat41f(gl_light_dir)).unit();
        const vector3f r = (n * (n.dot(l)*2.f) - l).unit(); // reflected light

        vector3f spec_color = normalize_color(model1.specular(mat21f_to_vec2f(uv)));
        const float spec_coef = 2.f;

        vector3f ambient_color{ 0.3f, 0.3f, 0.3f };
        const float ambient_coef = 1.f;
        
        vector3f diffuse_color = normalize_color(model1.diffuse(mat21f_to_vec2f(uv)));
        const float diffuse_coef = 1.f;

        const float attenuation_coef = 1.f;

        vector3f light_color = { 0.5f, 0.5f, 0.5f };
        
        const float shine = 200.f;
       
        for (int i = 0; i<3; ++i)
        {
            const float a = ambient_color[i] * ambient_coef * diffuse_color[i];
            const float b = attenuation_coef * light_color[i];
            const float c = diffuse_coef * diffuse_color[i] * n.dot(l);
            const float d = spec_coef * spec_color[i] * std::pow(r.z, shine);
            const float y = a + b * (c + d);
            const float z = y * 255.f;
            color[i] = std::min(255, static_cast<int>(z));
        }
        return false;
    }
};

int main(int argc, char** argv)
{
    model1.load_from_disk("assets/african_head.obj");
    model1.load_diffuse_map_from_disk("assets/african_head_diffuse.tga");
    model1.load_normal_map_from_disk("assets/african_head_nm.tga");
    model1.load_specular_map_from_disk("assets/african_head_spec.tga");
    
    constexpr int width  = 800;
    constexpr int height = 800;
    
    // lesson6
    vector3f eye = {1.f, 1.f, 3.f};
    vector3f centre = {0.f, 0.f, 0.f};
    vector3f up = {0.f, 1.f, 0.f};
    
    gl_look_at(eye, centre, up);
    gl_view_port(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
    gl_light_dir = gl_light_dir.unit();
    
    TGAImage buffer(width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);
    
    phong_shader shader;
    shader.uniform_m = gl_viewport * gl_projection * gl_modelview;
    shader.uniform_mit = (gl_modelview).inverse().transpose();
    for(unsigned i = 0; i < model1.f.size(); ++i)
    {
        std::array<vector3f, 3> screen_coords;
        for(unsigned j = 0; j < 3; ++j)
        {
            screen_coords[j] = shader.vertex(i, j);
        }
        gl_triangle(screen_coords, shader, buffer, zbuffer);
    }
    
    buffer.flip_vertically();
    zbuffer.flip_vertically();
    buffer.write_tga_file("lesson6_buffer.tga");
    zbuffer.write_tga_file("lesson6_zbuffer.tga");
    
    return 0;
}
