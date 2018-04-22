#pragma once

#include <cstdint>

namespace ods {
namespace style {

const uint8_t BorderSideLeft	= 1;
const uint8_t BorderSideTop		= 1 << 1;
const uint8_t BorderSideRight	= 1 << 2;
const uint8_t BorderSideBottom	= 1 << 3;
const uint8_t BorderSideAll		= 
	BorderSideLeft | BorderSideTop | BorderSideRight | BorderSideBottom;

enum class BorderStyle : uint8_t
{
	Dashed,
	Dotted,
	Double,
	Inset,
	None,
	Outset,
	Solid
};

enum class BorderWidth : uint8_t
{
	Medium,
	Thick,
	Thin
};

enum class FontSizeType : uint8_t
{
	Pt,
	In,
	Cm,
	None
};

enum class FontStyle : uint8_t
{
	Normal,
	Italic,
	Oblique
};

enum class FontWeight : uint8_t
{
	Normal,
	Bold,
	N100,
	N200,
	N300,
	N400,
	N500,
	N600,
	N700,
	N800,
	N900
};

enum class Family : uint8_t
{
	Cell,
	Column,
	Row,
	Table,
	None
};

}} // ods::style::