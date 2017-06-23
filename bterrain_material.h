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
#ifndef BTERRAIN_MATERIAL_H
#define BTERRAIN_MATERIAL_H

#include "bmaterial.h"

class BTerrainMaterial : public BSpatialMaterial {
	
	GDCLASS(BTerrainMaterial, BSpatialMaterial);

public:
	
	enum ExtraTextureParam {
		EXTRA_TEXTURE_TERRAIN,
		
		EXTRA_TEXTURE_ALBEDO_A,
		EXTRA_TEXTURE_ALBEDO_R,
		EXTRA_TEXTURE_ALBEDO_G,
		EXTRA_TEXTURE_ALBEDO_B,		
		EXTRA_TEXTURE_ALBEDO_BASE,
		EXTRA_TEXTURE_MAX
	};
	
protected:

	struct ShaderNames {
		StringName extra_texture_names[EXTRA_TEXTURE_MAX];
		StringName terrain_uv_scale;
	};
	
	static ShaderNames *shader_names;
	
	Ref<Texture> extra_textures[TEXTURE_MAX];
	Vector2 terrain_uv_scale;
	
	virtual void _update_shader();
	static void _bind_methods();

    virtual void _modify_shader_parameters_string(String& code);
	virtual void _modify_shader_vertex_string(String& code);
	virtual void _modify_shader_fragment_string(String& code);
	
public:
	
	void set_extra_texture(ExtraTextureParam p_param, const Ref<Texture> &p_texture);
	Ref<Texture> get_extra_texture(ExtraTextureParam p_param) const;
	
	void set_terrain_uv_scale(const Vector2 &p_scale);
	Vector2 get_terrain_uv_scale() const;
	
	static void init_shaders();
	static void finish_shaders();
	
	BTerrainMaterial();
	~BTerrainMaterial();
		
};

VARIANT_ENUM_CAST(BTerrainMaterial::ExtraTextureParam)

#endif // BTERRAIN_MATERIAL_H
