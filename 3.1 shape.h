// Shape Declarations
class Shape {
  public:
    // Shape Interface
    Shape(const Transform *ObjectToWorld, const Transform *WorldToObject, bool reverseOrientation);
    virtual ~Shape();

    virtual Bounds3f ObjectBound() const = 0;
    virtual Bounds3f WorldBound() const;

    virtual bool Intersect(const Ray &ray, Float *tHit, SurfaceInteraction *isect, bool testAlphaTexture = true) const = 0;
    // checks for a ray–box intersection and returns the two parametric t values
    // of the intersection, if any.
    virtual bool IntersectP(const Ray &ray, bool testAlphaTexture = true) const {
        return Intersect(ray, nullptr, nullptr, testAlphaTexture);
    }
    // In order to properly use Shapes as area lights,
    // it is necessary to be able to compute the surface area of a shape in object space.
    virtual Float Area() const = 0;

    // Sample a point on the surface of the shape and return the PDF with
    // respect to area on the surface.
    virtual Interaction Sample(const Point2f &u, Float *pdf) const = 0;
    virtual Float Pdf(const Interaction &) const { return 1 / Area(); }

    // Sample a point on the shape given a reference point |ref| and
    // return the PDF with respect to solid angle from |ref|.
    virtual Interaction Sample(const Interaction &ref, const Point2f &u, Float *pdf) const;
    virtual Float Pdf(const Interaction &ref, const Vector3f &wi) const;

    // Returns the solid angle subtended by the shape w.r.t. the reference
    // point p, given in world space. Some shapes compute this value in
    // closed-form, while the default implementation uses Monte Carlo
    // integration; the nSamples parameter determines how many samples are
    // used in this case.
    virtual Float SolidAngle(const Point3f &p, int nSamples = 512) const;

    // Shape Public Data
    const Transform *ObjectToWorld, *WorldToObject;
    const bool reverseOrientation;
    const bool transformSwapsHandedness;
};
