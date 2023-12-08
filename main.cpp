#include <string_view>
#include <algorithm>
#include <execution>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
 
using namespace std;

class Domain {
public:
    //Конструктор класса domain
    Domain(const string& domainName)
        : domain_(domainName) {
    }
    //Проверяет, является ли текущий домен поддоменом переданного домена
    bool IsSubdomain(const Domain& otherDomain) const {
        if (otherDomain.domain_.size() <= domain_.size()) {
            return equal(otherDomain.domain_.begin(), otherDomain.domain_.end(), domain_.begin());
        }
        else {
            return false;
        }
    }
    //Перегрузка оператора сравнения для класса domain
    bool operator==(const Domain& otherDomain) const {
        return (domain_ == otherDomain.domain_);
    }
    //Возвращает доменное имя в виде строки
    string GetValue() const {
        return domain_;
    }
private:
    string domain_;  // доменное имя
};

class DomainChecker {
    public:
        // Конструктор с параметрами - принимает начальный и конечный итераторы для заполнения вектора forbidden_
        template <typename It>
        DomainChecker(It begin, It end)
            : forbidden_(begin, end) {
            SortUnique(); // Сортировка и удаление дубликатов
        }
        // Метод для проверки, является ли доменное имя запрещенным
        bool IsForbidden(const Domain& domain) {
            // Ищем позицию для вставки доменного имени в упорядоченный список
            auto it = upper_bound(forbidden_.begin(), forbidden_.end(), domain, [](const Domain& value, const Domain& rhs) {
                return value.GetValue() < rhs.GetValue(); 
            });
            // Если позиция найдена, проверяем, является ли переданное доменное имя поддоменом найденного запрещенного доменного имени              
            if (it == forbidden_.begin()) {
                return false;
            } else return domain.IsSubdomain(*(prev(it)));
         
            return false;
        }

    private:
        // Метод для сортировки и удаления дубликатов в векторе запрещенных доменных имен
        void SortUnique() {
            sort(forbidden_.begin(), forbidden_.end(), [](const Domain& lhs, const Domain& rhs) {
                return lhs.GetValue() < rhs.GetValue(); 
            });
            // Удаление дубликатов доменных имен
            auto last = unique(forbidden_.begin(), forbidden_.end(), [](const Domain& lhs, const Domain& rhs) {
                return rhs.IsSubdomain(lhs);
            });
            forbidden_.erase(last, forbidden_.end());
        }

    private:
        vector<Domain> forbidden_; // Вектор запрещенных доменных имен
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
template <typename Number>
vector<Domain> ReadDomains(istream& input, Number number) {
    vector<Domain> result;
    for (Number i = 0; i < number; ++i) {
        string line;
        getline(input, line);
        line.insert(0, 1, '.');
        reverse(line.begin(), line.end());
        result.emplace_back(move(line));
    }
    return result;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
