#pragma once

struct GLVector;
struct GLRotator;
struct GLScale;
struct GLTransform;

class Object
{
public:
	/** Default Constructor for an Object */
	Object();

	/** Constructs an object at the position (x, y, z). 
	@param The x coordinate.
	@param The y coordinate.
	@param The z coordinate. 
	*/
	Object(float x, float y, float z);

	/** Default destructor for an object. */
	virtual ~Object();

	/** Ticks every millisecond when running. */
	virtual void Tick() = 0;

	/** Sets the object's location
	@param The new location for the object.
	*/
	virtual void SetLocation(GLVector location);

	/** Sets the object's location
	@param The x coordinate.
	@param The y coordinate.
	@param The z coordinate. 
	*/
	virtual void SetLocation(float x, float y, float z);

	/** Sets the rotation of the object.
	@param The new rotation of the object.
	*/
	virtual void SetRotation(GLRotator rotation);

	/** Sets the rotation of the object.
	@param The x rotation coordinate.
	@param The y rotation coordinate.
	@param The z rotation coordinate. 
	*/
	virtual void SetRotation(float x, float y, float z);

	/** Sets the object's scale.
	@param The object's new scale.
	*/
	virtual void SetScale(GLScale scale);

	/** Sets the object's scale.
	@param The x scale.
	@param The y scale.
	@param The z scale. 
	*/
	virtual void SetScale(float x, float y, float z);

	/** Translates the object ina  given direction.
	@param The direction and magnitude of translation.
	*/
	virtual void Translate(GLVector v);

	/** Translates the object in a given direction.
	@param The amount to be translated in the x direction.
	@param The amount to be translated in the y direction.
	@param The amount to be translated in the z direction.
	*/
	virtual void Translate(float x, float y, float z);

	/** Rotate the object.
	@param The axis and magnitude of rotation.
	*/
	virtual void Rotate(GLRotator r);

	/** Rotate the object.
	@param The angle of rotation from the current rotation.
	@param The first vector endpoint.
	@param The last vector endpoint.
	*/
	virtual void Rotate(float theta, GLVector v1, GLVector v2);

	/** Returns the object's current location. */
	GLVector GetLocation();

	/** Returns the object's current rotation. */
	GLRotator GetRotation();

	/** Returns the object's current scale. */
	GLScale GetScale();

	/** Returns the object's current transform. */
	GLTransform GetTransform();

protected:
	/** Helper function which holds the logic for setting up the object's model. */
	virtual void StartRender();

	/** Closes of the object model. */
	virtual void EndRender();

protected:
	GLTransform* _transform;
	float _angle;
	

};
