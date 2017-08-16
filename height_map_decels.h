#ifndef HEIGHT_MAP_DECELS_H
#define HEIGHT_MAP_DECELS_H

#include <math/math_2d.h>
#include <scene/resources/mesh.h>

#include "height_map_data.h"

class HeightMapDecels {

public:
	struct Params {
		Point2i origin;
		Point2i size;
        bool decels_enabled;
		bool smooth;
		int lod;

		Params() {
			smooth = true;
			lod = 0;
            decels_enabled = false;

		}
	};

	Ref<Mesh> make_decels(Params params, const HeightMapData &data);
    int _make_end(int branch, int branch_type, int parent_branch, bool clock_wise, int current_branch_layer, int max_branch_layer, float branch_min_incline, float branch_max_incline);
private:
    union _Seed {uint64_t _Long; uint8_t _Short[8]; };
	Vector<Vector3> _output_vertices;
	Vector<Vector3> _output_normals;
	Vector<Color> _output_colors;
    Vector<Vector2> _output_uv;
	//	Vector<Vector2> _output_uv2;
	//	Vector<float> _output_tangents;
	//	Vector<float> _output_bones;
	//	Vector<float> _output_weights;

	Vector<int> _output_indices;
};

#endif // HEIGHT_MAP_DECELS_H
