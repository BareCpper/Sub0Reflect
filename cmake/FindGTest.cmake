cmake_minimum_required(VERSION 3.18) #FetchContent

include(CPM)

CPMAddPackage(
	NAME googletest
	GITHUB_REPOSITORY google/googletest
	GIT_TAG release-1.12.1
	VERSION 1.12.1
	EXCLUDE_FROM_ALL true
	OPTIONS 
		"INSTALL_GTEST OFF" 
		"gtest_force_shared_crt"
		"gtest_disable_pthreads" # @note For Context&Device tests we do not want to be running parallel!
)


if(googletest_ADDED)

	target_compile_options( gtest
	PRIVATE
		$<$<BOOL:${ANDROID}>:-fPIC> # @todo Error "relocation R_AARCH64_ADR_PREL_PG_HI21 cannot be used against symbol testing::FLAGS_gtest_fail_fast"
	)

	include(GoogleTest)

	# Alias same as findGTest()
	add_library( GTest::Main ALIAS gtest_main )
	add_library( GTest::GTest ALIAS gtest )

	add_library( GMock::Main ALIAS gmock_main )
	add_library( GMock::GMock ALIAS gmock )
endif()