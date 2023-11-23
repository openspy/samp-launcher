/*
	header file to include TWL precompile header file
*/
#define SDK_TS_VERSION 400

#if !__option (precompile)

	#ifdef	__cplusplus
		/* include precompiled header for C++ source */
		#ifdef	SDK_DEBUG
			#include "Twl_TS_Debug.mch++"		/* If a compile error occurs in this line, please precompile "Twl_TS.pch++" */
		#elif		SDK_RELEASE
			#include "Twl_TS_Release.mch++"	/* If a compile error occurs in this line, please precompile "Twl_TS.pch++" */
		#elif		SDK_FINALROM
			#include "Twl_TS_ROM.mch++"		/* If a compile error occurs in this line, please precompile "Twl_TS.pch++" */
		#else
			#error("no SDK_ define")
		#endif
	#else
		/* include precompiled header for C source */
		#ifdef	SDK_DEBUG
			#include "Twl_TS_Debug.mch"		/* If a compile error occurs in this line, please precompile "Twl_TS.pch" */
		#elif		SDK_RELEASE
			#include "Twl_TS_Release.mch"		/* If a compile error occurs in this line, please precompile "Twl_TS.pch" */
		#elif		SDK_FINALROM
			#include "Twl_TS_ROM.mch"			/* If a compile error occurs in this line, please precompile "Twl_TS.pch" */
		#else
			#error("no SDK_ define")
		#endif
	#endif

#endif
