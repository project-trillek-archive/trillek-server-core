#ifndef TRILLEKGAME_HPP_INCLUDED
#define TRILLEKGAME_HPP_INCLUDED

#include "systems/lua-system.hpp"
#include <memory>
#include <mutex>

namespace trillek {

class TrillekScheduler;
class FakeSystem;

#if defined(_MSC_VER)
class OS;
#endif

namespace physics {
class PhysicsSystem;
}
namespace component {
class Shared;
class System;
class SystemValue;
}

class TrillekGame {
public:

    TrillekGame() {};
    virtual ~TrillekGame() {};
    static void Initialize();

    /** \brief Return the scheduler
     *
     * \return TrillekScheduler& the scheduler
     */
    static TrillekScheduler& GetScheduler() { return *scheduler.get(); }

#if defined(_MSC_VER)
    /** \brief Return the GLFW instance
     *
     * \return OS& the instance
     */
    static OS& GetOS() { return *glfw_os.get(); };
#endif

    /** \brief Return the physics system instance
     *
     * \return physics::PhysicsSystem& the instance
     */
    static physics::PhysicsSystem& GetPhysicsSystem() { return *phys_sys.get(); }

    /** \brief Get the FakeSystem
     *
     * \return FakeSystem& the fake system
     */
    static FakeSystem& GetFakeSystem() { return *fake_system.get(); }

    /** \brief Get the storage of shared components
     *
     * \return component::Shared& the storage
     *
     */
    static component::Shared& GetSharedComponent() { return *shared_component.get(); };

    /** \brief Get the storage of system components stored by pointers
     *
     * \return component::System& the storage
     *
     */
    static component::System& GetSystemComponent() { return *system_component.get(); };

    /** \brief Get the storage of system components stored by values
     *
     * \return component::SystemValue& the storage
     *
     */
    static component::SystemValue& GetSystemValueComponent() { return *system_value_component.get(); };

    /** \brief Get the terminate flag
     *
     * The flag tells the world that the program will terminate
     *
     * \return bool true if we are about to terminate the program
     */
    static bool GetTerminateFlag() { return close_window; };

    /** \brief Tells that the user tries to close the window
     *
     * This function is called by a callback set in GLFW
     */
    static void NotifyCloseWindow() { close_window = true; };

    /** \brief Return the Lua system instance
    *
    * \return script::LuaSystem
    */
    static script::LuaSystem& GetLuaSystem() { return lua_sys; };

private:

    static std::unique_ptr<TrillekScheduler> scheduler;
    static std::unique_ptr<FakeSystem> fake_system;
    static std::unique_ptr<physics::PhysicsSystem> phys_sys;
#if defined(_MSC_VER)
    static std::unique_ptr<OS> glfw_os;
#endif
    static std::unique_ptr<component::Shared> shared_component;
    static std::unique_ptr<component::System> system_component;
    static std::unique_ptr<component::SystemValue> system_value_component;
    static bool close_window;

    static script::LuaSystem lua_sys;
};
}

#endif // TRILLEKGAME_HPP_INCLUDED
