#ifndef GODOT_DISCORD_APPLICATION_MANAGER_HPP
#define GODOT_DISCORD_APPLICATION_MANAGER_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include <Engine.hpp>
#include <FuncRef.hpp>

#include "godot_discord_types.hpp"
#include "discord.h"

namespace godot {

class GODOT_DISCORD_TYPE(ApplicationManager) : public Reference {
	GODOT_DISCORD_CLASS(ApplicationManager, Reference)

    discord::ApplicationManager *application_manager;
private:
    void oauth2_callback(Ref<FuncRef> p_func_ref, discord::Result p_result, discord::OAuth2Token const& p_token) {
        if (p_func_ref->is_valid()) {

            Ref<GODOT_DISCORD_TYPE(OAuth2Token)> return_val;
            return_val.instance();
            return_val->set_native_binding(p_token);
            
            p_func_ref->call_func(static_cast<int32_t>(p_result), return_val);
        };
    }

    void validate_or_exit_callback(Ref<FuncRef> p_func_ref, discord::Result p_result) {
        if (p_func_ref->is_valid()) {
            p_func_ref->call_func(static_cast<int32_t>(p_result));
        };
    }

    void ticket_callback(Ref<FuncRef> p_func_ref, discord::Result p_result, char const *p_ticket) {
        if (p_func_ref->is_valid()) {

            String ticket = String(p_ticket);
            
            p_func_ref->call_func(static_cast<int32_t>(p_result), ticket);
        };
    }

    std::function<void(discord::Result, discord::OAuth2Token const&)> get_oauth2_token_callback_function(Ref<FuncRef> p_callback) {
        return std::bind(&GODOT_DISCORD_TYPE(ApplicationManager)::oauth2_callback, this, p_callback, std::placeholders::_1, std::placeholders::_2);
    }

    std::function<void(discord::Result)> get_validate_or_exit_callback_function(Ref<FuncRef> p_callback) {
        return std::bind(&GODOT_DISCORD_TYPE(ApplicationManager)::validate_or_exit_callback, this, p_callback, std::placeholders::_1);
    }

    std::function<void(discord::Result, char const*)> get_ticket_callback_function(Ref<FuncRef> p_callback) {
        return std::bind(&GODOT_DISCORD_TYPE(ApplicationManager)::ticket_callback, this, p_callback, std::placeholders::_1, std::placeholders::_2);
    }

    bool check_native_binding() {
        if (application_manager) {
            return true;
        } else {
            Godot::print_error(String("Native binding not assigned!"), __func__, __FILE__, __LINE__);
            return false;
        }
    }
public:
    static void _register_methods() {
        register_method("get_current_locale", &GODOT_DISCORD_TYPE(ApplicationManager)::get_current_locale);
        register_method("get_current_branch", &GODOT_DISCORD_TYPE(ApplicationManager)::get_current_branch);
        register_method("get_oauth2_token", &GODOT_DISCORD_TYPE(ApplicationManager)::get_oauth2_token);
        register_method("validate_or_exit", &GODOT_DISCORD_TYPE(ApplicationManager)::get_ticket);
        register_method("get_ticket", &GODOT_DISCORD_TYPE(ApplicationManager)::get_ticket);
    }

    void assign_native_binding(discord::ApplicationManager *p_application_manager) {
        application_manager = p_application_manager;
    }

    String get_current_locale() {
        if (check_native_binding()) {
            char locale[128];
            application_manager->GetCurrentLocale(locale);
            return String(locale);
        }
        return "";
    }

    String get_current_branch() {
        if (check_native_binding()) {
            char branch[4096];
            application_manager->GetCurrentBranch(branch);
            return String(branch);
        }

        return "";
    }

    void get_oauth2_token(Ref<FuncRef> p_callback) {
        if (check_native_binding()) {
            application_manager->GetOAuth2Token(get_oauth2_token_callback_function(p_callback));
        }
    }

    void validate_or_exit(Ref<FuncRef> p_callback) {
        if (check_native_binding()) {
            application_manager->ValidateOrExit(get_validate_or_exit_callback_function(p_callback));
        }
    }

    void get_ticket(Ref<FuncRef> p_callback) {
        if (check_native_binding()) {
            application_manager->GetTicket(get_ticket_callback_function(p_callback));
        }
    }

    void _init() {application_manager = NULL;};
};

}; // namespace godot

#endif // GODOT_DISCORD_APPLICATION_MANAGER_HPP