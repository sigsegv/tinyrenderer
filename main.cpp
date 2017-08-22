#include <cassert>
#include <fstream>
#include "tgaimage.h"
#include "model.hpp"

struct vector2i
{
    int x, y;
};

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

int main(int argc, char** argv)
{
    model model1;
    model1.load_from_disk("assets/african_head.obj");
    assert(model1.v.size() == 1258);
    assert(model1.vt.size() == 1339);
    assert(model1.vn.size() == 1258);
    
    const int width = 500;
    const int height = 500;
    
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
            struct vector2i t0 = {x0, y0};
            struct vector2i t1 = {x1, y1};
            line(t0, t1, image, white);
        }
    }
    image.flip_vertically();
    image.write_tga_file("output_head_wireframe.tga");
    
    return 0;
}
