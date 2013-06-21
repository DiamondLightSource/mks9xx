TOP=../..

include $(TOP)/configure/CONFIG

# -------------------------------
# Build an Diamond Support Module
# -------------------------------

#LIBRARY_IOC += xxx

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxx.h

# xxxRecord.dbd will be installed into <top>/dbd
#DBD += xxx.dbd

# The following are compiled and added to the support library
#xxx_SRCS += xxxCodeA.c
#xxx_SRCS += xxxCodeB.c

# We need to link against the EPICS Base libraries
#xxx_LIBS += $(EPICS_BASE_IOC_LIBS)


# ------------------------
# Build an IOC Application
# ------------------------

PROD_IOC = mks9xx

# mks9xx.dbd will be installed into <top>/dbd
DBD += mks9xx.dbd

# mks9xx.dbd will be created from these files
mks9xx_DBD += base.dbd
#mks9xx_DBD += xxx.dbd

# mks9xx_registerRecordDeviceDriver.cpp will be created
# mks9xx.dbd
mks9xx_SRCS += mks9xx_registerRecordDeviceDriver.cpp

# These two lines are needed for non-vxWorks builds, such as Linux
mks9xx_SRCS_DEFAULT += mks9xxMain.cpp
mks9xx_SRCS_vxWorks += -nil-

# Add locally compiled object code
#mks9xx_SRCS +=

# The following adds object code from base/src/vxWorks
mks9xx_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# This line says that this IOC Application depends on the
# xxx Support Module
#mks9xx_LIBS += xxx

# We need to link this IOC Application against the EPICS Base libraries
mks9xx_LIBS += $(EPICS_BASE_IOC_LIBS)

# ---------------------------------------------------

# NOTE: To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file
#
# The difference between a file with an "st" extension
# and one with an "stt" extension is:
# The "st" extension files get passed through the C preprocessor
# whereas the "stt" files do not.

#ifneq ($(SNCSEQ),)
    # This builds sncExample as a component of mks9xx
#    mks9xx_SNCFLAGS += +r
#    mks9xx_DBD      += sncExample.dbd
#    mks9xx_SRCS     += sncExample.stt
#    mks9xx_LIBS     += seq pv

    # The following builds sncExample as a standalone application
#    PROD_HOST           += sncExample
#    sncExample_SNCFLAGS += +m
#    sncExample_SRCS     += sncExample.stt
#    sncExample_LIBS     += seq pv
    # Here, we need to specify the dependency on EPICS Base libraries
    # again because this is a standalone application and
    # not part of mks9xx
#    sncExample_LIBS     += $(EPICS_BASE_HOST_LIBS)
#endif

include $(TOP)/configure/RULES
