#ifndef HEIGHT_MAP_BRUSH_H
#define HEIGHT_MAP_BRUSH_H

#include "grid.h"
#include "height_map.h"

class HeightMapBrush {
public:
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

	void set_radius(int p_radius);
	int get_radius() const { return _radius; }

	void set_opacity(float opacity);
	float get_opacity() const { return _opacity; }

	void set_flatten_height(float flatten_height);
	float get_flatten_height() const { return _flatten_height; }

	void set_texture_index(int tid);
	int get_texture_index() const { return _texture_index; }

	void set_color(Color c);
	Color get_color() const { return _color; }

	void paint(HeightMap &height_map, Point2i cell_pos, int override_mode = -1);

private:
	void generate_procedural(int radius);

	void paint_height(HeightMap &height_map, Point2i cell_pos, float speed);
	void smooth_height(HeightMap &height_map, Point2i cell_pos, float speed);
	void flatten_height(HeightMap &height_map, Point2i cell_pos);
	void paint_indexed_texture(HeightMap &height_map, Point2i cell_pos);
	void paint_color(HeightMap &height_map, Point2i cell_pos);

private:
	int _radius;
	float _opacity;
	Grid2D<float> _shape;
	float _shape_sum;
	Mode _mode;
	float _flatten_height;
	int _texture_index;
	Color _color;
};

#endif // HEIGHT_MAP_BRUSH_H
