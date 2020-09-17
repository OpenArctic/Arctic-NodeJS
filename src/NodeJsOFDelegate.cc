#include "Global.h"
#include "NodeJsOFDelegate.h"

namespace arctic {
    NodeJsOFDelegate::NodeJsOFDelegate() : next_handle_(0) {

    }

    NodeJsOFDelegate::~NodeJsOFDelegate() {

    }

    uint8_t NodeJsOFDelegate::GetType() {
        return 2;
    }

    Object* NodeJsOFDelegate::Create(std::string id) {
        return nullptr;
    }

    Variant NodeJsOFDelegate::GetProperty(Object* instance, std::string name) {
        Handle handle = instance->GetHandle();
        auto it = objects_.find(handle.raw_handle);
        if (it != objects_.end()) {
            Napi::ObjectReference* ref = it->second.get();
            Napi::HandleScope scope(ref->Env());
            Napi::Value value = ref->Get(name);
            return NapiValue2Variant(value);
        }
        return Variant(Null{});
    }

    void NodeJsOFDelegate::SetProperty(Object* instance, std::string name, Variant value) {
        Handle handle = instance->GetHandle();
        auto it = objects_.find(handle.raw_handle);
        if (it != objects_.end()) {
            Napi::ObjectReference* ref = it->second.get();
            Napi::HandleScope scope(ref->Env());
            Napi::Value value_ = Variant2NapiValue(ref->Env(), value);
            ref->Set(name, value_);
        }
    }

    MaybeError<Variant> NodeJsOFDelegate::Invoke(Object* instance, std::string method) {
        Handle handle = instance->GetHandle();
        auto it = objects_.find(handle.raw_handle);
        if (it != objects_.end()) {
            Napi::ObjectReference* ref = it->second.get();
            Napi::HandleScope scope(ref->Env());
            Napi::Function function = ref->Get(method).As<Napi::Function>();
            Napi::Value result = function.Call(ref->Value(), 0, nullptr);
            if (ref->Env().IsExceptionPending()) {
                Napi::Error e = ref->Env().GetAndClearPendingException();
                Error err(e.Message());
                return MaybeError<Variant>(err);
            }
            Variant value = NapiValue2Variant(result);
            return MaybeError<Variant>(value);
        }
        return MaybeError<Variant>();
    }

    MaybeError<Variant> NodeJsOFDelegate::Invoke(Object* instance, std::string method, std::vector<NamedVariant> params) {
        Handle handle = instance->GetHandle();
        auto it = objects_.find(handle.raw_handle);
        if (it != objects_.end()) {
            Napi::ObjectReference* ref = it->second.get();
            Napi::HandleScope scope(ref->Env());
            Napi::Function function = ref->Get(method).As<Napi::Function>();
            std::vector<napi_value> params_;
            for (auto it = params.begin(); it != params.end(); ++it) {
                Napi::Value value = Variant2NapiValue(ref->Env(), it->value);
                params_.push_back(value);
            }
            Napi::Value result = function.Call(ref->Value(), params_);
            if (ref->Env().IsExceptionPending()) {
                Napi::Error e = ref->Env().GetAndClearPendingException();
                Error err(e.Message());
                return MaybeError<Variant>(err);
            }
            Variant value = NapiValue2Variant(result);
            return MaybeError<Variant>(value);
        }
        return MaybeError<Variant>();
    }

    uint64_t NodeJsOFDelegate::RegisterObject(Napi::Object obj) {
        Napi::ObjectReference* ref = new Napi::ObjectReference();
        *ref = Napi::Weak(obj);
        uint64_t handle = ++next_handle_;
        objects_[handle] = std::unique_ptr<Napi::ObjectReference>(ref);
        return handle;
    }

    Napi::Object NodeJsOFDelegate::FindObject(uint64_t handle) {
        auto it = objects_.find(handle);
        if (it != objects_.end()) {
            return it->second->Value();
        }
        return Napi::Object();
    }
}