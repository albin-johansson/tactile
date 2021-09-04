#pragma once

#ifdef NDEBUG
#define TACTILE_DEBUG 0
#else
#define TACTILE_DEBUG 1
#endif  // NDEBUG

#if TACTILE_DEBUG
#define TACTILE_RELEASE 0
#else
#define TACTILE_RELEASE 1
#endif  // TACTILE_DEBUG