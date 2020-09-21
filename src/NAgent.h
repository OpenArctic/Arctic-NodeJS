#ifndef ARCTIC_JS_ARCTIC_NAGENT_H_
#define ARCTIC_JS_ARCTIC_NAGENT_H_

#include <napi.h>
#include <uv.h>
#include "Arctic.h"
#include "NodeJsOFDelegate.h"

namespace arctic {
    class NAgent : public Napi::ObjectWrap<NAgent> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        NAgent(const Napi::CallbackInfo& info);

        static Napi::Object CreateHostAgent(const Napi::CallbackInfo& info, std::string& module_path);
        static Napi::Object CreateClientAgent(const Napi::CallbackInfo& info, std::string& module_path);

    private:
        Napi::Value Start(const Napi::CallbackInfo& info);
        Napi::Value Stop(const Napi::CallbackInfo& info);

        Napi::Value Export(const Napi::CallbackInfo& info);
        Object* FindInternal(uint8_t routing_id, std::string id);
        Napi::Value Find(const Napi::CallbackInfo& info);

        Napi::Value GetRoutingId(const Napi::CallbackInfo& info);

        Agent* agent_;
    };
}

#endif // ARCTIC_JS_ARCTIC_NAGENT_H_