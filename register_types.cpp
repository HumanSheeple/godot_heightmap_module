#include "register_types.h"
#ifndef _3D_DISABLED
#include "height_map.h"
#include "height_map_editor_plugin.h"
#include "bmaterial.h"
#include "bterrain_material.h"

HeightMapDataSaver *s_heightmap_data_saver = NULL;
HeightMapDataLoader *s_heightmap_data_loader = NULL;

#endif
//static BTerrain* _bterrain = NULL;

void register_hterrain_types() {

#ifndef _3D_DISABLED
    ClassDB::register_class<HeightMap>();
    ClassDB::register_class<HeightMapData>();
    ClassDB::register_class<BSpatialMaterial>();
    ClassDB::register_class<BTerrainMaterial>();

    s_heightmap_data_saver = memnew(HeightMapDataSaver());
    ResourceSaver::add_resource_format_saver(s_heightmap_data_saver);

    s_heightmap_data_loader = memnew(HeightMapDataLoader());
    ResourceLoader::add_resource_format_loader(s_heightmap_data_loader);

    SceneTree::add_idle_callback(BSpatialMaterial::flush_changes);
    BSpatialMaterial::init_shaders();
    BTerrainMaterial::init_shaders();

#ifdef TOOLS_ENABLED
    EditorPlugins::add_by_type<HeightMapEditorPlugin>();

}

void unregister_hterrain_types() {

#ifndef _3D_DISABLED
    if(s_heightmap_data_saver) {
        memdelete(s_heightmap_data_saver);
    }
    if(s_heightmap_data_loader) {
        memdelete(s_heightmap_data_loader);
    }
#endif

    BTerrainMaterial::finish_shaders();
    BSpatialMaterial::finish_shaders();
}
#endif
#endif
