#include "patterncompiler.h"

PatternCompiler::PatternCompiler()
{
    ns = new lspl::Namespace();
    builder = new lspl::patterns::PatternBuilder(ns);
}

lspl::patterns::PatternBuilder::BuildInfo PatternCompiler::compilePattern(const char *pattern) throw (lspl::patterns::PatternBuildingException)
{
    return builder->build(pattern);
}

