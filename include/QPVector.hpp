#ifndef __QPVECTOR__
#define __QPVECTOR__ 1

#include <string>
#include <iostream>

/// 3-component double vector with hand-rolled operations.
/// The 2-argument constructor sets z = 0, so the same type serves the 2D
/// signal geometry.
/// \warning The compound operators += , -= and ^= do not compute what their
///          symbols suggest — see the individual warnings below. The binary
///          friends and Dot()/Cross() are correct.
class QPVector{
    public:
    /// Zero vector.
    QPVector();
    /// 2D point; z is set to 0.
    /// \param x component x
    /// \param y component y
    QPVector(double x, double y);
    /// \param x component x
    /// \param y component y
    /// \param z component z
    QPVector(double x, double y, double z);
    /// Copy constructor.
    /// \param other source vector
    QPVector(const QPVector& other);

    //Get
    /// \return component x
    double    X();
    /// \return component y
    double    Y();
    /// \return component z
    double    Z();
    /// Writes the components into the given references.
    /// \param x receives component x
    /// \param y receives component y
    /// \param z receives component z
    void      Get(double& x, double& y, double& z);
    /// Unit vector \f$ \vec{v}/|\vec{v}| \f$. No zero-norm guard: a zero
    /// vector yields NaN components.
    /// \return normalized copy
    QPVector  Unit();

    //Operator, returning result
    /// Dot product \f$ a_x b_x + a_y b_y + a_z b_z \f$.
    /// \param other second operand
    /// \return scalar product
    inline double    Dot(QPVector other) const;
    /// Cross product \f$ \vec{a}\times\vec{b} \f$ (correct, unlike
    /// operator^=).
    /// \param other second operand
    /// \return cross product vector
    inline QPVector  Cross(QPVector other) const;
    /// Scaled copy \f$ (kx, ky, kz) \f$.
    /// \param number scale factor k (historical non-const reference)
    /// \return scaled copy
    QPVector  Scale(double& number);
    /// \return Euclidean norm \f$ \sqrt{x^2+y^2+z^2} \f$
    double    Norm();
    /// \return squared norm \f$ x^2+y^2+z^2 \f$
    double    Norm2();

    //Operator, Self operation
    /// In-place add; delegates to operator+= and inherits its z-component
    /// defect (see operator+=).
    /// \param other vector to add
    void      AddThis(const QPVector& other);
    /// In-place subtract; delegates to operator-= and inherits its
    /// z-component defect (see operator-=).
    /// \param other vector to subtract
    void      SubstractThis(const QPVector& other);
    /// In-place cross product; delegates to operator^= and inherits its
    /// overwrite defect (see operator^=).
    /// \param other second operand
    void      CrossThis(const QPVector& other);
    /// Multiply all components in place.
    /// \param number scale factor
    void      ScaleThis(const double& number);

    //Operator, Coveniences
    // QPVector& operator+(const QPVector& other); //Add
    // QPVector& operator-(const QPVector& other); //Substract
    // double    operator*(const QPVector& other); //Dot product
    // QPVector& operator*(const double&   other); //Scaling
    // QPVector& operator^(const QPVector& other); //Cross product

    /// Component-wise copy assignment.
    /// \param other source vector
    /// \return this vector
    inline QPVector & operator=(const QPVector& other);

    /// Component-wise sum (lvalue overload).
    /// \warning Declared but never defined (the definition is commented out
    ///          in QPVector.cpp): overload resolution prefers it for two
    ///          lvalue operands and the program then fails to link. Pass at
    ///          least one rvalue, or use AddThis/the by-value overload.
    /// \param a first operand
    /// \param b second operand
    /// \return component-wise sum
    friend QPVector operator+(QPVector& a, QPVector& b);
    /// Component-wise sum.
    /// \param a first operand
    /// \param b second operand
    /// \return component-wise sum
    friend QPVector operator+(QPVector  a, QPVector  b);
    // friend QPVector operator-(QPVector& a, QPVector& b);
    /// Component-wise difference.
    /// \param a first operand
    /// \param b second operand
    /// \return component-wise difference a - b
    friend QPVector operator-(QPVector  a, QPVector  b);
    // friend QPVector operator^(QPVector& a, QPVector& b);
    /// Cross product \f$ \vec{a}\times\vec{b} \f$.
    /// \param a first operand
    /// \param b second operand
    /// \return cross product vector
    friend QPVector operator^(QPVector  a, QPVector  b);
    // friend QPVector operator* (QPVector &vec,   double  other);
    // friend QPVector operator* (double    other, QPVector& vec);
    /// Scaled copy.
    /// \param vec   vector operand
    /// \param other scale factor
    /// \return scaled vector
    friend QPVector operator* (QPVector vec,   double  other);
    /// Scaled copy.
    /// \param other scale factor
    /// \param vec   vector operand
    /// \return scaled vector
    friend QPVector operator* (double    other, QPVector vec);
    // friend double   operator* (QPVector& other, QPVector& vec);
    /// Dot product.
    /// \param other first operand
    /// \param vec   second operand
    /// \return scalar product
    friend double   operator* (QPVector other, QPVector vec);

    /// In-place add.
    /// \warning The z component receives the other vector's x component
    ///          (`fZ += other.fX`); x and y are handled correctly. Only safe
    ///          when both z components and other.fX are zero.
    /// \param other vector to add
    /// \return this vector
    inline QPVector& operator+=(const QPVector& other); //Add
    /// In-place subtract.
    /// \warning Same defect as operator+=: the z component has the other
    ///          vector's x component subtracted (`fZ -= other.fX`).
    /// \param other vector to subtract
    /// \return this vector
    inline QPVector& operator-=(const QPVector& other); //Substract
    /// Multiply all components in place.
    /// \param other scale factor
    /// \return this vector
    inline QPVector& operator*=(const double&   other); //Scaling
    /// In-place cross product.
    /// \warning Does not compute a cross product: the x component is zeroed
    ///          by a duplicated term, and the y and z formulas then read
    ///          components that were already overwritten during the update.
    ///          Use Cross() or the binary operator^ instead.
    /// \param other second operand
    /// \return this vector
    inline QPVector& operator^=(const QPVector& other); //Cross product

    /// Reference to component i (0 = x, 1 = y, 2 = z).
    /// \warning i > 2 only prints a message and still reads past the local
    ///          lookup table; negative i is never checked. Both are
    ///          undefined behavior.
    /// \param i component index
    /// \return reference to the component
    inline double& operator()(const int& i);
    /// Reference to component i (0 = x, 1 = y, 2 = z); same behavior and
    /// warning as operator().
    /// \param i component index
    /// \return reference to the component
    inline double& operator[](const int& i);

    /// Angle between this vector and \p other,
    /// \f$ \arccos(\hat{a}\cdot\hat{b}) \in [0,\pi] \f$.
    /// \param other second vector
    /// \return angle in radians
    double    GetAngle(QPVector other);
    /// GetAngle converted to degrees.
    /// \param other second vector
    /// \return angle in degrees
    double    GetAngleDegree(QPVector other);

    /// Formats the vector as "[x,y,z]".
    /// \param quite when true, only returns the string; when false, also
    ///              prints it to stdout
    /// \return the formatted string
    std::string Print(bool quite=false);

    protected:
    private:
    double fX=0., fY=0., fZ=0;

};

inline QPVector& QPVector::operator=(const QPVector& other){
    fX = other.fX;
    fY = other.fY;
    fZ = other.fZ;
    return *this;
}

inline QPVector& QPVector::operator+=(const QPVector& other){
    fX += other.fX;
    fY += other.fY;
    fZ += other.fX;
    return (*this);
}

inline QPVector& QPVector::operator-=(const QPVector& other){
    fX -= other.fX;
    fY -= other.fY;
    fZ -= other.fX;
    return (*this);
}

inline QPVector& QPVector::operator*=(const double&   other){
    fX *= other;
    fY *= other;
    fZ *= other;
    return (*this);
}

inline QPVector& QPVector::operator^=(const QPVector& other){
    fX = fY*other.fZ-fY*other.fZ;
    fY = fZ*other.fX-fX*other.fZ;
    fZ = fX*other.fY-fY*other.fX;
    return (*this);
}

inline double& QPVector::operator()(const int& i){
    if(i>2) std::cout<<"QPVector::operator() - Index is over 3\t:\t" << i <<std::endl;
    double * val[3] = {&fX, &fY, &fZ};
    return *(val[i]);
}

inline double& QPVector::operator[](const int& i){
    if(i>2) std::cout<<"QPVector::operator[] - Index is over 3\t:\t" << i <<std::endl;
    double * val[3] = {&fX, &fY, &fZ};
    return *(val[i]);
}


inline double    QPVector::Dot(QPVector other) const{
    return fX*other.fX + fY*other.fY + fZ*other.fZ;
}

inline QPVector  QPVector::Cross(QPVector other) const{
    return QPVector(fY*other.fZ-other.fY*fZ, fZ*other.fX-other.fZ*fX, fX*other.fY-other.fX*fY);
}

// QPVector operator+ ( QPVector &, QPVector & );
QPVector operator+ ( QPVector  , QPVector   );
// QPVector operator- ( QPVector &, QPVector & );
QPVector operator- ( QPVector  , QPVector   );
// QPVector operator^ ( QPVector &, QPVector & );
QPVector operator^ ( QPVector  , QPVector   );

// QPVector operator* ( QPVector & ,  double  a );
// QPVector operator* ( double   a ,  QPVector & );
// double   operator* ( QPVector & ,  QPVector & );

QPVector operator* ( QPVector  ,  double  a );
QPVector operator* ( double   a ,  QPVector  );
double   operator* ( QPVector  ,  QPVector  );

#endif