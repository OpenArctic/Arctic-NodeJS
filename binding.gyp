{
  "targets": [
    {
      "target_name": "arctic",
      "msvs_settings": {
        "VCCLCompilerTool": { "AdditionalOptions": [ "-std:c++17" ] },
      },
      "sources": [ "src/Arctic.cc", "src/NAgent.cc", "src/NObject.cc", "src/AsyncWorker.cc", "src/NodeJsOFDelegate.cc" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "inc"
      ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
      "configurations": {
        "Release": {
          "msvs_settings": {
            "VCCLCompilerTool": {
              "Optimization": 0
            }
          }
        }
      }
    }
  ]
}