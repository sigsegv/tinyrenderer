#include <cassert>
#include <fstream>
#include "tgaimage.h"
#include "model.hpp"

const TGAColor white(255, 255, 255, 255);
const TGAColor red(255, 0, 0, 255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}

int main(int argc, char** argv)
{
    TGAImage image1(100, 100, TGAImage::RGB);
    line(13, 20, 80, 40, image1, white);
    image1.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image1.write_tga_file("output.tga");
    
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
            line(x0,y0,x1,y1,image,white);
        }
    }
    image.flip_vertically();
    image.write_tga_file("output_head.tga");
    
    return 0;
}
