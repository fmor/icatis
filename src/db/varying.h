#pragma once


template<short L>
class Varying
{
public:
	short length;
	char str[L];

	inline Varying() :
		length(L)
	{}
};


#define COPY_VARYING( dst, var ) memcpy( dst, var.str, var.length ); dst[var.length] = 0
