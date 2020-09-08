#ifndef ARCTIC_JS_ARCTIC_COMMON_H_
#define ARCTIC_JS_ARCTIC_COMMON_H_

//#include <Windows.h>

#include <atomic>
#include <map>
#include <memory>
#include <vector>

#include <napi.h>
#include <uv.h>
#include "Arctic.h"

namespace arctic {

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