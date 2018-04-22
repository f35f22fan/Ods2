#ifndef QUAZIP_GLOBAL_H
#define QUAZIP_GLOBAL_H

/*
Copyright (C) 2005-2014 Sergey A. Tachenov

This file is part of QuaZIP.

QuaZIP is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

QuaZIP is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with QuaZIP.  If not, see <http://www.gnu.org/licenses/>.

See COPYING file for the full LGPL text.

Original ZIP package is copyrighted by Gilles Vollant and contributors,
see quazip/(un)zip.h files for details. Basically it's the zlib license.
*/

#include <QtCore/qglobal.h>

#ifdef QUAZIP_SHARED
	#if defined(QUAZIP_BUILD)
		#define QUAZIP_EXPORT Q_DECL_EXPORT
	#else
		#define QUAZIP_EXPORT Q_DECL_IMPORT
	#endif
#else
	#define QUAZIP_EXPORT 
#endif

#ifdef __GNUC__
#define UNUSED __attribute__((__unused__))
#else
#define UNUSED
#endif

#define QUAZIP_EXTRA_NTFS_MAGIC 0x000Au
#define QUAZIP_EXTRA_NTFS_TIME_MAGIC 0x0001u

#endif // QUAZIP_GLOBAL_H
