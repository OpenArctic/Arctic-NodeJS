#ifndef ARCTIC_JS_ARCTIC_NOBJECT_H_
#define ARCTIC_JS_ARCTIC_NOBJECT_H_

#include <napi.h>
#include "Arctic.h"

namespace arctic {
    class NObject : public Napi::ObjectWrap<NObject> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        NObject(const Napi::CallbackInfo& info);

        Handle GetHandle() { return native_->GetHandle(); }

        static Napi::Object NewInstance(Napi::Env env, Object* native);

        Napi::Value GetProperty(const Napi::CallbackInfo& info);

    private:

        std::unique_ptr<Object> native_;
    };
}

#endif // ARCTIC_JS_ARCTIC_NOBJECT_H_