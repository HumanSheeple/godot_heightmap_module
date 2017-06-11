#ifndef HEIGHT_MAP_BRUSH_PANEL_H
#define HEIGHT_MAP_BRUSH_PANEL_H

#include <scene/gui/control.h>
#include "scene/gui/button_array.h"
#include "scene/gui/button.h"
#include "scene/gui/label.h"
#include "scene/gui/slider.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/item_list.h"
#include "height_map_brush.h"


class HeightMapBrushPanel : public Control {
	GDCLASS(HeightMapBrushPanel, Control)
protected:
        static void _bind_methods();
public:
	HeightMapBrushPanel();
	~HeightMapBrushPanel();

    HButtonArray *_Terrain_Brush_Mode_Selector;
    Label *_Terrain_Size_Label;
    Label *_Terrain_Opacity_Label;
    Label *_Terrain_Flatten_Height_Label;
    HSlider *_Terrain_Size_HSlider;
    HSlider *_Terrain_Opacity_HSlider;
    LineEdit *_Terrain_Size_Line_Edit;
    LineEdit *_Terrain_Opacity_Line_Edit;
    LineEdit *_Terrain_Flatten_Height_Line_Edit;
    ItemList *_Terrain_Texture_ItemList;
    ItemList *_Terrain_Brush_ItemList;
    Button *_Terrain_Save_To_Image_Button;
    int _Brush_Size = 1;
    int _Brush_Opacity = 1;
    int _Brush_Flatten_Height = 0;

    enum BrushMode{
        TERRAIN_BRUSH_MODE_ADD = 0,
        TERRAIN_BRUSH_MODE_SUBTRACT = 1,
        TERRAIN_BRUSH_MODE_SMOOTH = 2,
        TERRAIN_BRUSH_MODE_FLATTEN = 3,
        TERRAIN_BRUSH_MODE_TEXTURE = 4
    };
    BrushMode _BRUSHMODE;
    BrushMode get_mode() const { return _BRUSHMODE; }
    void _brush_mode(int b_mode);
    void _on_size_slider_value_changed(int size_value);
    void _on_size_line_edit_entered(String size_text);
    void _on_opacity_slider_value_changed(int opacity_value);
    void _on_opacity_line_edit_entered(String opacity_text);
    void _on_flatten_line_edit_entered(String flatten_text);
    Ref<ImageTexture> make_brush_icon(const uint8_t* p_brush_png);
};

#endif // HEIGHT_MAP_BRUSH_PANEL_H

