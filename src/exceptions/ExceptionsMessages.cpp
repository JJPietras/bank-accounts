//
// Created by student on 01.01.2020.
//

#include <string>

///////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       //
//                                      EXCEPTIONS                                       //
//                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////


//======================================= AMOUNT ========================================//

static const std::string SIGNS = "Number have opposite signs.";
static const std::string FRACTIONAL = "Too long fractional unit.";
static const std::string CURRENCY = "Amounts have different currencies.";

//======================================= ADDRESS =======================================//

static const std::string EMPTY_ADDRESS = "Street name or number string is empty.";
static const std::string NULL_ADDRESS = "Provided address is nullptr.";

//======================================= ACCOUNT =======================================//

static const std::string NOT_DATE_TIME = "Provided local_date_time is not a date time.";
static const std::string NULL_DATE_TIME = "Provided local_date_time is nullptr.";
static const std::string LESS_THAN_TWO = "There must be at least two owners of the Joint Account.";
static const std::string NO_OWNER_CLOSED = "Account already closed or has no owner.";

//===================================== CLIENT TYPE =====================================//

static const std::string NEGATIVE_MAX_AMOUNT = "Maximum amount (cost) of service that user can acquire is too low (less than 0.00).";
static const std::string WRONG_MULTIPLIER = "Service cost multiplier is inappropriate. Should be within 0.1 - 1.0.";
static const std::string NEGATIVE_TIER = "Base service tier is too low (less than 0).";
static const std::string NULL_CLIENT_TYPE = "Provided ClientType is nullptr.";
static const std::string NULL_AMOUNT = "Provided Amount is nullptr.";

//======================================== CLIENT =======================================//

static const std::string EMPTY_FIRST_NAME = "First name string is empty.";
static const std::string EMPTY_LAST_NAME = "Last name string is empty.";
static const std::string EMPTY_PERSONAL_ID = "Personal ID string is empty.";
static const std::string NEGATIVE_INCOME = "Provided income is less than 0.00.";

//========================================= LOAN ========================================//

static const std::string NULL_LOAN = "Provided loan is nullptr.";
static const std::string NULL_OWNER = "Provided owner is nullptr.";
static const std::string WRONG_BORROW_AMOUNT = "Borrowed amount cannot be less than 10.00 (USD, GBP, EUR, PLN).";
static const std::string NEGATIVE_AMOUNT = "Amount cannot be negative.";
static const std::string TOO_FEW_LOANS = "There must be more than one loan to consolidate.";
static const std::string MULTIPLE_CURRENCIES = "Loans must have the same currency type.";
static const std::string MULTIPLE_BORROWERS = "Loans must have the same borrower.";
static const std::string SAME_LOANS = "There are at least two identical loans in the collection";
static const std::string REPAID = "Loans was already repaid.";

//========================================= CARD ========================================//

static const std::string NULL_CARD = "Provided card is nullptr.";
static const std::string NULL_ACCOUNT = "Provided account is nullptr.";
static const std::string NULL_CLIENT = "Provided client is nullptr.";
static const std::string NULL_CARD_CLASS = "Provided card class is nullptr.";

//====================================== REPOSITORIES ===================================//

static const std::string EMPTY_COLLECTION = "Element cannot be removed, repository is empty.";
static const std::string CLIENT_EXISTS = "Client already exists in the repository.";
static const std::string LOAN_EXISTS = "Loan already exists in the repository.";
static const std::string ACCOUNT_EXISTS = "Account already exists in the repository.";
static const std::string CARD_EXISTS = "Card already exists in the repository.";

//======================================== BUILDERS =====================================//

static const std::string CLIENT_CRUCIAL_MISSING = "One of primary client fields was not specified.";
static const std::string LOAN_CRUCIAL_MISSING = "One of primary loan fields was not specified.";
static const std::string ACCOUNT_CRUCIAL_MISSING = "One of primary account fields was not specified";
static const std::string CARD_CRUCIAL_MISSING = "One of primary card fields was not specified";
static const std::string NULL_UUID = "Provided UUID is nullptr.";
static const std::string EMPTY_CONSOLIDATED_LOANS = "There are no consolidated loans.";
static const std::string NULL_LOAN_TYPE = "Provided loan type enum pointer is nullptr.";
static const std::string NULL_ACCOUNT_TYPE = "Provided account type enum pointer is nullptr.";
static const std::string NULL_CARD_TYPE = "Provided card type enum pointer is nullptr.";
static const std::string NULL_CURRENCY = "Provided currency is nullptr.";
static const std::string NULL_MAX_OWNERS = "Provided maxOwners is nullptr";
static const std::string ONE_LESS_OWNERS = "Maximum Owners must be greater than one";
static const std::string TOO_MANY_OWNERS = "There are too many owners for this account";
static const std::string WRONG_CURRENCY = "This type of account should have PLN currency deposit";
static const std::string WRONG_ACCOUNT = "Credit card can only be associated with CurrencyAccount";

//====================================== REPOSITORIES ===================================//

static const std::string NULL_REPO = "At least one of the parameters (repositories) is nullptr.";
static const std::string ELEMENT_IN_REPO = "Provided element is already a part of one of the repositories.";
static const std::string ELEMENT_NOT_REMOVED = "Element was not present in any repository.";
static const std::string BUILDER_EXCEPTION = "Builder could not build the object.";
static const std::string TOO_EXPENSIVE = "Client cannot purchase such expensive service.";
static const std::string TOO_HIGH_TIER = "Calculated tier for this service exceeds client's limit.";
static const std::string NULL_BUILDER = "Provided builder is nullptr.";
static const std::string CLOSED_REPO = "Account cannot be created in closed accounts repository";
static const std::string NO_CLIENT_IN_ACCOUNT = "Provided client is not the owner of this account.";
static const std::string NO_ACCOUNT_IN_CLIENT = "Provided account is not present in client's list";
static const std::string WRONG_REPO = "Provided card cannot be moved into selected repository";
static const std::string WRONG_AMOUNT = "Provided amount is either negative or nullptr or has different currency";
static const std::string NOT_CREDIT = "Provided card is not a credit card";