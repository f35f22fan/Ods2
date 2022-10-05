#include "ndff.hh"

namespace ods::ndff {

bool CheckMagicNumber(char *data)
{
	auto &l = ndff::MagicNumber;
	uchar *r = (uchar*)data;
	
	return l[0] == r[0] && l[1] == r[1] && l[2] == r[2] && l[3] == r[3];
}


}
