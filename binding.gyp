{
  "targets": [
    {
      # "type": "static_libray",
      "target_name": "addon",
      "sources": [ 
        "src/index.cpp", 
       # "src/escpos.cpp",
         "src/getDeviceList.cpp",
          "src/escposPrint.cpp" 
      ],
      "include_dirs": [
          "<!(node -e \"require('nan')\")",
          "src/include"
      ],
      "conditions": [
        [
          "OS=='win'",
          {
            "libraries": [
                "setupapi.lib", 
            ],
            # "copies": [
            #   {
            #     "destination": "<(module_root_dir)/build/Release/",
            #     "files": [ "<(module_root_dir)/src/ConsoleApplication1.dll" ]
            #   }
            # ]
          }
        ]
      ],
      'msvs_settings': {
        'VCCLCompilerTool': {
          'AdditionalOptions': [ '/EHsc'] 
        }
      }
    }
  ]
}