#include "Formula.hpp"

#include "Address.hpp"
#include "Cell.hpp"
#include "CellRef.hpp"
#include "ods.hh"
#include "Row.hpp"
#include "Sheet.hpp"
#include "Value.hpp"

#include <QRegularExpression>
#include <QRegularExpressionMatch>

namespace ods {

Formula::Formula(ods::Cell *p) : cell_(p) {
	ods::Row *row = cell_->row();
	default_sheet_ = row->sheet();
}
Formula::~Formula() {}

void
Formula::Add(const double d) {
	raw_str_.append(QString::number(d));
	nodes_.append(FormulaNode::Number(d));
}

void
Formula::Add(const ods::Op op)
{
	raw_str_.append(op::ToString(op));
}

void
Formula::AddCloseBrace()
{
	raw_str_.append(")");
}

void
Formula::AddOpenBrace()
{
	raw_str_.append("(");
}

void
Formula::Add(Function f) {
	raw_str_.append(f.toString());
}

void
Formula::Add(ods::Cell *cell, ods::Sheet *sheet) {
	if (sheet == nullptr)
		raw_str_ += QLatin1String("[.");
	else
		raw_str_ += QLatin1String("[\'") + sheet->name() + QLatin1String("\'.");
	
	raw_str_ += cell->QueryAddress() + QChar(']');
}

Formula*
Formula::Clone() {
	Formula *p = new Formula(cell_);
	p->nodes_ = nodes_;
	p->raw_str_ = raw_str_;
	
	return p;
}

QByteArray
GetColored(const QString &str, int start, int count)
{
	QString s = str.mid(0, start);
	s += MTL_COLOR_GREEN;
	s += str.mid(start, count);
	s += MTL_COLOR_DEFAULT;
	s += str.mid(start + count);
	return s.toLocal8Bit();
}

Address*
Formula::CellAddressOrRange(QStringRef s, int &skip,
	ods::Sheet *default_sheet)
{
	int end = s.indexOf(']');
	
	if (end == -1)
		return nullptr;
	
	s = s.mid(0, end);
	int colon = s.indexOf(':');
	skip += end + 1;
	
	if (colon != -1) {
		QStringRef start_cell = s.mid(0, colon);
		const int pos = colon + 1;
		QStringRef end_cell = s.mid(pos, end - pos);
		return Address::Range(default_sheet, start_cell, end_cell);
	}
	
	QStringRef cell = s.mid(0, end);
	return Address::Cell(default_sheet, cell);
}

Formula*
Formula::FromString(const QString &str, ods::Cell *cell)
{
	ods::Formula *f = new ods::Formula(cell);
	f->raw_str(str);
	return f;
}

const Value&
Formula::Eval()
{
	if (!ProcessFormulaString(raw_str_)) {
		return value_;
	}
	
	evaluating(true);
	EvaluateNodes();
	evaluating(false);
	
	return value_;
}

void
Formula::EvaluateNodes()
{
	PrintNodes();
	Group group = GetDeepestGroup();
	
	for (FormulaNode &node: nodes_) {
		if (node.is_function()) {
			Function *f = node.as_function();
			const ods::Value &value = f->Eval();
			
			if (value.has_error()) {
				auto ba = value.error().toLocal8Bit();
				mtl_warn("Function error: %s", ba.data());
				return;
			}
			
			if (value.is_double()) {
				double num = *value.as_double();
				mtl_line("Function value: %.2f", num);
			} else if (value.is_none()) {
				mtl_warn("Function Value not set");
			} else {
				mtl_warn("Function value of a non-double type");
			}
		}
	}
	
	
	mtl_line("Deepest group starts at: %d, count: %d",
		group.start, group.count);
}

Group
Formula::GetDeepestGroup() const
{
	Group deepest = {0, -1};
	
	for (int i = 0; i < nodes_.size(); i++) {
		const FormulaNode &node = nodes_[i];
		
		if (node.is_brace()) {
			if (node.as_brace() == Brace::Open) {
				if (i > deepest.start)
					deepest.start = i;
			} else if (deepest.count == -1) {
				// "+ 1" to include the closing brace
				deepest.count = i - deepest.start + 1;
			}
		}
	}
	
	if (deepest.start == 0)
		deepest.count = nodes_.size();
	
	return deepest;
}

void
Formula::PrintNodes() const
{
	mtl_line("Formula nodes:");
	
	for (int i = 0; i < nodes_.size(); i++) {
		const auto &node = nodes_[i];
		QString s;
		QString type;
		if (node.is_address()) {
			type = "Address";
			s = node.as_address()->toString();
		} else if (node.is_function()) {
			type = "Function";
			s = node.as_function()->toString();
		} else if (node.is_number()) {
			type = "Number";
			s = QString::number(node.as_number());
		} else if (node.is_op()) {
			type = "Op";
			s = ods::op::ToString(node.as_op());
		} else if (node.is_brace()) {
			type = "Brace";
			s = ods::ToString(node.as_brace());
		} else if (node.is_none()) {
			type = "None";
			s = "[NONE!!]";
		}
		
		auto ba = s.toLocal8Bit();
		auto type_ba = type.toLocal8Bit();
		printf("(%d) %s: \"%s\"\n", i+1, type_ba.data(), ba.data());
	}
}

bool
Formula::ProcessFormulaString(QString s)
{
	s = s.trimmed();
	const QString prefix = QLatin1String("of:=");
	
	if (s.startsWith(prefix))
		s = s.mid(prefix.size());
	
	if (s.isEmpty()) {
		mtl_trace();
		return false;
	}
	
	int resume_at = -1;
	QStringRef str_ref = s.midRef(0);
	
	while (ProcessNext(str_ref, resume_at, nodes_)) {
		str_ref = str_ref.mid(resume_at);
		
		if (str_ref.isEmpty())
			return true;
		
		auto ba = str_ref.toLocal8Bit();
		mtl_line("Next string: \"%s\"", ba.data());
	}
	
	return false;
}

bool
Formula::ProcessNext(QStringRef s, int &resume_at,
	QVector<FormulaNode> &vec)
{
	int index = ods::FindNonWhitespace(s);
	
	if (index == -1)
		return -1;
	
	resume_at = index;
	
	if (resume_at > 0) {
//		mtl_line("skipped %d spaces", resume_at);
		s = s.mid(resume_at);
	}
	
	if (s.startsWith('(')) {
		vec.append(FormulaNode::Brace(Brace::Open));
		resume_at += 1;
		
		return true;
	}
	
	if (s.startsWith(')')) {
		vec.append(FormulaNode::Brace(Brace::Close));
		resume_at += 1;
		
		return true;
	}
	
	if (s.startsWith('[')) {
		Address *a = CellAddressOrRange(s.mid(1), resume_at, default_sheet_);
		if (a != nullptr) {
			nodes_.append(FormulaNode::Address(a));
			resume_at += 1;
		}
		return a != nullptr;
	}
	
	// \d+(\.\d+)?
	static const auto digit_regex = QRegularExpression("(?<![a-zA-Z\\.])\\d+(\\.\\d+)?(?!\\.)");
	QRegularExpressionMatch rmatch;
	QString as_str = s.toString();
	int at = as_str.indexOf(digit_regex, 0, &rmatch);
	
	if (at == 0) {
		QStringRef number_str = rmatch.capturedRef(0);
		bool ok;
		double num = number_str.toDouble(&ok);
		
		if (ok) {
			vec.append(FormulaNode::Number(num));
			resume_at += number_str.size();
			return ok;
		} else {
			auto ba = number_str.toString().toLocal8Bit();
			mtl_warn("Bad number: \"%s\"", ba.data());
			return false;
		}
		
	}
	
	static const auto op_regex = QRegularExpression(ods::op_str::Regex);
	QRegularExpressionMatch regex_match;
	at = as_str.indexOf(op_regex, 0, &regex_match);
	
	if (at == 0) {
		QStringRef op_str = regex_match.capturedRef(0);
		Op op = ods::op::From(op_str);
		
		if (op == Op::None) {
			mtl_trace();
			value_.error(as_str);
			
			return false;
		} else {
			vec.append(FormulaNode::Op(op));
			resume_at += op_str.size();
			return true;
		}
	}
	
	Function *f = new Function();
	int skip = Function::TryNew(s, this, *f);

	if (skip != -1) {
		vec.append(FormulaNode::Function(f));
		resume_at += skip;
		return true;
	}
	delete f;
	
	if (!has_error()) {
		mtl_warn("Error message not set");
		value_.error(s.toString());
	}
	
	return false;
}

QString
Formula::ToXmlString() const
{
	mtl_warn("TBD");
	return formula::Prefix;
}

}
