
/** TP 5ETI IMI- CPE Lyon - 2015-2016 */



#include "lib/common/error_handling.hpp"

#include "discrete/bresenham.hpp"
#include "discrete/line_discrete.hpp"
#include "discrete/line_interpolation_parameter.hpp"
#include "image/image.hpp"
#include "image/drawer.hpp"
#include "discrete/triangle_scanline.hpp"
#include "image/image_zbuffer.hpp"
#include "lib/mesh/mesh.hpp"
#include "lib/mesh/mesh_io.hpp"
#include "render_engine/render_engine.hpp"
#include "lib/3d/mat4.hpp"
#include "lib/3d/mat3.hpp"
#include "image/texture.hpp"
#include "lib/common/basic_functions.hpp"
#include <iostream>
#include <fstream>
#include <cmath>



using namespace cpe;

int main(int argc,char *argv[])
{

    std::cout<<"***********************"<<std::endl;
    std::cout<<"run "<<argv[0]<<" with "<<argc-1<<" parameters ... \n"<<std::endl;


    try
    {
        int const Nx = 400;
        int const Ny = 250;

        image im(Nx,Ny);
        im.fill({1.0f,1.0f,1.0f});

        im(20,10) = color(1.0f,1.0f,0.0f);
        im(21,10) = color(1.0f,0.0f,0.0f);

        for(int k=0;k<20;++k)
            im(10+k,5) = color(k/20.0f,0.0f,1.0f-k/20.0f);

        for(int i=0;i<20;++i)
        {
            float scale = i/20.0f;
            im(5,2+i) = color(scale,scale,scale);
        }

        line_discrete line;
        line.push_back(ivec2(4,5)); // on ajoute une position avec des ivec2
        line.push_back({5,6});
        ivec2 const p(6,7); // faire attention au const
        line.push_back(p);
        //std::cout<<line<<std::endl;

        for (ivec2 const& p : line)
        {
            im(p) = color(1.0f,0.0f,1.0f);
        }

        ivec2 const p0(20,10);
        ivec2 const p1(30,9);
        ivec2 const p2(25,15);
        //draw_triangle_wireframe(im,p0,p1,p2,color(0.0f,0.0f,1.0f));
        //draw_triangle_wireframe(im,{15,12},{2,17},{6,3},{0,0,0});
        image_zbuffer zbuf(im.size());
        draw_triangle(im,zbuf,{15,12},{2,17},{6,3},{0,0,1},{0,1,0},{1,0,0},-10.10f,-10.5f,-10.3f);
        //auto scanline=triangle_scanline_factory({15,12},{2,17},{6,3},color(1,0,0),color(0,1,0),color(0,0,1));
        mat4 model; //identitee
        mat4 view; //identitee
        mat4 projection;

        //model.set_rotation(vec3(0,0,1),60*M_PI/180.0f);
        //view.set_rotation(vec3(1,1,1),30*M_PI/180.0f);
        //matrice de projection angle de vue de 60 degres,
        // image de taille carree,
        // sommets visibles entre z=0.1 et z=20.
        projection.set_projection_perspective(60*M_PI/180.0f,1.0f,0.1f,20.0f);
//        render(im,zbuf, vec3(0,0,-2), vec3(1,0,-2), vec3(0,1,-2),
//                     {0,0,1},{0,1,0},{1,0,0},
//                     vec3(0,0,1), vec3(0,0,1), vec3(0,0,1),
//                     model,view,projection
//                    );


        //chargement du fichier
        mesh m = load_mesh_file("data/dino.obj");
        cpe::texture t;
        //t.load("data/plane.ppm");
        t.load("data/dino.ppm");
        //applique potentiellement une rotation, translation
        // (scaling possible egalement)
        m.transform_apply_rotation({0.5,1,0},-M_PI/4.0f);
        m.transform_apply_translation({0,0,-1.5f});
        m.fill_color({1,0,1}); //applique la couleur blanche a
        // l'ensemble des sommets
        m.fill_normal(); //calcul automatique des normales

        render(im,zbuf, m, model,view,projection,t);
        //im.save("mon_image.ppm");

//        m = load_mesh_file("data/plane.obj");
//        m.transform_apply_scale(0.7f);
//        m.transform_apply_translation({0,0.34f,-1.5f});
//        m.transform_opposite_normal_orientation();

        //render(im,zbuf,m,model,view,projection,t);
        im.save("mon_image.ppm");

    }
    catch(cpe::exception_cpe const& e)
    {


        std::cout<<"**********************************"<<std::endl;
        std::cout<<"**********************************"<<std::endl;
        std::cout<<"Exception found"<<std::endl;
        std::cout<<e.info()<<std::endl;
        std::cout<<"\nSaving backtrace in file backtrace_log.txt"<<std::endl;
        std::cout<<"**********************************"<<std::endl;
        std::cout<<"**********************************"<<std::endl;


        std::ofstream back_file("backtrace_log.txt");
        back_file<<e.info()<<std::endl;
        back_file<<e.info_backtrace();
        back_file.close();

        exit(1);
    }


    std::cout<<"Exit Main"<<std::endl;

    return 0;
}
