#include "height_map_chunk.h"

HeightMapChunk::HeightMapChunk(Spatial *p_parent, Point2i p_cell_pos, Ref<Material> p_material, Ref<Material> decel_material) {
    cell_origin = p_cell_pos;

	VisualServer &vs = *VisualServer::get_singleton();

	_mesh_instance = vs.instance_create();

    parent_transform_changed(p_parent->get_global_transform());

    if (p_material.is_valid()) {
        vs.instance_geometry_set_material_override(_mesh_instance, p_material->get_rid());
	}

    Ref<World> world = p_parent->get_world();
	if (world.is_valid()) {
		vs.instance_set_scenario(_mesh_instance, world->get_scenario());
	}

	// TODO Is this needed?
	vs.instance_set_visible(_mesh_instance, true);

    _visible = true;
    _active = true;
    _dirty = true;
    _pending_update = false;

    _decel_mesh_instance = vs.instance_create();

    parent_transform_changed(p_parent->get_global_transform());

    if (decel_material.is_valid()) {
        vs.instance_geometry_set_material_override(_decel_mesh_instance, decel_material->get_rid());
    }
    if (world.is_valid()) {
        vs.instance_set_scenario(_decel_mesh_instance, world->get_scenario());
    }

    vs.instance_set_visible(_decel_mesh_instance, true);
}

HeightMapChunk::~HeightMapChunk() {
	VisualServer &vs = *VisualServer::get_singleton();
	if (_mesh_instance.is_valid()) {
		vs.free(_mesh_instance);
		_mesh_instance = RID();
	}
    if (_decel_mesh_instance.is_valid()) {
        vs.free(_decel_mesh_instance);
        _decel_mesh_instance = RID();
    }
	//	if(collider)
	//		collider->queue_delete();
}

void HeightMapChunk::enter_world(World &world) {
	ERR_FAIL_COND(_mesh_instance.is_valid() == false);
	VisualServer &vs = *VisualServer::get_singleton();
	vs.instance_set_scenario(_mesh_instance, world.get_scenario());
    ERR_FAIL_COND(_decel_mesh_instance.is_valid() == false);

    vs.instance_set_scenario(_decel_mesh_instance, world.get_scenario());
}

void HeightMapChunk::exit_world() {
	ERR_FAIL_COND(_mesh_instance.is_valid() == false);
	VisualServer &vs = *VisualServer::get_singleton();
	vs.instance_set_scenario(_mesh_instance, RID());
    ERR_FAIL_COND(_decel_mesh_instance.is_valid() == false);

    vs.instance_set_scenario(_decel_mesh_instance, RID());
}

void HeightMapChunk::parent_transform_changed(const Transform &parent_transform) {
	ERR_FAIL_COND(_mesh_instance.is_valid() == false);
	VisualServer &vs = *VisualServer::get_singleton();
	Transform local_transform(Basis(), Vector3(cell_origin.x, 0, cell_origin.y));
	Transform world_transform = parent_transform * local_transform;
	vs.instance_set_transform(_mesh_instance, world_transform);
    vs.instance_set_transform(_decel_mesh_instance, world_transform);
}

void HeightMapChunk::set_mesh(Ref<Mesh> mesh) {
	ERR_FAIL_COND(_mesh_instance.is_valid() == false);
	VisualServer &vs = *VisualServer::get_singleton();
	vs.instance_set_base(_mesh_instance, mesh.is_valid() ? mesh->get_rid() : RID());
	_mesh = mesh;
}


void HeightMapChunk::set_decel_mesh(Ref<Mesh> mesh) {
    ERR_FAIL_COND(_decel_mesh_instance.is_valid() == false);
    VisualServer &vs = *VisualServer::get_singleton();
    vs.instance_set_base(_decel_mesh_instance, mesh.is_valid() ? mesh->get_rid() : RID());
    _decel_mesh = mesh;
}

void HeightMapChunk::set_material(Ref<Material> material) {
	ERR_FAIL_COND(_mesh_instance.is_valid() == false);
	VisualServer &vs = *VisualServer::get_singleton();
	vs.instance_geometry_set_material_override(_mesh_instance, material.is_valid() ? material->get_rid() : RID());
}

void HeightMapChunk::set_decel_material(Ref<Material> material) {
    ERR_FAIL_COND(_decel_mesh_instance.is_valid() == false);
    VisualServer &vs = *VisualServer::get_singleton();
    vs.instance_geometry_set_material_override(_decel_mesh_instance, material.is_valid() ? material->get_rid() : RID());
}

void HeightMapChunk::set_visible(bool visible) {
	ERR_FAIL_COND(_mesh_instance.is_valid() == false);
	VisualServer &vs = *VisualServer::get_singleton();
	vs.instance_set_visible(_mesh_instance, visible);
}

void HeightMapChunk::set_decel_visible(bool decel_visible) {
    ERR_FAIL_COND(_decel_mesh_instance.is_valid() == false);
    VisualServer &vs = *VisualServer::get_singleton();
    vs.instance_set_visible(_decel_mesh_instance, decel_visible);
}
