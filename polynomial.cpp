#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

using std::vector;

template<typename T>
class Polynomial {
    private:
        vector<T> coord;
    public:
        Polynomial(const std::vector<T> &vect) {
            coord = vect;
        }

        bool operator == (const Polynomial &other) const {
            return (coord == other.coord) ? true : false;
        }

        bool operator == (T other) {
            if (other == 0) {
                for (size_t i = 0; i < coord.size(); ++i) {
                    if (coord[i] != other) {
                        return false;
                    }
                }
                return true;
            } else {
                if ((coord.size() == 1) && coord[0] == other) {
                    return true;
                } else {
                    return false;
                }
            }
        }
    
        bool operator != (const Polynomial &other) const{
            if (*this == other) {
                return false;
            } else {
                return true;
            }
        }

        Polynomial operator + (const Polynomial &other) const {
            std::vector<T> v;
            Polynomial<T> res(v);
            size_t N = std::min(coord.size(), other.coord.size());
            for (size_t i = 0; i < N; ++i) {
                res.coord.push_back(coord[i] + other.coord[i]);
            }
            if (coord.size() > other.coord.size()) {
                for (size_t i = N; i < coord.size(); ++i) {
                   res.coord.push_back(coord[i]);
                }
            } else {
                for (size_t i = N; i < other.coord.size(); ++i) {
                    res.coord.push_back(other.coord[i]);
                }
            }
            size_t i = res.coord.size() - 1;
            while (res.coord[i] == 0 && i != 0) {
                res.coord.erase(res.coord.begin() + i);
                --i;
            }
            return res;
        }

        Polynomial operator - (const Polynomial &other) const {
            std::vector<T> v;
            Polynomial<T> res(v);
            size_t N = std::max(coord.size(), other.coord.size());
            for (size_t i = 0; i < N; ++i) {
                if (i >= coord.size() or i >= other.coord.size()) {
                    if (i >= coord.size()) {
                        res.coord.push_back(-other.coord[i]);
                    } else {
                        res.coord.push_back(coord[i]);
                    }
                } else {
                    res.coord.push_back(coord[i] - other.coord[i]);
                }
            }
            size_t i = res.coord.size() - 1;
            while (res.coord[i] == 0 && i != 0) {
                res.coord.erase(res.coord.begin() + i);
                --i;
            }
            return res;
        }

        Polynomial operator * (const Polynomial &other) const {
            std::vector<T> v;
            Polynomial<T> res(v);
            int N = coord.size() + other.coord.size() - 1;
            for (size_t i = 0; i < N; ++i) {
                T push = 0;
                for (size_t j = 0; j <= i; ++j) {
                    T x = 0;
                    if (j < coord.size()) {
                        x = coord[j];
                    }
                    T y = 0;
                    if ((i - j) < other.coord.size()) {
                        y = other.coord[i - j];
                    }
                    push += x * y;
                }

                res.coord.push_back(push);
            }
            return res;
        }
        
        Polynomial operator * (const T &other) const {
            std::vector<T> v;
            Polynomial<T> res(v);
            for (size_t i = 0; i < coord.size(); ++i) {
                res.coord.push_back(coord[i] * other);
            }
            return res;
        }

        Polynomial &operator += (const Polynomial &other) {
            *this = *this + other;
            return *this;
        }
        
        Polynomial &operator -= (const Polynomial &other) {
            *this = *this - other;
            return *this;
        }

        Polynomial &operator *= (const Polynomial &other) {
            *this = *this * other;
            return *this;
        }
        
        Polynomial &operator *= (const T &other) {
            *this = *this * other;
            return *this;
        }

        T operator [] (int i) const {
            if (i < coord.size()) {
                return coord[i];
            } else {
                return 0;
            }
        }

        int Degree () const {
            if (coord.size() != 0) {
                return coord.size() - 1;
            } else {
                return -1;
            }
        }

        T operator () (int i) const {
            T res = 0;
            for (size_t k = 1; k < coord.size(); ++k) {
                T ans = 1;
                for (size_t j = 1; j <= k; ++j) {
                    ans *= i;
                }
                ans *= coord[k];
                res += ans;
            }
            if (coord.size() > 0) {
                res += coord[0];
            }
            return res;
        }

        Polynomial operator / (const Polynomial &other) const {
            std::vector<T> v;
            Polynomial<T> res(v);
            for (size_t i = 0; i < (coord.size() - other.coord.size() + 1); ++i) {
                res.coord.push_back(0);
            }
            Polynomial<T> p = *this;
            while (p.Degree() >= other.Degree() && !(p == 0)) {
                int x = p.coord[p.coord.size()-1] / other.coord[other.coord.size()-1];
                res.coord[p.coord.size() - other.coord.size()] = x;
                for (size_t j = 0; j < other.coord.size(); ++j) {
                    p.coord[p.coord.size()-j-1] -= other.coord[other.coord.size()-1-j]*x;
                }
                size_t k = p.coord.size() - 1;
                while (p.coord[k] == 0 && k != 0) {
                    p.coord.erase(p.coord.begin() + k);
                    --k;
                }
            }
            return res;
        }

        Polynomial operator % (const Polynomial &other) const {
            std::vector<T> v;
            Polynomial<T> res(v);
            res = *this - (*this / other)*other;
            return res;
        }

        Polynomial operator , (const Polynomial &other) const {
            std::vector<T> v;
            Polynomial<T> res(v);
            Polynomial<T> p1 = *this, p2 = other;
            while (p1.coord.size() > 1 && p2.coord.size() > 1) {
                if (p1.coord.size() > p2.coord.size()) {
                    p1 = p1 % p2;
                } else {
                    p2 = p2 % p1;
                }
            }
            if (p1.coord.size() < p2.coord.size()) {
                if (p1.coord[p1.coord.size()-1] != 0) {
                    if (p1.coord[p1.coord.size()-1] != 1)
                        for (size_t i = 0; i < p1.coord.size(); ++i) {
                            p1.coord[i] = p1.coord[i] / p1.coord[p1.coord.size()-1];
                        }
                    return p1;
                } else {
                    if (p2.coord[p2.coord.size()-1] != 1)
                        for (size_t i = 0; i < p2.coord.size(); ++i) {
                            p2.coord[i] = p2.coord[i] / p2.coord[p2.coord.size()-1];
                        }
                    return p2;
                }
            } else {
                if (p2.coord[p2.coord.size()-1] != 0) {
                    if (p2.coord[p2.coord.size()-1] != 1)
                        for (size_t i = 0; i < p2.coord.size(); ++i) {
                            p2.coord[i] = p2.coord[i] / p2.coord[p2.coord.size()-1];
                        }
                    return p2;
                } else {
                    if (p1.coord[p1.coord.size()-1] != 1)
                        for (size_t i = 0; i < p1.coord.size(); ++i) {
                            p1.coord[i] = p1.coord[i] / p1.coord[p1.coord.size()-1];
                        }
                    return p1;
                }
            }
        }
};

template<typename T>
std::ostream& operator << (std::ostream& stream, const Polynomial<T> &p) {
    for (int i = p.Degree(); i > -1; --i) {
        stream << ((p[i] != 0) ? ((p[i] > 0) ? ((i != p.Degree()) ? "+" : "") : "") : "");
        if (p[i] != 0) {
            if (((p[i] != -1) && (p[i] != 1)) || (i == 0)) {
                stream << p[i];
                if (i != 0) {
                    stream << '*';
                }
            } else {
                if (p[i] < 0) {
                stream << "-";
                }
            }
        }
        if (i != 0 && p[i] != 0) {
            stream << "x";
        }
        if (i != 1 && i != 0 && p[i] != 0) {
            stream << '^';
            stream << i;
        }
        if (p.Degree() == 0 && p[0] == 0) {
        stream << "0";
        }
    }
    return stream;
}

int main() {
    
}