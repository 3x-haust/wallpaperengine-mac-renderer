#include "FBOProvider.h"
#include "WallpaperEngine/Logging/Log.h"
#include <gmpxx.h>

using namespace WallpaperEngine::Render;
using namespace WallpaperEngine::Data::Model;

namespace {
TextureFormat parseFBOFormat (const std::string& format) {
    if (format == "rgba8888") {
	return TextureFormat_ARGB8888;
    }

    if (format == "rgba16161616f") {
	return TextureFormat_RGBA16161616f;
    }

    if (format == "rgba_backbuffer") {
#if defined(__APPLE__)
	return TextureFormat_RGBA16161616f;
#else
	return TextureFormat_ARGB8888;
#endif
    }

    sLog.error ("Unknown FBO format '", format, "', falling back to rgba8888");
    return TextureFormat_ARGB8888;
}
}

FBOProvider::FBOProvider (const FBOProvider* parent) : m_parent (parent) { }

std::shared_ptr<CFBO> FBOProvider::create (const FBO& base, uint32_t flags, const glm::vec2 size) {
    return this->m_fbos[base.name] = std::make_shared<CFBO> (
	       base.name, parseFBOFormat (base.format), flags, base.scale, size.x / base.scale, size.y / base.scale, size.x / base.scale,
	       size.y / base.scale
	   );
}

std::shared_ptr<CFBO> FBOProvider::create (
    const std::string& name, TextureFormat format, uint32_t flags, float scale, glm::vec2 realSize,
    glm::vec2 textureSize
) {
    return this->m_fbos[name] = std::make_shared<CFBO> (
	       name, format, flags, scale, realSize.x, realSize.y, textureSize.x, textureSize.y
	   );
}

std::shared_ptr<CFBO> FBOProvider::alias (const std::string& newName, const std::string& original) {
    return this->m_fbos[newName] = this->m_fbos[original];
}

std::shared_ptr<CFBO> FBOProvider::find (const std::string& name) const {
    if (const auto it = this->m_fbos.find (name); it != this->m_fbos.end ()) {
	return it->second;
    }

    if (this->m_parent == nullptr) {
	return nullptr;
    }

    return this->m_parent->find (name);
}
