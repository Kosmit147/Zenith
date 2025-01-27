#include "zenith/graphics/materials.hpp"

#include <glm/vec3.hpp>

#include <iterator>

#include "zenith/core/assert.hpp"
#include "zenith/graphics/shaders.hpp"
#include "zenith/log/logger.hpp"

namespace zth::materials {

namespace {

std::unique_ptr<MaterialList> material_list;

} // namespace

MaterialList::MaterialList()
    : emerald{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.1075f, 0.8725f, 0.1075f },
          .diffuse = glm::vec3{ 0.15136f, 1.22848f, 0.15136f },
          .specular = glm::vec3{ 0.633f, 0.727811f, 0.633f },
          .shininess = 76.8f,
      },

      jade{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.675f, 1.1125f, 0.7875f },
          .diffuse = glm::vec3{ 1.08f, 1.78f, 1.26f },
          .specular = glm::vec3{ 0.316228f, 0.316228f, 0.316228f },
          .shininess = 12.8f,
      },

      obsidian{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.26875f, 0.25f, 0.33125f },
          .diffuse = glm::vec3{ 0.3655f, 0.34f, 0.4505f },
          .specular = glm::vec3{ 0.332741f, 0.328634f, 0.346435f },
          .shininess = 38.4f,
      },

      pearl{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 1.25f, 1.03625f, 1.03625f },
          .diffuse = glm::vec3{ 2.0f, 1.658f, 1.658f },
          .specular = glm::vec3{ 0.296648f, 0.296648f, 0.296648f },
          .shininess = 11.264f,
      },

      ruby{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.8725f, 0.05875f, 0.05875f },
          .diffuse = glm::vec3{ 1.22848f, 0.08272f, 0.08272f },
          .specular = glm::vec3{ 0.727811f, 0.626959f, 0.626959f },
          .shininess = 76.8f,
      },

      turquoise{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.5f, 0.93625f, 0.8725f },
          .diffuse = glm::vec3{ 0.792f, 1.48302f, 1.38204f },
          .specular = glm::vec3{ 0.297254f, 0.30829f, 0.306678f },
          .shininess = 12.8f,
      },

      brass{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 1.64706f, 1.117645f, 0.137255f },
          .diffuse = glm::vec3{ 1.560784f, 1.137254f, 0.22745f },
          .specular = glm::vec3{ 0.992157f, 0.941176f, 0.807843f },
          .shininess = 27.89743616f,
      },

      bronze{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 1.0625f, 0.6375f, 0.27f },
          .diffuse = glm::vec3{ 1.428f, 0.8568f, 0.36288f },
          .specular = glm::vec3{ 0.393548f, 0.271906f, 0.166721f },
          .shininess = 25.6f,
      },

      chrome{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 1.25f, 1.25f, 1.25f },
          .diffuse = glm::vec3{ 0.8f, 0.8f, 0.8f },
          .specular = glm::vec3{ 0.774597f, 0.774597f, 0.774597f },
          .shininess = 76.8f,
      },

      copper{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.95625f, 0.3675f, 0.1125f },
          .diffuse = glm::vec3{ 1.4076f, 0.54096f, 0.1656f },
          .specular = glm::vec3{ 0.256777f, 0.137622f, 0.086014f },
          .shininess = 12.8f,
      },

      gold{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 1.23625f, 0.9975f, 0.3725f },
          .diffuse = glm::vec3{ 1.50328f, 1.21296f, 0.45296f },
          .specular = glm::vec3{ 0.628281f, 0.555802f, 0.366065f },
          .shininess = 51.2f,
      },

      silver{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.96125f, 0.96125f, 0.96125f },
          .diffuse = glm::vec3{ 1.01508f, 1.01508f, 1.01508f },
          .specular = glm::vec3{ 0.508273f, 0.508273f, 0.508273f },
          .shininess = 51.2f,
      },

      black_plastic{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.0f, 0.0f, 0.0f },
          .diffuse = glm::vec3{ 0.02f, 0.02f, 0.02f },
          .specular = glm::vec3{ 0.50f, 0.50f, 0.50f },
          .shininess = 32.0f,
      },

      cyan_plastic{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.0f, 0.5f, 0.3f },
          .diffuse = glm::vec3{ 0.0f, 1.01960784f, 1.01960784f },
          .specular = glm::vec3{ 0.50196078f, 0.50196078f, 0.50196078f },
          .shininess = 32.0f,
      },

      green_plastic{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.0f, 0.0f, 0.0f },
          .diffuse = glm::vec3{ 0.2f, 0.7f, 0.2f },
          .specular = glm::vec3{ 0.45f, 0.55f, 0.45f },
          .shininess = 32.0f,
      },

      red_plastic{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.0f, 0.0f, 0.0f },
          .diffuse = glm::vec3{ 1.0f, 0.0f, 0.0f },
          .specular = glm::vec3{ 0.7f, 0.6f, 0.6f },
          .shininess = 32.0f,
      },

      white_plastic{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.0f, 0.0f, 0.0f },
          .diffuse = glm::vec3{ 1.1f, 1.1f, 1.1f },
          .specular = glm::vec3{ 0.70f, 0.70f, 0.70f },
          .shininess = 32.0f,
      },

      yellow_plastic{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.0f, 0.0f, 0.0f },
          .diffuse = glm::vec3{ 1.0f, 1.0f, 0.0f },
          .specular = glm::vec3{ 0.60f, 0.60f, 0.50f },
          .shininess = 32.0f,
      },

      black_rubber{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.1f, 0.1f, 0.1f },
          .diffuse = glm::vec3{ 0.02f, 0.02f, 0.02f },
          .specular = glm::vec3{ 0.4f, 0.4f, 0.4f },
          .shininess = 10.0f,
      },

      cyan_rubber{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.0f, 0.25f, 0.25f },
          .diffuse = glm::vec3{ 0.8f, 1.0f, 1.0f },
          .specular = glm::vec3{ 0.04f, 0.7f, 0.7f },
          .shininess = 10.0f,
      },

      green_rubber{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.0f, 0.25f, 0.0f },
          .diffuse = glm::vec3{ 0.8f, 1.0f, 0.8f },
          .specular = glm::vec3{ 0.04f, 0.7f, 0.04f },
          .shininess = 10.0f,
      },

      red_rubber{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.25f, 0.0f, 0.0f },
          .diffuse = glm::vec3{ 1.0f, 0.8f, 0.8f },
          .specular = glm::vec3{ 0.7f, 0.04f, 0.04f },
          .shininess = 10.0f,
      },

      white_rubber{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.25f, 0.25f, 0.25f },
          .diffuse = glm::vec3{ 1.0f, 1.0f, 1.0f },
          .specular = glm::vec3{ 0.7f, 0.7f, 0.7f },
          .shininess = 10.0f,
      },

      yellow_rubber{
          .shader = &shaders::standard(),
          .ambient = glm::vec3{ 0.25f, 0.25f, 0.0f },
          .diffuse = glm::vec3{ 1.0f, 1.0f, 0.8f },
          .specular = glm::vec3{ 0.7f, 0.7f, 0.04f },
          .shininess = 10.0f,
      }
{}

auto MaterialList::operator[](usize index) const -> const Material&
{
    ZTH_ASSERT(index < size());
    const auto* materials = begin();
    return materials[index];
}

auto MaterialList::size() const -> usize
{
    return std::distance(begin(), end());
}

auto load_materials() -> void
{
    material_list = std::make_unique<MaterialList>();
    ZTH_CORE_INFO("Materials loaded.");
}

auto unload_materials() -> void
{
    material_list.reset();
    ZTH_CORE_INFO("Materials unloaded.");
}

auto materials() -> const MaterialList&
{
    ZTH_ASSERT(material_list != nullptr);
    return *material_list;
}

#define ZTH_MATERIAL_GETTER(material_name)                                                                             \
    auto material_name() -> const Material&                                                                            \
    {                                                                                                                  \
        ZTH_ASSERT(material_list != nullptr);                                                                          \
        return material_list->material_name;                                                                           \
    }

ZTH_MATERIAL_GETTER(plain);
ZTH_MATERIAL_GETTER(emerald);
ZTH_MATERIAL_GETTER(jade);
ZTH_MATERIAL_GETTER(obsidian);
ZTH_MATERIAL_GETTER(pearl);
ZTH_MATERIAL_GETTER(ruby);
ZTH_MATERIAL_GETTER(turquoise);
ZTH_MATERIAL_GETTER(brass);
ZTH_MATERIAL_GETTER(bronze);
ZTH_MATERIAL_GETTER(chrome);
ZTH_MATERIAL_GETTER(copper);
ZTH_MATERIAL_GETTER(gold);
ZTH_MATERIAL_GETTER(silver);
ZTH_MATERIAL_GETTER(black_plastic);
ZTH_MATERIAL_GETTER(cyan_plastic);
ZTH_MATERIAL_GETTER(green_plastic);
ZTH_MATERIAL_GETTER(red_plastic);
ZTH_MATERIAL_GETTER(white_plastic);
ZTH_MATERIAL_GETTER(yellow_plastic);
ZTH_MATERIAL_GETTER(black_rubber);
ZTH_MATERIAL_GETTER(cyan_rubber);
ZTH_MATERIAL_GETTER(green_rubber);
ZTH_MATERIAL_GETTER(red_rubber);
ZTH_MATERIAL_GETTER(white_rubber);
ZTH_MATERIAL_GETTER(yellow_rubber);

} // namespace zth::materials
