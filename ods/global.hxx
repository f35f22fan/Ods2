/*
 * Copyright © 2014 f35f22fan@gmail.com
 *
 * Read the LICENSE file that comes with this project for license details.
*/

#ifndef ODS_GLOBAL_HXX_
#define ODS_GLOBAL_HXX_

#include <QtCore/QtGlobal>

#ifdef ODS_SHARED
	#if defined (ODS_BUILDING)
		#define ODS_API Q_DECL_EXPORT
	#else
		#define ODS_API Q_DECL_IMPORT
	#endif
#else
	#define ODS_API 
#endif

#endif
