#include "trillek-game.hpp"
#include "components/component-factory.hpp"
#include "components/component-templates.hpp"
#include "physics/collidable.hpp"
#include "resources/md5mesh.hpp"
#include "resources/obj.hpp"
#include "resources/text-file.hpp"

#include "systems/physics.hpp"
#include "systems/resource-system.hpp"
#include "systems/transform-system.hpp"
#include "util/json-parser.hpp"

namespace trillek {
/** \brief Type of component
 *
 * DYNAMIC: The component is passed to the system and then stored in the
 * ComponentFactory container
 * SYSTEM : The component is passed to SystemComponent and is stored there
 * SHARED : The component is passed to SharedComponent and is stored there
 *
 * system_value is like system, but for primitive values that don't need a pointer
 * to be passed as argument of a fonction (bool, uint32_t).
 *
 * Only SHARED components can be shared between systems in different threads.
 * By default, DYNAMIC components are registered.
 *
 * The preferred choice is SYSTEM.
 */

using component::Component;

void ComponentFactory::RegisterTypes() {
    auto& shared = TrillekGame::GetSharedComponent();
    auto& system = TrillekGame::GetSystemComponent();
    auto& system_value = TrillekGame::GetSystemValueComponent();
    RegisterComponentType(ComponentAdder<SYSTEM,Component::Collidable>(system));
    RegisterComponentType(ComponentAdder<SHARED,Component::Velocity, bool>(shared));
    RegisterComponentType(ComponentAdder<SHARED,Component::VelocityMax, bool>(shared));
    RegisterComponentType(ComponentAdder<SYSTEM,Component::ReferenceFrame,id_t>(system_value));
    RegisterComponentType(ComponentAdder<SYSTEM,Component::IsReferenceFrame,bool>(system_value));
    RegisterComponentType(ComponentAdder<SYSTEM,Component::CombinedVelocity>(system));
    RegisterComponentType(ComponentAdder<SYSTEM,Component::ReferenceFrame,id_t>(system_value));
    RegisterComponentType(ComponentAdder<SYSTEM,Component::OxygenRate,float_t>(system_value));
    RegisterComponentType(ComponentAdder<SYSTEM,Component::Health,uint32_t>(system_value));
    RegisterComponentType(ComponentAdder<SHARED,Component::GraphicTransform,bool>(shared));
    RegisterComponentType(ComponentAdder<SHARED,Component::GameTransform,bool>(shared));
}

void util::JSONPasrser::RegisterTypes() {
    RegisterParser(resource::ResourceMap::GetInstance());
    RegisterParser(TransformMap::GetInstance());
    RegisterParser(ComponentFactory::GetInstance());
}

void resource::ResourceMap::RegisterTypes() {
    RegisterResourceType<resource::TextFile>();
    RegisterResourceType<resource::MD5Mesh>();
    RegisterResourceType<resource::OBJ>();
}

} // End of trillek
