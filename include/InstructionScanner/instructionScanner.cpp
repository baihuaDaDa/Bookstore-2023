#include "instructionScanner.hpp"

void instructionScanner::SetBuffer(const std::string &_buffer) {
    buffer = _buffer;
    keyword_list.clear();
    factor_num = 0;
    for (int i = 0; i < 5; i++)
        modify_list[i] = false;
}

bool instructionScanner::IsInteger(const std::string &token, InfoType info_type) {
    int max_len, tmp = 0;
    if (info_type == PRIVILEGE) max_len = 1;
    if (info_type == QUANTITY) max_len = 10;
    if (info_type == COUNT) max_len = 10;
    if (token.size() > max_len) return false;
    for (int i = 0; i < token.size(); i++) {
        if (!isdigit(token[i])) return false;
        tmp = tmp * 10 + token[i] - '0';
    }
    if (info_type == PRIVILEGE) privilege = tmp;
    if (info_type == QUANTITY) quantity = tmp;
    if (info_type == COUNT) count = tmp;
    return true;
}

bool instructionScanner::IsDouble(const std::string &token, InfoType info_type) {
    double tmp = 0;
    int after_dot_cnt = 1;
    const int max_len = 13;
    bool already_met_dot = false;
    if (token.size() > max_len) return false;
    for (int i = 0; i < token.size(); i++) {
        if (!isdigit(token[i]) && token[i] != '.') return false;
        if (token[i] == '.') {
            if (already_met_dot) return false;
            if (i == token.size() - 1 || i == 0) return false;
            already_met_dot = true;
            continue;
        }
        if (!already_met_dot) tmp = tmp * 10 + token[i] - '0';
        else if (after_dot_cnt <= 2) {
            tmp = tmp + (token[i] - '0') * std::pow(0.1, after_dot_cnt++);
        }
    }
    if (info_type == PRICE) price = tmp;
    if (info_type == TOTAL_COST) total_cost = tmp;
    return true;
}

bool instructionScanner::IsUerData(const std::string &token, InfoType info_type) {
    const int max_len = 30;
    if (token.size() > max_len) return false;
    for (int i = 0; i < token.size(); i++) {
        if (!isdigit(token[i]) && !isalpha(token[i]) && token[i] != '_')
            return false;
    }
    if (info_type == USER_ID) ID = ConstLenStr<31>(token);
    if (info_type == PASSWORD) password = ConstLenStr<31>(token);
    if (info_type == NEW_PASSWORD) new_password = ConstLenStr<31>(token);
    return true;
}

bool instructionScanner::IsASCII(const std::string &token, InfoType info_type) {
    int max_len;
    bool ban_quote = false;
    bool ban_line = false;
    if (info_type == USERNAME) max_len = 30;
    if (info_type == ISBN) max_len = 20;
    if (info_type == BOOK_NAME || info_type == AUTHOR_NAME || info_type == SINGLE_KEYWORD) {
        max_len = 60;
        ban_quote = true;
        if (info_type == SINGLE_KEYWORD) ban_line = true;
    }
    if (token.size() > max_len) return false;
    for (int i = 0; i < token.size(); i++) {
        if (!isprint(token[i]) || (ban_quote && token[i] == '\"') || (ban_line && token[i] == '|'))
            return false;
    }
    if (info_type == USERNAME) username = ConstLenStr<31>(token);
    if (info_type == ISBN) isbn = ConstLenStr<21>(token);
    if (info_type == BOOK_NAME) book_name = ConstLenStr<61>(token);
    if (info_type == AUTHOR_NAME) author_name = ConstLenStr<61>(token);
    if (info_type == SINGLE_KEYWORD) single_keyword = ConstLenStr<61>(token);
    return true;
}

bool instructionScanner::IsKeyword(const std::string &token) {
    const int max_len = 60;
    ConstLenStr<61> tmp = {};
    if (token.size() > max_len) return false;
    for (int i = 0; i < token.size(); i++) {
        if (!isprint(token[i]) || token[i] == '\"') return false;
        if (token[i] == '|') {
            if (tmp.GetSize() == 0) return false;
            if (i == token.size() - 1) return false;
            for (auto iter = keyword_list.begin(); iter != keyword_list.end(); iter++) {
                if (*iter == tmp) return false;
            }
            keyword_list.push_back(tmp);
            tmp.Clear();
        } else {
            tmp.Append(token[i]);
        }
    }
    keyword = ConstLenStr<61>(token);
    return true;
}

bool instructionScanner::ModifyScan(const std::string &token) {
    if (!modify_list[ISBN] && token.substr(0, 6) == "-ISBN" && token.size() > 6) {
        if (IsASCII(token.substr(6), ISBN)) {
            modify_list[ISBN] = true;
            return true;
        } else {
            return false;
        }
    } else if (!modify_list[BOOK_NAME] && token.back() == '\"' && token.substr(0, 7) == "-name\"" && token.size() > 8) {
        if (IsASCII(token.substr(7, token.size() - 1), BOOK_NAME)) {
            modify_list[BOOK_NAME] = true;
            return true;
        } else {
            return false;
        }
    } else if (!modify_list[AUTHOR_NAME] && token.back() == '\"' && token.substr(0, 9) == "-author=\"" &&
               token.size() > 10) {
        if (IsASCII(token.substr(9, token.size() - 1), AUTHOR_NAME)) {
            modify_list[AUTHOR_NAME] = true;
            return true;
        } else {
            return false;
        }
    }
    if (!modify_list[KEYWORD] && token.back() == '\"' && token.substr(0, 10) == "-keyword=\"" && token.size() > 11) {
        if (IsKeyword(token)) {
            modify_list[KEYWORD] = true;
            return true;
        } else {
            return false;
        }
    }
    if (!modify_list[PRICE] && token.substr(0, 7) == "-price=" && token.size() > 7) {
        if (IsDouble(token, PRICE)) {
            modify_list[PRICE] = true;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool instructionScanner::Scan() {
    int head = 0;
    for (; head < buffer.size(); head++) {
        if (buffer[head] != ' ') break;
    }
    if (head == buffer.size()) {
        instr_type = EMPTY;
        return true;
    }
    int tail = head;
    std::string token = {};
    for (; tail < buffer.size(); tail++) {
        if (buffer[tail] == ' ') {
            for (; tail < buffer.size(); tail++) {
                if (buffer[tail] != ' ') break;
            }
            if (token == "show" || token == "report") {
                token.push_back(' ');
                continue;
            }
            break;
        } else {
            token.push_back(buffer[tail]);
        }
    }
    if (token == "su") instr_type = SU;
    else if (token == "logout") instr_type = LOGOUT;
    else if (token == "register") instr_type = REGISTER;
    else if (token == "passwd") instr_type = PASSWD;
    else if (token == "useradd") instr_type = USERADD;
    else if (token == "delete") instr_type = DELETE;
    else if (token == "show") instr_type = SHOW;
    else if (token == "buy") instr_type = BUY;
    else if (token == "select") instr_type = SELECT;
    else if (token == "modify") instr_type = MODIFY;
    else if (token == "import") instr_type = IMPORT;
    else if (token == "show finance") instr_type = SHOW_FINANCE;
    else if (token == "log") instr_type = LOG;
    else if (token == "report finance") instr_type = REPORT_FINANCE;
    else if (token == "report employee") instr_type = REPORT_EMPLOYEE;
    else if (token == "quit") instr_type = QUIT;
    else if (token == "exit") instr_type = EXIT;
    else return false;
    int token_cnt = 0;
    while (token_cnt <= 5) {
        switch (token_cnt) {
            case 0:
                if (instr_type == QUIT || instr_type == EXIT
                    || instr_type == LOGOUT || instr_type == SHOW
                    || instr_type == SHOW_FINANCE || instr_type == LOG
                    || instr_type == REPORT_FINANCE || instr_type == REPORT_EMPLOYEE) {
                    if (tail != buffer.size()) {
                        if (instr_type != SHOW && instr_type != SHOW_FINANCE)
                            return false;
                    } else {

                        return true;
                    }
                } else if (tail == buffer.size()) return false;
                break;
            case 1:
                if (tail == buffer.size()) {
                    if (instr_type == SU) {
                        if (IsUerData(token, USER_ID)) {
                            factor_num = 1;
                            return true;
                        } else {
                            return false;
                        }
                    } else if (instr_type == DELETE) {
                        return IsUerData(token, USER_ID);
                    } else if (instr_type == SHOW) {
                        if (token.substr(0, 6) == "-ISBN" && token.size() > 6) {
                            if (IsASCII(token.substr(6), ISBN)) {
                                factor_num = 1;
                                show_what = ISBN;
                                return true;
                            } else {
                                return false;
                            }
                        } else if (token.back() == '\"' && token.substr(0, 7) == "-name\"" && token.size() > 8) {
                            if (IsASCII(token.substr(7, token.size() - 1), BOOK_NAME)) {
                                factor_num = 1;
                                show_what = BOOK_NAME;
                                return true;
                            } else {
                                return false;
                            }
                        } else if (token.back() == '\"' && token.substr(0, 9) == "-author=\"" && token.size() > 10) {
                            if (IsASCII(token.substr(9, token.size() - 1), AUTHOR_NAME)) {
                                factor_num = 1;
                                show_what = AUTHOR_NAME;
                                return true;
                            } else {
                                return false;
                            }
                        } else if (token.back() == '\"' && token.substr(0, 10) == "-keyword=\"" && token.size() > 11) {
                            if (IsASCII(token.substr(10, token.size() - 1), SINGLE_KEYWORD)) {
                                factor_num = 1;
                                show_what = SINGLE_KEYWORD;
                                return true;
                            } else {
                                return false;
                            }
                        } else {
                            return false;
                        }
                    } else if (instr_type == SELECT) {
                        return IsASCII(token, ISBN);
                    } else if (instr_type == SHOW_FINANCE) {
                        if (IsInteger(token, COUNT)) {
                            factor_num = 1;
                            return true;
                        } else {
                            return false;
                        }
                    } else if (instr_type == MODIFY) {
                        if (ModifyScan(token)) {
                            factor_num = 1;
                            return true;
                        } else {
                            return false;
                        }
                    } else {
                        return false;
                    }
                } else {
                    if (instr_type == SU || instr_type == REGISTER
                        || instr_type == PASSWD || instr_type == USERADD) {
                        if (!IsUerData(token, USER_ID)) return false;
                    } else if (instr_type == BUY) {
                        if (!IsASCII(token, ISBN)) return false;
                    } else if (instr_type == MODIFY) {
                        if (!ModifyScan(token)) return false;
                    } else if (instr_type == IMPORT) {
                        if (!IsInteger(token, QUANTITY)) return false;
                    } else return false;
                }
                break;
            case 2:
                if (tail == buffer.size()) {
                    if (instr_type == SU){
                        if (IsUerData(token, PASSWORD)) {
                            factor_num = 2;
                            return true;
                        } else {
                            return false;
                        }
                    } else if (instr_type == PASSWD) {
                        if (IsUerData(token, NEW_PASSWORD)) {
                            factor_num = 2;
                            return true;
                        } else {
                            return false;
                        }
                    } else if (instr_type == BUY) {
                        return IsInteger(token, QUANTITY);
                    } else if (instr_type == MODIFY) {
                        if (ModifyScan(token)) {
                            factor_num = 2;
                            return true;
                        } else {
                            return false;
                        }
                    } else if (instr_type == IMPORT) {
                        return IsDouble(token, TOTAL_COST);
                    } else return false;
                } else {
                    if (instr_type == REGISTER || instr_type == PASSWD || instr_type == USERADD) {
                        if (!IsUerData(token, PASSWORD)) return false;
                    } else if (instr_type == MODIFY) {
                        if (!ModifyScan(token)) return false;
                    } else return false;
                }
                break;
            case 3:
                if (tail == buffer.size()) {
                    if (instr_type == REGISTER) {
                        return IsASCII(token, USERNAME);
                    } else if (instr_type == PASSWD) {
                        if (IsASCII(token, NEW_PASSWORD)) {
                            factor_num = 3;
                            return true;
                        } else {
                            return false;
                        }
                    } else if (instr_type == MODIFY) {
                        if (ModifyScan(token)) {
                            factor_num = 3;
                            return true;
                        } else {
                            return false;
                        }
                    } else return false;
                } else {
                    if (instr_type == USERADD) {
                        if (!IsInteger(token, PRIVILEGE)) {
                            return false;
                        }
                    } else if (instr_type == MODIFY) {
                        if (!ModifyScan(token)) {
                            return false;
                        }
                    } else return false;
                }
                break;
            case 4:
                if (tail == buffer.size()) {
                    if (instr_type == USERADD) {
                        return IsASCII(token, USERNAME);
                    } else if (instr_type == MODIFY) {
                        if (ModifyScan(token)) {
                            factor_num = 4;
                            return true;
                        } else {
                            return false;
                        }
                    }
                } else {
                    if (instr_type == MODIFY) {
                        if (!ModifyScan(token)) {
                            return false;
                        }
                    } else return false;
                }
                break;
            case 5:
                if (tail == buffer.size()) {
                    if (instr_type == MODIFY) {
                        if (ModifyScan(token)) {
                            factor_num = 5;
                            return true;
                        } else {
                            return false;
                        }
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
                break;
        }
        token.clear();
        for (head = tail; tail < buffer.size(); tail++) {
            if (buffer[tail] == ' ') {
                for (; tail < buffer.size(); tail++) {
                    if (buffer[tail] != ' ') break;
                }
                break;
            } else {
                token.push_back(buffer[tail]);
            }
        }
        token_cnt++;
    }
}