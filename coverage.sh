#!/bin/sh

COVERAGE_DIR=.coverage
CURRENT_DIR=`pwd`

echo $CURRENT_DIR

lcov -c -i -d . -o .coverage.base --rc lcov_branch_coverage=1
lcov -c -d . -o .coverage.run --rc lcov_branch_coverage=1
lcov -d . -a .coverage.base -a .coverage.run -o .coverage.total --rc lcov_branch_coverage=1
lcov -d . -e .coverage.total $CURRENT_DIR/src/hello/* -o .coverage.filtered --rc lcov_branch_coverage=1

rm -Rf $COVERAGE_DIR
genhtml --branch-coverage -o $COVERAGE_DIR .coverage.filtered

rm -f .coverage.base .coverage.run .coverage.total
