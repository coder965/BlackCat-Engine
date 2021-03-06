// [12/17/2016 MRB]

#include "PhysicsImp/PhysicsImpPCH.h"
#include "Core/Memory/bcAlloc.h"
#include "Physics/bcException.h"
#include "PhysicsImp/bcExport.h"
#include "PhysicsImp/bcUtility.h"
#include "PhysicsImp/Fundation/bcPhysics.h"
#include "PhysicsImp/Fundation/bcSceneBuilder.h"
#include "PhysicsImp/Fundation/bcScene.h"
#include "PhysicsImp/Fundation/bcTransform.h"
#include "PhysicsImp/Fundation/bcMemoryBuffer.h"
#include "PhysicsImp/Fundation/bcSerializeBuffer.h"
#include "PhysicsImp/Fundation/bcMaterial.h"
#include "PhysicsImp/Shape/bcShapeGeometry.h"
#include "PhysicsImp/Shape/bcTriangleMesh.h"
#include "PhysicsImp/Shape/bcConvexMesh.h"
#include "PhysicsImp/Shape/bcHeightField.h"
#include "PhysicsImp/Shape/bcShape.h"
#include "PhysicsImp/Body/bcActor.h"
#include "PhysicsImp/Body/bcRigidActor.h"
#include "PhysicsImp/Body/bcRigidStatic.h"
#include "PhysicsImp/Body/bcRigidDynamic.h"
#include "PhysicsImp/Body/bcAggregate.h"
#include "PhysicsImp/Joint/bcJoint.h"
#include "PhysicsImp/Joint/bcD6Joint.h"
#include "PhysicsImp/Joint/bcDistanceJoint.h"
#include "PhysicsImp/Joint/bcFixedJoint.h"
#include "PhysicsImp/Joint/bcPrismaticJoint.h"
#include "PhysicsImp/Joint/bcRevoluteJoint.h"
#include "PhysicsImp/Joint/bcSphericalJoint.h"

namespace black_cat
{
	namespace physics
	{
		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_physics< g_api_physx >::bc_platform_physics()
		{
			m_pack.m_px_physics = nullptr;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_physics< g_api_physx >::bc_platform_physics(bc_platform_physics&& p_other) noexcept
		{
			operator=(std::move(p_other));
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_physics< g_api_physx >::~bc_platform_physics()
		{
			if (m_initialized)
			{
				destroy();
			}
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_platform_physics< g_api_physx >& bc_platform_physics< g_api_physx >::operator=(bc_platform_physics&& p_other) noexcept
		{
			m_pack.m_px_fundation = p_other.m_pack.m_px_fundation;
			m_pack.m_px_profile = p_other.m_pack.m_px_profile;
			m_pack.m_px_physics = p_other.m_pack.m_px_physics;
			m_pack.m_px_cooking = p_other.m_pack.m_px_cooking;
			m_pack.m_allocator = std::move(p_other.m_pack.m_allocator);
			m_pack.m_logger = std::move(p_other.m_pack.m_logger);

			p_other.m_pack.m_px_fundation = nullptr;
			p_other.m_pack.m_px_profile = nullptr;
			p_other.m_pack.m_px_physics = nullptr;
			p_other.m_pack.m_px_cooking = nullptr;

			return *this;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_scene bc_platform_physics< g_api_physx >::create_scene(bc_scene_builder&& p_desc)
		{
			bc_scene l_result;

			p_desc.get_platform_pack().m_px_desc.cpuDispatcher = m_pack.m_task_dispatcher.get();

			l_result.get_platform_pack().m_px_scene = m_pack.m_px_physics->createScene(p_desc.get_platform_pack().m_px_desc);
			l_result.get_platform_pack().m_allocator = m_pack.m_allocator->m_imp.get();
			l_result.get_platform_pack().m_task_dispatcher = m_pack.m_task_dispatcher->m_imp.get();
			l_result.get_platform_pack().m_simulation_callback = std::move(p_desc.get_platform_pack().m_simulation_callback);
			l_result.get_platform_pack().m_contact_filter_callback = std::move(p_desc.get_platform_pack().m_contact_filter_callback);
			l_result.get_platform_pack().m_contact_modify_callback = std::move(p_desc.get_platform_pack().m_contact_modify_callback);

			return l_result;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bcUINT32 bc_platform_physics< g_api_physx >::get_scene_count() const noexcept
		{
			return m_pack.m_px_physics->getNbScenes();
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_rigid_static_ref bc_platform_physics< g_api_physx >::create_rigid_static(const bc_transform& p_pose)
		{
			bc_rigid_static l_result;
			static_cast< bc_physics_reference& >(l_result).get_platform_pack().m_px_object =
				m_pack.m_px_physics->createRigidStatic(const_cast< bc_transform& >(p_pose).get_platform_pack().m_px_transform);

			return bc_rigid_static_ref(l_result);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_rigid_dynamic_ref bc_platform_physics< g_api_physx >::create_rigid_dynamic(const bc_transform& p_pose)
		{
			bc_rigid_dynamic l_result;

			static_cast< bc_physics_reference& >(l_result).get_platform_pack().m_px_object =
				m_pack.m_px_physics->createRigidDynamic(const_cast< bc_transform& >(p_pose).get_platform_pack().m_px_transform);

			return bc_rigid_dynamic_ref(l_result);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_shape_ref bc_platform_physics< g_api_physx >::create_shape(const bc_shape_geometry& p_geometry,
			const bc_material& p_material,
			bool p_is_exclusive)
		{
			return create_shape
			(
				p_geometry,
				p_material,
				core::bc_enum::and({ bc_shape_flag::simulation, bc_shape_flag::query, bc_shape_flag::visualization }),
				p_is_exclusive
			);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_shape_ref bc_platform_physics< g_api_physx >::create_shape(const bc_shape_geometry& p_geometry,
			const bc_material& p_material,
			bc_shape_flag p_shape_flags,
			bool p_is_exclusive)
		{
			bc_shape l_result;

			static_cast< bc_physics_reference& >(l_result).get_platform_pack().m_px_object =
				m_pack.m_px_physics->createShape
				(
					const_cast< bc_shape_geometry& >(p_geometry).get_platform_pack().m_px_geometry,
					*reinterpret_cast< physx::PxMaterial* >(static_cast< bc_physics_reference& >(const_cast< bc_material& >(p_material)).get_platform_pack().m_px_object),
					p_is_exclusive,
					static_cast< physx::PxShapeFlag::Enum >(p_shape_flags)
				);

			return bc_shape_ref(l_result);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_shape_ref bc_platform_physics< g_api_physx >::create_shape(const bc_shape_geometry& p_geometry,
			bc_material* const* p_materials,
			bcUINT16 p_material_count,
			bool p_is_exclusive)
		{
			return create_shape
			(
				p_geometry,
				p_materials,
				p_material_count,
				core::bc_enum::and({ bc_shape_flag::simulation, bc_shape_flag::query,bc_shape_flag::visualization }),
				p_is_exclusive
			);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_shape_ref bc_platform_physics< g_api_physx >::create_shape(const bc_shape_geometry& p_geometry,
			bc_material* const* p_materials,
			bcUINT16 p_material_count,
			bc_shape_flag p_shape_flags,
			bool p_is_exclusive)
		{
			bc_shape l_result;

			physx::PxMaterial** l_px_material_buffer = reinterpret_cast< physx::PxMaterial** >(bcAlloc(sizeof(physx::PxMaterial*) * p_material_count, core::bc_alloc_type::frame));

			for (bcUINT32 i = 0; i < p_material_count; ++i)
			{
				l_px_material_buffer[i] = static_cast< physx::PxMaterial* >
				(
					static_cast< bc_physics_reference* >(p_materials[i])->get_platform_pack().m_px_object
				);
			}

			static_cast< bc_physics_reference& >(l_result).get_platform_pack().m_px_object =
				m_pack.m_px_physics->createShape
				(
					const_cast< bc_shape_geometry& >(p_geometry).get_platform_pack().m_px_geometry,
					l_px_material_buffer,
					p_material_count,
					p_is_exclusive,
					static_cast< physx::PxShapeFlag::Enum >(p_shape_flags)
				);

			bcFree(l_px_material_buffer);

			return bc_shape_ref(l_result);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bcUINT32 bc_platform_physics< g_api_physx >::get_shape_count() const noexcept
		{
			return m_pack.m_px_physics->getNbShapes();
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_material_ref bc_platform_physics< g_api_physx >::create_material(bcFLOAT p_static_friction,
			bcFLOAT p_dynamic_friction,
			bcFLOAT p_restitution)
		{
			bc_material l_px_material;

			static_cast< bc_physics_reference& >(l_px_material).get_platform_pack().m_px_object =
				m_pack.m_px_physics->createMaterial(p_static_friction, p_dynamic_friction, p_restitution);

			return bc_material_ref(l_px_material);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bcUINT32 bc_platform_physics< g_api_physx >::get_material_count() const noexcept
		{
			return m_pack.m_px_physics->getNbMaterials();
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_aggregate_ref bc_platform_physics< g_api_physx >::create_aggregate(bcUINT32 p_max_size, bool p_enable_self_collision)
		{
			bc_aggregate l_px_aggregate;

			static_cast< bc_physics_reference& >(l_px_aggregate).get_platform_pack().m_px_object =
				m_pack.m_px_physics->createAggregate(p_max_size, p_enable_self_collision);

			return bc_aggregate_ref(l_px_aggregate);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_memory_buffer bc_platform_physics< g_api_physx >::create_convex_mesh(const bc_convex_mesh_desc& p_desc)
		{
			bc_memory_buffer l_result;
			core::bc_unique_ptr< physx::PxVec3 > l_vertex_buffer(static_cast< physx::PxVec3* >
				(
					bcAlloc(sizeof(physx::PxVec3) * p_desc.m_points.m_count, core::bc_alloc_type::frame)
				));

			physx::PxConvexMeshDesc l_px_desc = bc_convert_to_px_convex_mesh(p_desc, l_vertex_buffer.get());
			l_px_desc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

			physx::PxConvexMeshCookingResult::Enum l_px_result_flag;
			if (m_pack.m_px_cooking->cookConvexMesh(l_px_desc, *l_result.get_platform_pack().m_px_stream, &l_px_result_flag))
			{
				l_result.get_platform_pack().m_is_valid = true;

				return l_result;
			}

			// If failded try with eINFLATE_CONVEX flag
			l_px_desc.flags |= physx::PxConvexFlag::eINFLATE_CONVEX;

			if (m_pack.m_px_cooking->cookConvexMesh(l_px_desc, *l_result.get_platform_pack().m_px_stream, &l_px_result_flag))
			{
				l_result.get_platform_pack().m_is_valid = true;
			}

			return l_result;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_convex_mesh_ref bc_platform_physics< g_api_physx >::create_convex_mesh(bc_memory_buffer& p_buffer)
		{
			bcAssert(p_buffer.is_valid());

			bc_convex_mesh l_result;

			physx::PxDefaultMemoryInputData input
			(
				static_cast< physx::PxU8* >(p_buffer.get_buffer_pointer()),
				p_buffer.get_buffer_size()
			);
			static_cast< bc_physics_reference& >(l_result).get_platform_pack().m_px_object =
				m_pack.m_px_physics->createConvexMesh(input);

			return bc_convex_mesh_ref(l_result);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bcUINT32 bc_platform_physics< g_api_physx >::get_convex_mesh_count() const noexcept
		{
			return m_pack.m_px_physics->getNbConvexMeshes();
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_memory_buffer bc_platform_physics< g_api_physx >::create_triangle_mesh(const bc_triangle_mesh_desc& p_desc)
		{
			bc_memory_buffer l_result;
			bool is_16_bit_index = core::bc_enum::has(p_desc.m_flag, bc_triangle_mesh_flag::use_16bit_index);

			core::bc_unique_ptr< physx::PxVec3 > l_vertex_buffer(static_cast< physx::PxVec3* >
				(
					bcAlloc(sizeof(physx::PxVec3) * p_desc.m_points.m_count, core::bc_alloc_type::frame)
				));
			core::bc_unique_ptr< bcBYTE > l_index_buffer(static_cast< bcBYTE* >
				(
					bcAlloc(sizeof(bcBYTE) * p_desc.m_indices.m_count * (is_16_bit_index ? sizeof(bcUINT16) : sizeof(bcUINT32)), core::bc_alloc_type::frame)
				));

			physx::PxTriangleMeshDesc l_px_desc = bc_convert_to_px_triangle_mesh(p_desc, l_vertex_buffer.get(), l_index_buffer.get());

			if (m_pack.m_px_cooking->cookTriangleMesh(l_px_desc, *l_result.get_platform_pack().m_px_stream))
			{
				l_result.get_platform_pack().m_is_valid = true;
			}

			return l_result;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_triangle_mesh_ref bc_platform_physics< g_api_physx >::create_triangle_mesh(bc_memory_buffer& p_buffer)
		{
			bcAssert(p_buffer.is_valid());

			bc_triangle_mesh l_result;

			physx::PxDefaultMemoryInputData input
			(
				static_cast< physx::PxU8* >(p_buffer.get_buffer_pointer()),
				p_buffer.get_buffer_size()
			);
			static_cast< bc_physics_reference& >(l_result).get_platform_pack().m_px_object =
				m_pack.m_px_physics->createTriangleMesh(input);

			return bc_triangle_mesh_ref(l_result);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bcUINT32 bc_platform_physics< g_api_physx >::get_triangle_mesh_count() const noexcept
		{
			return m_pack.m_px_physics->getNbTriangleMeshes();
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_memory_buffer bc_platform_physics< g_api_physx >::create_height_field(const bc_height_field_desc& p_desc)
		{
			bc_memory_buffer l_result;
			core::bc_unique_ptr< physx::PxHeightFieldSample > l_sample_buffer(static_cast< physx::PxHeightFieldSample* >
				(
					bcAlloc(sizeof(physx::PxHeightFieldSample) * (p_desc.m_num_row * p_desc.m_num_column), core::bc_alloc_type::frame)
				));

			physx::PxHeightFieldDesc l_px_desc = bc_convert_to_px_height_field(p_desc, l_sample_buffer.get());

			if (m_pack.m_px_cooking->cookHeightField(l_px_desc, *l_result.get_platform_pack().m_px_stream))
			{
				l_result.get_platform_pack().m_is_valid = true;
			}

			return l_result;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_height_field_ref bc_platform_physics< g_api_physx >::create_height_field(bc_memory_buffer& p_buffer)
		{
			bcAssert(p_buffer.is_valid());

			bc_height_field l_result;

			physx::PxDefaultMemoryInputData input
			(
				static_cast< physx::PxU8* >(p_buffer.get_buffer_pointer()),
				p_buffer.get_buffer_size()
			);
			static_cast< bc_physics_reference& >(l_result).get_platform_pack().m_px_object =
				m_pack.m_px_physics->createHeightField(input);

			return bc_height_field_ref(l_result);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bcUINT32 bc_platform_physics< g_api_physx >::get_height_field_count() const noexcept
		{
			return m_pack.m_px_physics->getNbHeightFields();
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_fixed_joint_ref bc_platform_physics< g_api_physx >::create_fixed_joint(bc_rigid_actor* p_actor0,
			const bc_transform& p_local_frame0,
			bc_rigid_actor* p_actor1,
			const bc_transform& p_local_frame1)
		{
			bc_fixed_joint l_result;

			static_cast< bc_physics_reference& >(l_result).get_platform_pack().m_px_object =
				physx::PxFixedJointCreate
				(
					*m_pack.m_px_physics,
					static_cast< physx::PxRigidActor* >(static_cast< bc_physics_reference* >(p_actor0)->get_platform_pack().m_px_object),
					const_cast< bc_transform& >(p_local_frame0).get_platform_pack().m_px_transform,
					static_cast< physx::PxRigidActor* >(static_cast< bc_physics_reference* >(p_actor1)->get_platform_pack().m_px_object),
					const_cast< bc_transform& >(p_local_frame1).get_platform_pack().m_px_transform
				);

			return bc_fixed_joint_ref(l_result);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_distance_joint_ref bc_platform_physics< g_api_physx >::create_distance_joint(bc_rigid_actor* p_actor0,
			const bc_transform& p_local_frame0,
			bc_rigid_actor* p_actor1,
			const bc_transform& p_local_frame1)
		{
			bc_distance_joint l_result;

			static_cast< bc_physics_reference& >(l_result).get_platform_pack().m_px_object =
				physx::PxDistanceJointCreate
				(
					*m_pack.m_px_physics,
					static_cast< physx::PxRigidActor* >(static_cast< bc_physics_reference* >(p_actor0)->get_platform_pack().m_px_object),
					const_cast< bc_transform& >(p_local_frame0).get_platform_pack().m_px_transform,
					static_cast< physx::PxRigidActor* >(static_cast< bc_physics_reference* >(p_actor1)->get_platform_pack().m_px_object),
					const_cast< bc_transform& >(p_local_frame1).get_platform_pack().m_px_transform
				);

			return bc_distance_joint_ref(l_result);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_prismatic_joint_ref bc_platform_physics< g_api_physx >::create_prismatic_joint(bc_rigid_actor* p_actor0,
			const bc_transform& p_local_frame0,
			bc_rigid_actor* p_actor1,
			const bc_transform& p_local_frame1)
		{
			bc_prismatic_joint l_result;

			static_cast< bc_physics_reference& >(l_result).get_platform_pack().m_px_object =
				physx::PxPrismaticJointCreate
				(
					*m_pack.m_px_physics,
					static_cast< physx::PxRigidActor* >(static_cast< bc_physics_reference* >(p_actor0)->get_platform_pack().m_px_object),
					const_cast< bc_transform& >(p_local_frame0).get_platform_pack().m_px_transform,
					static_cast< physx::PxRigidActor* >(static_cast< bc_physics_reference* >(p_actor1)->get_platform_pack().m_px_object),
					const_cast< bc_transform& >(p_local_frame1).get_platform_pack().m_px_transform
				);

			return bc_prismatic_joint_ref(l_result);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_revolute_joint_ref bc_platform_physics< g_api_physx >::create_revolute_joint(bc_rigid_actor* p_actor0,
			const bc_transform& p_local_frame0,
			bc_rigid_actor* p_actor1,
			const bc_transform& p_local_frame1)
		{
			bc_revolute_joint l_result;

			static_cast< bc_physics_reference& >(l_result).get_platform_pack().m_px_object =
				physx::PxRevoluteJointCreate
				(
					*m_pack.m_px_physics,
					static_cast< physx::PxRigidActor* >(static_cast< bc_physics_reference* >(p_actor0)->get_platform_pack().m_px_object),
					const_cast< bc_transform& >(p_local_frame0).get_platform_pack().m_px_transform,
					static_cast< physx::PxRigidActor* >(static_cast< bc_physics_reference* >(p_actor1)->get_platform_pack().m_px_object),
					const_cast< bc_transform& >(p_local_frame1).get_platform_pack().m_px_transform
				);

			return bc_revolute_joint_ref(l_result);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_spherical_joint_ref bc_platform_physics< g_api_physx >::create_spherical_joint(bc_rigid_actor* p_actor0,
			const bc_transform& p_local_frame0,
			bc_rigid_actor* p_actor1,
			const bc_transform& p_local_frame1)
		{
			bc_spherical_joint l_result;

			static_cast< bc_physics_reference& >(l_result).get_platform_pack().m_px_object =
				physx::PxSphericalJointCreate
				(
					*m_pack.m_px_physics,
					static_cast< physx::PxRigidActor* >(static_cast< bc_physics_reference* >(p_actor0)->get_platform_pack().m_px_object),
					const_cast< bc_transform& >(p_local_frame0).get_platform_pack().m_px_transform,
					static_cast< physx::PxRigidActor* >(static_cast< bc_physics_reference* >(p_actor1)->get_platform_pack().m_px_object),
					const_cast< bc_transform& >(p_local_frame1).get_platform_pack().m_px_transform
				);

			return bc_spherical_joint_ref(l_result);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_d6_joint_ref bc_platform_physics< g_api_physx >::create_d6_joint(bc_rigid_actor* p_actor0,
			const bc_transform& p_local_frame0,
			bc_rigid_actor* p_actor1,
			const bc_transform& p_local_frame1)
		{
			bc_d6_joint l_result;

			static_cast< bc_physics_reference& >(l_result).get_platform_pack().m_px_object =
				physx::PxD6JointCreate
				(
					*m_pack.m_px_physics,
					static_cast< physx::PxRigidActor* >(static_cast< bc_physics_reference* >(p_actor0)->get_platform_pack().m_px_object),
					const_cast< bc_transform& >(p_local_frame0).get_platform_pack().m_px_transform,
					static_cast< physx::PxRigidActor* >(static_cast< bc_physics_reference* >(p_actor1)->get_platform_pack().m_px_object),
					const_cast< bc_transform& >(p_local_frame1).get_platform_pack().m_px_transform
				);

			return bc_d6_joint_ref(l_result);
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_memory_buffer bc_platform_physics< g_api_physx >::read_to_memory_buffer(void* p_data, bcUINT32 p_size)
		{
			bc_memory_buffer l_result;
			l_result.get_platform_pack().m_px_stream->write(p_data, p_size);
			l_result.get_platform_pack().m_is_valid = true;

			return l_result;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_serialize_buffer bc_platform_physics< g_api_physx >::create_serialize_buffer()
		{
			bc_serialize_buffer::platform_pack l_buffer_pack;
			l_buffer_pack.m_registry = physx::PxSerialization::createSerializationRegistry(*m_pack.m_px_physics);
			l_buffer_pack.m_collection = PxCreateCollection();

			bc_serialize_buffer l_buffer(l_buffer_pack);

			return l_buffer;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_memory_buffer bc_platform_physics< g_api_physx >::serialize(bc_serialize_buffer& p_buffer)
		{
			auto l_output_stream = core::bc_make_unique<physx::PxDefaultMemoryOutputStream>();

			physx::PxSerialization::complete
			(
				*p_buffer.get_platform_pack().m_collection,
				*p_buffer.get_platform_pack().m_registry
			);
			physx::PxSerialization::serializeCollectionToBinary
			(
				*l_output_stream.get(), 
				*p_buffer.get_platform_pack().m_collection,
				*p_buffer.get_platform_pack().m_registry
			);

			bc_memory_buffer l_result;
			l_result.get_platform_pack().m_px_stream = std::move(l_output_stream);
			l_result.get_platform_pack().m_is_valid = true;

			return l_result;
		}

		template<>
		BC_PHYSICSIMP_DLL
		bc_serialize_buffer bc_platform_physics< g_api_physx >::deserialize(bc_memory_buffer& p_buffer)
		{
			void* l_aligned_buffer = bcAlignedAllocThrow(p_buffer.get_buffer_size(), 128, core::bc_alloc_type::unknown);
			
			std::memcpy(l_aligned_buffer, p_buffer.get_buffer_pointer(), p_buffer.get_buffer_size());

			bc_serialize_buffer::platform_pack l_buffer_pack;
			l_buffer_pack.m_registry = physx::PxSerialization::createSerializationRegistry(*m_pack.m_px_physics);
			l_buffer_pack.m_collection = physx::PxSerialization::createCollectionFromBinary(l_aligned_buffer, *l_buffer_pack.m_registry);
			l_buffer_pack.m_collection_deserialize_buffer = l_aligned_buffer;

			bc_serialize_buffer l_result(l_buffer_pack);

			return l_result;
		}

		template<>
		BC_PHYSICSIMP_DLL
		void bc_platform_physics< g_api_physx >::_initialize(core::bc_unique_ptr< bc_iallocator > p_allocator,
			core::bc_unique_ptr< bc_itask_dispatcher > p_task_dispatcher,
			core::bc_unique_ptr< bc_ilogger > p_logger)
		{
			physx::PxTolerancesScale l_px_scale;

			m_pack.m_allocator = core::bc_make_unique< bc_px_allocator >(std::move(p_allocator));
			m_pack.m_task_dispatcher = core::bc_make_unique< bc_px_task_dispatcher >(std::move(p_task_dispatcher));
			m_pack.m_logger = core::bc_make_unique< bc_px_logger >(std::move(p_logger));

			m_pack.m_px_fundation = PxCreateFoundation(PX_PHYSICS_VERSION, *m_pack.m_allocator, *m_pack.m_logger);

			if (!m_pack.m_px_fundation)
			{
				throw bc_physics_exception(0, "Failed to create Physx fundation");
			}

			bool l_track_memory = false;
#ifdef BC_DEBUG
			l_track_memory = true;
			m_pack.m_px_profile = &physx::PxProfileZoneManager::createProfileZoneManager(m_pack.m_px_fundation);

			if (!m_pack.m_px_profile)
			{
				throw bc_physics_exception(0, "Failed to create Physx profile");
			}
#endif

			m_pack.m_px_physics = PxCreatePhysics
			(
				PX_PHYSICS_VERSION,
				*m_pack.m_px_fundation,
				l_px_scale,
				l_track_memory
#ifdef BC_DEBUG
				,
				m_pack.m_px_profile
#endif
			);

			if (!m_pack.m_px_physics)
			{
				throw bc_physics_exception(0, "Failed to create Physx physics");
			}

			m_pack.m_px_cooking = PxCreateCooking
			(
				PX_PHYSICS_VERSION,
				*m_pack.m_px_fundation,
				physx::PxCookingParams(l_px_scale)
			);

			if (!m_pack.m_px_cooking)
			{
				throw bc_physics_exception(0, "Failed to create Physx cooking");
			}

			if (!PxInitExtensions(*m_pack.m_px_physics))
			{
				throw bc_physics_exception(0, "Failed to create Physx extensions");
			}

			PxRegisterHeightFields(*m_pack.m_px_physics);

#ifdef BC_DEBUG
			if (m_pack.m_px_physics->getPvdConnectionManager() != nullptr)
			{
				const bcCHAR* l_pvd_host_ip = "127.0.0.1";
				bcINT32 l_port = 5425;
				bcUINT32 l_timeout = 100;
				physx::PxVisualDebuggerConnectionFlags l_connection_flags = physx::PxVisualDebuggerExt::getAllConnectionFlags();

				m_pack.m_visaulizer = physx::PxVisualDebuggerExt::createConnection
				(
					m_pack.m_px_physics->getPvdConnectionManager(),
					l_pvd_host_ip,
					l_port,
					l_timeout,
					l_connection_flags
				);
				m_pack.m_px_physics->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
			}
#endif
		}

		template<>
		BC_PHYSICSIMP_DLL
		void bc_platform_physics< g_api_physx >::_destroy()
		{
			m_pack.m_px_cooking->release();
			m_pack.m_px_physics->release();
#ifdef BC_DEBUG
			m_pack.m_px_profile->release();
			if (m_pack.m_visaulizer)
			{
				m_pack.m_visaulizer->release();
			}
#endif
			m_pack.m_px_fundation->release();

			m_pack.m_allocator.reset(nullptr);
			m_pack.m_task_dispatcher.reset(nullptr);
			m_pack.m_logger.reset(nullptr);
		}
	}
}