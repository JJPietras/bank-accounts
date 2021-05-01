//
// Created by student on 12.01.2020.
//

#include "exceptions/LoanExceptions.h"

using namespace std;

LoanException::LoanException(const string &message) : logic_error(message) {}

LoanConstructionException::LoanConstructionException(const string &message) : LoanException(message) {}

LoanModificationException::LoanModificationException(const string &message) : LoanException(message) {}

ConsolidationLoanException::ConsolidationLoanException(const string &message) : LoanException(message) {}

ConsolidationLoanConstructionException::ConsolidationLoanConstructionException(const string &message) : ConsolidationLoanException(message) {}

ConsolidationLoanMethodException::ConsolidationLoanMethodException(const string &message) : ConsolidationLoanException(message) {}

LoanRepositoryException::LoanRepositoryException(const string &message) : LoanException(message) {}

LoanBuilderException::LoanBuilderException(const string &message) : LoanException(message) {}

LoanBuilderModificationException::LoanBuilderModificationException(const string &message) : LoanBuilderException(message) {}

LoanBuilderBuildException::LoanBuilderBuildException(const string &message) : LoanBuilderException(message) {}

LoanManagerException::LoanManagerException(const string &message) : LoanException(message) {}

LoanManagerConstructionException::LoanManagerConstructionException(const string &message) : LoanManagerException(message) {}

LoanManagerMethodException::LoanManagerMethodException(const string &message) : LoanManagerException(message) {}
