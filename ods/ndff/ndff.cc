#include "ndff.hh"

namespace ods::ndff {

bool CheckMagicNumber(char *data)
{
	cu8 *l = ndff::MagicNumber;
	uchar *r = (uchar*)data;
	
	return l[0] == r[0] && l[1] == r[1] && l[2] == r[2] && l[3] == r[3];
}

const char* ops(const Op op) {
	switch (op) {
	case F64:     return "{F64}";
	case S8:      return "{S8 0-15}";
	case S16:     return "{S16 0-4095}";
	case S32_PS:  return "{PS or S32 0–268435455}";
	case S64:     return "{S64 60 bits}";
	case N8:      return "{N 0-15}";
	case N16:     return "{N 0-4095}";
	case N32_TE:  return "{N 0-268435455 or />}";
	case N64:     return "{N 64Bits)";
	case TS:      return "{<}";
	case TCF_CMS: return "{>}";
	case SCT:     return "{</>}";
	case FS8:     return "{FreeSpace8Bit}";
	case FS64:    return "{FreeSpace64Bit}";
	case JB64:    return "{JumpBy64Bit unsigned}";
	case None:    return "{None}";
	default:      return "{UnknownTag}";
	}
	
}


}
