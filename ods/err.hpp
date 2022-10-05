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
    #define MTL_COLOR_YELLOW    "\e[93m"
    #define MTL_COLOR_MAGENTA   "\e[35m"
    #define MTL_BLINK_START     "\e[5m"
    #define MTL_BLINK_END       "\e[25m"
    #define MTL_BOLD            "\e[1m"
    #define MTL_BOLD_END        "\x1B[0m"
	#define MTL_INVERTED		"\e[7m"
	#define MTL_INVERTED_CANCEL "\e[27m"
#else
	#define MTL_COLOR_BLUE		""
	#define MTL_COLOR_DEFAULT	""
	#define MTL_COLOR_GREEN		""
	#define MTL_COLOR_RED		""
    #define MTL_COLOR_YELLOW    ""
    #define MTL_COLOR_MAGENTA   ""
    #define MTL_BLINK_START     ""
    #define MTL_BLINK_END       ""
    #define MTL_BOLD            ""
    #define MTL_BOLD_END        ""
	#define MTL_INVERTED        ""
	#define MTL_INVERTED_CANCEL ""
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
	"%s[%s:%.3d %s]%s " fmt "\n", MTL_COLOR_BLUE, SRC_FILE_NAME, \
	__LINE__, __FUNCTION__, MTL_COLOR_DEFAULT, __VA_ARGS__)
#else
#define mtl_info(fmt, args...) fprintf(stdout, \
	"%s[%s:%.3d %s]%s " fmt "\n", MTL_COLOR_BLUE, SRC_FILE_NAME, \
	__LINE__, __FUNCTION__, MTL_COLOR_DEFAULT, ##args)
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
#define mtl_warn(fmt, ...) fprintf(stderr, \
	"%s[%s:%.3d %s] " fmt "%s\n", MTL_COLOR_RED, SRC_FILE_NAME, \
	__LINE__, __FUNCTION__, __VA_ARGS__, MTL_COLOR_DEFAULT)
#else
#define mtl_warn(fmt, args...) fprintf(stderr, \
	"%s[%s:%.3d %s] " fmt "%s\n", MTL_COLOR_RED, SRC_FILE_NAME, \
	__LINE__, __FUNCTION__, ##args, MTL_COLOR_DEFAULT)
#endif

#ifdef _MSC_VER
#define mtl_trace(fmt, ...) fprintf(stderr, \
	"%s[%s:%.3d %s] " fmt "%s\n", MTL_COLOR_GREEN, SRC_FILE_NAME, \
	__LINE__, __FUNCTION__, __VA_ARGS__, MTL_COLOR_DEFAULT)
#else
#define mtl_trace(fmt, args...) fprintf(stderr, \
	"%s%s[%s:%.3d %s]%s " fmt "%s\n", MTL_BOLD, MTL_COLOR_MAGENTA, SRC_FILE_NAME, \
	__LINE__, __FUNCTION__, MTL_BOLD_END, ##args, MTL_COLOR_DEFAULT)
#endif

#define mtl_status(status) fprintf (stderr, "%s[%s %.3d] %s%s\n", \
	MTL_COLOR_RED, SRC_FILE_NAME, \
	__LINE__, strerror(status), MTL_COLOR_DEFAULT)

#define mtl_tbd() {\
	mtl_trace("TBD");\
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

#define RET_IF_EQUAL(x, y) {\
	if (x == y) {\
		mtl_trace();\
		return false;\
	}\
}

#define RET_IF_EQUAL_VOID(x, y) {\
	if (x == y) {\
		mtl_trace();\
		return;\
	}\
}

#define RET_IF_EQUAL_NULL(x, y) {\
	if (x == y) {\
		mtl_trace();\
		return nullptr;\
	}\
}

#define CHECK_EQUAL(x, y) {\
	if (x != y) {\
		mtl_trace();\
		return false;\
	}\
}

#define CHECK_EQUAL_VOID(x, y) {\
	if (x != y) {\
		mtl_trace();\
		return;\
	}\
}

#define CHECK_EQUAL_NULL(x, y) {\
	if (x != y) {\
		mtl_trace();\
		return nullptr;\
	}\
}

#define CHECK_TRUE(x) {\
	if (!(x)) {\
		mtl_trace();\
		return false;\
	}\
}

#define CHECK_TRUE_NULL(x) {\
	if (!(x)) {\
		mtl_trace();\
		return nullptr;\
	}\
}

#define CHECK_TRUE_VOID(x) {\
	if (!(x)) {\
		mtl_trace();\
		return;\
	}\
}

#define CHECK_TRUE_QSTR(x) {\
	if (!(x)) {\
		mtl_trace();\
		return QString();\
	}\
}

#define CHECK_PTR(x) {\
	if (x == nullptr) {\
		mtl_trace();\
		return false;\
	}\
}

#define CHECK_PTR_VOID(x) {\
	if (x == nullptr) {\
		mtl_trace();\
		return;\
	}\
}

#define CHECK_PTR_NULL(x) {\
	if (x == nullptr) {\
		mtl_trace();\
		return nullptr;\
	}\
}
