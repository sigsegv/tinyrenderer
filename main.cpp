#include <cassert>
#include <fstream>
#include <random>
#include <limits>
#include "tgaimage.h"
#include "model.hpp"
#include "geometry.hpp"

const TGAColor white(255, 255, 255, 255);
const TGAColor red(255, 0, 0, 255);
const TGAColor green(0, 255, 0, 255);
const TGAColor blue(0, 0, 255, 255);

void line(vector2i t0, vector2i t1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(t0.x-t1.x)<std::abs(t0.y-t1.y)) {
        std::swap(t0.x, t0.y);
        std::swap(t1.x, t1.y);
        steep = true;
    }
    if (t0.x>t1.x) {
        std::swap(t0.x, t1.x);
        std::swap(t0.y, t1.y);
    }
    int dx = t1.x-t0.x;
    int dy = t1.y-t0.y;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = t0.y;
    for (int x=t0.x; x<=t1.x; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (t1.y>t0.y?1:-1);
            error2 -= dx*2;
        }
    }
}

/*
void triangle(vector2i t0, vector2i t1, vector2i t2, TGAImage& image, TGAColor& color)
{
    // sort vertices, so t0 lowest on y-axis, and t2 is highest y-axis
    if(t0.y>t1.y) std::swap(t0, t1);
    if(t0.y>t2.y) std::swap(t0, t2);
    if(t1.y>t2.y) std::swap(t1, t2);
    line(t0, t1, image, green);
    line(t1, t2, image, green);
    line(t2, t0, image, red);
}
*/

void triangle(const std::array<vector2i, 3>& pts,  TGAImage& image, const TGAColor& color)
{
    vector2i bboxmin{image.get_width()-1, image.get_height()-1};
    vector2i bboxmax{0, 0};
    vector2i clamp = bboxmin;
    for(int i=0; i<3; ++i)
    {
        for(int j=0; j<2; ++j)
        {
            int val = pts[i][j];
            bboxmin[j] = std::max(0, std::min(bboxmin[j], val));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], val));
        }
    }
    vector2i p;
    for(p.x = bboxmin.x; p.x <= bboxmax.x; ++p.x)
    {
        for(p.y = bboxmin.y; p.y <= bboxmax.y; ++p.y)
        {
            vector3f bc_screen = barycentric(pts[0], pts[1], pts[2], p);
            if(bc_screen.x < 0.f || bc_screen.y < 0.f || bc_screen.z < 0.f) continue;
            image.set(p.x, p.y, color);
        }
    }
}

void triangle3(const std::array<vector3f, 3>& pts,  TGAImage& image, const TGAColor& color, float* zbuffer)
{
    const int width = image.get_width();
    vector2f bboxmin{ std::numeric_limits<float>::max(),  std::numeric_limits<float>::max()};
    vector2f bboxmax{-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
    vector2f clamp{static_cast<float>(image.get_width()-1), static_cast<float>(image.get_height()-1)};
    for(int i=0; i<3; ++i)
    {
        for(int j=0; j<2; ++j)
        {
            int val = pts[i][j];
            bboxmin[j] = std::max(0.f, std::min(bboxmin[j], static_cast<float>(val)));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], static_cast<float>(val)));
        }
    }
    vector3f p;
    for(p.x = bboxmin.x; p.x <= bboxmax.x; ++p.x)
    {
        for(p.y = bboxmin.y; p.y <= bboxmax.y; ++p.y)
        {
            vector3f bc_screen = barycentric3(pts[0], pts[1], pts[2], p);
            if(bc_screen.x < 0.f || bc_screen.y < 0.f || bc_screen.z < 0.f) continue;
            p.z = 0;
            for(int i=0; i<3; ++i)
            {
                p.z += pts[i][2]*bc_screen[i];
            }
            const int iz = static_cast<int>(p.x + p.y * width);
            if(zbuffer[iz] < p.z)
            {
                zbuffer[iz] = p.z;
                image.set(p.x, p.y, color);
            }
        }
    }
}

int main(int argc, char** argv)
{
    model model1;
    model1.load_from_disk("assets/african_head.obj");
    assert(model1.v.size() == 1258);
    assert(model1.vt.size() == 1339);
    assert(model1.vn.size() == 1258);
    
    const int width = 500;
    const int height = 500;
    const float half_width = width / 2.f;
    const float half_height = height / 2.f;
    
    // example 1 : wire frame
    
    TGAImage image(width, height, TGAImage::RGB);
    image.clear();
    
    for(const face& f: model1.f)
    {
        std::array<vertex, 3> vertices = {{ model1.v[f.v1], model1.v[f.v2], model1.v[f.v3] }};
        for(size_t j = 0; j < 3; ++j)
        {
            const vertex& v0 = vertices[j];
            const size_t index = (j+1)%3;
            const vertex& v1 = vertices[index];
            int x0 = (v0.x+1.)*width/2.;
            int y0 = (v0.y+1.)*height/2.;
            int x1 = (v1.x+1.)*width/2.;
            int y1 = (v1.y+1.)*height/2.;
            vector2i t0 = {x0, y0};
            vector2i t1 = {x1, y1};
            line(t0, t1, image, white);
        }
    }
    image.flip_vertically();
    image.write_tga_file("output_head_wireframe.tga");
    
    // example 2 : single filled triangle
    
    TGAImage frame(200, 200, TGAImage::RGB);
    frame.clear();
    
    std::array<vector2i, 3> pts = {{ {10,10}, {100,30}, {190, 160} }};
    triangle(pts, frame, red);
    frame.flip_vertically();
    frame.write_tga_file("framebuffer.tga");
    
    // example 3 : filled triangles with random colors
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    TGAImage frame2(width, height, TGAImage::RGB);
    frame2.clear();
    
    for(const face& f: model1.f)
    {
        vector3f& wc_v1 = model1.v[f.v1];
        vector3f& wc_v2 = model1.v[f.v2];
        vector3f& wc_v3 = model1.v[f.v3];
        std::array<vector2i, 3> sc_pts;
        sc_pts[0] = { static_cast<int>((wc_v1.x + 1.f) * half_width), static_cast<int>((wc_v1.y + 1.f) * half_height) };
        sc_pts[1] = { static_cast<int>((wc_v2.x + 1.f) * half_width), static_cast<int>((wc_v2.y + 1.f) * half_height) };
        sc_pts[2] = { static_cast<int>((wc_v3.x + 1.f) * half_width), static_cast<int>((wc_v3.y + 1.f) * half_height) };
        triangle(sc_pts, frame2, TGAColor(dis(gen), dis(gen), dis(gen), 255));
    }
    frame2.flip_vertically();
    frame2.write_tga_file("output_head_filled_colors.tga");
    
    // example 4 : filled triangles flat shaded
    
    TGAImage frame3(width, height, TGAImage::RGB);
    frame3.clear();

    vector3f light_dir = {0.f, 0.f, -1.f};

    for(const face& f: model1.f)
    {
        vector3f& wc_v1 = model1.v[f.v1];
        vector3f& wc_v2 = model1.v[f.v2];
        vector3f& wc_v3 = model1.v[f.v3];
        std::array<vector2i, 3> sc_pts;
        sc_pts[0] = { static_cast<int>((wc_v1.x + 1.f) * half_width), static_cast<int>((wc_v1.y + 1.f) * half_height) };
        sc_pts[1] = { static_cast<int>((wc_v2.x + 1.f) * half_width), static_cast<int>((wc_v2.y + 1.f) * half_height) };
        sc_pts[2] = { static_cast<int>((wc_v3.x + 1.f) * half_width), static_cast<int>((wc_v3.y + 1.f) * half_height) };
        
        vector3f n = (wc_v3 - wc_v1).cross(wc_v2 - wc_v1).unit(); // get normal of face from cross product of two of the faces side
        float light_intensity = n.dot(light_dir);
        if(light_intensity > 0.f)
        {
            const int tint = static_cast<int>(light_intensity * 255.f);
            triangle(sc_pts, frame3, TGAColor(tint, tint, tint, 255));
        }
    }
    frame3.flip_vertically();
    frame3.write_tga_file("output_head_flat_shaded.tga");
    
    // example 5 : z buffer
    
    std::array<float, width * height> zbuffer;
    zbuffer.fill(std::numeric_limits<float>::lowest());
    
    TGAImage frame4(width, height, TGAImage::RGB);
    frame4.clear();
//    unsigned char* buf = frame4.buffer();
//    unsigned char* end = buf + (width * height * 3);
//    while(buf != end)
//    {
//        *buf = 0xFF; ++buf;
//        *buf = 0x00; ++buf;
//        *buf = 0xFF; ++buf;
//    }
    
    for(const face& f: model1.f)
    {
        vector3f& wc_v1 = model1.v[f.v1];
        vector3f& wc_v2 = model1.v[f.v2];
        vector3f& wc_v3 = model1.v[f.v3];
        std::array<vector3f, 3> sc_pts;
        sc_pts[0] = { (wc_v1.x + 1.f) * half_width, (wc_v1.y + 1.f) * half_height, wc_v1.z };
        sc_pts[1] = { (wc_v2.x + 1.f) * half_width, (wc_v2.y + 1.f) * half_height, wc_v2.z };
        sc_pts[2] = { (wc_v3.x + 1.f) * half_width, (wc_v3.y + 1.f) * half_height, wc_v3.z };
        
        vector3f n = (wc_v3 - wc_v1).cross(wc_v2 - wc_v1).unit(); // get normal of face from cross product of two of the faces side
        float light_intensity = n.dot(light_dir);
        if(light_intensity > 0.f)
        {
            const int tint = static_cast<int>(light_intensity * 255.f);
            triangle3(sc_pts, frame4, TGAColor(tint, tint, tint, 255), zbuffer.data());
        }
    }
    
    frame4.flip_vertically();
    frame4.write_tga_file("output_head_flat_shaded_zbuffer.tga");
    
    return 0;
}
