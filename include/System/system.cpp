#include "system.hpp"

void System::Initialise() {
    if (time == 0) {
        User shopkeeper = User("root", UserInfo("sjtu", "baihua", SHOPKEEPER));
        user_memory.Insert(shopkeeper.ID, shopkeeper.user_info);
        time++;
        LogRecord new_log = LogRecord(time, ConstLenStr<280>("Initialise shopkeeper baihua\n"));
        log_memory.write(new_log);
        log_memory.write_info(time, 1);
    }
}

void System::Su() {
    std::vector<Pair<ConstLenStr<31>, UserInfo>> data = user_memory.Find(scanner.ID);
    if (data.empty()) {
        std::cout << "Invalid\n";
        return;
    }
    if (scanner.factor_num == 2) {
        if (!(scanner.password == data[0].value.password)) {
            std::cout << "Invalid\n";
//            std::cout << scanner.ID << ' ' << scanner.password;
        } else {
            User user = User(scanner.ID, data[0].value);
            user_list.emplace_back(user, ConstLenStr<21>());
        }
    } else {
        if (user_list.back().index.user_info.privilege > data[0].value.privilege) {
            User user = User(scanner.ID, data[0].value);
            user_list.emplace_back(user, Book());
        } else {
            std::cout << "Invalid\n";
        }
    }
}

void System::Register() {
    std::vector<Pair<ConstLenStr<31>, UserInfo>> data = user_memory.Find(scanner.ID);
    if (!data.empty()) {
        std::cout << "Invalid\n";
        return;
    }
    UserInfo new_user_info = UserInfo(scanner.password, scanner.username, CUSTOMER);
    user_memory.Insert(scanner.ID, new_user_info);
}

void System::Logout() {
    if (user_list.empty()) {
        std::cout << "Invalid\n";
    } else {
        user_list.pop_back();
        if (!user_list.empty() && !(user_list.back().value == Book())) {
            user_list.back().value.isbn_indexing_info = isbn_memory.Find(user_list.back().value.isbn)[0].value;
        }
    }
}

void System::Passwd() {
    if (user_list.empty()) {
        std::cout << "Invalid\n";
        return;
    }
    std::vector<Pair<ConstLenStr<31>, UserInfo>> data = user_memory.Find(scanner.ID);
    if (data.empty()) {
        std::cout << "Invalid\n";
        return;
    }
    if (scanner.factor_num == 3) {
        if (!(scanner.password == data[0].value.password)) {
            std::cout << "Invalid\n";
        } else {
            for (auto iter = user_list.begin(); iter != user_list.end(); iter++) {
                if (iter->index.ID == scanner.ID) {
                    iter->index.user_info.password = scanner.new_password;
                }
            }
            user_memory.Delete(data[0].index, data[0].value);
            data[0].value.password = scanner.new_password;
            user_memory.Insert(data[0].index, data[0].value);
        }
    } else {
        if (user_list.back().index.user_info.privilege == SHOPKEEPER) {
            for (auto iter = user_list.begin(); iter != user_list.end(); iter++) {
                if (iter->index.ID == scanner.ID) {
                    iter->index.user_info.password = scanner.new_password;
                }
            }
            user_memory.Delete(data[0].index, data[0].value);
            data[0].value.password = scanner.new_password;
            user_memory.Insert(data[0].index, data[0].value);
        } else {
            std::cout << "Invalid\n";
        }
    }
}

void System::Useradd() {
    if (user_list.empty() || user_list.back().index.user_info.privilege <= 1) {
        std::cout << "Invalid\n";
    } else {
        if (scanner.privilege >= user_list.back().index.user_info.privilege) {
            std::cout << "Invalid\n";
        } else {
            std::vector<Pair<ConstLenStr<31>, UserInfo>> data = user_memory.Find(scanner.ID);
            if (!data.empty()) {
                std::cout << "Invalid\n";
            } else {
                UserInfo new_user(scanner.password, scanner.username, Privilege(scanner.privilege));
                user_memory.Insert(scanner.ID, new_user);
            }
        }
    }
}

void System::Delete() {
    if (user_list.empty() || user_list.back().index.user_info.privilege != SHOPKEEPER) {
        std::cout << "Invalid\n";
    } else {
        std::vector<Pair<ConstLenStr<31>, UserInfo>> data = user_memory.Find(scanner.ID);
        if (data.empty()) {
            std::cout << "Invalid\n";
        } else {
            for (auto iter = user_list.begin(); iter != user_list.end(); iter++) {
                if (iter->index.ID == scanner.ID) {
                    std::cout << "Invalid\n";
                    return;
                }
            }
            user_memory.Delete(scanner.ID, data[0].value);
        }
    }
}

void System::Show() {
    if (user_list.empty()) {
        std::cout << "Invalid\n";
        return;
    }
    if (scanner.factor_num == 0) {
        isbn_memory.Output();
    } else {
        std::vector<Pair<ConstLenStr<21>, ISBNIndexingInfo>> data_isbn = isbn_memory.Find(scanner.isbn);
        if (scanner.show_what == ISBN) {
            if (data_isbn.empty()) {
                std::cout << '\n';
            } else {
                for (auto iter = data_isbn.begin(); iter != data_isbn.end(); iter++) {
                    std::cout << iter->index << '\t' << iter->value << '\n';
                }
            }
        } else if (scanner.show_what == BOOK_NAME) {
            std::vector<Pair<ConstLenStr<61>, ConstLenStr<21>>> data_book_name = book_name_memory.Find(
                    scanner.book_name);
            if (data_book_name.empty()) {
                std::cout << '\n';
            } else {
                for (auto iter = data_book_name.begin(); iter != data_book_name.end(); iter++) {
                    data_isbn = isbn_memory.Find(iter->value);
                    std::cout << data_isbn[0].index << '\t' << data_isbn[0].value << '\n';
                }
            }
        } else if (scanner.show_what == AUTHOR_NAME) {
            std::vector<Pair<ConstLenStr<61>, ConstLenStr<21>>> data_author_name = author_name_memory.Find(
                    scanner.author_name);
            if (data_author_name.empty()) {
                std::cout << '\n';
            } else {
                for (auto iter = data_author_name.begin(); iter != data_author_name.end(); iter++) {
                    data_isbn = isbn_memory.Find(iter->value);
                    std::cout << data_isbn[0].index << '\t' << data_isbn[0].value << '\n';
                }
            }
        } else if (scanner.show_what == SINGLE_KEYWORD) {
            std::vector<Pair<ConstLenStr<61>, ConstLenStr<21>>> data_keyword = keyword_memory.Find(
                    scanner.single_keyword);
            if (data_keyword.empty()) {
                std::cout << '\n';
            } else {
                for (auto iter = data_keyword.begin(); iter != data_keyword.end(); iter++) {
                    data_isbn = isbn_memory.Find(iter->value);
                    std::cout << data_isbn[0].index << '\t' << data_isbn[0].value << '\n';
                }
            }
        }
    }
}

void System::Buy() {
    if (user_list.empty()) {
        std::cout << "Invalid\n";
        return;
    }
    std::vector<Pair<ConstLenStr<21>, ISBNIndexingInfo>> data = isbn_memory.Find(scanner.isbn);
    if (data.empty()) {
        std::cout << "Invalid\n";
    } else {
        if (data[0].value.storage < scanner.quantity || scanner.quantity == 0) {
            std::cout << "Invalid\n";
        } else {
            std::cout << scanner.quantity * data[0].value.price << '\n';
            isbn_memory.Delete(data[0].index, data[0].value);
            data[0].value.storage -= scanner.quantity;
            if (user_list.back().value.isbn == data[0].index) {
                user_list.back().value.isbn_indexing_info.storage -= scanner.quantity;
            }
            isbn_memory.Insert(data[0].index, data[0].value);
            finance_memory.get_info(finance_count, 1);
            if (finance_count == 0) {
                Finance first_trade = Finance(data[0].value.price * scanner.quantity, 0);
                finance_memory.write(first_trade);
                finance_count++;
                finance_memory.write_info(finance_count, 1);
            } else {
                Finance last_trade;
                finance_memory.read(last_trade, (finance_count - 1) * sizeof(Finance) + sizeof(int));
                last_trade.income += data[0].value.price * scanner.quantity;
                finance_memory.write(last_trade);
                finance_count++;
                finance_memory.write_info(finance_count, 1);
            }

        }
    }
}

void System::Select() {
    if (user_list.empty() || user_list.back().index.user_info.privilege <= 1) {
        std::cout << "Invalid\n";
        return;
    }
    std::vector<Pair<ConstLenStr<21>, ISBNIndexingInfo>> data = isbn_memory.Find(scanner.isbn);
    if (data.empty()) {
        user_list.back().value = Book(scanner.isbn);
        isbn_memory.Insert(scanner.isbn, user_list.back().value.isbn_indexing_info);
    } else {
        user_list.back().value = Book(scanner.isbn, data[0].value);
    }
}

void System::Modify() {
    if (user_list.empty() || user_list.back().index.user_info.privilege <= 1) {
        std::cout << "Invalid\n";
    } else {
        Book &book = user_list.back().value;
        if (book == Book()) {
            std::cout << "Invalid\n";
        } else {
            isbn_memory.Delete(book.isbn, book.isbn_indexing_info);
            if (scanner.modify_list[ISBN]) {
                if (book.isbn == scanner.isbn) {
                    std::cout << "Invalid\n";
                    return;
                }
                std::vector<Pair<ConstLenStr<21>, ISBNIndexingInfo>> data_isbn = isbn_memory.Find(scanner.isbn);
                if (!data_isbn.empty()) {
                    std::cout << "Invalid\n";
                    return;
                }
                ConstLenStr<61> tmp = {};
                for (int i = 0; i < book.isbn_indexing_info.keyword.GetSize(); i++) {
                    if (book.isbn_indexing_info.keyword[i] == '|') {
                        keyword_memory.Delete(tmp, book.isbn);
                        tmp.Clear();
                    } else {
                        tmp.Append(book.isbn_indexing_info.keyword[i]);
                    }
                }
                if (!(book.isbn_indexing_info.book_name == ConstLenStr<61>()))
                    book_name_memory.Delete(book.isbn_indexing_info.book_name, book.isbn);
                if (!(book.isbn_indexing_info.author_name == ConstLenStr<61>()))
                    author_name_memory.Delete(book.isbn_indexing_info.author_name, book.isbn);
                for (auto iter = user_list.begin(); iter != user_list.end(); iter++) {
                    if (iter->value.isbn == book.isbn) {
                        iter->value.isbn = scanner.isbn;
                    }
                }
            }
            if (scanner.modify_list[BOOK_NAME]) {
                if (!scanner.modify_list[ISBN]) {
                    if (!(book.isbn_indexing_info.book_name == ConstLenStr<61>()))
                        book_name_memory.Delete(book.isbn_indexing_info.book_name, book.isbn);
                }
                book.isbn_indexing_info.book_name = scanner.book_name;
            }
            if (scanner.modify_list[AUTHOR_NAME]) {
                if (!scanner.modify_list[ISBN]) {
                    if (!(book.isbn_indexing_info.author_name == ConstLenStr<61>()))
                        author_name_memory.Delete(book.isbn_indexing_info.author_name, book.isbn);
                }
                book.isbn_indexing_info.author_name = scanner.author_name;
            }
            if (scanner.modify_list[KEYWORD]) {
                if (!scanner.modify_list[ISBN]) {
                    const int max_len = 60;
                    ConstLenStr<61> tmp = {};
                    for (int i = 0; i < book.isbn_indexing_info.keyword.GetSize(); i++) {
                        if (book.isbn_indexing_info.keyword[i] == '|') {
                            keyword_memory.Delete(tmp, book.isbn);
                            tmp.Clear();
                        } else {
                            tmp.Append(book.isbn_indexing_info.keyword[i]);
                        }
                    }
                }
                book.isbn_indexing_info.keyword = scanner.keyword;
            }
            if (scanner.modify_list[PRICE]) {
                book.isbn_indexing_info.price = scanner.price;
            }
            isbn_memory.Insert(book.isbn, book.isbn_indexing_info);
            if (scanner.modify_list[ISBN]) {
                for (auto iter = scanner.keyword_list.begin(); iter != scanner.keyword_list.end(); iter++) {
                    keyword_memory.Insert(*iter, book.isbn);
                }
                if (!(book.isbn_indexing_info.book_name == ConstLenStr<61>()))
                    book_name_memory.Insert(book.isbn_indexing_info.book_name, book.isbn);
                if (!(book.isbn_indexing_info.author_name == ConstLenStr<61>()))
                    author_name_memory.Insert(book.isbn_indexing_info.author_name, book.isbn);
            }
            if (scanner.modify_list[BOOK_NAME]) {
                if (!scanner.modify_list[ISBN]) {
                    book_name_memory.Insert(book.isbn_indexing_info.book_name, book.isbn);
                }
            }
            if (scanner.modify_list[AUTHOR_NAME]) {
                if (!scanner.modify_list[ISBN]) {
                    author_name_memory.Insert(book.isbn_indexing_info.author_name, book.isbn);
                }
            }
            if (scanner.modify_list[KEYWORD]) {
                if (!scanner.modify_list[ISBN]) {
                    for (auto iter = scanner.keyword_list.begin(); iter != scanner.keyword_list.end(); iter++) {
                        keyword_memory.Insert(*iter, book.isbn);
                    }
                }
            }
        }
    }
}

void System::Import() {
    if (user_list.empty() || user_list.back().index.user_info.privilege <= 1) {
        std::cout << "Invalid\n";
    } else {
        Book &book = user_list.back().value;
        if (book == Book()) {
            std::cout << "Invalid\n";
        } else {
            if (scanner.quantity == 0 || scanner.total_cost == 0) {
                std::cout << "Invalid\n";
            } else {
                isbn_memory.Delete(book.isbn, book.isbn_indexing_info);
                book.isbn_indexing_info.storage += scanner.quantity;
                isbn_memory.Insert(book.isbn, book.isbn_indexing_info);
                finance_memory.get_info(finance_count, 1);
                if (finance_count == 0) {
                    Finance first_trade = Finance(0, scanner.total_cost);
                    finance_memory.write(first_trade);
                    finance_count++;
                    finance_memory.write_info(finance_count, 1);
                } else {
                    Finance last_trade;
                    finance_memory.read(last_trade, (finance_count - 1) * sizeof(Finance) + sizeof(int));
                    last_trade.expenditure += scanner.total_cost;
                    finance_memory.write(last_trade);
                    finance_count++;
                    finance_memory.write_info(finance_count, 1);
                }
            }
        }
    }
}

void System::ShowFinance() {
    if (user_list.empty() || user_list.back().index.user_info.privilege != SHOPKEEPER) {
        std::cout << "Invalid\n";
    } else {
        finance_memory.get_info(finance_count, 1);
        if (scanner.factor_num == 0) {
            if (finance_count == 0) {
                std::cout << "+ 0.00 - 0.00\n";
            } else {
                Finance last_trade;
                finance_memory.read(last_trade, (finance_count - 1) * sizeof(Finance) + sizeof(int));
                std::cout << "+ " << last_trade.income << " - " << last_trade.expenditure << '\n';
            }
        } else {
            if (scanner.count == 0) {
                std::cout << '\n';
            } else {
                if (scanner.count > finance_count) {
                    std::cout << "Invalid\n";
                } else if (scanner.count == finance_count) {
                    Finance last_trade;
                    finance_memory.read(last_trade, (finance_count - 1) * sizeof(Finance) + sizeof(int));
                    std::cout << "+ " << last_trade.income << " - " << last_trade.expenditure << '\n';
                } else {
                    Finance beginning_trade, ending_trade;
                    finance_memory.read(beginning_trade,
                                        (finance_count - scanner.count - 1) * sizeof(Finance) + sizeof(int));
                    finance_memory.read(ending_trade, (finance_count - 1) * sizeof(Finance) + sizeof(int));
                    std::cout << "+ " << ending_trade.income - beginning_trade.income <<
                              " - " << ending_trade.expenditure - beginning_trade.expenditure << '\n';
                }
            }
        }
    }
}

void System::Log() {

}

void System::ReportFinance() {

}

void System::ReportEmployee() {

}

void System::GetInstruction() {
    std::string _buffer;
    getline(std::cin, _buffer);
    std::cout << _buffer << '\n';
    scanner.SetBuffer(_buffer);
}

void System::Execute() {
    if (scanner.Scan()) {
        switch (scanner.instr_type) {
            case QUIT:
                if_exit = true;
                break;
            case EXIT:
                if_exit = true;
                break;
            case SU:
                Su();
                break;
            case LOGOUT:
                Logout();
                break;
            case REGISTER:
                Register();
                break;
            case PASSWD:
                Passwd();
                break;
            case USERADD:
                Useradd();
                break;
            case DELETE:
                Delete();
                break;
            case SHOW:
                Show();
                break;
            case BUY:
                Buy();
                break;
            case SELECT:
                Select();
                break;
            case MODIFY:
                Modify();
                break;
            case IMPORT:
                Import();
                break;
            case SHOW_FINANCE:
                ShowFinance();
                break;
            case LOG:
                Log();
                break;
            case REPORT_FINANCE:
                ReportFinance();
                break;
            case REPORT_EMPLOYEE:
                ReportEmployee();
                break;
            case EMPTY:
                break;
        }
    } else {
        std::cout << "Invalid\n";
    }
}

bool System::Exit() const {
    return (if_exit || std::cin.eof());
}
