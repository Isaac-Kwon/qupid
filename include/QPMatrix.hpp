#ifndef __QPMATRIX__
#define __QPMATRIX__ 1

#include <string>
#include <vector>
#include "QPVector.hpp"

/// Dense m×n matrix of double with hand-rolled linear algebra.
/// Storage is a single row-major array; element (i, j) sits at index
/// i·n + j, which is what At(int,int) uses.
/// \warning operator()(int,int) addresses the storage with the wrong stride
///          (the row count m instead of the column count n); the two
///          accessors agree only for square matrices. See
///          operator()(int,int).
class QPMatrix{
    public:
    /// Null matrix: 0×0, no storage.
    QPMatrix();
    /// n×n zero matrix. (The historical comment called this a unit matrix;
    /// the implementation zero-fills and never sets the diagonal.)
    /// \param n number of rows and columns
    QPMatrix(const int n);
    // QPMatrix(const int m, const int n); //Arb matrix of n x m, zero matrix
    /// m×n zero matrix.
    /// \param m number of rows
    /// \param n number of columns
    QPMatrix(const int m, const int n);
    /// m×n matrix filled row-major from the variadic arguments.
    /// \param m       number of rows
    /// \param n       number of columns
    /// \param npassed number of variadic values that follow
    /// \warning The variadic values are read as double: pass double literals
    ///          (`0.` not `0`), or the values are garbage.
    QPMatrix(const int m, const int n, const int npassed, ...);
    /// Column vector from a QPVector.
    /// \warning Declared but never defined (no implementation in
    ///          QPMatrix.cpp): using it fails at link time. Use
    ///          QPMatrix(QPVector&, int) instead.
    /// \param vec source vector
    QPMatrix(QPVector& vec);
    /// Deep copy.
    /// \param other source matrix
    QPMatrix(const QPMatrix& other);
    /// ncomp×1 column vector from the first min(ncomp, 3) components of
    /// \p vec.
    /// \param vec   source vector
    /// \param ncomp number of rows
    QPMatrix(QPVector& vec, int ncomp);

    /// Deep copy assignment; reallocates the storage.
    /// \param other source matrix
    /// \return this matrix
    QPMatrix& operator=(const QPMatrix&);

    /// Frees the element storage.
    ~QPMatrix();
    /// \return transposed copy
    QPMatrix Transpose();
    /// Determinant by recursive Laplace expansion along the first row.
    /// Square matrices only: otherwise prints a message and returns 0.
    /// A 0×0 matrix also returns 0.
    /// \return determinant
    double   Det();
    /// Sum of the diagonal elements up to min(m, n).
    /// Reads through operator()(int,int), so non-square matrices are
    /// affected by its stride defect.
    /// \return trace
    double   Trace();
    /// Minor matrix: this matrix with row \p i and column \p j removed.
    /// Writes through operator()(int,int) while reading through At, so the
    /// result is correct only for square input.
    /// \param i row to remove
    /// \param j column to remove
    /// \return (m-1)×(n-1) matrix
    QPMatrix SubMatrix(const int i, const int j);
    /// Cofactor \f$ C_{ij} = (-1)^{i+j} \det M_{ij} \f$ where \f$ M_{ij} \f$
    /// is the minor. Square matrices only: otherwise prints and returns 0.
    /// \param i row index
    /// \param j column index
    /// \return cofactor
    double   Cofactor(const int i, const int j);
    /// Inverse by the adjugate formula
    /// \f$ A^{-1} = \mathrm{adj}(A)/\det A \f$.
    /// Returns a null matrix (and prints a message) when the matrix is not
    /// square or \f$ |\det A| < 10^{-10} \f$.
    /// \return inverse, or a null matrix on failure
    QPMatrix Inverse();
    /// Value of linear element \p i of the row-major storage.
    /// Out of range prints a message and returns -1.
    /// \param i linear index in [0, m·n)
    /// \return element value, or -1 when out of range
    double   At(int i);
    /// Value at row \p i, column \p j: storage index i·n + j (row-major).
    /// Out of range prints a message and returns -1.
    /// \param i row index
    /// \param j column index
    /// \return element value, or -1 when out of range
    double   At(int i, int j);
    /// Reference to linear element \p i of the row-major storage.
    /// Out of range prints a message and returns a reference to a shared
    /// internal sentinel; writes through it are lost.
    /// \param i linear index in [0, m·n)
    /// \return reference to the element (or the sentinel)
    double&  operator[](int i);
    /// Same as operator[].
    /// \param i linear index in [0, m·n)
    /// \return reference to the element (or the sentinel)
    double&  operator()(int i);
    /// Reference to the element addressed as i·m + j.
    /// \warning The stride is the row count m, not the column count n: for
    ///          non-square matrices this addresses a different element than
    ///          At(int,int) (which uses i·n + j). The two agree only for
    ///          square matrices. Out of range behaves like operator[].
    /// \param i row index
    /// \param j column index
    /// \return reference to the addressed element (or the sentinel)
    double&  operator()(int i, int j);
    /// \param i linear index
    /// \return true when 0 <= i < m·n
    bool     IsInside(int i);
    /// \param i row index
    /// \param j column index
    /// \return true when 0 <= i < m and 0 <= j < n
    bool     IsInside(int i, int j);
    /// \param other matrix to compare
    /// \return true when both dimensions match
    bool     IsSameShape(QPMatrix & other);
    /// \param other matrix to compare
    /// \return true when this is m×n and \p other is n×m
    bool     IsTransShape(QPMatrix & other);
    /// Formats the matrix as tab-separated rows.
    /// \param quite when true, only returns the string; when false, also
    ///              prints it to stdout
    /// \return the formatted string
    std::string Print(bool quite=false);

    /// Scaled copy.
    /// \return matrix with every element multiplied by the scalar
    friend QPMatrix operator*(QPMatrix , double  );
    /// Scaled copy.
    /// \return matrix with every element multiplied by the scalar
    friend QPMatrix operator*(double  , QPMatrix );
    /// Matrix product. Inner dimensions are checked; on mismatch prints a
    /// message and returns a null matrix.
    /// \return matrix product
    friend QPMatrix operator*(QPMatrix   , QPMatrix   );
    /// Element-wise sum. Shapes are checked; on mismatch prints a message
    /// and returns a null matrix.
    /// \return element-wise sum
    friend QPMatrix operator+(QPMatrix &  , QPMatrix  & );
    /// Element-wise difference. Shapes are checked; on mismatch prints a
    /// message and returns a null matrix.
    /// \return element-wise difference
    friend QPMatrix operator-(QPMatrix &  , QPMatrix  & );

    /// Multiply every element in place.
    /// \param other scale factor
    /// \return this matrix
    inline QPMatrix& operator*=(const double& other); //Scaling


    protected:
    /// Releases the element storage. Does not reset the pointer; only the
    /// destructor and operator= (which reallocates) may call it.
    void     Freeing();

    private:
    void Init();
    double * fElement;
    int fM=1;
    int fN=1;

    double nval=-1;
};

QPMatrix operator*(QPMatrix , double   );
QPMatrix operator*(double   , QPMatrix );
QPMatrix operator*(QPMatrix , QPMatrix );
QPMatrix operator+(QPMatrix & , QPMatrix & );
QPMatrix operator-(QPMatrix & , QPMatrix & );

inline QPMatrix& QPMatrix::operator*=(const double&   other){
    for(int i=0; i<fM*fN; i++){
        fElement[i] *= other;
    }
    return (*this);
}

#endif