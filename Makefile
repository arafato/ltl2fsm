
################################################################################
#
# BUILD ENVIRONMENT CONFIGURATION
#
################################################################################

ENV_LIB_ROOT=../../env

include $(ENV_LIB_ROOT)/makefiles/framework/Makefile.preludium

################################################################################
#
# PACKAGE CONFIG
#
################################################################################

include Makefile.config.package

################################################################################
#
# BUILD ENVIRONMENT LOADING
#
################################################################################

include $(ENV_LIB_ROOT)/makefiles/framework/Makefile.postludium

################################################################################
#
# MODULES CONFIG
#
################################################################################

include Makefile.config.modules


# Local Variables:
# mode:makefile
# End:
