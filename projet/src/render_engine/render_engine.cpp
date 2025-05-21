/*
**    TP CPE Lyon
**    Copyright (C) 2015 Damien Rohmer
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**   This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "image/texture.hpp"

#include "render_engine.hpp"

#include "image/image.hpp"
#include "image/color.hpp"
#include "image/image_zbuffer.hpp"
#include "lib/3d/vec3.hpp"
#include "lib/3d/vec4.hpp"
#include "lib/3d/mat4.hpp"
#include "image/drawer.hpp"
#include "lib/mesh/mesh.hpp"
#include "lib/common/basic_functions.hpp"

#include <cmath>
#include <algorithm>
namespace cpe
{

// convert the (x,y) coordinate of a vec3 in discrete pixel position in an image of size (Nx,Ny)
static ivec2 coordinates_to_pixel_index(vec3 const& coord,int const Nx,int const Ny)
{
    int const x = (coord.x()+1.0f)/2.0f * Nx;
    int const y = (coord.y()+1.0f)/2.0f * Ny;

    return ivec2(x,y);
}

//helper function converting a vec3 (x,y,z) into a color (r,g,b)
// (a mesh structure store a color as a vec3)
static color to_color(vec3 const& p)
{
    return color(p.x(),p.y(),p.z());
}


void render(image& im,image_zbuffer& zbuffer,
            vec3 const& p0,vec3 const& p1,vec3 const& p2,
            color const& c0,color const& c1,color const& c2,
            vec3 const& n0,vec3 const& n1,vec3 const& n2,
            mat4 const& model,mat4 const& view,mat4 const& projection
            )
{
    vec3 p0_proj,p1_proj,p2_proj;
    color c0_shading,c1_shading,c2_shading;

    //apply vertex shader on the 3 vertices
    vertex_shader(p0_proj,c0_shading , p0,c0,n0 , model,view,projection);
    vertex_shader(p1_proj,c1_shading , p1,c1,n1 , model,view,projection);
    vertex_shader(p2_proj,c2_shading , p2,c2,n2 , model,view,projection);

    //p0_proj=model * view * projection * p0;
    //p1_proj=model * view * projection * p1;
    //p2_proj=model * view * projection * p2;
    //c0_shading = c0;
    //c1_shading = c1;
    //c2_shading = c2;
    //convert normalized coordinates to pixel coordinate
    ivec2 u0 = coordinates_to_pixel_index(p0_proj,im.Nx(),im.Ny());
    ivec2 u1 = coordinates_to_pixel_index(p1_proj,im.Nx(),im.Ny());
    ivec2 u2 = coordinates_to_pixel_index(p2_proj,im.Nx(),im.Ny());

    //draw triangle in the screen space
//    draw_triangle(im,zbuffer,u0,u1,u2 , c0_shading,c1_shading,c2_shading ,
//                  p0_proj.z(),p1_proj.z(),p2_proj.z() );
    draw_triangle(im,zbuffer,u0,u1,u2 , c0_shading,c1_shading,c2_shading ,
                  p0_proj.z(),p1_proj.z(),p2_proj.z(),p0,p1,p2,n0,n1,n2);
}


void render(image& im,image_zbuffer& zbuffer,
            vec3 const& p0,vec3 const& p1,vec3 const& p2,
            vec2 const& t0,vec2 const& t1,vec2 const& t2,
            vec3 const& n0,vec3 const& n1,vec3 const& n2,
            mat4 const& model,mat4 const& view,mat4 const& projection, cpe::texture t)
{
    vec3 p0_proj,p1_proj,p2_proj;
    color c0_shading,c1_shading,c2_shading;

    //apply vertex shader on the 3 vertices
    color c0 = color(1);
    color c1 = color(1);
    color c2 = color(1);

    bool fragment = true;

    if(!fragment)
    {
        vertex_shader(p0_proj,c0_shading , p0,c0,n0 , model,view,projection);
        vertex_shader(p1_proj,c1_shading , p1,c1,n1 , model,view,projection);
        vertex_shader(p2_proj,c2_shading , p2,c2,n2 , model,view,projection);
        ivec2 u0 = coordinates_to_pixel_index(p0_proj,im.Nx(),im.Ny());
        ivec2 u1 = coordinates_to_pixel_index(p1_proj,im.Nx(),im.Ny());
        ivec2 u2 = coordinates_to_pixel_index(p2_proj,im.Nx(),im.Ny());
        draw_triangle(im,zbuffer,u0,u1,u2 , c0_shading,c1_shading,c2_shading , t0, t1,t2,
                      p0_proj.z(),p1_proj.z(),p2_proj.z(), t );

    } else {
        vec3 pview0,pview1,pview2;
        vec3 n_view0,n_view1,n_view2;
        vertex_shader4fragment(p0_proj,c0_shading , p0,c0,n0 , model,view,projection,pview0,n_view0);
        vertex_shader4fragment(p1_proj,c1_shading , p1,c1,n1 , model,view,projection,pview1,n_view1);
        vertex_shader4fragment(p2_proj,c2_shading , p2,c2,n2 , model,view,projection,pview2,n_view2);
        ivec2 u0 = coordinates_to_pixel_index(p0_proj,im.Nx(),im.Ny());
        ivec2 u1 = coordinates_to_pixel_index(p1_proj,im.Nx(),im.Ny());
        ivec2 u2 = coordinates_to_pixel_index(p2_proj,im.Nx(),im.Ny());

        draw_triangle(im,zbuffer,u0,u1,u2 , c0_shading,c1_shading,c2_shading ,
                          p0_proj.z(),p1_proj.z(),p2_proj.z(),pview0,pview1,pview2,n_view0,n_view1,n_view2,t0,t1,t2,t);

            ////PARTIE FAUSSE POUR INTERPOLER COULEUR ETXTURE ET NON PAS TEXTURE

        //c0_shading = t(t0.x(),t0.y());
//        c1_shading = t(t1.x(),t1.y());
//        c2_shading = t(t2.x(),t2.y());
//        draw_triangle(im,zbuffer,u0,u1,u2 , c0_shading,c1_shading,c2_shading ,
//                      p0_proj.z(),p1_proj.z(),p2_proj.z() );
    }



//    p0_proj=model * view * projection * p0;
//    p1_proj=model * view * projection * p1;
//    p2_proj=model * view * projection * p2;
//    c0_shading = c0;
//    c1_shading = c1;
//    c2_shading = c2;
//    vec3 pview0 = model*p0;
//    vec4 n10 = transposed(inverted(model)) * vec4(n0.x(),n0.y(),n0.z(), 1);
//    vec3 n_view0 = vec3(n10.x(),n10.y(),n10.z());
//    vec3 pview1 = model*p1;
//    vec4 n11 = transposed(inverted(model)) * vec4(n1.x(),n1.y(),n1.z(), 1);
//    vec3 n_view1 = vec3(n11.x(),n11.y(),n11.z());
//    vec3 pview2 = model*p2;
//    vec4 n12 = transposed(inverted(model)) * vec4(n2.x(),n2.y(),n2.z(), 1);
//    vec3 n_view2 = vec3(n12.x(),n12.y(),n12.z());
//    //convert normalized coordinates to pixel coordinate


    //draw triangle in the screen space


    //vertex shader

    //fragment shader

}



void render(image& im,image_zbuffer& zbuffer,mesh const& m,
            mat4 const& model,mat4 const& view,mat4 const& projection,cpe::texture t)
{


    for(int i = 0 ; i < m.size_connectivity() ; ++i)
    {
        triangle_index index = m.connectivity(i);

        //vertex
        vec3 p0 = m.vertex(index.u0());
        vec3 p1 = m.vertex(index.u1());
        vec3 p2 = m.vertex(index.u2());

        //color
//        color c0 = to_color(m.color(index.u0()));
//        color c1 = to_color(m.color(index.u1()));
//        color c2 = to_color(m.color(index.u2()));

        //normal
        vec3 n0 = m.normal(index.u0());
        vec3 n1 = m.normal(index.u1());
        vec3 n2 = m.normal(index.u2());

        //texture
        vec2 t0 = m.texture_coord(index.u0());
        vec2 t1 = m.texture_coord(index.u1());
        vec2 t2 = m.texture_coord(index.u2());


        color c0 = t(t0.x(),t0.y());
        color c1 = t(t1.x(),t1.y());
        color c2 = t(t2.x(),t2.y());
//        render(im,zbuffer, p0, p1, p2,
//                     c0,c1,c2,
//                     n0, n1, n2,
//                     model,view,projection
//                    );
        render(im,zbuffer, p0, p1, p2,
                     t0,t1,t2,
                     n0, n1, n2,
                     model,view,projection,t
                    );
    }
    /*************************************
    // TO DO
    /*************************************
    // Completez l'affichage d'un maillage
    //
    // Pour tous les triangles
    //   p0,p1,p2 <- coordonnees des sommets du triangle
    //   c0,c1,c2 <- couleurs des sommets du triangle (utilisez la fonction to_color pour convertir un vec3 vers la structure color)
    //   n0,n1,n2 <- normales des sommets du triangle
    //
    //   Affichage du triangle courant
    */
}



void vertex_shader(vec3& p_proj,color& c_shading,
                   vec3 const& p,color const& c,vec3 const& n,
                   mat4 const& model, mat4 const& view,mat4 const& projection)
{
    /*************************************
    // TO DO
    /*************************************
    // Completez le vertex shader
    */

    p_proj=model * view * projection * p;    //a modifier
    vec3 pmodel = model*p;
    vec4 n1 = transposed(inverted(model)) * vec4(n.x(),n.y(),n.z(), 1);
    vec3 n_view = vec3(n1.x(),n1.y(),n1.z());
    float ka = 0.2;
    float kd = 0.8;
    float ks = 0.6;
    int es = 128;
    //ambiant
    color ambiant = color(ka);

    //diffuse
    //vec3 light_direction = normalized(p_proj - vec3(0.2, 0.3, -1.0));
    //vec3 light_direction = normalized(p_proj - modelview * view * projection*vec3(0, 0, -2.0));
    vec3 light_direction = normalized(pmodel - vec3(0, 0, -2.0));
    //vec3 light_direction = normalized(p_proj - vec3(-1.5,0.3,1.0));
    float diffuse = kd*((dot(n_view, light_direction) > 0) ? dot(n_view, light_direction) : 0.0f);

    //specular
    vec3 reflection_direction = reflected(light_direction, n_view); // Direction du reflet
    float specular = ks * pow((dot(reflection_direction, n_view) > 0) ? dot(reflection_direction, n_view) : 0, es);
     // Couleur de la lumière spéculaire

    color tot = ambiant + diffuse;
    c_shading=tot * c + specular*color(1); //a modifier

}

void vertex_shader4fragment(vec3& p_proj,color& c_shading,
                   vec3 const& p,color const& c,vec3 const& n,
                   mat4 const& model,mat4 const& view,mat4 const& projection,
                            vec3& p_model,vec3& n_model)
{
    p_proj=model * view * projection * p;    //a modifier
    c_shading = c;

    p_model = model*p;
    vec4 n10 = transposed(inverted(model)) * vec4(n.x(),n.y(),n.z(), 1);
    n_model = vec3(n10.x(),n10.y(),n10.z());
}


}

