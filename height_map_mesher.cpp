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
        //draw grass
        i = (params.size.x+1)*(params.size.y+1);
        float decel_offset_x = -0.5;
        float decel_offset_y = -0.5;
        float height_offset = 0.1;
        for (pos.y = params.origin.y; pos.y <= max.y; pos.y += stride) {
            for (pos.x = params.origin.x; pos.x <= max.x; pos.x += stride) {
                int loc = data.heights.index(pos);
                if ((data.colors[loc].g > 0.8)&&(data.colors[loc].r < 0.2)&&(data.colors[loc].b < 0.2)&&(params.lod==0)) {
                    for (decel_offset_y = -0.5; decel_offset_y < 0.5; decel_offset_y+=0.1){
                    for (decel_offset_x = -0.5; decel_offset_x < 0.5; decel_offset_x+=0.1){
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - 0.002 + decel_offset_x),
                            float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)-0.1),
                            float(pos.y - params.origin.y - 0.002 + decel_offset_y)));
                    _output_colors.push_back(Color(0,0.4,0,1));
                    _output_normals.push_back(data.normals[loc]);
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + 0.002 + decel_offset_x),
                            float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)-0.1),
                            float(pos.y - params.origin.y - 0.002 + decel_offset_y)));
                    _output_colors.push_back(Color(0,0.4,0,1));
                    _output_normals.push_back(data.normals[loc]);
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + 0.002 + decel_offset_x),
                            float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)-0.1),
                            float(pos.y - params.origin.y + 0.002 + decel_offset_y)));
                    _output_colors.push_back(Color(0,0.4,0,1));
                    _output_normals.push_back(data.normals[loc]);
                    height_offset = 0.06 + (0.07 * ((float)random() / RAND_MAX));
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x - 0.002 + decel_offset_x),
                            float(data.heights[loc]+height_offset+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)),
                            float(pos.y - params.origin.y - 0.002 + decel_offset_y)));
                    _output_colors.push_back(Color(0,0.4,0,1));
                    _output_normals.push_back(data.normals[loc]);
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + 0.002 + decel_offset_x),
                            float(data.heights[loc]+height_offset+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)),
                            float(pos.y - params.origin.y - 0.002 + decel_offset_y)));
                    _output_colors.push_back(Color(0,0.4,0,1));
                    _output_normals.push_back(data.normals[loc]);
                    _output_vertices.push_back(Vector3(
                            float(pos.x - params.origin.x + 0.002 + decel_offset_x),
                            float(data.heights[loc]+height_offset+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)),
                            float(pos.y - params.origin.y + 0.002 + decel_offset_y)));
                    _output_colors.push_back(Color(0,0.4,0,1));
                    _output_normals.push_back(data.normals[loc]);


                    _output_indices.push_back(i);
                    _output_indices.push_back(i+4);
                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i+4);
                    _output_indices.push_back(i);
                    _output_indices.push_back(i+3);

                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i+5);
                    _output_indices.push_back(i+4);
                    _output_indices.push_back(i+5);
                    _output_indices.push_back(i+1);
                    _output_indices.push_back(i+2);

                    _output_indices.push_back(i);
                    _output_indices.push_back(i+5);
                    _output_indices.push_back(i+3);
                    _output_indices.push_back(i+2);
                    _output_indices.push_back(i+5);
                    _output_indices.push_back(i);

                    _output_indices.push_back(i+3);
                    _output_indices.push_back(i+4);
                    _output_indices.push_back(i+5);

                    i+=6;
                    // daisies
                    if ((height_offset>0.12)&&(data.colors[loc].a>0.8)) {
                        //bottom of daisy head
                        height_offset += (0.05 * ((float)random() / RAND_MAX));
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x - 0.002 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(pos.y - params.origin.y - 0.002 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x + 0.002 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(pos.y - params.origin.y - 0.002 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x + 0.002 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(pos.y - params.origin.y + 0.002 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        //first petal underside
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x + 0.038 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(pos.y - params.origin.y - 0.024 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x + 0.054 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.003),
                                float(pos.y - params.origin.y + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x + 0.038 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(pos.y - params.origin.y + 0.018 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);

                        //yellow core

                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x - 0.002 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(pos.y - params.origin.y - 0.002 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,0,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x + 0.002 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(pos.y - params.origin.y - 0.002 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,0,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x + 0.002 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(pos.y - params.origin.y + 0.002 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,0,1));
                        _output_normals.push_back(data.normals[loc]);

                        // first petal topside
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x + 0.038 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(pos.y - params.origin.y - 0.024 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x + 0.054 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.002),
                                float(pos.y - params.origin.y + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x + 0.038 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(pos.y - params.origin.y + 0.018 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);

                        // second petal underside

                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x - 0.035 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(pos.y - params.origin.y + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x - 0.043 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.003),
                                float(pos.y - params.origin.y +0.028 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x - 0.012 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(pos.y - params.origin.y +0.03 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);

                        // second petal topside
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x - 0.035 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(pos.y - params.origin.y + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x - 0.043 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.002),
                                float(pos.y - params.origin.y +0.028 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x - 0.012 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(pos.y - params.origin.y +0.03+ decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);

                        // third petal underside

                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x + 0.02 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(pos.y - params.origin.y - 0.035 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x - 0.012 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.003),
                                float(pos.y - params.origin.y - 0.052 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x - 0.024 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.001),
                                float(pos.y - params.origin.y - 0.03 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);

                        // third petal topside

                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x + 0.02 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(pos.y - params.origin.y - 0.035 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x - 0.012 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset-0.002),
                                float(pos.y - params.origin.y - 0.052 + decel_offset_y)));
                        _output_colors.push_back(Color(1,1,1,1));
                        _output_normals.push_back(data.normals[loc]);
                        _output_vertices.push_back(Vector3(
                                float(pos.x - params.origin.x - 0.024 + decel_offset_x),
                                float(data.heights[loc]+((-1)*decel_offset_y*data.normals[loc].z)+((-1)*decel_offset_x*data.normals[loc].x)+height_offset+0.002),
                                float(pos.y - params.origin.y - 0.03 + decel_offset_y)));
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
