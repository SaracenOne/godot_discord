#include <Godot.hpp>
#include "godot_discord_types.hpp"

#include "godot_discord_core.hpp"
#include "godot_discord_activity_manager.hpp"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
	godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
	godot::Godot::nativescript_init(handle);

	godot::register_class<godot::GODOT_DISCORD_TYPE(User)>();
	godot::register_class<godot::GODOT_DISCORD_TYPE(ActivityTimestamps)>();
	godot::register_class<godot::GODOT_DISCORD_TYPE(ActivityAssets)>();
	godot::register_class<godot::GODOT_DISCORD_TYPE(PartySize)>();
	godot::register_class<godot::GODOT_DISCORD_TYPE(ActivityParty)>();
	godot::register_class<godot::GODOT_DISCORD_TYPE(ActivitySecrets)>();
	godot::register_class<godot::GODOT_DISCORD_TYPE(Activity)>();

	godot::register_class<godot::GODOT_DISCORD_TYPE(ActivityManager)>();
	
	godot::register_class<godot::GODOT_DISCORD_TYPE(Core)>();
}