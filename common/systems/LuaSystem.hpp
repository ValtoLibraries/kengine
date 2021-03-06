#pragma once

#include "ScriptSystem.hpp"

#include "lua/plua.hpp"
#include "reflection/Reflectible.hpp"

#include "EntityManager.hpp"

#include "common/components/LuaComponent.hpp"
#include "common/packets/LuaState.hpp"

namespace kengine {
    class LuaSystem : public kengine::ScriptSystem<LuaSystem, LuaComponent, kengine::packets::LuaState::Query> {
    public:
        LuaSystem(kengine::EntityManager & em) : ScriptSystem(em) {
            addScriptDirectory("scripts");
            _lua.open_libraries();

			ScriptSystem::init();
        }

    public:
		template<typename Ret, typename ...Args>
		void registerFunction(const std::string & name, const std::function<Ret(Args...)> & func) {
			_lua[name] = FWD(func);
		}

		template<typename Ret, typename ...Args>
		void registerGameObjectMember(const std::string & name, const std::function<Ret(Args...)> & func) {
			_lua["GameObject"][name] = FWD(func);
		}

		template<typename T>
		void registerTypeInternal() {
			putils::lua::registerType<T>(_lua);
		}

    public:
        void executeScript(const std::string & fileName) noexcept {
            try {
                _lua.script_file(fileName);
            } catch (const std::exception & e) {
                std::cerr << "[LuaSystem] Error in '" << fileName << "': " << e.what() << std::endl;
            }
        }

    public:
		void setSelf(kengine::GameObject & go) { _lua["self"] = &go; }
		void unsetSelf() { _lua["self"] = sol::nil; }

    public:
		sol::state & getState() { return _lua;  }

    public:
        void handle(const kengine::packets::LuaState::Query & q) noexcept {
            sendTo(kengine::packets::LuaState::Response{ &_lua }, *q.sender);
        }

    private:
        sol::state & _lua = *(new sol::state);
    };
}