/*************************************************************************/
/*                    This file is part of:                              */
/*                    BITSHIFT GODOT PLUGIN                              */
/*                    http://bit-shift.io                                */
/*************************************************************************/
/* Copyright (c) 2017   Fabian Mathews.                                  */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "bterrain_material.h"
#include <assert.h>

BTerrainMaterial::ShaderNames *BTerrainMaterial::shader_names = NULL;

void BTerrainMaterial::init_shaders() {

	shader_names = memnew(ShaderNames);

	shader_names->extra_texture_names[EXTRA_TEXTURE_TERRAIN] = "texture_terrain";
	shader_names->extra_texture_names[EXTRA_TEXTURE_ALBEDO_R] = "texture_albedo_r";
	shader_names->extra_texture_names[EXTRA_TEXTURE_ALBEDO_G] = "texture_albedo_g";
	shader_names->extra_texture_names[EXTRA_TEXTURE_ALBEDO_B] = "texture_albedo_b";
	shader_names->extra_texture_names[EXTRA_TEXTURE_ALBEDO_A] = "texture_albedo_a";
	shader_names->extra_texture_names[EXTRA_TEXTURE_ALBEDO_BASE] = "texture_albedo_base";
	
	shader_names->terrain_uv_scale = "terrain_uv_scale";
}

void BTerrainMaterial::finish_shaders() {

	memdelete(shader_names);
}

void BTerrainMaterial::_update_shader() {
	BSpatialMaterial::_update_shader();
}

void BTerrainMaterial::set_extra_texture(ExtraTextureParam p_param, const Ref<Texture> &p_texture) {

	ERR_FAIL_INDEX(p_param, EXTRA_TEXTURE_MAX);
	
	extra_textures[p_param] = p_texture;
	RID rid = p_texture.is_valid() ? p_texture->get_rid() : RID();
	VS::get_singleton()->material_set_param(_get_material(), shader_names->extra_texture_names[p_param], rid);
	
	_queue_shader_change();
	_change_notify();
}

Ref<Texture> BTerrainMaterial::get_extra_texture(ExtraTextureParam p_param) const {

	ERR_FAIL_INDEX_V(p_param, EXTRA_TEXTURE_MAX, Ref<Texture>());
	return extra_textures[p_param];
}

void BTerrainMaterial::set_terrain_uv_scale(const Vector2 &p_scale) {

	terrain_uv_scale = p_scale;
	VS::get_singleton()->material_set_param(_get_material(), shader_names->terrain_uv_scale, p_scale);
}

Vector2 BTerrainMaterial::get_terrain_uv_scale() const {

	return terrain_uv_scale;
}
    
void BTerrainMaterial::_modify_shader_parameters_string(String& code) {
	code += "uniform sampler2D texture_terrain;\n";
	code += "uniform sampler2D texture_albedo_a;\n";
	code += "uniform sampler2D texture_albedo_r;\n";
	code += "uniform sampler2D texture_albedo_g;\n";
	code += "uniform sampler2D texture_albedo_b;\n";
	code += "uniform sampler2D texture_albedo_base;\n";
	code += "uniform vec2 terrain_uv_scale;\n";
}

void BTerrainMaterial::_modify_shader_vertex_string(String& code) {
}

void BTerrainMaterial::_modify_shader_fragment_string(String& code) {
	code += "\tvec4 terrain_tex = texture(texture_terrain,UV);\n";
    code += "\tvec2 terrain_UV = vec2(1,1);\n";
	code += "\tvec2 terrain_UV=UV*terrain_uv_scale;\n";
	
	code += "\tvec3 albedo_a_tex = texture(texture_albedo_a,terrain_UV).rgb;\n";
	code += "\tvec3 albedo_r_tex = texture(texture_albedo_r,terrain_UV).rgb;\n";
	code += "\tvec3 albedo_g_tex = texture(texture_albedo_g,terrain_UV).rgb;\n";
	code += "\tvec3 albedo_b_tex = texture(texture_albedo_b,terrain_UV).rgb;\n";
	code += "\tvec3 albedo_base_tex = texture(texture_albedo_base,terrain_UV).rgb;\n";
	
	code += "\tvec3 layer_0 = albedo_base_tex;\n";

	//code += "\tALBEDO = albedo.rgb * albedo_tex.rgb * layer_4;\n";
    code += "\tALBEDO = texture(terrain_tex, terrain_UV).rgb;\n";
        
        //code += "\tALBEDO = NORMALMAP;\n"; // debug normalmap
}

void BTerrainMaterial::_bind_methods() {
	BSpatialMaterial::_bind_methods();
	
	ClassDB::bind_method(D_METHOD("set_extra_texture", "param:ExtraTexture", "texture"), &BTerrainMaterial::set_extra_texture);
	ClassDB::bind_method(D_METHOD("get_extra_texture:Texture", "param:ExtraTexture"), &BTerrainMaterial::get_extra_texture);
	
	ClassDB::bind_method(D_METHOD("set_terrain_uv_scale", "scale"), &BTerrainMaterial::set_terrain_uv_scale);
	ClassDB::bind_method(D_METHOD("get_terrain_uv_scale"), &BTerrainMaterial::get_terrain_uv_scale);
	
	ADD_GROUP("TerrainUV", "terrain_uv_");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "terrain_uv_scale"), "set_terrain_uv_scale", "get_terrain_uv_scale");
	
	ADD_GROUP("Color", "albedo_");
	//ADD_PROPERTY(PropertyInfo(Variant::COLOR, "albedo_color"), "set_albedo", "get_albedo");
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "albedo_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture", TEXTURE_ALBEDO);
	
	ADD_GROUP("Terrain", "terrain_");
	//ADD_PROPERTY(PropertyInfo(Variant::COLOR, "albedo_color"), "set_albedo", "get_albedo");
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "terrain_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_extra_texture", "get_extra_texture", EXTRA_TEXTURE_TERRAIN);
	
	
	ADD_GROUP("Normal", "normal_");
	//ADD_PROPERTY(PropertyInfo(Variant::COLOR, "albedo_color"), "set_albedo", "get_albedo");
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "normal_texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_texture", "get_texture", TEXTURE_NORMAL);
	
	ADD_GROUP("Terrain Layer (Alpha)", "terrain_layer_a_");
	//ADD_PROPERTY(PropertyInfo(Variant::COLOR, "albedo_color"), "set_albedo", "get_albedo");
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "terrain_layer_a_albedo", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_extra_texture", "get_extra_texture", EXTRA_TEXTURE_ALBEDO_A);
		
	ADD_GROUP("Terrain Layer (Red)", "terrain_layer_r_");
	//ADD_PROPERTY(PropertyInfo(Variant::COLOR, "albedo_color"), "set_albedo", "get_albedo");
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "terrain_layer_r_albedo", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_extra_texture", "get_extra_texture", EXTRA_TEXTURE_ALBEDO_R);
	
	ADD_GROUP("Terrain Layer (Green)", "terrain_layer_g_");
	//ADD_PROPERTY(PropertyInfo(Variant::COLOR, "albedo_color"), "set_albedo", "get_albedo");
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "terrain_layer_g_albedo", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_extra_texture", "get_extra_texture", EXTRA_TEXTURE_ALBEDO_G);
	
	ADD_GROUP("Terrain Layer (Blue)", "terrain_layer_b_");
	//ADD_PROPERTY(PropertyInfo(Variant::COLOR, "albedo_color"), "set_albedo", "get_albedo");
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "terrain_layer_b_albedo", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_extra_texture", "get_extra_texture", EXTRA_TEXTURE_ALBEDO_B);
	
	ADD_GROUP("Terrain Layer (Base)", "terrain_layer_base_");
	//ADD_PROPERTY(PropertyInfo(Variant::COLOR, "albedo_color"), "set_albedo", "get_albedo");
	ADD_PROPERTYI(PropertyInfo(Variant::OBJECT, "terrain_layer_base_albedo", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_extra_texture", "get_extra_texture", EXTRA_TEXTURE_ALBEDO_BASE);
	
	
	BIND_CONSTANT(EXTRA_TEXTURE_TERRAIN);
	BIND_CONSTANT(EXTRA_TEXTURE_ALBEDO_R);
	BIND_CONSTANT(EXTRA_TEXTURE_ALBEDO_G);
	BIND_CONSTANT(EXTRA_TEXTURE_ALBEDO_B);
	BIND_CONSTANT(EXTRA_TEXTURE_ALBEDO_A);
	BIND_CONSTANT(EXTRA_TEXTURE_ALBEDO_BASE);
}

BTerrainMaterial::BTerrainMaterial() {
	set_terrain_uv_scale(Vector2(10, 10));
	set_feature(FEATURE_NORMAL_MAPPING, true);
}

BTerrainMaterial::~BTerrainMaterial() {
	
}
