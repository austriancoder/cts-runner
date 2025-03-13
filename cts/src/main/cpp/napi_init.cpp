/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <filesystem> // For creating directories
#include <fstream>    // For file operations

#include <sys/stat.h>  // For mkdir
#include <fcntl.h>     // For open
#include <unistd.h>    // For read, write, close

#include <hilog/log.h>

#include "common/common.h"
#include "manager/plugin_manager.h"

#include "rawfile/raw_file_manager.h"

const int GLOBAL_RESMGR = 0xFF00;
const char *TAG = "[Sample_rawfile]";

namespace NativeXComponentSample {
EXTERN_C_START

static void CreateDirectory(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        mkdir(path.c_str(), 0755);  // Create directory with read/write/execute permissions
    }
}

static void CopyFile(const std::string& src, const std::string& dest) {
    std::ifstream srcFile(src, std::ios::binary);
    std::ofstream destFile(dest, std::ios::binary);
    destFile << srcFile.rdbuf(); // Copy contents
}

void CopyDirectoryStructure(NativeResourceManager* mNativeResMgr, const std::string& srcPath, const std::string& destPath) {
    RawDir* rawDir = OH_ResourceManager_OpenRawDir(mNativeResMgr, srcPath.c_str());
    if (!rawDir) {
        // Failed to open directory
        return;
    }

    int count = OH_ResourceManager_GetRawFileCount(rawDir);
    for (int i = 0; i < count; i++) {
        std::string filename = OH_ResourceManager_GetRawFileName(rawDir, i);
        std::string fullSrcPath = srcPath + "/" + filename;
        
        if (srcPath.empty())
            fullSrcPath = filename;
    
        std::string fullDestPath = destPath + "/" + filename;

        if (OH_ResourceManager_IsRawDir(mNativeResMgr, fullSrcPath.c_str())) {
            // Create directory at destination
            CreateDirectory(fullDestPath);
            // Recursively copy subdirectory
            CopyDirectoryStructure(mNativeResMgr, fullSrcPath, fullDestPath);
        } else {
            // Copy file
            CopyFile(fullSrcPath, fullDestPath);
        }
    }
}

static napi_value CopyAssets(napi_env env, napi_callback_info info)
{
    OH_LOG_Print(LOG_APP, LOG_INFO, GLOBAL_RESMGR, TAG, "GetFileList Begin");
    size_t argc = 2;
    napi_value argv[2] = {nullptr};

    napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

    napi_valuetype valueType;
    napi_typeof(env, argv[0], &valueType);
    NativeResourceManager *mNativeResMgr = OH_ResourceManager_InitNativeResourceManager(env, argv[0]);
    size_t strSize;
    char strBuf[256];
    napi_get_value_string_utf8(env, argv[1], strBuf, sizeof(strBuf), &strSize);
    std::string filename(strBuf, strSize);
    
    CopyDirectoryStructure(mNativeResMgr,filename.c_str(), "/data/storage/el2/base/files/");

    OH_ResourceManager_ReleaseNativeResourceManager(mNativeResMgr);
    
    return nullptr;
}

static napi_value Init(napi_env env, napi_value exports)
{
    OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "Init", "Init begins");
    if ((env == nullptr) || (exports == nullptr)) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "Init", "env or exports is null");
        return nullptr;
    }

    napi_property_descriptor desc[] = {
        {"copyAssets", nullptr, CopyAssets, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"createNativeNode", nullptr, PluginManager::createNativeNode, nullptr, nullptr, nullptr,
         napi_default, nullptr },
        {"getStatus", nullptr, PluginManager::GetXComponentStatus, nullptr, nullptr,
         nullptr, napi_default, nullptr},
        {"startRunner", nullptr, PluginManager::NapiStartRunner, nullptr, nullptr,
         nullptr, napi_default, nullptr}
    };
    if (napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc) != napi_ok) {
        OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "Init", "napi_define_properties failed");
        return nullptr;
    }
    
    return exports;
}
EXTERN_C_END

static napi_module nativenodeModule = { .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "nativenode",
    .nm_priv = ((void*)0),
    .reserved = { 0 } };

extern "C" __attribute__((constructor)) void RegisterModule(void)
{
    napi_module_register(&nativenodeModule);
}
} // namespace NativeXComponentSample
