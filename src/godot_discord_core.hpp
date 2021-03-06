#ifndef GODOT_DISCORD_CORE_HPP
#define GODOT_DISCORD_CORE_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include <Engine.hpp>

#include "godot_discord_types.hpp"

#include "godot_discord_activity_manager.hpp"
#include "godot_discord_application_manager.hpp"
#include "godot_discord_image_manager.hpp"
#include "godot_discord_user_manager.hpp"

#include "discord.h"

namespace godot {

class GODOT_DISCORD_TYPE(Core) : public Reference {
	GODOT_DISCORD_CLASS(Core, Reference)

	Ref<GODOT_DISCORD_TYPE(ActivityManager)> activity_manager;
	Ref<GODOT_DISCORD_TYPE(ApplicationManager)> application_manager;
	Ref<GODOT_DISCORD_TYPE(ImageManager)> image_manager;
	Ref<GODOT_DISCORD_TYPE(UserManager)> user_manager;

	std::unique_ptr<discord::Core> core;
	bool create_discord_core(int64_t p_client_id, uint64_t p_flags);
	void destroy_discord_core();

	void log_problems_function(discord::LogLevel p_level, const char *p_message);
public:
	static void _register_methods();

	Ref<GODOT_DISCORD_TYPE(ActivityManager)> get_activity_manager();
	Ref<GODOT_DISCORD_TYPE(ApplicationManager)> get_application_manager();
	Ref<GODOT_DISCORD_TYPE(ImageManager)> get_image_manager();
	Ref<GODOT_DISCORD_TYPE(UserManager)> get_user_manager();

	void update_callbacks();

	void _init();
	bool setup(int64_t p_client_id, uint64_t p_flags);

	~GODOT_DISCORD_TYPE(Core)();
};

}; // namespace godot

#endif // GODOT_DISCORD_CORE_HPP