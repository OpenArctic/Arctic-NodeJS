#ifndef ARCTIC_JS_ARCTIC_COMMON_H_
#define ARCTIC_JS_ARCTIC_COMMON_H_

//#include <Windows.h>

#include <atomic>
#include <map>
#include <memory>
#include <vector>

#include <napi.h>
#include <uv.h>

#include "Agent.h"
#include "Arctic.h"
#include "NodeJsOFDelegate.h"

namespace arctic {

    class AddonInstanceContext {
    public:
        AddonInstanceContext() : of_delegate_(nullptr) {};
        virtual ~AddonInstanceContext() {
            if (agent_) {
                agent_->Stop();
            }
        };

        void AddConstructor(std::string name, Napi::FunctionReference* ref) {
            if (ref == nullptr) {
                return;
            }
            constructors[name] = std::unique_ptr<Napi::FunctionReference>(ref);
        }

        Napi::FunctionReference* GetConstructor(std::string name) {
            auto it = constructors.find(name);
            if (it == constructors.end()) {
                return nullptr;
            }
            return it->second.get();
        }

        void SetAgent(Agent* agent) {
            agent_ = std::unique_ptr<Agent>(agent);
        }

        Agent* GetAgent() {
            return agent_.get();
        }

        void InstallNodeJsObjectFactoryDelegate() {
            of_delegate_ = new NodeJsOFDelegate();
            ObjectFactory* object_factory = agent_->GetObjectFactory();
            object_factory->AddDelegate(of_delegate_);
        }

        NodeJsOFDelegate* GetOFDelegate() {
            return of_delegate_;
        }

    private:
        std::map<std::string, std::unique_ptr<Napi::FunctionReference>> constructors;

        // Don't release it
        NodeJsOFDelegate* of_delegate_;
        std::unique_ptr<Agent> agent_;
    };

    inline Variant NapiValue2Variant(Napi::Value& value) {
        if (value.IsNull()) {
            return Variant(Null{});
        }
        else if (value.IsBoolean()) {
            return Variant(value.ToBoolean().Value());
        }
        else if (value.IsNumber()) {
            return Variant(value.ToNumber().DoubleValue());
        }
        else if (value.IsString()) {
            return Variant(value.ToString().Utf8Value());
        }
        else {
            return Variant(Null{});
        }
    }

    inline Napi::Value Variant2NapiValue(Napi::Env& env, Variant& value) {
        if (IsNull(value)) {
            return env.Null();
        }
        else if (IsBoolean(value)) {
            return Napi::Boolean::New(env, std::get<bool>(value));
        }
        else if (IsInteger(value)) {
            return Napi::Number::New(env, (double)(std::get<int>(value)));
        }
        else if (IsDouble(value)) {
            return Napi::Number::New(env, std::get<double>(value));
        }
        else if (IsString(value)) {
            return Napi::String::New(env, std::get<std::string>(value));
        }
        else {
            return env.Undefined();
        }
    }
}

#endif // ARCTIC_JS_ARCTIC_COMMON_H_