idf_component_register(SRCS "src/app_rainmaker.c"
					"src/standard_types/ioc_standard_params.c"
					"src/standard_types/ioc_standard_devices.c"
					INCLUDE_DIRS "include"
					REQUIRES esp_rainmaker)