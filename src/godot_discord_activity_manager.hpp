#ifndef GODOT_DISCORD_ACTIVITY_MANAGER_HPP
#define GODOT_DISCORD_ACTIVITY_MANAGER_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include <Engine.hpp>
#include <FuncRef.hpp>

#include "godot_discord_types.hpp"

#include "discord.h"

namespace godot {

class GODOT_DISCORD_TYPE(ActivityManager) : public Reference {
	GODOT_DISCORD_CLASS(ActivityManager, Reference)

    discord::ActivityManager *activity_manager;
private:
    void activity_callback(Ref<FuncRef> p_func_ref, discord::Result p_result) {
        if (p_func_ref->is_valid()) {
            p_func_ref->call_func(static_cast<int32_t>(p_result));
        };
    }

    std::function<void(discord::Result)> get_activity_callback_function(Ref<FuncRef> p_callback) {
        return std::bind(&GODOT_DISCORD_TYPE(ActivityManager)::activity_callback, this, p_callback, std::placeholders::_1);
    }

    void assign_signal_callbacks() {
        if (activity_manager) {
            activity_manager->OnActivityJoin.Connect(
                [this](const char* secret)
                { emit_signal("on_activity_join", String(secret));});
            activity_manager->OnActivitySpectate.Connect(
                [this](const char* secret)
                { emit_signal("on_activity_spectate", String(secret));});
            activity_manager->OnActivityJoinRequest.Connect(
                [this](discord::User const& user)
                {   
                    Ref<GODOT_DISCORD_TYPE(User)> user_wrap;
                    user_wrap.instance();
                    user_wrap->set_native_binding(user);
                    
                    emit_signal("on_activity_join_request", user_wrap);});

            activity_manager->OnActivityInvite.Connect(
                
                [this](discord::ActivityActionType action_type, discord::User const& user, discord::Activity const& activity) 
                { 
                    Ref<GODOT_DISCORD_TYPE(User)> user_wrap;
                    user_wrap.instance();
                    user_wrap->set_native_binding(user);

                    Ref<GODOT_DISCORD_TYPE(Activity)> activity_wrap;
                    activity_wrap.instance();
                    activity_wrap->set_native_binding(activity);

                    emit_signal("on_activity_invite", static_cast<int32_t>(action_type), user_wrap, activity_wrap);});
        }
    }

    bool check_native_binding() {
        if (activity_manager) {
            return true;
        } else {
            Godot::print_error(String("Native binding not assigned!"), __func__, __FILE__, __LINE__);
            return false;
        }
    }
public:
    static void _register_methods() {
        register_signal<GODOT_DISCORD_TYPE(ActivityManager)>("on_activity_join", "secret", GODOT_VARIANT_TYPE_STRING);
        register_signal<GODOT_DISCORD_TYPE(ActivityManager)>("on_activity_spectate", "secret", GODOT_VARIANT_TYPE_STRING);
        register_signal<GODOT_DISCORD_TYPE(ActivityManager)>("on_activity_join_request", "user", GODOT_VARIANT_TYPE_OBJECT);
        register_signal<GODOT_DISCORD_TYPE(ActivityManager)>("on_activity_invite", "action_type", GODOT_VARIANT_TYPE_INT, "user", GODOT_VARIANT_TYPE_OBJECT, "activity", GODOT_VARIANT_TYPE_OBJECT);

        register_method("register_command", &GODOT_DISCORD_TYPE(ActivityManager)::register_command);
        register_method("register_steam", &GODOT_DISCORD_TYPE(ActivityManager)::register_steam);
        register_method("update_activity", &GODOT_DISCORD_TYPE(ActivityManager)::update_activity);
        register_method("clear_activity", &GODOT_DISCORD_TYPE(ActivityManager)::clear_activity);
        register_method("send_request_reply", &GODOT_DISCORD_TYPE(ActivityManager)::send_request_reply);
        register_method("send_invite", &GODOT_DISCORD_TYPE(ActivityManager)::send_invite);
        register_method("accept_invite", &GODOT_DISCORD_TYPE(ActivityManager)::accept_invite);
    }

    void assign_native_binding(discord::ActivityManager *p_activity_manager) {
        activity_manager = p_activity_manager;

        assign_signal_callbacks();
    }

    int32_t register_command(String p_command) {
        if (check_native_binding()) {
            return static_cast<uint32_t>(activity_manager->RegisterCommand(p_command.utf8().get_data()));
        }
        return -1;
    }

    int32_t register_steam(uint32_t p_steam_id) {
        if (check_native_binding()) {
            return static_cast<uint32_t>(activity_manager->RegisterSteam(p_steam_id));
        }
        return -1;
    }

    void update_activity(Ref<Reference> p_activity, Ref<FuncRef> p_callback) {
        if (check_native_binding()) {
            GODOT_DISCORD_TYPE(Activity) *activity = static_cast<GODOT_DISCORD_TYPE(Activity) *>(p_activity.ptr());
            if(activity && p_callback.is_valid()) {
                activity_manager->UpdateActivity(activity->get_native_binding(), get_activity_callback_function(p_callback));
            }
        }
    }
    
    void clear_activity(Ref<FuncRef> p_callback) {
        if (check_native_binding()) {
            if (p_callback.is_valid()) {
                activity_manager->ClearActivity(get_activity_callback_function(p_callback));
            }
        }
    }
    
    void send_request_reply(int64_t p_user_id,
                          uint32_t p_reply,
                          Ref<FuncRef> p_callback) {
        
        if (check_native_binding()) {
            if (p_callback.is_valid()) {
                activity_manager->SendRequestReply(
                    static_cast<discord::UserId>(p_user_id),
                    static_cast<discord::ActivityJoinRequestReply>(p_reply),
                    get_activity_callback_function(p_callback)
            );
            }
        }
    };
    
    void send_invite(int64_t p_user_id,
                    uint32_t type,
                    String p_content,
                    Ref<FuncRef> p_callback) {
        if (check_native_binding()) {
            if(p_callback.is_valid()) {
                activity_manager->SendInvite(
                    static_cast<discord::UserId>(p_user_id),
                    static_cast<discord::ActivityActionType >(type),
                    p_content.utf8().get_data(),
                    get_activity_callback_function(p_callback)
                );
            }
        }
    };

    void accept_invite(int64_t p_user_id, Ref<FuncRef> p_callback) {
        if (check_native_binding()) {
            if(p_callback.is_valid()) {
                activity_manager->AcceptInvite(
                    static_cast<discord::UserId>(p_user_id),
                    get_activity_callback_function(p_callback)
                );
            }
        }
    };

    void _init() {activity_manager = NULL;};
};

}; // namespace godot

#endif // GODOT_DISCORD_ACTIVITY_MANAGER_HPP