#include "Global.h"
#include "NObject.h"

namespace arctic {

    Napi::FunctionReference* constructor;

    Napi::Object NObject::Init(Napi::Env env, Napi::Object exports) {
        Napi::Function func = DefineClass(env, "NObject", {
            InstanceMethod<&NObject::GetProperty>("getProperty"),
            });

        Napi::FunctionReference* ref = new Napi::FunctionReference();
        *ref = Napi::Persistent(func);
        constructor = ref;
        return exports;
    }

    NObject::NObject(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NObject>(info) {
    }

    Napi::Object NObject::NewInstance(Napi::Env env, Object* native) {
        Napi::EscapableHandleScope scope(env);
        Napi::Object instance = constructor->New({});
        NObject* obj = NObject::Unwrap(instance);
        obj->native_ = std::unique_ptr<Object>(native);
        return scope.Escape(napi_value(instance)).ToObject();
    }

    Napi::Value NObject::GetProperty(const Napi::CallbackInfo& info) {
        Napi::String name_ = info[0].As<Napi::String>();
        std::string name = name_.Utf8Value();
        Variant value = native_->GetProperty(name).get();
        return Variant2NapiValue(info.Env(), value);
    }
}