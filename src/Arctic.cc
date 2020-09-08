#include <napi.h>

#include "NAgent.h"
#include "NObject.h"

namespace arctic {

    std::string global_module_path;

    void ConfigureModulePath(const Napi::CallbackInfo& info) {
        Napi::String module_path = info[0].As<Napi::String>();
        global_module_path = module_path.Utf8Value();
        return;
    }

    Napi::Object CreateHostAgent(const Napi::CallbackInfo& info) {
        return NAgent::CreateHostAgent(info, global_module_path);
    }

    Napi::Object CreateClientAgent(const Napi::CallbackInfo& info) {
        return NAgent::CreateClientAgent(info, global_module_path);
    }

    Napi::Object Init(Napi::Env env, Napi::Object exports) {
        NAgent::Init(env, exports);
        NObject::Init(env, exports);

        exports.Set(Napi::String::New(env, "configure_module_path"),
            Napi::Function::New(env, ConfigureModulePath));
        exports.Set(Napi::String::New(env, "create_host_agent"),
            Napi::Function::New(env, CreateHostAgent));
        exports.Set(Napi::String::New(env, "create_client_agent"),
            Napi::Function::New(env, CreateClientAgent));

        return exports;
    }

    NODE_API_MODULE(arctic, Init)
}
