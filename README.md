# Icatis

This is a simple training purpose project for reusing the
vcl api and windows OS. It use firebird c api to connect
to database, so no need to have more than a free editioon
of c++ builder. This is a french language application. 

# Requirements

* C++ Builder Starter edition
* Firebird SQL >= 2.5 ( older version may work )

# Compilation

This project relies on Firebird database and need to
have static library fbclient_bor.lib available for the
linker.

On 32 bits and with defautl firebird install, this library
can be found here :

	C:\Program Files (x86)\Firebird\Firebird_2_5\lib

