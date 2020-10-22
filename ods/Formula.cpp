#include "Formula.hpp"

#include "Book.hpp"
#include "Cell.hpp"
#include "Duration.hpp"
#include "ods.hh"
#include "Row.hpp"
#include "Sheet.hpp"

#include "inst/OfficeSpreadsheet.hpp"

#include "formula/CellRef.hpp"
#include "formula/Node.hpp"
#include "formula/Value.hpp"

namespace ods { // ods::

Formula::Formula(Cell *cell) : cell_(cell), str_(formula::Prefix) {}

Formula::~Formula() {}

Formula*
Formula::Clone(ods::Cell *cell) const
{
	auto *pc = (cell != nullptr) ? cell : cell_;
	auto *p = new Formula(pc);
	p->str_ = str_;
	return p;
}

void
Formula::Add(ods::Cell *cell, ods::Sheet *sheet)
{
	if (sheet == nullptr) {
		str_ += QLatin1String("[.");
	} else {
		str_ += QLatin1String("[\'") + sheet->name()
			+ QLatin1String("\'.");
	}
	
	str_ += cell->QueryAddress();
	str_ += QChar(']');
}

void
Formula::Add(const double d)
{
	str_ += QString::number(d);
}

void
Formula::Add(const ods::Grouping g)
{
	if (g == ods::Grouping::Open)
		str_ += QChar('(');
	else
		str_ += QChar(')');
}

void
Formula::Add(const ods::Op op)
{
	str_ += ods::CharFromOp(op);
}

formula::CellRef*
Formula::CreateCellRef(const QString &address)
{
	if (address.isEmpty())
	{
		error_ = value::Error::BadFormulaString;
		return nullptr;
	}
	
	int dot = address.indexOf('.');
	
	if (dot == -1)
	{
		error_ = value::Error::BadFormulaString;
		return nullptr;
	}
	
	auto sheet_name = address.left(dot);
	
	if (sheet_name.startsWith('$'))
	{
		sheet_name = sheet_name.mid(1);
		mtl_warn();
	}
	
	auto *sp = cell_->book()->spreadsheet();
	auto *sheet = sp->GetSheet(sheet_name);
	
	if (sheet == nullptr)
		sheet = cell_->row()->sheet();
	
	auto cell_name = address.mid(dot + 1);
	int index = -1;
	const int count = cell_name.size();
	bool found = false;
	
	for (int i = 0; i < count; i++)
	{
		QChar c = cell_name.at(i);
		index++;
		
		if (c.isDigit())
		{
			found = true;
			break;
		}
	}
	
	if (!found)
	{
		error_ = value::Error::BadFormulaString;
		return nullptr;
	}
	
	auto letters = cell_name.leftRef(index);
	const int col = ods::ColumnLettersToNumber(letters);
	
	auto digits = cell_name.rightRef(count - index);
	bool ok;
	const int row = digits.toInt(&ok);
	
	if (!ok)
	{
		error_ = value::Error::BadFormulaString;
		return nullptr;
	}
	
	return formula::CellRef::New(sheet, row, col);
}

void
Formula::CreateNodes(const QString &str, QVector<formula::Node*> &vec)
{
	//auto ba = str.toLocal8Bit();
	//mtl_line("str: %s", ba.data()); // "([.A0])+[.B0]"
	const int count = str.size();
	int start = -1;
	auto current_type = formula::node::Type::None;
	
	for (int i = 0; i < count; i++)
	{
		QChar c = str.at(i);
		
		if (current_type == formula::node::Type::CellRef)
		{
			if (c == ']')
			{
				current_type = formula::node::Type::None;
				QString address = str.mid(start, i - start);
				auto *ref = CreateCellRef(address);
				
				if (has_error())
				{
					mtl_warn();
					return;
				}
				
				vec.append(formula::Node::New(ref));
			}
		} else if (c == '[') {
			current_type = formula::node::Type::CellRef;
			start = i + 1;
		} else if (c == '(') {
			vec.append(formula::Node::New(ods::Grouping::Open));
		} else if (c == ')') {
			vec.append(formula::Node::New(ods::Grouping::Close));
		} else if (CharToOp(c) != ods::Op::None) {
			vec.append(formula::Node::New(CharToOp(c)));
		} else if (c.isSpace()) {
			current_type = formula::node::Type::None;
			continue;
		} else {
			formula::Value value;
			int skip = SeekValue(str, i, value);//////////////
			
			if (value.has_error())
			{
				mtl_warn();
				return;
			}
			
			i += skip;
			vec.append(formula::Node::New(value));
		}
	}
}

bool
Formula::EnclosedInParentheses(QVector<formula::Node*> &vec, const int start,
	int *real_start, int *real_end) const
{
	int pre = start - 1;
	
	if (pre < 0)
		return false;
	
	if (pre >= vec.size())
		return false;
	
	formula::Node *pre_node = vec[pre];
	
	if (!pre_node->is_grouping())
		return false;
	
	if (!pre_node->is_open_grouping())
		return false;
	
	
	int how_many = 0;
	
	while (true)
	{
		int after_index = start + 3 + how_many;
		
		if (after_index >= vec.size())
			break;
		
		formula::Node *after = vec[after_index];
		
		if (after->is_grouping() && after->is_close_grouping())
			how_many++;
		else
			break;
	}
	
	
	if (real_start != nullptr)
		*real_start = start - how_many;
	
	if (real_end != nullptr)
		*real_end = start + 3 + how_many;
	
	return (how_many != 0);
}

void
Formula::Eval(ods::formula::Value &result)
{
	result.Clear();
	QString s = str_;
	
	if (str_.startsWith(ods::formula::Prefix))
		s = s.mid(formula::Prefix.size());
	
	QVector<formula::Node*> vec;
	CreateNodes(s, vec);
	
	if (has_error())
	{
		mtl_warn();
		return;
	}
	
	while (vec.size() > 1)
	{
		EvalDeepest(vec, result);
		
		if (result.has_error())
		{
			mtl_warn();
			return;
		}
	}
	
	QString result_type;
	
	if (result.is_double())
	{
		result_type = "[Double] ";
		result_type.append(QString::number(*result.as_double()));
	} else {
		it_happened();
	}
	
//	auto ba = result_type.toLocal8Bit();
//	mtl_line("Formula Value: %s", ba.data());
}

void
Formula::Eval3Nodes(QVector<formula::Node*> &vec, const int start, formula::Value &result)
{
	ods::Op last_op = ods::Op::None;
	const int end = start + 3;
	
	for (int i = start; i < end; i++)
	{
		auto *p = vec[i];
		
		if (p->is_cell_ref())
		{
			if (last_op == ods::Op::None)
			{
				result.SetValue(p->as_cell_ref());
				continue;
			}
			
			result.Operation(p->as_cell_ref(), last_op);
			
			if (result.has_error())
			{
				mtl_warn();
				return;
			}
		} else if (p->is_value()) {
			if (last_op == ods::Op::None)
			{
				result.SetValue(p->as_value());
				continue;
			}
			
			result.Operation(p->as_value(), last_op);
			
			if (result.has_error())
			{
				mtl_warn();
				return;
			}
		} else if (p->is_op()) {
			last_op = *p->as_op();
		} else {
			it_happened();
			result.error(value::Error::BadEvaluation);
			return;
		}
	}
	
	int real_start = start;
	int real_end = end;
	EnclosedInParentheses(vec, start, &real_start, &real_end);
	
	for (int i = real_start; i < real_end; i++)
	{
		delete vec[i];
	}
	
	vec.remove(real_start, real_end - real_start);
	vec.insert(real_start, formula::Node::New(result));
}

void
Formula::EvalDeepest(QVector<formula::Node*> &vec, formula::Value &result)
{
	//PrintNodes(vec);
	int start, end;
	bool has_parentheses;
	SeekInParentheses(vec, start, end, has_parentheses);
	
	if (has_parentheses)
	{
		if (start == end)
		{
			mtl_warn();
			// just empty parentheses, remove them and go return.
			for (int i = start - 1; i <= end; i++)
			{
				delete vec[i];
			}
			
			vec.remove(start - 1, 2);
			return;
		}
	}
	
	if (start >= end)
	{
		mtl_warn("start: %d, end: %d, vec size: %d", start, end, vec.size());
		result.error(value::Error::BadEvaluation);
		return;
	}
	
	Op highest_op = Op::None;
	int highest_op_index = -1;
	
	for (int i = start; i <= end; i++)
	{
		auto *node = vec[i];
		
		if (!node->is_op())
			continue;
		
		Op op = *node->as_op();
		
		if (ods::higher_priority(op, highest_op))
		{
			highest_op = op;
			highest_op_index = i;
		}
	}
	
	if (highest_op_index == -1)
	{
		mtl_warn();
		result.error(value::Error::BadEvaluation);
		return;
	}
	
	// it autodeletes the nodes and parentheses if any.
	Eval3Nodes(vec, highest_op_index - 1, result); 
}

Formula*
Formula::FromString(const QString &s, Cell *parent)
{
	if (s.isEmpty())
		return nullptr;
	
	auto *p = new Formula(parent);
	p->str_ = s;
	return p;
}

void
Formula::PrintNodes(QVector<formula::Node*> &vec)
{
	mtl_line("[Nodes]");
	
	for (int i = 0; i < vec.size(); i++)
	{
		formula::Node *node = vec[i];
		auto ba = node->debug_type().toLocal8Bit();
		printf("Node %d: %s\n", i, ba.data());
	}
	
	mtl_line("[/Nodes]");
}

void
Formula::SeekInParentheses(QVector<formula::Node*> &vec, int &start,
	int &end, bool &has_parentheses)
{
	has_parentheses = false;
	start = 0;
	end = -1;
	int current_deep = 0;
	int deepest = 0;
	const int count = vec.size();
	
	for (int i = 0; i < count; i++)
	{
		formula::Node *p = vec[i];
		
		if (p->is_grouping())
		{
			if (p->is_open_grouping())
			{
				has_parentheses = true;
				current_deep++;
				
				if (current_deep > deepest)
				{
					deepest = current_deep;
					start = i + 1;
				}
			} else {
				
				if (deepest == current_deep)
					end = i - 1;
				
				current_deep--;
			}
		}
	}
	
	if (end == -1)
		end = vec.size() - 1;
}

int
Formula::SeekValue(const QString &str, const int start_char_index, formula::Value &value)
{
	const int max = str.size();
	int end = start_char_index;
	
	for (int i = start_char_index; i <= max; i++)
	{
		if (i == max)
		{
			end = i;
			break;
		}
		
		const QChar c = str.at(i);
		
		if (!c.isDigit() && (c != '.'))
		{
			end = i;
			break;
		}
	}
	
	if (end == start_char_index && end != (max - 1))
	{
		auto ba = str.toLocal8Bit();
		mtl_warn("end: %d, start: %d, str: \"%s\"", end, start_char_index, ba.data());
		value.error(value::Error::BadFormulaString);
		return -1;
	}
	
	QString num_str = str.mid(start_char_index, end - start_char_index);
	bool ok;
	double num = num_str.toDouble(&ok);
	
	if (!ok)
	{
		auto ba = num_str.toLocal8Bit();
		mtl_warn("%s", ba.data());
		value.error(value::Error::BadFormulaString);
		return -1;
	}
	
	//mtl_line("Set double: %f", num);
	value.SetDouble(num);
	return end - start_char_index - 1;
}

QString
Formula::toString() const
{
	return str_;
}

} // ods::
