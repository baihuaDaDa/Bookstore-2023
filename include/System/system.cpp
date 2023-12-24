#include "system.hpp"

void System::initialise() {
    if (time == 0) {
        User shopkeeper = User("root", UserInfo("sjtu", "baihua", SHOPKEEPER));
        user_memory.Insert(shopkeeper.ID, shopkeeper.user_info);
        time++;
        LogRecord new_log = LogRecord(time, ConstLenStr<280>("initialise shopkeeper baihua"));
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
        if (scanner.show_what == ISBN) {
            std::vector<Pair<ConstLenStr<21>, ISBNIndexingInfo>> data_isbn = isbn_memory.Find(scanner.isbn);
            if (data_isbn.empty()) {
                std::cout << '\n';
            } else {
                for (auto iter = data_isbn.begin(); iter != data_isbn.end(); iter++) {
                    std::cout << iter->index << '\t' << iter->value << '\n';
                }
            }
        } else if (scanner.show_what == BOOK_NAME) {
            std::vector<Pair<ConstLenStr<61>, BookNameIndexingInfo>> data_book_name = book_name_memory.Find(
                    scanner.book_name);
            if (data_book_name.empty()) {
                std::cout << '\n';
            } else {
                for (auto iter = data_book_name.begin(); iter != data_book_name.end(); iter++) {
                    std::cout << iter->index << '\t' << iter->value << '\n';
                }
            }
        } else if (scanner.show_what == AUTHOR_NAME) {
            std::vector<Pair<ConstLenStr<61>, AuthorNameIndexingInfo>> data_author_name = author_name_memory.Find(
                    scanner.author_name);
            if (data_author_name.empty()) {
                std::cout << '\n';
            } else {
                for (auto iter = data_author_name.begin(); iter != data_author_name.end(); iter++) {
                    std::cout << iter->index << '\t' << iter->value << '\n';
                }
            }
        } else if (scanner.show_what == SINGLE_KEYWORD) {
            std::vector<Pair<ConstLenStr<61>, KeywordIndexingInfo>> data_keyword = keyword_memory.Find(
                    scanner.single_keyword);
            if (data_keyword.empty()) {
                std::cout << '\n';
            } else {
                for (auto iter = data_keyword.begin(); iter != data_keyword.end(); iter++) {
                    std::cout << iter->index << '\t' << iter->value << '\n';
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
            data[0].value.storage -= scanner.quantity;
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
        ISBNIndexingInfo new_info;
        user_list.back().value = Book(scanner.isbn);
        isbn_memory.Insert(scanner.isbn, new_info);
    } else {
        Book selected_book = Book(scanner.isbn, data[0].value.book_name,
                                  data[0].value.author_name, data[0].value.keyword,
                                  data[0].value.price, data[0].value.storage);
        user_list.back().value = selected_book;
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
            if (scanner.modify_list[ISBN]) {
                if (book.isbn == scanner.isbn) {
                    std::cout << "Invalid\n";
                    return;
                }
            }
            std::vector<ConstLenStr<61>> current_keyword_list;
            const int max_len = 60;
            ConstLenStr<61> tmp = {};
            for (int i = 0; i < book.keyword.GetSize(); i++) {
                if (book.keyword[i] == '|') {
                    current_keyword_list.push_back(tmp);
                    keyword_memory.Delete(tmp, KeywordIndexingInfo(book.isbn, book.book_name,
                                                                   book.author_name, book.price, book.storage));
                    tmp.Clear();
                } else {
                    tmp.Append(book.keyword[i]);
                }
            }
            isbn_memory.Delete(book.isbn,
                               ISBNIndexingInfo(book.book_name, book.author_name,
                                                book.keyword, book.price, book.storage));
            if (!(book.book_name == ConstLenStr<61>()))
                book_name_memory.Delete(book.book_name, BookNameIndexingInfo(book.isbn, book.author_name,
                                                                             book.keyword, book.price, book.storage));
            if (!(book.author_name == ConstLenStr<61>()))
                author_name_memory.Delete(book.author_name,
                                          AuthorNameIndexingInfo(book.isbn, book.book_name, book.keyword,
                                                                 book.price, book.storage));
            if (scanner.modify_list[ISBN]) {
                book.isbn = scanner.isbn;
            } else if (scanner.modify_list[BOOK_NAME]) {
                book.book_name = scanner.book_name;
            } else if (scanner.modify_list[AUTHOR_NAME]) {
                book.author_name = scanner.author_name;
            } else if (scanner.modify_list[KEYWORD]) {
                book.keyword = scanner.keyword;
            } else if (scanner.modify_list[PRICE]) {
                book.price = scanner.price;
            }
            ISBNIndexingInfo new_isbn_info = ISBNIndexingInfo(book.book_name, book.author_name,
                                                              book.keyword, book.price, book.storage);
            BookNameIndexingInfo new_book_name_info = BookNameIndexingInfo(book.isbn, book.author_name,
                                                                           book.keyword, book.price, book.storage);
            AuthorNameIndexingInfo new_author_name_info = AuthorNameIndexingInfo(book.isbn, book.book_name,
                                                                                 book.keyword,
                                                                                 book.price, book.storage);
            KeywordIndexingInfo new_keyword_info = KeywordIndexingInfo(book.isbn, book.book_name,
                                                                       book.author_name, book.price, book.storage);
            isbn_memory.Insert(book.isbn, new_isbn_info);
            book_name_memory.Insert(book.book_name, new_book_name_info);
            author_name_memory.Delete(book.author_name, new_author_name_info);
            for (auto iter = scanner.keyword_list.begin(); iter != scanner.keyword_list.end(); iter++) {
                keyword_memory.Insert(*iter, new_keyword_info);
            }
        }
    }
}

void System::Import() {
    if (user_list.empty() || user_list.back().index.user_info.privilege <= 1) {
        std::cout << "Invalid\n";
    } else {

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
    return if_exit;
}
