#include "Formula.hpp"

#include "Reference.hpp"
#include "Book.hpp"
#include "Cell.hpp"
#include "CellRef.hpp"
#include "ods.hh"
#include "Row.hpp"
#include "Sheet.hpp"
#include "inst/TableNamedRange.hpp"

#include <QRegularExpression>
#include <QRegularExpressionMatch>

namespace ods {

Formula::Formula(ods::Cell *p) : cell_(p) {
	ods::Row *row = cell_->row();
	default_sheet_ = row->sheet();
}
Formula::~Formula() {
	RemoveAllNodes();
}

void
Formula::Add(const double d) {
	nodes_.append(FormulaNode::Double(d));
}

void
Formula::Add(const ods::Op op)
{
	nodes_.append(FormulaNode::Op(op));
}

void
Formula::AddCloseBrace()
{
	nodes_.append(FormulaNode::Brace(Brace::Close));
}

void
Formula::AddOpenBrace()
{
	nodes_.append(FormulaNode::Brace(Brace::Open));
}

ods::Function*
Formula::Add(const ods::FunctionId id)
{
	auto *fn = ods::Function::New(id);
	fn->parent_formula_ = this;
	nodes_.append(FormulaNode::Function(fn));
	return fn;
}

void
Formula::Add(ods::Cell *cell)
{
	auto *a = default_sheet_->NewReference(cell);
	nodes_.append(FormulaNode::Reference(a));
}

void
Formula::Add(QString *s) {
	nodes_.append(FormulaNode::String(s));
}

void
Formula::AddCellRange(Cell *start, Cell *end)
{
	auto *a = default_sheet_->NewReference(start, end);
	nodes_.append(FormulaNode::Reference(a));
}

Formula*
Formula::Clone() {
mtl_trace();
	Formula *p = new Formula(cell_);
	
	for (FormulaNode *node : nodes_) {
mtl_trace();
		p->nodes_.append(node->Clone());
	}
	
	p->str_to_evaluate_ = str_to_evaluate_;
	
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

Reference*
Formula::CellAddressOrRange(QStringView s, int &skip,
	ods::Sheet *default_sheet)
{
	const QChar SingleQuote('\'');
	int end;
	const bool StartsWithSingleQuote = s.startsWith(SingleQuote);
	int end_of_single_quoted_string = -1;
	
	if (StartsWithSingleQuote) {
		end_of_single_quoted_string = ods::FindEndOfSingleQuotedString(s);
		if (end_of_single_quoted_string == -1)
			return nullptr;
		end_of_single_quoted_string++;
		end = s.indexOf(']', end_of_single_quoted_string);
	} else {
		end = s.indexOf(']');
	}
	
	if (end == -1)
		return nullptr;
	
	s = s.mid(0, end);
	int search_from = StartsWithSingleQuote ? end_of_single_quoted_string : 0;
	int colon = s.indexOf(':', search_from);
	skip += end + 1;
	
	if (colon != -1) {
		QStringView start_cell = s.mid(0, colon);
		const int pos = colon + 1;
		QStringView end_cell = s.mid(pos, end - pos);
		return Reference::CellRange(default_sheet, start_cell, end_cell);
	}
	
	QStringView cell = s.mid(0, end);
	return Reference::Cell(default_sheet, cell);
}

Formula*
Formula::FromString(const QString &str, ods::Cell *cell)
{
	if (str.isEmpty())
		return nullptr;
	
	ods::Formula *f = new ods::Formula(cell);
	f->str_to_evaluate_ = str;
	return f;
}

FormulaNode*
Formula::Eval()
{
	if (nodes_.isEmpty()) {
		MTL_CHECK_NULL(ProcessFormulaString(str_to_evaluate_, nodes_));
	}
	
	QVector<FormulaNode*> *cloned_vec = eval::CloneVec(nodes_);
	ods::AutoDeleteVecP adv(cloned_vec);
	evaluating(true);
	ods::FormulaNode *result = EvaluateNodes(*cloned_vec);
	evaluating(false);

	return result;
}

FormulaNode*
Formula::EvaluateNodes(QVector<FormulaNode *> &nodes)
{
#ifdef DEBUG_FORMULA_EVAL
	eval::PrintNodes(nodes, "From EvaluateNodes [start]");
#endif
	
	if (nodes.size() == 1) {
		MTL_CHECK_NULL(eval::EvalDeepestGroup(nodes));
	} else {
		while (nodes.size() > 1) {
			MTL_CHECK_NULL(eval::EvalDeepestGroup(nodes));
		}
	}
	MTL_CHECK_NULL((nodes.size() == 1));
	FormulaNode *result = nodes[0];
#ifdef DEBUG_FORMULA_EVAL
	auto ba = result->toString().toLocal8Bit();
	mtl_info("Formula Result: \"%s\"", ba.data());
#endif
	return result->Clone();
}

bool
Formula::ParseNext(QStringView s, int &resume_at,
	QVector<FormulaNode*> &vec,
	ods::Sheet *default_sheet, u8 &settings)
{
#ifdef DEBUG_FORMULA_PARSING
	auto proces_ba = s.toLocal8Bit();
	mtl_info("Processing: \"%s\"", proces_ba.data());
#endif
	
	const bool InsideFunctionParams = settings & ParsingFunctionParams;
	int whitespace_count = ods::FindNonWhitespace(s);
	
	if (whitespace_count == -1) {
		mtl_trace();
		return false;
	}
	
	resume_at += whitespace_count;
	
	if (whitespace_count > 0) {
		s = s.mid(whitespace_count);
#ifdef DEBUG_FORMULA_PARSING
		auto ba = s.toLocal8Bit();
		mtl_info("^\\\\s removed: \"%s\", count: %d", ba.data(), whitespace_count);
#endif
	}
	
	if (s.startsWith('(')) {
#ifdef DEBUG_FORMULA_PARSING
		mtl_info("Brace (");
#endif
		if (InsideFunctionParams) {
			mtl_it_happened();
			return true;
		} else {
			vec.append(FormulaNode::Brace(Brace::Open));
		}
		
		resume_at++;
		return true;
	}
	
	if (s.startsWith(u')')) {
#ifdef DEBUG_FORMULA_PARSING
mtl_info("Brace )");
#endif
		if (InsideFunctionParams) {
#ifdef DEBUG_FORMULA_PARSING
			mtl_info("Returning from function");
#endif
			settings |= ReachedFunctionEnd;
		} else {
			vec.append(FormulaNode::Brace(Brace::Close));
		}
		
		resume_at++;
		return true;
	}
	
	if (s.startsWith('[')) {
		Reference *a = CellAddressOrRange(s.mid(1), resume_at, default_sheet);
		if (a != nullptr) {
#ifdef DEBUG_FORMULA_PARSING
			auto ba = a->toString().toLocal8Bit();
			mtl_info("Reference: %s", ba.data());
#endif
			vec.append(FormulaNode::Reference(a));
			resume_at++;
		}
		return a != nullptr;
	}
	
	if (InsideFunctionParams && s.startsWith(';')) {
#ifdef DEBUG_FORMULA_PARSING
mtl_info("Param separator ;");
#endif
		settings |= ReachedParamSeparator;
		resume_at++;
		return true;
	}
	
	if (s.startsWith(u"\"")) {
		int end = s.indexOf(u"\"", 1);
		MTL_CHECK(end != -1);
		QStringView str_arg = s.mid(1, end - 1);
		auto ba = str_arg.toLocal8Bit();
		mtl_info("String: \"%s\"", ba.data());
		resume_at += end + 1;
		QString *p_str = new QString(str_arg.toString());
		vec.append(FormulaNode::String(p_str));
		return true;
	}
	
	// \d+(\.\d+)?
	static const auto digit_regex = QRegularExpression("(?<![a-zA-Z\\.])\\d+(\\.\\d+)?(?!\\.)");
	QRegularExpressionMatch rmatch;
	QString as_str = s.toString();
	int at = as_str.indexOf(digit_regex, 0, &rmatch);
	const bool is_number = (at == 0);
	if (is_number) {
		QStringView number_str = rmatch.capturedView(0);
		bool ok;
		double num = number_str.toDouble(&ok);
#ifdef DEBUG_FORMULA_PARSING
		mtl_info("Number %.2f", num);
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
	const bool found_op = (at == 0);
	if (found_op) {
		QStringView captured_op_str = regex_match.capturedView(0);
#ifdef DEBUG_FORMULA_PARSING
		auto ba = captured_op_str.toLocal8Bit();
		mtl_info("op \"%s\"", ba.data());
#endif
		Op op = ods::op::From(captured_op_str);
		MTL_CHECK((op != Op::None));
		vec.append(FormulaNode::Op(op));
		resume_at += captured_op_str.size();
		return true;
	}
	
	Function *f = Function::TryNew(s, resume_at, default_sheet);
	if (f != nullptr) {
#ifdef DEBUG_FORMULA_PARSING
		auto ba = s.mid(0, resume_at).toLocal8Bit();
		mtl_info("\"%s\"", ba.data());
#endif
		vec.append(FormulaNode::Function(f));
		return true;
	}
	
	inst::TableNamedRange *nr = StartsWithNamedRange(s, default_sheet);
	if (nr != nullptr) {
		vec.append(FormulaNode::NamedRange(nr));
		resume_at += nr->name().size();
		return true;
	}
	
	return false;
}

bool
Formula::ProcessFormulaString(QString s, QVector<FormulaNode*> &nodes)
{
#ifdef DEBUG_FORMULA_PARSING
	auto ba = s.toLocal8Bit();
	mtl_info("Breaking down into nodes: %s", ba.data());
#endif
	const QString prefix = QLatin1String("of:=");
	
	if (s.startsWith(prefix))
		s = s.mid(prefix.size());
	
	MTL_CHECK((!s.isEmpty()));
	int resume_at = 0;
	int chunk = 0;
	u8 settings = 0;
	QStringView str_ref(s);
	
	while (ParseNext(str_ref, chunk, nodes, default_sheet_, settings)) {
		resume_at += chunk;
		str_ref = str_ref.mid(chunk);
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

void
Formula::RemoveAllNodes()
{
	for (auto *node: nodes_) {
		delete node;
	}
	nodes_.clear();
	str_to_evaluate_.clear();
}

bool
Formula::ParseString(const QString &s, QVector<FormulaNode*> &result,
	ods::Sheet *default_sheet, const ParsingSettings ps)
{
	u8 settings = 0;
	int chunk = 0;
	int resume_at = 0;
	QStringView str_ref(s);
	
	while (ParseNext(str_ref, chunk, result, default_sheet, settings)) {
		resume_at += chunk;
		str_ref = str_ref.mid(chunk);
		chunk = 0;
		
		if (str_ref.isEmpty())
			return true;
	}
	
	if (ps & ods::TreatRemainderAsString) {
		auto *node = FormulaNode::String(new QString(str_ref.toString()));
		result.append(node);
		return true;
	}
	
	auto ba = str_ref.toLocal8Bit();
	mtl_trace("Remaining string is: \"%s\"", ba.data());
	return false;
}

inst::TableNamedRange*
Formula::StartsWithNamedRange(QStringView s, ods::Sheet *sheet) {
	const auto &named_ranges = sheet->book()->GetAllNamedRanges();
	for (inst::TableNamedRange *nr: named_ranges) {
		
		if (!nr->global() && (nr->GetSheet() != sheet)) {
			continue;
		}
		
		if (s.startsWith(nr->name())) {
//			QString new_range = QChar('[') + nr->cell_range_address() + QChar(']');
//			QString new_s = new_range;
//			new_s += original_string.mid(s.position() + name.size());
//			original_string = new_s;
//			s = original_string.midRef(s.position());
//			return true;
			return nr;
		}
	}
	return nullptr;
}

QString
Formula::ToXmlString()
{
	if (nodes_.isEmpty()) {
//		auto ba = str_to_evaluate_.toLocal8Bit();
//		mtl_info("\"%s\"", ba.data());
		// valid but has no nodes because they
		// get generated when Eval() gets called.
		return str_to_evaluate_;
	}
	
	QString s = formula::Prefix;
	for (FormulaNode *node: nodes_) {
		s.append(node->toString(ods::ToStringArgs::IncludeQuotMarks));
	}
	
	return s;
}

}
