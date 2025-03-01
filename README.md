Ods2 - a library to read/write/update OpenDocument Spreadsheet (.ods) files.
Supports the most commonly used formula functions (including evaluation).

#### Building
As of Jan 2025 moved to Qt6-only.<br/>
<b>libzip</b> and <b>zlib</b> are used for .zip support (libzip needs zlib, and Ods2 needs libzip).
<br/>
Building tested on Windows with Visual Studio 17 2022 and LLVM MinGW,
and with g++ on Fedora Linux 41.<br/>
See <b>how_to_build.html</b>

##### API usage
See examples/examples[1-2].cc

The file examples/Invoice.ods was generated with Ods2, the source code
that generated it is in examples/Invoice.cpp

##### License, etc
Licensed under the GPLv3 or MIT license, see LICENSE for details.
For questions, feature requests, etc: f35f22fan AT gmail DOT com

---
##### 59 out of 393 functions implemented

Column 1 | Column 2 | Column 3 | Column 4
-------- | ---------- | -------- | ---------
:ballot_box_with_check: ABS() | :black_square_button: ACCRINT() | :black_square_button: ACCRINTM() | :ballot_box_with_check: ACOS()
:black_square_button: ACOSH() | :ballot_box_with_check: ACOT() | :black_square_button: ACOTH() | :black_square_button: ADDRESS()
:black_square_button: AMORDEGRC() | :black_square_button: AMORLINC() | :ballot_box_with_check: AND() | :black_square_button: ARABIC()
:black_square_button: AREAS() | :ballot_box_with_check: ASIN() | :black_square_button: ASINH() | :ballot_box_with_check: ATAN()
:ballot_box_with_check: ATAN2() | :black_square_button: ATANH() | :black_square_button: AVEDEV() | :ballot_box_with_check: AVERAGE()
:black_square_button: AVERAGEA() | :black_square_button: AVERAGEIFS() | :black_square_button: B() | :black_square_button: BAHTTEXT()
:black_square_button: BASE() | :black_square_button: BESSELI() | :black_square_button: BESSELJ() | :black_square_button: BESSELK()
:black_square_button: BESSELY() | :black_square_button: BETADIST() | :black_square_button: BETAINV() | :black_square_button: BIN2DEC()
:black_square_button: BIN2HEX() | :black_square_button: BIN2OCT() | :black_square_button: BINOMDIST() | :ballot_box_with_check: BITAND()
:ballot_box_with_check: BITLSHIFT() | :ballot_box_with_check: BITOR() | :ballot_box_with_check: BITRSHIFT() | :ballot_box_with_check: BITXOR()
:black_square_button: CEILING() | :black_square_button: CELL() | :black_square_button: CHAR() | :black_square_button: CHIDIST()
:black_square_button: CHIINV() | :black_square_button: CHISQDIST() | :black_square_button: CHISQINV() | :black_square_button: CHITEST()
:black_square_button: CHOOSE() | :black_square_button: CLEAN() | :black_square_button: CODE() | :black_square_button: COLUMN()
:ballot_box_with_check: COLUMNS() | :black_square_button: COMBIN() | :black_square_button: COMBINA() | :black_square_button: COMPLEX()
:ballot_box_with_check: CONCATENATE() | :black_square_button: CONFIDENCE() | :black_square_button: CONVERT() | :black_square_button: CONVERT_ADD()
:black_square_button: CORREL() | :ballot_box_with_check: COS() | :black_square_button: COSH() | :ballot_box_with_check: COT()
:black_square_button: COTH() | :ballot_box_with_check: COUNT() | :ballot_box_with_check: COUNTA() | :ballot_box_with_check: COUNTBLANK()
:ballot_box_with_check: COUNTIF() | :black_square_button: COUNTIFS() | :black_square_button: COUPDAYBS() | :black_square_button: COUPDAYS()
:black_square_button: COUPDAYSNC() | :black_square_button: COUPNCD() | :black_square_button: COUPNUM() | :black_square_button: COUPPCD()
:black_square_button: COVAR() | :black_square_button: CRITBINOM() | :black_square_button: CSC() | :black_square_button: CSCH()
:black_square_button: CUMIPMT() | :black_square_button: CUMIPMT_ADD() | :black_square_button: CUMPRINC() | :black_square_button: CUMPRINC_ADD()
:black_square_button: CURRENT() | :ballot_box_with_check: DATE() | :black_square_button: DATEVALUE() | :black_square_button: DAVERAGE()
:ballot_box_with_check: DAY() | :black_square_button: DAYS() | :black_square_button: DAYS360() | :black_square_button: DAYSINMONTH()
:black_square_button: DAYSINYEAR() | :black_square_button: DB() | :black_square_button: DCOUNT() | :black_square_button: DCOUNTA()
:black_square_button: DDB() | :black_square_button: DDE() | :black_square_button: DEC2BIN() | :black_square_button: DEC2HEX()
:black_square_button: DEC2OCT() | :black_square_button: DECIMAL() | :black_square_button: DEGREES() | :black_square_button: DELTA()
:black_square_button: DEVSQ() | :black_square_button: DGET() | :black_square_button: DISC() | :black_square_button: DMAX()
:black_square_button: DMIN() | :black_square_button: DOLLAR() | :black_square_button: DOLLARDE() | :black_square_button: DOLLARFR()
:black_square_button: DPRODUCT() | :black_square_button: DSTDEV() | :black_square_button: DSTDEVP() | :black_square_button: DSUM()
:black_square_button: DURATION() | :black_square_button: DURATION_ADD() | :black_square_button: DVAR() | :black_square_button: DVARP()
:black_square_button: EASTERSUNDAY() | :black_square_button: EDATE() | :black_square_button: EFFECTIVE() | :black_square_button: EFFECT_ADD()
:black_square_button: EOMONTH() | :black_square_button: ERF() | :black_square_button: ERFC() | :black_square_button: ERRORTYPE()
:black_square_button: EVEN() | :black_square_button: EXACT() | :black_square_button: EXP() | :black_square_button: EXPONDIST()
:black_square_button: FACT() | :black_square_button: FACTDOUBLE() | :ballot_box_with_check: FALSE() | :black_square_button: FDIST()
:black_square_button: FIND() | :black_square_button: FINV() | :black_square_button: FISHER() | :black_square_button: FISHERINV()
:black_square_button: FIXED() | :black_square_button: FLOOR() | :black_square_button: FORECAST() | :ballot_box_with_check: FORMULA()
:black_square_button: FREQUENCY() | :black_square_button: FTEST() | :black_square_button: FV() | :black_square_button: FVSCHEDULE()
:black_square_button: GAMMA() | :black_square_button: GAMMADIST() | :black_square_button: GAMMAINV() | :black_square_button: GAMMALN()
:black_square_button: GAUSS() | :black_square_button: GCD() | :black_square_button: GCD_ADD() | :black_square_button: GEOMEAN()
:black_square_button: GESTEP() | :black_square_button: GROWTH() | :black_square_button: HARMEAN() | :black_square_button: HEX2BIN()
:black_square_button: HEX2DEC() | :black_square_button: HEX2OCT() | :black_square_button: HLOOKUP() | :ballot_box_with_check: HOUR()
:black_square_button: HYPERLINK() | :black_square_button: HYPGEOMDIST() | :ballot_box_with_check: IF() | :black_square_button: IMABS()
:black_square_button: IMAGINARY() | :black_square_button: IMARGUMENT() | :black_square_button: IMCONJUGATE() | :black_square_button: IMCOS()
:black_square_button: IMCOSH() | :black_square_button: IMCOT() | :black_square_button: IMCSC() | :black_square_button: IMCSCH()
:black_square_button: IMDIV() | :black_square_button: IMEXP() | :black_square_button: IMLN() | :black_square_button: IMLOG10()
:black_square_button: IMLOG2() | :black_square_button: IMPOWER() | :black_square_button: IMPRODUCT() | :black_square_button: IMREAL()
:black_square_button: IMSEC() | :black_square_button: IMSECH() | :black_square_button: IMSIN() | :black_square_button: IMSINH()
:black_square_button: IMSQRT() | :black_square_button: IMSUB() | :black_square_button: IMSUM() | :black_square_button: IMTAN()
:black_square_button: INDEX() | :ballot_box_with_check: INDIRECT() | :black_square_button: INFO() | :black_square_button: INT()
:black_square_button: INTERCEPT() | :black_square_button: INTRATE() | :black_square_button: IPMT() | :black_square_button: IRR()
:black_square_button: ISBLANK() | :black_square_button: ISERR() | :black_square_button: ISERROR() | :black_square_button: ISEVEN()
:black_square_button: ISEVEN() | :black_square_button: ISEVEN_ADD() | :black_square_button: ISFORMULA() | :black_square_button: ISLEAPYEAR()
:black_square_button: ISLOGICAL() | :black_square_button: ISNA() | :black_square_button: ISNONTEXT() | :black_square_button: ISNUMBER()
:black_square_button: ISODD() | :black_square_button: ISODD() | :black_square_button: ISODD_ADD() | :black_square_button: ISPMT()
:black_square_button: ISREF() | :black_square_button: ISTEXT() | :black_square_button: KURT() | :black_square_button: LARGE()
:black_square_button: LCM() | :black_square_button: LCM_ADD() | :black_square_button: LEFT() | :black_square_button: LEN()
:black_square_button: LINEST() | :black_square_button: LN() | :black_square_button: LOG() | :black_square_button: LOG10()
:black_square_button: LOGEST() | :black_square_button: LOGINV() | :black_square_button: LOGNORMDIST() | :black_square_button: LOOKUP()
:black_square_button: LOWER() | :ballot_box_with_check: MATCH() | :ballot_box_with_check: MAX() | :black_square_button: MAXA()
:black_square_button: MDETERM() | :black_square_button: MDURATION() | :black_square_button: MEDIAN() | :black_square_button: MID()
:ballot_box_with_check: MIN() | :black_square_button: MINA() | :ballot_box_with_check: MINUTE() | :black_square_button: MINVERSE()
:black_square_button: MIRR() | :black_square_button: MMULT() | :ballot_box_with_check: MOD() | :black_square_button: MODE()
:ballot_box_with_check: MONTH() | :black_square_button: MONTHS() | :black_square_button: MROUND() | :black_square_button: MULTINOMIAL()
:black_square_button: MUNIT() | :black_square_button: N() | :black_square_button: NA() | :black_square_button: NEGBINOMDIST()
:black_square_button: NETWORKDAYS() | :black_square_button: NOMINAL() | :black_square_button: NOMINAL_ADD() | :black_square_button: NORMDIST()
:black_square_button: NORMINV() | :black_square_button: NORMSDIST() | :black_square_button: NORMSINV() | :ballot_box_with_check: NOT()
:ballot_box_with_check: NOW() | :black_square_button: NPER() | :black_square_button: NPV() | :black_square_button: OCT2BIN()
:black_square_button: OCT2DEC() | :black_square_button: OCT2HEX() | :black_square_button: ODD() | :black_square_button: ODDFPRICE()
:black_square_button: ODDFYIELD() | :black_square_button: ODDLPRICE() | :black_square_button: ODDLYIELD() | :ballot_box_with_check: OFFSET()
:ballot_box_with_check: OR() | :black_square_button: PEARSON() | :black_square_button: PERCENTILE() | :black_square_button: PERCENTRANK()
:black_square_button: PERMUT() | :black_square_button: PERMUTATIONA() | :black_square_button: PHI() | :ballot_box_with_check: PI()
:black_square_button: PMT() | :black_square_button: POISSON() | :ballot_box_with_check: POWER() | :black_square_button: PPMT()
:black_square_button: PRICE() | :black_square_button: PRICEDISC() | :black_square_button: PRICEMAT() | :black_square_button: PROB()
:ballot_box_with_check: PRODUCT() | :black_square_button: PROPER() | :black_square_button: PV() | :black_square_button: QUARTILE()
:ballot_box_with_check: QUOTIENT() | :black_square_button: RADIANS() | :ballot_box_with_check: RAND() | :ballot_box_with_check: RANDBETWEEN()
:black_square_button: RANK() | :black_square_button: RATE() | :black_square_button: RECEIVED() | :black_square_button: REPLACE()
:black_square_button: REPT() | :black_square_button: RIGHT() | :black_square_button: ROMAN() | :ballot_box_with_check: ROUND()
:ballot_box_with_check: ROUNDDOWN() | :ballot_box_with_check: ROUNDUP() | :black_square_button: ROW() | :ballot_box_with_check: ROWS()
:black_square_button: RRI() | :black_square_button: RSQ() | :black_square_button: SEARCH() | :black_square_button: SEC()
:black_square_button: SECH() | :ballot_box_with_check: SECOND() | :black_square_button: SERIESSUM() | :black_square_button: SHEET()
:black_square_button: SHEETS() | :black_square_button: SIGN() | :ballot_box_with_check: SIN() | :black_square_button: SINH()
:black_square_button: SKEW() | :black_square_button: SLN() | :black_square_button: SLOPE() | :black_square_button: SMALL()
:black_square_button: SQRT() | :black_square_button: SQRTPI() | :black_square_button: STANDARDIZE() | :black_square_button: STDEV()
:black_square_button: STDEVA() | :black_square_button: STDEVP() | :black_square_button: STDEVPA() | :black_square_button: STEYX()
:black_square_button: STYLE() | :black_square_button: SUBSTITUTE() | :black_square_button: SUBTOTAL() | :ballot_box_with_check: SUM()
:ballot_box_with_check: SUMIF() | :black_square_button: SUMIFS() | :black_square_button: SUMPRODUCT() | :black_square_button: SUMSQ()
:black_square_button: SUMX2MY2() | :black_square_button: SUMX2PY2() | :black_square_button: SUMXMY2() | :black_square_button: SYD()
:black_square_button: T() | :ballot_box_with_check: TAN() | :black_square_button: TANH() | :black_square_button: TBILLEQ()
:black_square_button: TBILLPRICE() | :black_square_button: TBILLYIELD() | :black_square_button: TDIST() | :ballot_box_with_check: TEXT()
:ballot_box_with_check: TIME() | :ballot_box_with_check: TIMEVALUE() | :black_square_button: TINV() | :ballot_box_with_check: TODAY()
:black_square_button: TRANSPOSE() | :black_square_button: TREND() | :black_square_button: TRIM() | :black_square_button: TRIMMEAN()
:ballot_box_with_check: TRUE() | :black_square_button: TRUNC() | :black_square_button: TTEST() | :black_square_button: TYPE()
:black_square_button: UPPER() | :black_square_button: VALUE() | :black_square_button: VAR() | :black_square_button: VARA()
:black_square_button: VARP() | :black_square_button: VARPA() | :black_square_button: VDB() | :black_square_button: VLOOKUP()
:black_square_button: WEEKDAY() | :black_square_button: WEEKNUM() | :black_square_button: WEEKNUM_ADD() | :black_square_button: WEEKS()
:black_square_button: WEEKSINYEAR() | :black_square_button: WEIBULL() | :black_square_button: WORKDAY() | :black_square_button: XIRR()
:black_square_button: XNPV() | :black_square_button: XOR() | :ballot_box_with_check: YEAR() | :black_square_button: YEARFRAC()
:black_square_button: YEARS() | :black_square_button: YIELD() | :black_square_button: YIELDDISC() | :black_square_button: YIELDMAT()
:black_square_button: ZTEST() | 


