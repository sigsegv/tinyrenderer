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
    vector3f t0 = pts[0]; t0.z = 0;
    vector3f t1 = pts[1]; t1.z = 0;
    vector3f t2 = pts[2]; t2.z = 0;
    
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
    vector3f p{0.0, 0.0, 0.0};
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
                //if(p.z < 0 || p.z > 255)
                //{
                //    std::cout << p.z << " ";
                //}
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

/*
 // old code. keeping as probably useful for wire frame rendering.
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
*/
