#ifndef HEIGHT_MAP_BRUSH_PANEL_H
#define HEIGHT_MAP_BRUSH_PANEL_H

#include <scene/gui/label.h>
#include <scene/gui/slider.h>
#include <scene/gui/spin_box.h>
#include "scene/gui/item_list.h"
#include <scene/gui/control.h>
#include <scene/gui/check_box.h>

class HeightMapBrushPanel : public Control {
	GDCLASS(HeightMapBrushPanel, Control)
public:
	HeightMapBrushPanel();
	~HeightMapBrushPanel();

	static const char *PARAM_CHANGED;

	enum Params {
		BRUSH_SIZE = 0,
		BRUSH_OPACITY,
        BRUSH_HEIGHT,
        BRUSH_MODE_CHANGE,
        DECELS_CHANGED
	};
    Ref<ImageTexture> make_brush_icon(const uint8_t* p_brush_png);
    void init_params(int size, float opacity, float height, int mode);
    ItemList *_Terrain_ItemList;
    CheckBox *_Terrain_Decels_CheckBox;
protected:
	static void _bind_methods();

private:
	void on_param_changed(Variant value, int param);

private:
	Label *_size_label;
	Slider *_size_slider;
	Label *_opacity_label;
	Slider *_opacity_slider;

	SpinBox *_height_edit;
    Label *_decels_label;
};

#endif // HEIGHT_MAP_BRUSH_PANEL_H
