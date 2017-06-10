#ifndef HEIGHT_MAP_BRUSH_H
#define HEIGHT_MAP_BRUSH_H

#include "grid.h"
#include "height_map.h"
#include "core/math/math_funcs.h"
#include "core/math/math_2d.h"
#include "core/color.h"

class HeightMapBrush {
public:
//    class Operator {
//    public:Grid2D *dst;
//    public:float opacity = 1.0;
//    };
//    class AddOperator:public Operator {
//        float factor = 1.0f;
//        void exec(int x, int y, float value) {
//            dst[y][x] = dst[y][x] + factor*value;
//        }
//    };
//    class LerpOperator:public Operator {
//        float target_value = 0.f;
//        void exec(int x, int y, float value) {
//            dst[y][x] = Math::lerp(dst[y][x], target_value, (value*opacity));
//        }
//    };
//    class LerpOperatorColour:public Operator {
//        Color target_value = Color (1.f,1.f,1.f,1.f);
//        void exec(int x, int y, float value) {
//            dst[y][x] = dst[y][x].Color::linear_interpolate(target_value, value*opacity);
//        }
//    };
//    class SumOperator:public Operator {
//        float sum = 0.f;
//        void exec(int x, int y, float value) {
//            sum+= dst[y][x]*value;
//        }
//    };


	enum Mode {
		MODE_ADD = 0,
		MODE_SUBTRACT,
		MODE_SMOOTH,
		MODE_FLATTEN,
		MODE_TEXTURE,
		MODE_COUNT
	};

	HeightMapBrush();

	void set_mode(Mode mode);
	Mode get_mode() const { return _mode; }
    void set_opacity(int opacity_val);
    int get_opacity() const { return _opacity; }
	void set_radius(int p_radius);
	int get_radius() const { return _radius; }

	void paint_world_pos(HeightMap &height_map, Point2i cell_pos, int override_mode = -1);

private:
	void generate_procedural(int radius);
    void smooth_height(HeightMap &height_map, Point2i cell_pos, float speed);
	void paint_height(HeightMap &height_map, Point2i cell_pos, float speed);

private:
	int _radius;
	float _opacity;
	Grid2D<float> _shape;
	float _shape_sum;
	Mode _mode;
};

#endif // HEIGHT_MAP_BRUSH_H
