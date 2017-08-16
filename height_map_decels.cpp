#include "height_map_decels.h"
#include "utility.h"

Ref<Mesh> HeightMapDecels::make_decels(Params params, const HeightMapData &data) {

    _output_vertices.clear();
    _output_normals.clear();
    _output_colors.clear();
    _output_uv.clear();
    //	_output_uv2.clear();
    //	_output_bones.clear();
    //	_output_tangents.clear();
    //	_output_weights.clear();
    _output_indices.clear();

    if (!params.smooth) {
        ; // TODO Faceted version
        //return make_chunk_faceted(params, data)
    }

    int stride = 1 << params.lod;

    Point2i max = params.origin + params.size * stride;

    Point2i terrain_size(data.get_resolution(), data.get_resolution());

    if (max.y >= terrain_size.y)
        max.y = terrain_size.y;
    if (max.x >= terrain_size.x)
        max.x = terrain_size.x;

    //	Vector2 uv_scale = Vector2(
    //		1.0 / static_cast<real_t>(terrain_size_x),
    //		1.0 / static_cast<real_t>(terrain_size_y));
    // Note: UVs aren't needed because they can be deduced from world positions

    // Make central part
    Point2i pos;


        //draw decels
        if (params.decels_enabled==true) {
        int i = 0;
        float decel_offset_x = -0.5;
        float decel_offset_y = -0.5;
        float height_offset = 0.1;
        float decel_scale = 1.0;
        int decel_y = 0;
        int decel_x = 0;
        float decel_y1 = 0;
        float decel_x1 = 0;
        float decel_normal_y = 0;
        float decel_normal_x = 0;
        float decel_offset_z = 0;
        float grass_rand_x = 0;
        float grass_rand_y = 0;

        for (pos.y = params.origin.y; pos.y <= max.y; pos.y += stride) {
            decel_y = pos.y-params.origin.y;
            for (pos.x = params.origin.x; pos.x <= max.x; pos.x += stride) {
                decel_x = pos.x-params.origin.x;
                int loc = data.heights.index(pos);
                //draw grass
                if ((data.colors[loc].g > 0.8)&&(data.colors[loc].r < 0.2)&&(data.colors[loc].b < 0.2)&&(params.lod==0)) {
                    for (decel_offset_y = -0.5; decel_offset_y < 0.5; decel_offset_y+=0.05){
                        decel_normal_y = decel_offset_y*data.normals[loc].z;
                        decel_y1 = decel_y + decel_offset_y;
                    for (decel_offset_x = -0.5; decel_offset_x < 0.5; decel_offset_x+=0.05){
                        grass_rand_x = -0.03 + (0.06 * ((float)random() / RAND_MAX));
                        grass_rand_y = -0.03 + (0.06 * ((float)random() / RAND_MAX));
                        decel_normal_x = decel_offset_x*data.normals[loc].x;
                        decel_offset_z = decel_normal_y+decel_normal_x;
                        decel_offset_z = data.heights[loc]-decel_offset_z;
                        decel_y1 += grass_rand_y;
                        decel_x1 = decel_x + decel_offset_x + grass_rand_x;
                    _output_vertices.push_back(Vector3(
                            float(decel_x1 - 0.005),
                            float(decel_offset_z-0.1),
                            float(decel_y1 - 0.005)));
                    _output_colors.push_back(Color(0,1,0,0));
                    _output_normals.push_back(data.normals[loc]);
                    _output_uv.push_back(Vector2(0.0,0.0));
                    _output_vertices.push_back(Vector3(
                            float(decel_x1 + 0.005),
                            float(decel_offset_z-0.1),
                            float(decel_y1 - 0.005)));
                    _output_colors.push_back(Color(0,1,0,0));
                    _output_normals.push_back(data.normals[loc]);
                    _output_uv.push_back(Vector2(1.0,0.0));
                    _output_vertices.push_back(Vector3(
                            float(decel_x1 + 0.005),
                            float(decel_offset_z-0.1),
                            float(decel_y1 + 0.005)));
                    _output_colors.push_back(Color(0,1,0,0));
                    _output_normals.push_back(data.normals[loc]);
                    _output_uv.push_back(Vector2(2.0,0.0));
                    height_offset = 0.03 + (0.15 * ((float)random() / RAND_MAX));
                    decel_offset_z+=height_offset;
                    grass_rand_x = -0.03 + (0.06 * ((float)random() / RAND_MAX));
                    grass_rand_y = -0.03 + (0.06 * ((float)random() / RAND_MAX));
                    decel_y1 += grass_rand_y;
                    decel_x1 += grass_rand_x;
                    _output_vertices.push_back(Vector3(
                            float(decel_x1 - 0.002),
                            float(decel_offset_z),
                            float(decel_y1 - 0.002)));
                    _output_colors.push_back(Color(0,1,0,0));
                    _output_normals.push_back(data.normals[loc]);
                    _output_uv.push_back(Vector2(0.0,4.0));
                    _output_vertices.push_back(Vector3(
                            float(decel_x1 + 0.002),
                            float(decel_offset_z),
                            float(decel_y1 - 0.002)));
                    _output_colors.push_back(Color(0,1,0,0));
                    _output_normals.push_back(data.normals[loc]);
                    _output_uv.push_back(Vector2(1.0,4.0));
                    _output_vertices.push_back(Vector3(
                            float(decel_x1 + 0.002),
                            float(decel_offset_z),
                            float(decel_y1 + 0.002)));
                    _output_colors.push_back(Color(0,1,0,0));
                    _output_normals.push_back(data.normals[loc]);
                    _output_uv.push_back(Vector2(2.0,4.0));

                    _output_indices.push_back(i+4);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+4);
                    _output_indices.push_back(i+3);

                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i+5);
                    _output_indices.push_back(i+4);
                    _output_indices.push_back(i+5);
                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i+2);

                    _output_indices.push_back(i+5);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+3);
                    _output_indices.push_back(i+5);
                    _output_indices.push_back(i+2);
                    _output_indices.push_back(i);
                    i+=6;
                    decel_y1 += (0.45*grass_rand_y);
                    decel_x1 += (0.45*grass_rand_x);
                    if (((height_offset>0.15)&&(data.colors[loc].a>0.8))!=true) {
                    height_offset = 0.02 + (0.04 * ((float)random() / RAND_MAX));
                    decel_offset_z+=height_offset;
                    _output_vertices.push_back(Vector3(
                            float(decel_x1),
                            float(decel_offset_z),
                            float(decel_y1)));
                    _output_colors.push_back(Color(0,1,0,0));
                    _output_normals.push_back(data.normals[loc]);
                    _output_uv.push_back(Vector2(1.0,6.0));
                    _output_indices.push_back(i-3);
                    _output_indices.push_back(i-2);
                    _output_indices.push_back(i);

                    _output_indices.push_back(i-1);
                    _output_indices.push_back(i-3);
                    _output_indices.push_back(i);

                    _output_indices.push_back(i-2);
                    _output_indices.push_back(i-1);
                    _output_indices.push_back(i);
                    ++i;
                    }

                    // daisies
                    if ((height_offset>0.15)&&(data.colors[loc].a>0.8)) {
                        //bottom of daisy head
                        height_offset += (0.05 * ((float)random() / RAND_MAX));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.002),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 - 0.002)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.002),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 - 0.002)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.002),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 + 0.002)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        //first petal underside
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.038),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 - 0.024)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.054),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.003),
                                float(decel_y1)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.038),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 + 0.018)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));

                        //yellow core

                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.002),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 - 0.002)));
                        _output_colors.push_back(Color(1,1,0,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.002),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 - 0.002)));
                        _output_colors.push_back(Color(1,1,0,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.002),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 + 0.002)));
                        _output_colors.push_back(Color(1,1,0,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));

                        // first petal topside
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.038),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 - 0.024)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.054),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.002),
                                float(decel_y1)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.038),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 + 0.018)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));

                        // second petal underside

                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.035),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.043),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.003),
                                float(decel_y1 +0.028)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.012),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 +0.03)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));

                        // second petal topside
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.035),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.043),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.002),
                                float(decel_y1 +0.028)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.012),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 + 0.03)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));

                        // third petal underside

                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.02),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 - 0.035)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.012),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.003),
                                float(decel_y1 - 0.052)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.024),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 - 0.03)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));

                        // third petal topside

                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.02),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 - 0.035)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.012),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.002),
                                float(decel_y1 - 0.052)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.024),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 - 0.03)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_uv.push_back(Vector2(0.0,0.0));

                        _output_indices.push_back(i);
                        _output_indices.push_back(i-3);
                        _output_indices.push_back(i-2);
                        _output_indices.push_back(i);
                        _output_indices.push_back(i-3);
                        _output_indices.push_back(i-1);

                        _output_indices.push_back(i);
                        _output_indices.push_back(i-2);
                        _output_indices.push_back(i+1);

                        _output_indices.push_back(i+1);
                        _output_indices.push_back(i-2);
                        _output_indices.push_back(i+2);

                        _output_indices.push_back(i+2);
                        _output_indices.push_back(i-3);
                        _output_indices.push_back(i-2);

                        _output_indices.push_back(i+2);
                        _output_indices.push_back(i-1);
                        _output_indices.push_back(i-3);

                        _output_indices.push_back(i+3);
                        _output_indices.push_back(i+1);
                        _output_indices.push_back(i+2);

                        _output_indices.push_back(i+3);
                        _output_indices.push_back(i+2);
                        _output_indices.push_back(i+5);

                        _output_indices.push_back(i+3);
                        _output_indices.push_back(i+5);
                        _output_indices.push_back(i+4);

                        _output_indices.push_back(i+3);
                        _output_indices.push_back(i+7);
                        _output_indices.push_back(i+1);

                        _output_indices.push_back(i+7);
                        _output_indices.push_back(i+3);
                        _output_indices.push_back(i+9);

                        _output_indices.push_back(i+10);
                        _output_indices.push_back(i+9);
                        _output_indices.push_back(i+3);

                        _output_indices.push_back(i+9);
                        _output_indices.push_back(i+4);
                        _output_indices.push_back(i+10);

                        _output_indices.push_back(i+11);
                        _output_indices.push_back(i+10);
                        _output_indices.push_back(i+4);

                        _output_indices.push_back(i+11);
                        _output_indices.push_back(i+4);
                        _output_indices.push_back(i+5);

                        _output_indices.push_back(i+8);
                        _output_indices.push_back(i+5);
                        _output_indices.push_back(i+2);

                        _output_indices.push_back(i+5);
                        _output_indices.push_back(i+8);
                        _output_indices.push_back(i+11);

                        _output_indices.push_back(i+8);
                        _output_indices.push_back(i+9);
                        _output_indices.push_back(i+11);

                        _output_indices.push_back(i+9);
                        _output_indices.push_back(i+8);
                        _output_indices.push_back(i+7);

                        _output_indices.push_back(i+11);
                        _output_indices.push_back(i+9);
                        _output_indices.push_back(i+10);

                        _output_indices.push_back(i+8);
                        _output_indices.push_back(i+6);
                        _output_indices.push_back(i+7);

                        _output_indices.push_back(i);
                        _output_indices.push_back(i+15);
                        _output_indices.push_back(i+12);

                        _output_indices.push_back(i+15);
                        _output_indices.push_back(i);
                        _output_indices.push_back(i+6);

                        _output_indices.push_back(i+15);
                        _output_indices.push_back(i+13);
                        _output_indices.push_back(i+12);

                        _output_indices.push_back(i+13);
                        _output_indices.push_back(i+15);
                        _output_indices.push_back(i+16);

                        _output_indices.push_back(i+14);
                        _output_indices.push_back(i+16);
                        _output_indices.push_back(i+17);

                        _output_indices.push_back(i+16);
                        _output_indices.push_back(i+14);
                        _output_indices.push_back(i+13);

                        _output_indices.push_back(i+2);
                        _output_indices.push_back(i+17);
                        _output_indices.push_back(i+8);

                        _output_indices.push_back(i+17);
                        _output_indices.push_back(i+2);
                        _output_indices.push_back(i+14);

                        _output_indices.push_back(i);
                        _output_indices.push_back(i+14);
                        _output_indices.push_back(i+2);

                        _output_indices.push_back(i+14);
                        _output_indices.push_back(i);
                        _output_indices.push_back(i+12);

                        _output_indices.push_back(i+14);
                        _output_indices.push_back(i+12);
                        _output_indices.push_back(i+13);

                        _output_indices.push_back(i+8);
                        _output_indices.push_back(i+15);
                        _output_indices.push_back(i+6);

                        _output_indices.push_back(i+15);
                        _output_indices.push_back(i+8);
                        _output_indices.push_back(i+17);

                        _output_indices.push_back(i+15);
                        _output_indices.push_back(i+17);
                        _output_indices.push_back(i+16);

                        _output_indices.push_back(i+18);
                        _output_indices.push_back(i+1);
                        _output_indices.push_back(i+7);

                        _output_indices.push_back(i+21);
                        _output_indices.push_back(i+7);
                        _output_indices.push_back(i+18);

                        _output_indices.push_back(i+18);
                        _output_indices.push_back(i+19);
                        _output_indices.push_back(i+21);

                        _output_indices.push_back(i+21);
                        _output_indices.push_back(i+22);
                        _output_indices.push_back(i+19);

                        _output_indices.push_back(i+23);
                        _output_indices.push_back(i+22);
                        _output_indices.push_back(i+19);

                        _output_indices.push_back(i+22);
                        _output_indices.push_back(i+20);
                        _output_indices.push_back(i+23);

                        _output_indices.push_back(i+23);
                        _output_indices.push_back(i+6);
                        _output_indices.push_back(i);

                        _output_indices.push_back(i);
                        _output_indices.push_back(i+23);
                        _output_indices.push_back(i+20);

                        _output_indices.push_back(i+1);
                        _output_indices.push_back(i);
                        _output_indices.push_back(i+20);

                        _output_indices.push_back(i+20);
                        _output_indices.push_back(i+1);
                        _output_indices.push_back(i+18);

                        _output_indices.push_back(i+18);
                        _output_indices.push_back(i+19);
                        _output_indices.push_back(i+20);

                        _output_indices.push_back(i+6);
                        _output_indices.push_back(i+7);
                        _output_indices.push_back(i+23);

                        _output_indices.push_back(i+7);
                        _output_indices.push_back(i+23);
                        _output_indices.push_back(i+21);

                        _output_indices.push_back(i+23);
                        _output_indices.push_back(i+22);
                        _output_indices.push_back(i+21);


                        i+=24;

                    }
                    }

                    }

                }
                //draw concrete tiles
                if ((data.colors[loc].g > 0.8)&&(data.colors[loc].r < 0.2)&&(data.colors[loc].b > 0.8)&&(data.colors[loc].a > 0.8)&&(params.lod==0))  {
                    for (decel_offset_y = -0.5; decel_offset_y < 0.5; decel_offset_y+=0.5){
                        for (decel_offset_x = -0.5; decel_offset_x < 0.5; decel_offset_x+=0.5){
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + decel_offset_x),
                                    float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.1),
                                    float(pos.y - params.origin.y + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.5 + decel_offset_x),
                                    float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-((decel_offset_x+0.5)*data.normals[loc].x)-0.1),
                                    float(pos.y - params.origin.y + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.5 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.5)*data.normals[loc].z)-((decel_offset_x+0.5)*data.normals[loc].x)-0.1),
                                    float(pos.y - params.origin.y + 0.5 + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.5)*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.1),
                                    float(pos.y - params.origin.y + 0.5 + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));

                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + decel_offset_x),
                                    float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.5 + decel_offset_x),
                                    float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-((decel_offset_x+0.5)*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.5 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.5)*data.normals[loc].z)-((decel_offset_x+0.5)*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + 0.5 + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.5)*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + 0.5 + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));

                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.01 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.01)*data.normals[loc].z)-((decel_offset_x+0.01)*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + 0.01 + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.49 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.01)*data.normals[loc].z)-((decel_offset_x+0.49)*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + 0.01 + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.49 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.49)*data.normals[loc].z)-((decel_offset_x+0.49)*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + 0.49 + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.01 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.49)*data.normals[loc].z)-((decel_offset_x+0.01)*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + 0.49+ decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));

                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.01 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.01)*data.normals[loc].z)-((decel_offset_x+0.01)*data.normals[loc].x)+0.08),
                                    float(pos.y - params.origin.y + 0.01 + decel_offset_y)));
                            _output_colors.push_back(Color(0.7,0.7,0.7,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.49 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.01)*data.normals[loc].z)-((decel_offset_x+0.49)*data.normals[loc].x)+0.08),
                                    float(pos.y - params.origin.y + 0.01 + decel_offset_y)));
                            _output_colors.push_back(Color(0.7,0.7,0.7,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.49 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.49)*data.normals[loc].z)-((decel_offset_x+0.49)*data.normals[loc].x)+0.08),
                                    float(pos.y - params.origin.y + 0.49 + decel_offset_y)));
                            _output_colors.push_back(Color(0.7,0.7,0.7,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.01 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.49)*data.normals[loc].z)-((decel_offset_x+0.01)*data.normals[loc].x)+0.08),
                                    float(pos.y - params.origin.y + 0.49+ decel_offset_y)));
                            _output_colors.push_back(Color(0.7,0.7,0.7,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_uv.push_back(Vector2(0.0,0.0));

                            _output_indices.push_back(i+4);
                            _output_indices.push_back(i);
                            _output_indices.push_back(i+1);

                            _output_indices.push_back(i+4);
                            _output_indices.push_back(i+1);
                            _output_indices.push_back(i+5);

                            _output_indices.push_back(i+6);
                            _output_indices.push_back(i+5);
                            _output_indices.push_back(i+1);

                            _output_indices.push_back(i+2);
                            _output_indices.push_back(i+6);
                            _output_indices.push_back(i+1);

                            _output_indices.push_back(i+7);
                            _output_indices.push_back(i+6);
                            _output_indices.push_back(i+2);

                            _output_indices.push_back(i+7);
                            _output_indices.push_back(i+2);
                            _output_indices.push_back(i+3);

                            _output_indices.push_back(i+7);
                            _output_indices.push_back(i);
                            _output_indices.push_back(i+4);

                            _output_indices.push_back(i);
                            _output_indices.push_back(i+7);
                            _output_indices.push_back(i+3);

                            _output_indices.push_back(i+8);
                            _output_indices.push_back(i+4);
                            _output_indices.push_back(i+5);

                            _output_indices.push_back(i+8);
                            _output_indices.push_back(i+5);
                            _output_indices.push_back(i+9);

                            _output_indices.push_back(i+9);
                            _output_indices.push_back(i+5);
                            _output_indices.push_back(i+6);

                            _output_indices.push_back(i+6);
                            _output_indices.push_back(i+10);
                            _output_indices.push_back(i+9);

                            _output_indices.push_back(i+7);
                            _output_indices.push_back(i+10);
                            _output_indices.push_back(i+6);

                            _output_indices.push_back(i+7);
                            _output_indices.push_back(i+11);
                            _output_indices.push_back(i+10);

                            _output_indices.push_back(i+4);
                            _output_indices.push_back(i+11);
                            _output_indices.push_back(i+7);

                            _output_indices.push_back(i+8);
                            _output_indices.push_back(i+11);
                            _output_indices.push_back(i+4);

                            _output_indices.push_back(i+12);
                            _output_indices.push_back(i+8);
                            _output_indices.push_back(i+9);

                            _output_indices.push_back(i+13);
                            _output_indices.push_back(i+12);
                            _output_indices.push_back(i+9);

                            _output_indices.push_back(i+14);
                            _output_indices.push_back(i+13);
                            _output_indices.push_back(i+9);

                            _output_indices.push_back(i+10);
                            _output_indices.push_back(i+14);
                            _output_indices.push_back(i+9);

                            _output_indices.push_back(i+15);
                            _output_indices.push_back(i+14);
                            _output_indices.push_back(i+10);

                            _output_indices.push_back(i+10);
                            _output_indices.push_back(i+11);
                            _output_indices.push_back(i+15);

                            _output_indices.push_back(i+15);
                            _output_indices.push_back(i+11);
                            _output_indices.push_back(i+8);

                            _output_indices.push_back(i+15);
                            _output_indices.push_back(i+8);
                            _output_indices.push_back(i+12);

                            _output_indices.push_back(i+15);
                            _output_indices.push_back(i+13);
                            _output_indices.push_back(i+14);

                            _output_indices.push_back(i+15);
                            _output_indices.push_back(i+12);
                            _output_indices.push_back(i+13);
                            i+=16;
                        }
                    }
                }
                //draw tree
                if ((data.colors[loc].g < 0.2)&&(data.colors[loc].r < 0.2)&&(data.colors[loc].b < 0.2)&&(data.colors[loc].a < 0.2)&&(params.lod==0)) {
                    if ((((pos.x-params.origin.x)==0)&&((pos.y-params.origin.y)==0))||(((pos.x-params.origin.x)==4)&&((pos.y-params.origin.y)==0))||(((pos.x-params.origin.x)==8)&&((pos.y-params.origin.y)==0))||(((pos.x-params.origin.x)==12)&&((pos.y-params.origin.y)==0))||(((pos.x-params.origin.x)==0)&&((pos.y-params.origin.y)==4))||(((pos.x-params.origin.x)==4)&&((pos.y-params.origin.y)==4))||(((pos.x-params.origin.x)==8)&&((pos.y-params.origin.y)==4))||(((pos.x-params.origin.x)==12)&&((pos.y-params.origin.y)==4))||(((pos.x-params.origin.x)==0)&&((pos.y-params.origin.y)==8))||(((pos.x-params.origin.x)==4)&&((pos.y-params.origin.y)==8))||(((pos.x-params.origin.x)==8)&&((pos.y-params.origin.y)==8))||(((pos.x-params.origin.x)==12)&&((pos.y-params.origin.y)==8))||(((pos.x-params.origin.x)==0)&&((pos.y-params.origin.y)==12))||(((pos.x-params.origin.x)==4)&&((pos.y-params.origin.y)==12))||(((pos.x-params.origin.x)==8)&&((pos.y-params.origin.y)==12))||(((pos.x-params.origin.x)==12)&&((pos.y-params.origin.y)==12))) {
                    _Seed._Long = 10131049502708829145UL;
                    int _byte_selector1 = (int(pos.x)-int(params.origin.x))%8;
                    int _byte_selector2 = (int(pos.y)-int(params.origin.y))%8;
                    uint8_t rand_val1 = _Seed._Short[_byte_selector1];
                    uint8_t rand_val2 = _Seed._Short[_byte_selector2];

                    randval = (_Seed._Short[_byte_selector]*_Seed._Short[_byte_selector])%255;
                    decel_offset_y = -1.2 + (2.41 * ((float)random() / RAND_MAX));
                    decel_offset_x = -1.2 + (2.41 * ((float)random() / RAND_MAX));
                    decel_scale = 0.2 + (0.91 * ((float)random() / RAND_MAX));
                    int branch1_start = 0;
                    int branch2_start = 0;
                    int parent_branch1_start = 0;
                    int parent_branch2_start = 0;
                    float rand_rot = 2.0f*3.14159263 * ((float)random() / RAND_MAX);
                    float root_indent = 0.8;
                    float root_outdent = 1.2;
                    float root_height = 0.4 + (0.8 * ((float)random() / RAND_MAX));
                    float node1_maxheight = 2.4;
                    float node1_minheight = 2.4;
                    float node1_height = node1_minheight + (node1_maxheight * ((float)random() / RAND_MAX));
                    float node1_bend_y = -0.32 + (0.641 * ((float)random() / RAND_MAX));
                    float node1_bend_x = -0.32 + (0.641 * ((float)random() / RAND_MAX));
                    float node1_branch1_length = (0.6 + (1.7 * ((float)random() / RAND_MAX)));
                    float node1_branch2_length = (0.6 + (1.7 * ((float)random() / RAND_MAX)));
                    float node1_min_incline = 0.02;
                    float node1_max_incline = 0.14;
                    float branch_min_incline = 0.02;
                    float branch_max_incline = 0.14;
                    float node1_incline = node1_min_incline + (((2.0f*3.14159263)*node1_max_incline)* ((float)random() / RAND_MAX));
                    float branch_ratio = 0.9;
                    int max_branch_layer =36;

                    Vector3 A = Vector3(
                                     float(pos.x - params.origin.x - (root_indent*decel_scale) + decel_offset_x),
                                     float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                                     float(pos.y - params.origin.y + decel_offset_y));
                    Vector3 B = Vector3(
                                float(pos.x - params.origin.x - (root_outdent*decel_scale) + decel_offset_x),
                                float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                                float(pos.y - params.origin.y + decel_offset_y));
                    Vector3 C = Vector3(
                                float(pos.x - params.origin.x + decel_offset_x),
                                float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                                float(pos.y - params.origin.y + decel_offset_y));
                    Vector3 AH = (A-C);
                    Vector3 BH = (B-C);
                    Vector3 N = Vector3(0.0,1.0,0.0).normalized();

                    for(int hi = 0; hi < 16; hi+=2)
                    {
                        float angle = (hi * (2.0f*3.14159263 / 16.0f))+rand_rot;
                        Vector3 rotatedH = AH.rotated(N, angle);
                        _output_vertices.push_back(C + rotatedH);
                        _output_colors.push_back(Color(0,0,0,0));
                        _output_uv.push_back(Vector2((float(hi)*0.5),0.0));
                        _output_normals.push_back(C-(_output_vertices[i+hi]).normalized());
                        angle = (float(hi+1.0f) * (2.0f*3.14159263 / 16.0f))+rand_rot;
                        rotatedH = BH.rotated(N, angle);
                        _output_vertices.push_back(C + rotatedH);
                        _output_colors.push_back(Color(0,0,0,0));
                        _output_uv.push_back(Vector2((float(hi+1.0)*0.5),0.0));
                        _output_normals.push_back(C-(_output_vertices[i+hi+1]).normalized());

                    }
                    _output_vertices.push_back(_output_vertices[i]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(8.0,0.0));
                    _output_normals.push_back(_output_normals[i]);

                    //17 tree base above ground
                    C.y+= root_height;
                    height_offset =root_height;
                    A = Vector3(
                            float(pos.x - params.origin.x - (0.839*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+root_height),
                            float(pos.y - params.origin.y + (0.148*decel_scale) + decel_offset_y));

                    AH = (A-C);

                    for(int hi = 0; hi < 16; ++hi)
                    {
                        float angle = (hi * (2.0f*3.14159263 / 16.0f)) + rand_rot;
                        Vector3 rotatedH = AH.rotated(N, angle);
                        _output_vertices.push_back(C + rotatedH);
                        _output_colors.push_back(Color(0,0,0,0));
                        _output_uv.push_back(Vector2((float(hi)*0.5),4.0));
                        _output_normals.push_back(C-(_output_vertices[i+hi+17]).normalized());

                    }
                    _output_vertices.push_back(_output_vertices[i+17]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(8.0,4.0));
                    _output_normals.push_back(_output_normals[i+17]);
                    //34
                    decel_offset_y+= node1_bend_y;
                    decel_offset_x+= node1_bend_x;
                    height_offset += node1_height;

                    A = Vector3(
                            float(pos.x - params.origin.x - (0.711*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + decel_offset_y));

                    B = Vector3(
                            float(pos.x - params.origin.x + (0.711*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + decel_offset_y));

                    C = ((A-B)/2.0)+B;
                    AH = (A-C);

                    for(int hi = 0; hi < 16; ++hi)
                    {
                        float angle = (hi * (2.0f*3.14159263 / 16.0f)) + rand_rot;
                        Vector3 rotatedH = AH.rotated(N, angle);
                        _output_vertices.push_back(C + rotatedH);
                        _output_colors.push_back(Color(0,0,0,0));
                        _output_uv.push_back(Vector2((float(hi)*0.5),18.0));
                        _output_normals.push_back(C-(_output_vertices[i+hi+34]).normalized());

                    }
                    _output_vertices.push_back(_output_vertices[i+34]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(8.0,18.0));
                    _output_normals.push_back(_output_normals[i+34]);



                    //51 Bottom of First Branch
                    _output_vertices.push_back(_output_vertices[i+38]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(2.0,0.0));

                    //52
                    _output_vertices.push_back(_output_vertices[i+37]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(1.5,0.0));

                    //53
                    _output_vertices.push_back(_output_vertices[i+36]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(1.0,0.0));

                    //54
                    _output_vertices.push_back(_output_vertices[i+35]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(0.5,0.0));

                    Vector3 N2 = (_output_vertices[i+34]-_output_vertices[i+42]).normalized();
                    B = _output_vertices[i+42];

                    N = (_output_vertices[i+38]-_output_vertices[i+46]).normalized();
                    C = ((_output_vertices[i+34]-_output_vertices[i+42])/2.0)+_output_vertices[i+42];
                    AH = _output_vertices[i+34]-C;
                    for(int hi = 1; hi < 6; ++hi)
                    {
                        float angle = hi * (-2.0f*3.14159263 / 12.0f);
                        Vector3 rotatedH = AH.rotated(N, angle);
                        rotatedH = rotatedH.rotated(N2, node1_incline);
                        _output_vertices.push_back(C + rotatedH);
                        _output_colors.push_back(Color(0,0,0,0));
                        _output_uv.push_back(Vector2(-0.5-((hi-1)*0.6),0.0));


                    }

                    //60
                    _output_vertices.push_back(_output_vertices[i+41]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(-4.5,0.0));

                    //61
                    _output_vertices.push_back(_output_vertices[i+40]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(-5.0,0.0));

                    //62
                    _output_vertices.push_back(_output_vertices[i+39]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(-5.5,0.0));

                    //63 repeat of first vertex in cylinder
                    _output_vertices.push_back(_output_vertices[i+38]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(-6.0,0.0));

                    //64 near join vertex of first branch
                    _output_vertices.push_back(_output_vertices[i+34]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(0.0,-1.0));

                    //65 far join vertex of first branch
                    _output_vertices.push_back(_output_vertices[i+42]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(-4.0,-1.0));

                   //66 -> 78
                    A = ((_output_vertices[i+65]-_output_vertices[i+64])/2.0)+_output_vertices[i+64];
                    C = ((_output_vertices[i+57]-_output_vertices[i+51])/2.0)+_output_vertices[i+51];
                    N = (C-A).normalized();
                    float NB = sqrt((N.x*N.x)+(N.y*N.y)+(N.z*N.z));
                    Vector3 H = branch_ratio*(_output_vertices[i+38]-A);
                    B = ((N/NB)*(node1_branch1_length*sqrt((H.x*H.x)+(H.y*H.y)+(H.z*H.z)))) + A;

                    for(int hi = 0; hi <= 12; ++hi)
                    {
                        float angle = hi * (-2.0f*3.14159263 / 12.0f);
                        Vector3 rotatedH = H.rotated(N, angle);
                        _output_vertices.push_back(B + rotatedH);
                        _output_colors.push_back(Color(0,0,0,0));
                        _output_uv.push_back(Vector2((2.0-(0.6666666*hi)),4.0));
                    }

                    //79 First vertex of 2nd branch or stump
                    _output_vertices.push_back(_output_vertices[i+46]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(-2.0,0.0));
                    //80
                    _output_vertices.push_back(_output_vertices[i+47]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(-1.5,0.0));
                    //81
                    _output_vertices.push_back(_output_vertices[i+48]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(-1.0,0.0));
                    //82
                    _output_vertices.push_back(_output_vertices[i+49]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(-0.5,0.0));
                    //83
                    _output_vertices.push_back(_output_vertices[i+55]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(0.5,0.0));
                    //84
                    _output_vertices.push_back(_output_vertices[i+56]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(1.25,0.0));
                    //85
                    _output_vertices.push_back(_output_vertices[i+57]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(2.0,0.0));
                    //86
                    _output_vertices.push_back(_output_vertices[i+58]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(2.75,0.0));
                    //87
                    _output_vertices.push_back(_output_vertices[i+59]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(3.5,0.0));
                    //88
                    _output_vertices.push_back(_output_vertices[i+43]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(4.5,0.0));
                    //89
                    _output_vertices.push_back(_output_vertices[i+44]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(5.0,0.0));
                    //90
                    _output_vertices.push_back(_output_vertices[i+45]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(5.5,0.0));
                    //91 Repeat of bottom of 2nd branch or stump
                    _output_vertices.push_back(_output_vertices[i+46]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(6.0,0.0));
                    //92 2nd branch nearside join vertex
                    _output_vertices.push_back(_output_vertices[i+34]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(0.0,-1.0));
                    //93 2nd branch farside join vertex
                    _output_vertices.push_back(_output_vertices[i+42]);
                    _output_colors.push_back(Color(0,0,0,0));
                    _output_uv.push_back(Vector2(4.0,-1.0));


                    //root to trunk base
                    int c1 = 0;
                    int c2 = 0;
                    for (c1 = 0; c1 < 16; ++c1){
                    _output_indices.push_back(i+18);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+17);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+18);
                    _output_indices.push_back(i+1);
                    ++i;
                    }
                    // trunk base to first node
                    ++i;
                    for (c1 = 0; c1 < 16; ++c1){
                    _output_indices.push_back(i+18);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+17);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+18);
                    _output_indices.push_back(i+1);

                    ++i;
                    }
                    i+=18;
                    //Face up the first branch
                    for (c1 = 0; c1 < 12; ++c1){
                        _output_indices.push_back(i);
                        _output_indices.push_back(i+16);
                        _output_indices.push_back(i+15);
                        _output_indices.push_back(i+16);
                        _output_indices.push_back(i);
                        _output_indices.push_back(i+1);
                        ++i;
                    }
                    i-=12;
                    _output_normals.push_back(_output_normals[i-13]);
                    ++i;
                    _output_normals.push_back(_output_normals[i-15]);
                    ++i;
                    _output_normals.push_back(_output_normals[i-17]);
                    ++i;
                    _output_normals.push_back(_output_normals[i-19]);
                    ++i;
                    _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
                    ++i;
                    _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
                    ++i;
                    _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
                    ++i;
                    _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
                    ++i;
                    _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
                    ++i;
                    _output_normals.push_back(_output_normals[i-19]);
                    ++i;
                    _output_normals.push_back(_output_normals[i-21]);
                    ++i;
                    _output_normals.push_back(_output_normals[i-23]);
                    ++i;
                    _output_normals.push_back(_output_normals[i-25]);
                    ++i;
                    //Face up the first branch nearside join
                    _output_indices.push_back(i);
                    _output_indices.push_back(i-9);
                    _output_indices.push_back(i-10);
                    _output_normals.push_back(_output_normals[i-30]);
                    ++i;
                    //Face up first branch farside join
                    _output_indices.push_back(i);
                    _output_indices.push_back(i-5);
                    _output_indices.push_back(i-6);
                    _output_normals.push_back(_output_normals[i-23]);
                    ++i;
                    //Generate normals for top of first branch

                    C = ((_output_vertices[i]-_output_vertices[i+6])/2.0)+_output_vertices[i+6];

                    for (int hi = 0; hi <= 12; ++hi){
                        _output_normals.push_back((_output_vertices[i]-C).normalized());
                        ++i;
                    }

                   float branches = -100.0 + (200 * ((float)random() / RAND_MAX));

                                       // Build second branch if there is one
                                       if (branches>=0) {
                                           //94 First Branch Second Node

                                           A = ((_output_vertices[i+14]-_output_vertices[i+13])/2.0)+_output_vertices[i+13];
                                           C = ((_output_vertices[i+6]-_output_vertices[i])/2.0)+_output_vertices[i];
                                           N = (C-A).normalized();
                                           float NB = sqrt((N.x*N.x)+(N.y*N.y)+(N.z*N.z));
                                           Vector3 H = branch_ratio*(_output_vertices[i]-A);
                                           B = ((N/NB)*(node1_branch2_length*sqrt((H.x*H.x)+(H.y*H.y)+(H.z*H.z)))) + A;

                                           for(int hi = 0; hi <= 12; ++hi)
                                           {
                                               float angle = hi * (2.0f*3.14159263 / 12.0f);
                                               Vector3 rotatedH = H.rotated(N, angle);
                                               _output_vertices.push_back(B + rotatedH);
                                               _output_colors.push_back(Color(0,0,0,0));
                                               _output_uv.push_back(Vector2((-2.0+(0.6666666*hi)),4.0));
                                           }



                                           for (c1 = 0; c1 < 12; ++c1){
                                               _output_indices.push_back(i+16);
                                               _output_indices.push_back(i);
                                               _output_indices.push_back(i+15);
                                               _output_indices.push_back(i);
                                               _output_indices.push_back(i+16);
                                               _output_indices.push_back(i+1);
                                               ++i;
                                           }
                                           //91
                                           i-=12;
                                           //79
                                           _output_normals.push_back(_output_normals[i-33]);
                                           ++i;
                                           //80
                                           _output_normals.push_back(_output_normals[i-33]);
                                           ++i;
                                           //81
                                           _output_normals.push_back(_output_normals[i-33]);
                                           ++i;
                                           //82
                                           _output_normals.push_back(_output_normals[i-33]);
                                           ++i;
                                           //83
                                           _output_normals.push_back((((_output_vertices[i+9]-_output_vertices[i-1])*(_output_vertices[i]-_output_vertices[i-1]))+((_output_vertices[i-29]-_output_vertices[i+9])*(_output_vertices[i]-_output_vertices[i+9]))+((_output_vertices[i]-_output_vertices[i-1])*(_output_vertices[i+15]-_output_vertices[i-1]))+((_output_vertices[i+15]-_output_vertices[i+16])*(_output_vertices[i]-_output_vertices[i+16]))+((_output_vertices[i+1]-_output_vertices[i])*(_output_vertices[i+16]-_output_vertices[i]))+((_output_vertices[i+1]-_output_vertices[i-12])*(_output_vertices[i]-_output_vertices[i-12]))+((_output_vertices[i-13]-_output_vertices[i])*(_output_vertices[i-12]-_output_vertices[i]))+((_output_vertices[i]-_output_vertices[i-13])*(_output_vertices[i-29]-_output_vertices[i-13]))).normalized());
                                           _output_normals[i-28]=_output_normals[i];
                                           ++i;
                                           //84
                                           _output_normals.push_back(( ((_output_vertices[i+1]-_output_vertices[i-12])*(_output_vertices[i]-_output_vertices[i-12]))+((_output_vertices[i-13]-_output_vertices[i])*(_output_vertices[i-12]-_output_vertices[i]))+((_output_vertices[i]-_output_vertices[i-13])*(_output_vertices[i-1]-_output_vertices[i-13]))+((_output_vertices[i+1]-_output_vertices[i])*(_output_vertices[i+16]-_output_vertices[i]))+((_output_vertices[i+15]-_output_vertices[i+16])*(_output_vertices[i]-_output_vertices[i+16]))+((_output_vertices[i]-_output_vertices[i-1])*(_output_vertices[i+15]-_output_vertices[i-1])) ).normalized());
                                           _output_normals[i-28]=_output_normals[i];
                                           ++i;
                                           //85
                                           _output_normals.push_back(( ((_output_vertices[i+1]-_output_vertices[i-12])*(_output_vertices[i]-_output_vertices[i-12]))+((_output_vertices[i-13]-_output_vertices[i])*(_output_vertices[i-12]-_output_vertices[i]))+((_output_vertices[i]-_output_vertices[i-13])*(_output_vertices[i-1]-_output_vertices[i-13]))+((_output_vertices[i+1]-_output_vertices[i])*(_output_vertices[i+16]-_output_vertices[i]))+((_output_vertices[i+15]-_output_vertices[i+16])*(_output_vertices[i]-_output_vertices[i+16]))+((_output_vertices[i]-_output_vertices[i-1])*(_output_vertices[i+15]-_output_vertices[i-1])) ).normalized());
                                           _output_normals[i-28]=_output_normals[i];
                                           ++i;
                                           //86
                                           _output_normals.push_back(( ((_output_vertices[i+1]-_output_vertices[i-12])*(_output_vertices[i]-_output_vertices[i-12]))+((_output_vertices[i-13]-_output_vertices[i])*(_output_vertices[i-12]-_output_vertices[i]))+((_output_vertices[i]-_output_vertices[i-13])*(_output_vertices[i-1]-_output_vertices[i-13]))+((_output_vertices[i+1]-_output_vertices[i])*(_output_vertices[i+16]-_output_vertices[i]))+((_output_vertices[i+15]-_output_vertices[i+16])*(_output_vertices[i]-_output_vertices[i+16]))+((_output_vertices[i]-_output_vertices[i-1])*(_output_vertices[i+15]-_output_vertices[i-1])) ).normalized());
                                           _output_normals[i-28]=_output_normals[i];
                                           ++i;
                                           //87 8 Triangles to calculate!? Phew!
                                           _output_normals.push_back(( ((_output_vertices[i+6]-_output_vertices[i])*(_output_vertices[i+1]-_output_vertices[i]))+((_output_vertices[i+1]-_output_vertices[i])*(_output_vertices[i+16]-_output_vertices[i]))+((_output_vertices[i+15]-_output_vertices[i+16])*(_output_vertices[i]-_output_vertices[i+16]))+((_output_vertices[i]-_output_vertices[i-1])*(_output_vertices[i+15]-_output_vertices[i-1]))+((_output_vertices[i]-_output_vertices[i+6])*(_output_vertices[i-27]-_output_vertices[i+6]))+((_output_vertices[i-27]-_output_vertices[i-12])*(_output_vertices[i]-_output_vertices[i-12]))+((_output_vertices[i-13]-_output_vertices[i])*(_output_vertices[i-12]-_output_vertices[i]))+((_output_vertices[i]-_output_vertices[i-13])*(_output_vertices[i-1]-_output_vertices[i-13])) ).normalized());
                                           _output_normals[i-28]=_output_normals[i];
                                           ++i;
                                           //88
                                           _output_normals.push_back(_output_normals[i-45]);
                                           ++i;
                                           //89
                                           _output_normals.push_back(_output_normals[i-45]);
                                           ++i;
                                           //90
                                           _output_normals.push_back(_output_normals[i-45]);
                                           ++i;
                                           //91
                                           _output_normals.push_back(_output_normals[i-12]);
                                           ++i;
                                           //92
                                           // Face up Nearside 2nd branch join vertex
                                           _output_indices.push_back(i-10);
                                           _output_indices.push_back(i-9);
                                           _output_indices.push_back(i);
                                           _output_normals.push_back(_output_normals[i-58]);
                                           _output_normals[i-28]=_output_normals[i];
                                           _output_normals[i-42]=_output_normals[i];

                                           ++i;
                                           // Face up Farside 2nd branch join vertex
                                           _output_indices.push_back(i-6);
                                           _output_indices.push_back(i-5);
                                           _output_indices.push_back(i);
                                           _output_normals.push_back(_output_normals[i-51]);
                                           _output_normals[i-28]=_output_normals[i];
                                           ++i;
                                           // Add normals to top of 2nd branch
                                           C = ((_output_vertices[i]-_output_vertices[i+6])/2.0)+_output_vertices[i+6];

                                           for (int hi = 0; hi <= 12; ++hi){
                                               _output_normals.push_back((_output_vertices[i]-C).normalized());
                                               ++i;
                                           }

                                           branch1_start = i-13;
                                           branch2_start = i-41;
                                           parent_branch1_start = i-56;
                                           parent_branch2_start = i-28;
                                           float rand_branch_type = -50.0 + (300 * ((float)random() / RAND_MAX));
                                           if (rand_branch_type<0){ i += _make_end(branch2_start, 0, parent_branch2_start, true, 0, max_branch_layer, branch_min_incline, branch_max_incline); }
                                           if ((rand_branch_type>=0)&&(rand_branch_type<50.0)) { i += _make_end(branch2_start, 1, parent_branch2_start, true, 0, max_branch_layer, branch_min_incline, branch_max_incline); }
                                           if ((rand_branch_type>=50.0)&&(rand_branch_type<100.0))  { i += _make_end(branch2_start, 2, parent_branch2_start, true, 0, max_branch_layer, branch_min_incline, branch_max_incline); }
                                           if (rand_branch_type>=100.0) { i += _make_end(branch2_start, 3, parent_branch2_start, true, 0, max_branch_layer, branch_min_incline, branch_max_incline); }


                                            rand_branch_type = -50.0 + (300 * ((float)random() / RAND_MAX));
                                            if (rand_branch_type<0){ i += _make_end(branch1_start, 0, parent_branch1_start, false, 0, max_branch_layer, branch_min_incline, branch_max_incline); }
                                            if ((rand_branch_type>=0)&&(rand_branch_type<50.0)) { i += _make_end(branch1_start, 1, parent_branch1_start, false, 0, max_branch_layer, branch_min_incline, branch_max_incline); }
                                            if ((rand_branch_type>=50.0)&&(rand_branch_type<100.0)) { i += _make_end(branch1_start, 2, parent_branch1_start, false, 0, max_branch_layer, branch_min_incline, branch_max_incline); }
                                            if (rand_branch_type>=100.0) { i += _make_end(branch1_start, 3, parent_branch1_start, false, 0, max_branch_layer, branch_min_incline, branch_max_incline); }

                                       }
                    // Seal the gap if there is no branch
                        if(branches<0){
                            // Reconfigure the UV's
                        _output_uv[i+13]=Vector2(8.0,18.0);
                        _output_uv[i+3]=Vector2(7.5,18.0);
                        _output_uv[i+2]=Vector2(7.0,18.0);
                        _output_uv[i+1]=Vector2(6.5,18.0);
                        _output_uv[i]=Vector2(6.0,18.0);
                        _output_uv[i+12]=Vector2(6.0,18.0);
                        _output_uv[i+11]=Vector2(5.5,18.0);
                        _output_uv[i+10]=Vector2(5.0,18.0);
                        _output_uv[i+9]=Vector2(4.5,18.0);
                        _output_uv[i+14]=Vector2(4.0,18.0);
                        _output_uv[i+4]=Vector2(8.0,21.0);
                        _output_uv[i+5]=Vector2(7.18,22.37);
                        _output_uv[i+6]=Vector2(6.0,22.7);
                        _output_uv[i+7]=Vector2(5.3,22.07);
                        _output_uv[i+8]=Vector2(4.0,21.0);

                        // Face up the hole
                        _output_indices.push_back(i);
                        _output_indices.push_back(i+6);
                        _output_indices.push_back(i+5);
                        _output_indices.push_back(i);
                        _output_indices.push_back(i+5);
                        _output_indices.push_back(i+1);
                        _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
                        ++i;
                        _output_indices.push_back(i+4);
                        _output_indices.push_back(i+3);
                        _output_indices.push_back(i);
                        _output_indices.push_back(i+3);
                        _output_indices.push_back(i+1);
                        _output_indices.push_back(i);
                        _output_normals.push_back(_output_normals[i-33]);
                        ++i;
                        _output_indices.push_back(i+2);
                        _output_indices.push_back(i+1);
                        _output_indices.push_back(i);
                        _output_normals.push_back(_output_normals[i-33]);
                        ++i;
                        _output_indices.push_back(i);
                        _output_indices.push_back(i+1);
                        _output_indices.push_back(i+10);
                        _output_normals.push_back(_output_normals[i-33]);
                        ++i;
                        _output_normals.push_back((((_output_vertices[i-3]-_output_vertices[i+1])*(_output_vertices[i]-_output_vertices[i+1]))+((_output_vertices[i-3]-_output_vertices[i])*(_output_vertices[i-2]-_output_vertices[i]))+((_output_vertices[i-2]-_output_vertices[i])*(_output_vertices[i-1]-_output_vertices[i]))+((_output_vertices[i+9]-_output_vertices[i-1])*(_output_vertices[i]-_output_vertices[i-1]))+((_output_vertices[i+1]-_output_vertices[i-12])*(_output_vertices[i]-_output_vertices[i-12]))+((_output_vertices[i-13]-_output_vertices[i])*(_output_vertices[i-12]-_output_vertices[i]))+((_output_vertices[i]-_output_vertices[i-13])*(_output_vertices[i-29]-_output_vertices[i-13]))+((_output_vertices[i-29]-_output_vertices[i+9])*(_output_vertices[i]-_output_vertices[i+9]))).normalized());
                        _output_normals[i-28]=_output_normals[i];
                        ++i;
                        _output_normals.push_back((((_output_vertices[i]-_output_vertices[i-5])*(_output_vertices[i+1]-_output_vertices[i-5]))+((_output_vertices[i-4]-_output_vertices[i-5])*(_output_vertices[i]-_output_vertices[i-5]))+((_output_vertices[i-4]-_output_vertices[i])*(_output_vertices[i-1]-_output_vertices[i]))+((_output_vertices[i+1]-_output_vertices[i-12])*(_output_vertices[i]-_output_vertices[i-12]))+((_output_vertices[i-13]-_output_vertices[i])*(_output_vertices[i-12]-_output_vertices[i]))+((_output_vertices[i]-_output_vertices[i-13])*(_output_vertices[i-1]-_output_vertices[i-13]))).normalized());
                        _output_normals[i-28]=_output_normals[i];
                        ++i;
                        _output_indices.push_back(i);
                        _output_indices.push_back(i-6);
                        _output_indices.push_back(i+5);
                        _output_indices.push_back(i+5);
                        _output_indices.push_back(i+1);
                        _output_indices.push_back(i);
                        _output_normals.push_back((((_output_vertices[i]-_output_vertices[i+5])*(_output_vertices[i+1]-_output_vertices[i+5]))+((_output_vertices[i+5]-_output_vertices[i])*(_output_vertices[i+6]-_output_vertices[i]))+((_output_vertices[i-1]-_output_vertices[i-6])*(_output_vertices[i]-_output_vertices[i-6]))+((_output_vertices[i+1]-_output_vertices[i-12])*(_output_vertices[i]-_output_vertices[i-12]))+((_output_vertices[i-13]-_output_vertices[i])*(_output_vertices[i-12]-_output_vertices[i]))+((_output_vertices[i]-_output_vertices[i-13])*(_output_vertices[i-1]-_output_vertices[i-13]))).normalized());
                        _output_normals[i-28]=_output_normals[i];
                        ++i;
                        _output_indices.push_back(i+3);
                        _output_indices.push_back(i);
                        _output_indices.push_back(i+4);
                        _output_indices.push_back(i+3);
                        _output_indices.push_back(i+1);
                        _output_indices.push_back(i);
                        _output_normals.push_back((((_output_vertices[i]-_output_vertices[i+3])*(_output_vertices[i+1]-_output_vertices[i+3]))+((_output_vertices[i+4]-_output_vertices[i+3])*(_output_vertices[i]-_output_vertices[i+3]))+((_output_vertices[i-1]-_output_vertices[i+4])*(_output_vertices[i]-_output_vertices[i+4]))+((_output_vertices[i+1]-_output_vertices[i-12])*(_output_vertices[i]-_output_vertices[i-12]))+((_output_vertices[i-13]-_output_vertices[i])*(_output_vertices[i-12]-_output_vertices[i]))+((_output_vertices[i]-_output_vertices[i-13])*(_output_vertices[i-1]-_output_vertices[i-13]))).normalized());
                        _output_normals[i-28]=_output_normals[i];
                        ++i;
                        _output_indices.push_back(i);
                        _output_indices.push_back(i+2);
                        _output_indices.push_back(i+1);
                        _output_indices.push_back(i);
                        _output_indices.push_back(i+1);
                        _output_indices.push_back(i+6);
                        _output_normals.push_back((((_output_vertices[i+6]-_output_vertices[i])*(_output_vertices[i+1]-_output_vertices[i]))+((_output_vertices[i+1]-_output_vertices[i])*(_output_vertices[i+2]-_output_vertices[i]))+((_output_vertices[i-1]-_output_vertices[i+2])*(_output_vertices[i]-_output_vertices[i+2]))+((_output_vertices[i]-_output_vertices[i+6])*(_output_vertices[i-27]-_output_vertices[i+6]))+((_output_vertices[i-27]-_output_vertices[i-12])*(_output_vertices[i]-_output_vertices[i-12]))+((_output_vertices[i]-_output_vertices[i-13])*(_output_vertices[i-1]-_output_vertices[i-13]))+((_output_vertices[i-13]-_output_vertices[i])*(_output_vertices[i-12]-_output_vertices[i]))).normalized());
                        _output_normals[i-28]=_output_normals[i];
                        ++i;
                        _output_normals.push_back(_output_normals[i-45]);
                        ++i;
                        _output_normals.push_back(_output_normals[i-45]);
                        ++i;
                        _output_normals.push_back(_output_normals[i-45]);
                        ++i;
                        _output_normals.push_back(_output_normals[i-45]);
                        _output_normals[i-12]=_output_normals[i];
                        ++i;
                        // this one is messed up?
                        _output_normals.push_back(_output_normals[i-58]);
                        _output_normals[i-28]=_output_normals[i];
                        _output_normals[i-42]=_output_normals[i];
                        ++i;
                        _output_normals.push_back(_output_normals[i-51]);
                        _output_normals[i-28]=_output_normals[i];
                        ++i;

                        branch1_start = i-28;
                        parent_branch1_start = i-43;
                        float rand_branch_type = -50.0 + (300.0 * ((float)random() / RAND_MAX));
                        if (rand_branch_type<0){ i += _make_end(branch1_start, 0, parent_branch1_start, true, 0, max_branch_layer, branch_min_incline, branch_max_incline); }
                        if ((rand_branch_type>=0)&&(rand_branch_type<50.0)) { i += _make_end(branch1_start, 1, parent_branch1_start, true, 0, max_branch_layer, branch_min_incline, branch_max_incline); }
                        if ((rand_branch_type>=50.0)&&(rand_branch_type<100.0)) { i += _make_end(branch1_start, 2, parent_branch1_start, true, 0, max_branch_layer, branch_min_incline, branch_max_incline); }
                        if (rand_branch_type>=100.0) { i += _make_end(branch1_start, 3, parent_branch1_start, true, 0, max_branch_layer, branch_min_incline, branch_max_incline); }
                        }

                    }
                }
            }

        }

}

        if (_output_vertices.size() == 0) {
            print_line("No vertices generated!");
            return Ref<Mesh>();
        }

    PoolVector<Vector3> pool_vertices;
    PoolVector<Vector3> pool_normals;
    PoolVector<Color> pool_colors;
    PoolVector<int> pool_indices;
    PoolVector<Vector2> pool_uvs;

    copy_to(pool_vertices, _output_vertices);
    copy_to(pool_normals, _output_normals);
    copy_to(pool_colors, _output_colors);
    copy_to(pool_indices, _output_indices);
    copy_to(pool_uvs, _output_uv);

    Array arrays;
    arrays.resize(Mesh::ARRAY_MAX);
    arrays[Mesh::ARRAY_VERTEX] = pool_vertices;
    arrays[Mesh::ARRAY_NORMAL] = pool_normals;
    arrays[Mesh::ARRAY_COLOR] = pool_colors;
    arrays[Mesh::ARRAY_INDEX] = pool_indices;
    arrays[Mesh::ARRAY_TEX_UV] = pool_uvs;

    Ref<ArrayMesh> mesh_ref(memnew(ArrayMesh));
    mesh_ref->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);

    return mesh_ref;
}

int HeightMapDecels::_make_end(int branch, int branch_type, int parent_branch, bool clock_wise, int current_branch_layer, int max_branch_layer, float branch_min_incline, float branch_max_incline) {
    int j = _output_vertices.size();
    if (current_branch_layer>=1) {++current_branch_layer;}
    int new_branch_layer = current_branch_layer+1;
    if (new_branch_layer>=max_branch_layer) {branch_type=0;}
    float branch_incline = branch_min_incline + (((2.0f*3.14159263)*branch_max_incline)* ((float)random() / RAND_MAX));
    //j0
    _output_vertices.push_back(_output_vertices[branch+3]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(0.0,0.0));
    //j1
    _output_vertices.push_back(_output_vertices[branch+2]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(0.5,0.0));
    //j2
    _output_vertices.push_back(_output_vertices[branch+1]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(1.0,0.0));
    //j3
    _output_vertices.push_back(_output_vertices[branch]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(1.5,0.0));
    //j4->j8
    Vector3 A = ((_output_vertices[branch]-_output_vertices[branch+6])/2.0)+_output_vertices[branch+6];
    Vector3 N = (A - _output_vertices[branch+3]).normalized();
    Vector3 N2 = (_output_vertices[branch]-_output_vertices[branch+6]).normalized();
    Vector3 H = A - _output_vertices[branch];
    Vector3 B = _output_vertices[branch];


    float direction = 1.0;
    if (clock_wise==false) { direction = -1.0; }
    for(int hi = 5; hi > 0; --hi)
    {
        float angle = direction * hi * (2.0f*3.14159263 / 12.0f);
        Vector3 rotatedH = H.rotated(N, angle);
        rotatedH = rotatedH.rotated(N2, branch_incline);
        _output_vertices.push_back(A + rotatedH);
        _output_colors.push_back(Color(0,0,0,0));
        _output_uv.push_back(Vector2((((6.0-float(hi))*0.5)+1.5),4.0));
    }

    //j9
    _output_vertices.push_back(_output_vertices[branch+6]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(4.5,0.0));
    //j10
    _output_vertices.push_back(_output_vertices[branch+5]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(5.0,0.0));
    //j11
    _output_vertices.push_back(_output_vertices[branch+4]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(5.5,0.0));
    //j12 repeat of the first vertex of the cylindery joiny branch node thingamajig
    _output_vertices.push_back(_output_vertices[branch+3]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(6.0,0.0));
    //j13 Start of 2nd stump or branch
    _output_vertices.push_back(_output_vertices[branch+9]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(0.0,0.0));
    //j14
    _output_vertices.push_back(_output_vertices[branch+10]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(0.5,0.0));
    //j15
    _output_vertices.push_back(_output_vertices[branch+11]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(1.0,0.0));
    //j16
    _output_vertices.push_back(_output_vertices[branch+12]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(1.5,0.0));
    //j17
    _output_vertices.push_back(_output_vertices[j+4]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(2.0,0.0));
    //j18
    _output_vertices.push_back(_output_vertices[j+5]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(2.5,0.0));
    //j19
    _output_vertices.push_back(_output_vertices[j+6]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(3.0,0.0));
    //j20
    _output_vertices.push_back(_output_vertices[j+7]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(3.5,0.0));
    //j21
    _output_vertices.push_back(_output_vertices[j+8]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(4.0,0.0));
    //j22
    _output_vertices.push_back(_output_vertices[branch+6]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(4.5,0.0));
    //j23
    _output_vertices.push_back(_output_vertices[branch+7]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(5.0,0.0));
    //j24
    _output_vertices.push_back(_output_vertices[branch+8]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(5.5,0.0));
    //j25 Repeat of first vertex of branch join cylindery thingamajig
    _output_vertices.push_back(_output_vertices[branch+9]);
    _output_colors.push_back(Color(0,0,0,0));
    _output_uv.push_back(Vector2(6.0,0.0));

    if (clock_wise==true) {
        if ((branch_type==0)||(branch_type==1)) {
            _output_indices.push_back(j);
            _output_indices.push_back(j+6);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j+1);
            _output_indices.push_back(j);
            _output_normals.push_back(_output_normals[branch+3]);
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+4);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j+1);
            _output_indices.push_back(j);
            _output_normals.push_back(_output_normals[branch+2]);
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+2);
            _output_indices.push_back(j+1);
            _output_normals.push_back(_output_normals[branch+1]);
            ++j;
            _output_normals.push_back(_output_normals[branch]);
            ++j;
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j-6);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j+1);
            _output_indices.push_back(j);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+4);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j+1);
            _output_indices.push_back(j);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+2);
            _output_indices.push_back(j+1);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_normals.push_back(_output_normals[branch+6]);
            ++j;
            _output_normals.push_back(_output_normals[branch+5]);
            ++j;
            _output_normals.push_back(_output_normals[branch+4]);
            ++j;
            _output_normals.push_back(_output_normals[branch+3]);
            ++j;
        }
        if ((branch_type==2)||(branch_type==3)) {

            Vector3 A = ((_output_vertices[j+6]-_output_vertices[j])/2.0)+_output_vertices[j];
            Vector3 C = ((_output_vertices[j+3]-_output_vertices[j+9])/2.0)+_output_vertices[j+9];
            float y_bend = sqrt((((A-C).x)*((A-C).x))+(((A-C).y)*((A-C).y))+(((A-C).z)*((A-C).z)))*0.3-(0.005*current_branch_layer*sqrt((((A-C).x)*((A-C).x))+(((A-C).y)*((A-C).y))+(((A-C).z)*((A-C).z))));
            A.y+=y_bend;
            N = (A-C).normalized();
            float NB = sqrt((N.x*N.x)+(N.y*N.y)+(N.z*N.z));
            Vector3 H = 1.15*(_output_vertices[j]-A);
            float branch_length = 0.18*float(current_branch_layer)*(0.9 + (1.6 * ((float)random() / RAND_MAX)))* sqrt((H.x*H.x)+(H.y*H.y)+(H.z*H.z));
            Vector3 B = ((N/NB)*branch_length) + A;

            for(int hi = 0; hi <= 12; ++hi)
            {
                float angle = hi * (2.0f*3.14159263 / 12.0f);
                Vector3 rotatedH = H.rotated(N, angle);
                _output_vertices.push_back(B + rotatedH);
                _output_colors.push_back(Color(0,0,0,0));
                _output_uv.push_back(Vector2((float(hi)*0.5),4.0));
                _output_uv[j+hi]=Vector2((float(hi)*0.5),0.0);
            }
            for(int hi = 0; hi < 12; ++hi)
            {
            _output_indices.push_back(j);
            _output_indices.push_back(j+26);
            _output_indices.push_back(j+27);
            _output_indices.push_back(j);
            _output_indices.push_back(j+27);
            _output_indices.push_back(j+1);

            ++j;
            }

            _output_normals.push_back(_output_normals[branch+3]);
            _output_normals.push_back(_output_normals[branch+2]);
            _output_normals.push_back(_output_normals[branch+1]);
            _output_normals.push_back(_output_normals[branch]);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back(_output_normals[branch+6]);
            _output_normals.push_back(_output_normals[branch+5]);
            _output_normals.push_back(_output_normals[branch+4]);
            _output_normals.push_back(_output_normals[branch+3]);

            ++j;
        }
    // other side
        if ((branch_type==0)||(branch_type==2)) {

            _output_indices.push_back(j);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j+6);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j);
            _output_indices.push_back(j+1);
            _output_normals.push_back(_output_normals[branch+9]);
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j+4);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j);
            _output_indices.push_back(j+1);
            _output_normals.push_back(_output_normals[branch+10]);
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+1);
            _output_indices.push_back(j+2);
            _output_normals.push_back(_output_normals[branch+11]);
            ++j;
            _output_normals.push_back(_output_normals[branch]);
            ++j;
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j-6);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j);
            _output_indices.push_back(j+1);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j+4);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j);
            _output_indices.push_back(j+1);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+1);
            _output_indices.push_back(j+2);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_normals.push_back(_output_normals[branch+6]);
            ++j;
            _output_normals.push_back(_output_normals[branch+7]);
            ++j;
            _output_normals.push_back(_output_normals[branch+8]);
            ++j;
            _output_normals.push_back(_output_normals[branch+9]);
            ++j;

            _output_uv[j-10]=Vector2(-1.0,0.0);
            _output_uv[j-11]=Vector2(-0.5,0.0);
            _output_uv[j-12]=Vector2(0.0,0.0);
            _output_uv[j-13]=Vector2(0.5,0.0);
            _output_uv[j-1]=Vector2(0.5,0.0);
            _output_uv[j-2]=Vector2(1.0,0.0);
            _output_uv[j-3]=Vector2(1.5,0.0);
            _output_uv[j-4]=Vector2(2.0,0.0);
            _output_uv[j-5]=Vector2(1.5,1.5);
            _output_uv[j-6]=Vector2(1.0,1.9);
            _output_uv[j-7]=Vector2(0.5,2.0);
            _output_uv[j-8]=Vector2(0.0,1.9);
            _output_uv[j-9]=Vector2(-0.5,1.5);

        }
        if (branch_type==0) {
            _output_uv[j-23]=Vector2(7.0,0.0);
            _output_uv[j-24]=Vector2(6.5,0.0);
            _output_uv[j-25]=Vector2(6.0,0.0);
            _output_uv[j-26]=Vector2(5.5,0.0);
            _output_uv[j-14]=Vector2(5.5,0.0);
            _output_uv[j-15]=Vector2(5.0,0.0);
            _output_uv[j-16]=Vector2(4.5,0.0);
            _output_uv[j-17]=Vector2(4.0,0.0);
            _output_uv[j-22]=Vector2(4.0,1.5);
            _output_uv[j-21]=Vector2(4.75,1.9);
            _output_uv[j-20]=Vector2(5.5,2.0);
            _output_uv[j-19]=Vector2(6.25,1.9);
            _output_uv[j-18]=Vector2(7.0,1.5);
            j-=26;
            _output_normals[j] = (((_output_vertices[j+11]-_output_vertices[parent_branch+4])*(_output_vertices[j]-_output_vertices[parent_branch+4]))+((_output_vertices[parent_branch+3]-_output_vertices[j])*(_output_vertices[parent_branch+4]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+3])*(_output_vertices[j+1]-_output_vertices[parent_branch+3]))+((_output_vertices[j]-_output_vertices[j+11])*(_output_vertices[j+6]-_output_vertices[j+11]))+((_output_vertices[j+5]-_output_vertices[j])*(_output_vertices[j+6]-_output_vertices[j]))+((_output_vertices[j+1]-_output_vertices[j])*(_output_vertices[j+5]-_output_vertices[j]))).normalized();
            _output_normals[branch+3] = _output_normals[j];
            _output_normals[j+12] = _output_normals[j];
            ++j;
            _output_normals[j] = (((_output_vertices[j-1]-_output_vertices[parent_branch+3])*(_output_vertices[j]-_output_vertices[parent_branch+3]))+((_output_vertices[parent_branch+2]-_output_vertices[j])*(_output_vertices[parent_branch+2]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+2])*(_output_vertices[j+1]-_output_vertices[parent_branch+2]))+((_output_vertices[j]-_output_vertices[j-1])*(_output_vertices[j+4]-_output_vertices[j-1]))+((_output_vertices[j+3]-_output_vertices[j])*(_output_vertices[j+4]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[j+3])*(_output_vertices[j+1]-_output_vertices[j+3])) ).normalized();
            _output_normals[branch+2] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+2])*(_output_vertices[j]-_output_vertices[parent_branch+2]))+((_output_vertices[parent_branch+1]-_output_vertices[j])*(_output_vertices[parent_branch+2]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+1])*(_output_vertices[j+1]-_output_vertices[parent_branch+1]))+((_output_vertices[j-1]-_output_vertices[j+2])*(_output_vertices[j]-_output_vertices[j+2]))+((_output_vertices[j+1]-_output_vertices[j])*(_output_vertices[j+2]-_output_vertices[j])) ).normalized();
            _output_normals[branch+1] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+1])*(_output_vertices[j]-_output_vertices[parent_branch+1]))+((_output_vertices[parent_branch]-_output_vertices[j])*(_output_vertices[parent_branch+1]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch])*(_output_vertices[branch+11]-_output_vertices[parent_branch]))+((_output_vertices[j]-_output_vertices[j-1])*(_output_vertices[j+1]-_output_vertices[j-1]))+((_output_vertices[j+1]-_output_vertices[branch+11])*(_output_vertices[j]-_output_vertices[branch+11])) ).normalized();
            _output_normals[j+13] = _output_normals[j];
            _output_normals[branch] = _output_normals[j];
            _output_normals[branch+12] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j]-_output_vertices[j-3])*(_output_vertices[j+1]-_output_vertices[j-3]))+((_output_vertices[j-3]-_output_vertices[j])*(_output_vertices[j-2]-_output_vertices[j]))+((_output_vertices[j-1]-_output_vertices[j-2])*(_output_vertices[j]-_output_vertices[j-2]))+((_output_vertices[j+1]-_output_vertices[branch+10])*(_output_vertices[j]-_output_vertices[branch+10]))+((_output_vertices[branch+11]-_output_vertices[j])*(_output_vertices[branch+10]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[branch+11])*(_output_vertices[j-1]-_output_vertices[branch+11])) ).normalized();
            _output_normals[j+13] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j]-_output_vertices[j-5])*(_output_vertices[j+1]-_output_vertices[j-5]))+((_output_vertices[j-5]-_output_vertices[j])*(_output_vertices[j-4]-_output_vertices[j]))+((_output_vertices[j-1]-_output_vertices[j-4])*(_output_vertices[j]-_output_vertices[j-4]))+((_output_vertices[j+1]-_output_vertices[branch+9])*(_output_vertices[j]-_output_vertices[branch+9]))+((_output_vertices[branch+10]-_output_vertices[j])*(_output_vertices[branch+9]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[branch+10])*(_output_vertices[j-1]-_output_vertices[branch+10])) ).normalized();
            _output_normals[j+13] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j]-_output_vertices[j+5])*(_output_vertices[j+1]-_output_vertices[j+5]))+((_output_vertices[j+5]-_output_vertices[j])*(_output_vertices[j-6]-_output_vertices[j]))+((_output_vertices[j-1]-_output_vertices[j-6])*(_output_vertices[j]-_output_vertices[j-6]))+((_output_vertices[j+1]-_output_vertices[branch+8])*(_output_vertices[j]-_output_vertices[branch+8]))+((_output_vertices[branch+9]-_output_vertices[j])*(_output_vertices[branch+8]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[branch+9])*(_output_vertices[j-1]-_output_vertices[branch+9])) ).normalized();
            _output_normals[j+13] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j]-_output_vertices[j+3])*(_output_vertices[j+1]-_output_vertices[j+3]))+((_output_vertices[j+3]-_output_vertices[j])*(_output_vertices[j+4]-_output_vertices[j]))+((_output_vertices[j-1]-_output_vertices[j+4])*(_output_vertices[j]-_output_vertices[j+4]))+((_output_vertices[j+1]-_output_vertices[branch+7])*(_output_vertices[j]-_output_vertices[branch+7]))+((_output_vertices[branch+8]-_output_vertices[j])*(_output_vertices[branch+7]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[branch+8])*(_output_vertices[j-1]-_output_vertices[branch+8])) ).normalized();
            _output_normals[j+13] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j+1]-_output_vertices[j])*(_output_vertices[j+2]-_output_vertices[j]))+((_output_vertices[j-1]-_output_vertices[j+2])*(_output_vertices[j]-_output_vertices[j+2]))+((_output_vertices[branch+7]-_output_vertices[j])*(_output_vertices[j+1]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[branch+7])*(_output_vertices[j-1]-_output_vertices[branch+7])) ).normalized();
            _output_normals[j+13] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[branch+7]-_output_vertices[parent_branch+7])*(_output_vertices[j]-_output_vertices[parent_branch+7]))+((_output_vertices[parent_branch+6]-_output_vertices[j])*(_output_vertices[parent_branch+7]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+6])*(_output_vertices[j+1]-_output_vertices[parent_branch+6]))+((_output_vertices[branch+7]-_output_vertices[j-1])*(_output_vertices[j]-_output_vertices[j-1]))+((_output_vertices[j]-_output_vertices[j-1])*(_output_vertices[j+1]-_output_vertices[j-1])) ).normalized();
            _output_normals[j+13] = _output_normals[j];
            _output_normals[branch+6] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+6])*(_output_vertices[j]-_output_vertices[parent_branch+6]))+((_output_vertices[parent_branch+5]-_output_vertices[j])*(_output_vertices[parent_branch+6]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+5])*(_output_vertices[j+1]-_output_vertices[parent_branch+5]))+((_output_vertices[j-1]-_output_vertices[j-2])*(_output_vertices[j]-_output_vertices[j-2]))+((_output_vertices[j-3]-_output_vertices[j])*(_output_vertices[j-2]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[j-3])*(_output_vertices[j+1]-_output_vertices[j-3])) ).normalized();
            _output_normals[branch+5] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+5])*(_output_vertices[j]-_output_vertices[parent_branch+5]))+((_output_vertices[parent_branch+4]-_output_vertices[j])*(_output_vertices[parent_branch+5]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+4])*(_output_vertices[j+1]-_output_vertices[parent_branch+4]))+((_output_vertices[j-1]-_output_vertices[j-4])*(_output_vertices[j]-_output_vertices[j-4]))+((_output_vertices[j-5]-_output_vertices[j])*(_output_vertices[j-4]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[j-5])*(_output_vertices[j+1]-_output_vertices[j-5])) ).normalized();
            _output_normals[branch+4] = _output_normals[j];
            j+=2;
            _output_normals[j] = (((_output_vertices[j+11]-_output_vertices[parent_branch+8])*(_output_vertices[j]-_output_vertices[parent_branch+8]))+((_output_vertices[parent_branch+9]-_output_vertices[j])*(_output_vertices[parent_branch+8]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+9])*(_output_vertices[j+1]-_output_vertices[parent_branch+9]))+((_output_vertices[j]-_output_vertices[j+6])*(_output_vertices[j+11]-_output_vertices[j+6]))+((_output_vertices[j+6]-_output_vertices[j])*(_output_vertices[j+5]-_output_vertices[j]))+((_output_vertices[j+1]-_output_vertices[j+5])*(_output_vertices[j]-_output_vertices[j+5]))).normalized();
            _output_normals[branch+9] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+9])*(_output_vertices[j]-_output_vertices[parent_branch+9]))+((_output_vertices[parent_branch+10]-_output_vertices[j])*(_output_vertices[parent_branch+9]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+10])*(_output_vertices[j+1]-_output_vertices[parent_branch+10]))+((_output_vertices[j]-_output_vertices[j+4])*(_output_vertices[j-1]-_output_vertices[j+4]))+((_output_vertices[j+4]-_output_vertices[j])*(_output_vertices[j+3]-_output_vertices[j]))+((_output_vertices[j+1]-_output_vertices[j+3])*(_output_vertices[j]-_output_vertices[j+3])) ).normalized();
            _output_normals[branch+10] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+10])*(_output_vertices[j]-_output_vertices[parent_branch+10]))+((_output_vertices[parent_branch+11]-_output_vertices[j])*(_output_vertices[parent_branch+10]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+11])*(_output_vertices[j+1]-_output_vertices[parent_branch+11]))+((_output_vertices[j]-_output_vertices[j+2])*(_output_vertices[j-1]-_output_vertices[j+2]))+((_output_vertices[j+2]-_output_vertices[j])*(_output_vertices[j+1]-_output_vertices[j])) ).normalized();
            _output_normals[branch+11] = _output_normals[j];
            j+=8;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+6])*(_output_vertices[j]-_output_vertices[parent_branch+6]))+((_output_vertices[parent_branch+7]-_output_vertices[j])*(_output_vertices[parent_branch+6]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+7])*(_output_vertices[j+1]-_output_vertices[parent_branch+7]))+((_output_vertices[j]-_output_vertices[j-2])*(_output_vertices[j-1]-_output_vertices[j-2]))+((_output_vertices[j-2]-_output_vertices[j])*(_output_vertices[j-3]-_output_vertices[j]))+((_output_vertices[j+1]-_output_vertices[j-3])*(_output_vertices[j]-_output_vertices[j-3])) ).normalized();
            _output_normals[branch+7] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+7])*(_output_vertices[j]-_output_vertices[parent_branch+7]))+((_output_vertices[parent_branch+8]-_output_vertices[j])*(_output_vertices[parent_branch+7]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+8])*(_output_vertices[j+1]-_output_vertices[parent_branch+8]))+((_output_vertices[j]-_output_vertices[j-4])*(_output_vertices[j-1]-_output_vertices[j-4]))+((_output_vertices[j-4]-_output_vertices[j])*(_output_vertices[j-5]-_output_vertices[j]))+((_output_vertices[j+1]-_output_vertices[j-5])*(_output_vertices[j]-_output_vertices[j-5])) ).normalized();
            _output_normals[branch+8] = _output_normals[j];
            ++j;

        }

        if (branch_type==2) {
            Vector3 C = ((_output_vertices[j]-_output_vertices[j+6])/2.0)+_output_vertices[j+6];

            for (int hi = 0; hi <= 12; ++hi){
                _output_normals.push_back((_output_vertices[j]-C).normalized());
                ++j;
            }

            int branch1_start = j-13;
            int parent_branch1_start = j-39;
            bool last_wise = false;
            if (current_branch_layer&1) { last_wise = true;}

            int returnval = 39;
             float rand_branch_type = -50.0 + (300 * ((float)random() / RAND_MAX));
             if (rand_branch_type<0){ returnval += _make_end(branch1_start, 0, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
             if ((rand_branch_type>=0)&&(rand_branch_type<50.0)) { returnval += _make_end(branch1_start, 1, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
             if ((rand_branch_type>=50.0)&&(rand_branch_type<100.0)) { returnval += _make_end(branch1_start, 2, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
             if (rand_branch_type>=100.0) { returnval += _make_end(branch1_start, 3, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
            return int(returnval);

        }


        if ((branch_type==1)||(branch_type==3)) {
            _output_normals.push_back(_output_normals[branch+9]);
            _output_normals.push_back(_output_normals[branch+10]);
            _output_normals.push_back(_output_normals[branch+11]);
            _output_normals.push_back(_output_normals[branch]);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back(_output_normals[branch+6]);
            _output_normals.push_back(_output_normals[branch+7]);
            _output_normals.push_back(_output_normals[branch+8]);
            _output_normals.push_back(_output_normals[branch+9]);

            Vector3 A = ((_output_vertices[j+6]-_output_vertices[j])/2.0)+_output_vertices[j];
            Vector3 C = ((_output_vertices[j+3]-_output_vertices[j+9])/2.0)+_output_vertices[j+9];
            float y_bend = sqrt((((A-C).x)*((A-C).x))+(((A-C).y)*((A-C).y))+(((A-C).z)*((A-C).z)))*0.3-(0.005*current_branch_layer*sqrt((((A-C).x)*((A-C).x))+(((A-C).y)*((A-C).y))+(((A-C).z)*((A-C).z))));
            A.y+=y_bend;
            N = (A-C).normalized();
            float NB = sqrt((N.x*N.x)+(N.y*N.y)+(N.z*N.z));
            Vector3 H = 1.15*(_output_vertices[j]-A);
            float branch_length = 0.18*float(current_branch_layer)*(0.9 + (1.6 * ((float)random() / RAND_MAX)))* sqrt((H.x*H.x)+(H.y*H.y)+(H.z*H.z));
            Vector3 B = ((N/NB)*(branch_length)) + A;
            for(int hi = 0; hi <= 12; ++hi) {
                float angle = hi * (-2.0f*3.14159263 / 12.0f);
                Vector3 rotatedH = H.rotated(N, angle);
                _output_vertices.push_back(B + rotatedH);
                _output_colors.push_back(Color(0,0,0,0));
                _output_uv.push_back(Vector2((float(hi)*0.5),4.0));
            }
        }
        if (branch_type==1) {
            for(int hi = 0; hi < 12; ++hi)
            {
                _output_indices.push_back(j);
                _output_indices.push_back(j+14);
                _output_indices.push_back(j+13);
                _output_indices.push_back(j);
                _output_indices.push_back(j+1);
                _output_indices.push_back(j+14);
                ++j;
            }

            ++j;

            _output_uv[j-23]=Vector2(4.0,0.0);
            _output_uv[j-24]=Vector2(4.5,0.0);
            _output_uv[j-25]=Vector2(5.0,0.0);
            _output_uv[j-26]=Vector2(5.5,0.0);
            _output_uv[j-14]=Vector2(5.5,0.0);
            _output_uv[j-15]=Vector2(6.0,0.0);
            _output_uv[j-16]=Vector2(6.5,0.0);
            _output_uv[j-17]=Vector2(7.0,0.0);
            _output_uv[j-22]=Vector2(4.0,1.5);
            _output_uv[j-21]=Vector2(4.75,1.9);
            _output_uv[j-20]=Vector2(5.5,2.0);
            _output_uv[j-19]=Vector2(6.25,1.9);
            _output_uv[j-18]=Vector2(7.0,1.5);

            Vector3 C = ((_output_vertices[j]-_output_vertices[j+6])/2.0)+_output_vertices[j+6];

            for (int hi = 0; hi <= 12; ++hi){
                _output_normals.push_back((_output_vertices[j]-C).normalized());
                ++j;
            }

            int branch1_start = j-13;
            int parent_branch1_start = j-39;
            bool last_wise = true;
            if (current_branch_layer&1) { last_wise = false;}

            int returnval = 39;
             float rand_branch_type = -50.0 + (300 * ((float)random() / RAND_MAX));
             if (rand_branch_type<0){ returnval += _make_end(branch1_start, 0, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
             if ((rand_branch_type>=0)&&(rand_branch_type<50.0)) { returnval += _make_end(branch1_start, 1, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
             if ((rand_branch_type>=50.0)&&(rand_branch_type<100.0)) { returnval += _make_end(branch1_start, 2, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
             if (rand_branch_type>=100.0) { returnval += _make_end(branch1_start, 3, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
            return int(returnval);
        }

        if (branch_type==3) {
            for(int hi = 0; hi < 12; ++hi) {
                _output_indices.push_back(j);
                _output_indices.push_back(j+27);
                _output_indices.push_back(j+26);
                _output_indices.push_back(j);
                _output_indices.push_back(j+1);
                _output_indices.push_back(j+27);
                ++j;
            }
            ++j;
            Vector3 C = ((_output_vertices[j]-_output_vertices[j+6])/2.0)+_output_vertices[j+6];
            for (int hi = 0; hi <= 12; ++hi){
                _output_normals.push_back((_output_vertices[j]-C).normalized());
                ++j;
            }

            C = ((_output_vertices[j]-_output_vertices[j+6])/2.0)+_output_vertices[j+6];

            for (int hi = 0; hi <= 12; ++hi){
                _output_normals.push_back((_output_vertices[j]-C).normalized());
                ++j;
            }

            int branch1_start = j-26;
            int branch2_start = j-13;
            int parent_branch1_start = j-39;
            int parent_branch2_start = j-52;

            bool last_wise1 = false;
            bool last_wise2 = true;
            if (current_branch_layer&1) { last_wise1 = true; last_wise2=false;}

            int returnval = 52;
             float rand_branch_type = -50.0 + (300 * ((float)random() / RAND_MAX));
             if (rand_branch_type<0){ returnval += _make_end(branch1_start, 0, parent_branch1_start, last_wise1, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
             if ((rand_branch_type>=0)&&(rand_branch_type<50.0)) { returnval += _make_end(branch1_start, 1, parent_branch1_start, last_wise1, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
             if ((rand_branch_type>=50.0)&&(rand_branch_type<100.0)) { returnval += _make_end(branch1_start, 2, parent_branch1_start, last_wise1, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
             if (rand_branch_type>=100.0) { returnval += _make_end(branch1_start, 3, parent_branch1_start, last_wise1, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }

              rand_branch_type = -50.0 + (300 * ((float)random() / RAND_MAX));
              if (rand_branch_type<0){ returnval += _make_end(branch2_start, 0, parent_branch2_start, last_wise2, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
              if ((rand_branch_type>=0)&&(rand_branch_type<50.0)) { returnval += _make_end(branch2_start, 1, parent_branch2_start, last_wise2, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
              if ((rand_branch_type>=50.0)&&(rand_branch_type<100.0)) { returnval += _make_end(branch2_start, 2, parent_branch2_start, last_wise2, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
              if (rand_branch_type>=100.0) { returnval += _make_end(branch2_start, 3, parent_branch2_start, last_wise2, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
            return int(returnval);
        }
    }
    else {

        if ((branch_type==0)||(branch_type==1)) {
            _output_indices.push_back(j);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j+6);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j);
            _output_indices.push_back(j+1);
            _output_normals.push_back(_output_normals[branch+3]);
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j+4);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j);
            _output_indices.push_back(j+1);
            _output_normals.push_back(_output_normals[branch+2]);
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+1);
            _output_indices.push_back(j+2);
            _output_normals.push_back(_output_normals[branch+1]);
            ++j;
            _output_normals.push_back(_output_normals[branch]);
            ++j;
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j-6);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j);
            _output_indices.push_back(j+1);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j+4);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j);
            _output_indices.push_back(j+1);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+1);
            _output_indices.push_back(j+2);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_normals.push_back(_output_normals[branch+6]);
            ++j;
            _output_normals.push_back(_output_normals[branch+5]);
            ++j;
            _output_normals.push_back(_output_normals[branch+4]);
            ++j;
            _output_normals.push_back(_output_normals[branch+3]);
            ++j;
        }
        if ((branch_type==2)||(branch_type==3)) {

            Vector3 A = ((_output_vertices[j+6]-_output_vertices[j])/2.0)+_output_vertices[j];
            Vector3 C = ((_output_vertices[j+3]-_output_vertices[j+9])/2.0)+_output_vertices[j+9];
            float y_bend = sqrt((((A-C).x)*((A-C).x))+(((A-C).y)*((A-C).y))+(((A-C).z)*((A-C).z)))*0.3-(0.005*current_branch_layer*sqrt((((A-C).x)*((A-C).x))+(((A-C).y)*((A-C).y))+(((A-C).z)*((A-C).z))));
            A.y+=y_bend;
            N = (A-C).normalized();
            float NB = sqrt((N.x*N.x)+(N.y*N.y)+(N.z*N.z));
            Vector3 H = 1.15*(_output_vertices[j]-A);
            float branch_length = 0.18*float(current_branch_layer)*(0.9 + (1.6 * ((float)random() / RAND_MAX)))* sqrt((H.x*H.x)+(H.y*H.y)+(H.z*H.z));
            Vector3 B = ((N/NB)*branch_length) + A;

            for(int hi = 0; hi <= 12; ++hi)
            {
                float angle = hi * (-2.0f*3.14159263 / 12.0f);
                Vector3 rotatedH = H.rotated(N, angle);
                _output_vertices.push_back(B + rotatedH);
                _output_colors.push_back(Color(0,0,0,0));
                _output_uv.push_back(Vector2((float(hi)*0.5),4.0));
                _output_uv[j+hi]=Vector2((float(hi)*0.5),0.0);
            }


            for(int hi = 0; hi < 12; ++hi) {
                _output_indices.push_back(j);
                _output_indices.push_back(j+27);
                _output_indices.push_back(j+26);
                _output_indices.push_back(j);
                _output_indices.push_back(j+1);
                _output_indices.push_back(j+27);
                ++j;
            }
            _output_normals.push_back(_output_normals[branch+3]);
            _output_normals.push_back(_output_normals[branch+2]);
            _output_normals.push_back(_output_normals[branch+1]);
            _output_normals.push_back(_output_normals[branch]);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back(_output_normals[branch+6]);
            _output_normals.push_back(_output_normals[branch+5]);
            _output_normals.push_back(_output_normals[branch+4]);
            _output_normals.push_back(_output_normals[branch+3]);
            ++j;

        }

        if ((branch_type==0)||(branch_type==2)) {

            _output_indices.push_back(j);
            _output_indices.push_back(j+6);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j+1);
            _output_indices.push_back(j);
            _output_normals.push_back(_output_normals[branch+9]);
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+4);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j+1);
            _output_indices.push_back(j);
            _output_normals.push_back(_output_normals[branch+10]);
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+2);
            _output_indices.push_back(j+1);
            _output_normals.push_back(_output_normals[branch+11]);
            ++j;
            _output_normals.push_back(_output_normals[branch]);
            ++j;
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j-6);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j+5);
            _output_indices.push_back(j+1);
            _output_indices.push_back(j);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+4);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j+3);
            _output_indices.push_back(j+1);
            _output_indices.push_back(j);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_indices.push_back(j);
            _output_indices.push_back(j+2);
            _output_indices.push_back(j+1);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            ++j;
            _output_normals.push_back(_output_normals[branch+6]);
            ++j;
            _output_normals.push_back(_output_normals[branch+7]);
            ++j;
            _output_normals.push_back(_output_normals[branch+8]);
            ++j;
            _output_normals.push_back(_output_normals[branch+9]);
            ++j;


            _output_uv[j-10]=Vector2(-1.0,0.0);
            _output_uv[j-11]=Vector2(-0.5,0.0);
            _output_uv[j-12]=Vector2(0.0,0.0);
            _output_uv[j-13]=Vector2(0.5,0.0);
            _output_uv[j-1]=Vector2(0.5,0.0);
            _output_uv[j-2]=Vector2(1.0,0.0);
            _output_uv[j-3]=Vector2(1.5,0.0);
            _output_uv[j-4]=Vector2(2.0,0.0);
            _output_uv[j-5]=Vector2(1.5,1.5);
            _output_uv[j-6]=Vector2(1.0,1.9);
            _output_uv[j-7]=Vector2(0.5,2.0);
            _output_uv[j-8]=Vector2(0.0,1.9);
            _output_uv[j-9]=Vector2(-0.5,1.5);

        }

        if (branch_type==0) {
            _output_uv[j-23]=Vector2(7.0,0.0);
            _output_uv[j-24]=Vector2(6.5,0.0);
            _output_uv[j-25]=Vector2(6.0,0.0);
            _output_uv[j-26]=Vector2(5.5,0.0);
            _output_uv[j-14]=Vector2(5.5,0.0);
            _output_uv[j-15]=Vector2(5.0,0.0);
            _output_uv[j-16]=Vector2(4.5,0.0);
            _output_uv[j-17]=Vector2(4.0,0.0);
            _output_uv[j-22]=Vector2(4.0,1.5);
            _output_uv[j-21]=Vector2(4.75,1.9);
            _output_uv[j-20]=Vector2(5.5,2.0);
            _output_uv[j-19]=Vector2(6.25,1.9);
            _output_uv[j-18]=Vector2(7.0,1.5);

            j-=26;
            _output_normals[j] = (((_output_vertices[j+11]-_output_vertices[parent_branch+4])*(_output_vertices[j]-_output_vertices[parent_branch+4]))+((_output_vertices[parent_branch+3]-_output_vertices[j])*(_output_vertices[parent_branch+4]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+3])*(_output_vertices[j+1]-_output_vertices[parent_branch+3]))+((_output_vertices[j]-_output_vertices[j+11])*(_output_vertices[j+6]-_output_vertices[j+11]))+((_output_vertices[j+5]-_output_vertices[j])*(_output_vertices[j+6]-_output_vertices[j]))+((_output_vertices[j+1]-_output_vertices[j])*(_output_vertices[j+5]-_output_vertices[j]))).normalized();
            _output_normals[branch+3] = _output_normals[j];
            _output_normals[j+12] = _output_normals[j];
            ++j;
            _output_normals[j] = (((_output_vertices[j-1]-_output_vertices[parent_branch+3])*(_output_vertices[j]-_output_vertices[parent_branch+3]))+((_output_vertices[parent_branch+2]-_output_vertices[j])*(_output_vertices[parent_branch+2]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+2])*(_output_vertices[j+1]-_output_vertices[parent_branch+2]))+((_output_vertices[j]-_output_vertices[j-1])*(_output_vertices[j+4]-_output_vertices[j-1]))+((_output_vertices[j+3]-_output_vertices[j])*(_output_vertices[j+4]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[j+3])*(_output_vertices[j+1]-_output_vertices[j+3])) ).normalized();
            _output_normals[branch+2] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+2])*(_output_vertices[j]-_output_vertices[parent_branch+2]))+((_output_vertices[parent_branch+1]-_output_vertices[j])*(_output_vertices[parent_branch+2]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+1])*(_output_vertices[j+1]-_output_vertices[parent_branch+1]))+((_output_vertices[j-1]-_output_vertices[j+2])*(_output_vertices[j]-_output_vertices[j+2]))+((_output_vertices[j+1]-_output_vertices[j])*(_output_vertices[j+2]-_output_vertices[j])) ).normalized();
            _output_normals[branch+1] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+1])*(_output_vertices[j]-_output_vertices[parent_branch+1]))+((_output_vertices[parent_branch]-_output_vertices[j])*(_output_vertices[parent_branch+1]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch])*(_output_vertices[branch+11]-_output_vertices[parent_branch]))+((_output_vertices[j]-_output_vertices[j-1])*(_output_vertices[j+1]-_output_vertices[j-1]))+((_output_vertices[j+1]-_output_vertices[branch+11])*(_output_vertices[j]-_output_vertices[branch+11])) ).normalized();
            _output_normals[j+13] = _output_normals[j];
            _output_normals[branch] = _output_normals[j];
            _output_normals[branch+12] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j]-_output_vertices[j-3])*(_output_vertices[j+1]-_output_vertices[j-3]))+((_output_vertices[j-3]-_output_vertices[j])*(_output_vertices[j-2]-_output_vertices[j]))+((_output_vertices[j-1]-_output_vertices[j-2])*(_output_vertices[j]-_output_vertices[j-2]))+((_output_vertices[j+1]-_output_vertices[branch+10])*(_output_vertices[j]-_output_vertices[branch+10]))+((_output_vertices[branch+11]-_output_vertices[j])*(_output_vertices[branch+10]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[branch+11])*(_output_vertices[j-1]-_output_vertices[branch+11])) ).normalized();
            _output_normals[j+13] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j]-_output_vertices[j-5])*(_output_vertices[j+1]-_output_vertices[j-5]))+((_output_vertices[j-5]-_output_vertices[j])*(_output_vertices[j-4]-_output_vertices[j]))+((_output_vertices[j-1]-_output_vertices[j-4])*(_output_vertices[j]-_output_vertices[j-4]))+((_output_vertices[j+1]-_output_vertices[branch+9])*(_output_vertices[j]-_output_vertices[branch+9]))+((_output_vertices[branch+10]-_output_vertices[j])*(_output_vertices[branch+9]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[branch+10])*(_output_vertices[j-1]-_output_vertices[branch+10])) ).normalized();
            _output_normals[j+13] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j]-_output_vertices[j+5])*(_output_vertices[j+1]-_output_vertices[j+5]))+((_output_vertices[j+5]-_output_vertices[j])*(_output_vertices[j-6]-_output_vertices[j]))+((_output_vertices[j-1]-_output_vertices[j-6])*(_output_vertices[j]-_output_vertices[j-6]))+((_output_vertices[j+1]-_output_vertices[branch+8])*(_output_vertices[j]-_output_vertices[branch+8]))+((_output_vertices[branch+9]-_output_vertices[j])*(_output_vertices[branch+8]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[branch+9])*(_output_vertices[j-1]-_output_vertices[branch+9])) ).normalized();
            _output_normals[j+13] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j]-_output_vertices[j+3])*(_output_vertices[j+1]-_output_vertices[j+3]))+((_output_vertices[j+3]-_output_vertices[j])*(_output_vertices[j+4]-_output_vertices[j]))+((_output_vertices[j-1]-_output_vertices[j+4])*(_output_vertices[j]-_output_vertices[j+4]))+((_output_vertices[j+1]-_output_vertices[branch+7])*(_output_vertices[j]-_output_vertices[branch+7]))+((_output_vertices[branch+8]-_output_vertices[j])*(_output_vertices[branch+7]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[branch+8])*(_output_vertices[j-1]-_output_vertices[branch+8])) ).normalized();
            _output_normals[j+13] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j+1]-_output_vertices[j])*(_output_vertices[j+2]-_output_vertices[j]))+((_output_vertices[j-1]-_output_vertices[j+2])*(_output_vertices[j]-_output_vertices[j+2]))+((_output_vertices[branch+7]-_output_vertices[j])*(_output_vertices[j+1]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[branch+7])*(_output_vertices[j-1]-_output_vertices[branch+7])) ).normalized();
            _output_normals[j+13] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[branch+7]-_output_vertices[parent_branch+7])*(_output_vertices[j]-_output_vertices[parent_branch+7]))+((_output_vertices[parent_branch+6]-_output_vertices[j])*(_output_vertices[parent_branch+7]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+6])*(_output_vertices[j+1]-_output_vertices[parent_branch+6]))+((_output_vertices[branch+7]-_output_vertices[j-1])*(_output_vertices[j]-_output_vertices[j-1]))+((_output_vertices[j]-_output_vertices[j-1])*(_output_vertices[j+1]-_output_vertices[j-1])) ).normalized();
            _output_normals[j+13] = _output_normals[j];
            _output_normals[branch+6] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+6])*(_output_vertices[j]-_output_vertices[parent_branch+6]))+((_output_vertices[parent_branch+5]-_output_vertices[j])*(_output_vertices[parent_branch+6]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+5])*(_output_vertices[j+1]-_output_vertices[parent_branch+5]))+((_output_vertices[j-1]-_output_vertices[j-2])*(_output_vertices[j]-_output_vertices[j-2]))+((_output_vertices[j-3]-_output_vertices[j])*(_output_vertices[j-2]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[j-3])*(_output_vertices[j+1]-_output_vertices[j-3])) ).normalized();
            _output_normals[branch+5] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+5])*(_output_vertices[j]-_output_vertices[parent_branch+5]))+((_output_vertices[parent_branch+4]-_output_vertices[j])*(_output_vertices[parent_branch+5]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+4])*(_output_vertices[j+1]-_output_vertices[parent_branch+4]))+((_output_vertices[j-1]-_output_vertices[j-4])*(_output_vertices[j]-_output_vertices[j-4]))+((_output_vertices[j-5]-_output_vertices[j])*(_output_vertices[j-4]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[j-5])*(_output_vertices[j+1]-_output_vertices[j-5])) ).normalized();
            _output_normals[branch+4] = _output_normals[j];
            j+=2;
            _output_normals[j] = (((_output_vertices[j+11]-_output_vertices[parent_branch+8])*(_output_vertices[j]-_output_vertices[parent_branch+8]))+((_output_vertices[parent_branch+9]-_output_vertices[j])*(_output_vertices[parent_branch+8]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+9])*(_output_vertices[j+1]-_output_vertices[parent_branch+9]))+((_output_vertices[j]-_output_vertices[j+6])*(_output_vertices[j+11]-_output_vertices[j+6]))+((_output_vertices[j+6]-_output_vertices[j])*(_output_vertices[j+5]-_output_vertices[j]))+((_output_vertices[j+1]-_output_vertices[j+5])*(_output_vertices[j]-_output_vertices[j+5]))).normalized();
            _output_normals[branch+9] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+9])*(_output_vertices[j]-_output_vertices[parent_branch+9]))+((_output_vertices[parent_branch+10]-_output_vertices[j])*(_output_vertices[parent_branch+9]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+10])*(_output_vertices[j+1]-_output_vertices[parent_branch+10]))+((_output_vertices[j]-_output_vertices[j+4])*(_output_vertices[j-1]-_output_vertices[j+4]))+((_output_vertices[j+4]-_output_vertices[j])*(_output_vertices[j+3]-_output_vertices[j]))+((_output_vertices[j+1]-_output_vertices[j+3])*(_output_vertices[j]-_output_vertices[j+3])) ).normalized();
            _output_normals[branch+10] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+10])*(_output_vertices[j]-_output_vertices[parent_branch+10]))+((_output_vertices[parent_branch+11]-_output_vertices[j])*(_output_vertices[parent_branch+10]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+11])*(_output_vertices[j+1]-_output_vertices[parent_branch+11]))+((_output_vertices[j]-_output_vertices[j+2])*(_output_vertices[j-1]-_output_vertices[j+2]))+((_output_vertices[j+2]-_output_vertices[j])*(_output_vertices[j+1]-_output_vertices[j])) ).normalized();
            _output_normals[branch+11] = _output_normals[j];
            j+=8;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+6])*(_output_vertices[j]-_output_vertices[parent_branch+6]))+((_output_vertices[parent_branch+7]-_output_vertices[j])*(_output_vertices[parent_branch+6]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+7])*(_output_vertices[j+1]-_output_vertices[parent_branch+7]))+((_output_vertices[j]-_output_vertices[j-2])*(_output_vertices[j-1]-_output_vertices[j-2]))+((_output_vertices[j-2]-_output_vertices[j])*(_output_vertices[j-3]-_output_vertices[j]))+((_output_vertices[j+1]-_output_vertices[j-3])*(_output_vertices[j]-_output_vertices[j-3])) ).normalized();
            _output_normals[branch+7] = _output_normals[j];
            ++j;
            _output_normals[j] = ( ((_output_vertices[j-1]-_output_vertices[parent_branch+7])*(_output_vertices[j]-_output_vertices[parent_branch+7]))+((_output_vertices[parent_branch+8]-_output_vertices[j])*(_output_vertices[parent_branch+7]-_output_vertices[j]))+((_output_vertices[j]-_output_vertices[parent_branch+8])*(_output_vertices[j+1]-_output_vertices[parent_branch+8]))+((_output_vertices[j]-_output_vertices[j-4])*(_output_vertices[j-1]-_output_vertices[j-4]))+((_output_vertices[j-4]-_output_vertices[j])*(_output_vertices[j-5]-_output_vertices[j]))+((_output_vertices[j+1]-_output_vertices[j-5])*(_output_vertices[j]-_output_vertices[j-5])) ).normalized();
            _output_normals[branch+8] = _output_normals[j];
            ++j;

        }

        if (branch_type==2) {
            Vector3 C = ((_output_vertices[j]-_output_vertices[j+6])/2.0)+_output_vertices[j+6];

            for (int hi = 0; hi <= 12; ++hi){
                _output_normals.push_back((_output_vertices[j]-C).normalized());
                ++j;
            }
            int branch1_start = j-13;
            int parent_branch1_start = j-39;
            bool last_wise = true;
            if (current_branch_layer&1) { last_wise = false;}

            int returnval = 39;
             float rand_branch_type = -50.0 + (300 * ((float)random() / RAND_MAX));
             if (rand_branch_type<0){ returnval += _make_end(branch1_start, 0, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
             if ((rand_branch_type>=0)&&(rand_branch_type<50.0)) { returnval += _make_end(branch1_start, 1, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
             if ((rand_branch_type>=50.0)&&(rand_branch_type<100.0)) { returnval += _make_end(branch1_start, 2, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
             if (rand_branch_type>=100.0) { returnval += _make_end(branch1_start, 3, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
            return int(returnval);
            return int(39);

        }

        if ((branch_type==1)||(branch_type==3)) {
            _output_normals.push_back(_output_normals[branch+9]);
            _output_normals.push_back(_output_normals[branch+10]);
            _output_normals.push_back(_output_normals[branch+11]);
            _output_normals.push_back(_output_normals[branch]);
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back((Vector3(1.0,0.0,0.0)).normalized());
            _output_normals.push_back(_output_normals[branch+6]);
            _output_normals.push_back(_output_normals[branch+7]);
            _output_normals.push_back(_output_normals[branch+8]);
            _output_normals.push_back(_output_normals[branch+9]);
            Vector3 A = ((_output_vertices[j+6]-_output_vertices[j])/2.0)+_output_vertices[j];
            Vector3 C = ((_output_vertices[j+3]-_output_vertices[j+9])/2.0)+_output_vertices[j+9];
            float y_bend = sqrt((((A-C).x)*((A-C).x))+(((A-C).y)*((A-C).y))+(((A-C).z)*((A-C).z)))*0.3-(0.005*current_branch_layer*sqrt((((A-C).x)*((A-C).x))+(((A-C).y)*((A-C).y))+(((A-C).z)*((A-C).z))));
            A.y+=y_bend;
            N = (A-C).normalized();
            float NB = sqrt((N.x*N.x)+(N.y*N.y)+(N.z*N.z));
            Vector3 H = 1.15*(_output_vertices[j]-A);
            float branch_length = 0.18*float(current_branch_layer)*(0.9 + (1.6 * ((float)random() / RAND_MAX)))* sqrt((H.x*H.x)+(H.y*H.y)+(H.z*H.z));
            Vector3 B = ((N/NB)*branch_length) + A;

            for(int hi = 0; hi <= 12; ++hi)
            {
                float angle = hi * (2.0f*3.14159263 / 12.0f);
                Vector3 rotatedH = H.rotated(N, angle);
                _output_vertices.push_back(B + rotatedH);
                _output_colors.push_back(Color(0,0,0,0));
                _output_uv.push_back(Vector2((float(hi)*0.5),4.0));
            }

        }
            if (branch_type==1) {
                for(int hi = 0; hi < 12; ++hi)
                {
                    _output_indices.push_back(j);
                    _output_indices.push_back(j+13);
                    _output_indices.push_back(j+14);
                    _output_indices.push_back(j);
                    _output_indices.push_back(j+14);
                    _output_indices.push_back(j+1);
                    ++j;
                }

                ++j;

                _output_uv[j-23]=Vector2(4.0,0.0);
                _output_uv[j-24]=Vector2(4.5,0.0);
                _output_uv[j-25]=Vector2(5.0,0.0);
                _output_uv[j-26]=Vector2(5.5,0.0);
                _output_uv[j-14]=Vector2(5.5,0.0);
                _output_uv[j-15]=Vector2(6.0,0.0);
                _output_uv[j-16]=Vector2(6.5,0.0);
                _output_uv[j-17]=Vector2(7.0,0.0);
                _output_uv[j-22]=Vector2(4.0,1.5);
                _output_uv[j-21]=Vector2(4.75,1.9);
                _output_uv[j-20]=Vector2(5.5,2.0);
                _output_uv[j-19]=Vector2(6.25,1.9);
                _output_uv[j-18]=Vector2(7.0,1.5);

                Vector3 C = ((_output_vertices[j]-_output_vertices[j+6])/2.0)+_output_vertices[j+6];

                for (int hi = 0; hi <= 12; ++hi){
                    _output_normals.push_back((_output_vertices[j]-C).normalized());
                    ++j;
                }

                int branch1_start = j-13;
                int parent_branch1_start = j-26;
                bool last_wise = false;
                if (current_branch_layer&1) { last_wise = true;}

                int returnval = 39;
                 float rand_branch_type = -50.0 + (300 * ((float)random() / RAND_MAX));
                 if (rand_branch_type<0){ returnval += _make_end(branch1_start, 0, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
                 if ((rand_branch_type>=0)&&(rand_branch_type<50.0)) { returnval += _make_end(branch1_start, 1, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
                 if ((rand_branch_type>=50.0)&&(rand_branch_type<100.0)) { returnval += _make_end(branch1_start, 2, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
                 if (rand_branch_type>=100.0) { returnval += _make_end(branch1_start, 3, parent_branch1_start, last_wise, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
                return int(returnval);
            }
            if (branch_type==3) {
                for(int hi = 0; hi < 12; ++hi)
                {
                    _output_indices.push_back(j);
                    _output_indices.push_back(j+26);
                    _output_indices.push_back(j+27);
                    _output_indices.push_back(j);
                    _output_indices.push_back(j+27);
                    _output_indices.push_back(j+1);
                    ++j;
                }

                ++j;
                Vector3 C = ((_output_vertices[j]-_output_vertices[j+6])/2.0)+_output_vertices[j+6];

                for (int hi = 0; hi <= 12; ++hi){
                    _output_normals.push_back((_output_vertices[j]-C).normalized());
                    ++j;
                }
                C = ((_output_vertices[j]-_output_vertices[j+6])/2.0)+_output_vertices[j+6];

                for (int hi = 0; hi <= 12; ++hi){
                    _output_normals.push_back((_output_vertices[j]-C).normalized());
                    ++j;
                }

                int branch1_start = j-13;
                int branch2_start = j-26;
                int parent_branch1_start = j-52;
                int parent_branch2_start = j-39;

                bool last_wise1 = false;
                bool last_wise2 = true;
                if (current_branch_layer&1) { last_wise1 = true; last_wise2=false;}

                int returnval = 52;
                 float rand_branch_type = -50.0 + (300 * ((float)random() / RAND_MAX));
                 if (rand_branch_type<0){ returnval += _make_end(branch1_start, 0, parent_branch1_start, last_wise1, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
                 if ((rand_branch_type>=0)&&(rand_branch_type<50.0)) { returnval += _make_end(branch1_start, 1, parent_branch1_start, last_wise1, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
                 if ((rand_branch_type>=50.0)&&(rand_branch_type<100.0)) { returnval += _make_end(branch1_start, 2, parent_branch1_start, last_wise1, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
                 if (rand_branch_type>=100.0) { returnval += _make_end(branch1_start, 3, parent_branch1_start, last_wise1, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }

                  rand_branch_type = -50.0 + (300 * ((float)random() / RAND_MAX));
                  if (rand_branch_type<0){ returnval += _make_end(branch2_start, 0, parent_branch2_start, last_wise2, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
                  if ((rand_branch_type>=0)&&(rand_branch_type<50.0)) { returnval += _make_end(branch2_start, 1, parent_branch2_start, last_wise2, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
                  if ((rand_branch_type>=50.0)&&(rand_branch_type<100.0)) { returnval += _make_end(branch2_start, 2, parent_branch2_start, last_wise2, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
                  if (rand_branch_type>=100.0) { returnval += _make_end(branch2_start, 3, parent_branch2_start, last_wise2, new_branch_layer, max_branch_layer, branch_min_incline, branch_max_incline); }
                return int(returnval);

            }



    //((a+b+c+d)/4.0)+Plane(a,b,c).normal;
    //for(int i; i < 12; ++i)
    //{
    //    float angle = i * (2.0f*PI / 12.0f);
    //    Vector3 rotatedH = H.rotated(N, angle);
    //    Vector3 Bi = B + rotatedH;
    //}
    }

    if (branch_type==0){

        Vector3 C = ((_output_vertices[j-3]-_output_vertices[j-9])/2.0)+_output_vertices[j-9];
        Vector3 P_C = ((_output_vertices[parent_branch+3]-_output_vertices[parent_branch+9])/2.0)+_output_vertices[parent_branch+9];
        N = (C-P_C).normalized();
        //Vector3 N2 = (_output_vertices[branch]-_output_vertices[branch+6]).normalized();
        Vector3 N2 = (_output_vertices[j-9]-_output_vertices[j-3]);
        N2.y=0.0;
        N2 = N2.normalized();
        float NB = sqrt((N.x*N.x)+(N.y*N.y)+(N.z*N.z));
        float N2B = sqrt((N2.x*N2.x)+(N2.y*N2.y)+(N2.z*N2.z));
        Vector3 N3 = Vector3(0.0,-1.0,0.0).normalized();
        float N3B = sqrt((N3.x*N3.x)+(N3.y*N3.y)+(N3.z*N3.z));
        float branch_length = sqrt((((C-P_C).x)*((C-P_C).x))+(((C-P_C).y)*((C-P_C).y))+(((C-P_C).z)*((C-P_C).z)));
        float branch_girth = sqrt((((_output_vertices[branch]-_output_vertices[branch+6]).x)*((_output_vertices[branch]-_output_vertices[branch+6]).x))+(((_output_vertices[branch]-_output_vertices[branch+6]).y)*((_output_vertices[branch]-_output_vertices[branch+6]).y))+(((_output_vertices[branch]-_output_vertices[branch+6]).z)*((_output_vertices[branch]-_output_vertices[branch+6]).z)));
        float leaf_size = branch_length+branch_girth;
        _output_vertices.push_back(P_C);
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(0.0,0.0));

        _output_vertices.push_back(P_C + ((N2/N2B)*2.5*branch_girth)+((N3/N3B)*0.1*leaf_size));
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(1.0,0.0));

        _output_vertices.push_back(P_C - ((N2/N2B)*2.5*branch_girth)+((N3/N3B)*0.1*leaf_size));
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(1.0,0.0));


        _output_vertices.push_back(C);
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(0.0,4.0));

        _output_vertices.push_back(C + ((N2/N2B)*3.0*branch_girth)+((N3/N3B)*0.15*leaf_size));
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(1.0,4.0));

        _output_vertices.push_back(C-((N2/N2B)*3.0*branch_girth)+((N3/N3B)*0.15*leaf_size));
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(1.0,4.0));

        _output_vertices.push_back(((N/NB)*0.45*leaf_size) + C+((N3/N3B)*0.1*leaf_size));
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(0.0,9.0));

        _output_vertices.push_back(((N/NB)*0.45*leaf_size)+((N2/N2B)*2.5*branch_girth) + C+((N3/N3B)*0.25*leaf_size));
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(1.0,9.0));

        _output_vertices.push_back(((N/NB)*0.45*leaf_size)-((N2/N2B)*2.5*branch_girth) + C+((N3/N3B)*0.25*leaf_size));
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(1.0,9.0));

        _output_vertices.push_back(((N/NB)*0.9*leaf_size) + C+((N3/N3B)*0.25*leaf_size));
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(0.0,14.0));

        _output_vertices.push_back(((N/NB)*0.9*leaf_size)+((N2/N2B)*1.5*branch_girth) + C+((N3/N3B)*0.35*leaf_size));
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(1.0,14.0));

        _output_vertices.push_back(((N/NB)*0.9*leaf_size)-((N2/N2B)*1.5*branch_girth) + C+((N3/N3B)*0.35*leaf_size));
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(1.0,14.0));

        _output_vertices.push_back(_output_vertices[j+10]);
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(0.0,0.5));

        _output_vertices.push_back(_output_vertices[j+11]);
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(0.0,0.0));

        _output_vertices.push_back(_output_vertices[j+12]);
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(0.0,1.0));

        _output_vertices.push_back(((N/NB)*leaf_size) + C+((N3/N3B)*0.45*leaf_size));
        _output_colors.push_back(Color(1.0,0.0,0.0,0.0));
        _output_normals.push_back(Vector3(1.0,0.0,0.0).normalized());
        _output_uv.push_back(Vector2(1.2,0.5));


        _output_indices.push_back(j+1);
        _output_indices.push_back(j+4);
        _output_indices.push_back(j+3);

        _output_indices.push_back(j+6);
        _output_indices.push_back(j+3);
        _output_indices.push_back(j+4);

        _output_indices.push_back(j+2);
        _output_indices.push_back(j+5);
        _output_indices.push_back(j+1);

        _output_indices.push_back(j+4);
        _output_indices.push_back(j+1);
        _output_indices.push_back(j+5);

        _output_indices.push_back(j+4);
        _output_indices.push_back(j+7);
        _output_indices.push_back(j+6);

        _output_indices.push_back(j+9);
        _output_indices.push_back(j+6);
        _output_indices.push_back(j+7);

        _output_indices.push_back(j+5);
        _output_indices.push_back(j+8);
        _output_indices.push_back(j+4);

        _output_indices.push_back(j+7);
        _output_indices.push_back(j+4);
        _output_indices.push_back(j+8);

        _output_indices.push_back(j+7);
        _output_indices.push_back(j+10);
        _output_indices.push_back(j+9);

        _output_indices.push_back(j+12);
        _output_indices.push_back(j+9);
        _output_indices.push_back(j+10);

        _output_indices.push_back(j+8);
        _output_indices.push_back(j+11);
        _output_indices.push_back(j+7);

        _output_indices.push_back(j+10);
        _output_indices.push_back(j+7);
        _output_indices.push_back(j+11);

        _output_indices.push_back(j+15);
        _output_indices.push_back(j+13);
        _output_indices.push_back(j+16);

        _output_indices.push_back(j+13);
        _output_indices.push_back(j+14);
        _output_indices.push_back(j+16);

        //other side:


        _output_indices.push_back(j+1);
        _output_indices.push_back(j+3);
        _output_indices.push_back(j+4);

        _output_indices.push_back(j+6);
        _output_indices.push_back(j+4);
        _output_indices.push_back(j+3);

        _output_indices.push_back(j+2);
        _output_indices.push_back(j+1);
        _output_indices.push_back(j+5);

        _output_indices.push_back(j+4);
        _output_indices.push_back(j+5);
        _output_indices.push_back(j+1);

        _output_indices.push_back(j+4);
        _output_indices.push_back(j+6);
        _output_indices.push_back(j+7);

        _output_indices.push_back(j+9);
        _output_indices.push_back(j+7);
        _output_indices.push_back(j+6);

        _output_indices.push_back(j+5);
        _output_indices.push_back(j+4);
        _output_indices.push_back(j+8);

        _output_indices.push_back(j+7);
        _output_indices.push_back(j+8);
        _output_indices.push_back(j+4);

        _output_indices.push_back(j+7);
        _output_indices.push_back(j+9);
        _output_indices.push_back(j+10);

        _output_indices.push_back(j+12);
        _output_indices.push_back(j+10);
        _output_indices.push_back(j+9);

        _output_indices.push_back(j+8);
        _output_indices.push_back(j+7);
        _output_indices.push_back(j+11);

        _output_indices.push_back(j+10);
        _output_indices.push_back(j+11);
        _output_indices.push_back(j+7);

        _output_indices.push_back(j+15);
        _output_indices.push_back(j+16);
        _output_indices.push_back(j+13);

        _output_indices.push_back(j+13);
        _output_indices.push_back(j+16);
        _output_indices.push_back(j+14);

        return int(42);
    }
}

