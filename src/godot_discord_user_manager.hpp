#ifndef GODOT_DISCORD_USER_MANAGER_HPP
#define GODOT_DISCORD_USER_MANAGER_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include <Engine.hpp>
#include <FuncRef.hpp>

#include "godot_discord_types.hpp"

#include "discord.h"

namespace godot {

class GODOT_DISCORD_TYPE(UserManager) : public Reference {
	GODOT_DISCORD_CLASS(UserManager, Reference)

    discord::UserManager *user_manager;
    discord::User current_user;
private:
    void user_callback(Ref<FuncRef> p_func_ref, discord::Result p_result, const discord::User &p_user) {
        if (p_func_ref->is_valid()) {

            Ref<GODOT_DISCORD_TYPE(User)> return_val;
            return_val.instance();
            return_val->set_native_binding(p_user);

            p_func_ref->call_func(return_val);
        };
    }

    std::function<void(discord::Result, const discord::User &)> get_user_callback_function(Ref<FuncRef> p_callback) {
        return std::bind(&GODOT_DISCORD_TYPE(UserManager)::user_callback, this, p_callback, std::placeholders::_1, std::placeholders::_2);
    }

    void assign_signal_callbacks() {
        if (user_manager) {
             user_manager->OnCurrentUserUpdate.Connect(
                [this]()
                {
                    Ref<GODOT_DISCORD_TYPE(User)> user = get_current_user();
                    emit_signal("on_current_user_update", user);
                });
        }
    }

    bool check_native_binding() {
        if (user_manager) {
            return true;
        } else {
            Godot::print_error(String("Native binding not assigned!"), __func__, __FILE__, __LINE__);
            return false;
        }
    }
public:
    static void _register_methods() {
        register_signal<GODOT_DISCORD_TYPE(UserManager)>("on_current_user_update", "user", GODOT_VARIANT_TYPE_OBJECT);

        register_method("get_current_user", &GODOT_DISCORD_TYPE(UserManager)::get_current_user);
        register_method("get_user", &GODOT_DISCORD_TYPE(UserManager)::get_user);
        register_method("get_current_user_premium_type", &GODOT_DISCORD_TYPE(UserManager)::get_current_user_premium_type);
        register_method("current_user_has_flag", &GODOT_DISCORD_TYPE(UserManager)::current_user_has_flag);
    }

    void assign_native_binding(discord::UserManager *p_user_manager) {
        user_manager = p_user_manager;

        assign_signal_callbacks();
    }

    Ref<GODOT_DISCORD_TYPE(User)> get_current_user() {
        if (check_native_binding()) {
            user_manager->GetCurrentUser(&current_user);
        }
        Ref<GODOT_DISCORD_TYPE(User)> return_val;
        return_val.instance();
        return_val->set_native_binding(current_user);

        return return_val;
    }

    void get_user(uint64_t p_user_id, Ref<FuncRef> p_callback) {
        if (check_native_binding()) {
            user_manager->GetUser(p_user_id, get_user_callback_function(p_callback));
        }
    }

    uint32_t get_current_user_premium_type() {
        uint32_t return_val = 0;

        if (check_native_binding()) {
            discord::PremiumType premium_type;
            user_manager->GetCurrentUserPremiumType(&premium_type);
            return_val = static_cast<uint32_t>(premium_type);
        }

        return return_val;
    }
    
    bool current_user_has_flag(uint32_t p_user_flag) {
        if (check_native_binding()) {
            discord::UserFlag discord_user_flag = static_cast<discord::UserFlag>(p_user_flag);
            bool has_flag = false;
            user_manager->CurrentUserHasFlag(discord_user_flag, &has_flag);
            return has_flag;
        }

        return false;
    }
    void _init() {user_manager = NULL;};
};

}; // namespace godot

#endif // GODOT_DISCORD_USER_MANAGER_HPP