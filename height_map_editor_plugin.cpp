#include <core/os/input.h>
#include <scene/3d/camera.h>

#include "height_map_editor_plugin.h"

inline Ref<Texture> get_icon(String name) {
	return EditorNode::get_singleton()->get_gui_base()->get_icon(name, "EditorIcons");
}

HeightMapEditorPlugin::HeightMapEditorPlugin(EditorNode *p_editor) {
	_editor = p_editor;
	_mouse_pressed = false;

	_brush.set_radius(5);

	_brush_panel = memnew(HeightMapBrushPanel);
	_brush_panel->connect(HeightMapBrushPanel::PARAM_CHANGED, this, "_on_brush_param_changed");
	_brush_panel->init_params(
			_brush.get_radius(),
			_brush.get_opacity(),
            _brush.get_flatten_height(),
            _brush.get_mode());
	add_control_to_container(CONTAINER_SPATIAL_EDITOR_BOTTOM, _brush_panel);
	_brush_panel->hide();

	_toolbar = memnew(HBoxContainer);
	add_control_to_container(CONTAINER_SPATIAL_EDITOR_MENU, _toolbar);
	_toolbar->hide();

}

HeightMapEditorPlugin::~HeightMapEditorPlugin() {
}

bool HeightMapEditorPlugin::forward_spatial_gui_input(Camera *p_camera, const Ref<InputEvent> &p_event) {

	bool captured_event = false;

	Ref<InputEventMouseButton> mb_ref = p_event;
	Ref<InputEventMouseMotion> mm_ref = p_event;

	if (mb_ref.is_valid()) {
		InputEventMouseButton &mb = **mb_ref;

		if (mb.get_button_index() == BUTTON_LEFT || mb.get_button_index() == BUTTON_RIGHT) {
			if (mb.is_pressed() == false)
				_mouse_pressed = false;

			// Need to check modifiers before capturing the event because they are used in navigation schemes
			if (mb.get_control() == false && mb.get_alt() == false) {
				if (mb.is_pressed())
					_mouse_pressed = true;
				captured_event = true;

				// TODO Prepare undo/redo
			}
		}

	} else if (mm_ref.is_valid() && _mouse_pressed) {
		InputEventMouseMotion &mm = **mm_ref;
		Input &input = *Input::get_singleton();

		if (_brush.get_mode() == HeightMapBrush::MODE_ADD && input.is_mouse_button_pressed(BUTTON_RIGHT)) {
			paint(*p_camera, mm.get_position(), HeightMapBrush::MODE_SUBTRACT);
			captured_event = true;

		} else if (input.is_mouse_button_pressed(BUTTON_LEFT)) {
			paint(*p_camera, mm.get_position());
			captured_event = true;
		}
	}

	return captured_event;
}

void HeightMapEditorPlugin::paint(Camera &camera, Vector2 screen_pos, int override_mode) {
	ERR_FAIL_COND(_height_map == NULL);

	Vector3 origin = camera.project_ray_origin(screen_pos);
	Vector3 dir = camera.project_ray_normal(screen_pos);

	HeightMap &height_map = *_height_map;

	Point2i hit_pos_in_cells;
	if (height_map.cell_raycast(origin, dir, hit_pos_in_cells)) {
		_brush.paint(height_map, hit_pos_in_cells, override_mode);
	}
}

void HeightMapEditorPlugin::edit(Object *p_object) {
	_height_map = p_object ? p_object->cast_to<HeightMap>() : NULL;
}

bool HeightMapEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("HeightMap");
}

void HeightMapEditorPlugin::make_visible(bool p_visible) {
	_brush_panel->set_visible(p_visible);
	_toolbar->set_visible(p_visible);
}


void HeightMapEditorPlugin::on_brush_param_changed(Variant value, int param) {
    int i;
	switch (param) {
		case HeightMapBrushPanel::BRUSH_SIZE:
			_brush.set_radius(value);
			break;

		case HeightMapBrushPanel::BRUSH_OPACITY:
			_brush.set_opacity(value);
			break;

		case HeightMapBrushPanel::BRUSH_HEIGHT:
			_brush.set_flatten_height(value);
			break;

        case HeightMapBrushPanel::BRUSH_MODE_CHANGE:
            i = int(value);
            switch (i) {
                case 0:
                    _brush.set_mode(HeightMapBrush::MODE_ADD);
                    break;
                case 1:
                    _brush.set_mode(HeightMapBrush::MODE_SUBTRACT);
                    break;
                case 2:
                    _brush.set_mode(HeightMapBrush::MODE_SMOOTH);
                    break;
                case 3:
                    _brush.set_mode(HeightMapBrush::MODE_FLATTEN);
                    break;
                case 4:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(0,0,0,1));
                    break;
                case 5:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(1,1,1,1));
                    break;
                case 6:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(1,0,1,1));
                    break;
                case 7:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(0,0,1,0));
                    break;
                case 8:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(0,1,0,0));
                    break;
                case 9:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(1,0,0,1));
                    break;
                case 10:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(0,1,1,0));
                    break;
                case 11:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(0,1,0,1));
                    break;
                case 12:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(0,0,1,1));
                    break;
                case 13:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(0,0,0,0));
                    break;
                case 14:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(1,0,0,0));
                    break;
                case 15:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(0,1,1,1));
                    break;
                case 16:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(1,1,1,0));
                    break;
                case 17:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(1,1,0,1));
                    break;
                case 18:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(1,0,1,0));
                    break;
                case 19:
                    _brush.set_mode(HeightMapBrush::MODE_TEXTURE);
                    _brush.set_color(Color(1,1,0,0));

                    break;
            }


            break;
            case HeightMapBrushPanel::DECELS_CHANGED:
                if (bool(value) == true){
                    if (_height_map->_decels_enabled == false) {
                        _height_map->_decels_enabled = true;
                    } else { _height_map->_decels_enabled = false; }

                } else { _height_map->_decels_enabled = false;}
            break;

             default:
			ERR_PRINT("Unknown parameter");
			break;
	}
}

void HeightMapEditorPlugin::_bind_methods() {

	ClassDB::bind_method(D_METHOD("_on_brush_param_changed", "value", "param"), &HeightMapEditorPlugin::on_brush_param_changed);
}
