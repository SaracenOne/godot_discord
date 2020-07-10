#include "godot_discord_core.hpp"

#include <algorithm>

using namespace godot;

void GODOT_DISCORD_TYPE(Core)::_register_methods() {
	register_signal<GodotDiscordCore>("log", "level", GODOT_VARIANT_TYPE_INT, "message", GODOT_VARIANT_TYPE_STRING);

	register_method("get_activity_manager", &GODOT_DISCORD_TYPE(Core)::get_activity_manager);
	register_method("get_application_manager", &GODOT_DISCORD_TYPE(Core)::get_application_manager);
	register_method("get_image_manager", &GODOT_DISCORD_TYPE(Core)::get_image_manager);
	register_method("get_user_manager", &GODOT_DISCORD_TYPE(Core)::get_user_manager);

	register_method("setup", &GODOT_DISCORD_TYPE(Core)::setup);
	register_method("update_callbacks", &GODOT_DISCORD_TYPE(Core)::update_callbacks);
}

bool GODOT_DISCORD_TYPE(Core)::create_discord_core(int64_t p_client_id, uint64_t p_flags) {
	if (core) {
		destroy_discord_core();
	}

	// Create the actual Discord core
	discord::Core* local_core{};
	auto result = discord::Core::Create(p_client_id, p_flags, &local_core);
	core.reset(local_core);

    if (!core) {
		Godot::print_error(String("Failed to instantiate discord core! (err ") + String(static_cast<int>(result)) + String(")"),
		__FUNCTION__, __FILE__, __LINE__);
		return false;
    } else {
		core->SetLogHook(discord::LogLevel::Debug, std::function<void(discord::LogLevel, char const*)>
		(std::bind(
			&GODOT_DISCORD_TYPE(Core)::log_problems_function,
			this, 
			std::placeholders::_1,
			std::placeholders::_2)
		));
	
		activity_manager.instance();

		if (activity_manager.is_valid()) {
			discord::ActivityManager *native_activity_manager = &core->ActivityManager();
			activity_manager->assign_native_binding(native_activity_manager);
		}

		application_manager.instance();

		if (application_manager.is_valid()) {
			discord::ApplicationManager *native_application_manager = &core->ApplicationManager();
			application_manager->assign_native_binding(native_application_manager);
		}

		image_manager.instance();

		if (image_manager.is_valid()) {
			discord::ImageManager *native_image_manager = &core->ImageManager();
			image_manager->assign_native_binding(native_image_manager);
		}

		user_manager.instance();

		if (user_manager.is_valid()) {
			discord::UserManager *native_user_manager = &core->UserManager();
			user_manager->assign_native_binding(native_user_manager);
		}

		return true;
	};
}

void GODOT_DISCORD_TYPE(Core)::destroy_discord_core() {
	core.reset();
}

void GODOT_DISCORD_TYPE(Core)::log_problems_function(discord::LogLevel p_level, const char *p_message) {
	emit_signal("log", static_cast<int>(p_level), String(p_message));
}

//

Ref<GODOT_DISCORD_TYPE(ActivityManager)> GODOT_DISCORD_TYPE(Core)::get_activity_manager() {
	return Ref<GODOT_DISCORD_TYPE(ActivityManager)>(activity_manager);
}

Ref<GODOT_DISCORD_TYPE(ApplicationManager)> GODOT_DISCORD_TYPE(Core)::get_application_manager() {
	return Ref<GODOT_DISCORD_TYPE(ApplicationManager)>(application_manager);
}

Ref<GODOT_DISCORD_TYPE(ImageManager)> GODOT_DISCORD_TYPE(Core)::get_image_manager() {
	return Ref<GODOT_DISCORD_TYPE(ImageManager)>(image_manager);
}

Ref<GODOT_DISCORD_TYPE(UserManager)> GODOT_DISCORD_TYPE(Core)::get_user_manager() {
	return Ref<GODOT_DISCORD_TYPE(UserManager)>(user_manager);
}

bool GODOT_DISCORD_TYPE(Core)::setup(int64_t p_client_id, uint64_t p_flags) {
	return create_discord_core(p_client_id, p_flags);
}

void GODOT_DISCORD_TYPE(Core)::update_callbacks() {
	if (core) {
		core->RunCallbacks();
	}
}

void GODOT_DISCORD_TYPE(Core)::_init() {
}

GODOT_DISCORD_TYPE(Core)::~GODOT_DISCORD_TYPE(Core)() {
	destroy_discord_core();
}