//------------------------------------------------------------------------------
// Copyright (C) Gehriger Engineering, Inc.
//
// This material is the intellectual property of Gehriger Engineering, Inc. and
// may not be redistributed, modified, copied in any way, by any means without
// written permission of Gehriger Engineering.
//------------------------------------------------------------------------------
#pragma once
#include "geom.h"
#include "Point2d.h"
#include "Vector2d.h"
#include "Bounds.h"
#include "Angle.h"
#include <lc/util/lcmath.h>
#include <limits>
#include <cassert>

namespace lc::geom {

//! Geometric transformation class
//!
//!      A transformation contains the following information:
//!
//!       - scaling factor
//!       - rotation angle
//!       - insertion point (offset)
//!       - absolute rotation or scaling
//!
//!      The transformations are applied in the following order:
//!
//!       1. scale / flip
//!       2. rotate
//!       3. offset to insertion point
//!
//!      Absolute scaling means that if outer references further
//!      scale this reference, the outer scaling factors will be ignored.
//!
//!      Absolute rotation means that if outer references further
//!      rotate this reference, the outer rotation angles will be ignored.
//!
//! \param T coordinate integer type
template <typename T>
class Xform
{
public:
    using dist = T;
    using Vector = Vector2dT<T>;
    using Point = Point2dT<T>;

    static const Xform identity;
    static const Xform identityRel;

    // default constructor

    //! Constructor
    //!
    //! \param   ignoreAbsXform  (optional) Ignore absolute transformations
    explicit Xform(bool ignoreAbsXform = false);

    //! Constructor
    //!
    //! \note    Transformations are applied in parameter order
    //!
    //! \param   scale       Scaling factor
    //! \param   rotationCcw Rotation angle, counter-clockwise
    //! \param   offset      Translation
    Xform(double scale, Angle rotationCcw, const Vector& offset);

    //! Constructor
    //!
    //! \param   offset      Translation
    //! \param   ignoreAbsXform  (optional) Ignore absolute transformations
    explicit Xform(const Vector& offset, bool ignoreAbsXform = false);

    //! Constructor
    //!
    //! \param   offset      Translation
    //! \param   ignoreAbsXform  (optional) Ignore absolute transformations
    explicit Xform(const Point& offset, bool ignoreAbsXform = false);

    //! Tests if this is the identity transformation
    //!
    //! \return  true if identity transformation, false if not.
    [[nodiscard]] bool isIdentity() const;

    //! Overrides any absolute transformations (scaling factors and rotations)
    //! and makes them relative
    //!
    //! \param   doIgnore    (optional) Ignore absolute transformations
    //!
    //! \return  Reference to this transformation after applying transformation
    Xform& setIgnoreAbsTransformation(bool doIgnore = true);

    //! Test if absolute transformations are ignored
    //!
    //! \return  true if absolute transformations are ignored, false otherwise.
    [[nodiscard]] bool isAbsTransformationIgnored() const;

    //! Tests if this transformation doesn't have absolute transformation or
    //! scaling, or if absolute transformations are ignored.
    //!
    //! \return  true if this transformation is relative, false if not.
    [[nodiscard]] bool isRelative() const;

    //! Sets scaling factor
    //!
    //! \warning This only sets the scaling factor component of the transformation;
    //!          if you need to scale the entire transformation, use scale() instead!
    //!
    //! \param   scale               Scaling factor
    //! \param   isAbsoluteScaling   (optional) Specify if scaling is absolute
    //!
    //! \return  Reference to this transformation after applying transformation
    Xform& setScaling(double scale, bool isAbsoluteScaling = false);

    //! Gets the scaling factor
    //!
    //! \return  Scaling factor
    [[nodiscard]] double scaling() const;

    //! Tests if this transformation has a scaling component
    //!
    //! \return  true if scaled, false if not.
    [[nodiscard]] bool isScaled() const;

    //! Apply a scaling to the existing transformation
    //!
    //! \warning This scales the existing transformation, including the
    //!          translation component! If you want to set the scaling component
    //!          only, use setScaling() instead.
    //!
    //! \param   scale Scaling factor
    //! \param   isAbsoluteScaling (optional) if true, scaling is absolute
    //!
    //! \return  Reference to this transformation after applying transformation
    Xform& scale(double scale, bool isAbsoluteScaling = false);

    //! Specifies absolute scaling
    //!
    //! Absolute scaling means that if outer references further
    //! scaling this reference, the outer scaling factors will be ignored.
    //!
    //! \param   absolute    (optional) Scaling is absolute if true
    //!
    //! \return  Reference to this transformation after applying transformation
    Xform& setScalingAbsolute(bool absolute = true);

    //! Tests if this transformation has absolute scaling, or if absolute
    //! transformations are ignored
    //!
    //! \return  true if scaling factor is absolute; false otherwise
    [[nodiscard]] bool isScalingAbsolute() const;

    //! Specifies if to mirror about y-axis (negate x-coordinates)
    //!
    //! \warning This sets the mirroring component of the transformation; to apply
    //!          mirroring to the existing transformatino, use mirrorInX() instead!
    //!
    //! \param   mirror  (optional) Mirror about y-axis (negate x-coordinates) if true
    //!
    //! \return  Reference to this transformation after applying transformation
    Xform& setMirroredInX(bool mirror = true);

    //! Tests if mirrored about y-axis (x-coordinates are negated)
    //!
    //! \return true if mirrored; false otherwise
    [[nodiscard]] bool isMirroredInX() const;

    //! Apply mirroring about y-axis (negate x-coordinates) to the existing
    //! transformation
    //!
    //! \warning This applies mirroring to the existing transformation; to
    //!          set the mirroring component use setMirroredInX() instead!
    //!
    //! \param doMirror  If true, mirror about y-axis (negate x-coordinates)
    Xform& mirrorInX(bool doMirror = true);

    //! Specifies if to mirror about x-axis (negate y-coordinates)
    //!
    //! \warning This sets the mirroring component of the transformation; to apply
    //!          mirroring to the existing transformatino, use mirrorInY() instead!
    //!
    //! \param   mirror  (optional) Mirror about x-axis (negate y-coordinates) if true
    //!
    //! \return  Reference to this transformation after applying transformation
    Xform& setMirroredInY(bool mirror = true);

    //! Tests if mirrored about x-axis (y-coordinates are negated)
    //!
    //! \return true if mirrored; false otherwise
    [[nodiscard]] bool isMirroredInY() const;

    //! Apply mirroring about x-axis (negate y-coordinates) to the existing
    //! transformation
    //!
    //! \warning This applies mirroring to the existing transformation; to
    //!          set the mirroring component use setMirroredInY() instead!
    //!
    //! \param doMirror  If true, mirror about x-axis (negate y-coordinates)
    Xform& mirrorInY(bool doMirror = true);

    //! Sets rotation component
    //!
    //! \warning This only sets the rotation component of the transformation;
    //!          if you need to rotate the entire transformation, use rotate()
    //!          instead!
    //!
    //! \param   rotationCcw         Rotation i(counter-clockwise from positive x-axis)
    //! \param   isAbsoluteRotation  (optional) Specify if rotation is absolute
    //!
    //! \return  Reference to this transformation after applying transformation
    Xform& setRotation(Angle rotationCcw, bool isAbsoluteRotation = false);

    //! Gets the rotation angle
    //!
    //! \return  Rotation angle (counter-clockwise from positive x-axis)
    [[nodiscard]] Angle rotation() const;

    //! Tests if this transformation has a rotation component
    //!
    //! \return  true if rotated, false if not.
    [[nodiscard]] bool isRotated() const;

    //! Tests if this transformation has absolute rotation, or if absolute
    //! transformations are ignored
    //!
    //! \return  true if rotation is absolute; false otherwise
    [[nodiscard]] bool isRotationAbsolute() const;

    //! Rotate the existing transformation
    //!
    //! \warning This rotates the existing transformation; to set the rotation
    //!          angle use setRotation() instead!
    //!
    //! \param   rotationCcw          Rotation (counter-clockwise from positive x-axis)
    //! \param   isAbsoluteRotation   (optional) Specify if rotation is absolute
    //!
    //! \return  Reference to this transformation after applying transformation
    Xform& rotate(Angle rotationCcw, bool isAbsoluteRotation = false);

    //! Specifies absolute rotation
    //!
    //! Absolute rotation means that if outer references further
    //! rotate this reference, the outer rotation angles will be ignored.
    //!
    //! \param   absolute    (optional) Rotation is absolute if true
    //!
    //! \return  Reference to this transformation after applying transformation
    Xform& setRotationAbsolute(bool absolute = true);

    //! Sets translation vector
    //!
    //! \warning This sets the translation vector component of the transformation;
    //!          to translate the existing transformation instead, use translate()!
    //!
    //! \param   offset  Translation
    //!
    //! \return  Reference to this transformation after applying transformation
    Xform& setTranslation(const Vector& offset);

    //! Gets the translation vector
    //!
    //! \return  Translation vector
    [[nodiscard]] Vector translation() const;

    //! Apply a translation to the existing transformation
    //!
    //! \warning This translates the existing transformation; to set the
    //!          translation vector component use setTranslation()!
    //!
    //! \param   offset  Translation
    //!
    //! \return  Reference to this transformation after applying transformation
    Xform& translate(const Vector& offset);

    // Return rotation component in degrees

    //! Gets the inverse transformation
    //!
    //! \return  Inverse transformation, which, when applied to this
    //!          transformation, yields the identity transformation.
    [[nodiscard]] Xform getInverse() const;

    //! Returns the transformation resulting from combining this transformation
    //! with an inner transformation
    //!
    //! \param   inner Inner transformation
    //!
    //! \return  transformation resulting from combining this transformation with
    //!          the inner transformation
    [[nodiscard]] Xform applyTo(const Xform& inner) const;

    //! Applies transformation to point
    //!
    //! \param   pt  Point to transform
    //! \return      Transformed point
    [[nodiscard]] Point2d transformPoint(const Point2d& pt) const;

    //! Applies transformation to point
    //!
    //! \param   pt  Point to transform
    //! \return      Transformed point
    template <Scalar U>
    Point2dT<U> transformPoint(const Point2dT<U>& pt) const;

    //! Applies reverse transformation to point
    //!
    //! \param   pt  Point to transform
    //! \return      Transformed point
    [[nodiscard]] Point2d reverseTransformPoint(const Point2d& pt) const;

    //! Applies reverse transformation to point
    //!
    //! \param   pt  Point to transform
    //! \return      Transformed point
    template <Scalar U>
    Point2dT<U> reverseTransformPoint(const Point2dT<U>& pt) const;

    //! Applies transformation to vertex array
    //!
    //! \param   vertices    [in,out] Points to transform
    template <class Container>
    void transformVertices(Container& vertices) const;

    //! Applies transformation to vertex array
    //!
    //! \param   vertices    [in] Points to transform
    //! \param   result      [out] Transformed points (array will be resized if necessary)
    template <class ContainerIn, class ContainerOut>
    void transformVertices(const ContainerIn& vertices, ContainerOut& result) const;

    //! Applies transformation to vector
    //!
    //! \note The transformation's translation component is ignored
    //!
    //! \param   v   Vector to transform
    //! \return      Transformed vector
    [[nodiscard]] Vector transformVector(const Vector& v) const;

    //! Applies transformation to distance
    //!
    //! \note Only the transformation's scaling factor is used
    //!
    //! \param   d   Distance to transform
    //! \return      Transformed distance
    [[nodiscard]] double transformDistance(double d) const;

    //! Applies transformation to distance
    //!
    //! \note Only the transformation's scaling factor is used
    //!
    //! \param   d   Distance to transform
    //! \return      Transformed distance
    template <Scalar U>
    U transformDistance(U d) const;

    //! Applies reverse transformation to distance
    //!
    //! \note Only the transformation's scaling factor is used
    //!
    //! \param   d   Distance to transform
    //! \return      Transformed distance
    [[nodiscard]] double reverseTransformDistance(double d) const;

    //! Applies reverse transformation to distance
    //!
    //! \note Only the transformation's scaling factor is used
    //!
    //! \param   d   Distance to transform
    //! \return      Transformed distance
    template <Scalar U>
    U reverseTransformDistance(U d) const;

    //! Applies transformation to angle
    //!
    //! \note Only the transformation's rotation component is used
    //!
    //! \param   angleCcw  Angle (counter-clockwise from positive x-axis) to transform
    //! \return          Transformed angle
    [[nodiscard]] Angle transformAngle(Angle angleCcw) const;

    //! Applies reverse transformation to angle
    //!
    //! \note Only the transformation's rotation component is used
    //!
    //! \param   angleCcw  Angle (counter-clockwise from positive x-axis) to transform
    //! \return          Transformed angle
    [[nodiscard]] Angle reverseTransformAngle(Angle angleCcw) const;

    //! Applies transformation to bounds
    //!
    //! \warning The returned bounds are the axis-aligned bounding box of the
    //! transformed input bounds; i.e. it's surface is in general larger than the
    //! input bounds' surface.
    //!
    //! \param   bound   Bounds to transform
    //! \return          Transformed bounds
    template <bool mayBeEmpty = true>
    Bounds<T, mayBeEmpty> transformBounds(const Bounds<T, mayBeEmpty>& bound) const;

    //! Applies approximate transformation to bounds
    //!
    //! \warning The returned bounds are the axis-aligned bounding box of the
    //! transformed input bounds; i.e. it's surface is in general larger than the
    //! input bounds' surface.
    //!
    //! \param   bound   Bounds to transform
    //! \return          Transformed bounds
    template <bool mayBeEmpty = true>
    Bounds<T, mayBeEmpty> transformBoundsFast(const Bounds<T, mayBeEmpty>& bound) const;

    //! Canonicalize transformation
    //!
    //! Expresses x-mirroring in terms of rotation and y-mirroring
    void canonicalize();

    //! Compares for equality
    //!
    //! \param   rhs Transformation to compare to
    //!
    //! \return  true if the transformations are equivalent.
    bool operator==(const Xform& rhs) const;

    //! Compares for inequality
    //!
    //! \param   rhs Transformation to compare to
    //!
    //! \return  true if the transformations are equivalent.
    bool operator!=(const Xform& rhs) const;

private:
    Vector offset_;  // offset
    double scaling_;  // scaling factor
    Angle rotation_;  // rotation angle
    double rotSin_;  // sine of rotation
    double rotCos_;  // cosine of rotation
    bool mirroredInX_;  // flip about y-axis
    bool mirroredInY_;  // flip about x-axis
    bool isScaled_;  // true if scaling factor not 1.0
    bool isRotated_;  // true if rotation angle not 0 deg
    bool absRot_;  // true: maintain rotation through outer xforms
    bool absScale_;  // true: maintain scaling through outer xforms
    bool ignoreAbs_;
};

template <typename T>
const Xform<T> Xform<T>::identity(false);  // the static identity matrix
template <typename T>
const Xform<T> Xform<T>::identityRel(true);  // the static identity matrix

//------------------------------------------------------------------------------
template <typename T>
Xform<T>::Xform(bool ignoreAbsXform /* = false */)
    : offset_(0, 0)
    , scaling_(1.0)
    , rotation_(Angle::zero)
    , rotSin_(0.0)
    , rotCos_(1.0)
    , mirroredInX_(false)
    , mirroredInY_(false)
    , isScaled_(false)
    , isRotated_(false)
    , absRot_(false)
    , absScale_(false)
    , ignoreAbs_(ignoreAbsXform)
{
    static_assert(sizeof(bool) == 1, "sizeof(bool) not 1");
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>::Xform(double scale, Angle rotationCcw, const Vector& offset)
    : offset_(offset)
    , scaling_(scale)
    , rotation_(Angle::zero)
    , rotSin_(0.0)
    , rotCos_(1.0)
    , mirroredInX_(false)
    , mirroredInY_(false)
    , isScaled_(scale != 1.0)
    , isRotated_(!rotationCcw.equals(Angle::zero))
    , absRot_(false)
    , absScale_(false)
    , ignoreAbs_(false)
{
    setRotation(rotationCcw);
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>::Xform(const Vector& offset, bool ignoreAbsXform /*= false */)
    : offset_(offset)
    , scaling_(1.0)
    , rotation_(Angle::zero)
    , rotSin_(0.0)
    , rotCos_(1.0)
    , mirroredInX_(false)
    , mirroredInY_(false)
    , isScaled_(false)
    , isRotated_(false)
    , absRot_(false)
    , absScale_(false)
    , ignoreAbs_(ignoreAbsXform)
{}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>::Xform(const Point& offset, bool ignoreAbsXform /* = false */)
    : offset_(Vector(offset))
    , scaling_(1.0)
    , rotation_(Angle::zero)
    , rotSin_(0.0)
    , rotCos_(1.0)
    , mirroredInX_(false)
    , mirroredInY_(false)
    , isScaled_(false)
    , isRotated_(false)
    , absRot_(false)
    , absScale_(false)
    , ignoreAbs_(ignoreAbsXform)
{}


//------------------------------------------------------------------------------
template <typename T>
Xform<T>& Xform<T>::setIgnoreAbsTransformation(bool doIgnore /* = true */)
{
    ignoreAbs_ = doIgnore;

    if (absRot_ && !ignoreAbs_)
    {
        isRotated_ = true;
    }
    else if (rotation_.equals(Angle::zero))
    {
        isRotated_ = false;
    }

    if (absScale_ && !ignoreAbs_)
    {
        isScaled_ = true;
    }
    else if (scaling_ == 1.0)
    {
        isScaled_ = false;
    }

    return *this;
}

//------------------------------------------------------------------------------
template <typename T>
bool Xform<T>::isAbsTransformationIgnored() const
{
    return ignoreAbs_;
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>& Xform<T>::setScaling(double scale, bool isAbsoluteScaling /*= false*/)
{
    scaling_ = scale;
    absScale_ = isAbsoluteScaling && !ignoreAbs_;
    isScaled_ = (scaling_ != 1.0) || absScale_;
    return *this;
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>& Xform<T>::setRotation(Angle rotationCcw, bool isAbsoluteRotation /*= false*/)
{
    rotation_ = normalize(rotationCcw);
    absRot_ = isAbsoluteRotation && !ignoreAbs_;
    isRotated_ = (rotation_ != Angle::zero) || (absRot_ && !ignoreAbs_);
    rotSin_ = sin(rotation_);
    rotCos_ = cos(rotation_);
    return *this;
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>& Xform<T>::setTranslation(const Vector& offset)
{
    offset_ = offset;
    return *this;
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>& Xform<T>::setMirroredInX(bool mirror /* = true */)
{
    mirroredInX_ = mirror;
    return *this;
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>& Xform<T>::setMirroredInY(bool mirror /* = true */)
{
    mirroredInY_ = mirror;
    return *this;
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>& Xform<T>::setRotationAbsolute(bool absolute /* = true */)
{
    absRot_ = absolute;

    if (absRot_ && !ignoreAbs_)
    {
        isRotated_ = true;
    }
    else if (rotation_.equals(Angle::zero))
    {
        isRotated_ = false;
    }
    return *this;
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>& Xform<T>::setScalingAbsolute(bool absolute /* = true */)
{
    absScale_ = absolute;

    if (absScale_ && !ignoreAbs_)
    {
        isScaled_ = true;
    }
    else if (scaling_ == 1.0)
    {
        isScaled_ = false;
    }
    return *this;
}

//------------------------------------------------------------------------------
template <typename T>
double Xform<T>::scaling() const
{
    return scaling_;
}

//------------------------------------------------------------------------------
template <typename T>
Angle Xform<T>::rotation() const
{
    return rotation_;
}

//------------------------------------------------------------------------------
template <typename T>
typename Xform<T>::Vector Xform<T>::translation() const
{
    return offset_;
}

//------------------------------------------------------------------------------
template <typename T>
bool Xform<T>::isMirroredInX() const
{
    return mirroredInX_;
}

//------------------------------------------------------------------------------
template <typename T>
bool Xform<T>::isMirroredInY() const
{
    return mirroredInY_;
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T> Xform<T>::getInverse() const
{
    Xform inverse;
    inverse.translate(-translation());
    inverse.rotate(-rotation());
    inverse.scale(1.0 / scaling());
    inverse.mirrorInX(isMirroredInX());
    inverse.mirrorInY(isMirroredInY());
    return inverse;
}

//------------------------------------------------------------------------------
template <typename T>
bool Xform<T>::isRelative() const
{
    return ignoreAbs_ || (!absRot_ && !absScale_);
}

//------------------------------------------------------------------------------
template <typename T>
bool Xform<T>::isRotationAbsolute() const
{
    return absRot_ && !ignoreAbs_;
}

//------------------------------------------------------------------------------
template <typename T>
bool Xform<T>::isScalingAbsolute() const
{
    return absScale_ && !ignoreAbs_;
}

//------------------------------------------------------------------------------
template <typename T>
bool Xform<T>::isRotated() const
{
    return isRotated_;
}

//------------------------------------------------------------------------------
template <typename T>
bool Xform<T>::isScaled() const
{
    return isScaled_;
}

//------------------------------------------------------------------------------
template <typename T>
bool Xform<T>::isIdentity() const
{
    return *this == identity;
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>& Xform<T>::scale(double scale, bool isAbsoluteScaling)
{
    util::Round<dist> round;
    absScale_ |= isAbsoluteScaling;

    if (scale == 1.0)
        return (*this);

    if (offset_.x != 0)
    {
        offset_.x = round.nearest(offset_.x * scale);
    }

    if (offset_.y != 0)
    {
        offset_.y = round.nearest(offset_.y * scale);
    }

    if (scale > 0.0)
    {
        // positive scaling
        scaling_ *= scale;
    }
    else if (scale < 0.0)
    {
        // negative scaling: reduce to positive scaling + 180 deg rotation
        setRotation(rotation_ + Angle::pi);
        scaling_ *= -scale;
    }
    else
    {
        assert(false);
        UNREACHABLE;  // entered zero scaling factor
    }

    isScaled_ = (scaling_ != 1.0) || (absScale_ && !ignoreAbs_);

    return (*this);
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>& Xform<T>::mirrorInY(bool doMirror /* = true */)
{
    if (!doMirror)
        return (*this);

    offset_.y = -offset_.y;

    // if the transformation contains a rotation, we also have to mirror it.
    if (rotation_ > Angle::zero)
    {
        setRotation(-rotation_);
    }

    mirroredInY_ = !mirroredInY_;
    return (*this);
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>& Xform<T>::mirrorInX(bool doMirror /* = true */)
{
    if (!doMirror)
        return (*this);

    offset_.x = -offset_.x;

    // if the transformation contains a rotation, we also have to mirror it.
    if (rotation_ > Angle::zero)
    {
        setRotation(-rotation_);
    }

    mirroredInX_ = !mirroredInX_;
    return (*this);
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>& Xform<T>::rotate(Angle rotationCcw, bool isAbsoluteRotation)
{
    if (!offset_.isNull())
    {
        offset_ = Xform().rotate(rotationCcw).transformVector(offset_);
    }

    setRotation(rotation_ + rotationCcw, isAbsoluteRotation);
    return (*this);
}

//------------------------------------------------------------------------------
template <typename T>
Xform<T>& Xform<T>::translate(const Vector& off)
{
    offset_ += off;
    return (*this);
}

//------------------------------------------------------------------------------
//  Apply this transformation to an inner transformation
//
//  Returns:  combined transformation
//------------------------------------------------------------------------------
template <typename T>
Xform<T> Xform<T>::applyTo(const Xform& inner) const
{
    if (isIdentity())
        return Xform(inner).setIgnoreAbsTransformation(ignoreAbs_);
    else if (inner.isIdentity())
        return *this;

    Xform res;
    res.setIgnoreAbsTransformation(ignoreAbs_);
    const Xform& outer = *this;  // makes code more understandable...

    // combine scaling
    if (outer.isScaled_ || inner.isScaled_)
    {
        if (inner.absScale_ == false || ignoreAbs_)
        {
            // combine scaling ("absoluteness" depends on outer xform)
            res.setScaling(outer.scaling_ * inner.scaling_, outer.absScale_ && !ignoreAbs_);
        }
        else
        {
            res.setScaling(inner.scaling_, true);
        }
    }

    // combine reflections
    res.setMirroredInX(outer.mirroredInX_ ^ inner.mirroredInX_);
    res.setMirroredInY(outer.mirroredInY_ ^ inner.mirroredInY_);

    // combine rotations
    if (outer.isRotated_ || inner.isRotated_)
    {
        Angle rotation = inner.rotation_;

        if (outer.mirroredInX_)
        {
            rotation = Angle::twoPi - rotation;
        }

        if (outer.mirroredInY_)
        {
            rotation = Angle::twoPi - rotation;
        }

        if (!inner.absRot_ || ignoreAbs_)
        {
            // combine and retain outer absoluteness
            res.setRotation(outer.rotation_ + rotation, outer.absRot_);
        }
        else
        {
            res.setRotation(rotation, true);  // ignore outer absoluteness
        }
    }

    // combine offsets
    res.setTranslation(Vector(outer.transformPoint(Point(inner.offset_))));
    return res;
}

// -----------------------------------------------------------------------------
//  Transforms the specified point according to this Xform's transformation
//  matrix, and returns the transformed coordinate pair.
//
//  transform:   1. scale & flip; 2. rotate; 3. offset;
// -----------------------------------------------------------------------------
template <typename T>
Point2d Xform<T>::transformPoint(const Point2d& pt) const
{
    // mirror
    Point2d result(mirroredInX_ ? -pt.x : pt.x, mirroredInY_ ? -pt.y : pt.y);

    // scale
    if (isScaled_)
    {
        result *= scaling_;
    }

    // rotate
    if (isRotated_)
    {
        if (rotation_.equals(Angle::piHalf))  // 90deg CCW: -y -> x'  / x -> y'
        {
            result = {-result.y, +result.x};
        }
        else if (rotation_.equals(Angle::pi))  // 180deg CCW: -x -> x' / -y -> y'
        {
            result = {-result.x, -result.y};
        }
        else if (rotation_.equals(Angle::threePiHalf))  // 270deg CCW: y -> x'  / -x -> y'
        {
            result = {+result.y, -result.x};
        }
        else
        {
            result = Point2d(result.x * rotCos_ - result.y * rotSin_,
                             result.x * rotSin_ + result.y * rotCos_);
        }
    }

    // offset
    return result + static_cast<Vector2d>(offset_);
}


// -----------------------------------------------------------------------------
//  Transforms the specified point according to this Xform's transformation
//  matrix, and returns the transformed coordinate pair.
//
//  transform:   1. scale & flip; 2. rotate; 3. offset;
// -----------------------------------------------------------------------------
template <typename T>
template <Scalar U>
Point2dT<U> Xform<T>::transformPoint(const Point2dT<U>& pt) const
{
    if (isScaled_)
    {
        return round<Point2dT<U>>(transformPoint(static_cast<Point2d>(pt)));
    }
    else
    {
        // mirror
        Point2dT<U> result(mirroredInX_ ? -pt.x : pt.x, mirroredInY_ ? -pt.y : pt.y);

        // rotate & offset
        if (!isRotated_)  //  0deg CCW: x -> x'   /  y -> y'
        {
            return result + offset_;
        }
        else if (rotation_.equals(Angle::piHalf))  // 90deg CCW: -y -> x'  / x -> y'
        {
            return Point(-result.y, +result.x) + offset_;
        }
        else if (rotation_.equals(Angle::pi))  // 180deg CCW: -x -> x' / -y -> y'
        {
            return Point(-result.x, -result.y) + offset_;
        }
        else if (rotation_.equals(Angle::threePiHalf))  // 270deg CCW: y -> x'  / -x -> y'
        {
            return Point(+result.y, -result.x) + offset_;
        }
        else
        {
            return round<Point2dT<U>>(Point2d(result.x * rotCos_ - result.y * rotSin_,
                                              result.x * rotSin_ + result.y * rotCos_) +
                                      static_cast<Vector2d>(offset_));
        }
    }
}

//------------------------------------------------------------------------------
// Reverse transform a point
//
// reverse transform: 1. offset; 2. rotate; 3. scale & flip
//------------------------------------------------------------------------------
template <typename T>
Point2d Xform<T>::reverseTransformPoint(const Point2d& pt) const
{
    auto result = pt - static_cast<Vector2d>(offset_);

    if (isScaled_)
    {
        result /= scaling_;
    }

    if (isRotated_)
    {
        if (rotation_.equals(Angle::piHalf))  // 90deg CCW:  x <-  y' / -y <- x'
        {
            result = {+result.y, -result.x};
        }
        else if (rotation_.equals(Angle::pi))  // 180deg CCW: -x <- x' / -y <- y'
        {
            result = {-result.x, -result.y};
        }
        else if (rotation_.equals(Angle::threePiHalf))  // 270deg CCW: -x <- y' /  y <- x'
        {
            result = {-result.y, +result.x};
        }
        else
        {
            result = Point2d(+result.x * rotCos_ + result.y * rotSin_,
                             -result.x * rotSin_ + result.y * rotCos_);
        }
    }

    if (mirroredInX_)
    {
        result.x = -result.x;
    }

    if (mirroredInY_)
    {
        result.y = -result.y;
    }

    return result;
}

//------------------------------------------------------------------------------
// Reverse transform a point
//
// reverse transform: 1. offset; 2. rotate; 3. scale & flip
//------------------------------------------------------------------------------
template <typename T>
template <Scalar U>
Point2dT<U> Xform<T>::reverseTransformPoint(const Point2dT<U>& pt) const
{
    Point2dT<U> result;

    if (isScaled_)
    {
        return round<Point2dT<U>>(static_cast<Point2d>(pt));
    }
    else
    {
        result = pt - offset_;

        if (isRotated_)
        {
            if (rotation_.equals(Angle::piHalf))  // 90deg CCW:  x <-  y' / -y <- x'
            {
                result = Point(+result.y, -result.x);
            }
            else if (rotation_.equals(Angle::pi))  // 180deg CCW: -x <- x' / -y <- y'
            {
                result = Point(-result.x, -result.y);
            }
            else if (rotation_.equals(Angle::threePiHalf))  // 270deg CCW: -x <- y' /  y <- x'
            {
                result = Point(-result.y, +result.x);
            }
            else
            {
                result = round<Point2dT<U>>(Point2d(+result.x * rotCos_ + result.y * rotSin_,
                                                    -result.x * rotSin_ + result.y * rotCos_));
            }
        }
    }

    if (mirroredInX_)
    {
        result.x = -result.x;
    }

    if (mirroredInY_)
    {
        result.y = -result.y;
    }

    return result;
}

//------------------------------------------------------------------------------
template <typename T>
template <class Container>
void Xform<T>::transformVertices(Container& vertices) const
{
    // mirror
    if (mirroredInX_)
    {
        for (auto& v : vertices)
        {
            v.x = -v.x;
        }
    }

    if (mirroredInY_)
    {
        for (auto& v : vertices)
        {
            v.y = -v.y;
        }
    }

    // scale
    if (isScaled_)
    {
        for (auto& v : vertices)
        {
            using PointT = typename std::remove_reference<decltype(v)>::type;
            v = static_cast<PointT>(static_cast<Point2d>(v) * scaling_);
        }
    }

    // rotate
    if (isRotated_)
    {
        if (rotation_.equals(Angle::piHalf))  // 90deg CCW: -y -> x'  / x -> y'
        {
            for (auto& v : vertices)
            {
                auto tmp = v.x;
                v.x = -v.y;
                v.y = tmp;
            }
        }
        else if (rotation_.equals(Angle::pi))  // 180deg CCW: -x -> x' / -y -> y'
        {
            for (auto& v : vertices)
            {
                v.x = -v.x;
                v.y = -v.y;
            }
        }
        else if (rotation_.equals(Angle::threePiHalf))  // 270deg CCW: y -> x'  / -x -> y'
        {
            for (auto& v : vertices)
            {
                auto tmp = v.x;
                v.x = v.y;
                v.y = -tmp;
            }
        }
        else
        {
            for (auto& v : vertices)
            {
                using CoordT = typename std::remove_reference<decltype(v.x)>::type;

                auto tmp = v.x * rotSin_ + v.y * rotCos_;
                v.x = static_cast<CoordT>(v.x * rotCos_ - v.y * rotSin_);
                v.y = static_cast<CoordT>(tmp);
            }
        }
    }

    // offset
    if (!offset_.isNull())
    {
        auto offset = static_cast<typename Container::value_type::VectorT>(offset_);
        for (auto& v : vertices)
        {
            v += offset;
        }
    }
}


//------------------------------------------------------------------------------
template <typename T>
template <class ContainerIn, class ContainerOut>
void Xform<T>::transformVertices(const ContainerIn& vertices, ContainerOut& result) const
{
    result.resize(vertices.size());
    for (size_t i = 0, cnt = vertices.size(); i < cnt; ++i)
    {
        result[i] = static_cast<typename ContainerOut::value_type>(vertices[i]);
    }

    transformVertices(result);
}

// -----------------------------------------------------------------------------
//  Transforms the specified vector according to this Xform's transformation
//  matrix, and returns the transformed coordinate pair.
//
//  transform:   1. scale & flip; 2. rotate;
// -----------------------------------------------------------------------------
template <typename T>
typename Xform<T>::Vector Xform<T>::transformVector(const Vector& vec) const
{
    if (isScaled_)
    {
        // mirror
        Vector2d result(static_cast<double>(mirroredInX_ ? -vec.x : vec.x),
                        static_cast<double>(mirroredInY_ ? -vec.y : vec.y));

        // scale
        result *= scaling_;

        // rotate
        if (isRotated_)
        {
            if (rotation_.equals(Angle::piHalf))  // 90deg CCW: -y -> x'  / x -> y'
            {
                result = Vector2d(-result.y, +result.x);
            }
            else if (rotation_.equals(Angle::pi))  // 180deg CCW: -x -> x' / -y -> y'
            {
                result = Vector2d(-result.x, -result.y);
            }
            else if (rotation_.equals(Angle::threePiHalf))  // 270deg CCW: y -> x'  / -x -> y'
            {
                result = Vector2d(+result.y, -result.x);
            }
            else
            {
                result = Vector2d(result.x * rotCos_ - result.y * rotSin_,
                                  result.x * rotSin_ + result.y * rotCos_);
            }
        }

        // round
        return round<Vector>(result);
    }
    else
    {
        // mirror
        Vector result(mirroredInX_ ? -vec.x : vec.x, mirroredInY_ ? -vec.y : vec.y);

        // rotate & offset
        if (!isRotated_)  //  0deg CCW: x -> x'   /  y -> y'
        {
            return result;
        }
        else if (rotation_.equals(Angle::piHalf))  // 90deg CCW: -y -> x'  / x -> y'
        {
            return Vector(-result.y, +result.x);
        }
        else if (rotation_.equals(Angle::pi))  // 180deg CCW: -x -> x' / -y -> y'
        {
            return Vector(-result.x, -result.y);
        }
        else if (rotation_.equals(Angle::threePiHalf))  // 270deg CCW: y -> x'  / -x -> y'
        {
            return Vector(+result.y, -result.x);
        }
        else
        {
            return round<Vector>(Vector2d(result.x * rotCos_ - result.y * rotSin_,
                                          result.x * rotSin_ + result.y * rotCos_));
        }
    }
}

// -----------------------------------------------------------------------------
//  Transforms the specified distance according to this transformation matrix.
// -----------------------------------------------------------------------------
template <typename T>
double Xform<T>::transformDistance(double d) const
{
    return isScaled_ ? d * scaling_ : d;
}

template <typename T>
template <Scalar U>
U Xform<T>::transformDistance(U d) const
{
    return util::round<U>(transformDistance(static_cast<double>(d)));
}

// -----------------------------------------------------------------------------
//  Reverse transform a distance
// -----------------------------------------------------------------------------
template <typename T>
double Xform<T>::reverseTransformDistance(double d) const
{
    return isScaled_ ? d / scaling_ : d;
}

template <typename T>
template <Scalar U>
U Xform<T>::reverseTransformDistance(U d) const
{
    return util::round<U>(reverseTransformDistance(static_cast<double>(d)));
}

//------------------------------------------------------------------------------
template <typename T>
Angle Xform<T>::transformAngle(Angle angleCcw) const
{
    if (mirroredInX_)
    {
        angleCcw = Angle::pi - angleCcw;
    }

    if (mirroredInY_)
    {
        angleCcw = -angleCcw;
    }

    if (isRotated_)
    {
        angleCcw += rotation_;
    }

    return angleCcw;
}

//------------------------------------------------------------------------------
template <typename T>
Angle Xform<T>::reverseTransformAngle(Angle angleCcw) const
{
    if (isRotated_)
    {
        angleCcw -= rotation_;
    }

    if (mirroredInX_)
    {
        angleCcw = Angle::pi - angleCcw;
    }

    if (mirroredInY_)
    {
        angleCcw = -angleCcw;
    }

    return angleCcw;
}

//! Transforms the specified Bounds boundary according to this Xform's
//! transformation matrix.
//!
//! \note    Internal calculations are done using 64-bit integer arithmetic, as
//!          intermediate results may exceed 32-bit boundaries. Also, floating
//!          point arithmetic is used if scaling factors are involved.
//!
//! \note    The returned bound is the axis-aligned bounding box of the
//!          transformed input bound; i.e. it's surface is in general larger than
//!          the input bound's surface.
//!
//! \note    Note also that
//!          `xform.getInverse().transformBounds(xform.transformBounds(bnd)) != bnd`
//!          unless 'bnd' is axis-aligned.
//!
//! \param   bound   Input boundary
//! \return  Transformed boundary
template <typename T>
template <bool mayBeEmpty>
Bounds<T, mayBeEmpty> Xform<T>::transformBounds(const Bounds<T, mayBeEmpty>& bound) const
{
    if (bound.empty())
        return bound;

    int_least64_t x0 = bound.minX();
    int_least64_t y0 = bound.minY();
    int_least64_t x1 = bound.maxX();
    int_least64_t y1 = bound.maxY();

    if (mirroredInX_)
    {
        std::swap(x0, x1);
        x0 = -x0;
        x1 = -x1;
    }

    if (mirroredInY_)
    {
        std::swap(y0, y1);
        y0 = -y0;
        y1 = -y1;
    }

    if (isScaled_)
    {
        util::Round<int_least64_t> round;
        x0 = round.nearest(scaling_ * x0);
        y0 = round.nearest(scaling_ * y0);
        x1 = round.nearest(scaling_ * x1);
        y1 = round.nearest(scaling_ * y1);
    }

    // assert the entry conditions
    assert(x0 <= x1);
    assert(y0 <= y1);

    if (!isRotated_)
    {
        assert(offset_.x + x0 > std::numeric_limits<dist>::lowest() &&
               offset_.x + x0 < std::numeric_limits<dist>::max());
        assert(offset_.y + y0 > std::numeric_limits<dist>::lowest() &&
               offset_.y + y0 < std::numeric_limits<dist>::max());
        assert(offset_.x + x1 > std::numeric_limits<dist>::lowest() &&
               offset_.x + x1 < std::numeric_limits<dist>::max());
        assert(offset_.y + y1 > std::numeric_limits<dist>::lowest() &&
               offset_.y + y1 < std::numeric_limits<dist>::max());

        return {T(offset_.x + x0), T(offset_.y + y0), T(offset_.x + x1), T(offset_.y + y1)};
    }
    else if (rotation_.equals(Angle::piHalf))
    {
        assert(offset_.x - y1 > std::numeric_limits<dist>::lowest() &&
               offset_.x - y1 < std::numeric_limits<dist>::max());
        assert(offset_.y + x0 > std::numeric_limits<dist>::lowest() &&
               offset_.y + x0 < std::numeric_limits<dist>::max());
        assert(offset_.x - y0 > std::numeric_limits<dist>::lowest() &&
               offset_.x - y0 < std::numeric_limits<dist>::max());
        assert(offset_.y + x1 > std::numeric_limits<dist>::lowest() &&
               offset_.y + x1 < std::numeric_limits<dist>::max());

        return {T(offset_.x - y1), T(offset_.y + x0), T(offset_.x - y0), T(offset_.y + x1)};
    }
    else if (rotation_.equals(Angle::pi))
    {
        return {T(offset_.x - x1), T(offset_.y - y1), T(offset_.x - x0), T(offset_.y - y0)};
    }
    else if (rotation_.equals(Angle::threePiHalf))
    {
        return {T(offset_.x + y0), T(offset_.y - x1), T(offset_.x + y1), T(offset_.y - x0)};
    }
    else
    {
        util::Round<dist> round;
        if (rotation_ < Angle::piHalf)
        {
            return {round.nearest(offset_.x + x0 * rotCos_ - y1 * rotSin_),  // x0':(x0,y1)
                    round.nearest(offset_.y + x0 * rotSin_ + y0 * rotCos_),  // y0':(x0,y0)
                    round.nearest(offset_.x + x1 * rotCos_ - y0 * rotSin_),  // x1':(x1,y0)
                    round.nearest(offset_.y + x1 * rotSin_ + y1 * rotCos_)};  // y1':(x1,y1)
        }
        else if (rotation_ < Angle::pi)
        {
            return {round.nearest(offset_.x + x1 * rotCos_ - y1 * rotSin_),  // x0':(x1,y1)
                    round.nearest(offset_.y + x0 * rotSin_ + y1 * rotCos_),  // y0':(x0,y1)
                    round.nearest(offset_.x + x0 * rotCos_ - y0 * rotSin_),  // x1':(x0,y0)
                    round.nearest(offset_.y + x1 * rotSin_ + y0 * rotCos_)};  // y1':(x1,y0)
        }
        else if (rotation_ < Angle::threePiHalf)
        {
            return {round.nearest(offset_.x + x1 * rotCos_ - y0 * rotSin_),  // x0':(x1,y0)
                    round.nearest(offset_.y + x1 * rotSin_ + y1 * rotCos_),  // y0':(x1,y1)
                    round.nearest(offset_.x + x0 * rotCos_ - y1 * rotSin_),  // x1':(x0,y1)
                    round.nearest(offset_.y + x0 * rotSin_ + y0 * rotCos_)};  // y1':(x0,y0)
        }
        else
        {
            return {round.nearest(offset_.x + x0 * rotCos_ - y0 * rotSin_),  // x0':(x0,y0)
                    round.nearest(offset_.y + x1 * rotSin_ + y0 * rotCos_),  // y0':(x1,y0)
                    round.nearest(offset_.x + x1 * rotCos_ - y1 * rotSin_),  // x1':(x1,y1)
                    round.nearest(offset_.y + x0 * rotSin_ + y1 * rotCos_)};  // y1':(x0,y1)
        }
    }
}

//! Transforms the specified Bounds boundary according to this Xform's
//! transformation matrix.
//!
//! \note    Internal calculations are done using 64-bit integer arithmetic, as
//!          intermediate results may exceed 32-bit boundaries. Also, floating
//!          point arithmetic is used if scaling factors are involved.
//!
//! \note    The returned bound is the axis-aligned bounding box of the
//!          transformed input bound; i.e. it's surface is in general larger than
//!          the input bound's surface.
//!
//! \note    Note also that
//!          `xform.getInverse().transformBounds(xform.transformBounds(bnd)) != bnd`
//!          unless 'bnd' is axis-aligned.
//!
//! \param   bound   Input boundary
//! \return  Transformed boundary
template <typename T>
template <bool mayBeEmpty>
Bounds<T, mayBeEmpty> Xform<T>::transformBoundsFast(const Bounds<T, mayBeEmpty>& bound) const
{
    if (bound.empty())
        return bound;

    int_least64_t x0 = bound.minX();
    int_least64_t y0 = bound.minY();
    int_least64_t x1 = bound.maxX();
    int_least64_t y1 = bound.maxY();

    if (mirroredInX_)
    {
        std::swap(x0, x1);
        x0 = -x0;
        x1 = -x1;
    }

    if (mirroredInY_)
    {
        std::swap(y0, y1);
        y0 = -y0;
        y1 = -y1;
    }

    if (isScaled_)
    {
        x0 = util::fastCast<dist>(scaling_ * x0);
        y0 = util::fastCast<dist>(scaling_ * y0);
        x1 = util::fastCast<dist>(scaling_ * x1);
        y1 = util::fastCast<dist>(scaling_ * y1);
    }

    // assert the entry conditions
    assert(x0 <= x1);
    assert(y0 <= y1);

    if (!isRotated_)
    {
        assert(offset_.x + x0 > std::numeric_limits<dist>::lowest() &&
               offset_.x + x0 < std::numeric_limits<dist>::max());
        assert(offset_.y + y0 > std::numeric_limits<dist>::lowest() &&
               offset_.y + y0 < std::numeric_limits<dist>::max());
        assert(offset_.x + x1 > std::numeric_limits<dist>::lowest() &&
               offset_.x + x1 < std::numeric_limits<dist>::max());
        assert(offset_.y + y1 > std::numeric_limits<dist>::lowest() &&
               offset_.y + y1 < std::numeric_limits<dist>::max());

        return {T(offset_.x + x0), T(offset_.y + y0), T(offset_.x + x1), T(offset_.y + y1)};
    }
    else if (rotation_.equals(Angle::piHalf))
    {
        assert(offset_.x - y1 > std::numeric_limits<dist>::lowest() &&
               offset_.x - y1 < std::numeric_limits<dist>::max());
        assert(offset_.y + x0 > std::numeric_limits<dist>::lowest() &&
               offset_.y + x0 < std::numeric_limits<dist>::max());
        assert(offset_.x - y0 > std::numeric_limits<dist>::lowest() &&
               offset_.x - y0 < std::numeric_limits<dist>::max());
        assert(offset_.y + x1 > std::numeric_limits<dist>::lowest() &&
               offset_.y + x1 < std::numeric_limits<dist>::max());

        return {T(offset_.x - y1), T(offset_.y + x0), T(offset_.x - y0), T(offset_.y + x1)};
    }
    else if (rotation_.equals(Angle::pi))
    {
        return {T(offset_.x - x1), T(offset_.y - y1), T(offset_.x - x0), T(offset_.y - y0)};
    }
    else if (rotation_.equals(Angle::threePiHalf))
    {
        return {T(offset_.x + y0), T(offset_.y - x1), T(offset_.x + y1), T(offset_.y - x0)};
    }
    else
    {
        if (rotation_ < Angle::piHalf)
        {
            return {util::fastCast<dist>(offset_.x + x0 * rotCos_ - y1 * rotSin_),  // x0':(x0,y1)
                    util::fastCast<dist>(offset_.y + x0 * rotSin_ + y0 * rotCos_),  // y0':(x0,y0)
                    util::fastCast<dist>(offset_.x + x1 * rotCos_ - y0 * rotSin_),  // x1':(x1,y0)
                    util::fastCast<dist>(offset_.y + x1 * rotSin_ + y1 * rotCos_)};  // y1':(x1,y1)
        }
        else if (rotation_ < Angle::pi)
        {
            return {util::fastCast<dist>(offset_.x + x1 * rotCos_ - y1 * rotSin_),  // x0':(x1,y1)
                    util::fastCast<dist>(offset_.y + x0 * rotSin_ + y1 * rotCos_),  // y0':(x0,y1)
                    util::fastCast<dist>(offset_.x + x0 * rotCos_ - y0 * rotSin_),  // x1':(x0,y0)
                    util::fastCast<dist>(offset_.y + x1 * rotSin_ + y0 * rotCos_)};  // y1':(x1,y0)
        }
        else if (rotation_ < Angle::threePiHalf)
        {
            return {util::fastCast<dist>(offset_.x + x1 * rotCos_ - y0 * rotSin_),  // x0':(x1,y0)
                    util::fastCast<dist>(offset_.y + x1 * rotSin_ + y1 * rotCos_),  // y0':(x1,y1)
                    util::fastCast<dist>(offset_.x + x0 * rotCos_ - y1 * rotSin_),  // x1':(x0,y1)
                    util::fastCast<dist>(offset_.y + x0 * rotSin_ + y0 * rotCos_)};  // y1':(x0,y0)
        }
        else
        {
            return {util::fastCast<dist>(offset_.x + x0 * rotCos_ - y0 * rotSin_),  // x0':(x0,y0)
                    util::fastCast<dist>(offset_.y + x1 * rotSin_ + y0 * rotCos_),  // y0':(x1,y0)
                    util::fastCast<dist>(offset_.x + x1 * rotCos_ - y1 * rotSin_),  // x1':(x1,y1)
                    util::fastCast<dist>(offset_.y + x0 * rotSin_ + y1 * rotCos_)};  // y1':(x0,y1)
        }
    }
}


//------------------------------------------------------------------------------
template <typename T>
void Xform<T>::canonicalize()
{
    if (isMirroredInX())
    {
        setMirroredInX(false);
        setMirroredInY(!isMirroredInY());
        setRotation(rotation() + Angle::pi, isRotationAbsolute());
    }
}

//------------------------------------------------------------------------------
template <typename T>
bool Xform<T>::operator==(const Xform& rhs) const
{
    if (mirroredInX_ == rhs.mirroredInX_)
    {
        return offset_ == rhs.offset_ &&
               ::abs(scaling_ - rhs.scaling_) <= std::numeric_limits<double>::epsilon() &&
               rotation_.equals(rhs.rotation_) && mirroredInY_ == rhs.mirroredInY_ &&
               absRot_ == rhs.absRot_ && absScale_ == rhs.absScale_;
    }
    else
    {
        Xform l(*this);
        l.canonicalize();
        Xform r(rhs);
        r.canonicalize();
        return l == r;
    }
}

//------------------------------------------------------------------------------
template <typename T>
bool Xform<T>::operator!=(const Xform& rhs) const
{
    return !((*this) == rhs);
}

}  // namespace lc::geom
