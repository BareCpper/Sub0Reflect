cmake_minimum_required(VERSION 3.18) #FetchContent

include(CPM)

CPMAddPackage(
  NAME fmt
  GIT_TAG 9.0.0
  GITHUB_REPOSITORY fmtlib/fmt
)

if(fmt_ADDED)

	target_compile_options( fmt
	PRIVATE
		$<$<BOOL:${UNIX}>:-fPIC> # @todo Error "relocation R_X86_64_PC32 against symbol `stderr@@GLIBC_2.2.5' can not be used when making a shared object; recompile with -fPIC"
	)

endif()