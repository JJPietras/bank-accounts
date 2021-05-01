//
// Created by student on 12.01.2020.
//

#ifndef BANKKONTA_LOANEXCEPTIONS_H
#define BANKKONTA_LOANEXCEPTIONS_H

#include <string>
#include <stdexcept>

//=============================== LOAN ===============================//

class LoanException : public std::logic_error {
public:
    explicit LoanException(const std::string &message);
};

class LoanConstructionException : public LoanException {
public:
    explicit LoanConstructionException(const std::string &message);
};

class LoanModificationException : public LoanException {
public:
    explicit LoanModificationException(const std::string &message);
};

//============================== DERIVED =============================//

class ConsolidationLoanException : public LoanException {
public:
    explicit ConsolidationLoanException(const std::string &message);
};

class ConsolidationLoanConstructionException : public ConsolidationLoanException {
public:
    explicit ConsolidationLoanConstructionException(const std::string &message);
};

class ConsolidationLoanMethodException : public ConsolidationLoanException {
public:
    explicit ConsolidationLoanMethodException(const std::string &message);
};

//========================== LOAN REPOSITORY =========================//

class LoanRepositoryException : public LoanException {
public:
    explicit LoanRepositoryException(const std::string &message);
};

//=========================== LOAN BUILDER ===========================//

class LoanBuilderException : public LoanException {
public:
    explicit LoanBuilderException(const std::string &message);
};

class LoanBuilderModificationException : public LoanBuilderException {
public:
    explicit LoanBuilderModificationException(const std::string &message);
};

class LoanBuilderBuildException : public LoanBuilderException {
public:
    explicit LoanBuilderBuildException(const std::string &message);
};

//=========================== LOAN MANAGER ===========================//

class LoanManagerException : public LoanException {
public:
    explicit LoanManagerException(const std::string &message);
};

class LoanManagerConstructionException : public LoanManagerException {
public:
    explicit LoanManagerConstructionException(const std::string &message);
};

class LoanManagerMethodException : public LoanManagerException {
public:
    explicit LoanManagerMethodException(const std::string &message);
};

#endif //BANKKONTA_LOANEXCEPTIONS_H
