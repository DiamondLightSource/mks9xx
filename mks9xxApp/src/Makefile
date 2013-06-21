TOP=../..

include $(TOP)/configure/CONFIG

# -------------------------------
# Build an Diamond Support Module
# -------------------------------

LIBRARY_IOC += mks9xx

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxx.h

# xxxRecord.dbd will be installed into <top>/dbd
DBD += mks9xx.dbd

# The following are compiled and added to the support library
mks9xx_SRCS += mks9xx.cpp

# We need to link against the EPICS Base libraries
mks9xx_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
