option(BUILD_TESTS          "build tests from the 'test' directory"    ON)
option(BUILD_EXAMPLES       "build examples from 'examples' directory" ON)
option(BUILD_WITH_GL_CHECKS "enable error checking for OpenGL calls"   ON)
option(BUILD_WITH_AL_CHECKS "enable error checking for OpenAL calls"   ON)
option(BUILD_WITH_AL_DL     "use runtime dynamic linking for OpenAL"   ON)
option(USE_PKGCONFIG        "use pkg-config to find libraries"         ON)

set(MAX_STEPS_PER_TICK "" CACHE STRING "maximum number of animation \
sequence steps in a single tick before an error is raised; use a positive \
number to specify the number of steps, 0 to disable and leave empty for the \
default, which is 1000 in debug mode and disabled in release mode")
