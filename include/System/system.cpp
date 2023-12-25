#include "system.hpp"

void System::Initialise() {
    if (time == 0) {
        User shopkeeper = User("root", UserInfo("sjtu", "baihua", SHOPKEEPER));
        user_memory.Insert(shopkeeper.ID, shopkeeper.user_info);
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("initialise shopkeeper baihua"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>("baihua", new_log);
        log_memory.write(new_user_log_record);
    }
}

void System::Su() {
    std::vector<Pair<ConstLenStr<31>, UserInfo>> data = user_memory.Find(scanner.ID);
    if (data.empty()) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: the user does not exist"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
        log_memory.write(new_user_log_record);
        return;
    }
    if (scanner.factor_num == 2) {
        if (!(scanner.password == data[0].value.password)) {
            std::cout << "Invalid\n";
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: the password is wrong"));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
            log_memory.write(new_user_log_record);
//            std::cout << scanner.ID << ' ' << scanner.password;
        } else {
            User user = User(scanner.ID, data[0].value);
            user_list.emplace_back(user, ConstLenStr<21>());
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
            log_memory.write(new_user_log_record);
        }
    } else {
        if (!user_list.empty() && user_list.back().index.user_info.privilege > data[0].value.privilege) {
            User user = User(scanner.ID, data[0].value);
            user_list.emplace_back(user, Book());
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    user_list.back().index.ID, new_log);
            log_memory.write(new_user_log_record);
        } else {
            std::cout << "Invalid\n";
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: have not input the password"));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
            log_memory.write(new_user_log_record);
        }
    }
}

void System::Register() {
    std::vector<Pair<ConstLenStr<31>, UserInfo>> data = user_memory.Find(scanner.ID);
    if (!data.empty()) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: the user ID already exists"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
        log_memory.write(new_user_log_record);
        return;
    }
    UserInfo new_user_info = UserInfo(scanner.password, scanner.username, CUSTOMER);
    user_memory.Insert(scanner.ID, new_user_info);
    LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
    Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
            (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
    log_memory.write(new_user_log_record);
}

void System::Logout() {
    if (user_list.empty()) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: no user has been logged"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>("Visitor", new_log);
        log_memory.write(new_user_log_record);
    } else {
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                user_list.back().index.ID, new_log);
        log_memory.write(new_user_log_record);
        user_list.pop_back();
        if (!user_list.empty() && !(user_list.back().value == Book())) {
            user_list.back().value.isbn_indexing_info = isbn_memory.Find(user_list.back().value.isbn)[0].value;
        }
    }
}

void System::Passwd() {
    if (user_list.empty()) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: no user has logged"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>("Visitor", new_log);
        log_memory.write(new_user_log_record);
        return;
    }
    std::vector<Pair<ConstLenStr<31>, UserInfo>> data = user_memory.Find(scanner.ID);
    if (data.empty()) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: the user does not exist"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
        log_memory.write(new_user_log_record);
        return;
    }
    if (scanner.factor_num == 3) {
        if (!(scanner.password == data[0].value.password)) {
            std::cout << "Invalid\n";
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: the current password is wrong"));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    user_list.back().index.ID, new_log);
            log_memory.write(new_user_log_record);
        } else {
            for (auto iter = user_list.begin(); iter != user_list.end(); iter++) {
                if (iter->index.ID == scanner.ID) {
                    iter->index.user_info.password = scanner.new_password;
                }
            }
            user_memory.Delete(data[0].index, data[0].value);
            data[0].value.password = scanner.new_password;
            user_memory.Insert(data[0].index, data[0].value);
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    user_list.back().index.ID, new_log);
            log_memory.write(new_user_log_record);
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
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    user_list.back().index.ID, new_log);
            log_memory.write(new_user_log_record);
        } else {
            std::cout << "Invalid\n";
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: have not input the current password"));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    user_list.back().index.ID, new_log);
            log_memory.write(new_user_log_record);
        }
    }
}

void System::Useradd() {
    if (user_list.empty() || user_list.back().index.user_info.privilege <= 1) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: do not have the privilege"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
        log_memory.write(new_user_log_record);
    } else {
        if (scanner.privilege >= user_list.back().index.user_info.privilege) {
            std::cout << "Invalid\n";
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: do not have the privilege"));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    user_list.back().index.ID, new_log);
            log_memory.write(new_user_log_record);
        } else {
            std::vector<Pair<ConstLenStr<31>, UserInfo>> data = user_memory.Find(scanner.ID);
            if (!data.empty()) {
                std::cout << "Invalid\n";
                LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: the user ID already exists"));
                Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                        user_list.back().index.ID, new_log);
                log_memory.write(new_user_log_record);
            } else {
                UserInfo new_user(scanner.password, scanner.username, Privilege(scanner.privilege));
                user_memory.Insert(scanner.ID, new_user);
                LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
                Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                        user_list.back().index.ID, new_log);
                log_memory.write(new_user_log_record);
                if (user_list.back().index.user_info.privilege == EMPLOYEE)
                    employee_work_memory.Insert(user_list.back().index.ID, new_log);
            }
        }
    }
}

void System::Delete() {
    if (user_list.empty() || user_list.back().index.user_info.privilege != SHOPKEEPER) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: do not have the privilege"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
        log_memory.write(new_user_log_record);
    } else {
        std::vector<Pair<ConstLenStr<31>, UserInfo>> data = user_memory.Find(scanner.ID);
        if (data.empty()) {
            std::cout << "Invalid\n";
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: the user does not exist"));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    user_list.back().index.ID, new_log);
            log_memory.write(new_user_log_record);
        } else {
            for (auto iter = user_list.begin(); iter != user_list.end(); iter++) {
                if (iter->index.ID == scanner.ID) {
                    std::cout << "Invalid\n";
                    LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: the user has already logged"));
                    Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                            user_list.back().index.ID, new_log);
                    log_memory.write(new_user_log_record);
                    return;
                }
            }
            user_memory.Delete(scanner.ID, data[0].value);
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    user_list.back().index.ID, new_log);
            log_memory.write(new_user_log_record);
        }
    }
}

void System::Show() {
    if (user_list.empty()) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: do not have the privilege"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>("Visitor", new_log);
        log_memory.write(new_user_log_record);
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
    LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
    Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
            user_list.back().index.ID, new_log);
    log_memory.write(new_user_log_record);
}

void System::Buy() {
    if (user_list.empty()) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: do not have the privilege"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>("Visitor", new_log);
        log_memory.write(new_user_log_record);
        return;
    }
    std::vector<Pair<ConstLenStr<21>, ISBNIndexingInfo>> data = isbn_memory.Find(scanner.isbn);
    if (data.empty()) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: the book does not exist"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                user_list.back().index.ID, new_log);
        log_memory.write(new_user_log_record);
    } else {
        if (data[0].value.storage < scanner.quantity || scanner.quantity == 0) {
            std::cout << "Invalid\n";
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>(
                    (scanner.quantity == 0) ? "Invalid: the quantity is not positive"
                                            : "Invalid: the storage of the book is insufficient"));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    user_list.back().index.ID, new_log);
            log_memory.write(new_user_log_record);
        } else {
            std::cout << scanner.quantity * data[0].value.price << '\n';
            isbn_memory.Delete(data[0].index, data[0].value);
            data[0].value.storage -= scanner.quantity;
            if (user_list.back().value.isbn == data[0].index) {
                user_list.back().value.isbn_indexing_info.storage -= scanner.quantity;
            }
            isbn_memory.Insert(data[0].index, data[0].value);
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
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    user_list.back().index.ID, new_log);
            log_memory.write(new_user_log_record);
            FinanceRecord new_trade = FinanceRecord(time, ConstLenStr<40>(scanner.simplified_buffer));
            Pair<ConstLenStr<31>, FinanceRecord> new_user_trade = Pair<ConstLenStr<31>, FinanceRecord>(
                    user_list.back().index.ID, new_trade);
            finance_report_memory.write(new_user_trade);
        }
    }
}

void System::Select() {
    if (user_list.empty() || user_list.back().index.user_info.privilege <= 1) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: do not have the privilege"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
        log_memory.write(new_user_log_record);
        return;
    }
    std::vector<Pair<ConstLenStr<21>, ISBNIndexingInfo>> data = isbn_memory.Find(scanner.isbn);
    if (data.empty()) {
        user_list.back().value = Book(scanner.isbn);
        isbn_memory.Insert(scanner.isbn, user_list.back().value.isbn_indexing_info);
    } else {
        user_list.back().value = Book(scanner.isbn, data[0].value);
    }
    LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
    Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
            user_list.back().index.ID, new_log);
    log_memory.write(new_user_log_record);
    if (user_list.back().index.user_info.privilege == EMPLOYEE)
        employee_work_memory.Insert(user_list.back().index.ID, new_log);
}

void System::Modify() {
    if (user_list.empty() || user_list.back().index.user_info.privilege <= 1) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: do not have the privilege"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
        log_memory.write(new_user_log_record);
    } else {
        Book &book = user_list.back().value;
        if (book == Book()) {
            std::cout << "Invalid\n";
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: have not selected any book"));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    user_list.back().index.ID, new_log);
            log_memory.write(new_user_log_record);
        } else {
            isbn_memory.Delete(book.isbn, book.isbn_indexing_info);
            if (scanner.modify_list[ISBN]) {
                if (book.isbn == scanner.isbn) {
                    std::cout << "Invalid\n";
                    LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: the ISBN has not been changed"));
                    Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                            user_list.back().index.ID, new_log);
                    log_memory.write(new_user_log_record);
                    return;
                }
                std::vector<Pair<ConstLenStr<21>, ISBNIndexingInfo>> data_isbn = isbn_memory.Find(scanner.isbn);
                if (!data_isbn.empty()) {
                    std::cout << "Invalid\n";
                    LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: the ISBN already exists"));
                    Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                            user_list.back().index.ID, new_log);
                    log_memory.write(new_user_log_record);
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
                keyword_memory.Delete(tmp, book.isbn);
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
                    ConstLenStr<61> tmp = {};
                    for (int i = 0; i < book.isbn_indexing_info.keyword.GetSize(); i++) {
                        if (book.isbn_indexing_info.keyword[i] == '|') {
                            keyword_memory.Delete(tmp, book.isbn);
                            tmp.Clear();
                        } else {
                            tmp.Append(book.isbn_indexing_info.keyword[i]);
                        }
                    }
                    keyword_memory.Delete(tmp, book.isbn);
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
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    user_list.back().index.ID, new_log);
            log_memory.write(new_user_log_record);
            if (user_list.back().index.user_info.privilege == EMPLOYEE)
                employee_work_memory.Insert(user_list.back().index.ID, new_log);
        }
    }
}

void System::Import() {
    if (user_list.empty() || user_list.back().index.user_info.privilege <= 1) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: do not have the privilege"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
        log_memory.write(new_user_log_record);
    } else {
        Book &book = user_list.back().value;
        if (book == Book()) {
            std::cout << "Invalid\n";
            LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: have not selected any book"));
            Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                    user_list.back().index.ID, new_log);
            log_memory.write(new_user_log_record);
        } else {
            if (scanner.quantity == 0 || scanner.total_cost == 0) {
                std::cout << "Invalid\n";
                LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: the import data is illegal"));
                Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                        user_list.back().index.ID, new_log);
                log_memory.write(new_user_log_record);
            } else {
                isbn_memory.Delete(book.isbn, book.isbn_indexing_info);
                book.isbn_indexing_info.storage += scanner.quantity;
                isbn_memory.Insert(book.isbn, book.isbn_indexing_info);
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
                LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
                Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                        user_list.back().index.ID, new_log);
                log_memory.write(new_user_log_record);
                if (user_list.back().index.user_info.privilege == EMPLOYEE)
                    employee_work_memory.Insert(user_list.back().index.ID, new_log);
                FinanceRecord new_trade = FinanceRecord(time, ConstLenStr<40>(scanner.simplified_buffer));
                Pair<ConstLenStr<31>, FinanceRecord> new_user_trade = Pair<ConstLenStr<31>, FinanceRecord>(
                        user_list.back().index.ID, new_trade);
                finance_report_memory.write(new_user_trade);
            }
        }
    }
}

void System::ShowFinance() {
    if (user_list.empty() || user_list.back().index.user_info.privilege != SHOPKEEPER) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: do not have the privilege"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
        log_memory.write(new_user_log_record);
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
                    LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: there are not so many trade entries"));
                    Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                            user_list.back().index.ID, new_log);
                    log_memory.write(new_user_log_record);
                    return;
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
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                user_list.back().index.ID, new_log);
        log_memory.write(new_user_log_record);
    }
}

void System::Log() {
    if (user_list.empty() || user_list.back().index.user_info.privilege != SHOPKEEPER) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: do not have the privilege"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
        log_memory.write(new_user_log_record);
    } else {
        Pair<ConstLenStr<31>, LogRecord> log_info;
        for (int i = 1; i <= time; i++) {
            log_memory.read(log_info, (i - 1) * sizeof(Pair<ConstLenStr<31>, LogRecord>) + sizeof(int));
            std::cout << "Time: " << log_info.value.time << '\n' << "User: " << log_info.index << '\n' << "Operation: " << log_info.value.work_record << '\n';
        }
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                user_list.back().index.ID, new_log);
        log_memory.write(new_user_log_record);
    }
}

void System::ReportFinance() {
    if (user_list.empty() || user_list.back().index.user_info.privilege != SHOPKEEPER) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: do not have the privilege"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
        log_memory.write(new_user_log_record);
    } else {
        Pair<ConstLenStr<31>, FinanceRecord> finance_info;
        for (int i = 1; i <= finance_count; i++) {
            finance_report_memory.read(finance_info, (i - 1) * sizeof(Pair<ConstLenStr<31>, FinanceRecord>));
            std::cout << "Time: " << finance_info.value.time << '\n' << "User: " << finance_info.index << '\n' << "Operation: " << finance_info.value.finance_record << '\n';
        }
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                user_list.back().index.ID, new_log);
        log_memory.write(new_user_log_record);
    }
}

template<>
void memory<ConstLenStr<31>, LogRecord, CmpStr, CmpLogRecord>::Output() {
    memory_BlockNode.read(head, head_pos);
    int pos = head.next;
    BlockNode now;
    fstream file;
    Pair<ConstLenStr<31>, LogRecord> data[SIZE_OF_BLOCK] = {};
    while (pos != tail_pos) {
        memory_BlockNode.read(now, pos);
        file.open(element_file_name, std::ios::in);
        file.seekg(now.address);
        file.read(reinterpret_cast<char *>(data), sizeof(Pair<ConstLenStr<31>, LogRecord>) * now.size);
        file.close();
        for (int i = 0; i < now.size; i++) {
            std::cout << "Time: " << data[i].value.time << '\n' << "User: " << data[i].index << '\n' << "Operation: " << data[i].value.work_record << '\n';
        }
        pos = now.next;
    }
}

void System::ReportEmployee() {
    if (user_list.empty() || user_list.back().index.user_info.privilege != SHOPKEEPER) {
        std::cout << "Invalid\n";
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: do not have the privilege"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
        log_memory.write(new_user_log_record);
    } else {
        employee_work_memory.Output();
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>(scanner.simplified_buffer));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                user_list.back().index.ID, new_log);
        log_memory.write(new_user_log_record);
    }
}

void System::GetInstruction() {
    std::string _buffer;
    getline(std::cin, _buffer);
//    std::cout << _buffer << '\n';
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
        LogRecord new_log = LogRecord(++time, ConstLenStr<280>("Invalid: illegal instruction"));
        Pair<ConstLenStr<31>, LogRecord> new_user_log_record = Pair<ConstLenStr<31>, LogRecord>(
                (user_list.empty() ? "Visitor" : user_list.back().index.ID), new_log);
        log_memory.write(new_user_log_record);
    }
}

bool System::Exit() {
    if (if_exit || std::cin.eof()) {
        log_memory.write_info(time, 1);
        finance_memory.write_info(finance_count, 1);
        return true;
    } else return false;
}
