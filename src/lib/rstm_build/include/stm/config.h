/**
 *  Copyright (C) 2011
 *  University of Rochester Department of Computer Science
 *    and
 *  Lehigh University Department of Computer Science and Engineering
 *
 * License: Modified BSD
 *          Please see the file LICENSE.RSTM for licensing information
 */

/**
 *  This is the configured header for libstm. It records all of the
 *  configuration parameters, and should be included before anything that
 *  depends on a #define that is prefixed with 'STM'
 */

#ifndef RSTM_STM_INCLUDE_CONFIG_H
#define RSTM_STM_INCLUDE_CONFIG_H

// Target processor architecture
#define STM_CPU_X86
/* #undef STM_CPU_SPARC */

// Target compiler
#define STM_CC_GCC
/* #undef STM_CC_SUN */
/* #undef STM_CC_LLVM */

// Target OS
#define STM_OS_LINUX
/* #undef STM_OS_SOLARIS */
/* #undef STM_OS_MACOS */
/* #undef STM_OS_WINDOWS */

// The kind of build we're doing
#define STM_O3
/* #undef STM_O0 */
/* #undef STM_PG */

// Histogram generation
/* #undef STM_COUNTCONSEC_YES */

// ProfileTMtrigger
#define STM_PROFILETMTRIGGER_ALL
/* #undef STM_PROFILETMTRIGGER_PATHOLOGY */
/* #undef STM_PROFILETMTRIGGER_NONE */

// Configured thread-local-storage model
#define STM_TLS_GCC
/* #undef STM_TLS_PTHREAD */

// Configured logging granularity
#define STM_WS_WORDLOG
/* #undef STM_WS_BYTELOG */
#define STM_USE_WORD_LOGGING_VALUELIST

// Configured options
/* #undef STM_PROTECT_STACK */
/* #undef STM_ABORT_ON_THROW */

// Defined when we want to optimize for SSE execution
#define STM_USE_SSE

#endif // RSTM_STM_INCLUDE_CONFIG_H
