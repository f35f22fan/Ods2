#pragma once

#include <errno.h>
#include <cstdio>
#include <QDebug>

#ifndef SRC_FILE_NAME
#define SRC_FILE_NAME __FILE__
#endif

#ifdef __unix__
	#define MTL_COLOR_BLUE		"\x1B[34m"
	#define MTL_COLOR_DEFAULT	"\x1B[0m"
	#define MTL_COLOR_GREEN		"\x1B[32m"
	#define MTL_COLOR_RED		"\e[0;91m"
#else
	#define MTL_COLOR_BLUE		""
	#define MTL_COLOR_DEFAULT	""
	#define MTL_COLOR_GREEN		""
	#define MTL_COLOR_RED		""
#endif

#ifdef _MSC_VER
#define mtl_line(fmt, ...) fprintf(stdout, \
	"%s[%s %s %.3d]%s " fmt "\n", MTL_COLOR_BLUE, SRC_FILE_NAME, \
	__FUNCTION__, __LINE__, MTL_COLOR_DEFAULT, __VA_ARGS__)
#else
#define mtl_line(fmt, args...) fprintf(stdout, \
	"%s[%s %s %.3d]%s " fmt "\n", MTL_COLOR_BLUE, SRC_FILE_NAME, \
	__FUNCTION__, __LINE__, MTL_COLOR_DEFAULT, ##args)
#endif

#ifdef _MSC_VER
#define fix_me(fmt, ...) fprintf(stdout, \
	"%sFIXME [%s %.3d] " fmt "%s\n", MTL_COLOR_BLUE, SRC_FILE_NAME, \
	__LINE__, __VA_ARGS__, MTL_COLOR_DEFAULT)
#else
#define fix_me(fmt, args...) fprintf(stdout, \
	"%sFIXME [%s %.3d] " fmt "%s\n", MTL_COLOR_BLUE, SRC_FILE_NAME, \
	__LINE__, ##args, MTL_COLOR_DEFAULT)
#endif

#ifdef _MSC_VER
#define it_happened(fmt, ...) fprintf(stdout, \
	"%sIT HAPPENED [%s %.3d] " fmt "%s\n", MTL_COLOR_RED, SRC_FILE_NAME, \
	__LINE__, __VA_ARGS__, MTL_COLOR_DEFAULT)
#else
#define it_happened(fmt, args...) fprintf(stdout, \
	"%sIT HAPPENED [%s %.3d] " fmt "%s\n", MTL_COLOR_RED, SRC_FILE_NAME, \
	__LINE__, ##args, MTL_COLOR_DEFAULT)
#endif

#ifdef _MSC_VER
#define tbd(fmt, ...) fprintf(stdout, \
	"%sTBD [%s %.3d] " fmt "%s\n", MTL_COLOR_BLUE, SRC_FILE_NAME, \
	__LINE__, __VA_ARGS__, MTL_COLOR_DEFAULT)
#else
#define tbd(fmt, args...) fprintf(stdout, \
	"%sTBD [%s %.3d] " fmt "%s\n", MTL_COLOR_BLUE, SRC_FILE_NAME, \
	__LINE__, ##args, MTL_COLOR_DEFAULT)
#endif

#ifdef _MSC_VER
#define mtl_warn(fmt, ...) fprintf(stderr, \
	"%s[Warning %s %.3d] " fmt "%s\n", MTL_COLOR_RED, SRC_FILE_NAME, \
	__LINE__, __VA_ARGS__, MTL_COLOR_DEFAULT)
#else
#define mtl_warn(fmt, args...) fprintf(stderr, \
	"%s[Warning %s %.3d] " fmt "%s\n", MTL_COLOR_RED, SRC_FILE_NAME, \
	__LINE__, ##args, MTL_COLOR_DEFAULT)
#endif

#define mtl_status(status) fprintf (stderr, "%s[%s %.3d] %s%s\n", \
	MTL_COLOR_RED, SRC_FILE_NAME, \
	__LINE__, strerror(status), MTL_COLOR_DEFAULT)

#define mtl_errno fprintf (stderr, "[%s %.3d] %m\n", __LINE__, __FUNCTION__)

#define mtl_qline(qstr) qDebug().nospace() << MTL_COLOR_BLUE << "[" << \
	SRC_FILE_NAME << ' ' << __FUNCTION__ << ' ' << __LINE__ << "] " << \
	qstr << MTL_COLOR_DEFAULT

#define NO_ASSIGN_COPY_MOVE(TypeName)	\
	TypeName(const TypeName&) = delete;		\
	void operator=(const TypeName&) = delete; \
	TypeName(TypeName&&) = delete;

#define NO_MOVE(TypeName)	\
	TypeName(TypeName&&) = delete;

