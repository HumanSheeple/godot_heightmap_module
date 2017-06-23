#include "register_types.h"
#ifndef _3D_DISABLED
#include "height_map.h"
#include "height_map_editor_plugin.h"
#include "bmaterial.h"
#include "bterrain_material.h"
#endif
//static BTerrain* _bterrain = NULL;

void register_hterrain_types() {

#ifndef _3D_DISABLED
    ClassDB::register_class<HeightMap>();
    ClassDB::register_class<BSpatialMaterial>();
    ClassDB::register_class<BTerrainMaterial>();

    SceneTree::add_idle_callback(BSpatialMaterial::flush_changes);
    BSpatialMaterial::init_shaders();
    BTerrainMaterial::init_shaders();

#ifdef TOOLS_ENABLED
    EditorPlugins::add_by_type<HeightMapEditorPlugin>();

}

void unregister_hterrain_types() {

    BTerrainMaterial::finish_shaders();
    BSpatialMaterial::finish_shaders();
}
#endif
#endif
