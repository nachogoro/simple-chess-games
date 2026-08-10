#ifndef EXPORT_H_9F2C6B71_4E58_4A0D_B3C5_7A1D8E93F206
#define EXPORT_H_9F2C6B71_4E58_4A0D_B3C5_7A1D8E93F206

/**
 * \file Export.h
 * \brief Marks what a shared build of the library makes visible.
 *
 * Everything else is hidden, so that the internals under src/core/details
 * are not part of what a program links against, and so that a Windows build
 * exports the same set as every other platform rather than leaving it to
 * whatever the linker decides to guess.
 *
 * A declaration is in one of three situations, and all three have to be
 * spelled out: Windows needs a definition marked for export and a use
 * marked for import, and objects the loader will find in their own binary
 * marked as neither.
 *
 * The two APIs are resolved separately, because they are not shipped
 * together: the C library carries its own copy of the engine so as to be
 * self-contained, and has no reason to publish the C++ API on top of its
 * own. Inside that library the C++ API is the third case - it is right
 * there, and nobody outside is meant to reach it.
 *
 * The defines are set by the build:
 *   SIMPLECHESS_STATIC     nothing is exported and nothing is imported.
 *   SIMPLECHESS_BUILDING   this is the shared C++ library being compiled.
 *   SIMPLECHESS_C_BUILDING this is the shared C library being compiled.
 */

#if defined(SIMPLECHESS_STATIC)
#	define SIMPLECHESS_EXPORTED
#	define SIMPLECHESS_IMPORTED
#elif defined(_WIN32) || defined(__CYGWIN__)
#	define SIMPLECHESS_EXPORTED __declspec(dllexport)
#	define SIMPLECHESS_IMPORTED __declspec(dllimport)
#else
	// Elsewhere only the definition is marked; a use resolves through the
	// dynamic symbol table with nothing said about it. Everything unmarked
	// is hidden, because the build compiles with -fvisibility=hidden.
#	define SIMPLECHESS_EXPORTED __attribute__((visibility("default")))
#	define SIMPLECHESS_IMPORTED
#endif

// The C++ API.
#if defined(SIMPLECHESS_BUILDING)
#	define SIMPLECHESS_API SIMPLECHESS_EXPORTED
#elif defined(SIMPLECHESS_C_BUILDING)
	// Being compiled into the C library, which holds the engine without
	// publishing it. Marking these for import would be wrong twice over:
	// they are defined in this same binary, and nothing outside it is
	// supposed to find them.
#	define SIMPLECHESS_API
#else
#	define SIMPLECHESS_API SIMPLECHESS_IMPORTED
#endif

// The C API.
#if defined(SIMPLECHESS_C_BUILDING)
#	define SIMPLECHESS_C_API SIMPLECHESS_EXPORTED
#else
#	define SIMPLECHESS_C_API SIMPLECHESS_IMPORTED
#endif

#endif
