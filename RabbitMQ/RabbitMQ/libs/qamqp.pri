##QAMQP_VERSION = 0.5.0

#isEmpty(QAMQP_LIBRARY_TYPE) {
#    QAMQP_LIBRARY_TYPE = shared
#}

QT += network
##QAMQP_INCLUDEPATH = $${PWD}
##QAMQP_LIBS = -lqamqp
##CONFIG(debug, debug|release){
##    QAMQP_LIBS = -lqamqpd
##}
#contains(QAMQP_LIBRARY_TYPE, staticlib) {
#    DEFINES += QAMQP_STATIC
#} else {
#    DEFINES += QAMQP_SHARED
##    win32:QAMQP_LIBS = -lqamqpd
##    win32{
##        CONFIG(debug, debug|release) {
##            QAMQP_LIBS = -lqamqpd
##        }
##        else{
##            QAMQP_LIBS = -lqamqp
##        }
##    }
#}

##isEmpty(PREFIX) {
##    unix {
##        PREFIX = /usr
##    } else {
##        PREFIX = $$[QT_INSTALL_PREFIX]
##    }
##}
##isEmpty(LIBDIR) {
##    LIBDIR = lib
##}

