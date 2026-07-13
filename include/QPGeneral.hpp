
/// Project-wide constants of the qupid core library.
namespace qupid{
    /// Grid divisions per axis intended for numeric signal integration.
    /// QPSignalBase::Integral currently uses a hard-coded 5 instead of this
    /// value (the reference is commented out in the implementation).
    const int NIntegrateDivide = 100;

};