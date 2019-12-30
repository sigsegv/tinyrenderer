#pragma once

#include "our_gl.hpp"

vector3f normalize_color(const TGAColor& color)
{
    return{ color[0] / 255.f, color[1] / 255.f, color[2] / 255.f };
}

class phong_shader : public ishader
{
public:
    matrix<float, 2, 3> varying_uv; // same as above

    matrix44f uniform_m;   // projection*modelview
    matrix44f uniform_mit; // (projection*modelview).invert_transpose()

    virtual vector3f vertex(unsigned iface, unsigned nthvert) override
    {
        varying_uv.set_col(nthvert, vec2f_to_mat12f(p_model->get_uv(iface, nthvert)));
        vector3f vertex = p_model->get_vertex(iface, nthvert);
        matrix41f v = vec3f_to_mat41f(vertex);
        v = uniform_m * v;
        return mat41f_to_vec3f(v);
    }

    virtual bool fragment(const vector3f& bar, TGAColor& color) override
    {
        const matrix31f mat_bar = vec3f_to_mat31f(bar);
        const matrix21f uv = varying_uv*mat_bar;
        const vector3f n = mat41f_to_vec3f(uniform_mit * vec3f_to_mat41f(p_model->normal(mat21f_to_vec2f(uv)))).unit();
        const vector3f l = mat41f_to_vec3f(uniform_m * vec3f_to_mat41f(gl_light_dir)).unit();
        const vector3f r = (n * (n.dot(l)*2.f) - l).unit(); // reflected light

        vector3f spec_color = normalize_color(p_model->specular(mat21f_to_vec2f(uv)));
        const float spec_coef = 2.f;

        vector3f ambient_color{ 0.3f, 0.3f, 0.3f };
        const float ambient_coef = 0.2f;

        vector3f diffuse_color = normalize_color(p_model->diffuse(mat21f_to_vec2f(uv)));
        const float diffuse_coef = 1.f;

        const float attenuation_coef = 1.f;

        vector3f light_color = { 0.5f, 0.5f, 0.5f };

        const float shine = 200.f;

        for (int i = 0; i<3; ++i)
        {
            const float a = ambient_color[i] * ambient_coef * diffuse_color[i];
            const float b = attenuation_coef * light_color[i] * n.dot(l);
            const float c = diffuse_coef * diffuse_color[i] * n.dot(l);
            const float d = spec_coef * spec_color[i] * std::pow(r.z, shine);
            const float y = a + b * (c + d);
            const float z = y * 255.f;
            color[i] = std::min(255, static_cast<int>(z));
        }
        //color[3] = 255;
        return false;
    }
};
