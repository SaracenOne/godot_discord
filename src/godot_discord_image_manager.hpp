#ifndef GODOT_DISCORD_IMAGE_MANAGER_HPP
#define GODOT_DISCORD_IMAGE_MANAGER_HPP

#include <Godot.hpp>
#include <Node.hpp>
#include <Engine.hpp>
#include <Image.hpp>
#include <FuncRef.hpp>

#include "godot_discord_types.hpp"
#include "discord.h"

namespace godot {

class GODOT_DISCORD_TYPE(ImageManager) : public Reference {
	GODOT_DISCORD_CLASS(ImageManager, Reference)

    discord::ImageManager *image_manager;
private:
    void fetch_callback(Ref<FuncRef> p_func_ref, discord::Result p_result, discord::ImageHandle p_image_handle) {
        if (p_func_ref->is_valid()) {

            Ref<GODOT_DISCORD_TYPE(ImageHandle)> return_val;
            return_val.instance();
            return_val->set_native_binding(p_image_handle);

            p_func_ref->call_func(static_cast<int32_t>(p_result), return_val);
        };
    }

    std::function<void(discord::Result, discord::ImageHandle)> get_fetch_callback_function(Ref<FuncRef> p_callback) {
        return std::bind(&GODOT_DISCORD_TYPE(ImageManager)::fetch_callback, this, p_callback, std::placeholders::_1, std::placeholders::_2);
    }

    bool check_native_binding() {
        if (image_manager) {
            return true;
        } else {
            Godot::print_error(String("Native binding not assigned!"), __func__, __FILE__, __LINE__);
            return false;
        }
    }
public:
    static void _register_methods() {
        register_method("fetch", &GODOT_DISCORD_TYPE(ImageManager)::fetch);
        register_method("get_image", &GODOT_DISCORD_TYPE(ImageManager)::get_image);
    }

    void assign_native_binding(discord::ImageManager *p_image_manager) {
        image_manager = p_image_manager;
    }

    void fetch(Ref<GODOT_DISCORD_TYPE(ImageHandle)> p_image_handle, bool p_refresh, Ref<FuncRef> p_callback) {
        if (check_native_binding()) {
           image_manager->Fetch(p_image_handle->get_native_binding(), p_refresh, get_fetch_callback_function(p_callback));
        }
    }

    Ref<Image> get_image(Ref<GODOT_DISCORD_TYPE(ImageHandle)> p_image_handle) {
        Ref<Image> image;
        if (check_native_binding()) {
            discord::ImageDimensions image_dimensions;
            discord::ImageHandle image_handle = p_image_handle->get_native_binding();
            if(image_manager->GetDimensions(image_handle, &image_dimensions) == discord::Result::Ok) {
                PoolByteArray buffer;
                int buffer_size = (image_dimensions.GetWidth() * image_dimensions.GetHeight() * 4);
                buffer.resize(buffer_size);
                if(image_manager->GetData(image_handle, buffer.write().ptr(), buffer_size) == discord::Result::Ok) {
                    image.instance();
                    image->create_from_data(
                        image_dimensions.GetWidth(),
                        image_dimensions.GetHeight(), 
                        false,
                        Image::FORMAT_RGBA8,
                        buffer);
                }
            }
        }

        return image;
    }

    void _init() {image_manager = NULL;};
};

}; // namespace godot

#endif // GODOT_DISCORD_IMAGE_MANAGER_HPP