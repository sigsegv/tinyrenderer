#include "our_gl.hpp"

matrix44f gl_modelview;
matrix44f gl_viewport;
matrix44f gl_projection;

void gl_view_port(unsigned x, unsigned y, unsigned w, unsigned h)
{
    constexpr float kDepthHalf = 255.f / 2.f;
    matrix44f m = matrix44f::identity();
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = kDepthHalf;
    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = kDepthHalf;
    gl_viewport = m;
}

void gl_look_at(const vector3f& eye, const vector3f& centre, const vector3f& up)
{
    const vector3f z = (eye - centre).unit();
    const vector3f x = up.cross(z).unit();
    const vector3f y = z.cross(x).unit();
    matrix44f minv = matrix44f::identity();
    matrix44f tr = matrix44f::identity();
    for(int i=0; i<3; ++i)
    {
        minv[0][i] = x[i];
        minv[1][i] = y[i];
        minv[2][i] = z[i];
        tr[i][3] = -centre[i];
    }
    gl_modelview = minv*tr;
    
    constexpr float coeff = -1.f;
    gl_projection  = matrix44f::identity();
    gl_projection [3][2] = coeff/(eye-centre).magnitude();
}

void gl_triangle(const std::array<vector3f, 3>& pts, ishader& shader, TGAImage& buffer, TGAImage& zbuffer)
{
    vector3i t0 = vec3f_to_vec3i(pts[0]); t0.z = 0;
    vector3i t1 = vec3f_to_vec3i(pts[1]); t1.z = 0;
    vector3i t2 = vec3f_to_vec3i(pts[2]); t2.z = 0;
    
    const int width = buffer.get_width();
    vector2f bboxmin{ std::numeric_limits<float>::max(),  std::numeric_limits<float>::max()};
    vector2f bboxmax{-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max()};
    vector2f clamp{static_cast<float>(buffer.get_width()-1), static_cast<float>(buffer.get_height()-1)};
    for(int i=0; i<3; ++i)
    {
        for(int j=0; j<2; ++j)
        {
            int val = pts[i][j];
            bboxmin[j] = std::max(0.f, std::min(bboxmin[j], static_cast<float>(val)));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], static_cast<float>(val)));
        }
    }
    vector3i p{0, 0,  0};
    for(p.x = bboxmin.x; p.x <= bboxmax.x; ++p.x)
    {
        for(p.y = bboxmin.y; p.y <= bboxmax.y; ++p.y)
        {
            vector3f bary = barycentric3(t0, t1, t2, p);
            if(bary.x < 0.f || bary.y < 0.f || bary.z < 0.f) continue;
            p.z = 0;
            for(int i=0; i<3; ++i)
            {
                p.z += pts[i][2]*bary[i];
            }
            const int iz = static_cast<int>(p.x + p.y * width);
            if(zbuffer.buffer()[iz] < p.z)
            {
                if(p.z < 0 || p.z > 255)
                {
                    std::cout << p.z << " ";
                }
                zbuffer.buffer()[iz] = p.z;
                TGAColor color;
                if(!shader.fragment(bary, color))
                {
                    buffer.set(p.x, p.y, color);
                }
            }
        }
    }
}
