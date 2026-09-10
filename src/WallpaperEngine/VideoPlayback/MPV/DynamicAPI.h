#pragma once

// Scene recording does not need a video player. Load libmpv only when a
// wallpaper actually opens a video texture, so its loader cannot stall --help
// or an ordinary image/particle scene. Linux keeps its normal linked API.
#include <mpv/client.h>
#include <mpv/render_gl.h>
#include <mpv/stream_cb.h>
#ifdef __APPLE__
#include <dlfcn.h>
#include <stdexcept>
#include <string>
namespace WallpaperEngine::VideoPlayback::MPV {
struct DynamicAPI {
    decltype (&::mpv_command) mpv_command;
    decltype (&::mpv_create) mpv_create;
    decltype (&::mpv_get_property) mpv_get_property;
    decltype (&::mpv_initialize) mpv_initialize;
    decltype (&::mpv_render_context_create) mpv_render_context_create;
    decltype (&::mpv_render_context_free) mpv_render_context_free;
    decltype (&::mpv_render_context_render) mpv_render_context_render;
    decltype (&::mpv_set_option_string) mpv_set_option_string;
    decltype (&::mpv_set_property) mpv_set_property;
    decltype (&::mpv_set_property_string) mpv_set_property_string;
    decltype (&::mpv_stream_cb_add_ro) mpv_stream_cb_add_ro;
    decltype (&::mpv_terminate_destroy) mpv_terminate_destroy;
    decltype (&::mpv_wait_event) mpv_wait_event;
    DynamicAPI () {
	void* library = dlopen (WWB_MPV_LIBRARY_PATH, RTLD_LAZY | RTLD_LOCAL);
	if (!library) {
	    const char* error = dlerror ();
	    throw std::runtime_error (
		std::string ("Cannot load libmpv for video playback: ") + (error ? error : "unknown loader error")
	    );
	}
	mpv_command = reinterpret_cast<decltype (mpv_command)> (dlsym (library, "mpv_command"));
	if (!mpv_command) {
	    throw std::runtime_error ("Missing libmpv symbol: mpv_command");
	}
	mpv_create = reinterpret_cast<decltype (mpv_create)> (dlsym (library, "mpv_create"));
	if (!mpv_create) {
	    throw std::runtime_error ("Missing libmpv symbol: mpv_create");
	}
	mpv_get_property = reinterpret_cast<decltype (mpv_get_property)> (dlsym (library, "mpv_get_property"));
	if (!mpv_get_property) {
	    throw std::runtime_error ("Missing libmpv symbol: mpv_get_property");
	}
	mpv_initialize = reinterpret_cast<decltype (mpv_initialize)> (dlsym (library, "mpv_initialize"));
	if (!mpv_initialize) {
	    throw std::runtime_error ("Missing libmpv symbol: mpv_initialize");
	}
	mpv_render_context_create
	    = reinterpret_cast<decltype (mpv_render_context_create)> (dlsym (library, "mpv_render_context_create"));
	if (!mpv_render_context_create) {
	    throw std::runtime_error ("Missing libmpv symbol: mpv_render_context_create");
	}
	mpv_render_context_free
	    = reinterpret_cast<decltype (mpv_render_context_free)> (dlsym (library, "mpv_render_context_free"));
	if (!mpv_render_context_free) {
	    throw std::runtime_error ("Missing libmpv symbol: mpv_render_context_free");
	}
	mpv_render_context_render
	    = reinterpret_cast<decltype (mpv_render_context_render)> (dlsym (library, "mpv_render_context_render"));
	if (!mpv_render_context_render) {
	    throw std::runtime_error ("Missing libmpv symbol: mpv_render_context_render");
	}
	mpv_set_option_string
	    = reinterpret_cast<decltype (mpv_set_option_string)> (dlsym (library, "mpv_set_option_string"));
	if (!mpv_set_option_string) {
	    throw std::runtime_error ("Missing libmpv symbol: mpv_set_option_string");
	}
	mpv_set_property = reinterpret_cast<decltype (mpv_set_property)> (dlsym (library, "mpv_set_property"));
	if (!mpv_set_property) {
	    throw std::runtime_error ("Missing libmpv symbol: mpv_set_property");
	}
	mpv_set_property_string
	    = reinterpret_cast<decltype (mpv_set_property_string)> (dlsym (library, "mpv_set_property_string"));
	if (!mpv_set_property_string) {
	    throw std::runtime_error ("Missing libmpv symbol: mpv_set_property_string");
	}
	mpv_stream_cb_add_ro
	    = reinterpret_cast<decltype (mpv_stream_cb_add_ro)> (dlsym (library, "mpv_stream_cb_add_ro"));
	if (!mpv_stream_cb_add_ro) {
	    throw std::runtime_error ("Missing libmpv symbol: mpv_stream_cb_add_ro");
	}
	mpv_terminate_destroy
	    = reinterpret_cast<decltype (mpv_terminate_destroy)> (dlsym (library, "mpv_terminate_destroy"));
	if (!mpv_terminate_destroy) {
	    throw std::runtime_error ("Missing libmpv symbol: mpv_terminate_destroy");
	}
	mpv_wait_event = reinterpret_cast<decltype (mpv_wait_event)> (dlsym (library, "mpv_wait_event"));
	if (!mpv_wait_event) {
	    throw std::runtime_error ("Missing libmpv symbol: mpv_wait_event");
	}
	// Keep the handle alive for video objects and their callbacks.
    }
    static const DynamicAPI& instance () {
	static const DynamicAPI api;
	return api;
    }
};
}
#define WWB_MPV(name) WallpaperEngine::VideoPlayback::MPV::DynamicAPI::instance ().name
#else
#define WWB_MPV(name) name
#endif
