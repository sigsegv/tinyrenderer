#define _USE_MATH_DEFINES
#include <math.h>
#include <SFML/Graphics.hpp>
#include <shader_phong.hpp>

model model1;
TGAImage texture;

int main()
{
    model1.load_from_disk("assets/african_head.obj");
    model1.load_diffuse_map_from_disk("assets/african_head_diffuse.tga");
    model1.load_normal_map_from_disk("assets/african_head_nm.tga");
    model1.load_specular_map_from_disk("assets/african_head_spec.tga");

    //model1.load_from_disk("assets/diablo3_pose.obj");
    //model1.load_diffuse_map_from_disk("assets/diablo3_pose_diffuse.tga");
    //model1.load_normal_map_from_disk("assets/diablo3_pose_nm.tga");
    //model1.load_specular_map_from_disk("assets/diablo3_pose_spec.tga");

    constexpr int width = 600;
    constexpr int height = 600;

    // lesson6
    vector3f eye = { 1.f, 1.f, 3.f };
    vector3f centre = { 0.f, 0.f, 0.f };
    vector3f up = { 0.f, 1.f, 0.f };

    gl_view_port(width / 8, height / 8, width * 3 / 4, height * 3 / 4);

    TGAImage buffer(width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

    phong_shader shader;
    shader.gl_light_dir = { 1.f, 1.f, 1.f };
    shader.gl_light_dir.unit();
    shader.p_model = &model1;
    
    //buffer.write_tga_file("out-test-01.tga");

    sf::RenderWindow window(sf::VideoMode(width, height), "TinyRenderer");

    sf::Uint8* rgba32_buffer = new sf::Uint8[width * height * 4];
    //std::memset(rgba32_buffer, 0xFF, width * height * 4);

    sf::Texture sftexture;
    sftexture.create(width, height);
    
    sf::Sprite sprite;
    sprite.setTexture(sftexture);

    double camera_angle_deg = 90.0;
    double camera_distance = 4.0;
    double light_angle = 0.0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                camera_angle_deg += 4.0;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            {
                camera_angle_deg -= 4.0;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                //if(camera_distance > 0.3) camera_distance -= 0.2;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                //if (camera_distance < 100.0) camera_distance += 0.2;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                light_angle += 4.0;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                light_angle -= 4.0;
            }
        }

        buffer.clear();
        zbuffer.clear();

        shader.gl_light_dir.x = std::cos(light_angle / 180.0 * M_PI);
        shader.gl_light_dir.z = std::sin(light_angle / 180.0 * M_PI);
        shader.gl_light_dir.unit();
        //std::cout << shader.gl_light_dir << std::endl;
        eye.x = camera_distance * std::cos(camera_angle_deg / 180.0 * M_PI);
        eye.z = camera_distance * std::sin(camera_angle_deg / 180.0 * M_PI);
        
        gl_look_at(eye, centre, up);
        shader.uniform_m = gl_viewport * gl_projection * gl_modelview;
        shader.uniform_mit = (gl_modelview).inverse().transpose();
        for (unsigned i = 0; i < model1.f.size(); ++i)
        {
            std::array<vector3f, 3> screen_coords;
            for (unsigned j = 0; j < 3; ++j)
            {
                screen_coords[j] = shader.vertex(i, j);
            }
            gl_triangle(screen_coords, shader, buffer, zbuffer);
        }

        window.clear(sf::Color::Blue);

        const unsigned stride = width * 4;

        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                const unsigned offset = (height - y - 1) * stride + (x * 4);
                const TGAColor color = buffer.get(x,y);
                rgba32_buffer[offset] = color.r;
                rgba32_buffer[offset + 1] = color.g;
                rgba32_buffer[offset + 2] = color.b;
                rgba32_buffer[offset + 3] = 0xFF;
            }
        }

        sftexture.update(rgba32_buffer);
        
        window.draw(sprite);

        window.display();
    }

    return 0;
}