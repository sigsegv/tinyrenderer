#pragma once

#include "vector3.hpp"
#include "matrix.hpp"
#include "tgaimage.h"
#include "model.hpp"
#include "geometry.hpp"

using vector3f = vector3<float>;

extern matrix44f gl_modelview;
extern matrix44f gl_viewport;
extern matrix44f gl_projection;

class ishader
{
public:
    /**
     * Primary:  transform coordinates of vertices
     * Seconday: prepare fragment shader
     */
    virtual vector3f vertex(unsigned iface, unsigned nthvert) = 0;
    
    /**
     * Primary:  determine color of current pixel
     * Seconday: discard pixel by returning trueå
     */
    virtual bool fragment(const vector3f& bary, TGAColor& color) = 0;
    
    virtual ~ishader() {};

    model* p_model;
    vector3f gl_light_dir;
};

void gl_view_port(unsigned x, unsigned y, unsigned w, unsigned h);
void gl_look_at(const vector3f& eye, const vector3f& centre, const vector3f& up);
void gl_triangle(const std::array<vector3f, 3>& pts, ishader& shader, TGAImage& buffer, TGAImage& zbuffer);
