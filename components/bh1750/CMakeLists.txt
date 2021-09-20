idf_component_register(SRCS "src/bh1750.c"
					INCLUDE_DIRS "include"
 					REQUIRES i2cdev log esp_idf_lib_helpers)