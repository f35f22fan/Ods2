#pragma once

#include <errno.h>
#include <cstdio>
#include <QDebug>
#include "ods.hh"

#ifndef SRC_FILE_NAME
#define SRC_FILE_NAME __FILE__
#endif

// node => ods::FormulaNode*
#define mtl_print_node(node, msg) {\
	if (node == nullptr) {\
		mtl_info("%s [nullptr]", msg);\
	} else {\
		QString type_str, node_str;\
		function::NodeToStr(node, type_str, node_str);\
		auto type_ba = type_str.toLocal8Bit();\
		auto node_ba = node_str.toLocal8Bit();\
		mtl_info("%s [%s] %s%s%s", msg, type_ba.data(),\
		MTL_INVERTED, node_ba.data(), MTL_INVERTED_CANCEL);\
	}\
}

#ifdef _MSC_VER
#define mtl_info(fmt, ...) fprintf(stdout, \
    "%s[%s:%.3d %s]%s " fmt "\n", ods::color_blue(), SRC_FILE_NAME, \
    __LINE__, __FUNCTION__, ods::color_default(), ##__VA_ARGS__)
#else
#define mtl_info(fmt, args...) fprintf(stdout, \
    "%s[%s:%.3d %s]%s " fmt "\n", ods::color_blue(), SRC_FILE_NAME, \
    __LINE__, __FUNCTION__, ods::color_default(), ##args)
#endif

#ifdef _MSC_VER
#define mtl_it_happened(fmt, ...) fprintf(stdout, \
    "%sIT HAPPENED [%s %.3d] " fmt "%s\n", ods::color_red(), SRC_FILE_NAME, \
    __LINE__, ##__VA_ARGS__, ods::color_default())
#else
#define mtl_it_happened(fmt, args...) fprintf(stdout, \
    "%sIT HAPPENED [%s %.3d] " fmt "%s\n", ods::color_red(), SRC_FILE_NAME, \
    __LINE__, ##args, ods::color_default())
#endif

#ifdef _MSC_VER
#define mtl_warn(fmt, ...) fprintf(stderr, \
    "%s[%s:%.3d %s] " fmt "%s\n", ods::color_red(), SRC_FILE_NAME, \
    __LINE__, __FUNCTION__, ##__VA_ARGS__, ods::color_default())
#else
#define mtl_warn(fmt, args...) fprintf(stderr, \
    "%s[%s:%.3d %s] " fmt "%s\n", ods::color_red(), SRC_FILE_NAME, \
    __LINE__, __FUNCTION__, ##args, ods::color_default())
#endif

#ifdef _MSC_VER
#define mtl_trace(fmt, ...) fprintf(stderr, \
    "%s[%s:%.3d %s] " fmt "%s\n", ods::color_magenta(), SRC_FILE_NAME, \
    __LINE__, __FUNCTION__, ##__VA_ARGS__, ods::color_default())
#else
#define mtl_trace(fmt, args...) fprintf(stderr, \
    "%s%s[%s:%.3d %s]%s " fmt "%s\n", ods::color_bold_start(), ods::color_magenta(), SRC_FILE_NAME, \
    __LINE__, __FUNCTION__, ods::color_bold_end(), ##args, ods::color_default())
#endif

#define mtl_status(status) fprintf (stderr, "%s[%s %.3d] %s%s\n", \
	ods::color_red(), SRC_FILE_NAME, \
    __LINE__, strerror(status), ods::color_default())

#define mtl_tbd() {\
	mtl_trace("====================TBD");\
}

#define mtl_printq(s) {\
	auto ba = s.toLocal8Bit();\
	mtl_info("%s", ba.data());\
}

#define mtl_printq2(msg, s) {\
	auto ba = s.toLocal8Bit();\
	mtl_info("%s\"%s\"", msg, ba.data());\
}

#define mtl_print_error(node) {\
	if (node->is_error()) {\
		auto ba = node->as_error()->description().toLocal8Bit();\
		mtl_warn("%s", ba.data());\
	}\
}

#define NO_ASSIGN_COPY_MOVE(TypeName)	\
	TypeName(const TypeName&) = delete;		\
	void operator=(const TypeName&) = delete; \
	TypeName(TypeName&&) = delete;

#define NO_MOVE(TypeName)	\
	TypeName(TypeName&&) = delete;

#define MTL_CHECK(flag) {\
	if (!(flag)) {\
		mtl_trace();\
		return false;\
	}\
}

#define MTL_CHECK_VOID(flag) {\
	if (!(flag)) {\
		mtl_trace();\
		return;\
	}\
}

#define MTL_CHECK_NULL(flag) {\
	if (!(flag)) {\
		mtl_trace();\
		return nullptr;\
	}\
}

#define MTL_CHECK_ARG(flag, ret) {\
	if (!(flag)) {\
		mtl_trace();\
		return ret;\
	}\
}


