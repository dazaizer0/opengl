#ifndef YUMEGL_ELEMENTARY_OBJECTS_HPP
#define YUMEGL_ELEMENTARY_OBJECTS_HPP

#include "../../../config.h"
#include "../../../yume.h"

namespace rd2 {
    class Object {
    public:
        mathy::vec3yu<> position{ 0.0f, 0.0f, 0.0f };
        bool enable{};

        Object(mathy::vec3yu<> positionV, bool enableV);

        ~Object() = default;
    };

    Object::Object(mathy::vec3yu<> positionV, bool enableV) {
        position = positionV;
        enable = enableV;
    }

    class ExtendedObject : public Object {
    public:
        colour color = colour::WHITE();
        mathy::vec3yu<> size{};

        ExtendedObject(mathy::vec3yu<> positionV, colour colorV, mathy::vec3yu<> sizeV, bool enableV);

        ~ExtendedObject() = default;
    };

    ExtendedObject::ExtendedObject(mathy::vec3yu<> positionV, colour colorV, mathy::vec3yu<> sizeV, bool enableV) : Object(positionV, enableV) {
        size = sizeV;
    }
}
#endif
