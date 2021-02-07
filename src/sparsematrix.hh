// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
/*
 * File:   sparsematrix.hh
 * Author: Christian Heusel <christian@heusel.eu>
 *
 * Created on August 25, 2020
 */

#ifndef SPARSEMATRIX_HH
#define SPARSEMATRIX_HH

#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

#include "densematrix.hh"
#include "vector.hh"

namespace hdnum {

/*! \brief Sparse matrix Class with mathematical matrix operations
 */
template <typename REAL>
class SparseMatrix {
public:
    /** \brief Types used for array indices */
    using size_type = std::size_t;
    using VType = typename std::vector<REAL>;
    using VectorIterator = typename VType::iterator;
    using ConstVectorIterator = typename VType::const_iterator;

private:
    // Matrix data is stored in an STL vector!
    VType _data;

    // The non-null indices are stored in STL vectors with the size_type!
    // Explanation on how the mapping works can be found here:
    // https://de.wikipedia.org/wiki/Compressed_Row_Storage
    std::vector<size_type> _columnIndices;
    std::vector<size_type> _rowPtr;

    size_type m_rows;  // Number of Matrix rows
    size_type m_cols;  // Number of Matrix columns

    static bool bScientific;
    static size_type nIndexWidth;
    static size_type nValueWidth;
    static size_type nValuePrecision;

    //! get matrix element for write access:
    REAL at(const size_type row, const size_type col) {}

    //! get matrix element for read-only access:
    const REAL at(const size_type row, const size_type col) const {
        return _data[row * m_cols + col];
    }

public:
    //! default constructor (empty Matrix)
    SparseMatrix() noexcept
        : _data(), _columnIndices(), _rowPtr(), m_rows(0), m_cols(0) {}

    //! constructor
    SparseMatrix(const size_type _rows, const size_type _cols)
        : _data(), _columnIndices(), _rowPtr(_rows + 1), m_rows(_rows),
          m_cols(_cols) {}

    //! constructor from initializer list
    SparseMatrix(const std::initializer_list<std::initializer_list<REAL>> &v) {}

    size_type rowsize() const { return m_rows; }
    size_type colsize() const { return m_cols; }

    // pretty-print output properties
    bool scientific() const { return bScientific; }

    /* class iterator { */
    /* public: */
    /*     using self_type = iterator; */

    /*     // conform to the iterator traits */
    /*     // https://en.cppreference.com/w/cpp/iterator/iterator_traits */
    /*     using difference_type = std::ptrdiff_t; */
    /*     using value_type = REAL; */
    /*     using pointer = REAL *; */
    /*     using reference = REAL &; */
    /*     using iterator_category = std::forward_iterator_tag; */

    /*     iterator(pointer ptr) : _ptr {ptr} {} */

    /*     // prefix */
    /*     self_type operator++() { */
    /*         _ptr++; */
    /*         return *this; */
    /*     } */

    /*     // postfix */
    /*     self_type operator++(int junk) { */
    /*         self_type cached = *this; */
    /*         _ptr++; */
    /*         return cached; */
    /*     } */

    /*     reference operator*() { return *_ptr; } */
    /*     pointer operator->() { return _ptr; } */
    /*     bool operator==(const self_type &rhs) { return _ptr == rhs._ptr; } */
    /*     bool operator!=(const self_type &rhs) { return _ptr != rhs._ptr; } */

    /* private: */
    /*     pointer _ptr; */
    /* }; */

    /* class const_iterator { */
    /* public: */
    /*     using self_type = const_iterator; */

    /*     // conform to the iterator traits */
    /*     // https://en.cppreference.com/w/cpp/iterator/iterator_traits */
    /*     using difference_type = std::ptrdiff_t; */
    /*     using value_type = REAL; */
    /*     using pointer = REAL *; */
    /*     using reference = REAL &; */
    /*     using iterator_category = std::forward_iterator_tag; */

    /*     const_iterator(pointer ptr) : _ptr {ptr} {} */

    /*     // prefix */
    /*     self_type operator++() { */
    /*         _ptr++; */
    /*         return *this; */
    /*     } */

    /*     // postfix */
    /*     self_type operator++(int junk) { */
    /*         self_type cached = *this; */
    /*         _ptr++; */
    /*         return cached; */
    /*     } */

    /*     const value_type &operator*() { return *_ptr; } */
    /*     const pointer operator->() { return _ptr; } */
    /*     bool operator==(const self_type &rhs) { return _ptr == rhs._ptr; } */
    /*     bool operator!=(const self_type &rhs) { return _ptr != rhs._ptr; } */

    /* private: */
    /*     pointer _ptr; */
    /* }; */

    // regular (possibly modifying) Iterators
    /* iterator begin() {} */
    /* iterator end() {} */

    // const Iterators
    ConstVectorIterator cbegin() const {}
    ConstVectorIterator cend() const {}
    ConstVectorIterator begin() const { return this->cbegin(); }
    ConstVectorIterator end() const { return this->cend(); }

    /*!
      \brief   Switch between floating point (default=true) and fixed point
      (false) display

      \b Example:
      \code
      hdnum::DenseMatrix<double> A(4,4);
      A.scientific(false); // fixed point representation for all DenseMatrix
      objects A.width(8); A.precision(3); identity(A);  // Defines the identity
      matrix of the same dimension std::cout << "A=" << A << std::endl; \endcode

      \b Output:
      \verbatim
      A=
      0        1        2        3
      0     1.000    0.000    0.000    0.000
      1     0.000    1.000    0.000    0.000
      2     0.000    0.000    1.000    0.000
      3     0.000    0.000    0.000    1.000
      \endverbatim
    */
    void scientific(bool b) const { bScientific = b; }

    //! get index field width for pretty-printing
    size_type iwidth() const { return nIndexWidth; }

    //! get data field width for pretty-printing
    size_type width() const { return nValueWidth; }

    //! get data precision for pretty-printing
    size_type precision() const { return nValuePrecision; }

    //! set index field width for pretty-printing
    void iwidth(size_type i) const { nIndexWidth = i; }

    //! set data field width for pretty-printing
    void width(size_type i) const { nValueWidth = i; }

    //! set data precision for pretty-printing
    void precision(size_type i) const { nValuePrecision = i; }

    // write access on matrix element A_ij using A(i,j)
    REAL &operator()(const size_type row, const size_type col) {
        _data.push_back(REAL {});
        _columnIndices.push_back(col);
        if (row < _rowPtr[col]) {
            _rowPtr[col] = row;
        }
        return _data[_data.size() - 1];
    }

    //! read-access on matrix element A_ij using A(i,j)
    const REAL &operator()(const size_type row, const size_type col) const {
        if (m_cols - 1 <= col) {
            HDNUM_ERROR("Out of bounds access: column too big!");
        } else if (m_rows - 1 <= row) {
            HDNUM_ERROR("Out of bounds access: row too big!");
        }

        // Handle the zero matrix case
        if (_rowPtr[col] == 0 and _rowPtr[col + 1] == 0 or
            _rowPtr.size() == 1) {
            return REAL {};
        }

        // look for the entry
        for (auto i = _rowPtr[col]; i < _rowPtr[col + 1]; ++i) {
            if (_columnIndices[i] == col) {
                return _data[i];
            }
        }
        // look for the entry
        return REAL {};
    }

    //! read-access on matrix element A_ij using A[i][j]
    const ConstVectorIterator operator[](const size_type row) const {}

    //! write-access on matrix element A_ij using A[i][j]
    VectorIterator operator[](const size_type row) {}

    SparseMatrix operator=(const SparseMatrix &A) {}
    SparseMatrix operator=(const REAL value) {}

    bool operator==(const SparseMatrix &other) const {}
    bool operator!=(const SparseMatrix &other) const {}
    bool operator==(const hdnum::DenseMatrix<REAL> &other) const {}
    bool operator!=(const hdnum::DenseMatrix<REAL> &other) const {}

    // delete all the invalid comparisons
    bool operator<(const SparseMatrix &other) = delete;
    bool operator>(const SparseMatrix &other) = delete;
    bool operator<=(const SparseMatrix &other) = delete;
    bool operator>=(const SparseMatrix &other) = delete;

    SparseMatrix transpose() const {
        SparseMatrix A(m_cols, m_rows);
        for (size_type i = 0; i < m_rows; i++)
            for (size_type j = 0; j < m_cols; j++)
                A[j][i] = this->operator()(i, j);
        return A;
    }

    // Basic Matrix Operations
    [[nodiscard]] SparseMatrix operator+=(const SparseMatrix &B) {}
    [[nodiscard]] SparseMatrix operator-=(const SparseMatrix &B) {}
    [[nodiscard]] SparseMatrix operator*=(const REAL s) {}
    [[nodiscard]] SparseMatrix operator/=(const REAL s) {}

    void update(const REAL s, const SparseMatrix &B) {}

    template <class V>
    void mv(Vector<V> &y, const Vector<V> &x) const {}

    template <class V>
    void umv(Vector<V> &y, const Vector<V> &x) const {}

    template <class V>
    void umv(Vector<V> &y, const V &s, const Vector<V> &x) const {}

    void mm(const SparseMatrix<REAL> &A, const SparseMatrix<REAL> &B) {}

    [[nodiscard]] Vector<REAL> operator*(const Vector<REAL> &x) const {}

    [[nodiscard]] SparseMatrix operator+(const SparseMatrix &x) const {}
    [[nodiscard]] SparseMatrix operator-(const SparseMatrix &x) const {}
    [[nodiscard]] SparseMatrix operator*(const SparseMatrix &x) const {}
    [[nodiscard]] SparseMatrix operator/(const SparseMatrix &x) const {}

    //! compute row sum norm
    REAL norm_infty() const {
        REAL norm(0.0);
        for (size_type i = 0; i < rowsize(); i++) {
            REAL sum(0.0);
            for (size_type j = 0; j < colsize(); j++)
                sum += myabs((*this)(i, j));
            if (sum > norm) norm = sum;
        }
        return norm;
    }

    //! compute column sum norm
    REAL norm_1() const {
        REAL norm(0.0);
        for (size_type j = 0; j < colsize(); j++) {
            REAL sum(0.0);
            for (size_type i = 0; i < rowsize(); i++)
                sum += myabs((*this)(i, j));
            if (sum > norm) norm = sum;
        }
        return norm;
    }

    std::string to_string() noexcept {
        using std::to_string;

        // lambda that converts container contents into
        // { 1, 2, 3, 4 }
        auto comma_fold = [](auto container) {
            return "{ " +
                   std::accumulate(
                       std::next(container.begin()), container.end(),
                       std::to_string(
                           container[0]),  // start with first element
                       [](std::string a, REAL b) {
                           return a + ", " + std::to_string(b);
                       }) +
                   " }";
        };

        return "values = " + comma_fold(_data) +
               "\ncolInd = " + comma_fold(_columnIndices) +
               "\nrowPtr = " + comma_fold(_rowPtr) + "\n";
    }

    SparseMatrix<REAL> matchingIdentity() const {}
    static SparseMatrix identity(const size_type dimN) {}

    class builder {
        size_type m_rows {};  // Number of Matrix rows, 0 by default
        size_type m_cols {};  // Number of Matrix columns, 0 by default
        std::vector<std::map<size_type, REAL>> _rows;

    public:
        builder(size_type new_m_rows, size_type new_m_cols)
            : m_rows {new_m_rows}, m_cols {new_m_cols}, _rows {m_rows} {}

        builder() = default;

        std::pair<typename std::map<size_type, REAL>::iterator, bool> addEntry(
            size_type i, size_type j, REAL value) {
            return _rows.at(i).emplace(j, value);
        }

        std::pair<typename std::map<size_type, REAL>::iterator, bool> addEntry(
            size_type i, size_type j) {
            return addEntry(i, j, REAL {});
        };

        size_type colsize() noexcept { return m_cols; }
        size_type rowsize() noexcept { return m_rows; }

        size_type setNumCols(size_type new_m_cols) noexcept {
            m_cols = new_m_cols;
            return m_cols;
        }
        size_type setNumRows(size_type new_m_rows) {
            m_rows = new_m_rows;
            _rows.resize(m_cols);
            return m_rows;
        }

        void clear() noexcept {
            for (auto &row : _rows) {
                row.clear();
            }
        }

        std::string to_string() {
            std::string output;
            for (std::size_t i = 0; i < _rows.size(); i++) {
                for (const auto &[index, value] : _rows[i]) {
                    output += "i=" + std::to_string(i) +
                              ", j=" + std::to_string(index) + " => " +
                              std::to_string(value) + "\n";
                }
            }
            return output;
        }

        SparseMatrix build() {
            auto result = SparseMatrix<REAL>(m_rows, m_cols);

            for (std::size_t i = 0; i < _rows.size(); i++) {
                result._rowPtr[i + 1] = result._rowPtr[i];
                for (const auto &[index, value] : _rows[i]) {
                    result._columnIndices.push_back(index);
                    result._data.push_back(value);
                    result._rowPtr[i + 1]++;
                }
            }
            return result;
        }
    };
};

template <typename REAL>
bool SparseMatrix<REAL>::bScientific = true;
template <typename REAL>
std::size_t SparseMatrix<REAL>::nIndexWidth = 10;
template <typename REAL>
std::size_t SparseMatrix<REAL>::nValueWidth = 10;
template <typename REAL>
std::size_t SparseMatrix<REAL>::nValuePrecision = 3;

template <typename REAL>
std::ostream &operator<<(std::ostream &out, const SparseMatrix<REAL> &A) {
    return out;
}

//! make a zero matrix
template <typename REAL>
inline void zero(SparseMatrix<REAL> &A) {}

/*!
  \relates SparseMatrix
  \n
  \b Function: make identity matrix
  \code
  template<class T>
  inline void identity (SparseMatrix<T> &A)
  \endcode
  \param[in] A reference to a SparseMatrix that shall be filled with entries

  \b Example:
  \code
  hdnum::SparseMatrix<double> A(4,4);
  identity(A);
  // fixed point representation for all DenseMatrix objects
  A.scientific(false);
  A.width(10);
  A.precision(5);

  std::cout << "A=" << A << std::endl;
  \endcode

  \b Output:
  \verbatim
  A=
  0          1          2          3
  0     1.00000    0.00000    0.00000    0.00000
  1     0.00000    1.00000    0.00000    0.00000
  2     0.00000    0.00000    1.00000    0.00000
  3     0.00000    0.00000    0.00000    1.00000
  \endverbatim

*/
template <class T>
inline void identity(SparseMatrix<T> &A) {}

}  // namespace hdnum

#endif  // SPARSEMATRIX_HH
