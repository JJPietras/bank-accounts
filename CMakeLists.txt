cmake_minimum_required(VERSION 3.4)

project(Library)

set(SOURCE_FILES
        include/model/Amount.h
        include/model/ClientType.h
        include/model/Client.h
        include/model/Address.h
        include/model/Account.h
        include/model/CurrencyAccount.h
        include/model/SavingsAccount.h
        include/model/JointAccount.h
        include/model/Card.h
        include/model/DebitCard.h
        include/model/CreditCard.h
        include/model/Loan.h
        include/model/Mortgage.h
        include/model/StandardLoan.h
        include/model/ConsolidationLoan.h
        include/repository/Repository.h
        include/repository/ClientRepository.h
        include/repository/LoanRepository.h
        include/repository/AccountRepository.h
        include/repository/CardRepository.h
        include/manager/AccountManager.h
        include/manager/CardManager.h
        include/manager/ClientManager.h
        include/manager/LoanManager.h
        include/builder/ClientBuilder.h
        include/builder/AccountBuilder.h
        include/builder/CardBuilder.h
        include/builder/LoanBuilder.h
        include/model/TrustworthyClient.h
        include/model/NewClient.h
        include/model/CommonDebtor.h
        include/model/NormalClient.h
        include/model/CardClass.h
        include/model/StandardCard.h
        include/model/TitaniumCard.h
        include/model/PlatinumCard.h
        include/model/GoldCard.h
        include/model/SilverCard.h
	include/enum/CurrencyType.h
        include/enum/AccountEnum.h
        include/enum/CardEnum.h
        include/enum/LoanEnum.h
        include/enum/CardTypesEnum.h
        include/enum/ClientTypesEnum.h
	include/enum/ClientRepositoriesEnum.h 
	include/enum/LoanRepositoriesEnum.h
	include/enum/CardRepositoriesEnum.h 
	include/enum/AccountRepositoriesEnum.h
        include/exceptions/ClientExceptions.h 
	include/exceptions/CardExceptions.h
	include/exceptions/LoanExceptions.h
	include/exceptions/AmountExceptions.h
	include/exceptions/AddressExceptions.h 
	

        src/model/Amount.cpp
        src/model/ClientType.cpp
        src/model/Client.cpp
        src/model/Address.cpp
        src/model/Account.cpp
        src/model/CurrencyAccount.cpp
        src/model/SavingsAccount.cpp
        src/model/JointAccount.cpp
        src/model/Card.cpp
        src/model/DebitCard.cpp
        src/model/CreditCard.cpp
        src/model/Loan.cpp
        src/model/Mortgage.cpp
        src/model/StandardLoan.cpp
        src/model/ConsolidationLoan.cpp
        src/repository/ClientRepository.cpp
        src/repository/LoanRepository.cpp
        src/repository/AccountRepository.cpp
        src/repository/CardRepository.cpp
        src/manager/AccountManager.cpp
        src/manager/CardManager.cpp
        src/manager/ClientManager.cpp
        src/manager/LoanManager.cpp
        src/builder/ClientBuilder.cpp
        src/builder/AccountBuilder.cpp
        src/builder/CardBuilder.cpp
        src/builder/LoanBuilder.cpp
        src/model/TrustworthyClient.cpp
        src/model/NewClient.cpp
        src/model/CommonDebtor.cpp
        src/model/NormalClient.cpp
        src/model/CardClass.cpp
        src/model/StandardCard.cpp
        src/model/TitaniumCard.cpp
        src/model/PlatinumCard.cpp
        src/model/GoldCard.cpp
        src/model/SilverCard.cpp
        src/exceptions/ExceptionsMessages.cpp 
	src/exceptions/ClientExceptions.cpp 
	src/exceptions/AccountExceptions.cpp
        src/exceptions/CardExceptions.cpp 
        src/exceptions/LoanExceptions.cpp 
	src/exceptions/AmountExceptions.cpp
	src/exceptions/AddressExceptions.cpp)

#utwórz bibliotekę typu STATIC, SHARED albo MODULE ze wskazanych źródeł
add_library(Library ${SOURCE_FILES})

target_include_directories(Library PUBLIC include)

#Włączenie testów powoduje pojawienie się celu test w makefiles
enable_testing()# dodane wcześniej w punkcie 2

find_package(Boost 1.60.0 COMPONENTS "unit_test_framework" REQUIRED)

include_directories(
        include
        ${Boost_INCLUDE_DIRS}
)

set(SOURCE_TEST_FILES
        test/master.cpp
        test/AmountTest.cpp
        test/AddressTest.cpp test/ClientTypeTest.cpp test/ClientTest.cpp test/AccountTest.cpp test/LoanTest.cpp test/CardClassTest.cpp test/CardTest.cpp test/RepositoryTest.cpp test/BuilderTest.cpp test/ManagerTest.cpp)

add_executable(TestLib ${SOURCE_TEST_FILES})

#dołącz bibliotekę z testowanymi klasami do programu testów
target_link_libraries(TestLib
        Library
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )

#dodaj test TestBiblioteki1. Instrukcji cmake add_test może być wiele.
#Dodatkowe parametry wpływają na szczegółowość generowanego raportu. Standardowo znajduje się on w Testing/Temporary.
add_test(LibTest1 TestLib
        --report_level=detailed
        --log_level=all
        --color_output=yes
        )


add_custom_target(check ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1 BOOST_TEST_LOG_LEVEL=all
        ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> --verbose
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})