{
  "targets": [
    {
      "target_name": "arctic",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "CLANG_CXX_LIBRARY": "libc++",
        "MACOSX_DEPLOYMENT_TARGET": "10.7",
      },
      "msvs_settings": {
        "VCCLCompilerTool": { "ExceptionHandling": 1, "AdditionalOptions": [ "-std:c++17" ] },
      },
      "sources": [ "src/Arctic.cc", "src/NAgent.cc", "src/NObject.cc", "src/AsyncWorker.cc", "src/NodeJsOFDelegate.cc" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "inc"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
    }
  ]
}