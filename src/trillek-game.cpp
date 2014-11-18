#include "trillek-game.hpp"
#include "trillek-scheduler.hpp"

#if defined(_MSC_VER)
#include "os.hpp"
#endif

#include "systems/fake-system.hpp"
#include "systems/physics.hpp"
#include "components/shared-component.hpp"
#include "components/system-component.hpp"
#include "components/system-component-value.hpp"

namespace trillek {

void TrillekGame::Initialize() {
    scheduler.reset(new TrillekScheduler);
    fake_system.reset(new FakeSystem);
    phys_sys.reset(new physics::PhysicsSystem);
#if defined(_MSC_VER)
    glfw_os.reset(new OS);
#endif
    shared_component.reset(new component::Shared);
    system_component.reset(new component::System);
    system_value_component.reset(new component::SystemValue);
    close_window = false;
}
std::unique_ptr<TrillekScheduler> TrillekGame::scheduler;
std::unique_ptr<FakeSystem> TrillekGame::fake_system;
std::unique_ptr<physics::PhysicsSystem> TrillekGame::phys_sys;
#if defined(_MSC_VER)
std::unique_ptr<OS> TrillekGame::glfw_os;
#endif
std::unique_ptr<component::Shared> TrillekGame::shared_component;
std::unique_ptr<component::System> TrillekGame::system_component;
std::unique_ptr<component::SystemValue> TrillekGame::system_value_component;
bool TrillekGame::close_window;

script::LuaSystem TrillekGame::lua_sys;
} // End of namespace trillek
