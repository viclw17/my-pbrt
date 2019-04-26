This chapter defines a set of classes for **describing the way that light scatters at surfaces**.

Recall that in Section 5.6.1 we introduced
1. the BRDF to describe light **reflection** at a surface,
2. the BTDF to describe **transmission** at a surface, and
3. the BSDF to encompass both of these effects.

In this chapter, we will start by defining **a generic interface** to these surface reflection and transmission functions.

If a **SamplerIntegrator** is used, the SamplerIntegrator::Li() method implementation is called for each ray. After finding the closest intersection with a geometric primitive, it calls the **surface shader** that is associated with the primitive.

The surface shader is implemented as a method of **Material subclasses** and is responsible for deciding what the BSDF is at a particular point on the surface; it returns **a BSDF object that holds BRDFs and BTDFs** that it has allocated and initialized to represent scattering at that point.

The integrator then uses the BSDF to compute the scattered light at the point, based on the incoming illumination at the point.

---

## basic terminology for describing reflection from surfaces.
(1) diffuse,
(2) glossy specular,
(3) nearly-perfect specular, and
(4) retro-reflective distributions.
Given a particular category of reflection, the reflectance distribution function may be isotropic or anisotropic.

**reflection coordinate system**
shading coordinate system is defined by the orthonormal basis vectors (s,t,n)
two tangent vectors and the normal vector

We will provide utility functions to compute this value and some useful variations; their use helps clarify BRDF and BTDF implementations.

core/reflection.h
---

## There are important conventions and implementation details:

- The incident light direction wi and the outgoing viewing direction wo will both be **normalized** and **outward facing** after being transformed into the local coordinate system at the surface.
- By convention in pbrt, the **surface normal n always points to the “outside” of the object**, which makes it easy to determine if light is entering or exiting transmissive objects: if the incident light direction is in the same hemisphere as n, then light is entering; otherwise, it is exiting. Therefore, one detail to keep in mind is that the normal may be on the opposite side of the surface than one or both of the and wi direction vectors. Unlike many other renderers, pbrt does not flip the normal to lie on the same side as wo.
- The local coordinate system used for shading may **not** be exactly the same as the coordinate system returned by the Shape::Intersect() routines from Chapter 3; they can be modified between intersection and shading to achieve effects like **bump mapping**. See Chapter 9 for examples of this kind of modification.
- Finally, BRDF and BTDF implementations should not concern themselves with whether  and  lie in the same hemisphere. For example, although a reflective BRDF should in principle detect if the incident direction is above the surface and the outgoing direction is below and always return no reflection in this case, here we will expect the reflection function to instead compute and return the amount of light reflected using the appropriate formulas for their reflection model, ignoring the detail that they are not in the same hemisphere. Higher level code in pbrt will ensure that only reflective or transmissive scattering routines are evaluated as appropriate. The value of this convention will be explained in Section 9.1.

8.1 Basic interface

what about virtual base class? aka interface?

BRDFs and BTDFs share a common base class, BxDF. Because both have the exact same interface, sharing the same base class **reduces repeated code** and allows some parts of the system to work with BxDFs **generically** without distinguishing between BRDFs and BTDFs.
