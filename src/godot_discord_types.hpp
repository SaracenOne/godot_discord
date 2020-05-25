#ifndef GODOT_DISCORD_TYPES_HPP
#define GODOT_DISCORD_TYPES_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include <Engine.hpp>

#include "discord.h"

namespace godot {

#define GODOT_DISCORD_CLASS_INTERNAL(Name, Type) GODOT_CLASS(Name, Type)
#define GODOT_DISCORD_CLASS(Name, Type) GODOT_DISCORD_CLASS_INTERNAL(GODOT_DISCORD_TYPE(Name), Type )

#define MAKE_NEW_DISCORD_TYPE_WRAPPER(Name, NativeHandle) Ref<GODOT_DISCORD_TYPE(Name)>(&GODOT_DISCORD_TYPE(Name)())

#define GODOT_GET_CSTRING(Name) Name.utf8().get_data()

#define GODOT_DISCORD_TYPE(Name) GodotDiscord ## Name

#define GODOT_DISCORD_TYPE_BOOL bool
#define GODOT_DISCORD_TYPE_INT_32 std::int32_t
#define GODOT_DISCORD_TYPE_INT_64 std::int64_t
#define GODOT_DISCORD_TYPE_UINT_8 std::uint8_t
#define GODOT_DISCORD_TYPE_UINT_64 std::uint64_t
#define GODOT_DISCORD_TYPE_STRING String
#define GODOT_DISCORD_TYPE_ClientId GODOT_DISCORD_TYPE_INT_64
#define GODOT_DISCORD_TYPE_Version GODOT_DISCORD_TYPE_INT_32
#define GODOT_DISCORD_TYPE_Snowflake GODOT_DISCORD_TYPE_INT_64
#define GODOT_DISCORD_TYPE_Timestamp GODOT_DISCORD_TYPE_INT_64
#define GODOT_DISCORD_TYPE_UserId GODOT_DISCORD_TYPE_Snowflake
#define GODOT_DISCORD_TYPE_Locale GODOT_DISCORD_TYPE_String
#define GODOT_DISCORD_TYPE_LobbyId GODOT_DISCORD_TYPE_Snowflake
#define GODOT_DISCORD_TYPE_LobbySecret GODOT_DISCORD_TYPE_String
#define GODOT_DISCORD_TYPE_MetadataKey GODOT_DISCORD_TYPE_String
#define GODOT_DISCORD_TYPE_MetadataValue GODOT_DISCORD_TYPE_String
#define GODOT_DISCORD_TYPE_NetworkPeerId GODOT_DISCORD_TYPE_UINT_64
#define GODOT_DISCORD_TYPE_NetworkChannelId GODOT_DISCORD_TYPE_UINT_8
#define GODOT_DISCORD_TYPE_Path GODOT_DISCORD_TYPE_String
#define GODOT_DISCORD_TYPE_DateTime GODOT_DISCORD_TYPE_String

#define GODOT_DISCORD_TYPE_ActivityType discord::ActivityType

#define GODOT_DISCORD_DEFINE(ClassName, VariableMacros, BindMacros) GODOT_DISCORD_DEFINE_INTERNAL(ClassName, VariableMacros, BindMacros, GODOT_DISCORD_TYPE(ClassName))

#define GODOT_DISCORD_BIND_CLASS(GDClassName, Type) GODOT_CLASS( GDClassName, Type)

#define GODOT_DISCORD_DEFINE_INTERNAL(ClassName, VariableMacros, BindMacros, GDClassName)                                       \
class GDClassName : public Reference {                                                                                          \
    GODOT_DISCORD_BIND_CLASS( GDClassName, Reference)                                                                           \
                                                                                                                                \
    discord::ClassName internal_native_bindings;                                                                                \
    using internal_class_name = Discord ## ClassName;                                                                           \
    discord::ClassName &native_binding;                                                                                         \
public:                                                                                                                         \
    static void _register_methods() {                                                                                           \
        BindMacros(GDClassName)                                                                                                 \
    };                                                                                                                          \
                                                                                                                                \
    VariableMacros                                                                                                              \
                                                                                                                                \
    void _init() {};                                                                                                            \
    discord::ClassName &get_native_binding() {return native_binding;}                                                           \
    void set_native_binding(const discord::ClassName &p_native_binding) {native_binding = p_native_binding;}                                 \
                                                                                                                                            \
    GDClassName ## () : native_binding(internal_native_bindings) {memset(&internal_native_bindings, 0, sizeof(internal_native_bindings));} };

#define GODOT_VARIABLE_GET_SET_VAL(Name, GDType, DType, Getter, Setter) \
    GDType get_ ## Name ## () {return static_cast<GDType>(native_binding. ## Getter ## ());}\
    \
    void set_ ## Name ## ( GDType p_val ) {native_binding. ## Setter ## (static_cast<DType>(p_val));}

#define GODOT_VARIABLE_GET_SET_STRING(Name, Getter, Setter)                                                            \
    String get_ ## Name ## () {return String(native_binding. ## Getter ## ());}                                                 \
                                                                                                                                \
    void set_ ## Name ## ( String p_val ) { native_binding. ## Setter ## (p_val.utf8().get_data()); }

#define GODOT_VARIABLE_GET_OBJECT(Name, Type, Getter) \
    Ref< GODOT_DISCORD_TYPE(Type) > get_ ## Name ## () {Ref<GODOT_DISCORD_TYPE(Type)> return_val; return_val.instance(); if(return_val.is_valid()) { return_val->set_native_binding ## ( native_binding.Getter ## ()); return return_val; } else { return NULL;} }\
    \
    void set_ ## Name ## ( Ref< GODOT_DISCORD_TYPE(Type) > p_val ) { Godot::print_error(String("Cannot assign this variable!"), __func__, __FILE__, __LINE__); }


#define GODOT_VARIABLE_GET_SET_OBJECT(Name, Type, Getter) \
    Ref< GODOT_DISCORD_TYPE(Type) > get_ ## Name ## () {Ref<GODOT_DISCORD_TYPE(Type)> return_val; return_val.instance(); if(return_val.is_valid()) { return_val->set_native_binding ## ( native_binding.Getter ## ()); return return_val; } else { return NULL;} }\
    \
    void set_ ## Name ## ( Ref< GODOT_DISCORD_TYPE(Type) > p_val ) {memcpy(&native_binding. ## Getter ## (), &p_val->get_native_binding(), sizeof(GODOT_DISCORD_TYPE(Type)));}


#define GODOT_VAL_BINDING(ClassName, Type, Name, DefaultValue) \
    register_property<ClassName, Type>(#Name, &ClassName::set_ ## Name, &ClassName::get_ ## Name, DefaultValue);

#define GODOT_OBJ_BINDING(ClassName, Type, Name) \
    register_property<ClassName, Ref<GODOT_DISCORD_TYPE(Type)>>(#Name, &ClassName::set_ ## Name, &ClassName::get_ ## Name, nullptr);

// User
#define USER_BIND_MACROS(ClassName) \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_UserId, id, 0) \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_STRING, username, "") \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_STRING, discriminator, "") \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_STRING, avatar, "") \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_BOOL, bot, "") \

#define USER_VARIABLE_MACROS \
    GODOT_VARIABLE_GET_SET_VAL(id, GODOT_DISCORD_TYPE_UserId, GODOT_DISCORD_TYPE_UserId, GetId, SetId) \
    GODOT_VARIABLE_GET_SET_STRING(username, GetUsername, SetUsername) \
    GODOT_VARIABLE_GET_SET_STRING(discriminator, GetDiscriminator, SetDiscriminator) \
    GODOT_VARIABLE_GET_SET_STRING(avatar, GetAvatar, SetAvatar) \
    GODOT_VARIABLE_GET_SET_VAL(bot, GODOT_DISCORD_TYPE_BOOL, GODOT_DISCORD_TYPE_BOOL, GetBot, SetBot) \

GODOT_DISCORD_DEFINE(User, USER_VARIABLE_MACROS, USER_BIND_MACROS)

// Activity

#define ACTIVITY_TIMESTAMPS_BIND_MACROS(ClassName) \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_Timestamp, start, 0) \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_Timestamp, end, 0)

#define ACTIVITY_TIMESTAMPS_VARIABLE_MACROS \
    GODOT_VARIABLE_GET_SET_VAL(start, GODOT_DISCORD_TYPE_Timestamp, GODOT_DISCORD_TYPE_Timestamp, GetStart, SetStart) \
    GODOT_VARIABLE_GET_SET_VAL(end, GODOT_DISCORD_TYPE_Timestamp, GODOT_DISCORD_TYPE_Timestamp, GetEnd, SetEnd) \

GODOT_DISCORD_DEFINE(ActivityTimestamps, ACTIVITY_TIMESTAMPS_VARIABLE_MACROS, ACTIVITY_TIMESTAMPS_BIND_MACROS)

#define ACTIVITY_ASSETS_BIND_MACROS(ClassName) \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_STRING, large_image, "") \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_STRING, large_text, "") \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_STRING, small_image, "") \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_STRING, small_text, "") \

#define ACTIVITY_ASSETS_VARIABLE_MACROS \
    GODOT_VARIABLE_GET_SET_STRING(large_image, GetLargeImage, SetLargeImage) \
    GODOT_VARIABLE_GET_SET_STRING(large_text, GetLargeText, SetLargeText) \
    GODOT_VARIABLE_GET_SET_STRING(small_image, GetSmallImage, SetSmallImage) \
    GODOT_VARIABLE_GET_SET_STRING(small_text, GetSmallText, SetSmallText) \

GODOT_DISCORD_DEFINE(ActivityAssets, ACTIVITY_ASSETS_VARIABLE_MACROS, ACTIVITY_ASSETS_BIND_MACROS)

#define PARTY_SIZE_BIND_MACROS(ClassName) \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_INT_32, current_size, 0) \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_INT_32, max_size, 0)

#define PARTY_SIZE_VARIABLE_MACROS \
    GODOT_VARIABLE_GET_SET_VAL(current_size, GODOT_DISCORD_TYPE_INT_32, GODOT_DISCORD_TYPE_INT_32, GetCurrentSize, SetCurrentSize) \
    GODOT_VARIABLE_GET_SET_VAL(max_size, GODOT_DISCORD_TYPE_INT_32, GODOT_DISCORD_TYPE_INT_32, GetMaxSize, SetMaxSize) \

GODOT_DISCORD_DEFINE(PartySize, PARTY_SIZE_VARIABLE_MACROS, PARTY_SIZE_BIND_MACROS)

#define ACTIVITY_PARTY_BIND_MACROS(ClassName) \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_STRING, id, "") \
    GODOT_OBJ_BINDING(ClassName, PartySize, party_size) \

#define ACTIVITY_PARTY_VARIABLE_MACROS \
    GODOT_VARIABLE_GET_SET_STRING(id, GetId, SetId) \
    GODOT_VARIABLE_GET_SET_OBJECT(party_size, PartySize, GetSize) \

GODOT_DISCORD_DEFINE(ActivityParty, ACTIVITY_PARTY_VARIABLE_MACROS, ACTIVITY_PARTY_BIND_MACROS)

#define ACTIVITY_SECRETS_BIND_MACROS(ClassName) \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_STRING, match, "") \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_STRING, join, "") \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_STRING, spectate, "") \

#define ACTIVITY_SECRETS_VARIABLE_MACROS \
    GODOT_VARIABLE_GET_SET_STRING(match, GetMatch, SetMatch) \
    GODOT_VARIABLE_GET_SET_STRING(join, GetJoin, SetJoin) \
    GODOT_VARIABLE_GET_SET_STRING(spectate, GetSpectate, SetSpectate) \

GODOT_DISCORD_DEFINE(ActivitySecrets, ACTIVITY_SECRETS_VARIABLE_MACROS, ACTIVITY_SECRETS_BIND_MACROS)

#define ACTIVITY_BIND_MACROS(ClassName) \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_INT_64, type, 0) \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_INT_64, application_id, 0) \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_STRING, name, "") \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_STRING, state, "") \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_STRING, details, "") \
    GODOT_OBJ_BINDING(ClassName, ActivityTimestamps, timestamps) \
    GODOT_OBJ_BINDING(ClassName, ActivityAssets, assets) \
    GODOT_OBJ_BINDING(ClassName, ActivityParty, party) \
    GODOT_OBJ_BINDING(ClassName, ActivitySecrets, secrets) \
    GODOT_VAL_BINDING(ClassName, GODOT_DISCORD_TYPE_BOOL, instance, false) \

#define ACTIVITY_VARIABLE_MACROS \
    GODOT_VARIABLE_GET_SET_VAL(type, GODOT_DISCORD_TYPE_INT_64, GODOT_DISCORD_TYPE_ActivityType, GetType, SetType) \
    GODOT_VARIABLE_GET_SET_VAL(application_id, GODOT_DISCORD_TYPE_INT_64, GODOT_DISCORD_TYPE_INT_64, GetApplicationId, SetApplicationId) \
    GODOT_VARIABLE_GET_SET_STRING(name, GetName, SetName) \
    GODOT_VARIABLE_GET_SET_STRING(state, GetState, SetState) \
    GODOT_VARIABLE_GET_SET_STRING(details, GetDetails, SetDetails) \
    GODOT_VARIABLE_GET_SET_OBJECT(timestamps, ActivityTimestamps, GetTimestamps) \
    GODOT_VARIABLE_GET_SET_OBJECT(assets, ActivityAssets, GetAssets) \
    GODOT_VARIABLE_GET_SET_OBJECT(party, ActivityParty, GetParty) \
    GODOT_VARIABLE_GET_SET_OBJECT(secrets, ActivitySecrets, GetSecrets) \
    GODOT_VARIABLE_GET_SET_VAL(instance, GODOT_DISCORD_TYPE_BOOL, GODOT_DISCORD_TYPE_BOOL, GetInstance, SetInstance) \

GODOT_DISCORD_DEFINE(Activity, ACTIVITY_VARIABLE_MACROS, ACTIVITY_BIND_MACROS)

};

#endif // GODOT_DISCORD_TYPES_HPP