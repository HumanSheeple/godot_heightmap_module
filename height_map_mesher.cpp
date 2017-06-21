#include "height_map_mesher.h"

// TODO Is this already in engine?
template <typename T>
void copy_to(PoolVector<T> &to, Vector<T> &from) {

    to.resize(from.size());

    typename PoolVector<T>::Write w = to.write();

    for (int i = 0; i < from.size(); ++i) {
        w[i] = from[i];
    }
}

Ref<Mesh> HeightMapMesher::make_chunk(Params params, const HeightMapData &data) {

    _output_vertices.clear();
    _output_normals.clear();
    _output_colors.clear();
    //	_output_uv.clear();
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

    Point2i terrain_size = data.size();

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
    for (pos.y = params.origin.y; pos.y <= max.y; pos.y += stride) {
        for (pos.x = params.origin.x; pos.x <= max.x; pos.x += stride) {

            int loc = data.heights.index(pos);

            _output_vertices.push_back(Vector3(
                    pos.x - params.origin.x,
                    data.heights[loc],
                    pos.y - params.origin.y));

            _output_colors.push_back(data.colors[loc]);

            _output_normals.push_back(data.normals[loc]);

            // Texture arrays
            //			_output_uv.push_back(Vector2(data.texture_indices[0][loc], data.texture_indices[1][loc]));
            //			_output_uv2.push_back(Vector2(data.texture_indices[2][loc], data.texture_indices[3][loc]));
            //			_output_colors.push_back(Color(data.texture_weights[0][loc], data.texture_weights[1][loc], data.texture_weights[2][loc], data.texture_weights[3][loc]));
            // Err... no more attributes??
        }
    }

    if (_output_vertices.size() == 0) {
        print_line("No vertices generated!");
        return Ref<Mesh>();
    }

    int i = params.size.x+3;
    for (pos.y = 1; pos.y < (params.size.y-1); ++pos.y) {
        for (pos.x = 1; pos.x < (params.size.x-1); ++pos.x) {
                _output_indices.push_back(i);
                _output_indices.push_back(i+params.size.x+1);
                _output_indices.push_back(i-1);
                _output_indices.push_back(i-1);
                _output_indices.push_back(i+params.size.x+1);
                _output_indices.push_back(i+params.size.x);
                ++i;
            }
        i=i+3;
        }

    if (params.seams[0]==0) {
        i = ((params.size.x+1)*(params.size.y+1))-params.size.y;
        _output_indices.push_back(i);
        _output_indices.push_back(i-1);
        _output_indices.push_back(i-params.size.x-1);
        i++;
        for (pos.x = 1; pos.x < (params.size.x-1); ++pos.x) {
            _output_indices.push_back(i);
            _output_indices.push_back(i-params.size.x-2);
            _output_indices.push_back(i-params.size.x-1);
            _output_indices.push_back(i);
            _output_indices.push_back(i-1);
            _output_indices.push_back(i-params.size.x-2);
            i++;
        }
        _output_indices.push_back(i);
        _output_indices.push_back(i-1);
        _output_indices.push_back(i-params.size.x-2);
    }

    if (params.seams[0]==1) {
        i = ((params.size.x+1)*(params.size.y+1))-params.size.y+1;
        _output_indices.push_back(i);
        _output_indices.push_back(i-2);
        _output_indices.push_back(i-params.size.x-2);
        for (pos.x = 1; pos.x < (params.size.x-1); pos.x+=2) {
            _output_indices.push_back(i);
            _output_indices.push_back(i-params.size.x-2);
            _output_indices.push_back(i-params.size.x-1);
            _output_indices.push_back(i);
            _output_indices.push_back(i-params.size.x-1);
            _output_indices.push_back(i-params.size.x);
            i+=2;
            _output_indices.push_back(i);
            _output_indices.push_back(i-2);
            _output_indices.push_back(i-params.size.x-2);
        }
    }

    if (params.seams[1]==0) {
        i = params.size.x;
        _output_indices.push_back(i);
        _output_indices.push_back(i+params.size.x+1);
        _output_indices.push_back(i+params.size.x);
        i += 2*(params.size.x+1);
        for (pos.x = 1; pos.x < (params.size.x-1); ++pos.x) {
            _output_indices.push_back(i);
            _output_indices.push_back(i-1);
            _output_indices.push_back(i-params.size.x-2);
            _output_indices.push_back(i);
            _output_indices.push_back(i-params.size.x-2);
            _output_indices.push_back(i-params.size.x-1);
            i+=(params.size.x+1);
        }
        _output_indices.push_back(i);
        _output_indices.push_back(i-params.size.x-2);
        _output_indices.push_back(i-params.size.x-1);
    }

    if (params.seams[1]==1) {
        i = params.size.x;
        _output_indices.push_back(i);
        _output_indices.push_back(i+(2*(params.size.x+1)));
        _output_indices.push_back(i+params.size.x);
        i+=2*(params.size.x+1);
        for (pos.x = 1; pos.x < (params.size.x-1); pos.x+=2) {
            _output_indices.push_back(i);
            _output_indices.push_back(i+(2*(params.size.x+1)));
            _output_indices.push_back(i+params.size.x);
            _output_indices.push_back(i);
            _output_indices.push_back(i-1);
            _output_indices.push_back(i-params.size.x-2);
            _output_indices.push_back(i);
            _output_indices.push_back(i+params.size.x);
            _output_indices.push_back(i-1);
            i+=2*(params.size.x+1);

        }
    }
        if (params.seams[2]==0) {
            i = 1;
            _output_indices.push_back(i);
            _output_indices.push_back(i+params.size.x+1);
            _output_indices.push_back(i-1);
            i++;
            for (pos.x = 1; pos.x < (params.size.x-1); ++pos.x) {
                _output_indices.push_back(i);
                _output_indices.push_back(i+params.size.x+1);
                _output_indices.push_back(i-1);
                _output_indices.push_back(i-1);
                _output_indices.push_back(i+params.size.x+1);
                _output_indices.push_back(i+params.size.x);
                i++;
            }
            _output_indices.push_back(i);
            _output_indices.push_back(i+params.size.x);
            _output_indices.push_back(i-1);
        }

        if (params.seams[2]==1) {
            i = 2;
            for (pos.x = 1; pos.x < (params.size.x-2); pos.x+=2) {
                _output_indices.push_back(i);
                _output_indices.push_back(i+params.size.x);
                _output_indices.push_back(i-2);
                _output_indices.push_back(i);
                _output_indices.push_back(i+params.size.x+1);
                _output_indices.push_back(i+params.size.x);
                _output_indices.push_back(i);
                _output_indices.push_back(i+params.size.x+2);
                _output_indices.push_back(i+params.size.x+1);
                i+=2;
            }
            _output_indices.push_back(i);
            _output_indices.push_back(i+params.size.x);
            _output_indices.push_back(i-2);
        }

        if (params.seams[3]==0) {
            i = params.size.x+1;
            _output_indices.push_back(i);
            _output_indices.push_back(0);
            _output_indices.push_back(i+1);
            i += (params.size.x+1);
            for (pos.x = 1; pos.x < (params.size.x-1); ++pos.x) {
                _output_indices.push_back(i);
                _output_indices.push_back((i-params.size.x)-1);
                _output_indices.push_back(i+1);
                _output_indices.push_back(i+1);
                _output_indices.push_back((i-params.size.x)-1);
                _output_indices.push_back(i-params.size.x);
                i+=(params.size.x+1);
            }
            _output_indices.push_back(i);
            _output_indices.push_back((i-params.size.x)-1);
            _output_indices.push_back(i-params.size.x);
        }

        if (params.seams[3]==1) {
            i = 2*(params.size.x+1);

            for (pos.x = 1; pos.x < (params.size.x-1); pos.x+=2) {
                _output_indices.push_back(i);
                _output_indices.push_back(i-(2*(params.size.x+1)));
                _output_indices.push_back(i-params.size.x);
                _output_indices.push_back(i);
                _output_indices.push_back((i-params.size.x));
                _output_indices.push_back(i+1);
                _output_indices.push_back(i);
                _output_indices.push_back(i+1);
                _output_indices.push_back((i+params.size.x)+2);
                i+=2*(params.size.x+1);
            }
            _output_indices.push_back(i);
            _output_indices.push_back(i-(2*(params.size.x+1)));
            _output_indices.push_back(i-params.size.x);
        }
        //draw decels
        i = (params.size.x+1)*(params.size.y+1);
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
                    _output_colors.push_back(Color(0,0.3,0,1));
                    _output_normals.push_back(data.normals[loc]);
                    _output_vertices.push_back(Vector3(
                            float(decel_x1 + 0.005),
                            float(decel_offset_z-0.1),
                            float(decel_y1 - 0.005)));
                    _output_colors.push_back(Color(0,0.4,0,1));
                    _output_normals.push_back(data.normals[loc]);
                    _output_vertices.push_back(Vector3(
                            float(decel_x1 + 0.005),
                            float(decel_offset_z-0.1),
                            float(decel_y1 + 0.005)));
                    _output_colors.push_back(Color(0.3,0.5,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
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
                    _output_colors.push_back(Color(0,0.3,0,1));
                    _output_normals.push_back(data.normals[loc]);
                    _output_vertices.push_back(Vector3(
                            float(decel_x1 + 0.002),
                            float(decel_offset_z),
                            float(decel_y1 - 0.002)));
                    _output_colors.push_back(Color(0,0.4,0,1));
                    _output_normals.push_back(data.normals[loc]);
                    _output_vertices.push_back(Vector3(
                            float(decel_x1 + 0.002),
                            float(decel_offset_z),
                            float(decel_y1 + 0.002)));
                    _output_colors.push_back(Color(0.3,0.5,0.05,1));
                    _output_normals.push_back(data.normals[loc]);

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
                    _output_colors.push_back(Color(0.7,0.7,0.15,1));
                    _output_normals.push_back(data.normals[loc]);
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
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.002),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 - 0.002)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.002),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 + 0.002)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        //first petal underside
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.038),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 - 0.024)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.054),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.003),
                                float(decel_y1)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.038),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 + 0.018)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);

                        //yellow core

                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.002),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 - 0.002)));
                        _output_colors.push_back(Color(1,1,0,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.002),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 - 0.002)));
                        _output_colors.push_back(Color(1,1,0,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.002),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 + 0.002)));
                        _output_colors.push_back(Color(1,1,0,1));
                        _output_normals.push_back(data.normals[loc]);

                        // first petal topside
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.038),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 - 0.024)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.054),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.002),
                                float(decel_y1)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.038),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 + 0.018)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);

                        // second petal underside

                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.035),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.043),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.003),
                                float(decel_y1 +0.028)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.012),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 +0.03)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);

                        // second petal topside
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.035),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.043),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.002),
                                float(decel_y1 +0.028)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.012),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 + 0.03)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);

                        // third petal underside

                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.02),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 - 0.035)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.012),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.003),
                                float(decel_y1 - 0.052)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.024),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(decel_y1 - 0.03)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);

                        // third petal topside

                        _output_vertices.push_back(Vector3(
                                float(decel_x1 + 0.02),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 - 0.035)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.012),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.002),
                                float(decel_y1 - 0.052)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(decel_x1 - 0.024),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(decel_y1 - 0.03)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);

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
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.5 + decel_offset_x),
                                    float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-((decel_offset_x+0.5)*data.normals[loc].x)-0.1),
                                    float(pos.y - params.origin.y + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.5 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.5)*data.normals[loc].z)-((decel_offset_x+0.5)*data.normals[loc].x)-0.1),
                                    float(pos.y - params.origin.y + 0.5 + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.5)*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.1),
                                    float(pos.y - params.origin.y + 0.5 + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);

                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + decel_offset_x),
                                    float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.5 + decel_offset_x),
                                    float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-((decel_offset_x+0.5)*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.5 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.5)*data.normals[loc].z)-((decel_offset_x+0.5)*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + 0.5 + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.5)*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + 0.5 + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);

                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.01 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.01)*data.normals[loc].z)-((decel_offset_x+0.01)*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + 0.01 + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.49 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.01)*data.normals[loc].z)-((decel_offset_x+0.49)*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + 0.01 + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.49 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.49)*data.normals[loc].z)-((decel_offset_x+0.49)*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + 0.49 + decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.01 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.49)*data.normals[loc].z)-((decel_offset_x+0.01)*data.normals[loc].x)+0.05),
                                    float(pos.y - params.origin.y + 0.49+ decel_offset_y)));
                            _output_colors.push_back(Color(0.1,0.1,0.1,1));
                            _output_normals.push_back(data.normals[loc]);

                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.01 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.01)*data.normals[loc].z)-((decel_offset_x+0.01)*data.normals[loc].x)+0.08),
                                    float(pos.y - params.origin.y + 0.01 + decel_offset_y)));
                            _output_colors.push_back(Color(0.7,0.7,0.7,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.49 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.01)*data.normals[loc].z)-((decel_offset_x+0.49)*data.normals[loc].x)+0.08),
                                    float(pos.y - params.origin.y + 0.01 + decel_offset_y)));
                            _output_colors.push_back(Color(0.7,0.7,0.7,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.49 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.49)*data.normals[loc].z)-((decel_offset_x+0.49)*data.normals[loc].x)+0.08),
                                    float(pos.y - params.origin.y + 0.49 + decel_offset_y)));
                            _output_colors.push_back(Color(0.7,0.7,0.7,1));
                            _output_normals.push_back(data.normals[loc]);
                            _output_vertices.push_back(Vector3(
                                    float(pos.x - params.origin.x + 0.01 + decel_offset_x),
                                    float(data.heights[loc]-((decel_offset_y+0.49)*data.normals[loc].z)-((decel_offset_x+0.01)*data.normals[loc].x)+0.08),
                                    float(pos.y - params.origin.y + 0.49+ decel_offset_y)));
                            _output_colors.push_back(Color(0.7,0.7,0.7,1));
                            _output_normals.push_back(data.normals[loc]);

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
                    decel_offset_y = -1.2 + (2.41 * ((float)random() / RAND_MAX));
                    decel_offset_x = -1.2 + (2.41 * ((float)random() / RAND_MAX));
                    decel_scale = 0.2 + (0.91 * ((float)random() / RAND_MAX));
                    //0
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.844*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //1
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.924*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y + (0.383*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //2
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.597*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y + (0.597*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //3
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.383*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y + (0.924*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //4
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y + (0.844*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //5
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.383*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y + (0.924*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //6
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.597*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y + (0.597*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //7
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.923*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y + (0.383*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //8
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.844*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //9
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.924*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y - (0.383*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //10
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.597*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y - (0.597*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //11
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.383*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y - (0.924*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //12
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y - (0.844*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //13
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.383*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y - (0.924*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //14
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.597*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y - (0.597*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //15
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.924*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)-0.2),
                            float(pos.y - params.origin.y - (0.383*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //16 tree base above ground
                    height_offset = 0.4 + (0.8 * ((float)random() / RAND_MAX));
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.839*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.148*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //17
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.718*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.458*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //18
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.488*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.698*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //19
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.184*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.831*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //20
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.148*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.839*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //21
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.458*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.718*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //22
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.698*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.488*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //23
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.831*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.184*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //24
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.838*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.148*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //25
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.718*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.458*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //26
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.488*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.698*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //27
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.184*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.831*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //28
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.148*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.839*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //29
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.458*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.718*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //30
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.698*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.488*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //31
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.831*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.184*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    decel_offset_y+= -0.32 + (0.641 * ((float)random() / RAND_MAX));
                    decel_offset_x+= -0.32 + (0.641 * ((float)random() / RAND_MAX));
                    if (decel_scale < 0.4) { height_offset+=1.3; }
                    height_offset+= 2.4 + (decel_scale*(2.4 * ((float)random() / RAND_MAX)));
                    //32
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.711*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //33
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.657*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.272*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //34
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.503*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.503*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //35
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.272*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.657*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //36
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.711*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //37
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.272*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.657*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //38
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.503*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.503*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //39
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.657*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + (0.272*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //40
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.711*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //41
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.657*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.272*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //42
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.503*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.503*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //43
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.272*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.657*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //44
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.711*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //45
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.272*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.657*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //46
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.503*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.503*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //47
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.657*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset),
                            float(pos.y - params.origin.y - (0.272*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //48 bridge between first branching
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.503*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.629*decel_scale)),
                            float(pos.y - params.origin.y + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //49
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.272*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.814*decel_scale)),
                            float(pos.y - params.origin.y + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //50
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.88*decel_scale)),
                            float(pos.y - params.origin.y + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //51
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.272*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.814*decel_scale)),
                            float(pos.y - params.origin.y + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //52
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.503*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.629*decel_scale)),
                            float(pos.y - params.origin.y + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //53 First Branch First Node
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.04*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.627*decel_scale)),
                            float(pos.y - params.origin.y + (1.78*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //54
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.229*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.685*decel_scale)),
                            float(pos.y - params.origin.y + (1.743*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //55
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.422*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.844*decel_scale)),
                            float(pos.y - params.origin.y + (1.632*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //56
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.493*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(1.06*decel_scale)),
                            float(pos.y - params.origin.y + (1.48*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //57
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.422*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(1.276*decel_scale)),
                            float(pos.y - params.origin.y + (1.33*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //58
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (0.223*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(1.435*decel_scale)),
                            float(pos.y - params.origin.y + (1.218*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //59
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.035*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(1.493*decel_scale)),
                            float(pos.y - params.origin.y + (1.178*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //60
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (-0.299*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(1.435*decel_scale)),
                            float(pos.y - params.origin.y + (1.218*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //61
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.493*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(1.276*decel_scale)),
                            float(pos.y - params.origin.y + (1.329*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //62
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (-0.563*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(1.06*decel_scale)),
                            float(pos.y - params.origin.y + (1.481*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);
                    //63
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - (0.493*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.844*decel_scale)),
                            float(pos.y - params.origin.y + (1.632*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(1,1,0.6,1));
                    _output_normals.push_back(data.normals[loc]);
                    //64
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + (-0.299*decel_scale) + decel_offset_x),
                            float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.685*decel_scale)),
                            float(pos.y - params.origin.y + (1.743*decel_scale) + decel_offset_y)));
                    _output_colors.push_back(Color(0.3,0.3,0.05,1));
                    _output_normals.push_back(data.normals[loc]);

                    int c1 = 0;
                    int c2 = 0;
                    for (c2 = 0; c2 < 2; ++c2){
                    for (c1 = 0; c1 < 15; ++c1){
                    _output_indices.push_back(i+17);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+16);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+17);
                    _output_indices.push_back(i+1);
                    ++i;
                    }
                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+16);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i-15);
                    ++i;
                    }
                    // build joint between trunk and first branch
                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+16);
                    ++i;
                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+15);
                    ++i;
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+14);
                    _output_indices.push_back(i+28);
                    _output_indices.push_back(i+29);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+28);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+29);
                    _output_indices.push_back(i+30);
                    _output_indices.push_back(i+30);
                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i);
                    ++i;
                    _output_indices.push_back(i+18);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+29);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+18);
                    _output_indices.push_back(i+1);
                    ++i;
                    for (c2 = 0; c2 < 2; ++c2){
                    _output_indices.push_back(i+18);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+17);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+18);
                    _output_indices.push_back(i+1);
                    ++i;
                    }
                    _output_indices.push_back(i+17);
                    _output_indices.push_back(i+14);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+14);
                    _output_indices.push_back(i+17);
                    _output_indices.push_back(i+18);
                    for (c2 = 0; c2 < 4; ++c2){
                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+(14-c2));
                    ++i;
                    }
                    i+=4;
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+2);
                    _output_indices.push_back(i+1);
                    ++i;
                    _output_indices.push_back(i-15);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+1);
                    ++i;
                    //triangulate up the top of the first branch
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+13);
                    _output_indices.push_back(i+14);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i+13);
                    ++i;
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+11);
                    _output_indices.push_back(i+12);
                    for (c1 = 0; c1 < 4; c1+=2){
                    _output_indices.push_back(i+11-c1);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+10-c1);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i+10-c1);
                    ++i;
                    }
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+6);
                    _output_indices.push_back(i+7);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+5);
                    _output_indices.push_back(i+6);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i+5);

                    i-= 9;

                    float branches = -100.0 + (200 * ((float)random() / RAND_MAX));

                                       // Build second branch if there is one
                                       if (branches>0) {
                                           //65 First Branch Second Node
                                           _output_vertices.push_back(Vector3(
                                                   float(pos.x - params.origin.x - (0.04*decel_scale) + decel_offset_x),
                                                   float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.627*decel_scale)),
                                                   float(pos.y - params.origin.y - (1.78*decel_scale) + decel_offset_y)));
                                           _output_colors.push_back(Color(1,1,0.6,1));
                                           _output_normals.push_back(data.normals[loc]);
                                           //66
                                           _output_vertices.push_back(Vector3(
                                                   float(pos.x - params.origin.x + (0.229*decel_scale) + decel_offset_x),
                                                   float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.685*decel_scale)),
                                                   float(pos.y - params.origin.y - (1.743*decel_scale) + decel_offset_y)));
                                           _output_colors.push_back(Color(0.3,0.3,0.05,1));
                                           _output_normals.push_back(data.normals[loc]);
                                           //67
                                           _output_vertices.push_back(Vector3(
                                                   float(pos.x - params.origin.x + (0.422*decel_scale) + decel_offset_x),
                                                   float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.844*decel_scale)),
                                                   float(pos.y - params.origin.y - (1.632*decel_scale) + decel_offset_y)));
                                           _output_colors.push_back(Color(1,1,0.6,1));
                                           _output_normals.push_back(data.normals[loc]);
                                           //68
                                           _output_vertices.push_back(Vector3(
                                                   float(pos.x - params.origin.x + (0.493*decel_scale) + decel_offset_x),
                                                   float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(1.06*decel_scale)),
                                                   float(pos.y - params.origin.y - (1.48*decel_scale) + decel_offset_y)));
                                           _output_colors.push_back(Color(0.3,0.3,0.05,1));
                                           _output_normals.push_back(data.normals[loc]);
                                           //69
                                           _output_vertices.push_back(Vector3(
                                                   float(pos.x - params.origin.x + (0.422*decel_scale) + decel_offset_x),
                                                   float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(1.276*decel_scale)),
                                                   float(pos.y - params.origin.y - (1.33*decel_scale) + decel_offset_y)));
                                           _output_colors.push_back(Color(1,1,0.6,1));
                                           _output_normals.push_back(data.normals[loc]);
                                           //70
                                           _output_vertices.push_back(Vector3(
                                                   float(pos.x - params.origin.x + (0.223*decel_scale) + decel_offset_x),
                                                   float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(1.435*decel_scale)),
                                                   float(pos.y - params.origin.y - (1.218*decel_scale) + decel_offset_y)));
                                           _output_colors.push_back(Color(0.3,0.3,0.05,1));
                                           _output_normals.push_back(data.normals[loc]);
                                           //71
                                           _output_vertices.push_back(Vector3(
                                                   float(pos.x - params.origin.x - (0.035*decel_scale) + decel_offset_x),
                                                   float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(1.493*decel_scale)),
                                                   float(pos.y - params.origin.y - (1.178*decel_scale) + decel_offset_y)));
                                           _output_colors.push_back(Color(1,1,0.6,1));
                                           _output_normals.push_back(data.normals[loc]);
                                           //72
                                           _output_vertices.push_back(Vector3(
                                                   float(pos.x - params.origin.x + (-0.299*decel_scale) + decel_offset_x),
                                                   float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(1.435*decel_scale)),
                                                   float(pos.y - params.origin.y - (1.218*decel_scale) + decel_offset_y)));
                                           _output_colors.push_back(Color(0.3,0.3,0.05,1));
                                           _output_normals.push_back(data.normals[loc]);
                                           //73
                                           _output_vertices.push_back(Vector3(
                                                   float(pos.x - params.origin.x - (0.493*decel_scale) + decel_offset_x),
                                                   float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(1.276*decel_scale)),
                                                   float(pos.y - params.origin.y - (1.329*decel_scale) + decel_offset_y)));
                                           _output_colors.push_back(Color(1,1,0.6,1));
                                           _output_normals.push_back(data.normals[loc]);
                                           //74
                                           _output_vertices.push_back(Vector3(
                                                   float(pos.x - params.origin.x + (-0.563*decel_scale) + decel_offset_x),
                                                   float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(1.06*decel_scale)),
                                                   float(pos.y - params.origin.y - (1.481*decel_scale) + decel_offset_y)));
                                           _output_colors.push_back(Color(0.3,0.3,0.05,1));
                                           _output_normals.push_back(data.normals[loc]);
                                           //75
                                           _output_vertices.push_back(Vector3(
                                                   float(pos.x - params.origin.x - (0.493*decel_scale) + decel_offset_x),
                                                   float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.844*decel_scale)),
                                                   float(pos.y - params.origin.y - (1.632*decel_scale) + decel_offset_y)));
                                           _output_colors.push_back(Color(1,1,0.6,1));
                                           _output_normals.push_back(data.normals[loc]);
                                           //76
                                           _output_vertices.push_back(Vector3(
                                                   float(pos.x - params.origin.x + (-0.299*decel_scale) + decel_offset_x),
                                                   float(data.heights[loc]-(decel_offset_y*data.normals[loc].z)-(decel_offset_x*data.normals[loc].x)+height_offset+(0.685*decel_scale)),
                                                   float(pos.y - params.origin.y - (1.743*decel_scale) + decel_offset_y)));
                                           _output_colors.push_back(Color(0.3,0.3,0.05,1));
                                           _output_normals.push_back(data.normals[loc]);

                                           _output_indices.push_back(i);
                                           _output_indices.push_back(i+26);
                                           _output_indices.push_back(i+25);
                                           _output_indices.push_back(i);
                                           _output_indices.push_back(i+10);
                                           _output_indices.push_back(i+26);
                                           for (c2 = 25; c2 > 21; c2-=2) {
                                               _output_indices.push_back(i);
                                               _output_indices.push_back(i+c2);
                                               _output_indices.push_back(i+(c2-1));
                                               _output_indices.push_back(i+1);
                                               _output_indices.push_back(i);
                                               _output_indices.push_back(i+(c2-1));
                                               ++i;
                                           }
                                           _output_indices.push_back(i);
                                           _output_indices.push_back(i+21);
                                           _output_indices.push_back(i+32);
                                           _output_indices.push_back(i+1);
                                           _output_indices.push_back(i);
                                           _output_indices.push_back(i+32);
                                           ++i;
                                           _output_indices.push_back(i);
                                           _output_indices.push_back(i+31);
                                           _output_indices.push_back(i+30);
                                           _output_indices.push_back(i+1);
                                           _output_indices.push_back(i);
                                           _output_indices.push_back(i+30);
                                           ++i;
                                           _output_indices.push_back(i);
                                           _output_indices.push_back(i+29);
                                           _output_indices.push_back(i+2);
                                           _output_indices.push_back(i+2);
                                           _output_indices.push_back(i+29);
                                           _output_indices.push_back(i+28);
                                            i+=2;
                                            //top side of first node second branch
                                            _output_indices.push_back(i);
                                            _output_indices.push_back(i+26);
                                            _output_indices.push_back(i+25);
                                            _output_indices.push_back(i+1);
                                            _output_indices.push_back(i);
                                            _output_indices.push_back(i+25);
                                            ++i;
                                            _output_indices.push_back(i+23);
                                            _output_indices.push_back(i);
                                            _output_indices.push_back(i+24);

                                            for (c2 = 22; c2 > 18; c2-=2) {
                                                _output_indices.push_back(i);
                                                _output_indices.push_back(i+(c2+1));
                                                _output_indices.push_back(i+c2);
                                                _output_indices.push_back(i+1);
                                                _output_indices.push_back(i);
                                                _output_indices.push_back(i+(c2));
                                                ++i;
                                            }
                                            _output_indices.push_back(i);
                                            _output_indices.push_back(i+19);
                                            _output_indices.push_back(i+18);
                                            _output_indices.push_back(i);
                                            _output_indices.push_back(i+18);
                                            _output_indices.push_back(i+17);
                                            _output_indices.push_back(i+1);
                                            _output_indices.push_back(i);
                                            _output_indices.push_back(i+17);
                                           i+=26;
                                       }
                    // Seal the gap if there is no branch
                        if(branches<0){
                        for (c2 = 10; c2 > 2; c2-=2) {
                            _output_indices.push_back(i);
                            _output_indices.push_back(i+c2);
                            _output_indices.push_back(i+(c2-1));
                            _output_indices.push_back(i+1);
                            _output_indices.push_back(i);
                            _output_indices.push_back(i+(c2-1));
                            ++i;
                        }
                        i+=19;
                        }

                    }
                }
            }

        }



    PoolVector<Vector3> pool_vertices;
    PoolVector<Vector3> pool_normals;
    PoolVector<Color> pool_colors;
    PoolVector<int> pool_indices;

    copy_to(pool_vertices, _output_vertices);
    copy_to(pool_normals, _output_normals);
    copy_to(pool_colors, _output_colors);
    copy_to(pool_indices, _output_indices);

    Array arrays;
    arrays.resize(Mesh::ARRAY_MAX);
    arrays[Mesh::ARRAY_VERTEX] = pool_vertices;
    arrays[Mesh::ARRAY_NORMAL] = pool_normals;
    arrays[Mesh::ARRAY_COLOR] = pool_colors;
    arrays[Mesh::ARRAY_INDEX] = pool_indices;

    Ref<ArrayMesh> mesh_ref(memnew(ArrayMesh));
    mesh_ref->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);

    return mesh_ref;
}
