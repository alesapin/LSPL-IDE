#ifndef PATTERNCOMPILER_H
#define PATTERNCOMPILER_H
#include <lspl/Namespace.h>
#include <lspl/patterns/PatternBuilder.h>
#include <lspl/patterns/Pattern.h>
#include <lspl/text/readers/PlainTextReader.h>
#include <lspl/text/Text.h>
#include <lspl/text/Match.h>

class PatternCompiler
{
    lspl::NamespaceRef ns;
    lspl::patterns::PatternBuilderRef builder;
public:
    PatternCompiler();
    lspl::patterns::PatternBuilder::BuildInfo compilePattern(const char* pattern) throw (lspl::patterns::PatternBuildingException);
};

#endif // PATTERNCOMPILER_H
