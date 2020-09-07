#include "Global.h"
#include "NObject.h"

namespace arctic {

    Napi::FunctionReference* constructor;

    Napi::Object NObject::Init(Napi::Env env, Napi::Object exports) {
        Napi::Function func = DefineClass(env, "NObject", {
            InstanceMethod<&NObject::GetProperty>("getProperty"),
            InstanceMethod<&NObject::SetProperty>("setProperty"),
            InstanceMethod<&NObject::Invoke>("invoke"),
            InstanceMethod<&NObject::AddEventListener>("addEventListener"),
            InstanceMethod<&NObject::RemoveEventListener>("removeEventListener"),
            InstanceMethod<&NObject::FireEvent>("fireEvent"),
            });

        Napi::FunctionReference* ref = new Napi::FunctionReference();
        *ref = Napi::Persistent(func);
        constructor = ref;
        return exports;
    }

    NObject::NObject(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NObject>(info) {
    }

    NObject::EventHandler::EventHandler(std::string event_type, Napi::Function& callback) :
        ObjectEventListener(event_type) {
        Napi::FunctionReference* ref = new Napi::FunctionReference();
        *ref = Napi::Weak(callback);
        native_ = std::unique_ptr<Napi::FunctionReference>(ref);
    }

    Napi::Function NObject::EventHandler::GetFunction() {
        return native_->Value();
    }

    void NObject::EventHandler::DispatchEvent(Object* instance, std::string event_type, std::vector<NamedVariant> params) {
        Napi::Env env = native_->Env();
        std::vector<napi_value> params_;
        for (auto it = params.begin(); it != params.end(); ++it) {
            Napi::Value value = Variant2NapiValue(env, it->value);
            params_.push_back(value);
        }
        native_->Call(params_);
    }

    Napi::Object NObject::NewInstance(Napi::Env env, Object* native) {
        Napi::EscapableHandleScope scope(env);
        Napi::Object instance = constructor->New({});
        NObject* obj = NObject::Unwrap(instance);
        obj->native_ = native;
        return scope.Escape(napi_value(instance)).ToObject();
    }

    Napi::Value NObject::GetProperty(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);
        Napi::String name_ = info[0].As<Napi::String>();
        std::string name = name_.Utf8Value();
        Variant value = native_->GetProperty(name).get();
        deferred.Resolve(Variant2NapiValue(info.Env(), value));
        return deferred.Promise();
    }

    Napi::Value NObject::SetProperty(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);
        Napi::String name_ = info[0].As<Napi::String>();
        std::string name = name_.Utf8Value();
        Napi::Value value_ = info[1];
        Variant value = NapiValue2Variant(value_);
        native_->SetProperty(name, value).get();
        deferred.Resolve(env.Undefined());
        return deferred.Promise();
    }

    Napi::Value NObject::Invoke(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);
        Napi::String method_ = info[0].As<Napi::String>();
        std::string method = method_.Utf8Value();
        if (info.Length() > 1) {
            // Has parameters
            std::vector<NamedVariant> params;
            for (int i = 1; i < info.Length(); i++) {
                Napi::Value param_ = info[i];
                Variant param = NapiValue2Variant(param_);
                params.push_back({ "", param });
            }
            Variant result = native_->Invoke(method, params).get();
            deferred.Resolve(Variant2NapiValue(env, result));
        }
        else {
            Variant result = native_->Invoke(method).get();
            deferred.Resolve(Variant2NapiValue(env, result));
        }
        return deferred.Promise();
    }

    Napi::Value NObject::AddEventListener(const Napi::CallbackInfo& info) {
        Napi::String event_type_ = info[0].As<Napi::String>();
        std::string event_type = event_type_.Utf8Value();
        Napi::Function callback = info[1].As<Napi::Function>();
        EventHandler* event_handler = new EventHandler(event_type, callback);
        event_listeners_.push_back(event_handler);
        native_->AddEventListener(event_handler);
        return Napi::Value();
    }

    Napi::Value NObject::RemoveEventListener(const Napi::CallbackInfo& info) {
        Napi::String event_type_ = info[0].As<Napi::String>();
        std::string event_type = event_type_.Utf8Value();
        Napi::Function callback = info[1].As<Napi::Function>();
        auto it = event_listeners_.begin();
        for (; it != event_listeners_.end(); ++it) {
            EventHandler* event_handler = *it;
            if (event_handler->GetFunction() == callback) {
                if (event_type == event_handler->EventType()) {
                    native_->RemoveEventListener(event_handler);
                    event_listeners_.erase(it);
                    break;
                }
            }
        }
        return Napi::Value();
    }

    Napi::Value NObject::FireEvent(const Napi::CallbackInfo& info) {
        Napi::String event_type_ = info[0].As<Napi::String>();
        std::string event_type = event_type_.Utf8Value();
        std::vector<NamedVariant> params;
        if (info.Length() > 1) {
            // Has parameters
            for (int i = 1; i < info.Length(); i++) {
                Napi::Value param_ = info[i];
                Variant param = NapiValue2Variant(param_);
                params.push_back({ "", param });
            }
        }
        native_->FireEvent(event_type, params);
        return Napi::Value();
    }

    void NObject::Finalize(Napi::Env env) {
        for (auto it = event_listeners_.begin(); it != event_listeners_.end(); ++it) {
            EventHandler* event_handler = *it;
            delete event_handler;
        }
        event_listeners_.clear();
        delete native_;
    }
}