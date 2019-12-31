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
#include "shader_phong.hpp"

const TGAColor white(255, 255, 255, 255);
const TGAColor red(255, 0, 0, 255);
const TGAColor green(0, 255, 0, 255);
const TGAColor blue(0, 0, 255, 255);

model model1;
TGAImage texture;

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
    
    TGAImage buffer(width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);
    
    phong_shader shader;
    shader.gl_light_dir = { 1.f, 1.f, 1.f };
    shader.gl_light_dir.unit();
    shader.p_model = &model1;
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
