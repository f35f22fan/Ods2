#include "Formula.hpp"

#include "Address.hpp"
#include "Cell.hpp"
#include "CellRef.hpp"
#include "ods.hh"
#include "Row.hpp"
#include "Sheet.hpp"

#include <QRegularExpression>
#include <QRegularExpressionMatch>

namespace ods {

Formula::Formula(ods::Cell *p) : cell_(p) {
	ods::Row *row = cell_->row();
	default_sheet_ = row->sheet();
}
Formula::~Formula() {
	for (FormulaNode *k: nodes_) {
		delete k;
	}
	nodes_.clear();
	
	delete value_;
	value_ = nullptr;
}

void
Formula::Add(const double d) {
	raw_str_.append(QString::number(d));
	nodes_.append(FormulaNode::Double(d));
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
	
	for (FormulaNode *node : nodes_) {
		p->nodes_.append(node->Clone());
	}
	
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
	if (str.isEmpty())
		return nullptr;
	
	ods::Formula *f = new ods::Formula(cell);
	f->raw_str(str);
	return f;
}

FormulaNode*
Formula::Eval()
{
	CHECK_TRUE_NULL(ProcessFormulaString(raw_str_));
	evaluating(true);
	EvaluateNodes();
	evaluating(false);
	return value_;
}

void
Formula::EvaluateNodes()
{
	function::PrintNodes(nodes_);
	
	while (nodes_.size() > 1) {
		CHECK_TRUE_VOID(function::EvalDeepestGroup(nodes_));
	}
	
	if (nodes_.size() != 1) {
		mtl_trace();
		return;
	}
	
	FormulaNode *result = nodes_[0];
	auto ba = result->toString().toLocal8Bit();
	mtl_line("Result is: %s", ba.data());
}

bool
Formula::ProcessFormulaString(QString s)
{
	auto ba = s.toLocal8Bit();
	mtl_line("Breaking down into nodes: %s", ba.data());
	s = s.trimmed();
	const QString prefix = QLatin1String("of:=");
	
	if (s.startsWith(prefix))
		s = s.mid(prefix.size());
	
	if (s.isEmpty()) {
		mtl_trace();
		return false;
	}
	
	int resume_at = 0;
	int chunk = 0;
	QStringRef str_ref = s.midRef(0);
	u8 settings = 0;
	
	while (DecodeNext(str_ref, chunk, nodes_, default_sheet_, settings)) {
		str_ref = str_ref.mid(chunk);
		resume_at += chunk;
		chunk = 0;
		
		if (str_ref.isEmpty())
			return true;
	}
	
	if (str_ref.size() > 0) {
		auto ba = str_ref.toLocal8Bit();
		mtl_warn("String not processed: \"%s\"", ba.data());
	}
	
	return false;
}

bool
Formula::DecodeNext(QStringRef s, int &resume_at, QVector<FormulaNode*> &vec,
	ods::Sheet *default_sheet, u8 &settings)
{
	{
#ifdef DEBUG_FORMULA
		auto ba = s.toLocal8Bit();
		mtl_line("Processing: \"%s\"", ba.data());
#endif
	}
	
	const bool InsideFunctionParams = settings & ParsingFunctionParams;
	int whitespace_count = ods::FindNonWhitespace(s);
	
	if (whitespace_count == -1) {
		mtl_trace();
		return -1;
	}
	
	resume_at += whitespace_count;
	
	if (whitespace_count > 0) {
		s = s.mid(whitespace_count);
#ifdef DEBUG_FORMULA
		auto ba = s.toLocal8Bit();
		mtl_line("^\\\\s removed: \"%s\", count: %d", ba.data(), whitespace_count);
#endif
	}
	
	if (s.startsWith('(')) {
#ifdef DEBUG_FORMULA
		mtl_line("Brace (");
#endif
		if (InsideFunctionParams) {
			it_happened();
			return true;
		} else {
			vec.append(FormulaNode::Brace(Brace::Open));
		}
		
		resume_at++;
		return true;
	}
	
	if (s.startsWith(')')) {
#ifdef DEBUG_FORMULA
mtl_line("Brace )");
#endif
		if (InsideFunctionParams) {
#ifdef DEBUG_FORMULA
			mtl_line("Returning from function");
#endif
			settings |= ReachedFunctionEnd;
		} else {
			vec.append(FormulaNode::Brace(Brace::Close));
		}
		
		resume_at++;
		return true;
	}
	
	if (s.startsWith('[')) {
		Address *a = CellAddressOrRange(s.mid(1), resume_at, default_sheet);
		if (a != nullptr) {
#ifdef DEBUG_FORMULA
			auto ba = a->toString().toLocal8Bit();
			mtl_line("Address: %s", ba.data());
#endif
			vec.append(FormulaNode::Address(a));
			resume_at++;
		}
		return a != nullptr;
	}
	
	if (InsideFunctionParams && s.startsWith(';')) {
#ifdef DEBUG_FORMULA
mtl_line("Param separator ;");
#endif
		settings |= ReachedParamSeparator;
		resume_at++;
		return true;
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
#ifdef DEBUG_FORMULA
		mtl_line("Number %.2f", num);
#endif
		if (ok) {
			vec.append(FormulaNode::Double(num));
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
		QStringRef captured_op_str = regex_match.capturedRef(0);
#ifdef DEBUG_FORMULA
		auto ba = captured_op_str.toLocal8Bit();
		mtl_line("op \"%s\"", ba.data());
#endif
		Op op = ods::op::From(captured_op_str);
		
		if (op == Op::None) {
			mtl_trace();
			return false;
		} else {
			vec.append(FormulaNode::Op(op));
			resume_at += captured_op_str.size();
			return true;
		}
	}
	
	Function *f = Function::TryNew(s, resume_at, default_sheet);
	if (f != nullptr) {
#ifdef DEBUG_FORMULA
		auto ba = s.mid(0, resume_at).toLocal8Bit();
		mtl_line("\"%s\"", ba.data());
#endif
		vec.append(FormulaNode::Function(f));
		return true;
	}
	
	return false;
}

QString
Formula::ToXmlString() const
{
	QString s = formula::Prefix;
	
	for (FormulaNode *node: nodes_) {
		s.append(node->toString());
	}
	
	return s;
}

}
