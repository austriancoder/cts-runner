#include "runner.h"

#include "VK-GL-CTS/external/openglcts/modules/glcTestPackageRegistry.hpp"
#include "VK-GL-CTS/framework/qphelper/qpDebugOut.h"

#include "tcuDefs.hpp"
#include "tcuCommandLine.hpp"
#include "tcuPlatform.hpp"
#include "tcuApp.hpp"
#include "tcuResource.hpp"
#include "tcuTestLog.hpp"
#include "tcuTestSessionExecutor.hpp"
#include "deUniquePtr.hpp"
#include "qpDebugOut.h"

tcu::Platform *createPlatform(void);

namespace NativeXComponentSample {
static void run(const char *result, const char *test) {
    int exitStatus = EXIT_SUCCESS;
    
    const char *argv[] = {
        "runner",
        result,
        test,
        "--deqp-archive-dir=/data/storage/el2/base/files/",
        "--deqp-surface-type=pbuffer",
        "--deqp-surface-width=256",
        "--deqp-surface-height=256",
        "--deqp-log-images=enable",
        "--deqp-crashhandler=enable",
        "--deqp-gl-config-name=rgba8888d24s8ms0",
    };
    
    const int argc = DE_LENGTH_OF_ARRAY(argv);
    
    for (int i = 0; i < argc; i++)
        qpPrintf("%s", argv[i]);
    
    try
    {
        tcu::CommandLine cmdLine(argc, argv);
        
        tcu::DirArchive archive(cmdLine.getArchiveDir());
        tcu::TestLog log(cmdLine.getLogFileName(), cmdLine.getLogFlags());
        de::UniquePtr<tcu::Platform> platform(createPlatform());
        de::UniquePtr<tcu::App> app(new tcu::App(*platform, archive, log, cmdLine));

        // Main loop.
        for (;;)
        {
            if (!app->iterate())
            {
                if (cmdLine.getRunMode() == tcu::RUNMODE_EXECUTE &&
                    (!app->getResult().isComplete || app->getResult().numFailed))
                {
                    exitStatus = EXIT_FAILURE;
                }

                break;
            }
        }
    }
    catch (const std::exception &e)
    {
        tcu::die("%s", e.what());
    }
} 

void Runner::start()
{
    // setup mesa for our needs
    setenv("MESA_LOADER_DRIVER_OVERRIDE", "zink", 1);
    setenv("EGL_LOG_LEVEL", "debug", 1);
    setenv("MESA_LOG", "ohos", 1);
    //setenv("ZINK_DEBUG", "spirv,nir", 1);
    setenv("MESA_GL_VERSION_OVERRIDE", "4.6", 1);
    
    glcts::registerPackages();

    //run("--deqp-log-filename=/data/storage/el2/base/files/result_gl30.qpa","--deqp-case=KHR-GL30.*");
    //run("--deqp-log-filename=/data/storage/el2/base/files/result_gl31.qpa","--deqp-case=KHR-GL31.*");
    //run("--deqp-log-filename=/data/storage/el2/base/files/result_gl32.qpa","--deqp-case=KHR-GL32.*");
    //run("--deqp-log-filename=/data/storage/el2/base/files/result_gl33.qpa","--deqp-case=KHR-GL33.*");
    //run("--deqp-log-filename=/data/storage/el2/base/files/result_gl40.qpa","--deqp-case=KHR-GL40.*");
    //run("--deqp-log-filename=/data/storage/el2/base/files/result_gl46.qpa","--deqp-case=KHR-GL46.*");
    
    run("--deqp-log-filename=/data/storage/el2/base/files/KHR-GL40.texture_gather.gather-tesselation-shader.qpa","--deqp-case=KHR-GL40.texture_gather.gather-tesselation-shader");
}
}