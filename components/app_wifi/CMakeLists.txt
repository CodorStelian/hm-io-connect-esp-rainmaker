idf_component_register(SRCS "src/app_wifi.c"
					INCLUDE_DIRS "include"
					REQUIRES wifi_provisioning qrcode)

if(CONFIG_APP_WIFI_SHOW_DEMO_INTRO_TEXT)
    target_compile_definitions(${COMPONENT_TARGET} PRIVATE "-D RMAKER_DEMO_PROJECT_NAME=\"${CMAKE_PROJECT_NAME}\"")
endif()